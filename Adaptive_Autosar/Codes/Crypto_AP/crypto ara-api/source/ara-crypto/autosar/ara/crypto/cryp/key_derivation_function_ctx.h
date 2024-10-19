// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_derivation_function_ctx.h
//
// Purpose     : This file contains definitions for KeyDerivationFunctionCtx class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_KEY_DERIVATION_FUNCTION_CTX_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_KEY_DERIVATION_FUNCTION_CTX_H_

#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Key Derivation Function interface.
/// @trace RS_CRYPTO_02103
class KeyDerivationFunctionCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @traceid{SWS_CRYPT_21501}
  using Uptr = std::unique_ptr<KeyDerivationFunctionCtx>;

  /// @brief Add an application filler value stored in a (non-secret) ReadOnlyMemRegion.
  /// If <tt>(GetFillerSize() == 0)</tt>, then this method call will be ignored.
  /// @param[in] appFiller  the application specific "filler" value
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the appFiller is incorrect,
  /// i.e. if <tt>(appFiller.size() < GetFillerSize())</tt>;
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_21510
  // virtual ara::core::Result<void> AddAppFiller(const ReadOnlyMemRegion appFiller) noexcept = 0;

  /// @brief Add a secret application filler value stored in a SecretSeed object.
  /// If <tt>(GetFillerSize() == 0)</tt>, then this method call will be ignored.
  /// @param[in] appFiller  the application specific "filler" value
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the appFiller is incorrect,
  /// i.e. if <tt>(appFiller.size() < GetFillerSize())</tt>;
  /// @trace SWS_CRYPT_21511
  // virtual ara::core::Result<void> AddSecretAppFiller(const SecretSeed& appFiller) noexcept = 0;

  /// @brief Add a salt value stored in a (non-secret) ReadOnlyMemRegion.
  /// @param[in] salt  a salt value (if used, it should be unique for each instance of the target key)
  /// @trace SWS_CRYPT_21512
  virtual ara::core::Result<void> AddSalt(ReadOnlyMemRegion salt) noexcept = 0;

  /// @brief Add a secret salt value stored in a SecretSeed object.
  /// @param[in] salt  a salt value (if used, it should be unique for each instance of the target key)
  /// @trace SWS_CRYPT_21513
  virtual ara::core::Result<void> AddSecretSalt(const SecretSeed& salt) noexcept = 0;

  /// @brief Configure the number of iterations that will be applied by default.
  /// Implementation can restrict minimal and/or maximal value of the iterations number.
  /// @param[in] iterations  the required number of iterations of the base function (0 means implementation
  /// default number)
  /// @returns actual number of the iterations configured in the context now (after this method call)
  /// @trace SWS_CRYPT_21514
  virtual std::uint32_t ConfigIterations(std::uint32_t iterations = 0) noexcept = 0;

  /// @brief Derive a symmetric key from the provided key material and provided context configuration.
  /// @param[in] isSession  the "session" (or "temporary") attribute for the target key (if @c true)
  /// @param[in] isExportable  the exportability attribute for the target key (if @c true)
  /// @returns unique smart pointer to the created instance of derived symmetric key
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not sufficiently initialized
  /// @trace SWS_CRYPT_21515
  virtual ara::core::Result<SymmetricKey::Uptrc> DeriveKey(
    bool isSession = true, bool isExportable = false) const noexcept
    = 0;

  /// @brief Derive a "slave" key material (secret seed) from the provided "master" key material and
  /// provided context configuration.
  /// @param[in] isSession  the "session" (or "temporary") attribute for the target key (if @c true)
  /// @param[in] isExportable  the exportability attribute for the target key (if @c true)
  /// @returns unique smart pointer to the created @c SecretSeed object
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not sufficiently initialized
  /// @trace SWS_CRYPT_21516
  virtual ara::core::Result<SecretSeed::Uptrc> DeriveSeed(
    bool isSession = true, bool isExportable = false) const noexcept
    = 0;

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_21517
  virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

  /// @brief Get the fixed size of an application specific "filler" required by this context instance.
  /// If this instance of the key derivation context does not support filler values, 0 shall be returned.
  /// @returns size of the application specific filler in bytes
  /// Returned value is constant for this instance of the key derivation context, i.e. independent from
  /// configuration by the @c Init() call.
  /// @trace SWS_CRYPT_21518
  // virtual std::size_t GetFillerSize() const noexcept = 0;

  /// @uptrace{RS_CRYPTO_02103}
  /// @brief Get the fixed size of the target key ID required by diversification algorithm.
  /// Returned value is constant for each instance of the interface, i.e. independent from configuration by
  /// @returns size of the key ID in bytes set by Init() call.
  /// @trace SWS_CRYPT_21519
  virtual std::size_t GetKeyIdSize() const noexcept = 0;

  /// @brief Get the symmetric algorithm ID of target (slave) key.
  /// If the context was not configured yet by a call of the Init() method then kAlgIdUndefined should be
  /// @returns the symmetric algorithm ID of the target key, configured by the last call of the Init() method
  /// returned.
  /// @trace SWS_CRYPT_21520
  virtual CryptoAlgId GetTargetAlgId() const noexcept = 0;

  /// @brief Get allowed key usage of target (slave) key.
  /// The returned value depends on the source key-material allowed usage flags and the argument @c
  /// allowedUsage of last call of the @c Init() method.
  /// If the context has not yet been configured by a call of the Init() method, the allowed usage flags
  /// of the source key-material shall be returned.
  /// If the context has not yet been configured by a call of the Init() method and no source key-material
  /// has been set either, kAllowKdfMaterialAnyUsage shall be returned.
  /// @returns allowed key usage bit-flags of target keys
  /// @trace SWS_CRYPT_21521
  virtual AllowedUsageFlags GetTargetAllowedUsage() const noexcept = 0;

  /// @brief Get the bit-length of target (diversified) keys.
  /// Returned value is configured by the context factory method, i.e. independent from configuration by
  /// @returns the length of target (diversified) key in bits set by Init() calls.
  /// @trace SWS_CRYPT_21522
  virtual std::size_t GetTargetKeyBitLength() const noexcept = 0;

  /// @brief Initialize this context by setting at least the target key ID.
  /// The byte sequence provided via argument ctxLabel can include a few fields with different meaning
  /// separated by single 0x00 byte.
  /// If <tt>(targetAlgId == kAlgIdAny)</tt> then a diversified key can be loaded to any symmetric context
  /// that supports the same key length (if the "allowed usage" flags are also satisfied)!
  /// @param[in] targetKeyId  ID of the target key
  /// @param[in] targetAlgId  the identifier of the target symmetric crypto algorithm
  /// @param[in] allowedUsage  bit-flags that define a list of allowed transformations' types in which the
  /// target key may be used
  /// @param[in] ctxLabel  an optional application specific "context label" (this can identify the purpose
  /// of the target key and/or communication parties)
  /// @error SecurityErrorDomain::kIncompatibleArguments  if targetAlgId specifies a cryptographic
  /// algorithm different from a symmetric one with key length equal to GetTargetKeyBitLength();
  /// @error SecurityErrorDomain::kUsageViolation  if allowedUsage specifies more usages of the derived
  /// key-material than the source key-material, i.e. usage of the derived key-material may not be
  /// expanded beyond what the source key-material allows
  /// @trace SWS_CRYPT_21523
  virtual ara::core::Result<void> Init(ReadOnlyMemRegion targetKeyId, CryptoAlgId targetAlgId = kAlgIdAny,
    AllowedUsageFlags allowedUsage = kAllowKdfMaterialAnyUsage,
    ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) noexcept
    = 0;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_21524
  virtual ara::core::Result<void> Reset() noexcept = 0;

  /// @brief Set (deploy) key-material to the key derivation algorithm context.
  /// @param[in] sourceKM  the source key-material
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if deriving a key is prohibited by
  /// the "allowed usage" restrictions of the provided source key-material
  /// @error SecurityErrorDomain::kBruteForceRisk  if key length of the @c sourceKm is below of
  /// an internally defined limitation
  /// @trace SWS_CRYPT_21525
  virtual ara::core::Result<void> SetSourceKeyMaterial(const RestrictedUseObject& sourceKM) noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_KEY_DERIVATION_FUNCTION_CTX_H_
