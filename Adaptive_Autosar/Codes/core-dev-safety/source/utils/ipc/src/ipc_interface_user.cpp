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

#include "ipc_interface_user.hpp"
#include <thread>
#include <chrono>
namespace ara
{
namespace core
{
namespace ipc
{
using namespace iox::units::duration_literals;
core::ipc::IpcInterfaceUser::IpcInterfaceUser(
    const RunTimeNameT& name, const uint64_t maxMessages, const uint64_t messageSize) noexcept :
    IpcInterfaceBase(name, maxMessages, messageSize)
{
  OpenIpcChannel(iox::posix::IpcChannelSide::CLIENT);
}
bool core::ipc::IpcInterfaceUser::InitIpcUserChannel() noexcept
{
  constexpr iox::units::Duration kIpcUserConnectionTimeout = 500_ms;
  constexpr uint16_t kIpcUserConnectionPollTime = 50;
  bool isIPCChannelInit = false;

  iox::cxx::DeadlineTimer timer{kIpcUserConnectionTimeout};

  while ((!timer.hasExpired()) && (isIPCChannelInit == false))
  {
    if (!IsInitialized())
    {
      Reopen();
    }
    else
    {
      isIPCChannelInit = true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(kIpcUserConnectionPollTime));
  }

  return isIPCChannelInit;
}

} // namespace ipc
} // namespace core
} // namespace ara