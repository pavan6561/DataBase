// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : core_error_domain.h
//
// Purpose     : This file provides the implementation of class core error domain.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_CORE_ERROR_DOMAINN_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_CORE_ERROR_DOMAINN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

#include <cerrno>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief An enumeration with errors that can occur within this Functional Cluster.
/// @trace SWS_CORE_10901 SWS_CORE_10902 SWS_CORE_10903 SWS_CORE_10999 SWS_CORE_10900
/// @archtrace 609902
enum class CoreErrc : ErrorDomain::CodeType
{
  kInvalidArgument = 22, ///< invalid argument
  kInvalidMetaModelShortname = 137, ///< given string is not a valid model element shortname
  kInvalidMetaModelPath = 138, ///< missing or invalid path to model element
};

/// @brief Exception type thrown for CORE errors.
/// @trace SWS_CORE_00014
/// @archtrace 609902
/// @design 611548 614039
class CoreException : public Exception
{
 public:
  /// @brief Construct a new CoreException from an ErrorCode.
  /// @param[in] err The error code value..
  /// @trace SWS_CORE_00014
  /// @archtrace 609902
  /// @design 611549
  explicit CoreException(ErrorCode err) noexcept : Exception(err)
  {
  }
};

/// @brief An error domain for ara::core errors.
/// @trace SWS_CORE_00014 SWS_CORE_00011 SWS_CORE_10932 SWS_CORE_10931
/// @archtrace 609902
/// @design 611298
class CoreErrorDomain final : public ErrorDomain
{
  /// @brief To store the IdType.
  constexpr static ErrorDomain::IdType stkkId_{0x8000000000000014};

 public:
  /// @brief Alias for the error code value enumeration.
  /// @trace SWS_CORE_10933
  /// @archtrace 609902
  using Errc = CoreErrc;

  /// @brief Alias for the exception base class.
  /// @trace SWS_CORE_10934
  /// @archtrace 609902
  using Exception = CoreException;

  /// @brief Its a default constructor.
  /// @trace SWS_CORE_00014
  /// @archtrace 609902
  /// @design 611299
  constexpr CoreErrorDomain() noexcept : ErrorDomain(stkkId_)
  {
  }

  /// @brief Return the "shortname" ApApplicationErrorDomain.SN of this error domain.
  /// @returns Return the Name as "Core".
  /// @trace SWS_CORE_00014
  /// @archtrace 609902
  /// @design 611300
  char const* Name() const noexcept final
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    return "Core";
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }

  /// @brief Translate an error code value into a text message.
  /// @param[in] errorCode The error code value.
  /// @returns The text message never nullptr.
  /// @trace SWS_CORE_00014
  /// @archtrace 609902
  /// @design 611301
  char const* Message(ErrorDomain::CodeType errorCode) const noexcept final
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    char const* msg;
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
    Errc const code{static_cast<Errc>(errorCode)};
    switch (code)
    {
    case Errc::kInvalidArgument:
    {
      msg = "Invalid argument";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case Errc::kInvalidMetaModelShortname:
    {
      msg = "Invalid meta model shortname";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    case Errc::kInvalidMetaModelPath:
    {
      msg = "Invalid meta model path";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    default:
    {
      msg = "Unknown error";
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      break;
    }
    }
    return msg;
  }

  /// @brief Throw the exception type corresponding to the given ErrorCode.
  /// @param[in] errorCode Platform dependent error code to construct.
  /// @trace SWS_CORE_00014
  /// @archtrace 609902
  /// @design 611302
  void ThrowAsException(ErrorCode const& errorCode) const noexcept(false) final
  {
    ThrowOrTerminate<Exception>(errorCode);
  }
};
/// @brief namespace for internal.
namespace internal
{
/// @brief variable to store the CoreErrorDomain.
constexpr CoreErrorDomain g_coreErrorDomain;
} // namespace internal

/// @brief Return a reference to the global CoreErrorDomain.
/// @returns To return the CoreErrorDomain.
/// @trace SWS_CORE_00014 SWS_CORE_10982 SWS_CORE_10981 SWS_CORE_10980
/// @archtrace 609902
/// @design 611822
inline constexpr ErrorDomain const& GetCoreErrorDomain() noexcept
{
  return internal::g_coreErrorDomain;
}

/// @brief Create a new ErrorCode within CoreErrorDomain.
/// This function is used internally by constructors of ErrorCode. It is usually not
/// used directly by users.
/// @param[in] code The CoreErrorDomain-specific error code value.
/// @param[in] data Optional vendor-specific error data.
/// @returns A new ErrorCode instance.
/// @trace SWS_CORE_00014 SWS_CORE_10991 SWS_CORE_10990
/// @archtrace 609902
/// @design 611823
inline constexpr ErrorCode MakeErrorCode(CoreErrc code, ErrorDomain::SupportDataType data) noexcept
{
  return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetCoreErrorDomain(), data);
}

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_CORE_ERROR_DOMAINN_H_
// -----------------------------------------------------------------------------------------------------------
// - A5.1.1-2
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A5.1.1 required
//   Description:         Use of magic string literal.
//   Justification:       This will create another violation for initialization with magic string literal.This
//                        has been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
