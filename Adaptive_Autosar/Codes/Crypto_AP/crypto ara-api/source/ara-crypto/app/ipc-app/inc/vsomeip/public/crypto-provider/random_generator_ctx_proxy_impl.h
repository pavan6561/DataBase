// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : random_generator_ctx_proxy_impl.h
//
// Purpose     : Random number generator proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_RANDOM_GENERATOR_CTX_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_RANDOM_GENERATOR_CTX_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/common/security_error_domain.h"
#include "ara/crypto/cryp/random_generator_ctx.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "impl_type_cryptoproviderservicetype.h"
#include "randomgenerator_proxy.h"
#include "secret_seed_proxy_impl.h"
#include "symmetric_key_proxy_impl.h"
#include "vsomeip_interface.h" 
#include "extensionservice_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Class for Random Number Generator proxy implementation.
class RandomGeneratorCtxProxyImpl final : public ara::crypto::cryp::RandomGeneratorCtx
{
 public:
  /// @brief Constructor for the RandomGeneratorCtxProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the Random generator.
  /// @param[in] rngCPHandler CryptoProviderInterfaceProxy Shared pointer for the Random generator.
  /// @param[in] cp Reference to the crypto provider instance.
  RandomGeneratorCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
      rngCPHandler,
    CryptoProvider& cp) noexcept;

  /// @brief Destructor for RandomGeneratorCtxProxyImpl.
  ~RandomGeneratorCtxProxyImpl() noexcept
  {
    rngCryptoProviderHandler_->StopCryptoProviderService(
      CryptoProviderServiceType::kRandomGeneratorCtx, vsomeipRngInstance.serviceInstanceID_);
  }

  /// @brief Return an allocated buffer with a generated random sequence of the requested size.
  /// @param[in] count number of random bytes to generate
  /// @returns @c a buffer filled with the generated random sequence
  /// @trace SWS_CRYPT_22915
  ara::core::Result<ara::core::Vector<ara::core::Byte>> Generate(std::uint32_t count) noexcept override;

  /// @brief Set the internal state of the RNG using the provided seed
  /// @param[in] seed  a memory region with the seed value
  /// @returns @c true if the method is supported and the state has been set successfully
  /// @trace SWS_CRYPT_22911
  bool Seed(ReadOnlyMemRegion seed) noexcept override;

  /// @brief Set the internal state of the RNG using the provided seed
  /// @param[in] seed  a memory region with the seed value
  /// @returns @c true if the method is supported and the state has been set successfully
  /// @trace SWS_CRYPT_22912
  bool Seed(const SecretSeed& seed) noexcept override;

  /// @brief Set the internal state of the RNG using the provided seed
  /// @param[in] key  a SymmetricKey with the key used as seed value
  /// @returns @c true if the method is supported and the key has been set successfully
  /// @trace SWS_CRYPT_22913
  bool SetKey(const SymmetricKey& key) noexcept override;

  /// @brief Update the internal state of the RNG by mixing it with the provided additional entropy.
  /// This method is optional for implementation.
  /// An implementation of this method may "accumulate" provided entropy for future use.
  /// @param[in] entropy  a memory region with the additional entropy value
  /// @returns @c true if the method is supported and the entropy has been updated successfully
  /// @trace SWS_CRYPT_22914
  bool AddEntropy(ReadOnlyMemRegion entropy) noexcept override;

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use, and @c false
  /// otherwise.
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override;

  /// @brief Get a reference to Crypto Provider of this context.
  /// @returns a reference to Crypto Provider instance that provides this context
  /// @trace SWS_CRYPT_20654
  CryptoProvider& MyProvider() const noexcept override;

  /// @brief Get ExtensionService instance.
	/// @returns ExtensionService::Uptr.
	/// @trace SWS_CRYPT_22902
  ExtensionService::Uptr GetExtensionService() const noexcept override;  

 private:
  /// @brief Method to initializing the server-client connection for RandomGeneratorProxy context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief vsomeip instance for RNG proxy.
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::cryp::randomgenerator::proxy::RandomGeneratorProxy>
    vsomeipRngInstance;

  /// @brief shared handler for Hash function proxy.
  std::shared_ptr<ara::crypto::ipccom::cryp::randomgenerator::proxy::RandomGeneratorProxy> rngProxyHandler_;

  /// @brief crypto provider handler
  std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    rngCryptoProviderHandler_;

  // Autosar crypto provider reference handler
  CryptoProvider& cpUptr_;

  /// @brief Logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_RANDOM_GENERATOR_CTX_PROXY_IMPL_H_
