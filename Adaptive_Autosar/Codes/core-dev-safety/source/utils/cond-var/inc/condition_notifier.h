// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
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
#ifndef CORE_SOURCE_UTILS_COND_VAR_INC_CONDITION_NOTIFIER_H
#define CORE_SOURCE_UTILS_COND_VAR_INC_CONDITION_NOTIFIER_H

#include "iceoryx_hoofs/cxx/helplets.hpp"
#include "condition_variable_data.h"
#include "iceoryx_hoofs/log/logging_free_function_building_block.hpp"

namespace ara
{
/// @brief Declare a struct and name is LoggingComponentPosh.
struct LoggingComponentPosh
{
  // NOLINTNEXTLINE(readability-identifier-naming)
  static constexpr char Ctx[]{"POSH"};
  // NOLINTNEXTLINE(readability-identifier-naming)
  static constexpr char Description[]{"Log context of the POSH component!"};
};
// NOLINTNEXTLINE(readability-identifier-naming)
static constexpr auto LogFatal{iox::log::ffbb::LogFatal<LoggingComponentPosh>};
namespace core
{
/// @brief ConditionNotifier can notifiy waiting threads and processes using a shared memory
/// condition variable.
/// @archtrace 608896
/// @design 611231 614040
class ConditionNotifier
{
 public:
  static constexpr uint64_t INVALID_NOTIFICATION_INDEX{std::numeric_limits<uint64_t>::max()};
  /// @brief Declare a constrcutor class of ConditionNotifier.
  /// @param[in] condVarDataRef It is condition variable date reference.
  /// @param[in] index A numeric indicator to directly access individual characters in a string.
  /// @archtrace 608896
  /// @design 611232
  ConditionNotifier(ConditionVariableData& condVarDataRef, const uint64_t index) noexcept;
  /// @brief A copy constructor shall be disabled.
  /// @param[in] rhs Is the copy origin.
  ConditionNotifier(const ConditionNotifier& rhs) = delete;
  /// @brief A move constructor shall be disabled.
  /// @param[in] rhs Is the move origin.
  ConditionNotifier(ConditionNotifier&& rhs) noexcept = delete;
  /// @brief The copy assignment shall be disabled.
  /// @param[in] rhs Is the copy origin.
  /// @returns The reference to self.
  ConditionNotifier& operator=(const ConditionNotifier& rhs) = delete;
  /// @brief The move assignment shall be disabled.
  /// @param[in] rhs Is the move origin.
  /// @returns The reference to self.
  ConditionNotifier& operator=(ConditionNotifier&& rhs) noexcept = delete;
  /// @brief Destroys the ConditionNotifier object.
  ~ConditionNotifier() noexcept = default;

  /// @brief If threads are waiting on the condition variable, this call unblocks one of the waiting threads
  /// @archtrace 608896
  /// @design 611233
  void notify() noexcept;

  /// @brief Unblocks all threads currently waiting for this condition.
  /// If no threads are waiting, the function does nothing.
  /// @archtrace 608896 609970 609956
  /// @design 611234
  void notify_all() noexcept;

 protected:
  /// @brief Declare a getmemners method it is return the const ConditionVariableData pointer.
  /// @returns It return the const ConditionVariableData pointer.
  /// @archtrace 608896
  /// @design 611235
  ConditionVariableData* GetMembers() noexcept;

 private:
  /// @brief Declare a ConditionVariableData pointer  varible is mCondVarDataPtr_.
  ConditionVariableData* mCondVarDataPtr_{nullptr};
  /// @brief Declare a uint64 varible is mNotificationIndex_
  uint64_t mNotificationIndex_{INVALID_NOTIFICATION_INDEX};
};

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_UTILS_COND_VAR_INC_CONDITION_NOTIFIER_H
