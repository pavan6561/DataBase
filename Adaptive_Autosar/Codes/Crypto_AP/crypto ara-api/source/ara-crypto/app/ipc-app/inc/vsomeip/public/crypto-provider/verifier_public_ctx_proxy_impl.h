// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : verifier_public_ctx_proxy_impl.h
//
// Purpose     : Verifier public context proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_VERIFIER_PUBLIC_CTX_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_VERIFIER_PUBLIC_CTX_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/verifier_public_ctx.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "hash_function_ctx_proxy_impl.h"
#include "private_key_proxy_impl.h"
#include "public_key_proxy_impl.h"
#include "secret_seed_proxy_impl.h"
#include "signature_proxy_impl.h"
#include "symmetric_key_proxy_impl.h"
#include "verifierpubliccontext_proxy.h"
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
class VerifierPublicCtxProxyImpl final : public ara::crypto::cryp::VerifierPublicCtx
{
 public:
  /// @brief Constructor for the VerifierPublicCtxProxyImpl class.
  /// @param[in] instanceValue CryptoServiceInstance Id for the authentication cipher.
  /// @param[in] decPrCryptoProviderHandler CryptoProviderInterfaceProxy Shared pointer for the
  /// authentication cipher.
  /// @param[in] cp Reference to the crypto provider instance.
  VerifierPublicCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
      verPubCryptoProviderHandler,
    CryptoProvider& cp) noexcept;

  /// @brief Destructor for the VerifierPublicCtxProxyImpl class.
  ~VerifierPublicCtxProxyImpl() noexcept
  {
    verPubCryptoProviderHandler_->StopCryptoProviderService(
      CryptoProviderServiceType::kVerifierPublicCtx, vsomeipVpcInstance_.serviceInstanceID_);
  }

  /// @brief Verify signature by a digest value stored in the hash-function context. This is a pass-through
  /// interface to SWS_CRYPT_24113 for developer convenience, i.e. it adds additional input checks
  /// amd then calls the verify() interface from SWS_CRYPT_24113.
  /// @param[in] hashFn  hash function to be used for hashing
  /// @param[in] signature  the signature object for verification
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns @c true if the signature was verified successfully and @c false otherwise
  /// @error  SecurityErrorDomain::kProcessingNotFinished  if the method @c hashFn.Finish() was not called
  /// before this method call <br>
  /// @error  SecurityErrorDomain::kInvalidArgument  if the CryptoAlgId of @c hashFn differs from the
  /// CryptoAlgId of this context <br>
  /// @trace SWS_CRYPT_24111
  ara::core::Result<bool> VerifyPrehashed(const HashFunctionCtx& hashFn, const Signature& signature,
    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Verify signature BLOB by a directly provided hash or message value.
  /// This method can be used for implementation of the "multiple passes" signature algorithms that process
  /// a message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
  /// implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA).
  /// If the target algorithm doesn't support the @c context argument then the empty (default) value must be
  /// supplied! The user supplied @c context may be used for such algorithms as: Ed25519ctx, Ed25519ph
  /// , Ed448ph.
  /// @param[in] value  the (pre-)hashed or direct message value that should be verified
  /// @param[in] signature  the signature BLOB for the verification (the BLOB contains a plain sequence of
  /// the digital
  /// signature components located in fixed/maximum length fields defined by the algorithm specification,
  /// and each component is presented by a raw bytes sequence padded by zeroes to full length of the field;
  /// e.g. in case of (EC)DSA-256 (i.e. length of the q module is 256 bits) the signature BLOB must have two
  /// fixed-size fields: 32 + 32 bytes, for R and S components respectively, i.e. total BLOB size is 64 bytes)
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns @c true if the signature was verified successfully and @c false otherwise
  /// @error  SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key
  /// value <br>
  /// @error  SecurityErrorDomain::kInvalidInputSize  if the @c context argument has unsupported size <br>
  /// @trace SWS_CRYPT_24112
  ara::core::Result<bool> Verify(ReadOnlyMemRegion value, ReadOnlyMemRegion signature,
    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Verify signature by a digest value stored in the hash-function context. This is a pass-through
  /// interface to SWS_CRYPT_24112 for developer convenience, i.e. it adds additional input checks
  /// amd then calls the default verify() interface.
  /// @param[in] hashAlgId  hash function algorithm ID
  /// @param[in] hashValue  hash function value (resulting digest without any truncations)
  /// @param[in] signature  the signature object for the verification
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns @c true if the signature was verified successfully and @c false otherwise
  /// @error  SecurityErrorDomain::kIncompatibleObject if the CryptoAlgId of this context does not match the
  /// CryptoAlgId of @c signature; or the required CryptoAlgId of the hash is not kAlgIdDefault
  /// and the required hash CryptoAlgId of this context does not match @c hashAlgId or the hash
  /// CryptoAlgId of @c signature <br>
  /// @error  SecurityErrorDomain::kIncompatibleArguments if the provided hashAlgId is not kAlgIdDefault
  /// and the AlgId of the provided signature object does not match the provided hashAlgId
  /// @error  SecurityErrorDomain::kBadObjectReference  if the provided signature object does not
  /// reference the public key loaded to the context,
  /// i.e. if the COUID of the public key in the context is not equal to the COUID referenced from the
  /// signature object.
  /// @trace SWS_CRYPT_24113
  ara::core::Result<bool> VerifyPrehashed(CryptoAlgId hashAlgId, ReadOnlyMemRegion hashValue,
    const Signature& signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Verify signature by a digest value stored in the hash-function context. This is a pass-through
  /// interface to SWS_CRYPT_24112 for developer convenience, i.e. it adds additional input checks
  /// and then calls the default verify() interface.
  /// @param[in] hashFn hash function to be used for hashing
  /// @param[in] signature the data BLOB to be verified
  /// @param[in] context an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns @c true if the signature was verified successfully and @c false otherwise
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the method @c hashFn.Finish() was not called
  /// before this method call <br>
  /// @error SecurityErrorDomain::kInvalidArgument  if the CryptoAlgId of @c hashFn differs from the
  /// CryptoAlgId of this context <br>
  /// @trace SWS_CRYPT_24114
  ara::core::Result<bool> VerifyPrehashed(const HashFunctionCtx& hashFn, ReadOnlyMemRegion signature,
    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Set (deploy) a key to the verifier public algorithm context.
  /// @param[in] key  the source key object
  /// @error  SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context <br>
  /// @error  SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object <br>
  /// @trace SWS_CRYPT_24115
  ara::core::Result<void> SetKey(const PublicKey& key) noexcept override;

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

  /// @brief Extension service member class
  /// @trace SWS_CRYPT_24102
  SignatureService::Uptr GetSignatureService() const noexcept override;

 private:
  /// @brief Method to initializing the server-client connection for AuthCipherContextProxy context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);

  /// @brief vsome-ip instance for Authentication context proxy.
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::cryp::verifierpubliccontext::proxy::VerifierPublicContextProxy>
    vsomeipVpcInstance_;

  /// @brief crypto provider handler
  std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    verPubCryptoProviderHandler_;

  /// @brief authentication cipher context proxy handler
  std::shared_ptr<ara::crypto::ipccom::cryp::verifierpubliccontext::proxy::VerifierPublicContextProxy>
    verPubProxyHandler_;

  // Autosar crypto provider reference handler
  CryptoProvider& cpUptr_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_VERIFIER_PUBLIC_CTX_PROXY_IMPL_H_
