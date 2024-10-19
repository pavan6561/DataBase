// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_dn_proxy_impl.h
//
// Purpose     : X509 domain number proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_X509_DN_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_X509_DN_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/x509/x509_dn.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "vsomeip_interface.h"
#include "x509dn_proxy.h"
#include "x509provider_proxy.h"
#include "ara/crypto/common/security_error_domain.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
class X509DNProxyImpl final : public ara::crypto::x509::X509DN
{
 public:
  /// @brief Constructor for the X509DNProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the authentication cipher.
  /// @param[in] accCPHandler CryptoProviderInterfaceProxy Shared pointer for the authentication cipher.
  X509DNProxyImpl(CryptoServiceInstanceId const instanceValue, X509Provider& x509ProRef) noexcept;

  /// @brief Get the whole Distinguished Name (DN) as a single string.
  /// Capacity of the output string must be enough for storing the output value!
  /// If <tt>(dn == nullptr)</tt> then method only returns required buffer capacity.
  /// @returns StringView of the whole DN string
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(dn != nullptr)</tt>, but @c dn->capacity()
  /// is less than required for the output value storing
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40411
  ara::core::Result<ara::core::StringView> GetDnString() const noexcept override;

  /// @brief Set whole Distinguished Name (DN) from a single string.
  /// [Error]: SecurityErrorDomain::kUnexpectedValue if the @c dn string has incorrect syntax
  /// @param[in] dn the single string containing the whole DN value in text format
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40412
  ara::core::Result<void> SetDn(ara::core::StringView dn) noexcept override;

  /// @brief Get DN attribute by its ID (this method is applicable to all attributes except @c kOrgUnit and
  /// @c kDomainComponent).
  /// Capacity of the output string must be enough for storing the output value!
  /// If <tt>(attribute == nullptr)</tt> then method only returns required buffer capacity.
  /// @param[in] id the identifier of required attribute
  /// @returns StringView of the attribute
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c id argument has unsupported value
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(attribute != nullptr)</tt>,
  /// but @c attribute->capacity() is less than required for storing of the output
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40413
  ara::core::Result<ara::core::StringView> GetAttribute(AttributeId id) const noexcept override;

  /// @brief Set DN attribute by its ID (this method is applicable to all attributes except @c kOrgUnit and
  /// @c kDomainComponent).
  /// @param[in] id the identifier of required attribute
  /// @param[in] attribute the attribute value
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c id argument has unsupported value
  /// @error SecurityErrorDomain::kUnexpectedValue if the attribute string contains incorrect characters
  /// or it has unsupported length
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40414
  ara::core::Result<void> SetAttribute(AttributeId id, ara::core::StringView attribute) const
      noexcept override;

  /// @brief Return DN attribute by its ID and sequential index (this method is applicable to attributes
  /// @c kOrgUnit and @c kDomainComponent).
  /// Capacity of the output string must be enough for storing the output value!
  /// If <tt>(attribute == nullptr)</tt> then method only returns required buffer capacity.
  /// @param[in] id the identifier of required attribute
  /// @param[in] index the zero-based index of required component of the attribute
  /// @returns StringView of the attribute
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c id argument has unsupported value
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(attribute != nullptr)</tt>,
  /// but @c attribute->capacity() is less than required for storing of the output
  /// @error SecurityErrorDomain::kInvalidArgument
  /// if <tt>(id != kOrgUnit) && (id != kDomainComponent) && (index > 0)</tt>
  /// @error SecurityErrorDomain::kAboveBoundary if <tt>((id == kOrgUnit) || (id == kDomainComponent))</tt>
  /// and the @c index value is greater than or equal to the actual number of components in the
  /// specified attribute
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40415
  ara::core::Result<ara::core::StringView> GetAttribute(AttributeId id, unsigned index) const
      noexcept override;

  /// @brief Set DN attribute by its ID and sequential index (this method is applicable to attributes
  /// @c kOrgUnit and @c kDomainComponent).
  /// @param[in] id the identifier of required attribute
  /// @param[in] index the zero-based index of required component of the attribute
  /// @param[in] attribute the attribute value
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c id argument has unsupported value
  /// @error SecurityErrorDomain::kUnexpectedValue if the attribute string contains incorrect characters
  /// or it has unsupported length
  /// @error SecurityErrorDomain::kInvalidArgument
  /// if <tt>(id != kOrgUnit) && (id != kDomainComponent) && (index > 0)</tt>
  /// @error SecurityErrorDomain::kAboveBoundary if <tt>((id == kOrgUnit) || (id == kDomainComponent))</tt>
  /// and the @c index value is greater than the current number of components in the specified attribute
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40416
  ara::core::Result<void> SetAttribute(AttributeId id, unsigned index, ara::core::StringView attribute) const
      noexcept override;

  /// @brief Serialize itself publicly.
  /// @param[in] formatId the Crypto Provider specific identifier of the output format
  /// @returns a buffer with the serialized object
  /// @error SecurityErrorDomain::kInsufficientCapacity
  /// if <tt>(output.empty() == false)</tt>, but it's capacity is less than required
  /// @error SecurityErrorDomain::kUnknownIdentifier if an unknown format ID was specified
  /// @error SecurityErrorDomain::kUnsupportedFormat
  /// if the specified format ID is not supported for this object type
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_10711
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
      FormatId formatId = kFormatDefault) const noexcept override;

  /// @brief Get a reference to X.509 Provider of this object.
  /// @returns a reference to X.509 Provider instance that provides this object
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40911
  X509Provider& MyProvider() const noexcept override;

  /// @brief Method for Get the X509DN service instance id.
  /// @returns Crypto service instance id of CertificateProxyImpl.
  CryptoServiceInstanceId GetX509DnServiceInstanceId() const;

 private:
  /// @brief Method to initializing the server-client connection for AuthCipherContextProxy context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief vsome-ip instance for Authentication context proxy.
  ara::crypto::vsomeip::VsomeipInterface<ara::crypto::ipccom::x509::x509dn::proxy::X509DNProxy>
      vsomeipX509DnInstance_;

  /// Reference to X509Provider class
  X509Provider& x509ProviderRef_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;

  /// @brief local variable to hold GetDnString() value
  mutable ara::core::String getDnString_;

  /// @brief local variable to hold GetAttribute() value
  mutable ara::core::String getAttribute_;

  /// @brief local variable to hold GetAttributeWithIndex() value
  mutable ara::core::String getAttributeIndex_;

  /// @brief authentication cipher context proxy handler
  std::shared_ptr<ara::crypto::ipccom::x509::x509dn::proxy::X509DNProxy> x509DnProxyHandler_;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_X509_DN_PROXY_IMPL_H_
