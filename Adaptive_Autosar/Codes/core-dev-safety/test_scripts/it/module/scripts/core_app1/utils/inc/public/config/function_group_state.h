// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : function_group_state.h
//
// Purpose     : Header file to store data, parsed from json file.
// -----------------------------------------------------------------------------------------------------------

#ifndef SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_FUNCTION_GROUP_STATE_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_FUNCTION_GROUP_STATE_H_

#include <ara/core/string.h>
#include <ara/core/fixed_string.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief State Manager.
namespace sm
{
/// @brief Config.
namespace config
{
/// @brief Class used to store the FunctionGroup in sm configuration.
/// @archtrace 510858
/// @design 529452
class FunctionGroup final
{
 public:
  /// @brief Default constructor of the FunctionGroup class.
  /// @archtrace 510858
  /// @design 529453
  FunctionGroup() noexcept
  {
  }

  /// @brief Destructor of the FunctionGroup class.
  /// @archtrace 510858
  /// @design 529454
  ~FunctionGroup()
  {
  }

  /// @brief FunctionGroup copy constructor.
  FunctionGroup(FunctionGroup const&) = default;

  /// @brief FunctionGroup move constructor.
  FunctionGroup(FunctionGroup&&) noexcept = default;

  /// @brief FunctionGroup copy assignment.
  FunctionGroup& operator=(FunctionGroup const&) = default;

  /// @brief FunctionGroup move assignment.
  FunctionGroup& operator=(FunctionGroup&&) noexcept = default;

  /// @brief Used to store the function group name.
  ara::core::FixedString<ara::sm::kSmMaxFgName> function_group_name;

  /// @brief Used to store the function group state.
  ara::core::FixedString<ara::sm::kSmMaxFgState> function_group_state;
};

} // namespace config
} // namespace sm
} // namespace ara

#endif // SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_FUNCTION_GROUP_STATE_H_
