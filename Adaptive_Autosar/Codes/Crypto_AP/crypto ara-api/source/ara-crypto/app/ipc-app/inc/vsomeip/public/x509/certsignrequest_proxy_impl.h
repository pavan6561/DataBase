// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certsignrequest_proxy_impl.h
//
// Purpose     : Certificate signature request domain number proxy implementation to forward functional calls
//               from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_CERTISIGNREQUEST_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_CERTISIGNREQUEST_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/x509/cert_sign_request.h"
#include "certsignrequest_proxy.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "vsomeip_interface.h"
#include "x509_dn_proxy_impl.h"
#include "x509provider_proxy.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
class CertSignRequestProxyImpl final : public ara::crypto::x509::CertSignRequest
{
 public:
  /// @brief Constructor for the CertSignRequestProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the authentication
  /// cipher.
  /// @param[in] accCPHandler CryptoProviderInterfaceProxy Shared pointer for
  /// the authentication cipher.
  CertSignRequestProxyImpl(CryptoServiceInstanceId const instanceValue, X509Provider& x509ProRef) noexcept;

  /// @brief Verifies self-signed signature of the certificate request.
  /// @returns @c true if the signature is correct
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40311

  bool Verify() const noexcept override;

  /// @brief Export this certificate signing request in DER encoded ASN1 format.
  /// this is the CSR that can be sent to the CA for obtaining
  /// the certificate.
  /// @returns a buffer with the formatted CSR
  /// @error SecurityErrorDomain::kInvalidUsageOrder this error will be
  /// returned in case not all required
  /// information has been provided
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40313
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportASN1CertSignRequest() noexcept override;

  /// @brief Return format version of the certificate request.
  /// @returns format version of the certificate request
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40314
  unsigned Version() const noexcept override;

  /// @brief Return signature object of the request.
  /// @returns signature object of the request
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40315
  const ara::crypto::cryp::Signature& GetSignature() const noexcept override;

  /// @brief Get the subject DN.
  /// @returns subject DN
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40112
  const X509DN& SubjectDn() const noexcept override;

  /// @brief Check whether the CA attribute of X509v3 Basic Constraints is true
  /// (i.e. pathlen=0).
  /// @returns @c true if it is a CA request and @c false otherwise
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40113
  bool IsCa() const noexcept override;

  /// @brief Get the constraint on the path length defined in the Basic
  /// Constraints extension.
  /// @returns certification path length limit
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40114
  std::uint32_t GetPathLimit() const noexcept override;

  /// @brief Get the key constraints for the key associated with this PKCS#10
  /// object.
  /// @returns key constraints
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40115
  KeyConstraints GetConstraints() const noexcept override;

  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
      FormatId formatId = kFormatDefault) const noexcept override;

  /// @brief Get a reference to X.509 Provider of this object.
  /// @returns a reference to X.509 Provider instance that provides this object
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_40911
  X509Provider& MyProvider() const noexcept override;

 private:
  /// @brief Method to initializing the server-client connection for
  /// AuthCipherContextProxy context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief vsome-ip instance for Authentication context proxy.
  ara::crypto::vsomeip::VsomeipInterface<ara::crypto::ipccom::x509::csr::proxy::CertSignRequestProxy>
      vsomeipCertInstance_;

  /// @brief Reference to X509Provider class
  X509Provider& x509ProviderRef_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;

  /// @brief Handler to Certificate interface
  std::shared_ptr<ara::crypto::ipccom::x509::csr::proxy::CertSignRequestProxy> csrHandler_;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_CERTISIGNREQUEST_PROXY_IMPL_H_
