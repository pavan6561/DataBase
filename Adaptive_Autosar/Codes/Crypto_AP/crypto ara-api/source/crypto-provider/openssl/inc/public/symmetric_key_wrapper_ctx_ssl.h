// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_key_wrapper_ctx_ssl.h
//
// Purpose     : This file contains definitions for SymmetricKeyWrapperCtxWrapSSL128 class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_SYMMETRIC_KEY_WRAPPER_CTX_SSL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_SYMMETRIC_KEY_WRAPPER_CTX_SSL_H_

#include <ara/log/logging.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/symmetric_key_wrapper_ctx.h"
#include "extension_service_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Context of a symmetric key wrap algorithm (for AES it should be compatible with RFC3394
/// or RFC5649). The public interface of this context is dedicated for raw key material wrapping/unwrapping,
/// i.e. without any meta-information assigned to the key material in source crypto object. But additionally
/// this context type should support some "hidden" low-level methods suitable for whole crypto object
/// exporting/ importing. %Key Wrapping of a whole crypto object (including associated meta-information) can
/// be done by methods: @c ExportSecuredObject() and @c ImportSecuredObject(), but without compliance to
/// RFC3394 or RFC5649.
/// @trace SWS_CRYPT_24000
class SymmetricKeyWrapperCtxWrapDefaultSSL : public SymmetricKeyWrapperCtx
{
 public:
  /// @brief Constructor for the SymmetricKeyWrapperCtxWrapDefaultSSL class.
  /// @param algorithm Id for symmetric key wrapping.
  explicit SymmetricKeyWrapperCtxWrapDefaultSSL(AlgId algoId) noexcept;

  /// @brief Destructor for the SymmetricKeyWrapperCtxWrapDefaultSSL class.
  ~SymmetricKeyWrapperCtxWrapDefaultSSL() noexcept override;

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_24002
  ExtensionService::Uptr GetExtensionService() const noexcept;

  /// @brief Get expected granularity of the target key (block size).
  /// If the class implements RFC3394 (KW without padding) then this method should return 8
  /// (i.e. 8 octets = 64 bits).
  /// If the class implements RFC5649 (KW with padding) then this method should return 1 (i.e. 1 octet = 8
  /// bits).
  /// @returns size of the block in bytes
  /// @trace SWS_CRYPT_24011_001, SWS_CRYPT_24011_002, SWS_CRYPT_02107
  std::size_t GetTargetKeyGranularity() const noexcept override;

  /// @brief Get maximum length of the target key supported by the implementation.
  /// This method can be useful for some implementations different from RFC3394 / RFC5649.
  /// @returns maximum length of the target key in bits
  /// @trace SWS_CRYPT_24012
  std::size_t GetMaxTargetKeyLength() const noexcept override;

  /// @brief Calculate size of the wrapped key in bytes from original key length in bits.
  /// This method can be useful for some implementations different from RFC3394 / RFC5649.
  /// @param[in] keyLength  original key length in bits
  /// @returns size of the wrapped key in bytes
  /// @trace SWS_CRYPT_24013
  std::size_t CalculateWrappedKeySize(std::size_t keyLength) const noexcept override;

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
  ara::core::Result<ara::core::Vector<ara::core::Byte>> WrapKeyMaterial(const RestrictedUseObject& key) const
    noexcept override;

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
  ara::core::Result<SecretSeed::Uptrc> UnwrapSeed(
    ReadOnlyMemRegion wrappedSeed, AlgId targetAlgId, SecretSeed::Usage allowedUsage) const noexcept override;

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
  ara::core::Result<RestrictedUseObject::Uptrc> UnwrapKey(
    ReadOnlyMemRegion wrappedKey, AlgId algId, AllowedUsageFlags allowedUsage) const noexcept override;

  /// @brief Set (deploy) a key to the symmetric key wrapper algorithm context.
  /// @param[in] key  the source key object
  /// @param[in] directTransform  the "direction" indicator: deploy the key for direct transformation (if @c
  /// true) for reverse one (if @c false)
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// (taking into account the direction specified by @c transform) is prohibited by
  /// the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_24018_001, SWS_CRYPT_24018_002, SWS_CRYPT_24018_003, SWS_CRYPT_02101
  ara::core::Result<void> SetKey(const SymmetricKey& key, CryptoTransform transform) noexcept override;

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override
  {
    aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::GetCryptoPrimitiveId()";

    // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
    // pointer which will have primitive information related to this particular ctx.
    CryptoPrimitiveId::Uptr primitiveInfoPtr{ std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algorithmId_), algorithmId_) };
    return primitiveInfoPtr;
  }

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use,
  /// and @c false otherwise
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override
  {
    aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::IsInitialized()";
    return isCtxInitialized_;
  }

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_24019
  ara::core::Result<void> Reset() noexcept override;

 private:
  /// @brief Method to get the openssl cipher mode.
  EVP_CIPHER const* GetCipherMode() const noexcept;

  /// @brief Method to get the expected key size from the given target algorithm Id.
  /// @param[in] targetAlgId ID for the target algorithm of the key.
  size_t GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept;

  /// @brief Shared pointer for algorithm Identification class.
  std::shared_ptr<AlgorithmIdentification> algoIdentification_{
    AlgorithmIdentification::GetAlogIdentification()
  };

  /// @brief Pointer to the openssl cipher context.
  EVP_CIPHER_CTX* symmetricKeyWrapperctx_{ nullptr };

  /// @brief Buffer to hold key encryption key.
  ara::core::Vector<uint8_t> kek_{};

  /// @brief Variable to hold the transformation for this context.
  CryptoTransform ctxTransform_{ CryptoTransform::kEncrypt };

  /// @brief Flag to indicate if the context is initialized.
  bool isCtxInitialized_{ false };

  /// @brief Variable to hold the algorithm ID for key wrapping.
  AlgId algorithmId_;

  /// @brief Logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "SKWC", "Symmetric Key Wrapping context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif //CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_SYMMETRIC_KEY_WRAPPER_CTX_SSL_H_
