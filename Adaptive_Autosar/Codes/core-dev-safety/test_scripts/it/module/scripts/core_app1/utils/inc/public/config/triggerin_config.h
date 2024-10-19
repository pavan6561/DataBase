// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : triggerin_config.h
//
// Purpose     : Header file to store data, parsed from json file.
// -----------------------------------------------------------------------------------------------------------

#ifndef SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_TRIGGERIN_CONFIG_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_TRIGGERIN_CONFIG_H_

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
/// @brief Class used to store the TriggerIn in sm configuration.
/// @archtrace 511921
/// @design 529470
class TriggerIn final
{
 public:
  /// @brief Default constructor of the TriggerIn class.
  /// @archtrace 511921
  /// @design 529471
  TriggerIn() noexcept
  {
  }

  /// @brief Destructor of the TriggerIn class.
  /// @archtrace 511921
  /// @design 529472
  ~TriggerIn()
  {
  }

  /// @brief TriggerIn copy constructor.
  TriggerIn(TriggerIn const&) = default;

  /// @brief TriggerIn move constructor.
  TriggerIn(TriggerIn&&) noexcept = default;

  /// @brief TriggerIn copy assignment.
  TriggerIn& operator=(TriggerIn const&) = delete;

  /// @brief TriggerIn move assignment.
  TriggerIn& operator=(TriggerIn&&) noexcept = default;

  /// @brief Used to store the function group name.
  ara::core::FixedString<ara::sm::kSmMaxInstanceId> instanceSpecifier;

  /// @brief Used to store the function group state.
  ara::core::FixedString<ara::sm::kSmMaxFgName> functionGroupName;
};

} // namespace config
} // namespace sm
} // namespace ara

#endif // SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_TRIGGERIN_CONFIG_H_
