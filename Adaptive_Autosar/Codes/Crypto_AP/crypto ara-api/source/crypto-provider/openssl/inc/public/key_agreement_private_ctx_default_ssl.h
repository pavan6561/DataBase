// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_agreement_private_ctx_default_ssl.h
//
// Purpose   : This file contains definitions for KeyAgreementPrivateCtxDefaultssl class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_AGREEMENT_PRIVATE_CTX_DEFAULT_SSL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_AGREEMENT_PRIVATE_CTX_DEFAULT_SSL_H_

#include <openssl/evp.h>
#include <openssl/rsa.h>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/key_agreement_private_ctx.h"
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
/// @brief namespace for cryp.
namespace cryp
{
/// @brief Key Agreement Private key Context interface (Diffie Hellman or conceptually similar).
/// @trace SWS_CRYPT_21300
class KeyAgreementPrivateCtxDefaultssl : public KeyAgreementPrivateCtx
{
 public:
  /// @brief Constructor for KeyAgreementPrivateCtxDefaultssl class.
  KeyAgreementPrivateCtxDefaultssl(AlgId algo) noexcept : algId_{ algo }
  {
  }

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_21302 
  ExtensionService::Uptr GetExtensionService() const noexcept;

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
  ara::core::Result<SecretSeed::Uptrc> AgreeSeed(const PublicKey& otherSideKey,
    SecretSeed::Usage allowedUsage = kAllowKdfMaterialAnyUsage) const noexcept override;

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
  ara::core::Result<SymmetricKey::Uptrc> AgreeKey(const PublicKey& otherSideKey,
    KeyDerivationFunctionCtx& kdf, AlgId targetAlgId, AllowedUsageFlags allowedUsage,
    ReadOnlyMemRegion salt = ReadOnlyMemRegion(), ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) const
    noexcept override;

  /// @brief Set (deploy) a key to the key agreement private algorithm context.
  /// @param[in] key  the source key object
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this private key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_21313
  ara::core::Result<void> SetKey(const PrivateKey& key) noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_21314
  ara::core::Result<void> Reset() noexcept override;

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use,
  /// and @c false otherwise
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override
  {
    return isInitialize_;
  }

 private:
  /// @brief pointer to the openssl private key.
  EVP_PKEY* privateKey_;

  /// @brief algorithm Id for this context.
  AlgId algId_;

  /// @brief flag to indecate whether ctx is initialized or not.
  bool isInitialize_{ false };

  ///@brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "KAPC", "Key agreement private context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_AGREEMENT_PRIVATE_CTX_DEFAULT_SSL_H_
