// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : cert_sign_request_impl.h
//
// Purpose     : This file contains X.509 API. CertSignRequest interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CERT_SIGN_REQUEST_IMPL_H_
#define SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CERT_SIGN_REQUEST_IMPL_H_

#include "algorithm_identification.h"
#include "ara/crypto/x509/cert_sign_request.h"
#include "crypto_provider_ssl.h"
#include "signature_impl.h"
#include "x509_dn_impl.h"
#include "x509_public_key_info_impl.h"

namespace ara
{
namespace crypto
{
namespace x509
{
/// @brief Certificate Signing Request (CSR) object interface
/// This interface is dedicated for complete parsing of the request content.
/// @trace SWS_CRYPT_40300
class CertSignRequestImpl : public CertSignRequest
{
 public:
  /// @brief Constructor for CertSignRequestImpl class.s
  CertSignRequestImpl(X509_REQ* req) : csrReq_{req}, subjectdomainName_{X509_REQ_get_subject_name(req)}
  {
  }

  /// @brief Verifies self-signed signature of the certificate request.
  /// @returns @c true if the signature is correct
  /// @trace SWS_CRYPT_40311
  bool Verify() const noexcept override;

  /// @brief Export this certificate signing request in DER encoded ASN1 format.
  /// Note: this is the CSR that can be sent to the CA for obtaining the certificate.
  /// @returns a buffer with the formatted CSR
  /// @error SecurityErrorDomain::kInvalidUsageOrderthis error will be returned in case not all required
  /// information has been provided
  /// @trace SWS_CRYPT_40313
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportASN1CertSignRequest() noexcept override;

  /// @brief Return format version of the certificate request.
  /// @returns format version of the certificate request
  /// @trace SWS_CRYPT_40314
  unsigned Version() const noexcept override;

  /// @brief Return signature object of the request.
  /// @returns signature object of the request
  /// @trace SWS_CRYPT_40315
  ara::crypto::cryp::Signature::Uptrc GetSignature() const noexcept override;

  /// @brief Get the subject DN.
  /// @returns subject DN
  /// @trace SWS_CRYPT_40112
  X509DN::Uptr SubjectDn() const noexcept override;

  /// @brief Check whether the CA attribute of X509v3 Basic Constraints is true (i.e. pathlen=0).
  /// @returns @c true if it is a CA request and @c false otherwise
  /// @trace SWS_CRYPT_40113
  bool IsCa() const noexcept override;

  /// @brief Serialize itself publicly.
  /// @param[in] formatId the Crypto Provider specific identifier of the output format
  /// @returns a buffer with the serialized object
  /// @error SecurityErrorDomain::kInsufficientCapacity
  /// if <tt>(output.empty() == false)</tt>, but it's capacity is less than required
  /// @error SecurityErrorDomain::kUnknownIdentifier if an unknown format ID was specified
  /// @error SecurityErrorDomain::kUnsupportedFormat
  /// if the specified format ID is not supported for this object type
  /// @trace SWS_CRYPT_10711
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
      FormatId formatId = kFormatDefault) const noexcept override;

  /// @brief Load the subject public key information object to realm of specified crypto provider.
  /// If <tt>(cryptoProvider == nullptr)</tt> then @c X509PublicKeyInfo object will be loaded in realm of
  /// the Stack-default Crypto Provider
  /// @param[in] cryptoProvider unique pointer of a target Crypto Provider, where the public key will be
  /// used
  /// @returns constant reference of the subject public key interface
  /// @trace SWS_CRYPT_40111
  X509PublicKeyInfo::Uptrc SubjectPubKey(cryp::CryptoProvider::Uptr cryptoProvider = nullptr) const
      noexcept override;

  /// @brief Get the key constraints for the key associated with this PKCS#10 object.
  /// @returns key constraints
  /// @trace SWS_CRYPT_40115
  KeyConstraints GetConstraints() const noexcept override;

 private:
  CryptoAlgId GetAlgoIdFromTheCSR(EVP_PKEY* public_key) const noexcept;

  /// @brief openssl CSR pointer.
  X509_REQ* csrReq_;

  /// @brief instance of a subject name.
  X509_NAME* subjectdomainName_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{
      ara::log::CreateLogger("CREQ", "Certificate Sign Request IMPL", ara::log::LogLevel::kVerbose)};
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CERT_SIGN_REQUEST_IMPL_H_
