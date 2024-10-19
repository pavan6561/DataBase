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

#include "condition_notifier.h"

namespace ara
{
constexpr char LoggingComponentPosh::Ctx[];
constexpr char LoggingComponentPosh::Description[];
namespace core
{
ConditionNotifier::ConditionNotifier(ConditionVariableData& condVarDataRef, const uint64_t index) noexcept :
    mCondVarDataPtr_(&condVarDataRef), mNotificationIndex_(index)
{
  if (index >= MAX_NUMBER_OF_NOTIFIERS)
  {
    LogFatal() << "The provided index " << index << " is too large. The index has to be in the range of [0, "
               << MAX_NUMBER_OF_NOTIFIERS << "[.";
  }
}

void ConditionNotifier::notify() noexcept
{
  GetMembers()->mActiveNotifications[mNotificationIndex_].store(true, std::memory_order_release);
  GetMembers()->mWasNotified.store(true, std::memory_order_relaxed);
  GetMembers()->mSemaphore->post().or_else([](auto) {
    iox::errorHandler(PoshError::kPopoConditionNotifierSemaphoreCorruptInNotify, iox::ErrorLevel::FATAL);
  });
}

void ConditionNotifier::notify_all() noexcept
{
  while (mNotificationIndex_)
  {
    GetMembers()->mActiveNotifications[mNotificationIndex_].store(true, std::memory_order_release);
    mNotificationIndex_ -= 1;
  }
  GetMembers()->mWasNotified.store(true, std::memory_order_relaxed);
  GetMembers()->mSemaphore->post().or_else([](auto) {
    iox::errorHandler(PoshError::kPopoConditionNotifierSemaphoreCorruptInNotify, iox::ErrorLevel::FATAL);
  });
}

ConditionVariableData* ConditionNotifier::GetMembers() noexcept
{
  return mCondVarDataPtr_;
}

} // namespace core
} // namespace ara
