// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_dn_impl.h
//
// Purpose     : This file contains X.509 API. X509DN interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_DN_IMPL_H_
#define CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_DN_IMPL_H_

#include <ara/core/map.h>
#include <ara/core/string.h>
#include <ara/log/logging.h>
#include <openssl/x509.h>

#include "ara/crypto/common/security_error_domain.h"
#include "ara/crypto/x509/x509_dn.h"

namespace ara
{
namespace crypto
{
namespace x509
{
/// @brief Interface of X.509 Distinguished Name (DN).
/// @trace SWS_CRYPT_40400
class X509DNImpl : public X509DN
{
 public:
  /// @brief Constructor for X509DNImpl class.
  X509DNImpl(X509_NAME* dn) noexcept;

  /// @brief Get the whole Distinguished Name (DN) as a single string.
  /// Capacity of the output string must be enough for storing the output value!
  /// If <tt>(dn == nullptr)</tt> then method only returns required buffer capacity.
  /// @returns StringView of the whole DN string.
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(dn != nullptr)</tt>, but @c dn->capacity()
  /// is less than required for the output value storing.
  /// @trace SWS_CRYPT_40411
  ara::core::Result<ara::core::StringView> GetDnString() const noexcept override;

  /// @brief Set whole Distinguished Name (DN) from a single string.
  /// [Error]: SecurityErrorDomain::kUnexpectedValue if the @c dn string has incorrect syntax
  /// @param[in] dn the single string containing the whole DN value in text format.
  /// @trace SWS_CRYPT_40412
  ara::core::Result<void> SetDn(ara::core::StringView dn) noexcept override;

  /// @brief Get DN attribute by its ID (this method is applicable to all attributes except @c kOrgUnit and
  /// @c kDomainComponent).
  /// Capacity of the output string must be enough for storing the output value!
  /// If <tt>(attribute == nullptr)</tt> then method only returns required buffer capacity.
  /// @param[in] id the identifier of required attribute.
  /// @returns StringView of the attribute.
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c id argument has unsupported value.
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(attribute != nullptr)</tt>, but
  /// @c attribute->capacity() is less than required for storing of the output.
  /// @trace SWS_CRYPT_40412
  ara::core::Result<ara::core::StringView> GetAttribute(AttributeId id) const noexcept override;

  /// @brief Set DN attribute by its ID (this method is applicable to all attributes except @c kOrgUnit and
  /// @c kDomainComponent).
  /// @param[in] id the identifier of required attribute.
  /// @param[in] attribute the attribute value.
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c id argument has unsupported value.
  /// @error SecurityErrorDomain::kUnexpectedValue if the attribute string contains incorrect characters
  /// or it has unsupported length.
  /// @trace SWS_CRYPT_40414
  ara::core::Result<void> SetAttribute(AttributeId id, ara::core::StringView attribute) noexcept override;

  /// @brief Return DN attribute by its ID and sequential index (this method is applicable to attributes
  /// @c kOrgUnit and @c kDomainComponent).
  /// Capacity of the output string must be enough for storing the output value!
  /// If <tt>(attribute == nullptr)</tt> then method only returns required buffer capacity.
  /// @param[in] id the identifier of required attribute.
  /// @param[in] index the zero-based index of required component of the attribute.
  /// @returns StringView of the attribute.
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c id argument has unsupported value.
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(attribute != nullptr)</tt>, but
  /// @c attribute->capacity() is less than required for storing of the output.
  /// @error SecurityErrorDomain::kInvalidArgument
  /// if <tt>(id != kOrgUnit) && (id != kDomainComponent) && (index > 0)</tt>
  /// @error SecurityErrorDomain::kAboveBoundary if <tt>((id == kOrgUnit) || (id == kDomainComponent))</tt>
  /// and the @c index value is greater than or equal to the actual number of components in the
  /// specified attribute.
  /// @trace SWS_CRYPT_40415
  ara::core::Result<ara::core::StringView> GetAttribute(AttributeId id, unsigned index) const
      noexcept override;

  /// @brief Set DN attribute by its ID and sequential index (this method is applicable to attributes
  /// @c kOrgUnit and @c kDomainComponent).
  /// @param[in] id the identifier of required attribute.
  /// @param[in] index the zero-based index of required component of the attribute.
  /// @param[in] attribute the attribute value.
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c id argument has unsupported value.
  /// @error SecurityErrorDomain::kUnexpectedValue if the attribute string contains incorrect characters
  /// or it has unsupported length.
  /// @error SecurityErrorDomain::kInvalidArgument
  /// if <tt>(id != kOrgUnit) && (id != kDomainComponent) && (index > 0)</tt>
  /// @error SecurityErrorDomain::kAboveBoundary if <tt>((id == kOrgUnit) || (id == kDomainComponent))</tt>
  /// and the @c index value is greater than the current number of components in the specified attribute.
  /// @trace SWS_CRYPT_40416
  ara::core::Result<void> SetAttribute(
      AttributeId id, unsigned index, ara::core::StringView attribute) noexcept override;

  /// @brief Serialize itself publicly.
  /// @param[in] formatId the Crypto Provider specific identifier of the output format.
  /// @returns a buffer with the serialized object.
  /// @error SecurityErrorDomain::kInsufficientCapacity.
  /// if <tt>(output.empty() == false)</tt>, but it's capacity is less than required.
  /// @error SecurityErrorDomain::kUnknownIdentifier if an unknown format ID was specified.
  /// @error SecurityErrorDomain::kUnsupportedFormat
  /// if the specified format ID is not supported for this object type.
  /// @trace SWS_CRYPT_10711
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
      FormatId formatId = kFormatDefault) const noexcept override;

 private:
  /// @brief Parse DN value in name variable.
  void PrseOpensSSlDn(ara::core::StringView dn) noexcept;

  /// @brief Map to store the attribute.
  ara::core::Map<ara::core::String, ara::core::String> attrMap_;

  /// @brief Variable to store distinguished name.
  ara::core::String distName_;

  /// @brief pointer to the X509_NAME variable.
  X509_NAME* domainName_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{
      ara::log::CreateLogger("X5DI", "X509 Domain Name IPML", ara::log::LogLevel::kVerbose)};
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_DN_H_
