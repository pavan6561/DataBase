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

#ifndef UTILS_IPC_INC_VERSION_INFO_HPP
#define UTILS_IPC_INC_VERSION_INFO_HPP

#include "iceoryx_hoofs/cxx/helplets.hpp"
#include "iceoryx_hoofs/cxx/serialization.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"
#include "compatibility_check_level.hpp"
#include "ipc_versions.hpp"

#include <cstdint>
namespace ara
{
namespace core
{
namespace ipc
{
namespace version
{
static const uint64_t COMMIT_ID_STRING_SIZE{12U};
/// @brief String is alias of CommitIdStringT.
using CommitIdStringT = iox::cxx::string<COMMIT_ID_STRING_SIZE>;
static const uint64_t BUILD_DATE_STRING_SIZE{36U};
/// @brief String is alias of BuildDateStringT.
using BuildDateStringT = iox::cxx::string<BUILD_DATE_STRING_SIZE>;
/// @brief Is used to compare the RouDis and runtime's version information.
/// @design 614029
class VersionInfo
{
 public:
  /// @brief Generates an VersionInfo initialized with the information given by the auto generated
  /// iceoryx_versions.hpp defines.
  /// @param{in] versionMajor It is the major version.
  /// @param{in] versionMinor It is the minor version.
  /// @param{in] versionPatch It is the patch version.
  /// @param{in] versionTweak It is the tweak/RC version.
  /// @param{in] buildDateString The date when the component was build
  /// as string with maximal 36 readable chars.
  /// @param{in] commitIdString The commit id is shortened internally to 12 readable chars.
  VersionInfo(const uint16_t versionMajor, const uint16_t versionMinor, const uint16_t versionPatch,
      const uint16_t versionTweak, const BuildDateStringT& buildDateString,
      const CommitIdStringT& commitIdString) noexcept;

  /// @brief Construction of the VersionInfo using serialized strings.
  /// @param[in] serial The serialization object from read from to initialize this object.
  VersionInfo(const iox::cxx::Serialization& serial) noexcept;

  /// @brief Serialization of the VersionInfo.
  operator iox::cxx::Serialization() const noexcept;

  /// @brief Checks if the to versions information are identical.
  /// @param[in] rhs The right side of the compare with equal.
  bool operator==(const VersionInfo& rhs) const noexcept;

  /// @brief Checks if the to versions information are not identical.
  /// @param[in] rhs The right side of the compare with unequal.
  bool operator!=(const VersionInfo& rhs) const noexcept;

  /// @brief Compares this version versus another with respect to the compatibility value give.
  /// @param[in] other The other version compared with this version.
  /// @param[in] compatibilityCheckLevel Gives the level how deep it should be compared.
  bool CheckCompatibility(const VersionInfo& other,
      const ara::core::ipc::version::CompatibilityCheckLevel compatibilityCheckLevel) const noexcept;

  /// @brief The serialization could fail, which cause an invalid object, else true.
  /// @return Returns if the object is valid.
  bool IsValid() noexcept;

  /// @brief Creates a version object of the current iceoryx version.
  /// @return Returns the current version of iceoryx as an object.
  static VersionInfo GetCurrentVersion() noexcept;

 protected:
  static const uint64_t stkSerializationStringSize{100u};
  /// @brief String is alias of SerializationStringT.
  using SerializationStringT = iox::cxx::string<stkSerializationStringSize>;

  static_assert(
      COMMIT_ID_STRING_SIZE <= stkSerializationStringSize, "CommitId needs to transferred completely.");
  static_assert(
      COMMIT_ID_STRING_SIZE <= BUILD_DATE_STRING_SIZE, "BuildDate needs to transferred completely.");
  static_assert(iox::cxx::strlen2(IPC_BUILDDATE) < BUILD_DATE_STRING_SIZE,
      "COMMIT_BUILD_DATE_STRING_SIZE needs to be bigger.");

 protected:
  /// @brief Declaration of a valid.
  bool mValid{true};
  /// @brief Declaration of a mVersionMajor_.
  uint16_t mVersionMajor{0u};
  /// @brief Declaration of a mVersionMajor.
  uint16_t mVersionMinor{0u};
  /// @brief Declaration of a versionPatch.
  uint16_t mVersionPatch{0u};
  /// @brief Declaration of a versionTweak.
  uint16_t mVersionTweak{0u};
  /// @brief Declaration of a buildDateString.
  BuildDateStringT mBuildDateString;
  /// @brief Declaration of a commitIdString.
  CommitIdStringT mCommitIdString;
};

} // namespace version
} // namespace ipc
} // namespace core
} // namespace ara
#endif // UTILS_IPC_INC_VERSION_INFO_HPP
