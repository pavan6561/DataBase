// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certificate.h
//
// Purpose     : This file contains X.509 API. Certificate interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_CERTIFICATE_H_
#define SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_CERTIFICATE_H_

#include <openssl/x509.h>

#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/crypto/x509/basic_cert_info.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
/// @brief X.509 Certificate interface.
/// @trace SWS_CRYPT_40200
class Certificate : public BasicCertInfo
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_40201
  using Uptr = std::unique_ptr<Certificate>;

  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_40202
  using Uptrc = std::unique_ptr<const Certificate>;

  /// @brief Certificate verification status.
  /// @trace SWS_CRYPT_40203
  enum class Status : std::uint32_t
  {

    // The certificate is valid
    kValid = 0,
    // The certificate is invalid
    kInvalid = 1,
    // Status of the certificate is unknown yet
    kUnknown = 2,
    // The certificate has correct signature, but the ECU has no a root of trust for this certificate
    kNoTrust = 3,
    // The certificate has correct signature, but it is already expired (its validity period has ended)
    kExpired = 4,
    // The certificate has correct signature, but its validity period is not started yet
    kFuture = 5

  };

  /// @brief Get the X.509 version of this certificate object.
  /// @returns X.509 version
  /// @trace SWS_CRYPT_40211
  virtual std::uint32_t X509Version() const noexcept = 0;

  /// @brief Check whether this certificate belongs to a root CA.
  /// @returns @c true if the TrustMaster has set this certificate as root
  /// @trace SWS_CRYPT_40212
  virtual bool IsRoot() const noexcept = 0;

  /// @brief Get the issuer certificate DN.
  /// @returns Issuer DN of this certificate
  /// @trace SWS_CRYPT_40213
  virtual X509DN::Uptr IssuerDn() const = 0;

  /// @brief Get the "Not Before" of the certificate.
  /// @returns "Not Before" of the certificate
  /// @trace SWS_CRYPT_40214
  virtual time_t StartTime() const noexcept = 0;

  /// @brief Get the "Not After" of the certificate.
  /// @returns "Not After" of the certificate
  /// @trace SWS_CRYPT_40215
  virtual time_t EndTime() const noexcept = 0;

  /// @brief Get the serial number of this certificate.
  /// If <tt>(sn.empty() == true)</tt> then this method only returns required size of the output buffer.
  /// @param[out] sn  an optional output buffer for storing the serial number
  /// @returns size of the certificate serial number in bytes
  /// @error SecurityErrorDomain::kInsufficientCapacity  if <tt>(sn.empty() == false)</tt>, but its size
  /// is not enough for storing the output value
  /// @trace SWS_CRYPT_40216
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> SerialNumber() const noexcept = 0;

  /// @brief Get the DER encoded AuthorityKeyIdentifier of this certificate.
  /// If <tt>(id.empty() == true)</tt> then this method only returns required size of the output buffer.
  /// @param[out] id  the optional output buffer
  /// @returns size of the DER encoded AuthorityKeyIdentifier in bytes
  /// @error SecurityErrorDomain::kInsufficientCapacity  if <tt>(id.empty() == false)</tt>, but its size is
  /// not enough for storing the output value
  /// @trace SWS_CRYPT_40217
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> AuthorityKeyId() const noexcept = 0;

  /// @brief Get the DER encoded SubjectKeyIdentifier of this certificate.
  /// If <tt>(id.empty() == true)</tt> then this method only returns required size of the output buffer.
  /// @param[out] id  the optional output buffer
  /// @returns size of the DER encoded SubjectKeyIdentifier in bytes
  /// @error SecurityErrorDomain::kInsufficientCapacity  if <tt>(id.empty() == false)</tt>, but its size is
  /// not enough for storing the output value
  /// @trace SWS_CRYPT_40218
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> SubjectKeyId() const noexcept = 0;

  /// @brief Verify signature of the certificate.
  /// Call with <tt>(caCert == nullptr)</tt> is applicable only if this is a certificate of a root CA.
  /// @param[in] caCert  the optional pointer to a Certification Authority certificate used for signature of
  /// the current one
  /// @returns @c true if this certificate was verified successfully and @c false otherwise
  /// @trace SWS_CRYPT_40219
  //virtual bool VerifyMe(ara::core::Optional<const Certificate> caCert) const noexcept = 0;

  /// @brief Calculate a fingerprint from the whole certificate.
  /// The produced fingerprint value saved to the output buffer starting from leading bytes of the hash
  /// value. If the capacity of the output buffer is less than the digest size then the digest will be
  /// truncated and only leading bytes will be saved. If the capacity of the output buffer is higher than
  /// the digest size then only leading bytes of the buffer will be updated.
  /// @param[out] fingerprint  output buffer for the fingerprint storage
  /// @param[in] hashCtx  an initialized hash function context
  /// @returns number of bytes actually saved to the output buffer
  /// @error SecurityErrorDomain::kIncompleteArgState  if the @c hashCtx context is not initialized
  /// @trace SWS_CRYPT_40220
  virtual ara::core::Result<std::size_t> GetFingerprint(
    ReadWriteMemRegion fingerprint, cryp::HashFunctionCtx& hashCtx) const noexcept
    = 0;

  /// @brief Return last verification status of the certificate.
  /// @returns the certificate verification status
  /// @trace SWS_CRYPT_40221
  virtual Status GetStatus() const noexcept = 0;

  /// @brief Getter method to get the Openssl crtificate object.
  // Note :- GetCertificate() is an internal API which is to be used by Crypto-daemon only and not to be
  // exposed to the application
  virtual X509* GetCertificate() const noexcept = 0;

  /// @brief Setter method to set verificationStatus.
  // Note :- SetStatus() is an internal API which is to be used by Crypto-daemon only and not to be
  // exposed to the application
  virtual void SetStatus(Status verificationStatus) noexcept = 0;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_CERTIFICATE_H_
