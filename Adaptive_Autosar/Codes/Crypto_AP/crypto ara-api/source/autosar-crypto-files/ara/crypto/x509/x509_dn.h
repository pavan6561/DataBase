// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ocsp_request.h
//
// Purpose     : This file contains X.509 API. X509DN interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_CERT_X509_DN_H_
#define SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_CERT_X509_DN_H_

#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/crypto/x509/x509_object.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
/// @brief Interface of X.509 Distinguished Name (DN).
/// @trace SWS_CRYPT_40400
class X509DN : public X509Object
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_40401
  using Uptr = std::unique_ptr<X509DN>;

  /// @brief Unique smart pointer of the constant interface.
  /// @trace SWS_CRYPT_40402
  using Uptrc = std::unique_ptr<const X509DN>;

  /// @brief Enumeration of DN attributes' identifiers.
  /// @trace SWS_CRYPT_40403
  enum class AttributeId : std::uint32_t
  {
    // Common Name
    kCommonName = 0,
    // Country
    kCountry = 1,
    // State
    kState = 2,
    // Locality
    kLocality = 3,
    // Organization
    kOrganization = 4,
    // Organization Unit
    kOrgUnit = 5,
    // Street
    kStreet = 6,
    // Postal Code
    kPostalCode = 7,
    // Title
    kTitle = 8,
    // Surname
    kSurname = 9,
    // Given Name
    kGivenName = 10,
    // Initials
    kInitials = 11,
    // Pseudonym
    kPseudonym = 12,
    // Generation Qualifier
    kGenerationQualifier = 13,
    // Domain Component
    kDomainComponent = 14,
    // Distinguished Name Qualifier
    kDnQualifier = 15,
    // E-mail
    kEmail = 16,
    // URI
    kUri = 17,
    // DNS
    kDns = 18,
    // Host Name (UNSTRUCTUREDNAME)
    kHostName = 19,
    // IP Address (UNSTRUCTUREDADDRESS)
    kIpAddress = 20,
    // Serial Numbers
    kSerialNumbers = 21,
    // User ID
    kUserId = 22
  };

  /// @brief Get the whole Distinguished Name (DN) as a single string.
  /// Capacity of the output string must be enough for storing the output value!
  /// If <tt>(dn == nullptr)</tt> then method only returns required buffer capacity.
  /// @returns StringView of the whole DN string.
  /// @error  SecurityErrorDomain::kInsufficientCapacity  if <tt>(dn != nullptr)</tt>, but @c dn->capacity()
  /// is less than required for the output value storing.
  /// @trace SWS_CRYPT_40411
  virtual ara::core::Result<ara::core::StringView> GetDnString() const noexcept = 0;

  /// @brief Set whole Distinguished Name (DN) from a single string.
  /// [Error]: SecurityErrorDomain::kUnexpectedValue  if the @c dn string has incorrect syntax.
  /// @param[in] dn  the single string containing the whole DN value in text format.
  /// @trace SWS_CRYPT_40412
  virtual ara::core::Result<void> SetDn(ara::core::StringView dn) noexcept = 0;

  /// @traceid{SWS_CRYPT_40413}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02306}
  /// @brief Get DN attribute by its ID (this method is applicale to all attributes except @c kOrgUnit and
  /// @c kDomainComponent).
  /// Capacity of the output string must be enough for storing the output value!
  /// If <tt>(attribute == nullptr)</tt> then method only returns required buffer capacity.
  /// @param[in] id  the identifier of required attribute.
  /// @returns StringView of the attribute.
  /// @error  SecurityErrorDomain::kUnknownIdentifier  if the @c id argument has unsupported value.
  /// @error  SecurityErrorDomain::kInsufficientCapacity  if <tt>(attribute != nullptr)</tt>,
  /// but @c attribute->capacity() is less than required for storing of the output.
  /// @trace SWS_CRYPT_40412
  virtual ara::core::Result<ara::core::StringView> GetAttribute(AttributeId id) const noexcept = 0;

  /// @brief Set DN attribute by its ID (this method is applicale to all attributes except @c kOrgUnit and
  /// @c kDomainComponent).
  /// @param[in] id  the identifier of required attributet.
  /// @param[in] attribute  the attribute value.
  /// @error  SecurityErrorDomain::kUnknownIdentifier  if the @c id argument has unsupported value.
  /// @error  SecurityErrorDomain::kUnexpectedValue  if the attribute string contains incorrect characters
  /// or it has unsupported length.
  /// @trace SWS_CRYPT_40414
  virtual ara::core::Result<void> SetAttribute(AttributeId id, ara::core::StringView attribute) noexcept = 0;

  /// @brief Return DN attribute by its ID and sequential index (this method is applicale to attributes
  /// @c kOrgUnit and @c kDomainComponent).
  /// Capacity of the output string must be enough for storing the output value!
  /// If <tt>(attribute == nullptr)</tt> then method only returns required buffer capacity.
  /// @param[in] id  the identifier of required attribute.
  /// @param[in] index  the zero-based index of required component of the attribute.
  /// @returns StringView of the attribute.
  /// @error  SecurityErrorDomain::kUnknownIdentifier  if the @c id argument has unsupported value.
  /// @error  SecurityErrorDomain::kInsufficientCapacity  if <tt>(attribute != nullptr)</tt>,
  /// but @c attribute->capacity() is less than required for storing of the output.
  /// @error  SecurityErrorDomain::kInvalidArgument
  /// if <tt>(id != kOrgUnit) && (id != kDomainComponent) && (index > 0)</tt>
  /// @error  SecurityErrorDomain::kAboveBoundary if <tt>((id == kOrgUnit) || (id == kDomainComponent))</tt>
  /// and the @c index value is greater than or equal to the actual number of components in the
  /// specified attribute.
  /// @trace SWS_CRYPT_40415
  virtual ara::core::Result<ara::core::StringView> GetAttribute(AttributeId id, unsigned index) const noexcept
    = 0;

  /// @brief Set DN attribute by its ID and sequential index (this method is applicale to attributes
  /// @c kOrgUnit and @c kDomainComponent).
  /// @param[in] id  the identifier of required attribute.
  /// @param[in] index  the zero-based index of required component of the attribute.
  /// @param[in] attribute  the attribute value.
  /// @error  SecurityErrorDomain::kUnknownIdentifier  if the @c id argument has unsupported value.
  /// @error  SecurityErrorDomain::kUnexpectedValue  if the attribute string contains incorrect characters
  /// or it has unsupported length.
  /// @error  SecurityErrorDomain::kInvalidArgument
  /// if <tt>(id != kOrgUnit) && (id != kDomainComponent) && (index > 0)</tt>
  /// @error  SecurityErrorDomain::kAboveBoundary if <tt>((id == kOrgUnit) || (id == kDomainComponent))</tt>
  /// and the @c index value is greater than the current number of components in the specified attribute.
  /// @trace SWS_CRYPT_40416
  virtual ara::core::Result<void> SetAttribute(
    AttributeId id, unsigned index, ara::core::StringView attribute) noexcept
    = 0;

  /// @brief Check for equality of this and another Distinguished Name (DN) objects.
  /// @param[in] other  another instance of DN for comparison
  /// @returns @c true if the provided DN is identical to this one and @c false otherwise
  /// @trace SWS_CRYPT_40417
  //virtual bool operator==(const X509DN& other) const noexcept = 0;

  /// @brief Check for inequality of this and another Distinguished Name (DN) objects.
  /// @param[in] other  another instance of DN for comparison
  /// @returns @c true if the provided DN is not identical to this one and @c false otherwise
  /// @trace SWS_CRYPT_40418
  //bool operator!=(const X509DN& other) const noexcept
  //{
  //  return !(*this == other);
  //}
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_DN_H_
