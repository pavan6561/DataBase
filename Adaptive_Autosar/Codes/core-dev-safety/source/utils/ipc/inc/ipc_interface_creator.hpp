// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
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

#ifndef UTILS_IPC_INC_IPC_INTERFACE_CREATOR_HPP
#define UTILS_IPC_INC_IPC_INTERFACE_CREATOR_HPP

#include "iceoryx_hoofs/posix_wrapper/file_lock.hpp"
#include "iceoryx_hoofs/error_handling/error_handler.hpp"

#include "ipc_interface_base.hpp"
#include "iceoryx_hoofs/cxx/method_callback.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"
#include "iceoryx_hoofs/cxx/variant_queue.hpp"
#include "iceoryx_hoofs/cxx/vector.hpp"
#include "iceoryx_hoofs/internal/posix_wrapper/ipc_channel.hpp"
#include "iceoryx_hoofs/internal/units/duration.hpp"
#include "iceoryx_hoofs/log/logstream.hpp"
#include "ipc_settings.hpp"
namespace ara
{
namespace core
{
namespace ipc
{
#define kIpcError(error) \
  error(kIpcInterfaceAppWithSameStillRunning) error(kPoshRoudiProcessSendViaIpcChannelFaild) \
      error(kIpcInterfaceUnableToCreateApplicationChannel) error(kIpcInterfaceCouldNotAcquireFileLock)
/// @brief enum Class creating for IpcError.
enum class IpcError : uint32_t
{
  NO_ERROR = iox::POSH_MODULE_IDENTIFIER << iox::ERROR_ENUM_OFFSET_IN_BITS,
  kIpcError(CREATE_ICEORYX_ERROR_ENUM)
};
/// @brief Declare a asStringLiteral method.
/// @design 611174
const char* asStringLiteral(const ara::core::ipc::IpcError error) noexcept;
constexpr uint32_t ROUDI_MAX_MESSAGES{5U};
constexpr uint32_t ROUDI_MESSAGE_SIZE{512U};

namespace runtime
{
using namespace iox::units::duration_literals;
constexpr iox::units::Duration PROCESS_WAITING_FOR_ROUDI_TIMEOUT{60_s};
constexpr iox::units::Duration PROCESS_DEFAULT_KILL_DELAY{45_s};
constexpr iox::units::Duration PROCESS_TERMINATED_CHECK_INTERVAL{250_ms};
constexpr iox::units::Duration DISCOVERY_INTERVAL{100_ms};
} // namespace runtime
namespace mepoo
{
/// @brief Duration is alias name of DurationNsT.
using DurationNsT = std::chrono::duration<std::int64_t, std::nano>;
/// @brief A uint64_t is alias name of SequenceNumberT.
using SequenceNumberT = std::uint64_t;
/// @brief A steady_clock is alias name of BaseClockT.
using BaseClockT = std::chrono::steady_clock;
/// @brief A time_point is alias name of TimePointNsT.
using TimePointNsT = std::chrono::time_point<BaseClockT, DurationNsT>;
} // namespace mepoo
/// @brief Class for creating and handling a IPC channel
/// @note This class makes sures the IPC channel is created uniquely
/// @archtrace 608897
/// @design 611174 614029
class IpcInterfaceCreator : public IpcInterfaceBase
{
 public:
  /// @brief Constructs a IpcInterfaceCreator and opens a new IPC channel.
  /// If it fails isInitialized will return false. Therefore, isInitialized
  /// should always be called before using this class.
  /// @param[in] name Unique identifier of the IPC channel.
  /// @param[in] maxMessages A maximum number of queued messages.
  /// @param[in] messageSize A maximum message size.
  /// @archtrace 608897
  /// @design 611175
  IpcInterfaceCreator(const ara::core::ipc::RunTimeNameT& name,
      const uint64_t maxMessages = ROUDI_MAX_MESSAGES,
      const uint64_t messageSize = ROUDI_MESSAGE_SIZE) noexcept;

  /// @brief The copy constructor and assignment operator is deleted since
  /// this class manages a resource (IPC channel) which cannot
  /// be copied. Move is also not needed, it is also deleted.
  IpcInterfaceCreator(const IpcInterfaceCreator&) = delete;
  /// @brief The assignment operator is deleted.
  IpcInterfaceCreator& operator=(const IpcInterfaceCreator&) = delete;

  /// @brief It is an move constructor.
  IpcInterfaceCreator(IpcInterfaceCreator&&) = default;
  /// @brief The assignment operator is deleted.
  IpcInterfaceCreator& operator=(IpcInterfaceCreator&&) = delete;
  /// @brief Declare a CleanupResource method.
  /// @archtrace 608897
  /// @design 611176
  void CleanupResource() noexcept;

 private:
  friend class IpcRuntimeInterface;
  /// @brief To declare a FileLock .
  iox::posix::FileLock mFileLock_;
};

} // namespace ipc
} // namespace core
} // namespace ara

#endif // UTILS_IPC_INC_IPC_INTERFACE_CREATOR_HPP