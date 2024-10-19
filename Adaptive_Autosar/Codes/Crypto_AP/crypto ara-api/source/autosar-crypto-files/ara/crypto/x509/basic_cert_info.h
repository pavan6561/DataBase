// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : basic_cert_info.h
//
// Purpose     : This file contains X.509 API. BasicCertInfo interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_BASIC_CERT_INFO_H_
#define SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_BASIC_CERT_INFO_H_

#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/x509/x509_dn.h"
#include "ara/crypto/x509/x509_public_key_info.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
/// @brief Basic Certificate Information interface.
/// SWS_CRYPT_40100
class BasicCertInfo : public X509Object
{
 public:
  /// @brief X.509 v3 Key Constraints type definition.
  /// @trace SWS_CRYPT_40101
  using KeyConstraints = std::uint32_t;

  /// @brief No key constraints.
  /// @trace SWS_CRYPT_40150
  static const KeyConstraints kConstrNone = 0;

  /// @brief The key can be used for digital signature production.
  /// @trace SWS_CRYPT_40151
  static const KeyConstraints kConstrDigitalSignature = 0x8000;

  /// @brief The key can be used in cases requiring the "non-repudiation" guarantee
  /// @trace SWS_CRYPT_40152
  static const KeyConstraints kConstrNonRepudiation = 0x4000;

  /// @brief The key can be used for key encipherment.
  /// @trace SWS_CRYPT_40153
  static const KeyConstraints kConstrKeyEncipherment = 0x2000;

  /// @brief The key can be used for data encipherment.
  /// @trace SWS_CRYPT_40154
  static const KeyConstraints kConstrDataEncipherment = 0x1000;

  /// @brief The key can be used for a key agreement protocol execution.
  /// @trace SWS_CRYPT_40155
  static const KeyConstraints kConstrKeyAgreement = 0x0800;

  /// @brief The key can be used for certificates signing.
  /// @trace SWS_CRYPT_40156
  static const KeyConstraints kConstrKeyCertSign = 0x0400;

  /// @brief The key can be used for Certificates Revokation Lists (CRL) signing.
  /// @trace SWS_CRYPT_40157
  static const KeyConstraints kConstrCrlSign = 0x0200;

  /// @brief The enciphermet key can be used for enciphering only.
  /// @trace SWS_CRYPT_40158
  static const KeyConstraints kConstrEncipherOnly = 0x0100;

  /// @brief The enciphermet key can be used for deciphering only.
  /// @trace SWS_CRYPT_40159
  static const KeyConstraints kConstrDecipherOnly = 0x0080;

  /// @brief Load the subject public key information object to realm of specified crypto provider.
  /// If <tt>(cryptoProvider == nullptr)</tt> then @c X509PublicKeyInfo object will be loaded in realm of
  /// the Stack-default Crypto Provider
  /// @param[in] cryptoProvider  unique pointer of a target Crypto Provider, where the public key will be
  /// used
  /// @returns constant reference of the subject public key interface
  /// @trace SWS_CRYPT_40111
  virtual X509PublicKeyInfo::Uptrc SubjectPubKey(cryp::CryptoProvider::Uptr cryptoProvider = nullptr) const
    noexcept
    = 0;

  /// @brief Get the subject DN.
  /// @returns subject DN
  /// @trace SWS_CRYPT_40112
  virtual X509DN::Uptr SubjectDn() const noexcept = 0;

  /// @brief Check whether the CA attribute of X509v3 Basic Constraints is true (i.e. pathlen=0).
  /// @returns @c true if it is a CA request and @c false otherwise
  /// @trace SWS_CRYPT_40113
  virtual bool IsCa() const noexcept = 0;

  /// @brief Get the constraint on the path length defined in the Basic Constraints extension.
  /// @returns certification path length limit
  /// @trace SWS_CRYPT_40114
  //virtual std::uint32_t GetPathLimit() const noexcept = 0;

  /// @brief Get the key constraints for the key associated with this PKCS#10 object.
  /// @returns key constraints
  /// @trace SWS_CRYPT_40115
  virtual KeyConstraints GetConstraints() const noexcept = 0;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_BASIC_CERT_INFO_H_
