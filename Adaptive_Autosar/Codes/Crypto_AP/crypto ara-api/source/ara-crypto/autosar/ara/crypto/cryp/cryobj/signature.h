// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature.h
//
// Purpose     : This file contains definitions for Signature abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_SIGNATURE_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_SIGNATURE_H_

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
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_SIGNATURE_H_
