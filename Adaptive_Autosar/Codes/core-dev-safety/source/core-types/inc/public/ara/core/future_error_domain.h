// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : future_error_domain.h
//
// Purpose     : This file provides the implementation of class future error domain.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FUTURE_ERROR_DOMAIN_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FUTURE_ERROR_DOMAIN_H_

#include "ara/core/error_domain.h"
#include "ara/core/error_code.h"
#include "ara/core/exception.h"

#include <cstdint>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Specifies the types of internal errors that can occur upon
/// calling Future::get or Future::GetResult.
/// These definitions are equivalent to the ones from std::FutureErrc.
/// @trace SWS_CORE_00013 SWS_CORE_10999
/// @archtrace 609889
enum class FutureErrc : int32_t
{
  kBrokenPromise = 101, ///< the asynchronous task abandoned its shared state
  kFutureAlreadyRetrieved = 102, ///< the contents of the shared state were already accessed
  kPromiseAlreadySatisfied = 103, ///< attempt to store a value into the shared state twice
  kNoState = 104, ///< attempt to access Promise or Future without an associated state
};

/// @brief Exception type thrown by Future and Promise classes.
/// @trace SWS_CORE_00013
/// @archtrace 609889
/// @design 611554
class FutureException : public Exception
{
 public:
  /// @brief Construct a new FutureException from an ErrorCode.
  /// @param[in] err The error code value.
  /// @trace SWS_CORE_00013
  /// @archtrace 609889
  /// @design 611555
  explicit FutureException(ErrorCode err) noexcept : Exception(std::move(err))
  {
  }
};

/// @brief Error domain for errors originating from classes Future and Promise.
/// @trace SWS_CORE_00013
/// @archtrace 609889
/// @design 611806 614039
class FutureErrorDomain final : public ErrorDomain
{
  constexpr static ErrorDomain::IdType stkkId_{0x8000000000000013};

 public:
  /// @brief Alias for the error code value enumeration.
  /// @trace SWS_CORE_00013
  /// @archtrace 609889
  using Errc = FutureErrc;

  /// @brief Alias for the exception base class.
  /// @trace SWS_CORE_00013
  /// @archtrace 609889
  using Exception = FutureException;

  /// @brief It is an default constructor.
  /// @trace SWS_CORE_00013
  /// @archtrace 609889
  /// @design 611807
  constexpr FutureErrorDomain() noexcept : ErrorDomain(stkkId_)
  {
  }

  /// @brief Return the "shortname" ApApplicationErrorDomain.SN of this error domain.
  /// @returns "Future".
  /// @trace SWS_CORE_00013
  /// @archtrace 609889
  /// @design 611808
  char const* Name() const noexcept final
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    return "Future";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }

  /// @brief Translate an error code value into a text message.
  /// @param[in] errorCode The error code value.
  /// @returns the text message, never nullptr.
  /// @trace SWS_CORE_00013
  /// @archtrace 609889
  /// @design 611809
  char const* Message(ErrorDomain::CodeType errorCode) const noexcept final
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    char const* msg;
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
    Errc const code{static_cast<Errc>(errorCode)};
    switch (code)
    {
    case Errc::kBrokenPromise:
    {
      msg = "broken promise";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case Errc::kFutureAlreadyRetrieved:
    {
      msg = "future already retrieved";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case Errc::kPromiseAlreadySatisfied:
    {
      msg = "promise already satisfied";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case Errc::kNoState:
    {
      msg = "no state associated with this future";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    default:
    {
      msg = "unknown future error";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    }
    return msg;
  }

  /// @brief Throw the exception type corresponding to the given ErrorCode.
  /// @param[in] errorCode The Error Code instance.
  /// @trace SWS_CORE_00013
  /// @archtrace 609889
  /// @design 611810
  void ThrowAsException(ErrorCode const& errorCode) const noexcept(false) final
  {
    ThrowOrTerminate<Exception>(errorCode);
  }
};

namespace internal
{
constexpr FutureErrorDomain g_futureErrorDomain;
}

/// @brief Obtain the reference to the single global FutureErrorDomain instance.
/// @returns reference to the FutureErrorDomain instance.
/// @trace SWS_CORE_00013
/// @archtrace 609889
/// @design 611843
inline constexpr ErrorDomain const& GetFutureErrorDomain() noexcept
{
  return internal::g_futureErrorDomain;
}

/// @brief Create a new ErrorCode for FutureErrorDomain with the
/// given support data type and message.
/// @param[in] code An enumeration value from FutureErrc.
/// @param[in] data A vendor-defined supplementary value.
/// @returns the new ErrorCode instance
/// @trace SWS_CORE_00013
/// @archtrace 609889
/// @design 611844
inline constexpr ErrorCode MakeErrorCode(FutureErrc code, ErrorDomain::SupportDataType data) noexcept
{
  return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetFutureErrorDomain(), data);
}

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FUTURE_ERROR_DOMAIN_H_
// -----------------------------------------------------------------------------------------------------------
// - A5.1.1-2
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A5.1.1 required
//   Description:         Use of magic string literal.
//   Justification:       This will create another violation for initialization with magic string literal.This
//                        has been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
