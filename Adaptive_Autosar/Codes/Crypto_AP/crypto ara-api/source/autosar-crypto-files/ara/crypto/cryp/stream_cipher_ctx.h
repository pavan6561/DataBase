// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : stream_cipher_ctx.h
//
// Purpose     : This file contains definitions for StreamCipherCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_STREAM_CIPHER_CTX_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_STREAM_CIPHER_CTX_H_

#include <ara/core/result.h>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
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
/// @brief Generalized Stream Cipher Context interface (it covers all modes of operation).
/// @trace SWS_CRYPT_23600
class StreamCipherCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_23601
  using Uptr = std::unique_ptr<StreamCipherCtx>;

  /// @brief Get BlockService instance.
  /// @trace SWS_CRYPT_23602    
  virtual BlockService::Uptr GetBlockService() const noexcept = 0;

  /// @brief Check the operation mode for the bytewise property.
  /// @returns @c true if the mode can process messages the byte-by-byte (without padding up to the block
  /// boundary) and @c false if only the block-by-block (only full blocks can be processed, the padding is
  /// mandatory)
  /// @trace SWS_CRYPT_23611, SWS_CRYPT_01662
  virtual bool IsBytewiseMode() const noexcept = 0;

  /// @brief Check if the seek operation is supported in the current mode.
  /// @returns @c true the seek operation is supported in the current mode and @c false otherwise
  /// @trace SWS_CRYPT_23612, SWS_CRYPT_01661
  virtual bool IsSeekableMode() const noexcept = 0;

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
  virtual ara::core::Result<void> Seek(std::int64_t offset, bool fromBegin = true) noexcept = 0;

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
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) noexcept
    = 0;

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
  virtual ara::core::Result<void> ProcessBlocks(ReadWriteMemRegion inOut) noexcept = 0;

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
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBytes(ReadOnlyMemRegion in) noexcept
    = 0;

  /// @brief Processes a non-final part of message (that is not aligned to the block-size boundary).
  /// This method sets size of the output container according to actually saved value.
  /// If <tt>(IsBytewiseMode() == false)</tt> then it @b must be:
  /// <tt>bs = GetBlockSize(), out.capacity() >= (((in.size() + bs - 1) / bs) * bs)</tt>
  /// If <tt>(IsBytewiseMode() == true)</tt>  then it @b must be:  <tt>out.capacity() >= in.size()</tt>
  /// This method is "copy inefficient", therefore it should be used only in conditions when an application
  /// cannot control the chunking of the original message!
  /// The input buffer must not point inside the output container!
  /// @tparam Alloc  a custom allocator type of the output container
  /// @returns  a managed container for the output data
  /// @param[in] in  an input data buffer
  /// @error SecurityErrorDomain::kInsufficientCapacity  if capacity of the output container is not enough
  /// @error SecurityErrorDomain::kInOutBuffersIntersect  if the input buffer points inside of
  /// the preallocated output container
  /// @error:  SecurityErrorDomain::kProcessingNotStarted  if data processing was not started by a call of
  /// the @c Start() method
  /// @trace SWS_CRYPT_23617
  template <typename Alloc = VENDOR_IMPLEMENTATION_DEFINED>
  ara::core::Result<ByteVector<Alloc>> ProcessBytes(ReadOnlyMemRegion in) noexcept
  {
    ara::core::Result<ara::core::Vector<ara::core::Byte>> result{ ProcessBytes(in) };
    if (result)
    {
      ByteVector<Alloc> out;
      out.resize(result.Value().size());
      memcpy(core::data(out), result.Value().data(), result.Value().size());
      return ara::core::Result<ByteVector<Alloc>>::FromValue(out);
    }
    else
    {
      return ara::core::Result<ByteVector<Alloc>>::FromError(result.Error());
    }
  }

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
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> FinishBytes(ReadOnlyMemRegion in) noexcept
    = 0;

  /// @brief Process the final part of message (that may be not aligned to the block-size boundary).
  /// This method sets the size of the output container according to actually saved value.
  /// If <tt>(IsBytewiseMode() == false)</tt> then it @b must be:
  /// <tt>bs = GetBlockSize(), </tt>
  /// <tt>out.capacity() >= (((in.size() + bs * ((CryptoTransform::kEncrypt == GetTransformation.Value()) ? 2
  /// : 1) - 1) / bs) * bs)</tt>
  /// If <tt>(IsBytewiseMode() == true)</tt>  then it @b must be:  <tt>out.capacity() >= in.size()</tt>
  /// Usage of this method is mandatory for processing of the last data chunk in block-wise modes!
  /// This method may be used for processing of a whole message in a single call (in any mode)!
  /// @tparam Alloc  a custom allocator type of the output container
  /// @returns a managed container for output data
  /// @param[in] in  an input data buffer
  /// The input buffer @b must @b not point inside the output container!
  /// @error SecurityErrorDomain::kInsufficientCapacity  if capacity of the output container is not enough
  /// @error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers intersect
  /// @error SecurityErrorDomain::kProcessingNotStarted  if data processing was not started by a call of
  /// the @c Start() method
  /// @trace SWS_CRYPT_23619
  template <typename Alloc = VENDOR_IMPLEMENTATION_DEFINED>
  ara::core::Result<ByteVector<Alloc>> FinishBytes(ReadOnlyMemRegion in) noexcept
  {
    ara::core::Result<ara::core::Vector<ara::core::Byte>> result = FinishBytes(in);
    if (result)
    {
      ByteVector<Alloc> out;
      out.resize(result.Value().size());
      memcpy(core::data(out), result.Value().data(), result.Value().size());
      return ara::core::Result<ByteVector<Alloc>>::FromValue(out);
    }
    else
    {
      return ara::core::Result<ByteVector<Alloc>>::FromError(result.Error());
    }
  }

  /// @brief Count number of bytes now kept in the context cache.
  /// In block-wise modes if an application has supplied input data chunks with incomplete last block
  /// then the context saves the rest part of the last (incomplete) block to internal "cache" memory
  /// and wait a next call for additional input to complete this block.
  /// @returns number of bytes now kept in the context cache
  /// @trace SWS_CRYPT_23620
  virtual std::size_t CountBytesInCache() const noexcept = 0;

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
  virtual ara::core::Result<void> SetKey(
    const SymmetricKey& key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept
    = 0;

  /// @brief Get the kind of transformation configured for this context: kEncrypt or kDecrypt
  /// @returns @c CryptoTransform
  /// @error SecurityErrorDomain::kUninitializedContext  if the transformation direction of this context
  /// is configurable during an initialization, but the context was not initialized yet
  /// @trace SWS_CRYPT_23624_001, SWS_CRYPT_23624_002, SWS_CRYPT_01665
  virtual ara::core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

  /// @brief Initialize the context for a new data stream processing or generation (depending from the
  /// primitive).
  /// If IV size is greater than maximally supported by the algorithm then an implementation may use the
  /// leading bytes only from the sequence.
  /// @param[in] iv  an optional Initialization Vector (IV) or "nonce" value
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by deploying a key
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation, but provided IV value is not empty,
  /// i.e. if <tt>(iv.empty() == false)</tt>
  /// @trace SWS_CRYPT_23625_001, SWS_CRYPT_23625_002, SWS_CRYPT_23625_003, SWS_CRYPT_01654
  /// SWS_CRYPT_01655
  virtual ara::core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept = 0;

  /// @brief Initialize the context for a new data stream processing or generation (depending from the
  /// primitive).
  /// If IV size is greater than maximally supported by the algorithm then an implementation may use the
  /// leading bytes only from the sequence.
  /// @param[in] iv  the Initialization Vector (IV) or "nonce" object
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by deploying a key
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation
  /// @error SecurityErrorDomain::kUsageViolation  if this transformation type is prohibited by
  /// the "allowed usage" restrictions of the provided @c SecretSeed object
  /// @trace SWS_CRYPT_23626_001, SWS_CRYPT_23626_002, SWS_CRYPT_23626_003, SWS_CRYPT_23626_004,
  /// SWS_CRYPT_23626_005, SWS_CRYPT_01654, SWS_CRYPT_01655
  virtual ara::core::Result<void> Start(const SecretSeed& iv) noexcept = 0;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_23627
  virtual ara::core::Result<void> Reset() noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_STREAM_CIPHER_CTX_H_
