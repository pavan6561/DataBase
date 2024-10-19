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

#ifndef UTILS_IPC_INC_IPC_MESSAGE_INL1
#define UTILS_IPC_INC_IPC_MESSAGE_INL1

#include "ipc_message.hpp"
namespace ara
{
namespace core
{
namespace ipc
{
template <typename T>
/// @brief Adds a new entry to the IpcMessage, if the entry is invalid
///  no entry is added and the IpcMessage becomes invalid.
/// @param[in] entry To add to the message.
void core::ipc::IpcMessage::AddEntry(const T& entry) noexcept
{
  std::stringstream newEntry;
  newEntry << entry;

  if (!IsValidEntry(newEntry.str()))
  {
    LogError() << "\'" << newEntry.str().c_str() << "\' is an invalid IPC channel entry";
    mIsValid_ = false;
  }
  else
  {
    mMsg_.append(newEntry.str() + stkSeparator_);
    ++mNumberOfElements_;
  }
}

template <typename T>
/// @brief Adds a new entry to the IpcMessage, if the entry is invalid
/// no entry is added and the IpcMessage becomes invalid.
/// @param[in] entry To add to the message.
core::ipc::IpcMessage& core::ipc::IpcMessage::operator<<(const T& entry) noexcept
{
  AddEntry(entry);
  return *this;
}

} // namespace ipc
} // namespace core
} // namespace ara

#endif // UTILS_IPC_INC_IPC_MESSAGE_INL
