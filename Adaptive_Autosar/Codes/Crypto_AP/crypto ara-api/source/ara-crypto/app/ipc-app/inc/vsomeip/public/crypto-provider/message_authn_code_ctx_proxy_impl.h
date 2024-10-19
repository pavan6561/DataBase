// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : message_authn_code_ctx_proxy_impl.h
//
// Purpose     : Message authentication code proxy implementation to forward functional calls from application
//               to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_MESSAGE_AUTHN_CODE_CTX_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_MESSAGE_AUTHN_CODE_CTX_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/message_authn_code_ctx.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "messageauthncode_proxy.h"
#include "secret_seed_proxy_impl.h"
#include "signature_proxy_impl.h"
#include "symmetric_key_proxy_impl.h"
#include "vsomeip_interface.h" 
#include "digestservice_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Keyed Message Authentication Code Context interface definition (MAC/HMAC).
/// @trace SWS_CRYPT_22100
class MessageAuthnCodeCtxProxyImpl final : public ara::crypto::cryp::MessageAuthnCodeCtx
{
 public:
  /// @brief Constructor for the MessageAuthnCodeCtxProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the Message authentication code.
  /// @param[in] macCPHandler CryptoProviderInterfaceProxy Shared pointer for the Message authentication code.
  /// @param[in] cp Reference to the crypto provider instance.
  MessageAuthnCodeCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
      macCPHandler,
    CryptoProvider& cp) noexcept;

  /// @brief Destructor for MessageAuthnCodeCtxProxyImpl class.
  ~MessageAuthnCodeCtxProxyImpl() noexcept
  {
    macCryptoProviderHandler_->StopCryptoProviderService(
      CryptoProviderServiceType::kMessageAuthCodeCtx, vsomeipMacInstance_.serviceInstanceID_);
  }

  /// @brief Initialize the context for a new data stream processing or generation (depending from the
  /// primitive).
  /// If IV size is greater than maximally supported by the algorithm then an implementation may use the
  /// leading bytes only from the sequence.
  /// @param[in] iv  an optional Initialization Vector (IV) or "nonce" value
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by deploying
  /// a key
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation, but provided IV value is not empty,
  /// i.e. if <tt>(iv.empty() == false)</tt>
  /// @trace SWS_CRYPT_22110_001, SWS_CRYPT_22110_002, SWS_CRYPT_22110_003, SWS_CRYPT_22110_004
  /// SWS_CRYPT_01201_001, SWS_CRYPT_01202
  ara::core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept override;

  /// @brief Initialize the context for a new data stream processing or generation (depending from the
  /// primitive).
  /// If IV size is greater than maximally supported by the algorithm then an implementation may use the
  /// leading bytes only from the sequence.
  /// @param[in] iv  the Initialization Vector (IV) or "nonce" object
  /// @error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by deploying
  /// a key
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation
  /// @error SecurityErrorDomain::kUsageViolation  if this transformation type is prohibited by
  /// the "allowed usage" restrictions of the provided @c SecretSeed object
  /// @trace SWS_CRYPT_22111_001, SWS_CRYPT_22111_002, SWS_CRYPT_22111_003, SWS_CRYPT_22111_004
  /// SWS_CRYPT_22111_005, SWS_CRYPT_01201_001, SWS_CRYPT_01202, SWS_CRYPT_01203
  ara::core::Result<void> Start(const SecretSeed& iv) noexcept override;

  /// @brief Update the digest calculation context by a new part of the message.
  /// This method is dedicated for cases then the @c RestrictedUseObject is a part of the "message".
  /// @param[in] in  a part of input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @trace SWS_CRYPT_22112_001, SWS_CRYPT_22112_002, SWS_CRYPT_01204, SWS_CRYPT_01205_001
  /// SWS_CRYPT_01205_002
  ara::core::Result<void> Update(const RestrictedUseObject& in) noexcept override;

  /// @brief Update the digest calculation context by a new part of the message.
  /// @param[in] in  a part of the input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @trace SWS_CRYPT_22113_001, SWS_CRYPT_22113_002, SWS_CRYPT_01204, SWS_CRYPT_01205_001
  /// SWS_CRYPT_01205_002
  ara::core::Result<void> Update(ReadOnlyMemRegion in) noexcept override;

  /// @brief Update the digest calculation context by a new part of the message.
  /// This method is convenient for processing of constant tags.
  /// @param[in] in  a byte value that is a part of input message
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @trace SWS_CRYPT_22114_001, SWS_CRYPT_22114_002, SWS_CRYPT_01204, SWS_CRYPT_01205_001
  /// SWS_CRYPT_01205_002
  ara::core::Result<void> Update(std::uint8_t in) noexcept override;

  /// @brief Finish the digest calculation and optionally produce the "signature" object.
  /// Only after call of this method the digest can be signed, verified, extracted or compared!
  /// If the signature object produced by a keyed MAC/HMAC/AE/AEAD algorithm then the dependence COUID of the
  /// "signature" should be set to COUID of used symmetric key.
  /// @param[in] makeSignatureObject  if this argument is @c true then the method will also produce the
  /// signature object
  /// @returns unique smart pointer to created signature object, if <tt>(makeSignatureObject == true)</tt> or
  /// @c nullptr if <tt>(makeSignatureObject == false)</tt>
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @error SecurityErrorDomain::kUsageViolation  if the buffered digest belongs to a MAC/HMAC/AE/AEAD
  /// context initialized by a key without @c kAllowSignature permission,
  /// but <tt>(makeSignatureObject == true)</tt>
  /// @trace SWS_CRYPT_22115_001, SWS_CRYPT_22115_002, SWS_CRYPT_22115_003, SWS_CRYPT_01207
  /// SWS_CRYPT_01208, SWS_CRYPT_01209
  ara::core::Result<Signature::Uptrc> Finish(bool makeSignatureObject = false) noexcept override;

  /// @brief Get requested part of calculated digest to existing memory buffer.
  /// Entire digest value is kept in the context up to next call @c Start(), therefore any its part can be
  /// extracted again or verified.
  /// If <tt>(full_digest_size <= offset)</tt> then <tt>return_size = 0</tt> bytes;
  /// else <tt>return_size = min(output.size(), (full_digest_size - offset))</tt> bytes.
  /// This method can be implemented as "inline" after standardization of function @c ara::core::memcpy().
  /// @param[out] output  an output buffer for storing the requested digest fragment (or fully)
  /// @param[in] offset  position of the first byte of digest that should be placed to the output buffer
  /// @returns number of digest bytes really stored to the output buffer (they are always <= @c output.size()
  /// and denoted below as @a return_size)
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the digest calculation was not finished
  /// by a call of the @c Finish() method
  /// @error SecurityErrorDomain::kUsageViolation  if the buffered digest belongs to a MAC/HMAC/AE/AEAD
  /// context initialized by a key without @c kAllowSignature permission
  /// @trace SWS_CRYPT_22116_001, SWS_CRYPT_22116_002, SWS_CRYPT_22116_003, SWS_CRYPT_01210_001,
  /// SWS_CRYPT_01210_002, SWS_CRYPT_01220
  ara::core::Result<ara::core::Vector<ara::core::Byte>> GetDigest(std::size_t offset = 0) const
    noexcept override;

  /// @brief Set (deploy) a key to the message authn code algorithm context.
  /// @param[in] key  the source key object
  /// @param[in] directTransform  the "direction" indicator: deploy the key for direct transformation (if @c
  /// true) or for reverse one (if @c false)
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// (taking into account the direction specified by @c transform) is prohibited by the "allowed usage"
  /// restrictions of provided key object.
  /// @trace SWS_CRYPT_22118_001, SWS_CRYPT_22118_002, SWS_CRYPT_22118_003, SWS_CRYPT_01211
  ara::core::Result<void> SetKey(
    const SymmetricKey& key, CryptoTransform transform = CryptoTransform::kMacGenerate) noexcept override;

  /// @brief Check the calculated digest against an expected "signature" object.
  /// Entire digest value is kept in the context up to next call @c Start(), therefore it can be
  /// verified again or extracted.
  /// This method can be implemented as "inline" after standardization of function @c ara::core::memcmp().
  /// @param[in] expected  the signature object containing an expected digest value
  /// @returns @c true if value and meta-information of the provided "signature" object is identical to
  /// calculated digest and current configuration of the context respectively; but @c false otherwise
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the digest calculation was not finished by
  /// a call of the @c Finish() method
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided "signature" object was produced by
  /// another crypto primitive type
  /// @trace SWS_CRYPT_22119_001, SWS_CRYPT_22119_002, SWS_CRYPT_22119_003, SWS_CRYPT_01213
  ara::core::Result<bool> Check(const Signature& expected) const noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_22120
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

  /// @brief Get DigestService instance.
  /// @trace SWS_CRYPT_22102
  DigestService::Uptr GetDigestService() const noexcept override;

 private:
  /// @brief Method to initializing the server-client connection for MessageAuthnCodeProxy context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief vsomeip instance for Message authentication code.
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::cryp::messageauthenticationcode::proxy::MessageAuthnCodeProxy>
    vsomeipMacInstance_;

  /// @brief crypto provider handler for Message authentication code.
  std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    macCryptoProviderHandler_;

  /// @brief key derivation proxy handler for Message authentication code.
  std::shared_ptr<ara::crypto::ipccom::cryp::messageauthenticationcode::proxy::MessageAuthnCodeProxy>
    macProxyHandler_;

  // Autosar crypto provider reference handler
  CryptoProvider& cpUptr_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_MESSAGE_AUTHN_CODE_CTX_PROXY_IMPL_H_
