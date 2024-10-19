// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_encapsulator_public_ctx_default_ssl.h
//
// Purpose   : This file contains definitions for KeyEncapsulatorPublicCtxDefaultSsl class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_ENCAPSULATOR_PUBLIC_CTX_DEFAULT_SSL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_ENCAPSULATOR_PUBLIC_CTX_DEFAULT_SSL_H_

#include <ara/log/logging.h>
#include <openssl/conf.h>
#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/params.h>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/key_encapsulator_public_ctx.h"
#include "crypto_primitive_id_impl.h"
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
/// @brief Asymmetric %Key Encapsulation Mechanism (KEM) Public key Context interface.
/// @trace SWS_CRYPT_21800
class KeyEncapsulatorPublicCtxDefaultSsl : public KeyEncapsulatorPublicCtx
{
 public:
  /// @brief Construcotr for KeyEncapsulatorPublicCtxDefaultSsl class.
  KeyEncapsulatorPublicCtxDefaultSsl(AlgId algo) noexcept : algId_{ algo }
  {
  }

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_22210
  ExtensionService::Uptr GetExtensionService() const noexcept override;

  /// @brief Add the content to be encapsulated (payload) according to RFC 5990 ("keying data").
  /// At the moment only SymmetricKey and SecretSeed objects are supported.
  /// @param[in] keyingData  the payload to be protected
  /// @trace SWS_CRYPT_21810
  ara::core::Result<void> AddKeyingData(RestrictedUseObject& keyingData) noexcept override;

  /// @brief Encapsulate %Key Encryption %Key (KEK).
  /// @param[in] kdf  a context of a key derivation function, which should be used for the target
  /// KEK production
  /// @param[in] kekAlgId  an algorithm ID of the target KEK
  /// @param[in] salt  an optional salt value (if used, it should be unique for each instance of the
  /// target key)
  /// @param[in] ctxLabel  an optional application specific "context label" (it can identify purpose of the
  /// target key and/or communication parties)
  /// @returns unique smart pointer to a symmetric key object derived from a randomly generated material
  /// encapsulated to the output buffer
  /// Only first @c GetEncapsulatedSize() bytes of the output buffer should be updated by this method.
  /// Produced @c SymmetricKey object has following attributes: session, non-exportable, Allowed Key Usage:
  /// @c kAllowKeyExporting.
  /// This method can be used for direct production of the target key, without creation of the intermediate
  /// @c SecretSeed object.
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by
  /// a public key value
  /// @error SecurityErrorDomain::kInvalidArgument  if @c kekAlgId specifies incorrect algorithm
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the @c output.size() is not enough to save
  /// the encapsulation result
  /// @trace SWS_CRYPT_21813
  ara::core::Result<ara::core::Vector<ara::core::Byte>> Encapsulate(KeyDerivationFunctionCtx& kdf,
    AlgId kekAlgId, ReadOnlyMemRegion salt = ReadOnlyMemRegion(),
    ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Set (deploy) a key to the key encapsulator public algorithm context.
  /// @param[in] key  the source key object
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_21815
  ara::core::Result<void> SetKey(const PublicKey& key) noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_21816
  ara::core::Result<void> Reset() noexcept override;

  /// @brief Get entropy (bit-length) of the key encryption key (KEK) material.
  /// For RSA system the returned value corresponds to the length of module N (minus 1).
  /// For DH-like system the returned value corresponds to the length of module q (minus 1).
  /// @returns entropy of the KEK material in bits
  /// @trace SWS_CRYPT_21817
  std::size_t GetKekEntropy() const noexcept override
  {
    // For RSA_PKCS#1V_1.5 this size is 256 (module size) - 11 (padding size)
    return 245;
  }

  /// @brief Get fixed size of the encapsulated data block.
  /// @returns size of the encapsulated data block in bytes
  /// @trace SWS_CRYPT_21818
  std::size_t GetEncapsulatedSize() const noexcept override
  {
    return 256;
  }

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use,
  /// and @c false otherwise
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override
  {
    return isInitialize_;
  }

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override
  {
    // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
    // pointer which will have primitive information related to this perticular ctx.
    CryptoPrimitiveId::Uptr primitiveInfoPtr(std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_));
    return primitiveInfoPtr;
  }

 private:
  /// @brief Configure the encrypt ctx of the Openssl.
  void ConfigureEncryptorCtx(EVP_PKEY_CTX* keyCtx) const noexcept;

  /// @brief Get the openssl cipher mode for this context according to algoId.
  const EVP_CIPHER* GetCipherMode(AlgId algoId) const noexcept;

  /// @brief method to wrap the key value.
  ara::core::Vector<uint8_t> WrapKey(const SymmetricKey& kek, AlgId kekalgId) const noexcept;

  /// @brief Method to encrypt the plain text.
  ara::core::Vector<uint8_t> GenerateCipherText(ReadOnlyMemRegion in) const noexcept;

  /// @brief Method to generate entropy.
  ara::core::Vector<uint8_t> GenerateEntropy() const noexcept;

  /// @brief pointer to the openssl public key.
  EVP_PKEY* publicKey_{ nullptr };

  /// @brief variable to hold the public key length.
  size_t publicKeyLength_;

  /// @brief variable to store the key data.
  ReadWriteMemRegion keyingData_;

  /// @brief Variable to hold the algorithm ID for key wrapping
  AlgId algId_;

  /// @brief flag to indecate whether ctx is initialized or not.
  bool isInitialize_{ false };

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "KEMC", "Key Encapsulation Mechanism context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_ENCAPSULATOR_PUBLIC_CTX_DEFAULT_SSL_H_
