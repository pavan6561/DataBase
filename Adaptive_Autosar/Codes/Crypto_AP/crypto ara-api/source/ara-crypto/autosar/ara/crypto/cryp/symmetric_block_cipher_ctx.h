// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_block_cipher_ctx.h
//
// Purpose     : This file contains definitions for SymmetricBlockCipherCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_SYMMETRIC_BLOCK_CIPHER_CTX_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_SYMMETRIC_BLOCK_CIPHER_CTX_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/crypto_service.h"
#include "ara/core/result.h"

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
class SymmetricBlockCipherCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_23701
  using Uptr = std::unique_ptr<SymmetricBlockCipherCtx>;

  /// @brief Get CryptoService instance.
  /// @trace SWS_CRYPT_23702
  virtual CryptoService::Uptr GetCryptoService() const noexcept = 0;

  /// @brief Set (deploy) a key to the symmetric algorithm context.
  /// @param[in] key  the source key object
  /// @param[in] directTransform  the "direction" indicator: deploy the key for direct transformation
  /// (if @c true) or for reverse one (if @c false)
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// (taking into account the direction specified by @c transform) is prohibited by
  /// the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_23710
  virtual ara::core::Result<void> SetKey(
    const SymmetricKey& key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept
    = 0;

  /// @brief Get the kind of transformation configured for this context: kEncrypt or kDecrypt
  /// @returns @c CryptoTransform
  /// @error SecurityErrorDomain::kUninitializedContext  if the transformation direction of this context
  /// is configurable during an initialization, but the context was not initialized yet
  /// @trace SWS_CRYPT_23711
  virtual ara::core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

  /// @brief Indicate that the currently configured transformation accepts only complete blocks of input data.
  /// @returns @c true if the transformation requires the maximum size of input data and @c false otherwise
  /// @error SecurityErrorDomain::kUninitializedContext  if the transformation direction of this context
  /// is configurable during an initialization, but the context was not initialized yet
  /// @trace SWS_CRYPT_23712
  ara::core::Result<bool> IsMaxInputOnly() const noexcept
  {
    //return !IsEncryption();
    auto result = GetTransformation();
    if (!result)
    {
      return ara::core::Result<bool>::FromError(result.Error());
    }
    if (CryptoTransform::kDecrypt == result.Value())
    {
      return ara::core::Result<bool>::FromValue(true);
    }
    else
    {
      return ara::core::Result<bool>::FromValue(false);
    }
  }

  /// @brief Indicate that the currently configured transformation can produce only complete blocks of output
  /// data.
  /// @returns @c true if the transformation can produce only the maximum size of output data and @c false
  /// otherwise.
  /// @error SecurityErrorDomain::kUninitializedContext  if the transformation direction of this context
  /// is configurable during an initialization, but the context was not initialized yet
  /// @trace SWS_CRYPT_23713
  ara::core::Result<bool> IsMaxOutputOnly() const noexcept
  {
    auto result = GetTransformation();

    if (!result)
    {
      return ara::core::Result<bool>::FromError(result.Error());
    }
    return ara::core::Result<bool>::FromValue((GetTransformation().Value() == CryptoTransform::kEncrypt));
  }

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_23714
  virtual ara::core::Result<void> Reset() noexcept = 0;

  /// @brief Process provided blocks without padding.
  /// The @c in and @c out buffers @b must have same size and this size @b must be divisible by the block size
  /// (see @c GetBlockSize()).
  /// <b>Pointers to the input and output buffers must be aligned to the block-size boundary!</b>
  /// @returns  an output data buffer
  /// @param[in] in  an input data buffer
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by
  /// the block size (see @c GetBlockSize())
  /// @error SecurityErrorDomain::kIncompatibleArguments  if sizes of the input and output buffer are not
  /// equal
  /// @error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers partially intersect
  /// @trace SWS_CRYPT_23715
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) const
    noexcept
    = 0;

  /// @brief Process (encrypt / decrypt) an input block according to the cryptorconfiguration.
  /// @returns the output buffer
  /// Encryption with <tt>(suppressPadding == true)</tt> expects that:
  /// <tt>in.size() == GetMaxInputSize(true) && out.size() >= GetMaxOutputSize(true)</tt>.
  /// Encryption with <tt>(suppressPadding == false)</tt> expects that:
  /// <tt>in.size() <= GetMaxInputSize(false) && in.size() > 0 && out.size() >= GetMaxOutputSize(false)</tt>.
  /// Decryption expects that:
  /// <tt>in.size() == GetMaxInputSize() && out.size() >= GetMaxOutputSize(suppressPadding)</tt>.
  /// The case <tt>(out.size() < GetMaxOutputSize())</tt> should be used with caution, only if you are
  /// strictly certain about the size of the output data!
  /// In case of <tt>(suppressPadding == true)</tt> the actual size of plain text should be equal to full
  /// size of the plain data block (defined by the algorithm)!
  /// @param[in] in  the input data block
  /// @param[in] suppressPadding  if @c true then the method doesn't apply the padding, but the payload
  /// should fill the whole block of the plain data
  /// @returns actual size of output data (it always <tt><= out.size()</tt>) or 0 if the input data block has
  /// incorrect content
  /// @error:  SecurityErrorDomain::kIncorrectInputSize  if the mentioned above rules about the input size is
  /// violated
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the @c out.size() is not enough to store the
  /// transformation result
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_23716
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept
    = 0;

  /// @brief Process (encrypt / decrypt) an input block according to the crypto configuration.
  /// This method sets the size of the output container according to actually saved value!
  /// Encryption with <tt>(suppressPadding == true)</tt> expects what:
  /// <tt>in.size() == GetMaxInputSize(true) && out.capacity() >= GetMaxOutputSize(true)</tt>.
  /// Encryption with <tt>(suppressPadding == false)</tt> expects what:
  /// <tt>in.size()<= GetMaxInputSize(false) && in.size()>0 && out.capacity() >=GetMaxOutputSize(false)</tt>.
  /// Decryption expects what:
  /// <tt>in.size() == GetMaxInputSize() && out.capacity() >= GetMaxOutputSize(suppressPadding)</tt>.
  /// The case <tt>(out.capacity() < GetMaxOutputSize())</tt> should be used with caution, only if you are
  /// strictly certain about the size of the output data!
  /// In case of <tt>(suppressPadding == true)</tt> the actual size of plain text should be equal to full
  /// size of the plain data block (defined by the algorithm)!
  /// @tparam Alloc  a custom allocator type of the output container
  /// @returns  the managed container for output block
  /// @param[in] in  the input data block
  /// @param[in] suppressPadding  if @c true then the method doesn't apply the padding, but the payload
  /// should fill the whole block of the plain data
  /// @error SecurityErrorDomain::kIncorrectInputSize  if the mentioned above rules about the input size is
  /// violated
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the @c out.size() is not enough to store the
  /// transformation result
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_23717
  template <typename Alloc = VENDOR_IMPLEMENTATION_DEFINED>
  ara::core::Result<ByteVector<Alloc>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const
    noexcept
  {
    // ara::core::Result<ara::core::Vector<ara::core::Byte>> result = ProcessBlock(in, suppressPadding);
    //
    // if (result)
    // {
    //   ByteVector<Alloc> out;
    //   out.resize(result.Value()->size());
    //   memcpy(core::data(out), result.Value()->data(), result.Value()->size());
    //   return ara::core::Result<ByteVector<Alloc>>::FromValue(out);
    // }
    // else
    // {
    //   return ara::core::Result<ByteVector<Alloc>>::FromError(result.Error());
    // }
  }
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_SYMMETRIC_BLOCK_CIPHER_CTX_H_
