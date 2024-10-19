// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signer_private_ctx_proxy_impl.h
//
// Purpose     : Signer private context proxy implementation to forward functional calls from application to
//               ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_SIGNER_PRIVATE_CTX_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_SIGNER_PRIVATE_CTX_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/signer_private_ctx.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "hash_function_ctx_proxy_impl.h"
#include "private_key_proxy_impl.h"
#include "public_key_proxy_impl.h"
#include "secret_seed_proxy_impl.h"
#include "signature_proxy_impl.h"
#include "signerprivatecontext_proxy.h"
#include "symmetric_key_proxy_impl.h"
#include "vsomeip_interface.h" 
#include "signatureservice_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
class SignerPrivateCtxProxyImpl final : public ara::crypto::cryp::SignerPrivateCtx
{
 public:
  /// @brief Constructor for the SignerPrivateCtxProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the authentication cipher.
  /// @param[in] decPrCryptoProviderHandler CryptoProviderInterfaceProxy Shared pointer for the
  /// authentication cipher.
  /// @param[in] cp Reference to the crypto provider instance.
  SignerPrivateCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
      signPrCryptoProviderHandler,
    CryptoProvider& cp) noexcept;

  /// @brief Destructor for the SignerPrivateCtxProxyImpl class.
  ~SignerPrivateCtxProxyImpl() noexcept
  {
    signPrCryptoProviderHandler_->StopCryptoProviderService(
      CryptoProviderServiceType::kSignerPrivateCtx, vsomeipSnprInstance_.serviceInstanceID_);
  }

  /// @brief Sign a provided digest value stored in the hash-function context.
  /// This method must put the hash-function algorithm ID and a @a COUID of the used key-pair to the
  /// resulting signature object!
  /// The user supplied @c context may be used for such algorithms as: Ed25519ctx, Ed25519ph, Ed448ph.
  /// If the target algorithm doesn't support the @c context argument then the empty (default) value must be
  /// supplied!
  /// @param[in] hashFn  a finalized hash-function context that contains a digest value ready for sign
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns unique smart pointer to serialized signature
  /// @error SecurityErrorDomain::kInvalidArgument  if hash-function algorithm does not comply with
  /// the signature algorithm specification of this context
  /// @error SecurityErrorDomain::kInvalidInputSize  if the user supplied @c context has incorrect
  /// (or unsupported) size
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the method @c hash.Finish() was not called
  /// before the call of this method
  /// @error SecurityErrorDomain::kUninitializedContext  this context was not initialized by a key value
  /// @trace SWS_CRYPT_23511
  ara::core::Result<Signature::Uptrc> SignPreHashed(
    const HashFunctionCtx& hashFn, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Sign a directly provided hash or message value.
  /// This method can be used for implementation of the "multiple passes" signature algorithms that process
  /// a message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
  /// implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA).
  /// If the target algorithm doesn't support the @c context argument then the empty (default) value must be
  /// supplied!
  /// @param[in] value  the (pre-)hashed or direct message value that should be signed
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns actual size of the signature value stored to the output buffer
  /// @error SecurityErrorDomain::kInvalidInputSize  if size of the input @c value or @c context arguments
  /// are incorrect / unsupported
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
  /// @trace SWS_CRYPT_23512
  ara::core::Result<ara::core::Vector<ara::core::Byte>> Sign(
    ReadOnlyMemRegion value, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Sign a directly provided digest value and create the @c Signature object.
  /// This method must put the hash-function algorithm ID and a @a COUID of the used key-pair to the
  /// resulting signature object!
  /// The user supplied @c context may be used for such algorithms as: Ed25519ctx, Ed25519ph, Ed448ph.
  /// If the target algorithm doesn't support the @c context argument then the empty (default) value must be
  /// supplied!
  /// @param[in] hashAlgId  hash function algorithm ID
  /// @param[in] hashValue  hash function value (resulting digest without any truncations)
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns unique smart pointer to serialized signature.
  /// @error SecurityErrorDomain::kInvalidArgument  if hash-function algorithm does not comply with
  /// the signature algorithm specification of this context.
  /// @error SecurityErrorDomain::kInvalidInputSize  if the user supplied @c context has incorrect
  /// (or unsupported) size.
  /// @error SecurityErrorDomain::kUninitializedContext  this context was not initialized by a key value.
  /// @trace SWS_CRYPT_23513
  ara::core::Result<Signature::Uptrc> SignPreHashed(AlgId hashAlgId, ReadOnlyMemRegion hashValue,
    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Set (deploy) a key to the signer private algorithm context.
  /// @param[in] key  the source key object.
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context.
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object.
  /// @trace SWS_CRYPT_23515
  ara::core::Result<void> SetKey(const PrivateKey& key) noexcept override;

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

  /// @brief Get SignatureService instance.
  /// @trace SWS_CRYPT_23510
  SignatureService::Uptr GetSignatureService() const noexcept override;  

 private:
  /// @brief Method to initializing the server-client connection for Signer private context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief vsome-ip instance for Signer private context proxy.
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::cryp::signerprivatecontext::proxy::SignerPrivateContextProxy>
    vsomeipSnprInstance_;

  /// @brief crypto provider handler.
  std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    signPrCryptoProviderHandler_;

  /// @brief Signer private context proxy handler.
  std::shared_ptr<ara::crypto::ipccom::cryp::signerprivatecontext::proxy::SignerPrivateContextProxy>
    signPrProxyHandler_;

  // Autosar crypto provider reference handler
  CryptoProvider& cpUptr_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_SIGNER_PRIVATE_CTX_PROXY_IMPL_H_
