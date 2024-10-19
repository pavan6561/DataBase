// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : auth_cipher_ctx.h
//
// Purpose     : This file contains definitions for AuthCipherCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_AUTH_CIPHER_CTX_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_AUTH_CIPHER_CTX_H_

#include <ara/core/optional.h>
#include <ara/core/result.h>

#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/block_service.h"

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
/// 3. Process the confidential part of the message via calls of @c ProcessBlocks(), @c ProcessBytes()
/// (and optionally @c FinishBytes()) methods.
/// 4. Call the @c Finish() method due to finalize the authentication code calculation (and get it
/// optionally).
/// 5. Copy of the calculated MAC may be extracted (by @c GetDigest()) or compared internally
/// (by @c Compare()).
/// Receiver side should not use decrypted data before finishing of the whole decryption and authentication
/// process! I.e. decrypted data can be used only after successful MAC verification!
/// @trace SWS_CRYPT_20100
class AuthCipherCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_20101
  using Uptr = std::unique_ptr<AuthCipherCtx>;

  /// @brief Get BlockService instance.
  /// @trace SWS_CRYPT_20102
  virtual BlockService::Uptr GetBlockService() const noexcept = 0;

  /// @brief Get maximal supported size of associated public data.
  /// @returns maximal supported size of associated public data in bytes
  /// @trace SWS_CRYPT_20103
  virtual std::uint64_t GetMaxAssociatedDataSize() const noexcept = 0;

  /// @brief Get the kind of transformation configured for this context: kEncrypt or kDecrypt
  /// @returns @c CryptoTransform
  /// @error SecurityErrorDomain::kUninitializedContext  if the transformation direction of this context
  /// is configurable during an initialization, but the context was not initialized yet
  /// @trace SWS_CRYPT_21715_001, SWS_CRYPT_21715_002, SWS_CRYPT_21715_003
  virtual ara::core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

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
  virtual ara::core::Result<void> SetKey(
    const SymmetricKey& key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept
    = 0;

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
  virtual ara::core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept = 0;

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
  virtual ara::core::Result<void> Start(const SecretSeed& iv) noexcept = 0;

  /// @brief Update the digest calculation by the specified RestrictedUseObject.
  /// This method is dedicated for cases then the @c RestrictedUseObject is a part of the "message".
  /// @param[in] in  a part of input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @error SecurityErrorDomain::kInvalidUsageOrder if ProcessConfidentialData has already been called
  /// @trace SWS_CRYPT_20312_001, SWS_CRYPT_20312_002, SWS_CRYPT_20312_003
  virtual ara::core::Result<void> UpdateAssociatedData(const RestrictedUseObject& in) noexcept = 0;

  /// @brief Update the digest calculation by a new chunk of associated data.
  /// @param[in] in  a part of the input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @error SecurityErrorDomain::kInvalidUsageOrder if ProcessConfidentialData has already been called
  /// @trace SWS_CRYPT_20313_001, SWS_CRYPT_20313_002, SWS_CRYPT_20313_003
  virtual ara::core::Result<void> UpdateAssociatedData(ReadOnlyMemRegion in) noexcept = 0;

  /// @brief Update the digest calculation by the specified Byte.
  /// This method is convenient for processing of constant tags.
  /// @param[in] in  a byte value that is a part of input message
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @error SecurityErrorDomain::kInvalidUsageOrder if ProcessConfidentialData has already been called
  /// @trace SWS_CRYPT_20314_001, SWS_CRYPT_20314_002, SWS_CRYPT_20314_003, SWS_CRYPT_01800
  virtual ara::core::Result<void> UpdateAssociatedData(std::uint8_t in) noexcept = 0;

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
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessConfidentialData(
    ReadOnlyMemRegion in, ara::core::Optional<ReadOnlyMemRegion> expectedTag) noexcept
    = 0;

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
  virtual ara::core::Result<void> ProcessConfidentialData(
    ReadWriteMemRegion inOut, ara::core::Optional<ReadOnlyMemRegion> expectedTag) noexcept
    = 0;

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
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> GetDigest(std::size_t offset = 0) const
    noexcept
    = 0;

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
  /// @trace SWS_CRYPT_20316
  template <typename Alloc = VENDOR_IMPLEMENTATION_DEFINED>
  ara::core::Result<ByteVector<Alloc>> GetDigest(std::size_t offset = 0) const noexcept
  {
    ara::core::Result<ara::core::Vector<ara::core::Byte>> result{ GetDigest(offset) };
    if (result)
    {
      ByteVector<Alloc> output;
      output.resize(result.Value().size());
      memcpy(core::data(output), result.Value().data(), result.Value().size());
      return ara::core::Result<ByteVector<Alloc>>::FromValue(output);
    }
    else
    {
      return ara::core::Result<ByteVector<Alloc>>::FromError(result.Error());
    }
  }

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_20414
  virtual ara::core::Result<void> Reset() noexcept = 0;

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
  virtual ara::core::Result<bool> Check(const Signature& expected) const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_AUTH_CIPHER_CTX_H_
