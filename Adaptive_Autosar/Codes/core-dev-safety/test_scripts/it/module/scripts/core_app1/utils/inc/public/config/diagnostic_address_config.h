// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : diagnostic_address_config.h
//
// Purpose     : Header file to store data, parsed from json file.
// -----------------------------------------------------------------------------------------------------------

#ifndef SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_DIAGNOSTIC_ADDRESS_CONFIG_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_DIAGNOSTIC_ADDRESS_CONFIG_H_

#include <ara/core/string.h>
#include <ara/core/vector.h>
#include "function_group_state.h"
#include <ara/core/fixed_vector.h>
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
/// @brief Class used to store the DiagnosticAddress in sm configuration.
/// @archtrace 510853
/// @design 529449
class DiagnosticAddress final
{
 public:
  /// @brief Constructor of the DiagnosticAddress class.
  /// @archtrace 510853
  /// @design 529450
  DiagnosticAddress() noexcept : address{""}, functionGrp{}
  {
  }

  /// @brief Destructor of the DiagnosticAddress class.
  /// @archtrace 510853
  /// @design 529451
  ~DiagnosticAddress()
  {
  }

  /// @brief DiagnosticAddress copy constructor.
  DiagnosticAddress(DiagnosticAddress const&) = default;

  /// @brief DiagnosticAddress move constructor.
  DiagnosticAddress(DiagnosticAddress&&) noexcept = default;

  /// @brief DiagnosticAddress copy assignment.
  DiagnosticAddress& operator=(DiagnosticAddress const&) = delete;

  /// @brief DiagnosticAddress move assignment.
  DiagnosticAddress& operator=(DiagnosticAddress&&) noexcept = default;

  /// @brief Used to store the diagnostic address.
  ara::core::FixedString<ara::sm::kSmDiagAddress> address;

  /// @brief Used to store the FunctionGroup.
  ara::core::FixedVector<ara::sm::config::FunctionGroup, ara::sm::kSmMaxFgGroup> functionGrp;
};

} // namespace config
} // namespace sm
} // namespace ara

#endif // SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_DIAGNOSTIC_ADDRESS_CONFIG_H_
