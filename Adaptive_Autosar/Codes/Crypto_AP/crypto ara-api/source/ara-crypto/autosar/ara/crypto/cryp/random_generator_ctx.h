// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : random_generator_ctx.h
//
// Purpose     : This file contains definitions for RandomGeneratorCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_RANDOM_GENERATOR_CTX_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_RANDOM_GENERATOR_CTX_H_

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"
#include "ara/core/result.h"

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
/// @traceid{SWS_CRYPT_22900}@tracestatus{draft}
class RandomGeneratorCtx : public CryptoContext
{
 public:
  /// @brief Shared smart pointer of the interface.
  /// @trace SWS_CRYPT_22901
  using Uptr = std::unique_ptr<RandomGeneratorCtx>;

  /// @brief Get ExtensionService instance.
	/// @returns ExtensionService::Uptr.
	/// @trace SWS_CRYPT_22902
  virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

  /// @brief Set the internal state of the RNG using the provided seed
  /// @param[in] seed  a memory region with the seed value
  /// @returns true if the method is supported and the state has been set successfully
  /// @trace SWS_CRYPT_22911
  virtual bool Seed(ReadOnlyMemRegion seed) noexcept = 0;

  /// @brief Set the internal state of the RNG using the provided seed
  /// @param[in] seed  a memory region with the seed value
  /// @returns true if the method is supported and the state has been set successfully
  /// @trace SWS_CRYPT_22912
  virtual bool Seed(const SecretSeed& seed) noexcept = 0;

  /// @brief Set the internal state of the RNG using the provided seed
  /// @param[in] key  a SymmetricKey with the key used as seed value
  /// @returns @c true if the method is supported and the key has been set successfully
  /// @trace SWS_CRYPT_22913
  virtual bool SetKey(const SymmetricKey& key) noexcept = 0;

  /// @brief Update the internal state of the RNG by mixing it with the provided additional entropy.
  /// This method is optional for implementation.
  /// An implementation of this method may "accumulate" provided entropy for future use.
  /// @param[in] entropy  a memory region with the additional entropy value
  /// @returns true if the method is supported and the entropy has been updated successfully
  /// @trace SWS_CRYPT_22914

  virtual bool AddEntropy(ReadOnlyMemRegion entropy) = 0;

  /// @brief Return an allocated buffer with a generated random sequence of the requested size.
  /// @param[in] count number of random bytes to generate
  /// @returns a buffer filled with the generated random sequence
  /// @error SecurityErrorDomain::kUninitializedContext  if this context implements a local RNG
  /// (i.e., the RNG state is controlled by the application), and has to be seeded by the
  /// application because it either has not already been seeded or ran out of entropy.
  /// @error SecurityErrorDomain::kBusyResource  if this context implements a global RNG (i.e., the RNG
  /// state is controlled by the stack and not the application) that is currently out-of-entropy
  /// and therefore cannot provide the requested number of random bytes
  /// @trace SWS_CRYPT_22915
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> Generate(std::uint32_t count) = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_RANDOM_GENERATOR_CTX_H_
