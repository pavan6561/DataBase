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

#include "version_info.hpp"
#include "iceoryx_hoofs/cxx/convert.hpp"

#include <algorithm>
namespace ara
{
namespace core
{
namespace ipc
{
namespace version
{
VersionInfo::VersionInfo(const uint16_t versionMajor, const uint16_t versionMinor,
    const uint16_t versionPatch, const uint16_t versionTweak, const BuildDateStringT& buildDateString,
    const CommitIdStringT& commitIdString) noexcept :
    mVersionMajor(versionMajor),
    mVersionMinor(versionMinor), mVersionPatch(versionPatch), mVersionTweak(versionTweak),
    mBuildDateString(buildDateString), mCommitIdString(commitIdString)
{
}

VersionInfo::VersionInfo(const iox::cxx::Serialization& serial) noexcept
{
  SerializationStringT tmpMBuildDataString;
  SerializationStringT tmpCommitIdString;
  mValid = serial.extract(
      mVersionMajor, mVersionMinor, mVersionPatch, mVersionTweak, tmpMBuildDataString, tmpCommitIdString);
  mBuildDateString = BuildDateStringT(iox::cxx::TruncateToCapacity, tmpMBuildDataString.c_str());
  mCommitIdString = CommitIdStringT(iox::cxx::TruncateToCapacity, tmpCommitIdString.c_str());
}

/// @brief Serialization of the VersionInfo.
VersionInfo::operator iox::cxx::Serialization() const noexcept
{
  SerializationStringT tmpMBuildDataString{mBuildDateString};
  SerializationStringT tmpCommitIdString{mCommitIdString};
  return iox::cxx::Serialization::create(
      mVersionMajor, mVersionMinor, mVersionPatch, mVersionTweak, tmpMBuildDataString, tmpCommitIdString);
}

bool VersionInfo::operator==(const VersionInfo& rhs) const noexcept
{
  return (mValid == rhs.mValid) && (mVersionMajor == rhs.mVersionMajor)
      && (mVersionMinor == rhs.mVersionMinor) && (mVersionPatch == rhs.mVersionPatch)
      && (mVersionTweak == rhs.mVersionTweak) && (mBuildDateString == rhs.mBuildDateString)
      && (mCommitIdString == rhs.mCommitIdString);
}

bool VersionInfo::operator!=(const VersionInfo& rhs) const noexcept
{
  return !(*this == rhs);
}

bool VersionInfo::CheckCompatibility(
    const VersionInfo& other, const CompatibilityCheckLevel compatibilityCheckLevel) const noexcept
{
  bool isCompatible{false};
  switch (compatibilityCheckLevel)
  {
  case CompatibilityCheckLevel::kOff:
    isCompatible = true;
    break;
  case CompatibilityCheckLevel::kMajor:
    isCompatible = (mValid == other.mValid) && (mVersionMajor == other.mVersionMajor);
    break;
  case CompatibilityCheckLevel::kMinor:
    isCompatible = (mValid == other.mValid) && (mVersionMajor == other.mVersionMajor)
        && (mVersionMinor == other.mVersionMinor);
    break;
  case CompatibilityCheckLevel::kPatch:
    isCompatible = (mValid == other.mValid) && (mVersionMajor == other.mVersionMajor)
        && (mVersionMinor == other.mVersionMinor) && (mVersionPatch == other.mVersionPatch);
    break;
  case CompatibilityCheckLevel::kCommitId:
    isCompatible = (mValid == other.mValid) && (mVersionMajor == other.mVersionMajor)
        && (mVersionMinor == other.mVersionMinor) && (mVersionPatch == other.mVersionPatch)
        && (mVersionTweak == other.mVersionTweak) && (mCommitIdString == other.mCommitIdString);
    break;
  case CompatibilityCheckLevel::kBuildDate:
    isCompatible = (*this == other);
    break;
  }
  return isCompatible;
}

bool VersionInfo::IsValid() noexcept
{
  return mValid;
}

VersionInfo VersionInfo::GetCurrentVersion() noexcept
{
  BuildDateStringT buildDateStringCxx{IPC_BUILDDATE};
  CommitIdStringT shortCommitIdString{iox::cxx::TruncateToCapacity, IPC_SHA1, COMMIT_ID_STRING_SIZE};

  return VersionInfo(static_cast<uint16_t>(IPC_VERSION_MAJOR), static_cast<uint16_t>(IPC_VERSION_MINOR),
      static_cast<uint16_t>(IPC_VERSION_PATCH), static_cast<uint16_t>(IPC_VERSION_TWEAK), buildDateStringCxx,
      shortCommitIdString);
}

} // namespace version
} // namespace ipc
} // namespace core
} // namespace ara
