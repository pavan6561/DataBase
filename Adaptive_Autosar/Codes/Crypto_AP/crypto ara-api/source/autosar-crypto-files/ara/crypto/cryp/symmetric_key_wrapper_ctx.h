// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_key_wrapper_ctx.h
//
// Purpose     : This file contains definitions for SymmetricKeyWrapperCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_WRAPPER_CTX_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_WRAPPER_CTX_H_

#include <cstddef>

#include <ara/core/result.h>
#include <ara/core/utility.h>
#include <ara/core/vector.h>
#include <iomanip>
#include <iostream>
#include <string.h>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "crypto_primitive_id_impl.h"
#include "secret_seed_impl.h"
#include "symmetric_key_impl.h"
#include "ara/crypto/cryp/extension_service.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Context of a symmetric key wrap algorithm (for AES it should be compatible with RFC3394 or
/// RFC5649). The public interface of this context is dedicated for raw key material wrapping/unwrapping,
/// i.e. without any meta-information assigned to the key material in source crypto object. But additionally
/// this context type should support some "hidden" low-level methods suitable for whole crypto object
/// exporting/importing. Key Wrapping of a whole crypto object (including associated meta-information)
/// can be done by methods: @c ExportSecuredObject() and @c ImportSecuredObject(), but without compliance to
/// RFC3394 or RFC5649.
/// @trace SWS_CRYPT_24000
class SymmetricKeyWrapperCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_24001
  using Uptr = std::unique_ptr<SymmetricKeyWrapperCtx>;

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_24002
  virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

  /// @brief Get expected granularity of the target key (block size).
  /// If the class implements RFC3394 (KW without padding) then this method should return 8
  /// (i.e. 8 octets = 64 bits).
  /// If the class implements RFC5649 (KW with padding) then this method should return 1 (i.e. 1 octet = 8
  /// bits).
  /// @returns size of the block in bytes
  /// @trace SWS_CRYPT_24011_001, SWS_CRYPT_24011_002, SWS_CRYPT_02107
  virtual std::size_t GetTargetKeyGranularity() const noexcept = 0;

  /// @brief Get maximum length of the target key supported by the implementation.
  /// This method can be useful for some implementations different from RFC3394 / RFC5649.
  /// @returns maximum length of the target key in bits
  /// @trace SWS_CRYPT_24012
  virtual std::size_t GetMaxTargetKeyLength() const noexcept = 0;

  /// @brief Calculate size of the wrapped key in bytes from original key length in bits.
  /// This method can be useful for some implementations different from RFC3394 / RFC5649.
  /// @param[in] keyLength  original key length in bits
  /// @returns size of the wrapped key in bytes
  /// @trace SWS_CRYPT_24013
  virtual std::size_t CalculateWrappedKeySize(std::size_t keyLength) const noexcept = 0;

  /// @brief Execute the "key wrap" operation for the provided key material.
  /// This method should be compliant to RFC3394 or RFC5649, if an implementation is based on the AES block
  /// cipher and applied to an AES key.
  /// Method @c CalculateWrappedKeySize() can be used for size calculation of the required output buffer.
  /// @param[out] wrapped  an output buffer for the wrapped key
  /// @param[in] key  a key that should be wrapped
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the size of the @c wrapped buffer is not enough
  /// for storing the result
  /// @error SecurityErrorDomain::kInvalidInputSize  if the @c key object has an unsupported length
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_24014_001, SWS_CRYPT_24014_003, SWS_CRYPT_24014_004, SWS_CRYPT_02102
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> WrapKeyMaterial(
    const RestrictedUseObject& key) const noexcept
    = 0;

  /// @brief Execute the "key unwrap" operation for provided BLOB and produce @c SecretSeed object.
  /// This method should be compliant to RFC3394 or RFC5649, if implementation is based on the AES block
  /// cipher and applied to an AES key material.
  /// The created @c SecretSeed object has following attributes: session and non-exportable (because it was
  /// imported without meta-information).
  /// @param[in] wrappedSeed  a memory region that contains wrapped seed
  /// @param[in] targetAlgId  the target symmetric algorithm identifier (also defines a target seed-length)
  /// @param[in] allowedUsage  allowed usage scope of the target seed
  /// @returns unique smart pointer to @c SecretSeed object, which keeps unwrapped key material
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided wrapped seed is unsupported
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_24015_001, SWS_CRYPT_24015_002, SWS_CRYPT_24015_003, SWS_CRYPT_02103
  /// SWS_CRYPT_02104, SWS_CRYPT_02105, SWS_CRYPT_02106_001, SWS_CRYPT_02106_002
  virtual ara::core::Result<SecretSeed::Uptrc> UnwrapSeed(
    ReadOnlyMemRegion wrappedSeed, AlgId targetAlgId, SecretSeed::Usage allowedUsage) const noexcept
    = 0;

  /// @brief Execute the "key unwrap" operation for provided BLOB and produce @c Key object.
  /// This method should be compliant to RFC3394 or RFC5649, if implementation is based on the AES block
  /// cipher and applied to an AES key.
  /// The created @c Key object has following attributes: session and non-exportable (because it was imported
  /// without meta-information)!
  /// @c SymmetricKey may be unwrapped in following way:
  /// <tt>SymmetricKey::Uptrc key = SymmetricKey::Cast(UnwrapKey(wrappedKey, ...));</tt> @n
  /// @c PrivateKey may be unwrapped in following way:
  /// <tt>PrivateKey::Uptrc key = PrivateKey::Cast(UnwrapKey(wrappedKey, ...));</tt> @n
  /// In both examples the @c Cast() method may additionally @b throw the @c BadObjectTypeException if an
  /// actual type of the unwrapped key differs from the target one!
  /// @param[in] wrappedKey  a memory region that contains wrapped key
  /// @param[in] algId  an identifier of the target symmetric crypto algorithm
  /// @param[in] allowedUsage  bit-flags that define a list of allowed transformations' types in which the
  /// target key can be used
  /// @returns unique smart pointer to @c Key object, which keeps unwrapped key material
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided wrapped key is unsupported
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_24016_001, SWS_CRYPT_24016_002, SWS_CRYPT_24016_003, SWS_CRYPT_02103
  /// SWS_CRYPT_02104, SWS_CRYPT_02105, SWS_CRYPT_02106_001, SWS_CRYPT_02106_002
  virtual ara::core::Result<RestrictedUseObject::Uptrc> UnwrapKey(
    ReadOnlyMemRegion wrappedKey, AlgId algId, AllowedUsageFlags allowedUsage) const noexcept
    = 0;

  /// @brief Set (deploy) a key to the symmetric key wrapper algorithm context.
  /// @param[in] key  the source key object
  /// @param[in] directTransform  the "direction" indicator: deploy the key for direct transformation (if @c
  /// true) or for reverse one (if @c false)
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// (taking into account the direction specified by @c transform) is prohibited by
  /// the "allowed usage" restrictions of provided key object
  virtual ara::core::Result<void> SetKey(const SymmetricKey& key, CryptoTransform transform) noexcept = 0;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_24019
  virtual ara::core::Result<void> Reset() noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_WRAPPER_CTX_H_
