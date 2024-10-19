// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : auth_cipher_ctx_ssl_aes_gcm.h
//
// Purpose     : This file contains definitions for AuthCipherCtxSslAesGCM class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_AUTH_CIPHER_CTX_SSL_AES_GCM_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_AUTH_CIPHER_CTX_SSL_AES_GCM_H_

#include <ara/log/logging.h>
#include <openssl/evp.h>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/auth_cipher_ctx.h"
#include "crypto_config.h"
#include "crypto_primitive_id_impl.h"
#include "block_service_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Generalized Authenticated Cipher Context interface.
/// Methods of the derived interface @c BufferedDigest are used for authentication of associated public data.
/// Methods of the derived interface @c StreamCipherCtx are used for encryption/decryption and authentication
/// of confidential part of message.
/// The data processing must be executed in following order:
/// 1. Call one of the @c Start() methods.
/// 2. Process all associated public data via calls of @c Update() methods.
/// 3. Process the confidential part of the message via calls of @c ProcessBlocks(), @c ProcessBytes() (and
/// optionally @c FinishBytes()) methods.
/// 4. Call the @c Finish() method due to finalize the authentication code calculation
/// (and get it optionally).
/// 5. Copy of the calculated MAC may be extracted (by @c GetDigest()) or compared internally
/// (by @c Compare()).
/// Receiver side should not use decrypted data before finishing of the whole decryption and authentication
/// process! I.e. decrypted data can be used only after successful MAC verification!
class AuthCipherCtxSslAesGCM : public AuthCipherCtx
{
 public:
  /// @brief Constructor for the AuthCipherCtxSslAesGCM class.
  /// @param[in] algIdValue Algorithm Id for the authentication cipher
  explicit AuthCipherCtxSslAesGCM(CryptoPrimitiveId::AlgId algIdValue) noexcept;

  /// @brief Destructor for the AuthCipherCtxSslAesGCM class.
  ~AuthCipherCtxSslAesGCM() noexcept override;

  /// @brief Get BlockService instance.
  /// @trace SWS_CRYPT_20102
  BlockService::Uptr GetBlockService() const noexcept override;

  /// @brief Get maximal supported size of associated public data.
  /// @returns maximal supported size of associated public data in bytes
  /// @trace SWS_CRYPT_20103
  std::uint64_t GetMaxAssociatedDataSize() const noexcept override
  {
    aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::GetMaxAssociatedDataSize()";
    return configInstance_->GetCryptoMaxBufferSize();
  }

  /// @brief Get the kind of transformation configured for this context: kEncrypt or kDecrypt
  /// @returns @c CryptoTransform
  /// @error SecurityErrorDomain::kUninitializedContext  if the transformation direction of this context
  /// is configurable during an initialization, but the context was not initialized yet
  /// @trace SWS_CRYPT_21715_001, SWS_CRYPT_21715_002, SWS_CRYPT_21715_003
  ara::core::Result<CryptoTransform> GetTransformation() const noexcept override
  {
    aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::GetTransformation()";

    if (acProcessState_ < authenticationCipherExecutionStates::kAcInitialize)
    {
      return ara::core::Result<CryptoTransform>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
    return ara::core::Result<CryptoTransform>::FromValue(transformationValue_);
  }

  /// @brief Set (deploy) a key to the authenticated cipher symmetric algorithm context.
  /// @param[in] key  the source key object
  /// @param[in] directTransform  the "direction" indicator: deploy the key for encryption (if @c true)
  /// or for decryption (if @c false)
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// (taking into account the direction specified by @c transform) is prohibited by
  /// the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_23911_001, SWS_CRYPT_23911_002, SWS_CRYPT_23911_003_001, SWS_CRYPT_23911_003_002
  /// SWS_CRYPT_01807_001, SWS_CRYPT_01807_002
  ara::core::Result<void> SetKey(
    const SymmetricKey& key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept override;

  /// @brief Initialize the context for a new data processing or generation (depending from the primitive).
  /// If IV size is greater than maximally supported by the algorithm then an implementation may use the
  /// leading bytes only from the sequence.
  /// @param[in] iv  an optional Initialization Vector (IV) or "nonce" value
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation, but provided IV value is not empty,
  /// i.e. if <tt>(iv.empty() == false)</tt>
  /// @trace SWS_CRYPT_24714_001, SWS_CRYPT_24714_002, SWS_CRYPT_24714_003, SWS_CRYPT_24714_004
  /// SWS_CRYPT_01808_001, SWS_CRYPT_01808_002, SWS_CRYPT_01808_003, SWS_CRYPT_01808_004
  ara::core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept override;

  /// @brief Initialize the context for a new data processing or generation (depending from the primitive).
  /// If IV size is greater than maximally supported by the algorithm then an implementation may use the
  /// leading bytes only from the sequence.
  /// @param[in] iv  the Initialization Vector (IV) or "nonce" object
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation
  /// @error SecurityErrorDomain::kUsageViolation  if this transformation type is prohibited by
  /// the "allowed usage" restrictions of the provided @c SecretSeed object
  /// @trace SWS_CRYPT_24715_001, SWS_CRYPT_24715_002, SWS_CRYPT_24715_003, SWS_CRYPT_24715_004,
  /// SWS_CRYPT_24715_005_001, SWS_CRYPT_24715_005_002, SWS_CRYPT_01808_001, SWS_CRYPT_01808_002
  /// SWS_CRYPT_01808_003, SWS_CRYPT_01808_004, SWS_CRYPT_01808_005_001, SWS_CRYPT_01808_005_002
  ara::core::Result<void> Start(const SecretSeed& iv) noexcept override;

  /// @brief Update the digest calculation by the specified RestrictedUseObject.
  /// This method is dedicated for cases then the @c RestrictedUseObject is a part of the "message".
  /// @param[in] in  a part of input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @error SecurityErrorDomain::kInvalidUsageOrder if ProcessConfidentialData has already been called
  /// @trace SWS_CRYPT_20312_001, SWS_CRYPT_20312_002, SWS_CRYPT_20312_003
  ara::core::Result<void> UpdateAssociatedData(const RestrictedUseObject& in) noexcept override;

  /// @brief Update the digest calculation by a new chunk of associated data.
  /// @param[in] in  a part of the input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @error SecurityErrorDomain::kInvalidUsageOrder if ProcessConfidentialData has already been called
  /// @trace SWS_CRYPT_20313_001, SWS_CRYPT_20313_002, SWS_CRYPT_20313_003
  ara::core::Result<void> UpdateAssociatedData(ReadOnlyMemRegion in) noexcept override;

  /// @brief Update the digest calculation by the specified Byte.
  /// This method is convenient for processing of constant tags.
  /// @param[in] in  a byte value that is a part of input message
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @error SecurityErrorDomain::kInvalidUsageOrder if ProcessConfidentialData has already been called
  /// @trace SWS_CRYPT_20314_001, SWS_CRYPT_20314_002, SWS_CRYPT_20314_003, SWS_CRYPT_01800
  ara::core::Result<void> UpdateAssociatedData(std::uint8_t in) noexcept override;

  /// @brief Process confidential data
  /// The input buffer will be overwritten by the processed message.
  /// This function is the final call, i.e. all associated data must have been already provided.
  /// Hence, the function will check the authentication tag and only return the processed data, if the tag is
  /// valid.
  /// @param[in] in the input buffer containing the full message
  /// @param[in] expectedTag  pointer to read only mem region
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by
  /// the block size (see @c GetBlockSize())
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
  /// the @c Start() method
  /// @error SecurityErrorDomain::kAuthTagNotValid  if the processed data cannot be authenticated
  /// @trace SWS_CRYPT_23634_001, SWS_CRYPT_23634_002, SWS_CRYPT_23634_003, SWS_CRYPT_23634_004,
  /// SWS_CRYPT_01801, SWS_CRYPT_01802, SWS_CRYPT_01803, SWS_CRYPT_01804, SWS_CRYPT_01805_001
  /// SWS_CRYPT_01805_002
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessConfidentialData(
    ReadOnlyMemRegion in, ara::core::Optional<ReadOnlyMemRegion> expectedTag) noexcept override;

  /// @brief Process confidential data
  /// The input buffer will be overwritten by the processed message
  /// After this method is called no additional associated data may be updated.
  /// @param[in] inOut  the input buffer containing the full message
  /// @param[in] expectedTag  pointer to read only mem region
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by
  /// the block size (see @c GetBlockSize())
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
  /// the @c Start() method
  /// @error SecurityErrorDomain::kAuthTagNotValid  if the processed data cannot be authenticated
  /// @trace SWS_CRYPT_23635_001, SWS_CRYPT_23635_002, SWS_CRYPT_23635_003, SWS_CRYPT_23635_004,
  /// SWS_CRYPT_01801, SWS_CRYPT_01802, SWS_CRYPT_01803, SWS_CRYPT_01804, SWS_CRYPT_01805_001
  /// SWS_CRYPT_01805_002
  ara::core::Result<void> ProcessConfidentialData(
    ReadWriteMemRegion inOut, ara::core::Optional<ReadOnlyMemRegion> expectedTag) noexcept override;

  /// @brief Retrieve the calculated digest.
  /// The entire digest value is kept in the context until the next call of @c Start().
  /// Therefore, the digest can be re-checked or extracted at any time.
  /// If the offset is larger than the digest, an empty buffer shall be returned.
  /// This method can be implemented as "inline" after standardization of function @c ara::core::memcpy().
  /// @param[in] offset  position of the first byte of digest that should be placed to the output buffer
  /// @returns an output buffer storing the requested digest fragment or the full digest
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the digest calculation was not finished
  /// by a call of the @c Finish() method
  /// @error SecurityErrorDomain::kUsageViolation  if the buffered digest belongs to a MAC/HMAC/AE/AEAD
  /// context initialized by a key without @c kAllowSignature permission
  ara::core::Result<ara::core::Vector<ara::core::Byte>> GetDigest(std::size_t offset = 0) const
    noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_20414
  ara::core::Result<void> Reset() noexcept override;

  /// @brief Check the calculated digest against an expected "signature" object.
  /// Entire digest value is kept in the context up to next call @c Start(), therefore it can be
  /// verified again or extracted.
  /// This method can be implemented as "inline" after standardization of function @c ara::core::memcmp().
  /// @param[in] expected  the signature object containing an expected digest value
  /// @returns @c true if value and meta-information of the provided "signature" object is identical to
  /// calculated digest and current configuration of the context respectively; but @c false otherwise
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the digest calculation was not finished by
  /// a call of the @c Finish() method
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided "signature" object was produced by
  /// another crypto primitive type
  /// @trace SWS_CRYPT_20319_001, SWS_CRYPT_20319_002, SWS_CRYPT_20319_003, SWS_CRYPT_20319_004
  ara::core::Result<bool> Check(const Signature& expected) const noexcept override;

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
    aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::IsInitialized()";

    bool intializationState{ false };

    if (acProcessState_ >= authenticationCipherExecutionStates::kAcInitialize)
    {
      intializationState = true;
    }

    return intializationState;
  }

 private:
  /// @brief Variable to to store the mac size.
  static uint8_t constexpr stkMacSizeForAuthentication_{ 16 };

  /// @brief Get the openssl cipher mode for this context according to algoId.
  EVP_CIPHER const* GetCipherMode() const noexcept;

  /// @brief Method to get the expected key size from the given target algorithm Id.
  /// @param[in] targetAlgId ID for the target algorithm of the key.
  size_t GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept;

  /// @brief enum class to define the execution states for authentication cipher function ctx.
  enum class authenticationCipherExecutionStates : std::uint8_t
  {
    kAcIdle = 0,
    kAcInitialize = 1,
    kAcProcessingStarted = 2,
    kAcProcessingFinished = 3
  };

  /// @brief algorithm Id for this context.
  CryptoPrimitiveId::AlgId algId_;

  /// @brief openssl cipher context pointer.
  EVP_CIPHER_CTX* symmetricAuthenticationCipherCtx_{ nullptr };

  /// @brief transformation for this context.
  CryptoTransform transformationValue_{ CryptoTransform::kEncrypt };

  /// @brief Key value for this context.
  ara::core::Vector<uint8_t> keyValue_{};

  /// @brief IV value for this context.
  ara::core::Vector<uint8_t> IvValue_{};

  /// @brief Variable to hold the execution state for stream cipher ctx.
  authenticationCipherExecutionStates acProcessState_{ authenticationCipherExecutionStates::kAcIdle };

  /// @brief calculated mac value for additional authentication.
  uint8_t tagValue_[stkMacSizeForAuthentication_]{};

  /// @brief Variable to store the allowed usage flags for given symmetric key.
  AllowedUsageFlags allowedKeyUsage_{ kAllowDataEncryption | kAllowDataDecryption };

  /// @brief Pointer to the crypto configuration.
  std::shared_ptr<ara::crypto::config::CryptoConfig> configInstance_{
    ara::crypto::config::CryptoConfig::GetCryptoConfigInstance()
  };

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "ASCC", "Authentication Cipher context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_ACC_AUTH_CIPHER_CTX_SSL_AES_GCM_H_
