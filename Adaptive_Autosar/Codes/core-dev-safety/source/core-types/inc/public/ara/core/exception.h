// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : exception.h
//
// Purpose     : This file provides the implementation of class exception.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_EXCEPTIONS_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_EXCEPTIONS_H_

#include "ara/core/error_code.h"

#include <exception>
#include <system_error>
#include <utility>
#include <ostream>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Base type for all AUTOSAR exception types.
/// @trace SWS_CORE_S7.2.1.5_P1 SWS_CORE_10910 SWS_CORE_10911 SWS_CORE_10912 SWS_CORE_00004 SWS_CORE_00005
/// @archtrace 609901
/// @design 611550 614039
class Exception : public std::exception
{
  /// @brief Variable to store the ErrorCode.
  ErrorCode const stkMErrorCode_;

 public:
  /// @brief Construct a new Exception object with a specific ErrorCode.
  /// @param[in] err It is an error code value.
  /// @trace SWS_CORE_S7.2.1.5_P1
  /// @archtrace 609901
  /// @design 611551
  explicit Exception(ErrorCode err) noexcept : stkMErrorCode_(std::move(err))
  {
  }

  /// @brief Return the embedded ErrorCode that was given to the constructor.
  /// @returns reference to the embedded ErrorCode.
  /// @trace SWS_CORE_S7.2.1.5_P1
  /// @archtrace 609901
  /// @design 611552
  ErrorCode const& Error() const noexcept
  {
    return stkMErrorCode_;
  }

  /// @brief Return the explanatory string.
  /// @returns a null-terminated string.
  /// @trace SWS_CORE_S7.2.1.5_P1
  /// @archtrace 609901
  /// @design 611553
  char const* what() const noexcept override
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    return std::exception::what();
  }

 protected:
  /// @brief Copy assign from another instance.
  /// @param[in] other The other instance.
  /// @trace SWS_CORE_S7.2.1.5_P1
  /// @archtrace 609901
  Exception& operator=(Exception const& other) = default;
};

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_EXCEPTIONS_H_
