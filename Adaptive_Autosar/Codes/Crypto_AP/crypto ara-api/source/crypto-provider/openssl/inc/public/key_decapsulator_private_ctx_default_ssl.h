// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_decapsulator_private_ctx_default_ssl.h
//
// Purpose   : This file contains definitions for KeyDecapsulatorPrivateCtxDefaultSsl class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_DECAPSULATOR_PRIVATE_CTX_DEFAULT_SSL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_DECAPSULATOR_PRIVATE_CTX_DEFAULT_SSL_H_

#include "algorithm_identification.h"
#include "ara/crypto/cryp/key_decapsulator_private_ctx.h"
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
/// @brief Asymmetric Key Encapsulation Mechanism (KEM) Private key Context interface.
/// @trace SWS_CRYPT_21400
class KeyDecapsulatorPrivateCtxDefaultSsl : public KeyDecapsulatorPrivateCtx
{
 public:
  /// @brief Constructor for KeyDecapsulatorPrivateCtxDefaultSsl class.
  KeyDecapsulatorPrivateCtxDefaultSsl(AlgId algo) noexcept : algId_{ algo }
  {
  }

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_21402
  ExtensionService::Uptr GetExtensionService() const noexcept;

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
  ara::core::Result<SecretSeed::Uptrc> DecapsulateSeed(ReadOnlyMemRegion input,
    SecretSeed::Usage allowedUsage = kAllowKdfMaterialAnyUsage) const noexcept override;

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
  ara::core::Result<SymmetricKey::Uptrc> DecapsulateKey(ReadOnlyMemRegion input,
    KeyDerivationFunctionCtx& kdf, AlgId kekAlgId, ReadOnlyMemRegion salt = ReadOnlyMemRegion(),
    ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Set (deploy) a key to the key decapsulator private algorithm context.
  /// @param[in] key  the source key object
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this private key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_21413
  ara::core::Result<void> SetKey(const PrivateKey& key) noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_21414
  ara::core::Result<void> Reset() noexcept override;

  /// @brief Get entropy (bit-length) of the key encryption key (KEK) material.
  /// For RSA system the returned value corresponds to the length of module N (minus 1).
  /// For DH-like system the returned value corresponds to the length of module q (minus 1).
  /// @returns entropy of the KEK material in bits
  /// @trace SWS_CRYPT_21415
  std::size_t GetKekEntropy() const noexcept override
  {
    return 256 - 1;
  }

  /// @brief Get fixed size of the encapsulated data block.
  /// @returns size of the encapsulated data block in bytes
  /// @trace SWS_CRYPT_21416
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
  /// @brief Configure the decrypt ctx of the Openssl.
  void ConfigureDecryptorCtx(EVP_PKEY_CTX* keyCtx) const noexcept;

  /// @brief method to unwrap the key value.
  ara::core::Vector<uint8_t> UnWrapKey(const SymmetricKey& kek, ReadOnlyMemRegion in, AlgId kekalgId) const
    noexcept;

  /// @brief Get the openssl cipher mode for this context according to algoId.
  const EVP_CIPHER* GetCipherMode(AlgId algoId) const noexcept;

  /// @brief Method to decrypt the cipher text.
  ara::core::Vector<uint8_t> DecryptCipherText(ReadOnlyMemRegion in) const noexcept;

  /// @brief pointer to the openssl private key.
  EVP_PKEY* privateKey_{ nullptr };

  /// @brief variable to store the key data.
  ReadWriteMemRegion keyingData_;

  /// @brief Variable to hold the algorithm ID for key wrapping
  AlgId algId_;

  /// @brief flag to indecate whether ctx is initialized or not.
  bool isInitialize_{ false };

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "KDMC", "Key Decapsulation Mechanism context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_KEY_DECAPSULATOR_PRIVATE_CTX_DEFAULT_SSL_H_
