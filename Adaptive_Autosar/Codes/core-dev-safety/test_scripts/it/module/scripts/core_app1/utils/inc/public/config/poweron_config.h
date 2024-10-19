// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : poweron_config.h
//
// Purpose     : Header file to store data, parsed from json file.
// -----------------------------------------------------------------------------------------------------------

#ifndef SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_POWERON_CONFIG_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_POWERON_CONFIG_H_

#include <ara/core/string.h>
#include <ara/core/fixed_string.h>
#include "sm_config_macros.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief State Manager.
namespace sm
{
/// @brief Config.
namespace config
{
/// @brief Class used to store the PowerOnFunctionGrp in sm configuration.
/// @archtrace 510859
/// @design 529461
class PowerOnFunctionGrp final
{
 public:
  /// @brief Default constructor of the PowerOnFunctionGrp class.
  /// @archtrace 510859
  /// @design 529462
  PowerOnFunctionGrp() noexcept
  {
  }

  /// @brief Destructor of the PowerOnFunctionGrp class.
  /// @archtrace 510859
  /// @design 529463
  ~PowerOnFunctionGrp()
  {
  }

  /// @brief PowerOnFunctionGrp copy constructor.
  PowerOnFunctionGrp(PowerOnFunctionGrp const&) = default;

  /// @brief PowerOnFunctionGrp move constructor.
  PowerOnFunctionGrp(PowerOnFunctionGrp&&) noexcept = default;

  /// @brief PowerOnFunctionGrp copy assignment.
  PowerOnFunctionGrp& operator=(PowerOnFunctionGrp const&) = delete;

  /// @brief PowerOnFunctionGrp move assignment.
  PowerOnFunctionGrp& operator=(PowerOnFunctionGrp&&) noexcept = default;

  /// @brief Used to store the function group name.
  ara::core::FixedString<ara::sm::kSmMaxFgName> function_group_name;

  /// @brief Used to store the function group state.
  ara::core::FixedString<ara::sm::kSmMaxFgState> function_group_state;
};

} // namespace config
} // namespace sm
} // namespace ara

#endif // SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_POWERON_CONFIG_H_
