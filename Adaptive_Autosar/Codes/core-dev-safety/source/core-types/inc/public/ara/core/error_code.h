// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : error_code.h
//
// Purpose     : This file provides the implementation of class error code.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_ERROR_CODE_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_ERROR_CODE_H_

#include "ara/core/error_domain.h"
#include "ara/core/string_view.h"

#include <ostream>
#include <utility>
#include <exception>
#include <type_traits>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Encapsulation of an error code.
/// @trace SWS_CORE_10300 SWS_CORE_00016
/// @archtrace 609899
/// @design 614039 611440
class ErrorCode
{
  /// @brief Definition for operator<<.
  /// @param[in] out A ostream parameter.
  /// @param[in] e ErrorCode of ErrorDomain.
  /// @returns the std::ostream value.
  friend std::ostream& operator<<(std::ostream& out, ErrorCode const& e)
  {
    return (out << e.mDomain_->Name() << ":" << e.mValue_ << ":" << e.mSupportData_);
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }

  /// @brief Alias name for ErrorDomain::CodeType.
  using CodeType = ErrorDomain::CodeType;

  /// @brief Alias name for ErrorDomain::SupportDataType.
  using SupportDataType = ErrorDomain::SupportDataType;
  /// @brief Alias name for ErrorDomain::IdType.
  using DomainIdType = ErrorDomain::IdType;

 public:
  /// @brief Construct a new ErrorCode instance with parameters.
  /// @tparam EnumT An enum type that contains error code values.
  /// @tparam E To contain the boolean value.
  /// @param[in] e A domain-specific error code value.
  /// @param[in] data Optional vendor-specific supplementary error context data.
  /// @trace SWS_CORE_10300
  /// @archtrace 609899
  /// @design 611441
  template <typename EnumT, typename E = typename std::enable_if<std::is_enum<EnumT>::value>::type>
  constexpr ErrorCode(EnumT e, SupportDataType data = 0) noexcept
      // Call MakeErrorCode() unqualified, so the correct overload is found via ADL.
      :
      ErrorCode(MakeErrorCode(e, data))
  {
  }

  /// @brief Construct a new ErrorCode instance with parameters.
  /// @param[in] value A domain-specific error code value.
  /// @param[in] domain The ErrorDomain associated with value.
  /// @param[in] data Optional vendor-specific supplementary error context data.
  /// @trace SWS_CORE_10300
  /// @archtrace 609899
  /// @design 611442
  constexpr ErrorCode(CodeType value, ErrorDomain const& domain, SupportDataType data = 0) noexcept :
      mValue_(value), mSupportData_(data), mDomain_(&domain), mDomainValue_(domain.Id())
  {
  }

  /// @brief Return the raw error code value.
  /// @returns the raw error code value.
  /// @trace SWS_CORE_10300
  /// @archtrace 609899
  /// @design 611443
  constexpr CodeType Value() const noexcept
  {
    return mValue_;
  }

  /// @brief Return the supplementary error context data.
  /// @returns the supplementary error context data.
  /// @trace SWS_CORE_10300
  /// @archtrace 609899
  /// @design 611444
  constexpr SupportDataType SupportData() const noexcept
  {
    return mSupportData_;
  }

  /// @brief Return the domain with which this ErrorCode is associated.
  /// @returns It return the ErrorDomain.
  /// @trace SWS_CORE_10300
  /// @archtrace 609899
  /// @design 611445
  constexpr ErrorDomain const& Domain() const noexcept
  {
    return *mDomain_;
  }

  /// @brief Return a textual representation of this ErrorCode.
  /// @returns the error message text.
  /// @trace SWS_CORE_10300
  /// @archtrace 609899
  /// @design 611446
  StringView Message() const noexcept
  {
    return Domain().Message(Value());
  }
  /// @brief Return a numeric representation of this ErrorCode.
  /// @returns It return a domain value.
  /// @trace SWS_CORE_10300
  /// @archtrace 609899
  /// @design 611447
  constexpr DomainIdType GetDomainId() const noexcept
  {
    return mDomainValue_;
  }
  /// @brief Throw this error as exception.
  /// @trace SWS_CORE_10300
  /// @archtrace 609899
  /// @design 611448
  [[noreturn]] void ThrowAsException() const noexcept(false)
  {
    Domain().ThrowAsException(*this);

    // Never reached, but apparently needed to avoid warnings from certain compilers (such as 5.4.0).
    std::terminate();
  }

 private:
  /// @brief To store the CodeType.
  CodeType mValue_;
  /// @brief To store the SupportDataType.
  SupportDataType mSupportData_;
  /// @brief To store the DomainIdType.
  DomainIdType mDomainValue_;
  /// @brief To store the ErrorDomain.
  ErrorDomain const* mDomain_; // non-owning pointer to the associated ErrorDomain
};

/// @brief Global operator== for ErrorCode.
/// @param[in] lhs The left hand side of the comparison.
/// @param[in] rhs The right hand side of the comparison.
/// @returns true if the two instances compare equal, false otherwise.
/// @trace SWS_CORE_10300
/// @archtrace 609899
/// @design 611824
constexpr inline bool operator==(ErrorCode const& lhs, ErrorCode const& rhs)
{
  return (lhs.Domain() == rhs.Domain()) && (lhs.Value() == rhs.Value());
}

/// @brief Global operator!= for ErrorCode.
/// @param[in] lhs The left hand side of the comparison.
/// @param[in] rhs The right hand side of the comparison.
/// @returns true if the two instances compare not equal, false otherwise.
/// @trace SWS_CORE_10300
/// @archtrace 609899
/// @design 611825
constexpr inline bool operator!=(ErrorCode const& lhs, ErrorCode const& rhs)
{
  return (lhs.Domain() != rhs.Domain()) || (lhs.Value() != rhs.Value());
}

/// @brief Throw the exception type corresponding to the given ErrorCode.
/// @tparam ExceptionType An enum type that contains error code values.
/// @param[in] errorCode The Error Code instance.
/// @trace SWS_CORE_10300
/// @archtrace 609899
/// @design 611826
template <typename ExceptionType>
void ThrowOrTerminate(ErrorCode errorCode)
{
#ifndef ARA_NO_EXCEPTIONS
  throw ExceptionType(std::move(errorCode));
#else
  (void)errorCode;
  std::terminate();
#endif
}

/// @brief namespace ifc.
namespace ifc
{
/// @brief Helper function for other Function Cluster's error domains.
/// @tparam ExceptionType An enum type that contains error code values.
/// @param[in] errorCode The Error Code instance.
/// @trace SWS_CORE_10300
/// @archtrace 609899
/// @design 611942
template <typename ExceptionType>
[[noreturn]] void ThrowOrTerminate(ErrorCode errorCode) {
#ifndef ARA_NO_EXCEPTIONS
  throw ExceptionType(std::move(errorCode));
#else
  (void)errorCode;
  std::terminate();
#endif
}
} // namespace ifc

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_ERROR_CODE_H_
