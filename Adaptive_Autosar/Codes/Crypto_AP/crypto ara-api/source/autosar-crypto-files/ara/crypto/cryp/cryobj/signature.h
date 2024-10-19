// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature.h
//
// Purpose     : This file contains definitions for Signature abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_SIGNATURE_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_SIGNATURE_H_

#include <ara/log/logging.h>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/cryobj/crypto_object.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Signature container interface
/// This interface is applicable for keeping the Digital Signature, Hash Digest, (Hash-based) Message
/// Authentication Code (MAC/HMAC).
/// In case of a keyed signature (Digital Signature or MAC/HMAC) a @a COUID of the signature verification key
/// can be obtained by a call of @c CryptoObject::HasDependence()!
/// @trace SWS_CRYPT_23300
class Signature
  : public CryptoObject
  , public Serializable
{
 public:
  /// @brief constructor for Signature class
  Signature(ReadWriteMemRegion digest) noexcept : CryptoObject{ digest }, Serializable{}
  {
    aralog_.LogDebug() << "In Signature constructor";
  }

  /// @brief Signature object initialized
  /// @trace SWS_CRYPT_23302
  static const CryptoObjectType kObjectType = CryptoObjectType::kSignature;

  /// @brief Unique smart pointer of the interface
  /// @trace SWS_CRYPT_23301
  using Uptrc = std::unique_ptr<const Signature>;

  /// @brief Get an ID of hash algorithm used for this signature object production.
  /// @returns ID of used hash algorithm only (without signature algorithm specification)
  /// @trace SWS_CRYPT_23311
  virtual CryptoPrimitiveId::AlgId GetHashAlgId() const noexcept = 0;

  /// @brief Get the hash size required by current signature algorithm.
  /// @returns required hash size in bytes
  /// @trace SWS_CRYPT_23312
  virtual std::size_t GetRequiredHashSize() const noexcept = 0;

  /// @brief Getter method for Signature value.
  ReadWriteMemRegion GetSignatureValue() const noexcept
  {
    aralog_.LogDebug() << "In Signature::GetSignatureValue()";
    return GetValue();
  }

 private:
  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "SIGN", "Signature Crypto Object", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_INC_PUBLIC_CRYOBJ_SIGNATURE_H_
