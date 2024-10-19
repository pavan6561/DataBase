// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : public_key.h
//
// Purpose     : This file contains definitions for PublicKey abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_PUBLIC_KEY_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_PUBLIC_KEY_H_

#include <openssl/evp.h>
#include <openssl/pem.h>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace cryp
{
/// @brief General Asymmetric Public %Key interface.
/// @trace SWS_CRYPT_22700
class PublicKey
  : public RestrictedUseObject
  , public Serializable
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_22701.
  using Uptrc = std::unique_ptr<const PublicKey>;

  /// @brief const object type
  /// @trace SWS_CRYPT_22702
  static const CryptoObjectType kObjectType = CryptoObjectType::kPublicKey;

  /// @brief constructor for PublicKey class.
  PublicKey(ReadWriteMemRegion key) : RestrictedUseObject(key)
  {
  }

  /// @traceid{SWS_CRYPT_22711}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02202}
  /// @brief Check the key for its correctness.
  /// @param[in] strongCheck  the severeness flag that indicates type of the required check:
  ///            strong (if @c true) or fast (if @c false)
  /// @returns @c true if the key is correct
  /// @trace SWS_CRYPT_22711
  //virtual bool CheckKey(bool strongCheck = true) const noexcept = 0;

  /// @brief Calculate hash of the Public Key value.
  /// The original public key value BLOB is available via the @c Serializable interface.
  /// @param[in] hashFunc  a hash-function instance that should be used the hashing
  /// @returns a buffer preallocated for the resulting hash value
  /// @error SecurityErrorDomain::kInsufficientCapacity  if size of the hash buffer is not enough for
  /// storing of the result
  /// @error SecurityErrorDomain::kIncompleteArgState  if the @c hashFunc context is not initialized
  /// @trace SWS_CRYPT_22712
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> HashPublicKey(HashFunctionCtx& hashFunc) const
    noexcept
    = 0;

  /// @brief Calculate hash of the Public Key value.
  /// The original public key value BLOB is available via the @c Serializable interface.
  /// @param[in] hashFunc  a hash-function instance that should be used the hashing
  /// @returns a buffer preallocated for the resulting hash value
  /// @error SecurityErrorDomain::kInsufficientCapacity  if size of the hash buffer is not enough for
  /// storing of the result
  /// @error SecurityErrorDomain::kIncompleteArgState  if the @c hashFunc context is not initialized
  /// @trace SWS_CRYPT_22713
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

  /// @brief Getter method to get the public key value in text format.
  ReadWriteMemRegion GetPublicKeyValue() const
  {
    return GetValue();
  }

  /// @brief Getter method to get the public key value in openssl object format.
  virtual EVP_PKEY* GetPublicKey() const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_PUBLIC_KEY_H_
