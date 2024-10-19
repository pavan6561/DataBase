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

#ifndef UTILS_IPC_INC_IPC_MESSAGE_HPP
#define UTILS_IPC_INC_IPC_MESSAGE_HPP

#include "ipc_logging.hpp"

#include <cstdint>
#include <sstream>
#include <string>
namespace ara
{
namespace core
{
namespace ipc
{
/// @details
//    The symbol , is per default the separator.
//
//    A valid entry is an arbitrary string which does not contain the
//    separator symbol, otherwise, if it contains the separator symbol it
//    is defined as a invalid entry. A valid entry can contain none or more
//    characters.
//
//    A message is a Concatenation of valid entries separated by the
//    separator. A message is defined as valid if all entries contained in
//    that message are valid and it ends with the separator or it is empty,
//    otherwise it is defined as invalid.

/// @brief It create a IpcMessage class.
/// @archtrace 608897
/// @design 611192 614029
class IpcMessage
{
 public:
  /// @brief Creates an empty and valid IPC channel message.
  /// @archtrace 608897
  IpcMessage() noexcept = default;

  /// @brief Creates a separator separated string. If one element
  /// contains a separator symbol the IpcMessage becomes invalid
  /// and returns false on IsValid().
  /// @param[in] msg Array of strings to combine in a message
  /// @archtrace 608897
  /// @design 611193
  IpcMessage(const std::initializer_list<std::string>& msg) noexcept;

  /// @brief Takes a separator separated string and interprets it as
  /// a IpcMessage. In this case the IpcMessage can only become
  /// invalid if it is not empty and does not end with the separator.
  /// @param[in] msg Array of strings to combine in a message.
  /// @archtrace 608897
  /// @design 611194
  IpcMessage(const std::string& msg) noexcept;

  /// @brief Adds a new entry to the IpcMessage, if the entry is invalid
  /// no entry is added and the IpcMessage becomes invalid.
  /// @tparam T Datatype which is convertible to string via std::stringstream.
  /// @param[in] entry Datatype which is convertible to string via std::to_string
  /// @return If the entry is invalid the IpcMessage value is invalid.
  /// @archtrace 608897
  /// @design 611195
  template <typename T>
  IpcMessage& operator<<(const T& entry) noexcept;

  /// @brief Returns the number of entries stored in IpcMessage.
  /// If the message is invalid the return value is undefined.
  /// @return number of entries in message
  /// @archtrace 608897
  /// @design 611196
  uint32_t GetNumberOfElements() const noexcept;

  /// @brief Returns the entry at position f_index. If f_index is larger
  /// then the sum of the entries stored in IpcMessage it returns
  /// @param[in] index A desired entry position.
  /// @return If the element exists it returns the element at f_index
  /// otherwise an empty string.
  /// If the message is invalid the return value is undefined.
  /// @archtrace 608897
  /// @design 611197
  std::string GetElementAtIndex(const uint32_t index) const noexcept;

  /// @brief Returns if an entry is valid.
  /// Non valid entries are containing at least one separator
  /// @param[in] entry A string to check.
  /// @return true = if it is a valid entry otherwise false
  /// @archtrace 608897
  /// @design 611198
  bool IsValidEntry(const std::string& entry) const noexcept;

  /// @brief Check if the message is valid
  /// @return If one element in the CTor initializer_list was invalid it returns false, otherwise true.
  /// @archtrace 608897
  /// @design 611199
  bool IsValid() const noexcept;

  /// @brief The message is casted to the actual separator separated string.
  /// If the message is invalid the return value is undefined.
  /// @return the current message as separator separated string
  /// @archtrace 608897
  /// @design 611200
  std::string GetMessage() const noexcept;

  /// @brief Takes a separator separated string and interprets it as
  /// a IpcMessage. In this case the IpcMessage can only become
  /// invalid if it is not empty and does not end with the separator.
  /// All the entries that were stored previously in the IpcMessage
  /// will be cleared after a call to SetMessage.
  /// @param[in] msg Array of strings to combine in a message
  /// @archtrace 608897
  /// @design 611201
  void SetMessage(const std::string& msg) noexcept;

  /// @brief Clears the message. After a call to ClearMessage() the
  /// message becomes valid again.
  /// @archtrace 608897
  /// @design 611202
  void ClearMessage() noexcept;

  /// @brief Adds a new entry to the IpcMessage, if the entry is invalid
  /// no entry is added and the IpcMessage becomes invalid.
  /// @tparam T Datatype which is convertible to string via std::stringstream.
  /// @param[in] entry To add to the message.
  /// @archtrace 608897
  /// @design 611203
  template <typename T>
  void AddEntry(const T& entry) noexcept;

  /// @brief Compares two IpcMessages to be equal.
  /// @param[in] rhs IpcMessage to compare with.
  /// @return True if the two IpcMessages compare equal.
  /// @archtrace 608897
  /// @design 611204
  bool operator==(const IpcMessage& rhs) const noexcept;

 private:
  /// @brief The declaration of static stkSeparator_.
  static const char stkSeparator_;
  /// @brief Th declaration of a mMsg_.
  std::string mMsg_;
  /// @brief The declaration of a mIsValid_.
  bool mIsValid_{true};
  /// @brief The declaration of a mNumberOfElements_.
  uint32_t mNumberOfElements_{0};
};

} // namespace ipc
} // namespace core
} // namespace ara

#include "ipc_message.inl"

#endif // UTILS_IPC_INC_IPC_MESSAGE_HPP
