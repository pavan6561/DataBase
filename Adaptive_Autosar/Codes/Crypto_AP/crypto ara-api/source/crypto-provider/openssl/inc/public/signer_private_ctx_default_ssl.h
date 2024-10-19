// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signer_private_ctx_default_ssl.h
//
// Purpose   : This file contains definitions for SigEncodePrivateCtxDefaultSsl class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_SIGNER_PRIVATE_CTX_DEFAULT_SSL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_SIGNER_PRIVATE_CTX_DEFAULT_SSL_H_

#include <ara/log/logging.h>
#include <openssl/evp.h>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/signer_private_ctx.h"
#include "signature_impl.h"
#include "signature_service_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
/// @brief Signature Private key Context interface.
/// @trace SWS_CRYPT_23500
class SignerPrivateCtxDefaultssl : public SignerPrivateCtx
{
 public:
  /// @brief Constructor for SignerPrivateCtxDefaultssl class.
  SignerPrivateCtxDefaultssl(AlgId algo) noexcept : algId_{ algo }
  {
  }

  /// @brief Extension service member class
  /// @trace SWS_CRYPT_23510
  SignatureService::Uptr GetSignatureService() const noexcept override;

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
  /// @returns unique smart pointer to serialized signature
  /// @error SecurityErrorDomain::kInvalidArgument  if hash-function algorithm does not comply with
  /// the signature algorithm specification of this context
  /// @error SecurityErrorDomain::kInvalidInputSize  if the user supplied @c context has incorrect
  /// (or unsupported) size
  /// @error SecurityErrorDomain::kUninitializedContext  this context was not initialized by a key value
  /// @trace SWS_CRYPT_23513
  ara::core::Result<Signature::Uptrc> SignPreHashed(AlgId hashAlgId, ReadOnlyMemRegion hashValue,
    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

  /// @brief Set (deploy) a key to the signer private algorithm context.
  /// @param[in] key  the source key object
  /// @error SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context
  /// @error SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object
  /// @trace SWS_CRYPT_23515
  ara::core::Result<void> SetKey(const PrivateKey& key) noexcept override;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_23516
  ara::core::Result<void> Reset() noexcept;

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use,
  /// and @c false otherwise
  /// @trace SWS_CRYPT_20412
  bool IsInitialized() const noexcept override
  {
    return isInitialize_;
  }

  /// @brief Getter method to get the private key loaded to this ctx.
  EVP_PKEY* GetPrivateKey() const noexcept
  {
    return privateKey_;
  }

 private:
  /// @brief Configure the signer ctx of the Openssl.
  void ConfigureSignCtx(EVP_PKEY_CTX* keyCtx) const noexcept;

  /// @brief algorithm Id for this context.
  AlgId algId_;

  /// @brief pointer to the openssl private key.
  EVP_PKEY* privateKey_{ nullptr };

  /// @brief flag to indecate whether ctx is initialized or not.
  bool isInitialize_{ false };

  ///@brief Logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "SPRC", "Signer Private context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_SIGNER_PRIVATE_CTX_DEFAULT_SSL_H_
