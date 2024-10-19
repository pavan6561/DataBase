// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : security_error_domain.h
//
// Purpose     : This file contains definitions for SecurityErrorDomain class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_SECURITY_ERROR_DOMAIN_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_SECURITY_ERROR_DOMAIN_H_

#include <ara/core/error_code.h>
#include <ara/core/exception.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief Enumeration of all Security Error Code values that may be reported by @c ara::crypto.
/// @trace SWS_CRYPT_10099
enum class SecurityErrc : ara::core::ErrorDomain::CodeType
{
  /// Reserved (a multiplier of error class IDs)
  kErrorClass = 0x1000000,
  /// Reserved (a multiplier of error sub-class IDs)
  kErrorSubClass = 0x10000,
  /// Reserved (a multiplier of error sub-sub-class IDs)
  kErrorSubSubClass = 0x100,

  /// ResourceException: Generic resource fault!
  kResourceFault = 1 * kErrorClass,
  /// ResourceException: Specified resource is busy!
  kBusyResource = kResourceFault + 1,
  /// ResourceException: Insufficient capacity of specified resource!
  kInsufficientResource = kResourceFault + 2,
  /// ResourceException: Specified resource was not reserved!
  kUnreservedResource = kResourceFault + 3,
  /// ResourceException: Specified resource has been modified!
  kModifiedResource = kResourceFault + 4,

  /// LogicException: Generic logic fault!
  kLogicFault = 2 * kErrorClass,
  /// InvalidArgumentException: An invalid argument value is provided!
  kInvalidArgument = kLogicFault + 1 * kErrorSubClass,
  /// InvalidArgumentException: Unknown identifier is provided!
  kUnknownIdentifier = kInvalidArgument + 1,
  /// InvalidArgumentException: Insufficient capacity of the output buffer!
  kInsufficientCapacity = kInvalidArgument + 2,
  /// InvalidArgumentException: Invalid size of an input buffer!
  kInvalidInputSize = kInvalidArgument + 3,
  /// InvalidArgumentException: Provided values of arguments are incompatible!
  kIncompatibleArguments = kInvalidArgument + 4,
  /// InvalidArgumentException: Input and output buffers are intersect!
  kInOutBuffersIntersect = kInvalidArgument + 5,
  /// InvalidArgumentException: Provided value is below the lower boundary!
  kBelowBoundary = kInvalidArgument + 6,
  /// InvalidArgumentException: Provided value is above the upper boundary!
  kAboveBoundary = kInvalidArgument + 7,
  /// AuthTagNotValidException: Provided authentication-tag cannot be verified!
  kAuthTagNotValid = kInvalidArgument + 8,

  /// UnsupportedException: Unsupported request (due to limitations of the implementation)!
  kUnsupported = kInvalidArgument + 1 * kErrorSubSubClass,

  /// InvalidUsageOrderException: Invalid usage order of the interface!
  kInvalidUsageOrder = kLogicFault + 2 * kErrorSubClass,
  /// InvalidUsageOrderException: Context of the interface was not initialized!
  kUninitializedContext = kInvalidUsageOrder + 1,
  /// InvalidUsageOrderException: Data processing was not started yet!
  kProcessingNotStarted = kInvalidUsageOrder + 2,
  /// InvalidUsageOrderException: Data processing was not finished yet!
  kProcessingNotFinished = kInvalidUsageOrder + 3,

  /// RuntimeException: Generic runtime fault!
  kRuntimeFault = 3 * kErrorClass,
  /// RuntimeException: Unsupported serialization format for this object type!
  kUnsupportedFormat = kRuntimeFault + 1,
  /// RuntimeException: Operation is prohibitted due to a risk of a brute force attack!
  kBruteForceRisk = kRuntimeFault + 2,
  /// RuntimeException: The operation violates content restrictions of the target container!
  kContentRestrictions = kRuntimeFault + 3,
  /// RuntimeException: Incorrect reference between objects!
  kBadObjectReference = kRuntimeFault + 4,
  /// RuntimeException: Provided content already exists in the target storage!
  kContentDuplication = kRuntimeFault + 6,

  /// UnexpectedValueException: Unexpected value of an argument is provided!
  kUnexpectedValue = kRuntimeFault + 1 * kErrorSubClass,
  /// UnexpectedValueException: The provided object is incompatible with requested operation or its
  /// configuration!
  kIncompatibleObject = kUnexpectedValue + 1,
  /// UnexpectedValueException: Incomplete state of an argument!
  kIncompleteArgState = kUnexpectedValue + 2,
  /// UnexpectedValueException: Specified container is empty!
  kEmptyContainer = kUnexpectedValue + 3,
  /// kMissingArgumentException: Expected argument, but none provided!
  kMissingArgument = kUnexpectedValue + 4,

  /// BadObjectTypeException: Provided object has unexpected type!
  kBadObjectType = kUnexpectedValue + 1 * kErrorSubSubClass,

  /// UsageViolationException: Violation of allowed usage for the object!
  kUsageViolation = kRuntimeFault + 2 * kErrorSubClass,

  /// AccessViolationException: Access rights violation!
  kAccessViolation = kRuntimeFault + 3 * kErrorSubClass,

};

/// @brief Exception type thrown for CRYPTO errors.
/// @trace SWS_CRYPT_19905
class SecurityException : public ara::core::Exception
{
 public:
  /// @brief Construct a new SecurityException from an ErrorCode.
  /// @param err  the ErrorCode
  /// @trace SWS_CRYPT_19906
  explicit SecurityException(ara::core::ErrorCode err) noexcept : ara::core::Exception(err)
  {
  }
};

/// @brief Security Error Domain class that provides interfaces as defined by ara::core::ErrorDomain such as
/// a name of the Security Error Domain or messages for each error code.
/// This class represents an error domain responsible for all errors that may be reported by
/// public APIs in @c ara::crypto namespace.
/// @domainid{0x8000'0000'0000'0801}
/// @trace SWS_CRYPT_19900
class SecurityErrorDomain final : public ara::core::ErrorDomain
{
  constexpr static ara::core::ErrorDomain::IdType kId{ 100 };

 public:
  /// @brief security error
  /// @trace SWS_CRYPT_19903
  using Errc = SecurityErrc;

  /// @brief Alias for the exception base class
  /// @trace SWS_CRYPT_19904
  using Exception = SecurityException;

  /// @brief Ctor of the SecurityErrorDomain
  /// @trace SWS_CRYPT_19902
  constexpr SecurityErrorDomain() noexcept : ara::core::ErrorDomain(kId)
  {
  }

  /// @brief returns Text "Security"
  /// @returns "Security" text
  /// @trace SWS_CRYPT_19950
  const char* Name() const noexcept final
  {
    return "Security";
  }

  /// @brief Translate an error code value into a text message.
  /// @param[in] errorCode  an error code identifier from the @c SecurityErrc enumeration
  /// @returns message text of error code
  /// @trace SWS_CRYPT_19953
  const char* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept final
  {
    Errc const kCode{ static_cast<Errc>(errorCode) };
    ara::core::String errorMessage;
    switch (kCode)
    {
      case Errc::kResourceFault:
        errorMessage = "Generic resource fault!";
        break;

      case Errc::kBusyResource:
        errorMessage = "Specified resource is busy!";
        break;

      case Errc::kInsufficientResource:
        errorMessage = "Insufficient capacity of specified resource!";
        break;

      case Errc::kUnreservedResource:
        errorMessage = "Specified resource was not reserved!";
        break;

      case Errc::kLogicFault:
        errorMessage = "Generic logic fault!";
        break;

      case Errc::kInvalidArgument:
        errorMessage = "An invalid argument value is provided!";
        break;

      case Errc::kUnknownIdentifier:
        errorMessage = "Unknown identifier is provided!";
        break;

      case Errc::kInsufficientCapacity:
        errorMessage = "Insufficient capacity of the output buffer!";
        break;

      case Errc::kInvalidInputSize:
        errorMessage = "Invalid size of an input buffer!";
        break;

      case Errc::kIncompatibleArguments:
        errorMessage = "Provided values of arguments are incompatible!";
        break;

      case Errc::kInOutBuffersIntersect:
        errorMessage = "Input and output buffers are intersect!";
        break;

      case Errc::kBelowBoundary:
        errorMessage = "Provided value is below the lower boundary!";
        break;

      case Errc::kAboveBoundary:
        errorMessage = "Provided value is above the upper boundary!";
        break;

      case Errc::kUnsupported:
        errorMessage = "Unsupported request (due to limitations of the implementation)!";
        break;

      case Errc::kInvalidUsageOrder:
        errorMessage = "Invalid usage order of the interface!";
        break;

      case Errc::kUninitializedContext:
        errorMessage = "Context of the interface was not initialized!";
        break;

      case Errc::kProcessingNotStarted:
        errorMessage = "Data processing was not started yet!";
        break;

      case Errc::kProcessingNotFinished:
        errorMessage = "Data processing was not finished yet!";
        break;

      case Errc::kRuntimeFault:
        errorMessage = "Generic runtime fault!";
        break;

      case Errc::kUnsupportedFormat:
        errorMessage = "Unsupported serialization format for this object type!";
        break;

      case Errc::kBruteForceRisk:
        errorMessage = "Operation is prohibitted due to a risk of a brute force attack!";
        break;

      case Errc::kContentRestrictions:
        errorMessage = "The operation violates content restrictions of the target container!";
        break;

      case Errc::kBadObjectReference:
        errorMessage = "Incorrect reference between objects!";
        break;

      case Errc::kContentDuplication:
        errorMessage = "Provided content already exists in the target storage!";
        break;

      case Errc::kUnexpectedValue:
        errorMessage = "Unexpected value of an argument is provided!";
        break;

      case Errc::kIncompatibleObject:
        errorMessage = "The provided object is incompatible with requested operation or its configuration!";
        break;

      case Errc::kIncompleteArgState:
        errorMessage = "Incomplete state of an argument!";
        break;

      case Errc::kEmptyContainer:
        errorMessage = "Specified container is empty!";
        break;

      case Errc::kBadObjectType:
        errorMessage = "Provided object has unexpected type!";
        break;

      case Errc::kUsageViolation:
        errorMessage = "Violation of allowed usage for the object!";
        break;

      case Errc::kAccessViolation:
        errorMessage = "Access rights violation!";
        break;

      default:
        errorMessage = "Unknown error!";
        break;
    }

    return errorMessage.c_str();
  }

  /// @brief throws exception of error code
  /// @param[in] errorCode  an error code identifier from the @c SecurityErrc enumeration
  /// @trace SWS_CRYPT_19954
  void ThrowAsException(const ara::core::ErrorCode& errorCode) const final
  {
    ara::core::ThrowOrTerminate<Exception>(errorCode);
  }
};

namespace internal
{
static const SecurityErrorDomain g_cryptoErrorDomain;
}

/// @brief Return a reference to the global CryptoErrorDomain.
/// @returns the CryptoErrorDomain
/// @trace SWS_CRYPT_19952
constexpr const ara::core::ErrorDomain& GetCryptoErrorDomain() noexcept
{
  return internal::g_cryptoErrorDomain;
}

/// @brief Makes Error Code instances from the Security Error Domain.
///  The returned @c ErrorCode instance always references to @c SecurityErrorDomain.
/// @param[in] code  an error code identifier from the @c SecurityErrc enumeration
/// @param[in] data  supplementary data for the error description
/// @returns an instance of @c ErrorCode created according the arguments
/// @trace SWS_CRYPT_19951
constexpr ara::core::ErrorCode MakeErrorCode(
  SecurityErrorDomain::Errc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
  return ara::core::ErrorCode(
    static_cast<ara::core::ErrorDomain::CodeType>(code), GetCryptoErrorDomain(), data);
}

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_SECURITY_ERROR_DOMAIN_H_
