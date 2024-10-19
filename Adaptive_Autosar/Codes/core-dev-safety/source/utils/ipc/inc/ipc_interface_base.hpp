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

#ifndef UTILS_IPC_INC_IPC_INTERFACE_BASE_HPP
#define UTILS_IPC_INC_IPC_INTERFACE_BASE_HPP

#include "iceoryx_hoofs/cxx/deadline_timer.hpp"
#include "iceoryx_hoofs/cxx/optional.hpp"
#include "iceoryx_hoofs/internal/posix_wrapper/unix_domain_socket.hpp"
#include "iceoryx_hoofs/internal/relocatable_pointer/relative_pointer.hpp"
#include "iceoryx_hoofs/internal/units/duration.hpp"
#include "iceoryx_hoofs/platform/errno.hpp"
#include "iceoryx_hoofs/platform/fcntl.hpp"
#include "iceoryx_hoofs/platform/stat.hpp"
#include "iceoryx_hoofs/platform/time.hpp"
#include "iceoryx_hoofs/platform/types.hpp"
#include "iceoryx_hoofs/platform/unistd.hpp"
#include "ipc_settings.hpp"
#include "ipc_message.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace ara
{
namespace core
{
namespace ipc
{
class IpcInterfaceUser;
class IpcInterfaceCreator;
constexpr uint32_t MAX_RUNTIME_NAME_LENGTH{iox::MAX_IPC_CHANNEL_NAME_LENGTH};
/// @brief String is alias name of RunTimeNameT.
using RunTimeNameT = iox::cxx::string<MAX_RUNTIME_NAME_LENGTH>;
/// @brief Base-Class should never be used by the end-user.
/// Handles the common properties and methods for the childs. The handling of
/// the IPC channels must be done by the children.
/// @note This class won't uniquely identify if another object is using the same IPC channel
/// @archtrace 609490 606814
/// @design 611177 614029
class IpcInterfaceBase
{
 public:
  /// @brief Receives a message from the IPC channel and stores it in answer.
  /// @param[out] answer If a message is received it is stored there.
  /// @return If the call failed or an invalid message was
  /// received it returns false, otherwise true.
  /// @archtrace 609490 609491 606814
  /// @design 611178
  bool Receive(IpcMessage& answer) const noexcept;

  /// @brief Tries to Receive a message from the IPC channel within a
  /// pecified timeout. It stores the message in answer.
  /// @param[in] timeout For receiving a message.
  /// @param[in] answer The answer of the IPC channel. 
  /// If TimedReceive failed the content of answer is undefined.
  /// @return If a valid message was received before the timeout occures
  /// it returns true, otherwise false. It also returns false if clock_gettime() failed
  /// @archtrace 609490 609492 606814
  /// @design 611179
  bool TimedReceive(const iox::units::Duration timeout, core::ipc::IpcMessage& answer) const noexcept;

  /// @brief Tries to Send the message specified in msg.
  /// @param[in] msg Must be a valid message, if its an invalid message,
  /// send will return false.
  /// @return If a valid message was Send it returns true,
  /// therwise if the message was invalid it will return false.
  /// @archtrace 609490 609494 606814
  /// @design 611180
  bool Send(const core::ipc::IpcMessage& msg) const noexcept;

  /// @brief Tries to Send the message specified in msg to the message
  /// queue within a specified timeout.
  /// @param[in] msg Must be a valid message, if its an invalid message, 
  /// send will return false.
  /// @param[in] timeout Specifies the duration to wait for sending.
  /// @return If a valid message was Send it returns true,
  /// otherwise if the message was invalid it will return false.
  /// @archtrace 609490 609495 606814
  /// @design 611181
  bool TimedSend(const ara::core::ipc::IpcMessage& msg, const iox::units::Duration timeout) const noexcept;

  /// @brief Returns the interface name, the unique char string which
  /// explicitly identifies the IPC channel.
  /// @return name of the IPC channel
  /// @archtrace 609490 609496 606814
  /// @design 611182
  const ara::core::ipc::RunTimeNameT& GetRuntimeName() const noexcept;

  /// @brief If the IPC channel could not be opened or linked in the constructor
  /// will return false, otherwise true. This is needed since the constructor is 
  /// not allowed to throw an exception.
  /// You should always check a IPC channel with IsInitialized before using it, 
  /// since all other methods will fail and return false if a message could not
  /// be successfully initialized.
  /// @return initialization state
  /// @archtrace 609490 609507 606814
  /// @design 611183
  bool IsInitialized() const noexcept;

  /// @brief Since there might be an outdated IPC channel due to an unclean termination
  /// this function closes the IPC channel if it's existing.
  /// @param[in] name A name of the IPC channel to clean up
  /// @archtrace 609490 609503 606814
  /// @design 611190
  static void CleanupOutdatedIpcChannel(const ara::core::ipc::RunTimeNameT& name) noexcept;

  friend class IpcInterfaceUser;
  friend class IpcInterfaceCreator;
  friend class IpcRuntimeInterface;

  /// @brief Opens an existing IPC channel using the same parameters as before.
  /// If the queue was not open, it is just opened.
  /// @return true if successfully reopened, false if not.
  /// @archtrace 609490 609499 606814
  /// @design 611184
  bool Reopen() noexcept;
  /// @brief Closes an existing IPC channel using the same parameters as before.
  /// @return true if successfully close IPC channel, false if not.
  /// @archtrace 609490 609498 606814
  /// @design 611185
  bool CloseIpcChannel() noexcept;
  /// @brief Checks if the IPC channel has its counterpart in the file system
  /// @return If the IPC channel, which corresponds to a descriptor,
  /// is still availabe in the file system it returns true,
  /// otherwise it was deleted or the IPC channel was not open and returns false.
  /// @archtrace 609490 609500 606814
  /// @design 611186
  bool IpcChannelMapsToFile() noexcept;


 public:
  /// @brief The default constructor is explicitly deleted since every
  /// IPC channel needs a unique string to be identified with.
  IpcInterfaceBase() = delete;
  /// @brief It is a parameterised constructor.
  /// @param[in] runtimeName A runtime name of IPC Interface Base.
  /// @param[in] maxMessages A max size messages of IPC Interface Base.
  /// @param[in] messageSize A message size of IPC Interface Base.
  /// @archtrace 609490
  /// @design 611187
  IpcInterfaceBase(const ara::core::ipc::RunTimeNameT& runtimeName, const uint64_t maxMessages,
      const uint64_t messageSize) noexcept;
  /// @brief The destructor is invoked.
  virtual ~IpcInterfaceBase() noexcept = default;

  /// @brief Delete copy and move ctor and assignment since they are not needed.
  IpcInterfaceBase(const core::ipc::IpcInterfaceBase&) = delete;
  /// @brief Delete copy and move ctor and assignment since they are not needed.
  IpcInterfaceBase(core::ipc::IpcInterfaceBase&&) = default;
  /// @brief Delete copy and move ctor and assignment since they are not needed.
  IpcInterfaceBase& operator=(const core::ipc::IpcInterfaceBase&) = delete;
  /// @brief Delete copy and move ctor and assignment since they are not needed.
  IpcInterfaceBase& operator=(core::ipc::IpcInterfaceBase&&) = delete;

  /// @brief Set the content of answer from buffer.
  /// @param[in] buffer Raw message as char pointer
  /// @param[out] answer Raw message is setting this IpcMessage
  /// @return answer.isValid()
  /// @archtrace 609490 609493 606814
  /// @design 611191
  static bool SetMessageFromString(const char* buffer, core::ipc::IpcMessage& answer) noexcept;

  /// @brief Opens a IPC channel and default permissions
  /// stored in m_perms and stores the descriptor
  /// @param[in] channelSide A channelSide of the queue. SERVER will also destroy the IPC channel in the dTor, while CLIENT
  /// keeps the IPC channel in the file system after the dTor is called.
  /// @return Returns true if a IPC channel could be opened, otherwise false.
  /// @archtrace 609490 609497 606814
  /// @design 611188
  bool OpenIpcChannel(const iox::posix::IpcChannelSide channelSide) noexcept;

  /// @brief Closes a IPC channel.
  /// @return Returns true if the IPC channel could be closed, otherwise false.

  /// @brief If a IPC channel was moved then mRuntimeName was cleared
  /// and this object gave up the control of that specific
  /// IPC channel and therefore should not unlink or close it.
  /// Otherwise the object which it was moved to can end up with
  /// an invalid IPC channel descriptor.
  /// @return Returns true if the IPC channel is closable, otherwise false.
  /// @archtrace 609490 609502 606814
  /// @design 611189
  bool HasClosableIpcChannel() const noexcept;

 protected:
  /// @brief Declaration of runtime name.
  ara::core::ipc::RunTimeNameT mRuntimeName;
  /// @brief Declaration of maximum message size.
  uint64_t mMaxMessageSize{0U};
  /// @brief Declaration of maximum messages.
  uint64_t mMaxMessage{0U};
  /// @brief Declaration of channel side.
  iox::posix::IpcChannelSide mChannelSide{iox::posix::IpcChannelSide::CLIENT};
  /// @brief Declaration of ipc channel.
  core::platform::IoxIpcChannelType mIpcChannel;
};

} // namespace ipc
} // namespace core
} // namespace ara

#endif // UTILS_IPC_INC_IPC_INTERFACE_BASE_HPP
