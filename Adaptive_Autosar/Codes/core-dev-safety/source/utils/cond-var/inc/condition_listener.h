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

#ifndef CORE_SOURCE_UTILS_COND_VAR_INC_CONDITION_LISTENER_H
#define CORE_SOURCE_UTILS_COND_VAR_INC_CONDITION_LISTENER_H

#include "iceoryx_hoofs/cxx/helplets.hpp"
#include "condition_variable_data.h"

#include <thread>
#include <mutex>
namespace ara
{
namespace core
{
/// @brief ConditionListener allows one to wait using a shared memory condition variable.
/// @archtrace 608896
/// @design 611236 614040
class ConditionListener
{
 public:
  /// @brief Vector alias of NotificationVectorT.
  using NotificationVectorT
      = iox::cxx::vector<iox::cxx::BestFittingType_t<MAX_NUMBER_OF_NOTIFIERS>, MAX_NUMBER_OF_NOTIFIERS>;
  /// @brief Declare a constrcutor class of ConditionListener.
  /// @param[in] condVarData A condition variable is an object able to block the calling thread until notified to resume.
  /// @archtrace 608896
  /// @design 611237
  explicit ConditionListener(ConditionVariableData& condVarData) noexcept;
  /// @brief Destroys the ConditionListener object.
  ~ConditionListener() noexcept = default;
  /// @brief A copy constructor shall be disabled.
  /// @param[in] rhs Is the copy origin.
  ConditionListener(const ConditionListener& rhs) = delete;
  /// @brief  A move constructor shall be disabled.
  /// @param[in] rhs Is the move origin.
  ConditionListener(ConditionListener&& rhs) noexcept = delete;
  /// @brief The copy assignment shall be disabled.
  /// @param[in] rhs Is the copy origin.
  /// @returns The reference to self.
  ConditionListener& operator=(const ConditionListener& rhs) = delete;
  /// @brief The move assignment shall be disabled.
  /// @param[in] rhs Is the move origin.
  /// @returns The reference to self.
  ConditionListener& operator=(ConditionListener&& rhs) noexcept = delete;

  /// @brief Was the ConditionListener notified by a ConditionNotifier.
  /// @returns true if it was notified otherwise false
  /// @archtrace 608896
  /// @design 611238
  bool WasNotified() const noexcept;

  /// @brief Used in classes to signal a thread which waits in wait() to return and stop working.
  /// Destroy will send an empty notification to wait() and after this call wait() turns into a
  /// non blocking call which always returns an empty vector.
  /// @archtrace 608896
  /// @design 611239
  void Destroy() noexcept;

  /// @brief Returns a sorted vector of indices of active notifications; blocking if ConditionVariableData
  /// was not notified unless Destroy() was called before. The indices of active notifications are
  /// never empty unless Destroy() was called, then it's always empty.
  /// @returns a sorted vector of active notifications.
  /// @archtrace 608896
  /// @design 611240
  NotificationVectorT wait() noexcept;

  /// @brief Wait causes the current thread to block until the condition variable is notified or a spurious
  /// wakeup occurs, optionally looping until some predicate is satisfied (bool(stopWaiting()) == true).
  /// @tparam Predicate A callable object or function that takes no arguments and returns a value
  /// that can be evaluated as a bool.
  /// @param[in] stopWaiting Predicate which returns false if the waiting should be continued
  /// @archtrace 608896
  /// @design 611241
  template <typename Predicate>
  void wait(Predicate stopWaiting) noexcept
  {
    while (!stopWaiting())
    {
      if (WasNotified())
      {
        break;
      }
      else
      {
        std::cout << "Waiting for 1ms in Listener\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }
  }
  /// @brief Atomically unlocks lock, blocks the current executing thread, and adds it to the list of
  /// threads waiting on *this.
  /// @param[in] lock An object of type std::unique_lock<std::mutex>, which must be locked by the
  /// current thread.
  /// @returns The execution of the current thread (which shall have locked lck's mutex) is blocked until notified.
  /// @archtrace 608896
  /// @design 611242
  NotificationVectorT wait(std::unique_lock<std::mutex>& lock) noexcept;
  /// @brief This overload may be used to ignore spurious awakenings while waiting for a specific
  /// condition to become true.
  /// @tparam Predicate A callable object or function that takes no arguments and returns a value
  /// that can be evaluated as a bool.
  /// @param[in] lock An object of type std::unique_lock<std::mutex>, which must be locked by the
  /// current thread.
  /// @param[in] stopWaiting Predicate which returns false if the waiting should be continued
  /// @archtrace 608896
  /// @design 611243
  template <typename Predicate>
  void wait(std::unique_lock<std::mutex>& lock, Predicate stopWaiting) noexcept
  {
    lock.unlock();
    while (!stopWaiting())
    {
      if (WasNotified())
      {
        break;
      }
      else
      {
        std::cout << "Waiting for 1ms in Listener\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }
    lock.lock();
  }

  /// @brief Returns a sorted vector of indices of active notifications; blocking for the specified
  /// time if ConditionVariableData was not notified unless Destroy() was called before.
  /// The indices of active notifications can be empty (spurious wakeups).
  /// When Destroy() was called then it is always empty.
  /// @param[in] timeToWait Duration how long at most this method should wait.
  /// @returns a sorted vector of active notifications.
  /// @archtrace 608896 609961
  /// @design 611244
  NotificationVectorT TimedWait(const iox::units::Duration& timeToWait) noexcept;

  /// @brief Returns a sorted vector of indices of active notifications; blocking for the specified
  /// time if ConditionVariableData was not notified unless Destroy() was called before.
  /// The indices of active notifications can be empty (spurious wakeups).
  /// When Destroy() was called then it is always empty.
  /// @param[in] lock An object of type std::unique_lock<std::mutex>,
  /// which must be locked by the current thread.
  /// @param[in] timeToWait Duration how long at most this method should wait.
  /// @returns a sorted vector of active notifications.
  /// @archtrace 608896 609961
  /// @design 611245
  NotificationVectorT TimedWait(
      std::unique_lock<std::mutex>& lock, const iox::units::Duration& timeToWait) noexcept;

 protected:
  /// @brief Declare a getmember method it is return the const ConditionVariableData pointer.
  /// @returns It returns const ConditionVariableData pointer.
  /// @archtrace 608896
  /// @design 611246
  const ConditionVariableData* GetMembers() const noexcept;
  /// @brief Declare a getmember method it is return the ConditionVariableData pointer.
  /// @returns It returns ConditionVariableData pointer.
  /// @archtrace 608896
  /// @design 611247
  ConditionVariableData* GetMembers() noexcept;

 private:
  /// @brief Declare a ResetUnchecked method it is return the void.
  /// @param[in] index A numeric indicator to directly access individual characters in a string.
  /// @design 611236
  void ResetUnchecked(const uint64_t index) noexcept;
  /// @brief Declare a ResetSemaphore method it is return the void.
  /// @design 611236
  void ResetSemaphore() noexcept;
  /// @brief Declare a WaitImpl method it is return the NotificationVectorT.
  /// @design 611236
  NotificationVectorT WaitImpl(const iox::cxx::function_ref<bool()>& waitCall) noexcept;

 private:
  /// @brief Declare a ConditionVariableData pointer  varible is mCondVarDataPtr_.
  ConditionVariableData* mCondVarDataPtr_{nullptr};
  /// @brief Declare a atomic_bool varible is mToBeDestroyed_.
  std::atomic_bool mToBeDestroyed_{false};
};

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_UTILS_COND_VAR_INC_CONDITION_LISTENER_H
