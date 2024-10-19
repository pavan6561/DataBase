// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
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

#include "condition_listener.h"

namespace ara
{
namespace core
{
ConditionListener::ConditionListener(ConditionVariableData& condVarData) noexcept :
    mCondVarDataPtr_(&condVarData)
{
}

void ConditionListener::ResetSemaphore() noexcept
{
  // Count the semaphore down to zero
  bool hasFatalError{false};
  while (!hasFatalError
      && GetMembers()
             ->mSemaphore->tryWait()
             .or_else([&](iox::posix::SemaphoreError) {
               iox::errorHandler(
                   PoshError::kPopoConditionListenerSemaphoreCorruptedInRest, iox::ErrorLevel::FATAL);
               hasFatalError = true;
             })
             .value())
  {
  }
}

void ConditionListener::Destroy() noexcept
{
  mToBeDestroyed_.store(true, std::memory_order_relaxed);
  GetMembers()->mSemaphore->post().or_else([](auto) {
    iox::errorHandler(PoshError::kPopoConditionListenerSemaphoreCorruptedInDestory, iox::ErrorLevel::FATAL);
  });
}

bool ConditionListener::WasNotified() const noexcept
{
  return GetMembers()->mWasNotified.load(std::memory_order_relaxed);
}

ConditionListener::NotificationVectorT ConditionListener::wait() noexcept
{
  return WaitImpl([this]() -> bool {
    if (this->GetMembers()->mSemaphore->wait().has_error())
    {
      std::cout << "[POPO] Error in ConditionListener Wait()\n\n";
      iox::errorHandler(PoshError::kPopoConditionListenerSemaphoreCorruptedInWait, iox::ErrorLevel::FATAL);
      return false;
    }
    return true;
  });
}

ConditionListener::NotificationVectorT ConditionListener::wait(std::unique_lock<std::mutex>& lock) noexcept
{
  lock.unlock();
  return WaitImpl([this, &lock]() -> bool {
    if (this->GetMembers()->mSemaphore->wait().has_error())
    {
      std::cout << "[POPO] Error in ConditionListener Wait()\n\n";
      iox::errorHandler(PoshError::kPopoConditionListenerSemaphoreCorruptedInWait, iox::ErrorLevel::FATAL);
      return false;
    }
    return true;
  });
}

ConditionListener::NotificationVectorT ConditionListener::TimedWait(
    const iox::units::Duration& timeToWait) noexcept
{
  return WaitImpl([this, timeToWait]() -> bool {
    if (this->GetMembers()->mSemaphore->timedWait(timeToWait).has_error())
    {
      iox::errorHandler(
          PoshError::kPopoConditionListenerSemaphoreCorruptedInTimeWait, iox::ErrorLevel::FATAL);
    }
    return false;
  });
}

ConditionListener::NotificationVectorT ConditionListener::TimedWait(
    std::unique_lock<std::mutex>& lock, const iox::units::Duration& timeToWait) noexcept
{
  lock.unlock();
  return WaitImpl([this, &lock, timeToWait]() -> bool {
    if (this->GetMembers()->mSemaphore->timedWait(timeToWait).has_error())
    {
      iox::errorHandler(
          PoshError::kPopoConditionListenerSemaphoreCorruptedInTimeWait, iox::ErrorLevel::FATAL);
    }
    return false;
  });
}

ConditionListener::NotificationVectorT ConditionListener::WaitImpl(
    const iox::cxx::function_ref<bool()>& waitCall) noexcept
{
  using TypeT = iox::cxx::BestFittingType_t<ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER>;
  NotificationVectorT activeNotifications;

  ResetSemaphore();
  bool doReturnAfterNotificationCollection{false};
  while (!mToBeDestroyed_.load(std::memory_order_relaxed))
  {
    for (TypeT i{0U}; i < MAX_NUMBER_OF_NOTIFIERS; i++)
    {
      if (GetMembers()->mActiveNotifications[i].load(std::memory_order_relaxed))
      {
        ResetUnchecked(i);
        activeNotifications.emplace_back(i);
      }
    }
    if (!activeNotifications.empty() || doReturnAfterNotificationCollection)
    {
      return activeNotifications;
    }

    doReturnAfterNotificationCollection = !waitCall();
  }

  return activeNotifications;
}
void ConditionListener::ResetUnchecked(const uint64_t index) noexcept
{
  GetMembers()->mActiveNotifications[index].store(false, std::memory_order_relaxed);
  GetMembers()->mWasNotified.store(false, std::memory_order_relaxed);
}

const ConditionVariableData* ConditionListener::GetMembers() const noexcept
{
  return mCondVarDataPtr_;
}

ConditionVariableData* ConditionListener::GetMembers() noexcept
{
  return mCondVarDataPtr_;
}

} // namespace core
} // namespace ara
