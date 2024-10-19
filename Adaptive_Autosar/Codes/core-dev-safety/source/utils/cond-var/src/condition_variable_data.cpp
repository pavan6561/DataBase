// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2020 - 2021 by Apex.AI Inc. All rights reserved.
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

#include "condition_variable_data.h"

namespace ara
{
const char* POSH_ERROR_NAMES[]{POSH_ERRORS(CREATE_ICEORYX_ERROR_STRING)};
const char* asStringLiteral(const PoshError error) noexcept
{
  return POSH_ERROR_NAMES[iox::errorToStringIndex(error)];
}

namespace core
{
ConditionVariableData::ConditionVariableData() noexcept : ConditionVariableData("")
{
}

ConditionVariableData::ConditionVariableData(const RuntimeNameT& runtimeName) noexcept :
    mRuntimeName(runtimeName)
{
  iox::posix::UnnamedSemaphoreBuilder()
      .initialValue(0U)
      .isInterProcessCapable(true)
      .create(mSemaphore)
      .or_else([](auto) {
        std::cout << "errorHandler" << std::endl;
        iox::errorHandler(
            PoshError::kPopoConditionVaribaleDataFailedToCreateSemaphore, iox::ErrorLevel::FATAL);
      });
  for (auto& id : mActiveNotifications)
  {
    id.store(false, std::memory_order_relaxed);
  }
}
} // namespace core
} // namespace ara
