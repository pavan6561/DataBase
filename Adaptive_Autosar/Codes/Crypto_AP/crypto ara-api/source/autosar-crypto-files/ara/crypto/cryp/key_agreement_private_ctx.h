// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_agreement_private_ctx.h
//
// Purpose     : This file contains definitions for KeyAgreementPrivateCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_KEY_AGREEMENT_PRIVATE_CTX_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_KEY_AGREEMENT_PRIVATE_CTX_H_

#include <ara/core/result.h>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
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
/// @brief Key Agreement Private key Context interface (Diffie Hellman or conceptually similar).
/// @trace SWS_CRYPT_21300
class KeyAgreementPrivateCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of this interface.
  /// @trace SWS_CRYPT_21301
  using Uptr = std::unique_ptr<KeyAgreementPrivateCtx>;

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_21302 
  virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

  /// @brief Produce a common secret seed via execution of the key-agreement algorithm between this private
  /// key and a public key of another side.
  /// Produced @c SecretSeed object has following attributes: session, non-exportable,
  /// AlgID (this Key-Agreement Algorithm ID).
  /// @param[in] otherSideKey  the public key of the other side of the Key-Agreement
  /// @param[in] allowedUsage  the allowed usage scope of the target seed
  /// @returns unique pointer to @c SecretSeed object, which contains the key material produced by the
  /// Key-Agreement algorithm
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @error SecurityErrorDomain::kIncompatibleObject  if the public and private keys correspond to
  /// different algorithms
  /// @trace SWS_CRYPT_21311
  virtual ara::core::Result<SecretSeed::Uptrc> AgreeSeed(
    const PublicKey& otherSideKey, SecretSeed::Usage allowedUsage = kAllowKdfMaterialAnyUsage) const noexcept
    = 0;

  /// @brief Produce a common symmetric key via execution of the key-agreement algorithm between this
  /// private key and a public key of another side.
  /// Produced @c SymmetricKey object has following attributes: session, non-exportable.
  /// This method can be used for direct production of the target key, without creation of the intermediate
  /// @c SecretSeed object.
  /// @param[in] otherSideKey  the public key of the other side of the Key-Agreement
  /// @param[in] kdf  the Context of a Key Derivation Function, which should be used for the target key
  /// production
  /// @param[in] targetAlgId  identifier of the target symmetric algorithm
  /// (also defines a target key-length)
  /// @param[in] allowedUsage  the allowed usage scope of the target key
  /// @param[in] salt  an optional salt value (if used, it should be unique for each instance of the
  /// target key)
  /// @param[in] ctxLabel  an optional application specific "context label" (it can identify purpose of the
  /// target key and/or communication parties)
  /// @returns a unique pointer to @c SecretSeed object, which contains the key material produced by the
  /// Key-Agreement algorithm
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @error SecurityErrorDomain::kIncompatibleObject  if the public and private keys correspond to different
  /// algorithms
  /// @trace SWS_CRYPT_21312
  virtual ara::core::Result<SymmetricKey::Uptrc> AgreeKey(const PublicKey& otherSideKey,
    KeyDerivationFunctionCtx& kdf, AlgId targetAlgId, AllowedUsageFlags allowedUsage,
    ReadOnlyMemRegion salt = ReadOnlyMemRegion(), ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) const
    noexcept
    = 0;

  /// @brief Set (deploy) a key to the key agreement private algorithm context.
  /// @param[in] key  the source key object
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this private key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_21313
  virtual ara::core::Result<void> SetKey(const PrivateKey& key) noexcept = 0;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_21314
  virtual ara::core::Result<void> Reset() noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_KEY_AGREEMENT_PRIVATE_CTX_H_
