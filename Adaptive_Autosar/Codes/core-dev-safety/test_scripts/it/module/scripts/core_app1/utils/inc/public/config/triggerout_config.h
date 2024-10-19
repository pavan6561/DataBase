// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : triggerout_config.h
//
// Purpose     : Header file to store data, parsed from json file.
// -----------------------------------------------------------------------------------------------------------

#ifndef SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_TRIGGEROUT_CONFIG_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_TRIGGEROUT_CONFIG_H_

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
/// @brief Class used to store the TriggerOut in sm configuration.
/// @archtrace 511920
/// @design 529476
class TriggerOut final
{
 public:
  /// @brief Default constructor of the TriggerOut class.
  /// @archtrace 511920
  /// @design 529477
  TriggerOut() noexcept
  {
  }

  /// @brief Destructor of the TriggerOut class.
  /// @archtrace 511920
  /// @design 529478
  ~TriggerOut()
  {
  }

  /// @brief TriggerOut copy constructor.
  TriggerOut(TriggerOut const&) = default;

  /// @brief TriggerOut move constructor.
  TriggerOut(TriggerOut&&) noexcept = default;

  /// @brief TriggerOut copy assignment.
  TriggerOut& operator=(TriggerOut const&) = delete;

  /// @brief TriggerOut move assignment.
  TriggerOut& operator=(TriggerOut&&) noexcept = default;

  /// @brief Used to store the function group name.
  ara::core::FixedString<ara::sm::kSmMaxInstanceId> instanceSpecifier;

  /// @brief Used to store the function group state.
  ara::core::FixedString<ara::sm::kSmMaxFgName> functionGroupName;
};

} // namespace config
} // namespace sm
} // namespace ara

#endif // SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_TRIGGEROUT_CONFIG_H_