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

#include "ipc_interface_creator.hpp"
namespace ara
{
namespace core
{
namespace ipc
{
core::ipc::IpcInterfaceCreator::IpcInterfaceCreator(
    const RunTimeNameT& runtimeName, const uint64_t maxMessages, const uint64_t messageSize) noexcept :
    IpcInterfaceBase(runtimeName, maxMessages, messageSize),
    mFileLock_(std::move(iox::posix::FileLock::create(runtimeName)
                             .or_else([&runtimeName](auto& error) {
                               if (error == iox::posix::FileLockError::LOCKED_BY_OTHER_PROCESS)
                               {
                                 LogFatal() << "An application with the name " << runtimeName
                                            << " is still running. Using the "
                                               "same name twice is not supported.";
                                 errorHandler(ara::core::ipc::IpcError::kIpcInterfaceAppWithSameStillRunning,
                                     iox::ErrorLevel::FATAL);
                               }
                               else
                               {
                                 LogFatal()
                                     << "Error occurred while acquiring file lock named " << runtimeName;
                                 errorHandler(ara::core::ipc::IpcError::kIpcInterfaceCouldNotAcquireFileLock,
                                     iox::ErrorLevel::FATAL);
                               }
                             })
                             .value()))
{
  // check if the IPC channel is still there (e.g. because of no proper termination
  // of the process)
  CleanupOutdatedIpcChannel(runtimeName);

  OpenIpcChannel(iox::posix::IpcChannelSide::SERVER);
}

void IpcInterfaceCreator::CleanupResource() noexcept
{
  mIpcChannel.destroy().or_else(
      [this](auto) { LogWarn() << "unable to cleanup ipc channel resource " << mRuntimeName; });
}
const char* IPCERROR_NAMES[]{kIpcError(CREATE_ICEORYX_ERROR_STRING)};
const char* asStringLiteral(const IpcError error) noexcept
{
  return IPCERROR_NAMES[iox::errorToStringIndex(error)];
}
} // namespace ipc
} // namespace core
} // namespace ara
