// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : hash_function_ctx_default_ssl_sha256.h
//
// Purpose     : This file contains definitions for HashFunctionCtxDefaultSSSLSha256 class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPESNSSL_INC_PUBLIC_HASH_FUNCTION_CTX_DEFAULT_SSL_SHA256_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPESNSSL_INC_PUBLIC_HASH_FUNCTION_CTX_DEFAULT_SSL_SHA256_H_

#include <ara/log/logging.h>
#include <openssl/evp.h>

#include "algorithm_identification.h"
#include "ara/crypto/common/security_error_domain.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "crypto_primitive_id_impl.h"
#include "digest_service_impl.h"

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
/// @trace SWS_CRYPT_00902
class HashFunctionCtxDefaultSSSLSha256 : public HashFunctionCtx
{
 public:
  /// @brief Constructor for HashFunctionCtxDefaultSSSLSha256 class.
  HashFunctionCtxDefaultSSSLSha256() noexcept;

  /// @brief Destructor for HashFunctionCtxDefaultSSSLSha256 class.
  ~HashFunctionCtxDefaultSSSLSha256() noexcept override;

  /// @brief Get DigestService instance.
  /// @trace SWS_CRYPT_21102
  DigestService::Uptr GetDigestService() const noexcept override;

  /// @brief Initialize the context for a new data stream processing or generation
  /// (depending on the primitive) . If IV size is greater than maximally supported
  /// by the algorithm then an implementation may use the leading bytes only from the sequence.
  /// @param[in] iv  an optional Initialization Vector (IV) or "nonce" value
  /// @trace SWS_CRYPT_21110, SWS_CRYPT_00908
  ara::core::Result<void> Start(ReadOnlyMemRegion iv) noexcept override;

  /// @brief Initialize the context for a new data stream processing or generation (depending on the
  /// primitive) without IV.
  /// @trace SWS_CRYPT_21118, SWS_CRYPT_00908
  ara::core::Result<void> Start() noexcept override;

  /// @brief Initialize the context for a new data stream processing or generation
  /// (depending on the primitive) . If IV size is greater than maximally supported
  /// by the algorithm then an implementation may use the leading bytes only from the sequence.
  /// @param[in] iv  the Initialization Vector (IV) or "nonce" object
  /// @trace SWS_CRYPT_21111, SWS_CRYPT_00908
  ara::core::Result<void> Start(const SecretSeed& iv) noexcept override;

  /// @brief Update the digest calculation context by a new part of the message.
  /// This method is dedicated for cases then the @c RestrictedUseObject is a part of the "message".
  /// @param[in] in  a part of input message that should be processed
  /// @trace SWS_CRYPT_21112, SWS_CRYPT_00905, SWS_CRYPT_00909
  ara::core::Result<void> Update(const RestrictedUseObject& in) noexcept override;

  /// @brief Update the digest calculation context by a new part of the message.
  /// @param[in] in  a part of the input message that should be processed
  /// @trace SWS_CRYPT_21113, SWS_CRYPT_00905, SWS_CRYPT_00909
  ara::core::Result<void> Update(ReadOnlyMemRegion in) noexcept override;

  /// @brief Update the digest calculation context by a new part of the message.
  /// This method is convenient for processing of constant tags.
  /// @param[in] in  a byte value that is a part of input message
  /// @trace SWS_CRYPT_21114, SWS_CRYPT_00905, SWS_CRYPT_00909
  ara::core::Result<void> Update(std::uint8_t in) noexcept override;

  /// @brief Finish the digest calculation and optionally produce the "signature" object.
  /// Only after call of this method the digest can be signed, verified, extracted or compared.
  /// @param[in] makeSignatureObject  if this argument is @c true then the method will also produce the
  /// signature object
  /// @returns unique smart pointer to created signature object, if <tt>(makeSignatureObject == true)</tt> or
  /// an empty Signature object if <tt>(makeSignatureObject == false)</tt>
  /// @trace SWS_CRYPT_21115, SWS_CRYPT_00903, SWS_CRYPT_00906, SWS_CRYPT_00910,
  ara::core::Result<ara::core::Vector<ara::core::Byte>> Finish() noexcept override;

  /// @brief Get requested part of calculated digest.
  /// Entire digest value is kept in the context up to next call @c Start(), therefore any its part can
  /// be extracted again or verified.
  /// If <tt>(full_digest_size <= offset)</tt> then <tt>return_size = 0</tt> bytes;
  /// else <tt>return_size = min(output.size(), (full_digest_size - offset))</tt> bytes.
  /// This method can be implemented as "inline" after standardization of function @c ara::core::memcpy().
  /// @param[out] output  an output buffer for storing the requested digest fragment (or fully)
  /// @param[in] offset  position of the first byte of digest that should be placed to the output buffer
  /// @returns number of digest bytes really stored to the output buffer (they are always <= @c output.size()
  /// and denoted below as @a return_size)
  /// @trace SWS_CRYPT_21116, SWS_CRYPT_00907, SWS_CRYPT_00919
  ara::core::Result<ara::core::Vector<ara::core::Byte>> GetDigest(std::size_t offset = 0) const
    noexcept override;

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use, and @c false
  /// otherwise
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override
  {
    aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256::IsInitialized()";
    return isCtxInitialized_;
  }

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

 private:
  /// @brief enum class to define the execution states for hash function ctx.
  enum class HashExecutionStates : std::uint8_t
  {
    kHashIdle = 0,
    kHashStart = 1,
    kHashUpdate = 2,
    kHashFinal = 3
  };

  /// @brief pointer to the openssl Hash context.
  EVP_MD_CTX* hashSha256Ctx_{ nullptr };

  /// @brief buffer to store the digest value.
  ara::core::Vector<uint8_t> digestValue_{};

  /// @brief variable to store the length of the calculated digest value.
  uint32_t digestValueLength_{ 0 };

  /// @brief variable to store the execution state of Hash function ctx.
  HashExecutionStates hashingState_{ HashExecutionStates::kHashIdle };

  bool isCtxInitialized_{ false };

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "SHAC", "SHA256 Hash context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_HASH_FUNCTION_CTX_DEFAULT_SSL_SHA256_H_
