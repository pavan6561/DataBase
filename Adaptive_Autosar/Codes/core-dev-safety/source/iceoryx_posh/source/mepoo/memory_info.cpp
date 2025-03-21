// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2022 by Apex.AI Inc. All rights reserved.
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

#include "iceoryx_posh/mepoo/memory_info.hpp"

namespace iox
{
namespace mepoo
{
MemoryInfo::MemoryInfo(uint32_t deviceId, uint32_t memoryType) noexcept :
    deviceId(deviceId), memoryType(memoryType)
{
}

bool MemoryInfo::operator==(const MemoryInfo& rhs) const noexcept
{
  return deviceId == rhs.deviceId && memoryType == rhs.memoryType;
}
} // namespace mepoo
} // namespace iox
