// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : powermode_config.h
//
// Purpose     : Header file to store data, parsed from json file.
// -----------------------------------------------------------------------------------------------------------

#ifndef SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_POWERMODE_CONFIG_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_POWERMODE_CONFIG_H_

#include <ara/core/string.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief State Manager.
namespace sm
{
/// @brief Config.
namespace config
{
/// @brief Class used to store the PowerModes in sm configuration.
/// @archtrace 510859
/// @design 529458
class PowerModes final
{
 public:
  /// @brief Default constructor of the PowerModes class.
  /// @archtrace 510859
  /// @design 529459
  PowerModes() noexcept
  {
  }

  /// @brief Destructor of the PowerModes class.
  /// @archtrace 510875
  /// @design 529460
  ~PowerModes()
  {
  }

  /// @brief PowerModes copy constructor.
  PowerModes(PowerModes const&) = default;

  /// @brief PowerModes move constructor.
  PowerModes(PowerModes&&) noexcept = default;

  /// @brief PowerModes copy assignment.
  PowerModes& operator=(PowerModes const&) = delete;

  /// @brief PowerModes move assignment.
  PowerModes& operator=(PowerModes&&) noexcept = default;

  /// @brief Used to store the function group name.
  ara::core::FixedString<ara::sm::kSmMaxInstanceId> instance_id;

  /// @brief Used to store the function group state.
  ara::core::FixedString<ara::sm::kSmMaxFgName> function_group_name;
};

} // namespace config
} // namespace sm
} // namespace ara

#endif // SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_POWERMODE_CONFIG_H_
