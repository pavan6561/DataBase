// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_decapsulator_private_ctx.h
//
// Purpose     : This file contains definitions for KeyDecapsulatorPrivateCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_KEY_DECAPSULATOR_PRIVATE_CTX_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_KEY_DECAPSULATOR_PRIVATE_CTX_H_

#include <ara/core/result.h>

#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace cryp
{
/// @brief Asymmetric %Key Encapsulation Mechanism (KEM) Private key Context interface.
/// @trace SWS_CRYPT_21400
class KeyDecapsulatorPrivateCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_21401
  using Uptr = std::unique_ptr<KeyDecapsulatorPrivateCtx>;

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_21402
  virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

  /// @brief Decapsulate key material.
  /// Returned Key Material object should be used for derivation of a symmetric key.
  /// Produced @c SecretSeed object has following attributes: session, non-exportable,
  /// AlgID = this KEM AlgID.
  /// @param[in] input  a buffer with the encapsulated seed (its size should be equal
  /// @c GetEncapsulatedSize() bytes)
  /// @param[in] allowedUsage  the allowed usage scope of the target seed
  /// @returns unique smart pointer to @c SecretSeed object, which keeps the key material decapsulated
  /// from the input buffer
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a private key
  /// value
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the @c output.size() is not enough to save
  /// the decapsulation result
  /// @trace SWS_CRYPT_21411
  virtual ara::core::Result<SecretSeed::Uptrc> DecapsulateSeed(
    ReadOnlyMemRegion input, SecretSeed::Usage allowedUsage = kAllowKdfMaterialAnyUsage) const noexcept
    = 0;

  /// @brief Decapsulate Key Encryption Key (KEK).
  /// Produced @c SymmetricKey object has following attributes: session, non-exportable, Key Usage:
  /// @c kAllowKeyImporting.
  /// This method can be used for direct production of the target key, without creation of the intermediate
  /// @c SecretSeed object.
  /// @param[in] input  an input buffer (its size should be equal @c GetEncapsulatedSize() bytes)
  /// @param[in] kdf  a context of a key derivation function, which should be used for the target
  /// KEK production
  /// @param[in] kekAlgId  an algorithm ID of the target KEK
  /// @param[in] salt  an optional salt value (if used, it should be unique for each instance
  /// of the target key)
  /// @param[in] ctxLabel  an pptional application specific "context label" (it can identify purpose
  /// of the target key and/or communication parties)
  /// @returns unique smart pointer to a symmetric key object derived from a key material decapsulated from
  /// the input block
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a private key
  /// value
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c kekAlgId specifies incorrect algorithm
  /// @error SecurityErrorDomain::kInvalidInputSize  if <tt>(input.size() <>
  /// this->GetEncapsulatedSize())</tt>
  /// @trace SWS_CRYPT_21412
  virtual ara::core::Result<SymmetricKey::Uptrc> DecapsulateKey(ReadOnlyMemRegion input,
    KeyDerivationFunctionCtx& kdf, AlgId kekAlgId, ReadOnlyMemRegion salt = ReadOnlyMemRegion(),
    ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) const noexcept
    = 0;

  /// @brief Set (deploy) a key to the key decapsulator private algorithm context.
  /// @param[in] key  the source key object
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this private key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_21413
  virtual ara::core::Result<void> SetKey(const PrivateKey& key) noexcept = 0;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_21414
  virtual ara::core::Result<void> Reset() noexcept = 0;

  /// @brief Get entropy (bit-length) of the key encryption key (KEK) material.
  /// For RSA system the returned value corresponds to the length of module N (minus 1).
  /// For DH-like system the returned value corresponds to the length of module q (minus 1).
  /// @returns entropy of the KEK material in bits
  /// @trace SWS_CRYPT_21415
  virtual std::size_t GetKekEntropy() const noexcept = 0;

  /// @brief Get fixed size of the encapsulated data block.
  /// @returns size of the encapsulated data block in bytes
  /// @trace SWS_CRYPT_21416
  virtual std::size_t GetEncapsulatedSize() const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // ARA_CRYPTO_CRYP_KEY_DECAPSULATOR_PRIVATE_CTX_H_
