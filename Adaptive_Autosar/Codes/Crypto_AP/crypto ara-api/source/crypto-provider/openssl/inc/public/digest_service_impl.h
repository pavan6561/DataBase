// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : digest_service_impl.h
//
// Purpose     :  This file contains definitions for DigestService class implementation.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_DIGEST_SERVICE_IMPL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_DIGEST_SERVICE_IMPL_H_

#include "ara/crypto/cryp/digest_service.h"
#include "ara/core/string.h"
#include "ara/crypto/common/security_error_domain.h"
#include "block_service_impl.h"

/// @brief namespace for adaptive runtime application.
namespace ara
{
/// @brief namespace for crypto function cluster.
namespace crypto
{
/// @brief namespace for crypto services.
namespace cryp
{
/// @brief Implementation for Digest Service.
class DigestServiceImpl : public DigestService
{
 public:
  /// @brief Constructor for DigestServiceImp class.
  DigestServiceImpl(CryptoAlgId algoid, CryptoObjectUid keyuuid, CryptoObjectUid* seeduuid, size_t keySize,
      size_t ivSize, AllowedUsageFlags usage, ReadOnlyMemRegion digest, bool isCtxInitialized, bool isStarted,
      bool isFinished) noexcept;

  /// @brief Destructor for DigestServiceImp class.
  ~DigestServiceImpl() noexcept;

  /// @brief Get the output digest size.
  /// @returns size of the full output from this digest-function in bytes
  /// @trace SWS_CRYPT_29012
  std::size_t GetDigestSize() const noexcept;

  /// @brief Compare the calculated digest against an expected value.
  /// be Entire digest value is kept in the context up to next call @c Start(), therefore any its part can
  /// verified again or extracted.
  /// If <tt>(full_digest_size <= offset) || (expected.size() == 0)</tt> then return @c false;
  /// else <tt>comparison_size = min(expected.size(), (full_digest_size - offset))</tt> bytes.
  /// This method can be implemented as "inline" after standardization of function @c ara::core::memcmp().
  /// @param[in] expected  the memory region containing an expected digest value
  /// @param[in] offset  position of the first byte in calculated digest for the comparison starting
  /// @returns @c true if the expected bytes sequence is identical to first bytes of calculated digest
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the digest calculation was not finished
  /// by a call of the @c Finish() method
  /// @error SecurityErrorDomain::kBruteForceRisk  if the buffered digest belongs to a MAC/HMAC/AE/AEAD
  /// context, which was initialized by a key without @c kAllowSignature permission, but actual
  /// size of requested digest is less than 8 bytes (it is a protection from the brute-force attack)
  /// @trace SWS_CRYPT_29013
  ara::core::Result<bool> Compare(ReadOnlyMemRegion expected, std::size_t offset = 0) const noexcept;

  /// @brief Check current status of the stream processing: started or no.
  /// @returns @c true if the processing was start by a call of the @c Start()
  /// methods and was not finished yet.
  /// @trace SWS_CRYPT_29014
  bool IsStarted() const noexcept;

  /// @brief Check current status of the stream processing: finished or no.
  /// @returns @c true if a previously started stream processing was finished by a call of the @c Finish()
  /// or @c FinishBytes() methods
  /// @trace SWS_CRYPT_29015
  bool IsFinished() const noexcept;

  /// @brief Get default expected size of the Initialization Vector (IV) or nonce.
  /// @returns default expected size of IV in bytes
  /// @trace SWS_CRYPT_29032
  std::size_t GetIvSize() const noexcept;

  /// @brief Get block (or internal buffer) size of the base algorithm.
  /// @returns size of the block in bytes
  /// @trace SWS_CRYPT_29033
  std::size_t GetBlockSize() const noexcept;

  /// @brief Verify validity of specific Initialization Vector (IV) length.
  /// @param[in] ivSize  the length of the IV in bytes
  /// @returns @c true if provided IV length is supported by the algorithm and @c false otherwise
  /// @trace SWS_CRYPT_29034
  bool IsValidIvSize(std::size_t ivSize) const noexcept;

  /// @brief Get actual bit-length of an IV loaded to the context.
  /// @param[in] ivUid  optional pointer to a buffer for saving an @a COUID of a IV object now loaded to
  /// the context.
  /// If the context was initialized by a @c SecretSeed object then the output buffer @c *ivUid must be
  /// filled by @a COUID of this loaded IV object, in other cases @c *ivUid must be filled by all zeros.
  /// @returns actual length of the IV (now set to the algorithm context) in bits
  /// @trace SWS_CRYPT_29035
  std::size_t GetActualIvBitLength(ara::core::Optional<CryptoObjectUid> ivUid) const noexcept;

  /// @brief Verify supportiveness of specific key length by the context.
  /// @param[in] keyBitLength  length of the key in bits
  /// @returns @c true if provided value of the key length is supported by the context
  /// @trace SWS_CRYPT_29048
  bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept;

  /// @brief Get minimal supported key length in bits.
  /// @returns minimal supported length of the key in bits
  /// @trace SWS_CRYPT_29043
  std::size_t GetMinKeyBitLength() const noexcept;

  /// @brief Get maximal supported key length in bits.
  /// @returns maximal supported length of the key in bits
  /// @trace SWS_CRYPT_29044
  std::size_t GetMaxKeyBitLength() const noexcept;

  /// @brief Get actual bit-length of a key loaded to the context. If no key was set to the context yet
  /// then 0 is returned.
  /// @returns actual length of a key (now set to the algorithm context) in bits
  /// @trace SWS_CRYPT_29045
  std::size_t GetActualKeyBitLength() const noexcept;

  /// @brief Get allowed usages of this context (according to the key object attributes
  /// loaded to this context).
  /// If the context is not initialized by a key object yet then zero (all flags are reset)
  /// must be returned.
  /// @returns a combination of bit-flags that specifies allowed usages of the context
  /// @trace SWS_CRYPT_29046
  AllowedUsageFlags GetAllowedUsage() const noexcept;

  /// @brief Get the COUID of the key deployed to the context this extension service is attached to.
  /// If no key was set to the context yet then an empty COUID (Nil) is returned.
  /// @returns the COUID of the CryptoObject
  /// @trace SWS_CRYPT_29047
  CryptoObjectUid GetActualKeyCOUID() const noexcept;

  /// @brief Check if a key has been set to this context.
  /// @returns FALSE if no key has been set
  /// @trace SWS_CRYPT_29049
  bool IsKeyAvailable() const noexcept;

 private:
  /// @brief Variable to hold the block service instance.
  BlockServiceImpl* blockServiceInstance_;

  /// @brief Variable to hold the algorithmId for which this instance is created.
  CryptoAlgId algId_;

  /// @brief Variable to hold status if process Started.
  bool isProcessStarted_;

  /// @brief Variable to hold status if process Finished.
  bool isProcessFinished_;

  /// @brief Variable to hold the digest value.
  ara::core::Vector<uint8_t> digetsValue_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_;
};
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_DIGEST_SERVICE_IMPL_H_
