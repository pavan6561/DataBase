// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name : certificate_proxy_impl.h
//
// Purpose : Certificate domain number proxy implementation to forward functional calls from application
// to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_CERTIFICATE_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_CERTIFICATE_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/x509/certificate.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "vsomeip_interface.h"
#include "certificate_proxy.h"
#include "x509provider_proxy.h"
#include "x509_dn_proxy_impl.h"
#include "x509publickeyinfo_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
class CertificateProxyImpl final : public ara::crypto::x509::Certificate
{
 public:
  /// @brief Constructor for the CertificateProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the authentication cipher.
  /// @param[in] accCPHandler CryptoProviderInterfaceProxy Shared pointer for the authentication cipher.
  CertificateProxyImpl(CryptoServiceInstanceId const instanceValue, X509Provider& x509ProRef) noexcept;

  /// @brief Get the X.509 version of this certificate object.
  /// @returns X.509 version
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40211
  std::uint32_t X509Version() const noexcept override;

  /// @brief Check whether this certificate belongs to a root CA.
  /// @returns @c true if the TrustMaster has set this certificate as root
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40212
  bool IsRoot() const noexcept override;

  /// @brief Get the issuer certificate DN.
  /// @returns Issuer DN of this certificate
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40213

  const X509DN& IssuerDn() const override;

  /// @brief Get the "Not Before" of the certificate.
  /// @returns "Not Before" of the certificate
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40214
  time_t StartTime() const noexcept override;

  /// @brief Get the "Not After" of the certificate.
  /// @returns "Not After" of the certificate
  /// @trace SWS_CRYPT_40215
  time_t EndTime() const noexcept override;

  /// @brief Get the serial number of this certificate.
  /// If <tt>(sn.empty() == true)</tt> then this method only returns required size of the output buffer.
  /// @param[out] sn an optional output buffer for storing the serial number
  /// @returns size of the certificate serial number in bytes
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(sn.empty() == false)</tt>, but its size
  /// is not enough for storing the output value
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40216

  ara::core::Result<ara::core::Vector<ara::core::Byte>> SerialNumber() const noexcept override;

  /// @brief Get the DER encoded AuthorityKeyIdentifier of this certificate.
  /// If <tt>(id.empty() == true)</tt> then this method only returns required size of the output buffer.
  /// @param[out] id the optional output buffer
  /// @returns size of the DER encoded AuthorityKeyIdentifier in bytes
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(id.empty() == false)</tt>, but its size is
  /// not enough for storing the output value
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40217

  ara::core::Result<ara::core::Vector<ara::core::Byte>> AuthorityKeyId() const noexcept override;

  /// @brief Get the DER encoded SubjectKeyIdentifier of this certificate.
  /// If <tt>(id.empty() == true)</tt> then this method only returns required size of the output buffer.
  /// @param[out] id the optional output buffer
  /// @returns size of the DER encoded SubjectKeyIdentifier in bytes
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(id.empty() == false)</tt>, but its size is
  /// not enough for storing the output value
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40218
  ara::core::Result<ara::core::Vector<ara::core::Byte>> SubjectKeyId() const noexcept override;

  /// @brief Calculate a fingerprint from the whole certificate.
  /// The produced fingerprint value saved to the output buffer starting from leading bytes of the hash value.
  /// If the capacity of the output buffer is less than the digest size then the digest will be truncated
  /// and only leading bytes will be saved.
  /// If the capacity of the output buffer is higher than the digest size then only leading bytes of the
  /// buffer will be updated.
  /// @param[out] fingerprint output buffer for the fingerprint storage
  /// @param[in] hashCtx an initialized hash function context
  /// @returns number of bytes actually saved to the output buffer
  /// @error SecurityErrorDomain::kIncompleteArgState if the @c hashCtx context is not initialized
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40220
  ara::core::Result<std::size_t> GetFingerprint(
      ReadWriteMemRegion fingerprint, cryp::HashFunctionCtx& hashCtx) const noexcept override;

  /// @brief Return last verification status of the certificate.
  /// @returns the certificate verification status
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40221
  Status GetStatus() const noexcept override;

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

  /// @brief Get the subject DN.
  /// @returns subject DN
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40112
  const X509DN& SubjectDn() const noexcept override;

  /// @brief Check whether the CA attribute of X509v3 Basic Constraints is true (i.e. pathlen=0).
  /// @returns @c true if it is a CA request and @c false otherwise
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40113
  bool IsCa() const noexcept override;

  /// @brief Get the constraint on the path length defined in the Basic Constraints extension.
  /// @returns certification path length limit
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40114
  std::uint32_t GetPathLimit() const noexcept override;

  /// @brief Get the key constraints for the key associated with this PKCS#10 object.
  /// @returns key constraints
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40115
  BasicCertInfo::KeyConstraints GetConstraints() const noexcept override;

  /// @brief Get a reference to X.509 Provider of this object.
  /// @returns a reference to X.509 Provider instance that provides this object
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40911
  X509Provider& MyProvider() const noexcept override;

  /// @brief Load the subject public key information object to realm of specified crypto provider.
  /// If <tt>(cryptoProvider == nullptr)</tt> then @c X509PublicKeyInfo object will be loaded in realm of
  /// the Stack-default Crypto Provider
  /// @param[in] cryptoProvider unique pointer of a target Crypto Provider, where the public key will be used
  /// @returns constant reference of the subject public key interface
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40111
  const X509PublicKeyInfo& SubjectPubKey(cryp::CryptoProvider::Uptr cryptoProvider = nullptr) const
      noexcept override;

  /// @brief Method for Get the Certificate instance id.
  /// @returns Crypto service instance id of CertificateProxyImpl.
  CryptoServiceInstanceId GetCertificateServiceInstanceId() const;

 private:
  /// @brief Method to initializing the server-client connection for AuthCipherContextProxy context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief vsome-ip instance for Authentication context proxy.
  ara::crypto::vsomeip::VsomeipInterface<ara::crypto::ipccom::x509::certificate::proxy::CertificateProxy>
      vsomeipCertInstance_;

  /// Reference to X509Provider class
  X509Provider& x509ProviderRef_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;

  /// @brief Handler to Certificate interface
  std::shared_ptr<ara::crypto::ipccom::x509::certificate::proxy::CertificateProxy> certificateHandler_;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_CERTIFICATE_PROXY_IMPL_H_
