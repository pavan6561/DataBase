// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certificate_impl.h
//
// Purpose     : This file contains X.509 API. Certificate interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CERTIFICATE_IMPL_H_
#define CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CERTIFICATE_IMPL_H_

#include <ctime>

#include <ara/log/logging.h>
#include <openssl/bio.h>
#include <openssl/x509_vfy.h>
#include <openssl/x509v3.h>
#include <string>

#include "ara/crypto/x509/certificate.h"
#include "crypto_provider_ssl.h"
#include "x509_dn_impl.h"
#include "x509_public_key_info_impl.h"

namespace ara
{
namespace crypto
{
namespace x509
{
/// @brief X.509 Certificate interface.
/// @trace SWS_CRYPT_40200
class CertificateImpl : public Certificate
{
 public:
  /// @brief Constructor for CertificateImpl class.
  CertificateImpl(X509* cert) :
      Certificate{}, certificatePtr_{cert}, issuerDomainName_{X509_get_issuer_name(cert)},
      subjectdomainName_{X509_get_subject_name(cert)}, startTime_{0}, endTime_{0},
      certStatus_{Status::kUnknown}, aralog_{ara::log::CreateLogger(
                                         "CERT", "Certificate Impl", ara::log::LogLevel::kVerbose)}
  {
    startTime_ = GetTimeT(X509_get0_notBefore(cert));
    endTime_ = GetTimeT(X509_get0_notAfter(cert));
  }

  /// @brief Get the X.509 version of this certificate object.
  /// @returns X.509 version
  /// @trace SWS_CRYPT_40211
  std::uint32_t X509Version() const noexcept override;

  /// @brief Check whether this certificate belongs to a root CA.
  /// @returns @c true if the TrustMaster has set this certificate as root
  /// @trace SWS_CRYPT_40212
  bool IsRoot() const noexcept override;

  /// @brief Get the issuer certificate DN.
  /// @returns Issuer DN of this certificate.
  /// @trace SWS_CRYPT_40213
  X509DN::Uptr IssuerDn() const override;

  /// @brief Get the "Not Before" of the certificate.
  /// @returns "Not Before" of the certificate.
  /// @trace SWS_CRYPT_40214
  time_t StartTime() const noexcept override;

  /// @brief Get the "Not After" of the certificate.
  /// @returns "Not After" of the certificate.
  /// @trace SWS_CRYPT_40215
  time_t EndTime() const noexcept override;

  /// @brief Get the serial number of this certificate.
  /// If <tt>(sn.empty() == true)</tt> then this method only returns required size of the output buffer.
  /// @param[out] sn an optional output buffer for storing the serial number.
  /// @returns size of the certificate serial number in bytes.
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(sn.empty() == false)</tt>, but its size
  /// is not enough for storing the output value.
  /// @trace SWS_CRYPT_40216
  ara::core::Result<ara::core::Vector<ara::core::Byte>> SerialNumber() const noexcept override;

  /// @brief Get the DER encoded AuthorityKeyIdentifier of this certificate.
  /// If <tt>(id.empty() == true)</tt> then this method only returns required size of the output buffer.
  /// @param[out] id the optional output buffer.
  /// @returns size of the DER encoded AuthorityKeyIdentifier in bytes.
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(id.empty() == false)</tt>, but its size is
  /// not enough for storing the output value.
  /// @trace SWS_CRYPT_40217
  ara::core::Result<ara::core::Vector<ara::core::Byte>> AuthorityKeyId() const noexcept override;

  /// @brief Get the DER encoded SubjectKeyIdentifier of this certificate.
  /// If <tt>(id.empty() == true)</tt> then this method only returns required size of the output buffer.
  /// @param[out] id the optional output buffer.
  /// @returns size of the DER encoded SubjectKeyIdentifier in bytes.
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(id.empty() == false)</tt>, but its size is
  /// not enough for storing the output value.
  /// @trace SWS_CRYPT_40218
  ara::core::Result<ara::core::Vector<ara::core::Byte>> SubjectKeyId() const noexcept override;

  /// @brief Verify signature of the certificate.
  /// Call with <tt>(caCert == nullptr)</tt> is applicable only if this is a certificate of a root CA.
  /// @param[in] caCert the optional pointer to a Certification Authority certificate used for signature of
  /// the current one.
  /// @returns @c true if this certificate was verified successfully and @c false otherwise.
  /// @trace SWS_CRYPT_40219
  //bool VerifyMe(ara::core::Optional<const Certificate> caCert) const noexcept override;

  /// @brief Calculate a fingerprint from the whole certificate.
  /// The produced fingerprint value saved to the output buffer starting from leading bytes of the hash
  /// value. If the capacity of the output buffer is less than the digest size then the digest will be
  /// truncated and only leading bytes will be saved. If the capacity of the output buffer is higher than
  /// the digest size then only leading bytes of the buffer will be updated.
  /// @param[out] fingerprint output buffer for the fingerprint storage.
  /// @param[in] hashCtx an initialized hash function context.
  /// @returns number of bytes actually saved to the output buffer.
  /// @error SecurityErrorDomain::kIncompleteArgState if the @c hashCtx context is not initialized.
  /// @trace SWS_CRYPT_40220
  ara::core::Result<std::size_t> GetFingerprint(
      ReadWriteMemRegion fingerprint, cryp::HashFunctionCtx& hashCtx) const noexcept override;

  /// @brief Return last verification status of the certificate.
  /// @returns the certificate verification status.
  /// @trace SWS_CRYPT_40221
  Status GetStatus() const noexcept override;

  /// @brief Get the subject DN.
  /// @returns subject DN.
  /// @trace SWS_CRYPT_40112
  X509DN::Uptr SubjectDn() const noexcept override;

  /// @brief Check whether the CA attribute of X509v3 Basic Constraints is true (i.e. pathlen=0).
  /// @returns @c true if it is a CA request and @c false otherwise.
  /// @trace SWS_CRYPT_40113
  bool IsCa() const noexcept override;

  // Note :- GetCertificate() is an internal API which is to be used by Crypto-daemon only and not to be
  // exposed to the application.

  /// @brief Getter method to get the Openssl certificate object.
  X509* GetCertificate() const noexcept override;

  // Note :- SetStatus() is an internal API which is to be used by Crypto-daemon only and not to be
  // exposed to the application.

  /// @brief Setter method to set verificationStatus.
  void SetStatus(Status verificationStatus) noexcept override;

  /// @brief Serialize itself publicly.
  /// @param[in] formatId the Crypto Provider specific identifier of the output format.
  /// @returns a buffer with the serialized object.
  /// @error SecurityErrorDomain::kInsufficientCapacity if <tt>(output.empty() == false)</tt>, but it's
  /// capacity is less than required.
  /// @error SecurityErrorDomain::kUnknownIdentifier if an unknown format ID was specified.
  /// @error SecurityErrorDomain::kUnsupportedFormat if the specified format ID is not supported for this
  /// object type.
  /// @trace SWS_CRYPT_10711
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
      FormatId formatId = kFormatDefault) const noexcept override;

  /// @brief Load the subject public key information object to realm of specified crypto provider.
  /// If <tt>(cryptoProvider == nullptr)</tt> then @c X509PublicKeyInfo object will be loaded in realm of
  /// the Stack-default Crypto Provider.
  /// @param[in] cryptoProvider unique pointer of a target Crypto Provider, where the public key will be
  /// used.
  /// @returns constant reference of the subject public key interface.
  /// @trace SWS_CRYPT_40111
  X509PublicKeyInfo::Uptrc SubjectPubKey(cryp::CryptoProvider::Uptr cryptoProvider = nullptr) const
      noexcept override;

  /// @brief Get the key constraints for the key associated with this PKCS#10 object.
  /// @returns key constraints.
  /// @trace SWS_CRYPT_40115
  KeyConstraints GetConstraints() const noexcept override;

 private:
  /// @brief method to get the Public Key Algorithm Information from the Certificate.
  CryptoAlgId GetAlgoIdFromTheCertificate(EVP_PKEY* const publicKey) const noexcept;

  /// @brief method to convert ASN1_TIME type to time_t.
  time_t GetTimeT(ASN1_TIME const* time) noexcept;

  /// @brief pointer to the certificate.
  X509* certificatePtr_;

  /// @brief instance to store issuer name.
  X509_NAME* issuerDomainName_;

  /// @brief instance of a subject name.
  X509_NAME* subjectdomainName_;

  /// @brief variable to store the start time of certificate.
  time_t startTime_;

  /// @brief variable to store the end time of certificate.
  time_t endTime_;

  /// @brief variable to store the certificate status.
  Status certStatus_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CERTIFICATE_IMPL_H_
