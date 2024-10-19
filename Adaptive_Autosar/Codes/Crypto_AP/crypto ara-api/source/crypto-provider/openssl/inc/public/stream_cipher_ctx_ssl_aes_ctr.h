// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : stream_cipher_ctx_ssl_aes_ctr.h
//
// Purpose     : This file contains definitions for StreamCipherCtxsslCtr class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_STREAM_CIPHER_CTX_SSL_AES_CTR_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_STREAM_CIPHER_CTX_SSL_AES_CTR_H_

#include <ara/log/logging.h>
#include <mutex>
#include <openssl/evp.h>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/stream_cipher_ctx.h"
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
/// @brief Generalized Stream Cipher Context interface (it covers all modes of operation).
/// @trace SWS_CRYPT_23600
class StreamCipherCtxsslCtr : public StreamCipherCtx
{
 public:
  /// @brief Constructor for StreamCipherCtxsslCtr class.
  /// @param[in] algorithm ID for stream cipher algorithm.
  explicit StreamCipherCtxsslCtr(CryptoPrimitiveId::AlgId algIdValue) noexcept;

  /// @brief Destructor for StreamCipherCtxsslCtr class.
  ~StreamCipherCtxsslCtr() noexcept override;

  /// @brief Get BlockService instance.
  /// @trace SWS_CRYPT_23602    
  BlockService::Uptr GetBlockService() const noexcept override;

  /// @brief Check the operation mode for the bytewise property.
  /// @returns @c true if the mode can process messages the byte-by-byte (without padding up to the block
  /// boundary) and @c false if only the block-by-block (only full blocks can be processed, the padding is
  /// mandatory)
  /// @trace SWS_CRYPT_23611, SWS_CRYPT_01662
  bool IsBytewiseMode() const noexcept override
  {
    aralog_.LogDebug() << "In StreamCipherCtxsslCtr::IsBytewiseMode()";
    return true;
  }

  /// @brief Check if the seek operation is supported in the current mode.
  /// @returns @c true the seek operation is supported in the current mode and @c false otherwise
  /// @trace SWS_CRYPT_23612, SWS_CRYPT_01661
  bool IsSeekableMode() const noexcept override
  {
    aralog_.LogDebug() << "In StreamCipherCtxsslCtr::IsSeekableMode()";
    return true;
  }

  /// @brief Set the position of the next byte within the stream of the encryption/decryption gamma.
  /// @param[in] offset  the offset value in bytes, relative to begin or current position in the gamma stream
  /// @param[in] fromBegin  the starting point for positioning within the stream: from begin (if @c true) or
  /// from current position (if @c false)
  /// @error SecurityErrorDomain::kUnsupported  if the seek operation is not supported by the current mode
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
  /// the @c Start() method
  /// @error SecurityErrorDomain::kBelowBoundary  if the @c offset value is incorrect (in context of the
  /// the @c fromBegin argument), i.e. it points before begin of the stream (note: it is an optional
  /// error condition)
  /// @error SecurityErrorDomain::kInvalidArgument  if the offset is not aligned to the required boundary
  /// (see @c IsBytewiseMode())
  /// @trace SWS_CRYPT_23613_001, SWS_CRYPT_23613_003, SWS_CRYPT_23613_004, SWS_CRYPT_23613_005
  /// SWS_CRYPT_01660
  ara::core::Result<void> Seek(std::int64_t offset, bool fromBegin = true) noexcept override;

  /// @brief Process initial parts of message aligned to the block-size boundary.
  /// It is a copy-optimized method that doesn't use the internal cache buffer! It can be used only before
  /// processing of any non-aligned to the block-size boundary data.
  /// <b>Pointers to the input and output buffers must be aligned to the block-size boundary!</b>
  /// The input and output buffers may completely coincide, but they must not partially intersect!
  /// @returns  an output data buffer
  /// @param[in] in  an input data buffer
  /// @error SecurityErrorDomain::kIncompatibleArguments  if sizes of the input and output buffers
  /// are not equal
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by
  /// the block size (see @c GetBlockSize())
  /// @error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers partially intersect
  /// @error SecurityErrorDomain::kInvalidUsageOrder  if this method is called after processing of
  /// non-aligned data (to the block-size boundary)
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
  /// the @c Start() method
  /// @trace SWS_CRYPT_23614_001, SWS_CRYPT_23614_003, SWS_CRYPT_23614_005, SWS_CRYPT_23614_006
  /// SWS_CRYPT_01656
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) noexcept override;

  /// @brief Process initial parts of message aligned to the block-size boundary.
  /// It is a copy-optimized method that doesn't use internal cache buffer! It can be used up to first
  /// non-block aligned data processing.
  /// <b>Pointer to the input-output buffer must be aligned to the block-size boundary!</b>
  /// @param[in,out] inOut  an input and output data buffer, i.e. the whole buffer should be updated
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the @c inOut buffer is not divisible by
  /// the block size (see @c GetBlockSize())
  /// @error SecurityErrorDomain::kInvalidUsageOrder  if this method is called after processing of
  /// non-aligned data (to the block-size boundary)
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
  /// the @c Start() method
  /// @trace SWS_CRYPT_23615, SWS_CRYPT_01656
  ara::core::Result<void> ProcessBlocks(ReadWriteMemRegion inOut) noexcept override;

  /// @brief Process a non-final part of message (that is not aligned to the block-size boundary).
  /// If <tt>(IsBytewiseMode() == false)</tt> then it @b must be:
  /// <tt>bs = GetBlockSize(), out.size() >= (((in.size() + bs - 1) / bs) * bs)</tt>
  /// If <tt>(IsBytewiseMode() == true)</tt>  then it @b must be:  <tt>out.size() >= in.size()</tt>
  /// The input and output buffers must not intersect!
  /// This method is "copy inefficient", therefore it should be used only in conditions when an application
  /// cannot control the chunking of the original message!
  /// @param[in] in  an input data buffer
  /// @returns an output data buffer
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the output buffer has capacity
  /// insufficient for placing of the transformation result
  /// @error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers intersect
  /// @error SecurityErrorDomain::kProcessingNotStarted  if data processing was not started by a call of
  /// the @c Start() method
  /// @trace SWS_CRYPT_23616_003, SWS_CRYPT_23616_006, SWS_CRYPT_01657
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBytes(ReadOnlyMemRegion in) noexcept override;

  /// @brief Process the final part of message (that may be not aligned to the block-size boundary).
  /// If <tt>(IsBytewiseMode() == false)</tt> then it @b must be:
  /// <tt>bs = GetBlockSize(), </tt>
  /// <tt>out.size() >= (((in.size() + bs * ((CryptoTransform::kEncrypt == GetTransformation().Value()) ? 2 :
  /// 1) - 1) / bs) * bs)</tt>
  /// If <tt>(IsBytewiseMode() == true)</tt>  then it @b must be:  <tt>out.size() >= in.size()</tt>
  /// The input and output buffers must not intersect!
  /// Usage of this method is mandatory for processing of the last data chunk in block-wise modes!
  /// This method may be used for processing of a whole message in a single call (in any mode)!
  /// @param[in] in  an input data buffer
  /// @returns an output data buffer
  /// @error SecurityErrorDomain::kInsufficientCapacity  if capacity of the output buffer is not enough
  /// @error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers intersect
  /// @error SecurityErrorDomain::kProcessingNotStarted  if data processing was not started by a call of
  /// the @c Start() method
  /// @trace SWS_CRYPT_23618_001, SWS_CRYPT_23618_004, SWS_CRYPT_01658, SWS_CRYPT_01659
  ara::core::Result<ara::core::Vector<ara::core::Byte>> FinishBytes(ReadOnlyMemRegion in) noexcept override;

  /// @brief Count number of bytes now kept in the context cache.
  /// In block-wise modes if an application has supplied input data chunks with incomplete last block
  /// then the context saves the rest part of the last (incomplete) block to internal "cache" memory
  /// and wait a next call for additional input to complete this block.
  /// @returns number of bytes now kept in the context cache
  /// @trace SWS_CRYPT_23620
  std::size_t CountBytesInCache() const noexcept override;

  /// @brief Set (deploy) a key to the stream chiper algorithm context.
  /// @param[in] key  the source key object
  /// @param[in] directTransform  the "direction" indicator: deploy the key for direct transformation (if @c
  /// true) or for reverse one (if @c false)
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// (taking into account the direction specified by @c transform) is prohibited by
  /// the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_23623_001, SWS_CRYPT_23623_002, SWS_CRYPT_23623_003, SWS_CRYPT_23623_004
  /// SWS_CRYPT_01652, SWS_CRYPT_01653
  ara::core::Result<void> SetKey(
    const SymmetricKey& key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept override;

  /// @brief Get the kind of transformation configured for this context: kEncrypt or kDecrypt
  /// @returns @c CryptoTransform
  /// @error SecurityErrorDomain::kUninitializedContext  if the transformation direction of this context
  /// is configurable during an initialization, but the context was not initialized yet
  /// @trace SWS_CRYPT_23624_001, SWS_CRYPT_23624_002, SWS_CRYPT_01665
  ara::core::Result<CryptoTransform> GetTransformation() const noexcept override
  {
    aralog_.LogDebug() << "In StreamCipherCtxsslCtr::GetTransformation()";

    if (scProcessState_ < streamCipherExecutionStates::kScInitialize)
    {
      return ara::core::Result<CryptoTransform>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
    return transformationValue_;
  }

  /// @brief Initialize the context for a new data stream processing or generation (depending from the
  /// primitive).
  /// If IV size is greater than maximally supported by the algorithm then an implementation may use the
  /// leading bytes only from the sequence.
  /// @param[in] iv  an optional Initialization Vector (IV) or "nonce" value
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by deploying
  /// a key
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation, but provided IV value is not empty,
  /// i.e. if <tt>(iv.empty() == false)</tt>
  /// @trace SWS_CRYPT_23625_001, SWS_CRYPT_23625_002, SWS_CRYPT_23625_003, SWS_CRYPT_01654
  /// SWS_CRYPT_01655
  ara::core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept override;

  /// @brief Initialize the context for a new data stream processing or generation (depending from the
  /// primitive).
  /// If IV size is greater than maximally supported by the algorithm then an implementation may use the
  /// leading bytes only from the sequence.
  /// @param[in] iv  the Initialization Vector (IV) or "nonce" object
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by deploying
  /// a key
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation
  /// @error SecurityErrorDomain::kUsageViolation  if this transformation type is prohibited by
  /// the "allowed usage" restrictions of the provided @c SecretSeed object
  /// @trace SWS_CRYPT_23626_001, SWS_CRYPT_23626_002, SWS_CRYPT_23626_003, SWS_CRYPT_23626_004,
  /// SWS_CRYPT_23626_005, SWS_CRYPT_01654, SWS_CRYPT_01655
  ara::core::Result<void> Start(const SecretSeed& iv) noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_23627
  ara::core::Result<void> Reset() noexcept override;

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use,
  /// and @c false otherwise
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override
  {
    aralog_.LogDebug() << "In StreamCipherCtxsslCtr::IsInitialized()";

    if (scProcessState_ >= streamCipherExecutionStates::kScInitialize)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

 private:
  /// @brief Method to get openssl cipher mode according to the algorithm ID.
  EVP_CIPHER const* GetCipherMode() const noexcept;

  /// @brief Method to get the expected key size from the given target algorithm Id.
  /// @param[in] targetAlgId ID for the target algorithm of the key.
  size_t GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept;

  /// @brief enum class to define the execution states for stream cipher function ctx.
  enum class streamCipherExecutionStates : std::uint8_t
  {
    kScIdle = 0,
    kScInitialize = 1,
    kScProcessingStarted = 2,
    kScProcessingFinished = 3
  };

  /// @brief Variable to store the algorithm ID.
  CryptoPrimitiveId::AlgId algId_;

  /// @brief Pointer for openssl cipher context.
  EVP_CIPHER_CTX* symetricStreamCipherCtx_{ nullptr };

  /// @brief Variable to store the transformation for this context.
  CryptoTransform transformationValue_{ CryptoTransform::kEncrypt };

  /// @brief Variable to store the key value for this context.
  ara::core::Vector<uint8_t> keyValue_{};

  /// @brief Variable to store the IV value for this context.
  ara::core::Vector<uint8_t> IvValue_{};

  /// @brief Variable to hold the execution state for stream cipher ctx.
  streamCipherExecutionStates scProcessState_{ streamCipherExecutionStates::kScIdle };

  /// @brief Variable to store the seek offset value.
  std::int64_t seekOffset_{ 0 };

  /// @brief Flag to indicate if seek offset is from the begin or from current position
  bool isFromBigin_{ false };

  /// @brief Variable to store the index.
  size_t indexValue_{ 0 };

  /// @brief Variable to store the current index.
  size_t currentindex_{ 0 };

  /// @brief Flag to indicate if seek action is required.
  bool isseekActionRequired_{ false };

  /// @brief Mutex for seek operation.
  std::mutex seekMutex_{};

  /// @brief Logger for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "SSCC", "Symmetric Stream Cipher context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_STREAM_CIPHER_CTX_SSL_AES_CTR_H_
