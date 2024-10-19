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
#ifndef CORE_SOURCE_UTILS_COND_VAR_INC_CONDITION_VARIABLE_DATA_H
#define CORE_SOURCE_UTILS_COND_VAR_INC_CONDITION_VARIABLE_DATA_H

#include "iceoryx_hoofs/posix_wrapper/unnamed_semaphore.hpp"
#include "iceoryx_hoofs/error_handling/error_handler.hpp"
#include "iceoryx_hoofs/cxx/vector.hpp"

#include <atomic>

namespace ara
{
namespace mepoo
{
/// @brief The uint64_t is a alias of SequenceNumberT.
using SequenceNumberT = std::uint64_t;
/// @brief The steady_clock is a alias of BaseClockT.
using BaseClockT = std::chrono::steady_clock;

// use signed integer for duration;
// there is a bug in gcc 4.8 which leads to a wrong calculated time
// when sleep_until() is used with a timepoint in the past

/// @brief Duration is a alias of DurationNsT.
using DurationNsT = std::chrono::duration<std::int64_t, std::nano>;
/// @brief The time_point is a alias of TimePointNsT.
using TimePointNsT = std::chrono::time_point<BaseClockT, DurationNsT>;

} // namespace mepoo
#define POSH_ERRORS(error) \
  error(kPopoConditionVaribaleDataFailedToCreateSemaphore) \
      error(kPopoConditionListenerSemaphoreCorruptedInWasTriggred) \
          error(kPopoConditionListenerSemaphoreCorruptedInWait) \
              error(kPopoConditionListenerSemaphoreCorruptedInTimeWait) \
                  error(kPopoConditionListenerSemaphoreCorruptedInRest) \
                      error(kPopoConditionListenerSemaphoreCorruptedInDestory) \
                          error(kPopoConditionNotifierIndexTooLarge) \
                              error(kPopoConditionNotifierSemaphoreCorruptInNotify) \
                                  error(kPopoNotificationInfoTypeInconsistencyInGetOrigin) \
                                      error(kIpcInterfaceCouldNotAcquireFileLock)
/// @brief Declare the enum class.
enum class PoshError : uint32_t
{
  NO_ERROR = iox::POSH_MODULE_IDENTIFIER << iox::ERROR_ENUM_OFFSET_IN_BITS,
  POSH_ERRORS(CREATE_ICEORYX_ERROR_ENUM)
};
/// @brief Declare the asStringLiteral method it is return the const char pointer.
/// @design 614040
const char* asStringLiteral(const PoshError error) noexcept;
/*TO DO*/

constexpr uint32_t MAX_RUNTIME_NAME_LENGTH{100U};
/// @brief String is a alias of RuntimeNameT.
using RuntimeNameT = iox::cxx::string<MAX_RUNTIME_NAME_LENGTH>;

constexpr uint32_t IOX_MAX_NUMBER_OF_NOTIFIERS{static_cast<uint32_t>(256)};

constexpr uint32_t MAX_NUMBER_OF_NOTIFIERS{IOX_MAX_NUMBER_OF_NOTIFIERS};
/// @note Waitset and Listener share both the max available notifiers, if one of them is running
/// out of of notifiers
/// the variable above must be increased
constexpr uint32_t MAX_NUMBER_OF_ATTACHMENTS_PER_WAITSET{MAX_NUMBER_OF_NOTIFIERS};
constexpr uint32_t MAX_NUMBER_OF_EVENTS_PER_LISTENER{MAX_NUMBER_OF_NOTIFIERS};
namespace core
{
/// @brief Declare the ConditionVariableData struct.
/// @design 614040
struct ConditionVariableData
{
  /// @brief Constructs a ConditionVariableData object.
  ConditionVariableData() noexcept;
  /// @brief Constructs a ConditionVariableData with parameters.
  explicit ConditionVariableData(const RuntimeNameT& runtimeName) noexcept;
  /// @brief A copy construction shall be disabled.
  /// @param[in] rhs Is the copy origin.
  ConditionVariableData(const ConditionVariableData& rhs) = delete;
  /// @brief A move constructor shall be disabled.
  /// @param[in] rhs Is the move origin.
  ConditionVariableData(ConditionVariableData&& rhs) = delete;
  /// @brief The copy assignment shall be disabled.
  /// @param[in] rhs Is the copy origin.
  /// @returns The reference to self.
  ConditionVariableData& operator=(const ConditionVariableData& rhs) = delete;
  /// @brief The move assignment shall be disabled.
  /// @param[in] rhs Is the move origin.
  /// @returns The reference to self.
  ConditionVariableData& operator=(ConditionVariableData&& rhs) = delete;
  /// @brief Destroys the ConditionVariableData object.
  ~ConditionVariableData() noexcept = default;

  /// @brief Declare the unnamed semaphore.
  iox::cxx::optional<iox::posix::UnnamedSemaphore> mSemaphore;

  /// @brief Declare the RuntimeNameT with varible is mRuntimeName.
  RuntimeNameT mRuntimeName;
  /// @brief Declare the atomic_bool with varible is mToBeDestroyed.
  std::atomic_bool mToBeDestroyed{false};
  /// @brief Declare the atomic_bool with varible is mActiveNotifications.
  std::atomic_bool mActiveNotifications[MAX_NUMBER_OF_NOTIFIERS];
  /// @brief Declare the atomic_bool with varible is mWasNotified.
  std::atomic_bool mWasNotified{false};
};

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_UTILS_COND_VAR_INC_CONDITION_VARIABLE_DATA_H
