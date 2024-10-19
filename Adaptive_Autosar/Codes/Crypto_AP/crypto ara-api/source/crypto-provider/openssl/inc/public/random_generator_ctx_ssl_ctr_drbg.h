// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : random_generator_ctx_ssl_ctr_drbg.h
//
// Purpose     : This file contains definitions for RandomGeneratorCtxDefaultSSLCtrDrbg class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_RANDOM_GENERATOR_CTX_SSL_CTR_DRBG_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_RANDOM_GENERATOR_CTX_SSL_CTR_DRBG_H_

#include <ara/core/result.h>
#include <ara/log/logging.h>
#include <openssl/core_names.h>
#include <openssl/evp.h>

#include "algorithm_identification.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/random_generator_ctx.h"
#include "crypto_config.h"
#include "crypto_primitive_id_impl.h"
#include "symmetric_key_impl.h"
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
/// @brief Interface of Random Number Generator Context.
class RandomGeneratorCtxDefaultSSLCtrDrbg : public RandomGeneratorCtx
{
 public:

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_22902
  ExtensionService::Uptr GetExtensionService() const noexcept override;

  /// @brief Constructor for RandomGeneratorCtxDefaultSSLCtrDrbg class.
  /// @param[in] initialize flag to initialize local context.
  explicit RandomGeneratorCtxDefaultSSLCtrDrbg(bool initialize) noexcept;

  /// @brief Move constructor for RandomGeneratorCtxDefaultSSLCtrDrbg class.
  RandomGeneratorCtxDefaultSSLCtrDrbg(RandomGeneratorCtxDefaultSSLCtrDrbg&& otehr) noexcept = default;

  /// @brief Destructor for RandomGeneratorCtxDefaultSSLCtrDrbg class.
  ~RandomGeneratorCtxDefaultSSLCtrDrbg() noexcept override;

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use,
  /// and @c false otherwise.
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override
  {
    stAralog_.LogDebug() << "In RandomGeneratorCtxDefaultSSLCtrDrbg::IsInitialized()";
    return isCtxinitialize_;
  }

  /// @brief Set the internal state of the RNG using the provided seed
  /// @param[in] seed  a memory region with the seed value
  /// @returns @c true if the method is supported and the state has been set successfully
  /// @trace SWS_CRYPT_22911, SWS_CRYPT_00502
  bool Seed(ReadOnlyMemRegion seed) noexcept override;

  /// @brief Set the internal state of the RNG using the provided seed
  /// @param[in] seed  a memory region with the seed value
  /// @returns @c true if the method is supported and the state has been set successfully
  /// @trace SWS_CRYPT_22912, SWS_CRYPT_00502, SWS_CRYPT_00503
  bool Seed(const SecretSeed& seed) noexcept override;

  /// @brief Set the internal state of the RNG using the provided seed
  /// @param[in] key  a SymmetricKey with the key used as seed value
  /// @returns @c true if the method is supported and the key has been set successfully
  /// @trace SWS_CRYPT_22913, SWS_CRYPT_00502, SWS_CRYPT_00503
  bool SetKey(const SymmetricKey& key) noexcept override;

  /// @brief Update the internal state of the RNG by mixing it with the provided additional entropy.
  /// This method is optional for implementation.
  /// An implementation of this method may "accumulate" provided entropy for future use.
  /// @param[in] entropy  a memory region with the additional entropy value
  /// @returns @c true if the method is supported and the entropy has been updated successfully
  /// @trace SWS_CRYPT_22914, SWS_CRYPT_00502
  bool AddEntropy(ReadOnlyMemRegion entropy) override;

  /// @brief Return an allocated buffer with a generated random sequence of the requested size.
  /// @param[in] count number of random bytes to generate
  /// @returns @c a buffer filled with the generated random sequence
  /// @trace SWS_CRYPT_22915, SWS_CRYPT_00507, SWS_CRYPT_00508
  ara::core::Result<ara::core::Vector<ara::core::Byte>> Generate(std::uint32_t count) override;

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

  /// @brief Copy-assign another RandomGeneratorCtxDefaultSSLCtrDrbg to this instance.
  RandomGeneratorCtxDefaultSSLCtrDrbg& operator=(RandomGeneratorCtxDefaultSSLCtrDrbg& other) & noexcept
    = default;

  /// @brief Move-assign another RandomGeneratorCtxDefaultSSLCtrDrbg to this instance.
  RandomGeneratorCtxDefaultSSLCtrDrbg& operator=(RandomGeneratorCtxDefaultSSLCtrDrbg&& other) & noexcept
    = default;

 private:
  /// @brief Method to instantiate global context.
  static void IntstantiateGlobalCtx();

  /// @brief Flag to indicate random number generation is in process for global context.
  static bool stIsGenerationInProcess_;

  /// @brief Pointer for openssl random number generator context.
  EVP_RAND_CTX* randomNumberGeneratorCtx_{ nullptr };

  /// @brief Pointer for openssl random number generator context.
  static EVP_RAND_CTX* stGlobalRandomNumberGeneratorCtx_;

  /// @brief Flag to indicate that current context is local context.
  bool isLocalCtx_{ false };

  /// @brief Flag to indicate that this context is initialized.
  bool isCtxinitialize_{ false };

  /// @brief Buffer to store the entropy.
  ara::core::Vector<std::uint8_t> entropyValue_{};

  /// @brief Pointer to the crypto configuration.
  std::shared_ptr<ara::crypto::config::CryptoConfig> configInstance_{
    ara::crypto::config::CryptoConfig::GetCryptoConfigInstance()
  };

  /// @brief Variable to store the current key size.
  size_t keySize_;

  /// @brief Logger variable for ara::log.
  static ara::log::Logger& stAralog_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_RANDOM_GENERATOR_CTX_SSL_CTR_DRBG_H_
