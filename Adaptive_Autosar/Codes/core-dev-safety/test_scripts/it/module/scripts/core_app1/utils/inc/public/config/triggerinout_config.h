// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : triggerinout_config.h
//
// Purpose     : Header file to store data, parsed from json file.
// -----------------------------------------------------------------------------------------------------------

#ifndef SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_TRIGGERINOUT_CONFIG_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_TRIGGERINOUT_CONFIG_H_

#include <ara/core/string.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief State Manager.
namespace sm
{
/// @brief config.
namespace config
{
/// @brief Class used to store the TriggerInOut in sm configuration.
/// @archtrace 512280
/// @design 529473
class TriggerInOut final
{
 public:
  /// @brief Default constructor of the TriggerInOut class.
  /// @archtrace 512280
  /// @design 529474
  TriggerInOut() noexcept
  {
  }

  /// @brief Destructor of the TriggerInOut class.
  /// @archtrace 512280
  /// @design 529475
  ~TriggerInOut()
  {
  }

  /// @brief TriggerInOut copy constructor.
  TriggerInOut(TriggerInOut const&) = default;

  /// @brief TriggerInOut move constructor.
  TriggerInOut(TriggerInOut&&) noexcept = default;

  /// @brief TriggerInOut copy assignment.
  TriggerInOut& operator=(TriggerInOut const&) = delete;

  /// @brief TriggerInOut move assignment.
  TriggerInOut& operator=(TriggerInOut&&) noexcept = default;

  /// @brief Used to store the function group name.
  ara::core::FixedString<ara::sm::kSmMaxInstanceId> instanceSpecifier;

  /// @brief Used to store the function group state.
  ara::core::FixedString<ara::sm::kSmMaxFgName> functionGroupName;
};

} // namespace config
} // namespace sm
} // namespace ara

#endif // SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_TRIGGERINOUT_CONFIG_H_