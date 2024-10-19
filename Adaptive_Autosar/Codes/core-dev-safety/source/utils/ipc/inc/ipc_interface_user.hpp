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

#ifndef UTILS_IPC_INC_IPC_INTERFACE_USER_HPP
#define UTILS_IPC_INC_IPC_INTERFACE_USER_HPP

#include "ipc_interface_base.hpp"
namespace ara
{
namespace core
{
namespace ipc
{
constexpr uint32_t APP_MAX_MESSAGES{5U};
constexpr uint32_t APP_MESSAGE_SIZE{512U};
/// @brief Class for using a IPC channel
/// @archtrace 608897
/// @design 611172 614029
class IpcInterfaceUser : public IpcInterfaceBase
{
 public:
  /// @brief Constructs a IpcInterfaceUser and opens a IPC channel.
  /// Therefore, isInitialized should always be called
  /// before using this class.
  /// @param[in] name Unique identifier of the IPC channel.
  /// @param[in] maxMessages A maximum number of queued messages.
  /// @param[in] messageSize A maximum message size.
  /// @archtrace 608897
  /// @design 611173
  IpcInterfaceUser(const ara::core::ipc::RunTimeNameT& name, const uint64_t maxMessages = APP_MAX_MESSAGES,
      const uint64_t messageSize = APP_MESSAGE_SIZE) noexcept;

  /// @brief The copy constructor and assignment operator are deleted since
  /// this class manages a resource (IPC channel) which cannot
  /// be copied. Since move is not needed it is also deleted.
  IpcInterfaceUser(const IpcInterfaceUser&) = delete;
  /// @brief The copy assignment operator is deleted.
  IpcInterfaceUser& operator=(const IpcInterfaceUser&) = delete;

  /// @brief The move constructor to a class.
  IpcInterfaceUser(IpcInterfaceUser&&) = default;
  /// @brief The move assignment operator is deleted.
  IpcInterfaceUser& operator=(IpcInterfaceUser&&) = delete;
      /// @design 611172
      bool InitIpcUserChannel()  noexcept;
};

} // namespace ipc
} // namespace core
} // namespace ara

#endif // UTILS_IPC_INC_IPC_INTERFACE_USER_HPP