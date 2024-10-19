// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : hash_function_ctx_proxy_impl.h
//
// Purpose     : Hash function proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_HASH_FUNCTION_CTX_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_HASH_FUNCTION_CTX_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/common/security_error_domain.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "hashfunction_proxy.h"
#include "private_key_proxy_impl.h"
#include "public_key_proxy_impl.h"
#include "secret_seed_proxy_impl.h"
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
/// @brief Hash function interface.
/// @trace SWS_CRYPT_21100
class HashFunctionCtxProxyImpl final : public ara::crypto::cryp::HashFunctionCtx
{
 public:
  /// @brief Constructor for the HashFunctionCtxProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the Hash function.
  /// @param[in] hashCPHandler CryptoProviderInterfaceProxy Shared pointer for the Hash function.
  /// @param[in] cp Reference to the crypto provider instance.
  HashFunctionCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
      hashCPHandler,
    CryptoProvider& cp) noexcept;

  /// @brief Destructor for HashFunctionCtxProxyImpl class.
  ~HashFunctionCtxProxyImpl() noexcept
  {
    hashCryptoProviderHandler_->StopCryptoProviderService(
      CryptoProviderServiceType::kHashFunctionCtx, vsomeipHashInstance.serviceInstanceID_);
  }

  /// @brief Method to Get the crypto service instance id.
  /// @returns Crypto service instance id of HashFunctionCtxProxyImpl.
  CryptoServiceInstanceId GetHashFunctionCtxServiceInstanceId() const
  {
    return this->hashFunctionServiceInstanceID_;
  }

  /// @brief Get DigestService instance.
  /// @return DigestService::Uptr.
  /// @trace SWS_CRYPT_21102
  DigestService::Uptr GetDigestService() const noexcept override;  

  /// @brief Initialize the context for a new data stream processing or generation.
  /// (depending on the primitive). If IV size is greater than maximally supported by the algorithm then an
  /// implementation may use the leading bytes only from the sequence.
  /// @param[in] iv  an optional Initialization Vector (IV) or "nonce" value
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation, but provided IV value is not empty,
  /// i.e. if <tt>(iv.empty() == false)</tt>
  /// @trace SWS_CRYPT_21110
  ara::core::Result<void> Start(ReadOnlyMemRegion iv) noexcept override;

  /// @brief Initialize the context for a new data stream processing or generation (depending on the
  /// primitive) without IV.
  /// @error SecurityErrorDomain::kMissingArgument  the configured hash function expected an IV
  /// @trace SWS_CRYPT_21118
  ara::core::Result<void> Start() noexcept override;

  /// @brief Initialize the context for a new data stream processing or generation
  /// (depending on the primitive). If IV size is greater than maximally supported by the algorithm then an
  /// implementation may use the leading bytes only from the sequence.
  /// @param[in] iv  the Initialization Vector (IV) or "nonce" object
  /// @error SecurityErrorDomain::kInvalidInputSize  if the size of provided IV is not supported
  /// (i.e. if it is not enough for the initialization)
  /// @error SecurityErrorDomain::kUnsupported  if the base algorithm (or its current implementation)
  /// principally doesn't support the IV variation
  /// @trace SWS_CRYPT_21111
  ara::core::Result<void> Start(const SecretSeed& iv) noexcept override;

  /// @brief Update the digest calculation context by a new part of the message.
  /// This method is dedicated for cases then the @c RestrictedUseObject is a part of the "message".
  /// @param[in] in  a part of input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @trace SWS_CRYPT_21112
  ara::core::Result<void> Update(const RestrictedUseObject& in) noexcept override;

  /// @brief Update the digest calculation context by a new part of the message.
  /// @param[in] in  a part of the input message that should be processed
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @trace SWS_CRYPT_21113
  ara::core::Result<void> Update(ReadOnlyMemRegion in) noexcept override;

  /// @brief Update the digest calculation context by a new part of the message.
  /// This method is convenient for processing of constant tags.
  /// @param[in] in  a byte value that is a part of input message
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @trace SWS_CRYPT_21114
  ara::core::Result<void> Update(std::uint8_t in) noexcept override;

  /// @brief Finish the digest calculation and optionally produce the "signature" object.
  /// Only after call of this method the digest can be signed, verified, extracted or compared.
  /// @param[in] makeSignatureObject  if this argument is @c true then the method will also produce the
  /// signature object
  /// @returns unique smart pointer to created signature object, if <tt>(makeSignatureObject == true)</tt> or
  /// an empty Signature object if <tt>(makeSignatureObject == false)</tt>
  /// @error SecurityErrorDomain::kProcessingNotStarted  if the digest calculation was not initiated
  /// by a call of the @c Start() method
  /// @error SecurityErrorDomain::kInvalidUsageOrder  if the digest calculation has not started yet or not
  /// been updated at least once
  /// @trace SWS_CRYPT_21115
  ara::core::Result<ara::core::Vector<ara::core::Byte>> Finish() noexcept override;

  /// @brief Get requested part of calculated digest.
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
  /// @trace SWS_CRYPT_21116
  ara::core::Result<ara::core::Vector<ara::core::Byte>> GetDigest(std::size_t offset = 0) const
    noexcept override;

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
  /// @brief Method to initializing the server-client connection for HashFunctionProxy context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  // private data member
  /// @brief vsomeip instance for Hash function proxy.
  ara::crypto::vsomeip::VsomeipInterface<ara::crypto::ipccom::cryp::hashfunction::proxy::HashFunctionProxy>
    vsomeipHashInstance;

  /// @brief vsomeip instance for Crypto provider proxy.
  std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    hashCryptoProviderHandler_;

  /// @brief shared handle for Hash function proxy.
  std::shared_ptr<ara::crypto::ipccom::cryp::hashfunction::proxy::HashFunctionProxy> hashProxyHandler_;

  /// @brief Used to store the crypto service instance id.
  CryptoServiceInstanceId const hashFunctionServiceInstanceID_;

  // Autosar crypto provider reference handler
  CryptoProvider& cpUptr_;

  /// @brief Logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_HASH_FUNCTION_CTX_PROXY_IMPL_H_
