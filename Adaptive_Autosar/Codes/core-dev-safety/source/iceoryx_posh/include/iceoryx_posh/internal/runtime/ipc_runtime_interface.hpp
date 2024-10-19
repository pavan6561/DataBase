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

#ifndef IOX_POSH_RUNTIME_IPC_RUNTIME_INTERFACE_HPP
#define IOX_POSH_RUNTIME_IPC_RUNTIME_INTERFACE_HPP

#include "iceoryx_posh/internal/runtime/ipc_interface_creator.hpp"
#include "iceoryx_posh/internal/runtime/ipc_interface_user.hpp"
#include "iceoryx_posh/internal/runtime/ipc_message_types.hpp"
#include "iceoryx_hoofs/cxx/deadline_timer.hpp"
#include "iceoryx_posh/version/version_info.hpp"

namespace iox
{
namespace runtime
{
class IpcRuntimeInterface
{
  public:
    /// @brief Runtime Interface for the own IPC channel and the one to the RouDi daemon
    /// @param[in] roudiName name of the RouDi IPC channel
    /// @param[in] runtimeName name of the application's runtime and its IPC channel
    /// @param[in] roudiWaitingTimeout timeout for searching the RouDi IPC channel
    IpcRuntimeInterface(const iox::RuntimeName_t& roudiName,
                        const iox::RuntimeName_t& runtimeName,
                        const iox::version::VersionInfo& versionInfo,
                        const units::Duration roudiWaitingTimeout) noexcept;
    ~IpcRuntimeInterface() noexcept = default;

    /// @brief Not needed therefore deleted
    IpcRuntimeInterface(const IpcRuntimeInterface&) = delete;
    IpcRuntimeInterface& operator=(const IpcRuntimeInterface&) = delete;
    IpcRuntimeInterface(IpcRuntimeInterface&&) = default;
    IpcRuntimeInterface& operator=(IpcRuntimeInterface&&) = delete;

    /// @brief sends the keep alive trigger to the RouDi daemon
    /// @return true if sending was successful, false if not
    bool sendKeepalive() noexcept;

    /// @brief send a request to the RouDi daemon
    /// @param[in] msg request to RouDi
    /// @param[out] answer response from RouDi
    /// @return true if communication was successful, false if not
    bool sendRequestToDaemeon(const IpcMessage& msg, IpcMessage& answer) noexcept;

    /// @brief get the adress offset of the segment manager
    /// @return address offset as rp::BaseRelativePointer::offset_t
    iox::rp::BaseRelativePointer::offset_t getSegmentManagerAddressOffset() const noexcept;

    /// @brief get the size of the management shared memory object
    /// @return size in bytes
    size_t getShmTopicSize() noexcept;

    /// @brief get the segment id of the shared memory object
    /// @return segment id
    uint64_t getSegmentId() const noexcept;

  private:
    enum class RegAckResult
    {
        SUCCESS,
        TIMEOUT
    };

    void waitForRoudi(cxx::DeadlineTimer& timer) noexcept;

    RegAckResult waitForRegAck(const int64_t transmissionTimestamp) noexcept;

  private:
    iox::RuntimeName_t m_runtimeName;
    const iox::version::VersionInfo m_versionInfo;
    iox::cxx::optional<iox::rp::BaseRelativePointer::offset_t> m_segmentManagerAddressOffset;
    IpcInterfaceCreator m_AppIpcInterface;
    IpcInterfaceUser m_RoudiIpcInterface;
    uint64_t m_shmTopicSize{0U};
    uint64_t m_segmentId{0U};
};

} // namespace runtime
} // namespace iox

#endif // IOX_POSH_RUNTIME_IPC_RUNTIME_INTERFACE_HPP
