// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name : decryptor_private_ctx_ssl_rsa.h
//
// Purpose : This file contains definitions for DecryptorPrivateCtxRSA class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_DECRYPTOR_PRIVATE_CTX_SSL_RSA_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_DECRYPTOR_PRIVATE_CTX_SSL_RSA_H_

#include <ara/log/logging.h>
#include <openssl/evp.h>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/decryptor_private_ctx.h"
#include "crypto_primitive_id_impl.h"
#include "private_key_impl.h"
#include "crypto_service_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
/// @brief Asymmetric Decryption Private key Context interface.
/// @trace SWS_CRYPT_20800
class DecryptorPrivateCtxRSA : public DecryptorPrivateCtx
{
 public:
  /// @brief Constructor for DecryptorPrivateCtxRSA class.
  DecryptorPrivateCtxRSA(CryptoPrimitiveId::AlgId algIdValue) noexcept;

  /// @brief Get CryptoService instance.
  /// @trace SWS_CRYPT_20802
  CryptoService::Uptr GetCryptoService() const noexcept override;

  /// @brief Set (deploy) a key to the decryptor private algorithm context.
  /// @param[in] key the source key object
  /// @error SecurityErrorDomain::kIncompatibleObject if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_20810
  ara::core::Result<void> SetKey(const PrivateKey& key) noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_20811
  ara::core::Result<void> Reset() noexcept override;

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
  /// @param[in] in the input data block
  /// @param[in] suppressPadding if @c true then the method doesn't apply the padding, but the payload
  /// should fill the whole block of the plain data
  /// @returns actual size of output data (it always <tt><= out.size()</tt>) or 0 if the input data block
  /// has incorrect content
  /// @error SecurityErrorDomain::kIncorrectInputSize if the mentioned above rules about the input size is
  /// violated
  /// @error SecurityErrorDomain::kInsufficientCapacity if the @c out.size() is not enough to store the
  /// transformation result
  /// @error SecurityErrorDomain::kUninitializedContext if the context was not initialized by a key value
  /// @trace SWS_CRYPT_20812
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept override;

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use,
  /// and @c false otherwise
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override
  {
    return isInitialize_;
  }

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

 private:
  /// @brief Configure the encryptor ctx of the Openssl.
  void ConfigureEncryptorCtx(EVP_PKEY_CTX* keyCtx) const noexcept;

  /// @brief pointer to the openssl private key.
  EVP_PKEY* privateKey_{ nullptr };

  /// @brief shared pointer to the algorithm identification class.
  std::shared_ptr<AlgorithmIdentification> algoIdentification_;

  /// @brief algorithm Id for this context.
  CryptoPrimitiveId::AlgId algId_;

  /// @brief flag to indicate whether ctx is initialized or not.
  bool isInitialize_{ false };

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "DPC", "Decryptor private context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_DECRYPTOR_PRIVATE_CTX_SSL_RSA_H_
