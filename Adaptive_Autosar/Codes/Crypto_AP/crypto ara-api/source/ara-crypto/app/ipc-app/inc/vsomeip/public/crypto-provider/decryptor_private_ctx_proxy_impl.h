// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : decryptor_private_ctx_proxy_impl.h
//
// Purpose     : Decryptor private context proxy implementation to forward functional calls from application
//               to ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_DECRYPTOR_PRIVATE_CTX_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_DECRYPTOR_PRIVATE_CTX_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/decryptor_private_ctx.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "decryptorprivatecontext_proxy.h"
#include "private_key_proxy_impl.h"
#include "public_key_proxy_impl.h"
#include "secret_seed_proxy_impl.h"
#include "signature_proxy_impl.h"
#include "symmetric_key_proxy_impl.h"
#include "vsomeip_interface.h"  
#include "cryptoservice_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
class DecryptorPrivateCtxProxyImpl final : public ara::crypto::cryp::DecryptorPrivateCtx
{
 public:
  /// @brief Constructor for the DecryptorPrivateCtxProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the authentication cipher.
  /// @param[in] decPrCryptoProviderHandler CryptoProviderInterfaceProxy Shared pointer for the
  /// authentication cipher.
  /// @param[in] cp Reference to the crypto provider instance.
  DecryptorPrivateCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
      decPrCryptoProviderHandler,
    CryptoProvider& cp) noexcept;

  /// @brief Destructor for the DecryptorPrivateCtxProxyImpl class.
  ~DecryptorPrivateCtxProxyImpl() noexcept
  {
    decPrCryptoProviderHandler_->StopCryptoProviderService(
      CryptoProviderServiceType::kDecryptorPrivateCtx, vsomeipDpcInstance_.serviceInstanceID_);
  }

  /// @brief Get CryptoService instance.
  /// @trace SWS_CRYPT_20802
  CryptoService::Uptr GetCryptoService() const noexcept override;

  /// @brief Set (deploy) a key to the decryptor private algorithm context.
  /// @param[in] key  the source key object.
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context.
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object.
  /// @trace SWS_CRYPT_20810
  ara::core::Result<void> SetKey(const PrivateKey& key) noexcept override;

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
  /// @param[in] in  the input data block
  /// @param[in] suppressPadding  if @c true then the method doesn't apply the padding, but the payload
  /// should fill the whole block of the plain data.
  /// @returns actual size of output data (it always <tt><= out.size()</tt>) or 0 if the input data block
  /// has incorrect content.
  /// @error SecurityErrorDomain::kIncorrectInputSize  if the mentioned above rules about the input size is
  /// violated.
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the @c out.size() is not enough to store the
  /// transformation result.
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value.
  /// @trace SWS_CRYPT_20812
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_20414
  ara::core::Result<void> Reset() noexcept override;

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

 private:
  /// @brief Method to initializing the server-client connection for Decryptor private cipher context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief VsomeipInterface instance for Decryptor private cipher context proxy.
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::cryp::decryptorprivatecontext::proxy::DecryptorPrivateContextProxy>
    vsomeipDpcInstance_;

  /// @brief crypto provider handler.
  std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    decPrCryptoProviderHandler_;

  /// @brief Decryptor private cipher context proxy handler.
  std::shared_ptr<ara::crypto::ipccom::cryp::decryptorprivatecontext::proxy::DecryptorPrivateContextProxy>
    decPrProxyHandler_;

  // Autosar crypto provider reference handler
  CryptoProvider& cpUptr_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_DECRYPTOR_PRIVATE_CTX_PROXY_IMPL_H_
