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

#include "ipc_interface_base.hpp"
#include "iceoryx_hoofs/cxx/convert.hpp"
#include "iceoryx_hoofs/internal/posix_wrapper/message_queue.hpp"
#include "ipc_logging.hpp"
#include "ipc_message.hpp"
#include <chrono>
#include <thread>
#include <chrono>
namespace ara
{
namespace core
{
namespace ipc
{
using namespace iox::units::duration_literals;
IpcInterfaceBase::IpcInterfaceBase(
    const RunTimeNameT& runtimeName, const uint64_t maxMessages, const uint64_t messageSize) noexcept :
    mRuntimeName(runtimeName)
{
  mMaxMessage = maxMessages;
  mMaxMessageSize = messageSize;
  if (mMaxMessageSize > core::platform::IoxIpcChannelType::MAX_MESSAGE_SIZE)
  {
    LogWarn() << "Message size too large, reducing from " << messageSize << " to "
              << core::platform::IoxIpcChannelType::MAX_MESSAGE_SIZE;
    mMaxMessageSize = core::platform::IoxIpcChannelType::MAX_MESSAGE_SIZE;
  }
}

bool IpcInterfaceBase::Receive(core::ipc::IpcMessage& answer) const noexcept
{
  auto message{mIpcChannel.receive()};
  if (message.has_error())
  {
    return false;
  }

  return IpcInterfaceBase::SetMessageFromString(message.value().c_str(), answer);
}

bool IpcInterfaceBase::TimedReceive(const iox::units::Duration timeout, core::ipc::IpcMessage& answer) const
    noexcept
{
  return !mIpcChannel.timedReceive(timeout)
              .and_then([&answer](auto& message) {
                core::ipc::IpcInterfaceBase::SetMessageFromString(message.c_str(), answer);
              })
              .has_error()
      && answer.IsValid();
}

bool IpcInterfaceBase::SetMessageFromString(const char* buffer, core::ipc::IpcMessage& answer) noexcept
{
  answer.SetMessage(buffer);
  if (!answer.IsValid())
  {
    LogError() << "The received message " << answer.GetMessage() << " is not valid";
    return false;
  }
  return true;
}

bool IpcInterfaceBase::Send(const core::ipc::IpcMessage& msg) const noexcept
{
  if (!msg.IsValid())
  {
    LogError() << "Trying to Send the message " << msg.GetMessage() << " which "
               << "does not follow the specified syntax.";
    return false;
  }

  auto logLengthError = [&msg](iox::posix::IpcChannelError& error) {
    if (error == iox::posix::IpcChannelError::MESSAGE_TOO_LONG)
    {
      const size_t messageSize{static_cast<size_t>(msg.GetMessage().size())
          + core::platform::IoxIpcChannelType::NULL_TERMINATOR_SIZE};
      LogError() << "msg size of " << messageSize << " bigger than configured max message size";
    }
  };
  return !mIpcChannel.send(msg.GetMessage()).or_else(logLengthError).has_error();
}

bool IpcInterfaceBase::TimedSend(const IpcMessage& msg, iox::units::Duration timeout) const noexcept
{
  if (!msg.IsValid())
  {
    LogError() << "Trying to Send the message " << msg.GetMessage() << " which "
               << "does not follow the specified syntax.";
    return false;
  }

  auto logLengthError = [&msg](iox::posix::IpcChannelError& error) {
    if (error == iox::posix::IpcChannelError::MESSAGE_TOO_LONG)
    {
      const size_t messageSize{static_cast<size_t>(msg.GetMessage().size())
          + core::platform::IoxIpcChannelType::NULL_TERMINATOR_SIZE};
      LogError() << "msg size of " << messageSize << " bigger than configured max message size";
    }
  };
  return !mIpcChannel.timedSend(msg.GetMessage(), timeout).or_else(logLengthError).has_error();
}

const RunTimeNameT& IpcInterfaceBase::GetRuntimeName() const noexcept
{
  return mRuntimeName;
}

bool IpcInterfaceBase::IsInitialized() const noexcept
{
  return mIpcChannel.isInitialized();
}

bool IpcInterfaceBase::OpenIpcChannel(const iox::posix::IpcChannelSide channelSide) noexcept
{
  mIpcChannel.destroy().or_else(
      [this](auto) { LogWarn() << "unable to destroy previous ipc channel " << mRuntimeName; });

  mChannelSide = channelSide;
  core::platform::IoxIpcChannelType::create(mRuntimeName, mChannelSide, mMaxMessageSize, mMaxMessage)
      .and_then([this](auto& ipcChannel) { this->mIpcChannel = std::move(ipcChannel); });

  return mIpcChannel.isInitialized();
}

bool IpcInterfaceBase::CloseIpcChannel() noexcept
{
  return !mIpcChannel.destroy().has_error();
}

bool IpcInterfaceBase::Reopen() noexcept
{
  return OpenIpcChannel(mChannelSide);
}

bool IpcInterfaceBase::IpcChannelMapsToFile() noexcept
{
  return !mIpcChannel.isOutdated().value_or(true);
}

bool IpcInterfaceBase::HasClosableIpcChannel() const noexcept
{
  return mIpcChannel.isInitialized();
}

void IpcInterfaceBase::CleanupOutdatedIpcChannel(const RunTimeNameT& name) noexcept
{
  if (core::platform::IoxIpcChannelType::unlinkIfExists(name).value_or(false))
  {
    LogWarn() << "IPC channel still there, doing an unlink of " << name;
  }
}

} // namespace ipc
} // namespace core
} // namespace ara