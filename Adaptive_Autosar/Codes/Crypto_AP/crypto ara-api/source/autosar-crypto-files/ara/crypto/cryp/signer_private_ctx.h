// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signer_private_ctx.h
//
// Purpose     : This file contains definitions for SignerPrivateCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_SIGNER_PRIVATE_CTX_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_SIGNER_PRIVATE_CTX_H_

#include <ara/core/result.h>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/signature_service.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace cryp
{
/// @brief Signature Private key Context interface.
/// @trace SWS_CRYPT_23500
class SignerPrivateCtx : public CryptoContext
{
 public:
  
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_23501
  using Uptr = std::unique_ptr<SignerPrivateCtx>;

  /// @brief Get SignatureService instance.
  /// @trace SWS_CRYPT_23510
  virtual SignatureService::Uptr GetSignatureService() const noexcept = 0;

  /// @brief Sign a provided digest value stored in the hash-function context.
  /// This method must put the hash-function algorithm ID and a @a COUID of the used key-pair to the
  /// resulting signature object!
  /// The user supplied @c context may be used for such algorithms as: Ed25519ctx, Ed25519ph, Ed448ph.
  /// If the target algorithm doesn't support the @c context argument then the empty (default) value must be
  /// supplied!
  /// @param[in] hashFn  a finalized hash-function context that contains a digest value ready for sign
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns unique smart pointer to serialized signature
  /// @error SecurityErrorDomain::kInvalidArgument  if hash-function algorithm does not comply with
  /// the signature algorithm specification of this context
  /// @error SecurityErrorDomain::kInvalidInputSize  if the user supplied @c context has incorrect
  /// (or unsupported) size
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the method @c hash.Finish() was not called
  /// before the call of this method
  /// @error SecurityErrorDomain::kUninitializedContext  this context was not initialized by a key value
  /// @trace SWS_CRYPT_23511
  virtual ara::core::Result<Signature::Uptrc> SignPreHashed(
    const HashFunctionCtx& hashFn, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept
    = 0;

  /// @brief Sign a directly provided hash or message value.
  /// This method can be used for implementation of the "multiple passes" signature algorithms that process
  /// a message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
  /// implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA).
  /// If the target algorithm doesn't support the @c context argument then the empty (default) value must be
  /// supplied!
  /// @param[in] value  the (pre-)hashed or direct message value that should be signed
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns actual size of the signature value stored to the output buffer
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the input @c value or @c context arguments
  /// are incorrect / unsupported
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_23512
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> Sign(
    ReadOnlyMemRegion value, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept
    = 0;

  /// @brief Sign a directly provided digest value and create the @c Signature object.
  /// This method must put the hash-function algorithm ID and a @a COUID of the used key-pair to the
  /// resulting signature object!
  /// The user supplied @c context may be used for such algorithms as: Ed25519ctx, Ed25519ph, Ed448ph.
  /// If the target algorithm doesn't support the @c context argument then the empty (default) value must be
  /// supplied!
  /// @param[in] hashAlgId  hash function algorithm ID
  /// @param[in] hashValue  hash function value (resulting digest without any truncations)
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns unique smart pointer to serialized signature
  /// @error SecurityErrorDomain::kInvalidArgument  if hash-function algorithm does not comply with
  /// the signature algorithm specification of this context
  /// @error SecurityErrorDomain::kInvalidInputSize  if the user supplied @c context has incorrect
  /// (or unsupported) size
  /// @error SecurityErrorDomain::kUninitializedContext  this context was not initialized by a key value
  /// @trace SWS_CRYPT_23513
  virtual ara::core::Result<Signature::Uptrc> SignPreHashed(AlgId hashAlgId, ReadOnlyMemRegion hashValue,
    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept
    = 0;

  /// @brief Sign a directly provided hash or message value.
  /// This method can be used for implementation of the "multiple passes" signature algorithms that process
  /// a message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
  /// implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA).
  /// This method sets the size of the output container according to actually saved value!
  /// If the target algorithm doesn't support the @c context argument then the empty (default) value must be
  /// supplied!
  /// @tparam Alloc  a custom allocator type of the output container
  /// @param[out] signature  pre-reserved managed container for resulting signature
  /// @param[in] value  the (pre-)hashed or direct message value that should be signed
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the input @c value or @c context arguments
  /// are incorrect / unsupported
  /// @error SecurityErrorDomain::kInsufficientCapacity  if capacity of the output @c signature container
  /// is not enough
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_23514
  template <typename Alloc = VENDOR_IMPLEMENTATION_DEFINED>
  ara::core::Result<ByteVector<Alloc>> Sign(
    ReadOnlyMemRegion value, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept
  {
    ara::core::Result<ara::core::Vector<ara::core::Byte>> result = Sign(value, context);
    if (result)
    {
      ByteVector<Alloc> signature;
      signature.resize(result.Value().size());
      memcpy(core::data(signature), result.Value().data(), result.Value().size());
      return ara::core::Result<ByteVector<Alloc>>::FromValue(signature);
    }
    else
    {
      return ara::core::Result<ByteVector<Alloc>>::FromError(result.Error());
    }
  }

  /// @brief Set (deploy) a key to the signer private algorithm context.
  /// @param[in] key  the source key object
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_23515
  virtual ara::core::Result<void> SetKey(const PrivateKey& key) noexcept = 0;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_23516
  virtual ara::core::Result<void> Reset() noexcept = 0;

  /// @brief Getter method to get the private key loaded to this ctx.
  virtual EVP_PKEY* GetPrivateKey() const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_SIGNER_PRIVATE_CTX_H_
