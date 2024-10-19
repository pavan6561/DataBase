// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : decryptor_private_ctx.h
//
// Purpose     : This file contains definitions for DecryptorPrivateCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_DECRYPTOR_PRIVATE_CTX_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_DECRYPTOR_PRIVATE_CTX_H_

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/crypto_service.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace cryp
{
/// @brief Asymmetric Decryption Private key Context interface.
/// @trace SWS_CRYPT_20800
class DecryptorPrivateCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_20801
  using Uptr = std::unique_ptr<DecryptorPrivateCtx>;

  /// @brief Get CryptoService instance.
  /// @trace SWS_CRYPT_20802
  virtual CryptoService::Uptr GetCryptoService() const noexcept = 0;

  /// @brief Set (deploy) a key to the decryptor private algorithm context.
  /// @param[in] key  the source key object
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_20810
  virtual ara::core::Result<void> SetKey(const PrivateKey& key) noexcept = 0;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_20811
  virtual ara::core::Result<void> Reset() noexcept = 0;

  /// @brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
  /// Encryption with <tt>(suppressPadding == true)</tt> expects that:
  /// <tt>in.size() == GetMaxInputSize(true) && out.size() >= GetMaxOutputSize(true)</tt>.
  /// Encryption with <tt>(suppressPadding == false)</tt> expects that:
  /// <tt>in.size() <= GetMaxInputSize(false) && in.size() > 0 && out.size() >=
  /// GetMaxOutputSize(false)</tt>.
  /// Decryption expects that:
  /// <tt>in.size() == GetMaxInputSize() && out.size() >= GetMaxOutputSize(suppressPadding)</tt>.
  /// The case <tt>(out.size() < GetMaxOutputSize())</tt> should be used with caution, only if you are
  /// strictly certain about the size of the output data!
  /// In case of <tt>(suppressPadding == true)</tt> the actual size of plain text should be equal to full
  /// size of the plain data block (defined by the algorithm)!
  /// @param[in] in  the input data block
  /// @param[in] suppressPadding  if @c true then the method doesn't apply the padding, but the payload
  /// should fill the whole block of the plain data
  /// @returns actual size of output data (it always <tt><= out.size()</tt>) or 0 if the input data block
  /// has incorrect content
  /// @error SecurityErrorDomain::kIncorrectInputSize  if the mentioned above rules about the input size is
  /// violated
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the @c out.size() is not enough to store the
  /// transformation result
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_20812
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept
    = 0;

  /// @brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
  /// This method sets the size of the output container according to actually saved value!
  /// Encryption with <tt>(suppressPadding == true)</tt> expects what:
  /// <tt>in.size() == GetMaxInputSize(true) && out.capacity() >= GetMaxOutputSize(true)</tt>.
  /// Encryption with <tt>(suppressPadding == false)</tt> expects what:
  /// <tt>in.size() <= GetMaxInputSize(false) && in.size() > 0 && out.capacity()
  /// >= GetMaxOutputSize(false)</tt>.
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
  /// @trace SWS_CRYPT_20813
  template <typename Alloc = VENDOR_IMPLEMENTATION_DEFINED>
  ara::core::Result<ByteVector<Alloc>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const
    noexcept
  {
    ara::core::Result<ara::core::Vector<ara::core::Byte>> result = ProcessBlock(in, suppressPadding);
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
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_DECRYPTOR_PRIVATE_CTX_H_
