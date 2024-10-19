// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : block_service_impl.h
//
// Purpose     : This file contains definitions for BlockService class implementation.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_BLOCK_SERVICE_IMPL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_BLOCK_SERVICE_IMPL_H_

#include "ara/crypto/cryp/block_service.h"
#include "extension_service_impl.h"

/// @brief namespace for adaptive runtime application.
namespace ara
{
/// @brief namespace for crypto function cluster.
namespace crypto
{
/// @brief namespace for crypto services.
namespace cryp
{
/// @brief Extension meta-information service for block cipher contexts.
/// @trace SWS_CRYPT_29030
class BlockServiceImpl : public BlockService
{
 public:
  /// @brief Constructor for BlockServiceImpl class.
  BlockServiceImpl(CryptoAlgId algoid, CryptoObjectUid keyuuid, CryptoObjectUid* seeduuid, size_t keySize,
      size_t ivSize, AllowedUsageFlags usage, bool isCtxInitialized) noexcept;

  /// @brief Destructor for BlockServiceImpl class.
  ~BlockServiceImpl() noexcept;

  /// @brief Get default expected size of the Initialization Vector (IV) or nonce.
  /// @returns default expected size of IV in bytes
  /// @trace SWS_CRYPT_29032
  std::size_t GetIvSize() const noexcept override;

  /// @brief Get block (or internal buffer) size of the base algorithm.
  /// @returns size of the block in bytes
  /// @trace SWS_CRYPT_29033
  std::size_t GetBlockSize() const noexcept override;

  /// @brief Verify validity of specific Initialization Vector (IV) length.
  /// @param[in] ivSize the length of the IV in bytes
  /// @returns @c true if provided IV length is supported by the algorithm and @c false otherwise
  /// @trace SWS_CRYPT_29034
  bool IsValidIvSize(std::size_t ivSize) const noexcept override;

  /// @brief Get actual bit-length of an IV loaded to the context.
  /// @param[in] ivUid optional pointer to a buffer for saving an @a COUID of a IV object now loaded to
  /// the context.
  /// If the context was initialized by a @c SecretSeed object then the output buffer @c *ivUid must be
  /// filled by @a COUID of this loaded IV object, in other cases @c *ivUid must be filled by all zeros.
  /// @returns actual length of the IV (now set to the algorithm context) in bits
  /// @trace SWS_CRYPT_29035
  std::size_t GetActualIvBitLength(ara::core::Optional<CryptoObjectUid> ivUid) const noexcept override;

  /// @brief Verify supportness of specific key length by the context.
  /// @param[in] keyBitLength length of the key in bits
  /// @returns @c true if provided value of the key length is supported by the context
  /// @trace SWS_CRYPT_29048
  bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept override;

  /// @brief Get minimal supported key length in bits.
  /// @returns minimal supported length of the key in bits
  /// @trace SWS_CRYPT_29043
  std::size_t GetMinKeyBitLength() const noexcept override;

  /// @brief Get maximal supported key length in bits.
  /// @returns maximal supported length of the key in bits
  /// @trace SWS_CRYPT_29044
  std::size_t GetMaxKeyBitLength() const noexcept override;

  /// @brief Get actual bit-length of a key loaded to the context. If no key was set to the context yet
  /// then 0 is returned.
  /// @returns actual length of a key (now set to the algorithm context) in bits
  /// @trace SWS_CRYPT_29045
  std::size_t GetActualKeyBitLength() const noexcept override;

  /// @brief Get allowed usages of this context (according to the key object attributes loaded to this
  /// context).
  /// If the context is not initialized by a key object yet then zero (all flags are reset) must be
  /// returned.
  /// @returns a combination of bit-flags that specifies allowed usages of the context
  /// @trace SWS_CRYPT_29046
  AllowedUsageFlags GetAllowedUsage() const noexcept override;

  /// @brief Get the COUID of the key deployed to the context this extension service is attached to.
  /// If no key was set to the context yet then an empty COUID (Nil) is returned.
  /// @returns the COUID of the CryptoObject
  /// @trace SWS_CRYPT_29047
  CryptoObjectUid GetActualKeyCOUID() const noexcept override;

  /// @brief Check if a key has been set to this context.
  /// @returns FALSE if no key has been set
  /// @trace SWS_CRYPT_29049
  bool IsKeyAvailable() const noexcept override;

 private:
  /// @brief Variable to hold the algorithmId for which this instance is created.
  CryptoAlgId algId_;

  /// @brief Variable to hold the uuid of secret seed deployed in this context.
  CryptoObjectUid* seedUuId_;

  /// @brief Variable to hold the size of the IV
  size_t ivSizeInBits;

  /// @brief Variable to hold the instance of extension service
  ExtensionServiceImpl* extservice_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_BLOCK_SERVICE_IMPL_H_
