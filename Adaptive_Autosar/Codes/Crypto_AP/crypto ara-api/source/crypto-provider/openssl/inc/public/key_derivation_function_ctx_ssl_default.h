// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_derivation_function_ctx_ssl_default.h
//
// Purpose     : This file contains definitions for KeyDerivationFunctionCtxSSLPBKDF class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_DERIVATION_FUNCTION_CTX_SSL_DEFAULT_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_DERIVATION_FUNCTION_CTX_SSL_DEFAULT_H_

#include <cstdint>

#include <ara/log/logging.h>
#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/params.h>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "crypto_config.h"
#include "crypto_primitive_id_impl.h"
#include "secret_seed_impl.h"
#include "symmetric_key_impl.h"
#include "extension_service_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief %Key Derivation Function interface.
/// @trace SWS_CRYPT_21500
class KeyDerivationFunctionCtxSSLPBKDF : public KeyDerivationFunctionCtx
{
 public:
  /// @brief Constructor for KeyDerivationFunctionCtxSSLPBKDF class.
  KeyDerivationFunctionCtxSSLPBKDF() noexcept;

  /// @brief Initialize this context by setting at least the target key ID.
  /// The byte sequence provided via argument @c ctxLabel can include a few fields with different meaning
  /// separated by single @c 0x00 byte.
  /// If <tt>(targetAlgId == kAlgIdAny)</tt> then a diversified key can be loaded to any symmetric context
  /// that supports the same key length (if the "allowed usage" flags are also satisfied)!
  /// @param[in] targetKeyId  ID of the target key
  /// @param[in] targetAlgId  the identifier of the target symmetric crypto algorithm
  /// @param[in] allowedUsage  bit-flags that define a list of allowed transformations' types in which the
  /// target key may be used
  /// @param[in] ctxLabel  an optional application specific "context label" (this can identify the purpose of
  /// the target key and/or communication parties)
  /// @error SecurityErrorDomain::kIncompatibleArguments  if @c targetAlgId specifies a cryptographic
  /// algorithm different from a symmetric one with key length equal to @c GetTargetKeyBitLength();
  /// @error SecurityErrorDomain::kUsageViolation  if @c allowedUsage specifies more usages of the derived
  /// key-material than the source key-material, i.e. usage of the derived key-material may not be
  /// expanded beyond what the source key-material allows
  /// @trace SWS_CRYPT_21523_002, SWS_CRYPT_21523_003, SWS_CRYPT_21523_004
  ara::core::Result<void> Init(ReadOnlyMemRegion targetKeyId, AlgId targetAlgId = kAlgIdAny,
    AllowedUsageFlags allowedUsage = kAllowKdfMaterialAnyUsage,
    ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) noexcept override;

  /// @brief Get the fixed size of the target key ID required by diversification algorithm.
  /// Returned value is constant for each instance of the interface, i.e. independent from configuration by
  /// the @c Init() call.
  /// @returns size of the key ID in bytes
  /// @trace SWS_CRYPT_21519
  std::size_t GetKeyIdSize() const noexcept override
  {
    static uint8_t constexpr ktargetKeyIdNotSupported{ 0 };
    return ktargetKeyIdNotSupported;
  }

  /// @brief Get the symmetric algorithm ID of target (slave) key.
  /// If the context was not configured yet by a call of the @c Init() method then @c
  /// kAlgIdUndefined should be returned.
  /// @returns the symmetric algorithm ID of the target key, configured by the last call of the @c Init()
  /// method.
  /// @trace SWS_CRYPT_21520_001, SWS_CRYPT_21520_002
  AlgId GetTargetAlgId() const noexcept override
  {
    aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::GetTargetAlgId()";
    return targetAlgorithmId_;
  }

  /// @brief Get allowed key usage of target (slave) key.
  /// The returned value depends on the source key-material allowed usage flags and the argument
  /// @c allowedUsage of last call of the @c Init() method.
  /// If the context has not yet been configured by a call of the @c Init() method, the allowed usage flags
  /// of the source key-material shall be returned.
  /// If the context has not yet been configured by a call of the @c Init() method and no source key-material
  /// has been set either, kAllowKdfMaterialAnyUsage shall be returned.
  /// @returns allowed key usage bit-flags of target keys
  /// @trace SWS_CRYPT_21521_001, SWS_CRYPT_21521_002, SWS_CRYPT_21521_003
  AllowedUsageFlags GetTargetAllowedUsage() const noexcept override
  {
    aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::GetTargetAllowedUsage()";
    return allowedUsageForDerivedKey_;
  }

  /// @brief Get the bit-length of target (diversified) keys.
  /// Returned value is configured by the context factory method, i.e. independent from configuration by
  /// the @c Init() calls.
  /// @returns the length of target (diversified) key in bits
  /// @trace SWS_CRYPT_21522
  std::size_t GetTargetKeyBitLength() const noexcept override;

  /// @brief Add a salt value stored in a (non-secret) ReadOnlyMemRegion.
  /// @param[in] salt  a salt value (if used, it should be unique for each instance of the target key)
  /// @trace SWS_CRYPT_21510_001, SWS_CRYPT_00608
  ara::core::Result<void> AddSalt(ReadOnlyMemRegion salt) noexcept override;

  /// @brief Add a secret salt value stored in a SecretSeed object.
  /// @param[in] salt  a salt value (if used, it should be unique for each instance of the target key)
  /// @trace SWS_CRYPT_21513, SWS_CRYPT_00608
  ara::core::Result<void> AddSecretSalt(const SecretSeed& salt) noexcept override;

  /// @brief Configure the number of iterations that will be applied by default.
  /// Implementation can restrict minimal and/or maximal value of the iterations number.
  /// @param[in] iterations  the required number of iterations of the base function (0 means implementation
  /// default number)
  /// @returns actual number of the iterations configured in the context now (after this method call)
  /// @trace SWS_CRYPT_21514_001, SWS_CRYPT_21514_002
  std::uint32_t ConfigIterations(std::uint32_t iterations = 0) noexcept override;

  /// @brief Derive a symmetric key from the provided key material and provided context configuration.
  /// @param[in] isSession  the "session" (or "temporary") attribute for the target key (if @c true)
  /// @param[in] isExportable  the exportability attribute for the target key (if @c true)
  /// @returns unique smart pointer to the created instance of derived symmetric key
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not sufficiently initialized
  /// @trace SWS_CRYPT_21515_001, SWS_CRYPT_21515_002
  ara::core::Result<SymmetricKey::Uptrc> DeriveKey(bool isSession = true, bool isExportable = false) const
    noexcept override;

  /// @brief Derive a "slave" key material (secret seed) from the provided "master" key material and
  /// provided context configuration.
  /// @param[in] isSession  the "session" (or "temporary") attribute for the target key (if @c true)
  /// @param[in] isExportable  the exportability attribute for the target key (if @c true)
  /// @returns unique smart pointer to the created @c SecretSeed object
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not sufficiently initialized
  /// @trace SWS_CRYPT_21516_001, SWS_CRYPT_21516_002
  ara::core::Result<SecretSeed::Uptrc> DeriveSeed(bool isSession = true, bool isExportable = false) const
    noexcept override;

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_21517
  ExtensionService::Uptr GetExtensionService() const noexcept;

  /// @brief Set (deploy) key-material to the key derivation algorithm context.
  /// @param[in] sourceKM  the source key-material
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if deriving a key is prohibited by
  /// the "allowed usage" restrictions of the provided source key-material
  /// @error SecurityErrorDomain::kBruteForceRisk  if key length of the @c sourceKm is below of
  /// an internally defined limitation
  /// @trace SWS_CRYPT_21525_001, SWS_CRYPT_21525_002, SWS_CRYPT_21525_003, SWS_CRYPT_21525_004
  ara::core::Result<void> SetSourceKeyMaterial(const RestrictedUseObject& sourceKM) noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_21524
  ara::core::Result<void> Reset() noexcept override;

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override
  {
    aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::GetCryptoPrimitiveId()";
    // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
    // pointer which will have primitive information related to this particular ctx.
    CryptoPrimitiveId::Uptr primitiveInfoPtr{ std::make_unique<CryptoPrimitiveIdImpl>(
      algoIdentification_->GetAlgoName(algorithmids::kPBKDF2), algorithmids::kPBKDF2) };
    return primitiveInfoPtr;
  }

  /// @brief Destructor for KeyDerivationFunctionCtxSSLPBKDF class.
  ~KeyDerivationFunctionCtxSSLPBKDF() noexcept override;

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use,
  /// and @c false otherwise
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override
  {
    aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::IsInitialized()";
    return isCtxInitialized_;
  }

 private:
  /// @brief variable to store the number of KDF parameters.
  static uint8_t constexpr stkNoOfKDKParameters_{ 7 };

  /// @brief Method to get the expected key size from the given target algorithm Id.
  /// @param[in] targetAlgId ID for the target algorithm of the key.
  size_t GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept;

  /// @brief Method to get the expected IV size from the given target algorithm Id.
  /// @param[in] targetAlgId ID for the target algorithm of the key.
  size_t GetIVSizefromAlgoId(AlgId targetAlgId) const noexcept;

  /// @brief Method to configure openssl KDF ctx.
  /// @brief Pointer to the openssl parameter buffer.
  void ConfigureKdfCtx() noexcept;

  /// @brief target algorithm Id.
  AlgId targetAlgorithmId_{ kAlgIdUndefined };

  /// @brief buffer to store the salt value.
  ara::core::Vector<uint8_t> kdfSalt_{};

  /// @brief Variable to store key related information.
  ara::core::Vector<char> targetKeyIdValue_{};

  /// @brief buffer to store the password.
  ara::core::Vector<uint8_t> password_{};

  /// @brief pointer for the openssl KDF context.
  EVP_KDF_CTX* kdfctx_{ nullptr };

  /// @brief  Variable to hold the openssl configuration parameters.
  OSSL_PARAM params_[stkNoOfKDKParameters_]{};

  /// @brief variable to hold the usage flags for the derived key.
  AllowedUsageFlags allowedUsageForDerivedKey_{ kAllowKdfMaterialAnyUsage };

  /// @brief variable to hold the usage flags for the key material.
  AllowedUsageFlags allowedUsageForKeyMaterial_{ kAllowKdfMaterialAnyUsage };

  /// @brief flag to indicate if context is initialized.
  bool isCtxInitialized_{ false };

  /// @brief flag to indecate whether sourceKM is available or not.
  bool isSourceKMavailable_{false};

  /// @brief shared pointer for algo identification class.
  std::shared_ptr<AlgorithmIdentification> algoIdentification_{
    AlgorithmIdentification::GetAlogIdentification()
  };

  /// @brief Pointer to the crypto configuration.
  std::shared_ptr<ara::crypto::config::CryptoConfig> configInstance_{
    ara::crypto::config::CryptoConfig::GetCryptoConfigInstance()
  };

  /// @brief variable to hold the iteration for the key derivation.
  std::uint32_t itr_{ configInstance_->GetKdfIterationValue() };

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "KDFC", "Key Derivation function context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_DERIVATION_FUNCTION_CTX_SSL_DEFAULT_H_
