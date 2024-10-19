// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : msg_recovery_public_ctx_proxy_impl.h
//
// Purpose     : Message recovery public proxy implementation to forward functional calls from application
//               to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_MSG_RECOVERY_PUBLIC_CTX_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_MSG_RECOVERY_PUBLIC_CTX_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/msg_recovery_public_ctx.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "msgrecoverypubliccontext_proxy.h"
#include "private_key_proxy_impl.h"
#include "public_key_proxy_impl.h"
#include "secret_seed_proxy_impl.h"
#include "signature_proxy_impl.h"
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
class MsgRecoveryPublicCtxProxyImpl final : public ara::crypto::cryp::MsgRecoveryPublicCtx
{
 public:
  /// @brief Constructor for the MsgRecoveryPublicCtxProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the authentication cipher.
  /// @param[in] decPrCryptoProviderHandler CryptoProviderInterfaceProxy Shared pointer for the
  /// authentication cipher.
  /// @param[in] cp Reference to the crypto provider instance.
  MsgRecoveryPublicCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
      msgRecCryptoProviderHandler,
    CryptoProvider& cp) noexcept;

  /// @brief Destructor for the MsgRecoveryPublicCtxProxyImpl class.
  ~MsgRecoveryPublicCtxProxyImpl() noexcept
  {
    msgRecCryptoProviderHandler_->StopCryptoProviderService(
      CryptoProviderServiceType::kMsgRecoveryPublicCtx, vsomeipMrpcInstance_.serviceInstanceID_);
  }

  /// @brief Set (deploy) a key to the msg recovery public algorithm context.
  /// @param[in] key  the source key object
  /// @error  SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error  SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by
  /// the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_22211
  ara::core::Result<void> SetKey(const PublicKey& key) noexcept override;

  /// @brief Get maximum expected size of the input data block.
  /// If <tt>(IsEncryption() == false)</tt> then a value returned by this method is independent from
  /// the @c suppressPadding argument and it will be equal to the block size.
  /// @param[in] suppressPadding  if @c true then the method calculates the size for the case when the whole
  /// space of the plain data block is used for the payload only
  /// @returns maximum size of the input data block in bytes
  /// @trace SWS_CRYPT_22213
  std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept override;

  /// @brief Get maximum possible size of the output data block.
  /// If <tt>(IsEncryption() == true)</tt> then a value returned by this method is independent from the
  /// @c suppressPadding argument and will be equal to the block size.
  /// @param[in] suppressPadding  if @c true then the method calculates the size for the case when the whole
  /// space of the plain data block is used for the payload only.
  /// @returns maximum size of the output data block in bytes.
  /// @trace SWS_CRYPT_22214
  std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept override;

  /// @brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
  /// Encryption with <tt>(suppressPadding == true)</tt> expects that:
  /// <tt>in.size() == GetMaxInputSize(true) && out.size() >= GetMaxOutputSize(true)</tt>.
  /// Encryption with <tt>(suppressPadding == false)</tt> expects that:
  /// <tt>in.size() <= GetMaxInputSize(false) && in.size() > 0 && out.size()
  /// >= GetMaxOutputSize(false)</tt>.
  /// Decryption expects that:
  /// <tt>in.size() == GetMaxInputSize() && out.size() >= GetMaxOutputSize(suppressPadding)</tt>.
  /// The case <tt>(out.size() < GetMaxOutputSize())</tt> should be used with caution, only if you are
  /// strictly certain about the size of the output data!
  /// In case of <tt>(suppressPadding == true)</tt> the actual size of plain text should be equal to full
  /// size of the plain data block (defined by the algorithm)!
  /// @returns  the output buffer
  /// @param[in] in  the input data block
  /// @param[in] suppressPadding  if @c true then the method doesn't apply the padding, but the payload
  /// should fill the whole block of the plain data
  /// @returns actual size of output data (it always <tt><= out.size()</tt>) or 0 if the input data block
  /// has incorrect content
  /// @error  SecurityErrorDomain::kIncorrectInputSize  if the mentioned above rules about the input size is
  /// violated
  /// @error  SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_22215
  ara::core::Result<ara::core::Vector<ara::core::Byte>> DecodeAndVerify(ReadOnlyMemRegion in) const
    noexcept override;

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

  /// @brief Get ExtensionService instance.
  /// @trace SWS_CRYPT_22210
  ExtensionService::Uptr GetExtensionService() const noexcept override;  

 private:
  /// @brief Method to initializing the server-client connection for MsgRecoveryPublic Context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief vsome-ip instance for Msg Recovery Public Context proxy.
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::proxy::MsgRecoveryPublicContextProxy>
    vsomeipMrpcInstance_;

  /// @brief crypto provider handler
  std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    msgRecCryptoProviderHandler_;

  /// @brief MsgRecoveryPublic context proxy handler
  std::shared_ptr<ara::crypto::ipccom::cryp::msgrecoverypubliccontext::proxy::MsgRecoveryPublicContextProxy>
    msgRecProxyHandler_;

  // Autosar crypto provider reference handler
  CryptoProvider& cpUptr_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_MSG_RECOVERY_PUBLIC_CTX_PROXY_IMPL_H_
