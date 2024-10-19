// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_derivation_function_ctx_impl.h
//
// Purpose     : Key derivation function proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_KEY_DERIVATION_FUNCTION_CTX_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_KEY_DERIVATION_FUNCTION_CTX_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "keyderivationfunction_proxy.h"
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
/// @brief %Key Derivation Function interface.
/// @trace RS_CRYPTO_02103
class KeyDerivationFunctionCtxProxyImpl final : public ara::crypto::cryp::KeyDerivationFunctionCtx
{
 public:
  /// @brief Constructor for the KeyDerivationFunctionCtxProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the Key derivation function.
  /// @param[in] kdfCPHandler CryptoProviderInterfaceProxy Shared pointer for the key derivation function.
  /// @param[in] cp Reference to the crypto provider instance.
  KeyDerivationFunctionCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
      kdfCPHandler,
    CryptoProvider& cp) noexcept;

  /// @brief Destructor for KeyDerivationFunctionCtxProxyImpl class.
  ~KeyDerivationFunctionCtxProxyImpl() noexcept
  {
    kdfCryptoProviderHandler_->StopCryptoProviderService(
      CryptoProviderServiceType::kKeyDerivationFunctionCtx, vsomeipKdfInstance_.serviceInstanceID_);
  }

  /// @brief Add a salt value stored in a (non-secret) ReadOnlyMemRegion.
  /// @param[in] salt  a salt value (if used, it should be unique for each instance of the target key)
  /// @trace SWS_CRYPT_21510_001, SWS_CRYPT_00608
  ara::core::Result<void> AddSalt(ReadOnlyMemRegion salt) noexcept override;

  /// @brief Add a salt value stored in a (non-secret) ReadOnlyMemRegion.
  /// @param[in] salt  a salt value (if used, it should be unique for each instance of the target key)
  /// @trace SWS_CRYPT_21513, SWS_CRYPT_00608, SWS_CRYPT_00609_002
  ara::core::Result<void> AddSecretSalt(const SecretSeed& salt) noexcept override;

  /// @brief Configure the number of iterations that will be applied by default.
  /// Implementation can restrict minimal and/or maximal value of the iterations number.
  /// @param[in] iterations  the required number of iterations of the base function (0 means implementation
  /// default number)
  /// @returns actual number of the iterations configured in the context now (after this method call)
  /// @trace SWS_CRYPT_21514_001, SWS_CRYPT_21514_002
  std::uint32_t ConfigIterations(std::uint32_t iterations = 0) noexcept override;

  /// @brief Derive a symmetric key from the provided key material and provided context configuration.
  /// @param[in] isSession  the "session" (or "temporary") attribute for the target key (if @c true)
  /// @param[in] isExportable  the exportability attribute for the target key (if @c true)
  /// @returns unique smart pointer to the created instance of derived symmetric key
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not sufficiently initialized
  /// @trace SWS_CRYPT_21515_001, SWS_CRYPT_21515_002, SWS_CRYPT_00611_002
  ara::core::Result<SymmetricKey::Uptrc> DeriveKey(bool isSession, bool isExportable) const noexcept override;

  /// @brief Derive a "slave" key material (secret seed) from the provided "master" key material and
  /// provided context configuration.
  /// @param[in] isSession  the "session" (or "temporary") attribute for the target key (if @c true)
  /// @param[in] isExportable  the exportability attribute for the target key (if @c true)
  /// @returns unique smart pointer to the created @c SecretSeed object
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not sufficiently initialized
  /// @trace SWS_CRYPT_21516
  ara::core::Result<SecretSeed::Uptrc> DeriveSeed(bool isSession, bool isExportable) const noexcept override;

  /// @brief Get the fixed size of the target key ID required by diversification algorithm.
  /// Returned value is constant for each instance of the interface, i.e. independent from configuration by
  /// the @c Init() call.
  /// @returns size of the key ID in bytes
  /// @trace SWS_CRYPT_21519
  std::size_t GetKeyIdSize() const noexcept override;

  /// @brief Get the symmetric algorithm ID of target (slave) key.
  /// If the context was not configured yet by a call of the @c Init() method then @c kAlgIdUndefined should
  /// be returned.
  /// @returns the symmetric algorithm ID of the target key, configured by the last call of the @c Init()
  /// method.
  /// @trace SWS_CRYPT_21520_001, SWS_CRYPT_21520_002
  CryptoAlgId GetTargetAlgId() const noexcept override;

  /// @brief Get allowed key usage of target (slave) key.
  /// The returned value depends on the source key-material allowed usage flags and the argument
  /// @c allowedUsage of last call of the @c Init() method.
  /// If the context has not yet been configured by a call of the @c Init() method, the allowed usage flags
  /// of the source key-material shall be returned.
  /// If the context has not yet been configured by a call of the @c Init() method and no source key-material
  /// has been set either, kAllowKdfMaterialAnyUsage shall be returned.
  /// @returns allowed key usage bit-flags of target keys
  /// @trace SWS_CRYPT_21521_001, SWS_CRYPT_21521_002, SWS_CRYPT_21521_003
  AllowedUsageFlags GetTargetAllowedUsage() const noexcept override;

  /// @brief Get the bit-length of target (diversified) keys.
  /// Returned value is configured by the context factory method, i.e. independent from configuration by
  /// the @c Init() calls.
  /// @returns the length of target (diversified) key in bits
  /// @trace SWS_CRYPT_21522
  std::size_t GetTargetKeyBitLength() const noexcept override;

  /// @brief Initialize this context by setting at least the target key ID.
  /// The byte sequence provided via argument @c ctxLabel can include a few fields with different meaning
  /// separated by single @c 0x00 byte.
  /// If <tt>(targetAlgId == kAlgIdAny)</tt> then a diversified key can be loaded to any symmetric context
  /// that
  /// supports the same key length (if the "allowed usage" flags are also satisfied)!
  /// @param[in] targetKeyId  ID of the target key
  /// @param[in] targetAlgId  the identifier of the target symmetric crypto algorithm
  /// @param[in] allowedUsage  bit-flags that define a list of allowed transformations' types in which the
  /// target key may be used
  /// @param[in] ctxLabel  an optional application specific "context label" (this can identify the purpose of
  /// the target key and/or communication parties)
  /// @error SecurityErrorDomain::kIncompatibleArguments  if @c targetAlgId specifies a cryptographic
  /// algorithm different from a symmetric one with key length equal to @c GetTargetKeyBitLength();
  /// @error SecurityErrorDomain::kUsageViolation  if @c allowedUsage specifies more usages of the derived
  /// key-material than the source key-material, i.e. usage of the derived key-material may not be
  /// expanded beyond what the source key-material allows
  /// @trace SWS_CRYPT_21523_002, SWS_CRYPT_21523_003, SWS_CRYPT_21523_004
  ara::core::Result<void> Init(ReadOnlyMemRegion targetKeyId, CryptoAlgId targetAlgId = kAlgIdAny,
    AllowedUsageFlags allowedUsage = kAllowKdfMaterialAnyUsage,
    ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_21524
  ara::core::Result<void> Reset() noexcept override;

  /// @brief Set (deploy) key-material to the key derivation algorithm context.
  /// @param[in] sourceKM  the source key-material
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if deriving a key is prohibited by
  /// the "allowed usage" restrictions of the provided source key-material
  /// @error SecurityErrorDomain::kBruteForceRisk  if key length of the @c sourceKm is below of
  /// an internally defined limitation
  /// @trace SWS_CRYPT_21525
  ara::core::Result<void> SetSourceKeyMaterial(const RestrictedUseObject& sourceKM) noexcept override;

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
  /// @trace SWS_CRYPT_21517
  ExtensionService::Uptr GetExtensionService() const noexcept override;  

  /// @brief Method to Get the crypto service instance id.
  /// @returns Crypto service instance id of KeyDerivationFunctionCtxProxyImpl.
  CryptoServiceInstanceId GetKDFCtxServiceInstanceId() const
  {
    return this->kdfServiceInstanceID_;
  }

 private:
  /// @brief Method to initializing the server-client connection for KeyDerivationFunctionProxy context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief vsomeip instance for key derivation function.
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::cryp::keyderivationfunction::proxy::KeyDerivationFunctionProxy>
    vsomeipKdfInstance_;

  /// @brief crypto provider handler for key derivation function.
  std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    kdfCryptoProviderHandler_;

  /// @brief key derivation proxy handler for key derivation function.
  std::shared_ptr<ara::crypto::ipccom::cryp::keyderivationfunction::proxy::KeyDerivationFunctionProxy>
    kdfProxyHandler_;

  /// @brief Used to store the crypto service instance id.
  CryptoServiceInstanceId const kdfServiceInstanceID_;

  // Autosar crypto provider reference handler
  CryptoProvider& cpUptr_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_KEY_DERIVATION_FUNCTION_CTX_PROXY_IMPL_H_
