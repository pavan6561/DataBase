// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ocsp_request.h
//
// Purpose     : This file contains Cryptographic API. X509PublicKeyInfo interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H_
#define SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H_

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"
#include "ara/crypto/cryp/cryobj/public_key.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
/// @brief X.509 Public Key Information interface.
/// @trace SWS_CRYPT_24400
class X509PublicKeyInfo : public Serializable
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_24401
  using Uptrc = std::unique_ptr<const X509PublicKeyInfo>;

  /// @brief Get the CryptoPrimitiveId instance of this class.
  /// @trace SWS_CRYPT_24410
  virtual ara::crypto::cryp::CryptoPrimitiveId::Uptrc GetAlgorithmId() const = 0;

  /// @brief Get the hash size required by current signature algorithm.
  /// @returns required hash size in bytes
  /// @trace SWS_CRYPT_24411
  virtual std::size_t GetRequiredHashSize() const noexcept = 0;

  /// @brief Get an ID of hash algorithm required by current signature algorithm.
  /// @returns required hash algorithm ID or @c kAlgIdAny if the signature algorithm specification does not
  /// include a concrete hash function
  /// @trace SWS_CRYPT_24412
  virtual CryptoAlgId GetRequiredHashAlgId() const noexcept = 0;

  /// @brief Get size of the signature value produced and required by the current algorithm.
  /// @returns size of the signature value in bytes
  /// @trace SWS_CRYPT_24413
  virtual std::size_t GetSignatureSize() const noexcept = 0;

  /// @brief Get public key object of the subject.
  /// Created @c PublicKey object is @b session and @c non-exportable, because generic X.509 certificate or
  /// certificate signing request (CSR) doesn't have COUID of the public key, therefore it should be saved
  /// or transmitted only as a part of correspondent certificate or CSR.
  /// @returns unique smart pointer to the created public key of the subject
  /// @trace SWS_CRYPT_24414
  virtual ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc> GetPublicKey() const noexcept = 0;

  /// @brief Verify the sameness of the provided and kept public keys.
  /// This method compare the public key values only.
  /// @param[in] publicKey  the public key object for comparison
  /// @returns @c true if values of the stored public key and object provided by the argument are identical
  /// and @c false otherwise
  /// @trace SWS_CRYPT_24415
  virtual bool IsSameKey(const ara::crypto::cryp::PublicKey& publicKey) const noexcept = 0;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H_
