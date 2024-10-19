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

#ifndef UTILS_IPC_INC_VERSION_COMPATIBILITY_CHECK_LEVEL_HPP
#define UTILS_IPC_INC_VERSION_COMPATIBILITY_CHECK_LEVEL_HPP

#include "iceoryx_hoofs/log/logstream.hpp"
namespace ara
{
namespace core
{
namespace ipc
{
namespace version
{
/// @brief CompatibilityCheckLevel class of enum type.
enum class CompatibilityCheckLevel : int16_t
{
  kOff = 0,
  kMajor = 1,
  kMinor = 2,
  kPatch = 3,
  kCommitId = 4,
  kBuildDate = 5
};

/// @brief Inline function to check CompatibilityCheckLevel.
/// @param[in] logstream It is type of log given in level.
/// @param[in] level It is type of CompatibilityCheckLevel.
/// @return It returns logstream.
/// @archtrace 608897
/// @design 611205 614029
inline iox::log::LogStream& operator<<(
    iox::log::LogStream& logstream, const ara::core::ipc::version::CompatibilityCheckLevel& level) noexcept
{
  switch (level)
  {
  case CompatibilityCheckLevel::kOff:
    logstream << "CompatibilityCheckLevel::OFF";
    break;
  case CompatibilityCheckLevel::kMajor:
    logstream << "CompatibilityCheckLevel::MAJOR";
    break;
  case CompatibilityCheckLevel::kMinor:
    logstream << "CompatibilityCheckLevel::MINOR";
    break;
  case CompatibilityCheckLevel::kPatch:
    logstream << "CompatibilityCheckLevel::PATCH";
    break;
  case CompatibilityCheckLevel::kCommitId:
    logstream << "CompatibilityCheckLevel::COMMIT_ID";
    break;
  case CompatibilityCheckLevel::kBuildDate:
    logstream << "CompatibilityCheckLevel::BUILD_DATE";
    break;
  default:
    logstream << "CompatibilityCheckLevel::UNDEFINED";
    break;
  }
  return logstream;
}

} // namespace version
} // namespace ipc
} // namespace core
} // namespace ara
#endif // UTILS_IPC_INC_VERSION_COMPATIBILITY_CHECK_LEVEL_HPP
