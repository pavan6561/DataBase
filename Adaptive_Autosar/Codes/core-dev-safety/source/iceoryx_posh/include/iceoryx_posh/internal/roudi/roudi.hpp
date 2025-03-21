// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0
#ifndef IOX_POSH_ROUDI_ROUDI_MULTI_PROCESS_HPP
#define IOX_POSH_ROUDI_ROUDI_MULTI_PROCESS_HPP

#include "iceoryx_hoofs/cxx/generic_raii.hpp"
#include "iceoryx_hoofs/internal/concurrent/smart_lock.hpp"
#include "iceoryx_hoofs/internal/relocatable_pointer/relative_pointer.hpp"
#include "iceoryx_hoofs/platform/file.hpp"
#include "iceoryx_hoofs/posix_wrapper/posix_access_rights.hpp"
#include "iceoryx_posh/iceoryx_posh_types.hpp"
#include "iceoryx_posh/internal/capro/capro_message.hpp"
#include "iceoryx_posh/internal/roudi/introspection/mempool_introspection.hpp"
#include "iceoryx_posh/internal/roudi/process_manager.hpp"
#include "iceoryx_posh/internal/runtime/ipc_interface_creator.hpp"
#include "iceoryx_posh/internal/runtime/ipc_message_types.hpp"
#include "iceoryx_posh/mepoo/mepoo_config.hpp"
#include "iceoryx_posh/roudi/memory/roudi_memory_interface.hpp"
#include "iceoryx_posh/roudi/memory/roudi_memory_manager.hpp"
#include "iceoryx_posh/roudi/roudi_app.hpp"

#include <cstdint>
#include <cstdio>
#include <thread>

namespace iox
{
namespace roudi
{
using namespace iox::units::duration_literals;

class RouDi
{
  public:
    /// @brief Indicate whether the thread processing messages from the runtimes will start directly or deferred
    /// this is important for derived classes which may need to initialize their members before the thread starts
    enum class RuntimeMessagesThreadStart
    {
        IMMEDIATE,
        DEFER_START
    };

    struct RoudiStartupParameters
    {
        RoudiStartupParameters(
            const roudi::MonitoringMode monitoringMode = roudi::MonitoringMode::ON,
            const bool killProcessesInDestructor = true,
            const RuntimeMessagesThreadStart RuntimeMessagesThreadStart = RuntimeMessagesThreadStart::IMMEDIATE,
            const version::CompatibilityCheckLevel compatibilityCheckLevel = version::CompatibilityCheckLevel::PATCH,
            const units::Duration processKillDelay = roudi::PROCESS_DEFAULT_KILL_DELAY) noexcept
            : m_monitoringMode(monitoringMode)
            , m_killProcessesInDestructor(killProcessesInDestructor)
            , m_runtimesMessagesThreadStart(RuntimeMessagesThreadStart)
            , m_compatibilityCheckLevel(compatibilityCheckLevel)
            , m_processKillDelay(processKillDelay)
        {
        }

        const roudi::MonitoringMode m_monitoringMode;
        const bool m_killProcessesInDestructor;
        const RuntimeMessagesThreadStart m_runtimesMessagesThreadStart;
        const version::CompatibilityCheckLevel m_compatibilityCheckLevel;
        const units::Duration m_processKillDelay;
    };

    RouDi& operator=(const RouDi& other) = delete;
    RouDi(const RouDi& other) = delete;

    RouDi(RouDiMemoryInterface& roudiMemoryInterface,
          PortManager& portManager,
          RoudiStartupParameters roudiStartupParameters) noexcept;

    virtual ~RouDi() noexcept;

  protected:
    /// @brief Starts the thread processing messages from the runtimes
    /// Once this is done, applications can register and Roudi is fully operational.
    void startProcessRuntimeMessagesThread() noexcept;

    /// @brief Stops threads and kills all process known to RouDi
    /// Called in d'tor
    ///
    /// @note Intentionally not virtual to be able to call it in derived class
    void shutdown() noexcept;
    virtual void processMessage(const runtime::IpcMessage& message,
                                const iox::runtime::IpcMessageType& cmd,
                                const RuntimeName_t& runtimeName) noexcept;
    virtual void cyclicUpdateHook() noexcept;
    void IpcMessageErrorHandler() noexcept;

    version::VersionInfo parseRegisterMessage(const runtime::IpcMessage& message,
                                              uint32_t& pid,
                                              uid_t& userId,
                                              int64_t& transmissionTimestamp) noexcept;

    /// @brief Handles the registration request from process
    /// @param [in] name of the process which wants to register at roudi; this is equal to the IPC channel name
    /// @param [in] pid is the host system process id
    /// @param [in] user is the posix user id to which the process belongs
    /// @param [in] transmissionTimestamp is an ID for the application to check for the expected response
    /// @param [in] sessionId is an ID generated by RouDi to prevent sending outdated IPC channel transmission
    /// @param [in] versionInfo Version of iceoryx used
    void registerProcess(const RuntimeName_t& name,
                         const uint32_t pid,
                         const posix::PosixUser user,
                         const int64_t transmissionTimestamp,
                         const uint64_t sessionId,
                         const version::VersionInfo& versionInfo) noexcept;

    /// @brief Creates a unique ID which can be used to check outdated IPC channel transmissions
    /// @return a unique, monotonic and consecutive increasing number
    static uint64_t getUniqueSessionIdForProcess() noexcept;

  private:
    void processRuntimeMessages() noexcept;

    void monitorAndDiscoveryUpdate() noexcept;

    cxx::GenericRAII m_unregisterRelativePtr{[] {}, [] { rp::BaseRelativePointer::unregisterAll(); }};
    bool m_killProcessesInDestructor;
    std::atomic_bool m_runMonitoringAndDiscoveryThread;
    std::atomic_bool m_runHandleRuntimeMessageThread;

    const units::Duration m_runtimeMessagesThreadTimeout{100_ms};

  protected:
    RouDiMemoryInterface* m_roudiMemoryInterface{nullptr};
    /// @note destroy the memory right at the end of the dTor, since the memory is not needed anymore and we know that
    /// the lifetime of the MemoryBlocks must be at least as long as RouDi; this saves us from issues if the
    /// RouDiMemoryManager outlives some MemoryBlocks
    cxx::GenericRAII m_roudiMemoryManagerCleaner{[]() {},
                                                 [this]() {
                                                     if (this->m_roudiMemoryInterface->destroyMemory().has_error())
                                                     {
                                                         LogWarn() << "unable to cleanup roudi memory interface";
                                                     };
                                                 }};
    PortManager* m_portManager{nullptr};
    concurrent::smart_lock<ProcessManager> m_prcMgr;

  private:
    std::thread m_monitoringAndDiscoveryThread;
    std::thread m_handleRuntimeMessageThread;

  protected:
    ProcessIntrospectionType m_processIntrospection;
    MemPoolIntrospectionType m_mempoolIntrospection;

  private:
    roudi::MonitoringMode m_monitoringMode{roudi::MonitoringMode::ON};
    units::Duration m_processKillDelay;
};

} // namespace roudi
} // namespace iox

#endif // IOX_POSH_ROUDI_ROUDI_MULTI_PROCESS_HPP
