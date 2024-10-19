// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : secret_seed.h
//
// Purpose     : This file contains definitions for SecretSeed abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_SECRET_SEED_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_SECRET_SEED_H_

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/crypto_object.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Secret Seed object interface.
/// This object contains a raw bit sequence of specific length (without any filtering of allowed/disallowed
/// values)!
/// The secret seed value can be loaded only to a non-key input of a cryptographic transformation context
/// (like IV/salt/nonce)!
/// Bit length of the secret seed is specific to concrete crypto algorithm and corresponds to maximum of its
/// input/output/salt block-length.
/// @trace SWS_CRYPT_23000
class SecretSeed : public RestrictedUseObject
{
 public:
  /// @brief Static mapping of this interface to specific value of @c CryptoObjectType enumeration.
  /// @trace SWS_CRYPT_23003
  static const CryptoObjectType kObjectType = CryptoObjectType::kSecretSeed;

  /// @brief Unique smart pointer of a constant interface instance.
  /// @trace SWS_CRYPT_23001
  using Uptrc = std::unique_ptr<const SecretSeed>;

  /// @brief Unique smart pointer of a volatile interface instance.
  /// @trace SWS_CRYPT_23002
  using Uptr = std::unique_ptr<SecretSeed>;
  /// @brief Clone this Secret Seed object to new session object.
  /// Created object instance is session and non-exportable, @c AllowedUsageFlags attribute of the "cloned"
  /// object is identical to this attribute of the source object!
  /// If size of the @c xorDelta argument is less than the value size of this seed then only correspondent
  /// number of leading bytes of the original seed should be XOR-ed, but the rest should be copied without
  /// change.
  /// If size of the @c xorDelta argument is larger than the value size of this seed then extra bytes of
  /// the @c xorDelta should be ignored.
  /// @param[in] xorDelta  optional "delta" value that must be XOR-ed with the "cloned" copy of the original
  /// seed.
  /// @returns unique smart pointer to "cloned" session @c SecretSeed object
  /// @trace SWS_CRYPT_23011
  virtual ara::core::Result<SecretSeed::Uptr> Clone(ReadOnlyMemRegion xorDelta = ReadOnlyMemRegion()) const
    noexcept
    = 0;

  /// @brief Set value of this seed object as a "jump" from an initial state to specified number of steps,
  /// according to "counting" expression defined by a cryptographic algorithm associated with this object.
  /// @c steps may have positive and negative values that correspond to forward and backward direction of the
  /// "jump" respectively, but 0 value means only copy @c from value to this seed object.
  /// Seed size of the @c from argument always must be greater or equal of this seed size.
  /// @param[in] from  source object that keeps the initial value for jumping from
  /// @param[in] steps  number of steps for the "jump"
  /// @returns reference to this updated object
  /// @error SecurityErrorDomain::kIncompatibleObject  if this object and the @c from argument are associated
  /// with incompatible cryptographic algorithms
  /// @error SecurityErrorDomain::kInvalidInputSize  if value size of the @c from seed is less then
  /// value size of this one
  /// @trace SWS_CRYPT_23012
  virtual ara::core::Result<void> JumpFrom(const SecretSeed& from, std::int64_t steps) noexcept = 0;

  /// @brief Set next value of the secret seed according to "counting" expression defined by a cryptographic
  /// algorithm associated with this object.
  /// If the associated cryptographic algorithm doesn't specify a "counting" expression then generic increment
  /// operation must be implemented as default (little-endian notation, i.e. first byte is least significant).
  /// @trace SWS_CRYPT_23013
  virtual SecretSeed& Next() noexcept = 0;

  /// @brief Set value of this seed object as a "jump" from it's current state to specified number of steps,
  /// according to "counting" expression defined by a cryptographic algorithm associated with this object.
  /// @c steps may have positive and negative values that correspond to forward and backward direction of the
  /// "jump" respectively, but 0 value means no changes of the current seed value.
  /// @param[in] steps  number of "steps" for jumping (forward or backward) from the current state
  /// @returns reference to this updated object
  /// @trace SWS_CRYPT_23014
  virtual SecretSeed& Jump(std::int64_t steps) noexcept = 0;

  /// @brief XOR value of this seed object with another one and save result to this object.
  /// If seed sizes in this object and in the @c source argument are different then only correspondent number
  /// of leading bytes in this seed object should be updated.
  /// @param[in] source  right argument for the XOR operation
  /// @returns reference to this updated object
  /// @trace SWS_CRYPT_23015
  virtual SecretSeed& operator^=(const SecretSeed& source) noexcept = 0;

  /// @brief XOR value of this seed object with provided memory region and save result to this object.
  /// If seed sizes in this object and in the @c source argument are different then only correspondent number
  /// of leading bytes of this seed object should be updated.
  /// @param[in] source  right argument for the XOR operation
  /// @returns reference to this updated object
  /// @trace SWS_CRYPT_23016
  virtual SecretSeed& operator^=(ReadOnlyMemRegion source) noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_SECRET_SEED_H_
