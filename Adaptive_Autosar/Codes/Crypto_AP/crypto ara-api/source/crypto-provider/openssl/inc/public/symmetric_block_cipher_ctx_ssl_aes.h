// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_block_cipher_ctx_ssl_aes.h
//
// Purpose     : This file contains definitions for SymmetricBlockCipherCtxsslAES class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_SYMMETRIC_BLOCK_CIPHER_CTX_SSL_AES_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_SYMMETRIC_BLOCK_CIPHER_CTX_SSL_AES_H_

#include <ara/log/logging.h>
#include <openssl/evp.h>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/symmetric_block_cipher_ctx.h"
#include "crypto_primitive_id_impl.h"
#include "crypto_service_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Interface of a Symmetric Block Cipher Context with padding.
/// @trace SWS_CRYPT_23700
class SymmetricBlockCipherCtxsslAES : public SymmetricBlockCipherCtx
{
 public:
  
  /// @brief Constructor for SymmetricBlockCipherCtxsslAES class.
  /// @param[in] algorithm Id for the block cipher.
  explicit SymmetricBlockCipherCtxsslAES(CryptoPrimitiveId::AlgId algIdValue) noexcept;

  /// @brief Destructor for SymmetricBlockCipherCtxsslAES class.
  ~SymmetricBlockCipherCtxsslAES() noexcept override;

  /// @brief Get CryptoService instance.
  /// @trace SWS_CRYPT_23702
  CryptoService::Uptr GetCryptoService() const noexcept override;

  /// @brief Set (deploy) a key to the symmetric algorithm context.
  /// @param[in] key  the source key object
  /// @param[in] directTransform  the "direction" indicator: deploy the key for direct transformation (if @c
  /// true) or for reverse one (if @c false)
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// (taking into account the direction specified by @c transform) is prohibited by
  /// the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_23710_001, SWS_CRYPT_23710_002, SWS_CRYPT_23710_003, SWS_CRYPT_01502
  /// SWS_CRYPT_01503
  ara::core::Result<void> SetKey(
    const SymmetricKey& key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept override;

  /// @brief Get the kind of transformation configured for this context: kEncrypt or kDecrypt
  /// @returns @c CryptoTransform
  /// @error SecurityErrorDomain::kUninitializedContext  if the transformation direction of this context
  /// is configurable during an initialization, but the context was not initialized yet
  /// @trace SWS_CRYPT_23711_001, SWS_CRYPT_23711_002, SWS_CRYPT_01504
  ara::core::Result<CryptoTransform> GetTransformation() const noexcept override
  {
    aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES::GetTransformation()";

    if (!isInitialize_)
    {
      return ara::core::Result<CryptoTransform>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
    return transformationValue_;
  }

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_23714
  ara::core::Result<void> Reset() noexcept override;

  /// @brief Process provided blocks without padding.
  /// The @c in and @c out buffers @b must have same size and this size @b must be divisible by the block
  /// size (see @c GetBlockSize()).
  /// <b>Pointers to the input and output buffers must be aligned to the block-size boundary!</b>
  /// @returns  an output data buffer
  /// @param[in] in  an input data buffer
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by
  /// the block size (see @c GetBlockSize())
  /// @error SecurityErrorDomain::kIncompatibleArguments  if sizes of the input and output buffer are not
  /// equal
  /// @error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers partially intersect
  /// @trace SWS_CRYPT_23715_001, SWS_CRYPT_23715_003, SWS_CRYPT_23715_004, SWS_CRYPT_01505
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) const
    noexcept override;

  /// @brief Process (encrypt / decrypt) an input block according to the crypto configuration.
  /// @returns the output buffer
  /// Encryption with <tt>(suppressPadding == true)</tt> expects that:
  /// <tt>in.size() == GetMaxInputSize(true) && out.size() >= GetMaxOutputSize(true)</tt>.
  /// Encryption with <tt>(suppressPadding == false)</tt> expects that:
  /// <tt>in.size() <= GetMaxInputSize(false) && in.size() > 0 && out.size() >= GetMaxOutputSize(false)</tt>.
  /// Decryption expects that:
  /// <tt>in.size() == GetMaxInputSize() && out.size() >= GetMaxOutputSize(suppressPadding)</tt>.
  /// The case <tt>(out.size() < GetMaxOutputSize())</tt> should be used with caution, only if you are
  /// strictly certain about the size of the output data!
  /// In case of <tt>(suppressPadding == true)</tt> the actual size of plain text should be equal to full size
  /// of the plain data block (defined by the algorithm)!
  /// @param[in] in  the input data block
  /// @param[in] suppressPadding  if @c true then the method doesn't apply the padding, but the payload should
  /// fill the whole block of the plain data
  /// @returns actual size of output data (it always <tt><= out.size()</tt>) or 0 if the input data block has
  /// incorrect content
  /// @error:  SecurityErrorDomain::kIncorrectInputSize  if the mentioned above rules about the input size is
  /// violated
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the @c out.size() is not enough to store the
  /// transformation result
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_23716_001, SWS_CRYPT_23716_002, SWS_CRYPT_23716_003, SWS_CRYPT_23716_005
  /// SWS_CRYPT_01505, SWS_CRYPT_01506, SWS_CRYPT_01507
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept override;

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
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

 private:
  /// @brief Method to get the cipher mode according to the algoID.
  EVP_CIPHER const* GetCipherMode() const noexcept;

  /// @brief Method to get the expected key size from the given target algorithm Id.
  /// @param[in] targetAlgId ID for the target algorithm of the key.
  size_t GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept;

  /// @brief Variable to store the algorithm Id for this context.
  CryptoPrimitiveId::AlgId algId_;

  /// @brief Pointer to the openssl cipher context.
  EVP_CIPHER_CTX* symetricBlockCipherCtx_{ nullptr };

  /// @brief Variable to store the transformation for this context.
  CryptoTransform transformationValue_{ CryptoTransform::kEncrypt };

  /// @brief Variable to store the key value for this context.
  ara::core::Vector<uint8_t> keyValue_{};

  /// @brief Flag to indicate if this context is initialized.
  bool isInitialize_{ false };

  /// @brief Variable to store the maximum input size.
  std::uint32_t maxInputSize_;

  /// @brief Logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "SBCC", "Symmetric Block Cipher context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_SYMMETRIC_BLOCK_CIPHER_CTX_SSL_AES_H_
