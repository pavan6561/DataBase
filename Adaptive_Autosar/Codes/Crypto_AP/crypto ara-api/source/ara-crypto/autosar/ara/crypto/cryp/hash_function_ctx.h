// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : hash_function_ctx.h
//
// Purpose     : This file contains definitions for HashFunctionCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_HASH_FUNCTION_CTX_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_HASH_FUNCTION_CTX_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/core/result.h"
#include "ara/crypto/cryp/digest_service.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Hash function interface.
/// @trace SWS_CRYPT_21100
class HashFunctionCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_21101
  using Uptr = std::unique_ptr<HashFunctionCtx>;

  /// @brief Get DigestService instance.
  /// @return DigestService::Uptr.
  /// @trace SWS_CRYPT_21102
  virtual DigestService::Uptr GetDigestService() const noexcept = 0;  

  /// @brief Initialize the context for a new data stream processing or generation
  /// (depending on the primitive). If IV size is greater than maximally supported by the algorithm then an
  /// implementation may use the leading bytes only from the sequence.
  /// @param[in] iv  an optional Initialization Vector (IV) or "nonce" value
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation, but provided IV value is not empty,
  /// i.e. if <tt>(iv.empty() == false)</tt>
  /// @trace SWS_CRYPT_21110
  virtual ara::core::Result<void> Start(ReadOnlyMemRegion iv) noexcept = 0;

  /// @brief Initialize the context for a new data stream processing or generation (depending on the
  /// primitive) without IV.
  /// @error SecurityErrorDomain::kMissingArgument  the configured hash function expected an IV
  /// @trace SWS_CRYPT_21118
  virtual ara::core::Result<void> Start() noexcept = 0;

  /// @brief Initialize the context for a new data stream processing or generation
  /// (depending on the primitive). If IV size is greater than maximally supported by the algorithm then an
  /// implementation may use the leading bytes only from the sequence.
  /// @param[in] iv  the Initialization Vector (IV) or "nonce" object
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation
  /// @trace SWS_CRYPT_21111
  virtual ara::core::Result<void> Start(const SecretSeed& iv) noexcept = 0;

  /// @brief Update the digest calculation context by a new part of the message.
  /// This method is dedicated for cases then the @c RestrictedUseObject is a part of the "message".
  /// @param[in] in  a part of input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @trace SWS_CRYPT_21112
  virtual ara::core::Result<void> Update(const RestrictedUseObject& in) noexcept = 0;

  /// @brief Update the digest calculation context by a new part of the message.
  /// @param[in] in  a part of the input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @trace SWS_CRYPT_21113
  virtual ara::core::Result<void> Update(ReadOnlyMemRegion in) noexcept = 0;

  /// @brief Update the digest calculation context by a new part of the message.
  /// This method is convenient for processing of constant tags.
  /// @param[in] in  a byte value that is a part of input message
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @trace SWS_CRYPT_21114
  virtual ara::core::Result<void> Update(std::uint8_t in) noexcept = 0;

  /// @brief Finish the digest calculation and optionally produce the "signature" object.
  /// Only after call of this method the digest can be signed, verified, extracted or compared.
  /// @param[in] makeSignatureObject  if this argument is @c true then the method will also produce the
  /// signature object
  /// @returns unique smart pointer to created signature object, if <tt>(makeSignatureObject == true)</tt> or
  /// an empty Signature object if <tt>(makeSignatureObject == false)</tt>
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @error SecurityErrorDomain::kInvalidUsageOrder  if the digest calculation has not started yet or not
  /// been updated at least once
  /// @trace SWS_CRYPT_21115
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> Finish() noexcept = 0;

  /// @brief Get requested part of calculated digest.
  /// Entire digest value is kept in the context up to next call @c Start(), therefore any its part can be
  /// extracted again or verified.
  /// If <tt>(full_digest_size <= offset)</tt> then <tt>return_size = 0</tt> bytes;
  /// else <tt>return_size = min(output.size(), (full_digest_size - offset))</tt> bytes.
  /// This method can be implemented as "inline" after standardization of function @c ara::core::memcpy().
  /// @param[out] output  an output buffer for storing the requested digest fragment (or fully)
  /// @param[in] offset  position of the first byte of digest that should be placed to the output buffer
  /// @returns number of digest bytes really stored to the output buffer (they are always <= @c output.size()
  /// and denoted below as @a return_size)
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the digest calculation was not finished
  /// by a call of the @c Finish() method
  /// @trace SWS_CRYPT_21116
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> GetDigest(std::size_t offset = 0) const
    noexcept
    = 0;

  /// @brief Get requested part of calculated digest to pre-reserved managed container.
  /// This method sets the size of the output container according to actually saved value.
  /// Entire digest value is kept in the context up to next call @c Start(), therefore any its part can be
  /// extracted again or verified.
  /// If <tt>(full_digest_size <= offset)</tt> then <tt>return_size = 0</tt> bytes;
  /// else <tt>return_size = min(output.capacity(), (full_digest_size - offset))</tt> bytes.
  /// @tparam Alloc  a custom allocator type of the output container
  /// @param[out] output  a managed container for storing the requested digest fragment (or fully)
  /// @param[in] offset  position of first byte of digest that should be placed to the output buffer
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the digest calculation was not finished
  /// by a call of the @c Finish() method
  /// @error SecurityErrorDomain::kUsageViolation  if the buffered digest belongs to a MAC/HMAC/AE/AEAD
  /// context initialized by a key without @c kAllowSignature permission
  /// @trace SWS_CRYPT_21117
  template <typename Alloc = VENDOR_IMPLEMENTATION_DEFINED>
  ara::core::Result<ByteVector<Alloc>> GetDigest(std::size_t offset = 0) const noexcept
  {
    ara::core::Result<ara::core::Vector<ara::core::Byte>> result = GetDigest(offset);
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
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_HASH_FUNCTION_CTX_H_
