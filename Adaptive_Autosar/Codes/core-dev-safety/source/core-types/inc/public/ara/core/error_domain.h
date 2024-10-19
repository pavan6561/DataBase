// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : error_domain.h
//
// Purpose     : This file provides the implementation of class error domain.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_ERROR_DOMAIN_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_ERROR_DOMAIN_H_

#include <cstdint>
#include <exception>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

// forward declaration
class ErrorCode;

/// @brief Encapsulation of an error domain.
/// @trace SWS_CORE_10400 SWS_CORE_10930
/// @archtrace 609900
/// @design 611290 614039
class ErrorDomain
{
 public:
  /// @brief Alias type for a unique ErrorDomain identifier type.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  using IdType = std::uint64_t;
  /// @brief Alias type for a domain-specific error code value.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  using CodeType = std::int32_t;
  /// @brief Alias type for vendor-specific supplementary data.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  using SupportDataType = std::int32_t;

  /// @brief Copy construction shall be disabled.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  ErrorDomain(ErrorDomain const&) = delete;
  /// @brief Move construction shall be disabled.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  ErrorDomain(ErrorDomain&&) = delete;
  /// @brief Copy assignment shall be disabled.
  /// @param[in] id the unique identifier.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  ErrorDomain& operator=(ErrorDomain const&) = delete;
  /// @brief Move assignment shall be disabled.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  ErrorDomain& operator=(ErrorDomain&&) = delete;

  /// @brief Return the name of this error domain.
  /// @returns the name as a null-terminated string, never nullptr.
  /// @trace SWS_CORE_10400 SWS_CORE_10951
  /// @archtrace 609900
  /// @design 611291
  virtual char const* Name() const noexcept = 0;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Return a textual representation of the given error code.
  /// @param[in] errorCode The domain-specific error code.
  /// @returns the text as a null-terminated string, never nullptr.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  /// @design 611292
  virtual char const* Message(CodeType errorCode) const noexcept = 0;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Throws the given errorCode as Exception.
  /// @param[in] errorCode Platform dependent error code to construct.
  /// @remark if ARA_NO_EXCEPTIONS is defined, this function call will terminate.
  /// @trace SWS_CORE_10400 SWS_CORE_10950 SWS_CORE_10953
  /// @archtrace 609900
  /// @design 611293
  [[noreturn]] virtual void ThrowAsException(ErrorCode const& errorCode) const noexcept(false) = 0;

  /// @brief Return the unique domain identifier.
  /// @returns The error domain Id.
  /// @trace SWS_CORE_10400 SWS_CORE_10952 SWS_CORE_00010 SWS_CORE_00011
  /// @archtrace 609900
  /// @design 611294
  constexpr IdType Id() const noexcept
  {
    return stkMId_;
  }

  /// @brief Compare for equality with another ErrorDomain instance.
  /// @param[in] other The other instance.
  /// @returns true if other is equal to this, false otherwise.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  /// @design 611295
  constexpr bool operator==(ErrorDomain const& other) const noexcept
  {
    return stkMId_ == other.stkMId_;
  }

  /// @brief Compare for non-equality with another ErrorDomain instance.
  /// @param[in] other The other instance.
  /// @returns true if other is not equal to this, false otherwise
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  /// @design 611296
  constexpr bool operator!=(ErrorDomain const& other) const noexcept
  {
    return stkMId_ != other.stkMId_;
  }

 protected:
  /// @brief Construct a new instance with the given identifier.
  /// @param[in] id The unique identifier.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  /// @design 611297
  constexpr explicit ErrorDomain(IdType id) noexcept : stkMId_(id)
  {
  }

  /// @brief Destructor.
  /// This dtor is non-virtual (and trivial) so that this class is a literal type.
  /// While we do have virtual functions, we never need polymorphic destruction.
  /// @trace SWS_CORE_10400
  /// @archtrace 609900
  ~ErrorDomain() = default;

 private:
  /// @brief To store the error id type.
  IdType const stkMId_;
};

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_ERROR_DOMAIN_H_
