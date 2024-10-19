// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : cert_sign_request.h
//
// Purpose     : This file contains X.509 API. CertSignRequest interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H_
#define SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H_

#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/x509/basic_cert_info.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
/// @brief Certificate Signing Request (CSR) object interface
/// This interface is dedicated for complete parsing of the request content.
/// @trace SWS_CRYPT_40300
class CertSignRequest : public BasicCertInfo
{
 public:
  /// @brief Unique smart pointer of the constant interface.
  /// @trace SWS_CRYPT_40301
  using Uptrc = std::unique_ptr<const CertSignRequest>;

  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_40302
  using Uptr = std::unique_ptr<CertSignRequest>;

  /// @brief Verifies self-signed signature of the certificate request.
  /// @returns @c true if the signature is correct
  /// @trace SWS_CRYPT_40311
  virtual bool Verify() const noexcept = 0;

  /// @brief Get the challenge password for this request (if it was included to the request).
  /// @param[out] password  the optional pointer to an output string
  /// @returns length of the password if it was provided or 0 otherwise
  /// @error SecurityErrorDomain::kInsufficientCapacity  if <tt>(password != nullptr)</tt>, but its
  /// capacity is less then required for storing the password value
  /// @trace SWS_CRYPT_40312
  //  virtual ara::core::Result<std::size_t> ChallengePassword(ara::core::String* password = nullptr
  //  ) const noexcept  = 0;

  /// @brief Export this certificate signing request in DER encoded ASN1 format.
  /// Note: this is the CSR that can be sent to the CA for obtaining the certificate.
  /// @returns a buffer with the formatted CSR
  /// @error SecurityErrorDomain::kInvalidUsageOrder  this error will be returned in case not all required
  /// information has been provided
  /// @trace SWS_CRYPT_40313
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportASN1CertSignRequest() noexcept = 0;

  /// @brief Return format version of the certificate request.
  /// @returns format version of the certificate request
  /// @trace SWS_CRYPT_40314
  virtual unsigned Version() const noexcept = 0;

  /// @brief Return signature object of the request.
  /// @returns signature object of the request
  /// @trace SWS_CRYPT_40315
  virtual ara::crypto::cryp::Signature::Uptrc GetSignature() const noexcept = 0;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H_
