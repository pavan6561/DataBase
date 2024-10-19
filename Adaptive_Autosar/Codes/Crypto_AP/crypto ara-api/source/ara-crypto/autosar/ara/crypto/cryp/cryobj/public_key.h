// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : public_key.h
//
// Purpose     : This file contains definitions for Public Key abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_PUBLIC_KEY_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_PUBLIC_KEY_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/hash_function_ctx.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
//- @interface PublicKey
///
/// @traceid{SWS_CRYPT_22700}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02202}
/// @brief General Asymmetric Public %Key interface.
///

class PublicKey
  : public RestrictedUseObject
  , public Serializable
{
 public:
  /// @traceid{SWS_CRYPT_22701}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02202}
  /// @brief Unique smart pointer of the interface.

  using Uptrc = std::unique_ptr<const PublicKey>;

  /// @traceid{SWS_CRYPT_22702}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02202}
  /// @brief const object type

  static const CryptoObjectType kObjectType = CryptoObjectType::kPublicKey;

  /// @traceid{SWS_CRYPT_22711}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02202}
  /// @brief Check the key for its correctness.
  /// @param[in] strongCheck  the severeness flag that indicates type of the required check:
  /// strong (if @c true) or fast (if @c false)
  /// @returns @c true if the key is correct
  /// @threadsafety{Thread-safe}

  virtual bool CheckKey(bool strongCheck = true) const noexcept = 0;

  /// @traceid{SWS_CRYPT_22712}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02202}
  /// @brief Calculate hash of the Public Key value.
  /// The original public key value BLOB is available via the @c Serializable interface.
  /// @param[in] hashFunc  a hash-function instance that should be used the hashing
  /// @returns a buffer preallocated for the resulting hash value
  /// @error SecurityErrorDomain::kInsufficientCapacity  if size of the hash buffer is not enough for
  /// storing of the result
  /// @error SecurityErrorDomain::kIncompleteArgState  if the @c hashFunc context is not initialized
  /// @threadsafety{Thread-safe}
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> HashPublicKey(HashFunctionCtx& hashFunc) const
    noexcept
    = 0;

  /// @traceid{SWS_CRYPT_22713}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02202}
  /// @brief Calculate hash of the Public Key value.
  /// This method sets the size of the output container according to actually saved value!
  /// The original public key value BLOB is available via the Serializable interface.
  /// @tparam Alloc  a custom allocator type of the output container
  /// @returns  pre-reserved managed container for the resulting hash value
  /// @param[in] hashFunc  a hash-function instance that should be used the hashing

  /// @error SecurityErrorDomain::kInsufficientCapacity  if capacity of the hash buffer is not enough
  /// for storing of the result.
  /// @error SecurityErrorDomain::kIncompleteArgState  if the @c hashFunc context is not initialized
  /// @threadsafety{Thread-safe}

  template <typename Alloc = VENDOR_IMPLEMENTATION_DEFINED>
  ara::core::Result<ByteVector<Alloc>> HashPublicKey(HashFunctionCtx& hashFunc) const noexcept
  {
    //        ara::core::Result<ara::core::Vector<ara::core::Byte>> result = HashPublicKey( hashFunc);
    //        if(result)
    //        {
    //            ByteVector<Alloc> hash;
    //            hash.resize(result.Value()->size());
    //            memcpy(core::data(hash), result.Value()->data(), result.Value()->size());
    //            return ara::core::Result<ByteVector<Alloc>>::FromValue(hash);
    //        }
    //        else
    //        {
    //            return ara::core::Result<ByteVector<Alloc>>::FromError(result.Error());
    //        }
  }
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_PUBLIC_KEY_H_
