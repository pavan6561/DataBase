// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_provider_proxy_impl.h
//
// Purpose     : Crypto provider proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYPTO_PROVIDER_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYPTO_PROVIDER_PROXY_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/crypto_provider.h"
#include "authcipher_context_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "decryptor_private_ctx_proxy_impl.h"
#include "encryptor_public_ctx_proxy_impl.h"
#include "hash_function_ctx_proxy_impl.h"
#include "key_derivation_function_ctx_proxy_impl.h"
#include "message_authn_code_ctx_proxy_impl.h"
#include "msg_recovery_public_ctx_proxy_impl.h"
#include "private_key_proxy_impl.h"
#include "random_generator_ctx_proxy_impl.h"
#include "sig_encode_private_ctx_proxy_impl.h"
#include "signer_private_ctx_proxy_impl.h"
#include "streamcipher_context_proxy_impl.h"
#include "symmetric_key_proxy_impl.h"
#include "symmetricblock_cipher_ctx_proxy_impl.h"
#include "symmetrickeywrapper_ctx_proxy_impl.h"
#include "verifier_public_ctx_proxy_impl.h"
#include "vsomeip_interface.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Crypto Provider is a "factory" interface of all supported Crypto Primitives and a "trusted
/// environment" for internal communications between them.
/// All Crypto Primitives should have an actual reference to their parent Crypto Provider.
/// A Crypto Provider can be destroyed only after destroying of all its daughterly Crypto Primitives.
/// Each method of this interface that creates a Crypto Primitive instance is non-constant, because any such
/// creation increases a references counter of the Crypto Primitive.
/// @trace SWS_CRYPT_20700
class CryptoProviderproxyImpl : public ara::crypto::cryp::CryptoProvider
{
 public:
  /// @brief Constructor for the CryptoProviderproxyImpl class.
  /// @param[in] serviceInstanceID CryptoServiceInstance Id for the crypto provider.
  CryptoProviderproxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept;

  /// @brief Destructor for CryptoProviderproxyImpl class.
  virtual ~CryptoProviderproxyImpl() noexcept = default;

  /// @brief Create a Random Number Generator (RNG) context.
  /// @param[in] algId  identifier of target RNG algorithm. If no algId is given, the default RNG is returned
  /// @param[in] initialize  indicates whether the returned context shall be initialized (i.e., seeded) by
  /// the stack
  /// @returns unique smart pointer to the created RNG context
  /// @trace SWS_CRYPT_20741
  ara::core::Result<RandomGeneratorCtx::Uptr> CreateRandomGeneratorCtx(
    AlgId algId = kAlgIdDefault, bool initialize = true) noexcept override;

  /// @brief Create a symmetric block cipher context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20742
  ara::core::Result<SymmetricBlockCipherCtx::Uptr> CreateSymmetricBlockCipherCtx(
    AlgId algId) noexcept override;

  /// @brief Create a symmetric key-wrap algorithm context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20743
  ara::core::Result<SymmetricKeyWrapperCtx::Uptr> CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept override;

  /// @brief Create a symmetric stream cipher context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20744
  ara::core::Result<StreamCipherCtx::Uptr> CreateStreamCipherCtx(AlgId algId) noexcept override;

  /// @brief Create a symmetric authenticated cipher context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20745
  ara::core::Result<AuthCipherCtx::Uptr> CreateAuthCipherCtx(AlgId algId) noexcept override;

  /// @brief Create a symmetric message authentication code context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20746
  ara::core::Result<MessageAuthnCodeCtx::Uptr> CreateMessageAuthCodeCtx(AlgId algId) noexcept override;

  /// @brief Create a hash function context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20747
  ara::core::Result<HashFunctionCtx::Uptr> CreateHashFunctionCtx(AlgId algId) noexcept override;

  /// @brief Create a key derivation function context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20748
  ara::core::Result<KeyDerivationFunctionCtx::Uptr> CreateKeyDerivationFunctionCtx(
    AlgId algId) noexcept override;

  /// @brief Create an encryption public key context.
  /// @param[in] algId  identifier of the target asymmetric encryption/decryption algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric encryption/decryption
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20750
  ara::core::Result<EncryptorPublicCtx::Uptr> CreateEncryptorPublicCtx(AlgId algId) noexcept override;

  /// @brief Create a decryption private key context.
  /// @param[in] algId  identifier of the target asymmetric encryption/decryption algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric encryption/decryption
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20751
  ara::core::Result<DecryptorPrivateCtx::Uptr> CreateDecryptorPrivateCtx(AlgId algId) noexcept override;

  /// @brief Create a signature encoding private key context.
  /// @param[in] algId  identifier of the target asymmetric crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric signature encoding with message recovery
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20754
  ara::core::Result<SigEncodePrivateCtx::Uptr> CreateSigEncodePrivateCtx(AlgId algId) noexcept override;

  /// @brief Create a message recovery public key context.
  /// @param[in] algId  identifier of the target asymmetric crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric signature encoding with message recovery
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20755
  ara::core::Result<MsgRecoveryPublicCtx::Uptr> CreateMsgRecoveryPublicCtx(AlgId algId) noexcept override;

  /// @brief Create a signature private key context.
  /// @param[in] algId  identifier of the target signature crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from private key signature
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20756
  ara::core::Result<SignerPrivateCtx::Uptr> CreateSignerPrivateCtx(AlgId algId) noexcept override;

  /// @brief Create a signature verification public key context.
  /// @param[in] algId  identifier of the target signature crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from public key signature verification
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20757
  ara::core::Result<VerifierPublicCtx::Uptr> CreateVerifierPublicCtx(AlgId algId) noexcept override;

  /// @brief Allocate a new symmetric key object and fill it by a new randomly generated value.
  /// Any serializable (i.e. savable/non-session or exportable) key must generate own COUID!
  /// By default Crypto Provider should use an internal instance of a best from all supported RNG (ideally
  /// TRNG).
  /// @param[in] algId  the identifier of target symmetric crypto algorithm
  /// @param[in] allowedUsage  the flags that define a list of allowed transformations' types in which the
  /// target key can be used (see constants in scope of @c RestrictedUseObject)
  /// @param[in] isSession  the "session" (or "temporary") attribute of the target key (if @c true)
  /// @param[in] isExportable  the exportability attribute of the target key (if @c true)
  /// @returns smart unique pointer to the created symmetric key object
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId has an unsupported value
  /// @error SecurityErrorDomain::kIncompatibleArguments  if @c allowedUsage argument is incompatible with
  /// target algorithm @c algId (note: it is an optional error condition for this method)
  /// @trace SWS_CRYPT_20721
  ara::core::Result<SymmetricKey::Uptrc> GenerateSymmetricKey(
    AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept override;

  /// @brief Generate a random Secret Seed object of requested algorithm.
  /// @param[in] algId  the identifier of target crypto algorithm
  /// @param[in] allowedUsage  the lags that define a list of allowed transformations' types in which the
  /// target seed can be used (see constants in scope of @c RestrictedUseObject)
  /// @param[in] isSession  the "session" (or "temporary") attribute of the target seed (if @c true)
  /// @param[in] isExportable  the exportability attribute of the target seed (if @c true)
  /// @returns unique smart pointer to generated @c SecretSeed object
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId has an unsupported value
  /// @error SecurityErrorDomain::kIncompatibleArguments  if @c allowedUsage argument is incompatible with
  /// target algorithm @c algId (note: it is an optional error condition for this method)
  /// @trace SWS_CRYPT_20723
  ara::core::Result<SecretSeed::Uptrc> GenerateSeed(
    AlgId algId, SecretSeed::Usage allowedUsage, bool isSession, bool isExportable) noexcept override;

  /// @brief Allocate a new private key context of correspondent type and generates the key value randomly.
  /// A common COUID should be shared for both private and public keys.
  /// Any serializable (i.e. savable/non-session or exportable) key must generate own COUID!
  /// @param[in] algId  the identifier of target public-private key crypto algorithm
  /// @param[in] allowedUsage  the flags that define a list of allowed transformations' types in which the
  /// target key can be used (see constants in scope of @c RestrictedUseObject)
  /// @param[in] isSession  the "session" (or "temporary") attribute for the target key (if @c true)
  /// @param[in] isExportable  the exportability attribute of the target key (if @c true)
  /// @returns smart unique pointer to the created private key object
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId has an unsupported value
  /// @error SecurityErrorDomain::kIncompatibleArguments  if @c allowedUsage argument is incompatible with
  /// target algorithm algId (note: it is an optional error condition for this method)
  /// @trace SWS_CRYPT_20722
  ara::core::Result<PrivateKey::Uptrc> GeneratePrivateKey(
    AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept override;

  /// @brief Construct @c Signature object from directly provided components of a digital signature/MAC or
  /// authenticated encryption (AE/AEAD).
  /// All integers inside a digital signature BLOB value are always presented in Big Endian bytes order
  /// (i.e. MSF - Most Significant byte First).
  /// @param[in] signAlgId  identifier of an applied signature/MAC/AE/AEAD crypto algorithm
  /// @param[in] value  raw BLOB value of the signature/MAC
  /// @param[in] key  symmetric or asymmetric key (according to @c signAlgId) applied for the sign or MAC/AE/
  /// AEAD operation
  /// @param[in] hashAlgId  identifier of a hash function algorithm applied together with the signature
  /// algorithm
  /// @returns unique smart pointer to the created @c Signature object
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c signAlgId or @c hashAlgId arguments have
  /// unsupported values
  /// @error SecurityErrorDomain::kInvalidArgument  if @c signAlgId or @c hashAlgId arguments specify
  /// crypto algorithms different from the signature/MAC/AE/AEAD and message digest respectively
  /// @error SecurityErrorDomain::kIncompatibleArguments  if @c signAlgId and @c hashAlgId arguments specify
  /// incompatible algorithms (if @c signAlgId includes hash function specification)
  /// or if a crypto primitive associated with the @c key argument is incompatible with provided
  /// @c signAlgId or @c hashAlgId arguments
  /// @error SecurityErrorDomain::kInvalidInputSize  if the @c value argument has invalid size
  /// (i.e. incompatible with the @c signAlgId argument)
  /// @trace SWS_CRYPT_20760
  ara::core::Result<Signature::Uptrc> CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value,
    const RestrictedUseObject& key, AlgId hashAlgId = kAlgIdNone) noexcept override;

  /// @brief Convert a common name of crypto algorithm to a correspondent vendor specific binary algorithm ID.
  /// @param[in] primitiveName  the unified name of the crypto primitive (see "Crypto Primitives Naming
  /// Convention" for more details)
  /// @returns vendor specific binary algorithm ID or @c kAlgIdUndefined if a primitive with provided name is
  /// not supported.
  /// @trace SWS_CRYPT_20711
  AlgId ConvertToAlgId(ara::core::StringView primitiveName) const noexcept override;

  /// @brief Convert a vendor specific binary algorithm ID to a correspondent common name of the crypto
  /// algorithm.
  /// @param[in] algId  the vendor specific binary algorithm ID
  /// @returns the common name of the crypto algorithm (see "Crypto Primitives Naming Convention" for more
  /// details)
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20712
  ara::core::Result<ara::core::String> ConvertToAlgName(AlgId algId) const noexcept override;

 private:
  /// @brief initialize proxy implementation.
  void Initialize(const CryptoServiceInstanceId serviceInstanceID);

  /// @brief Function to check if given algorithm Id is supported by crypto provider.
  bool IsAlgorithmSupported(AlgId algId) noexcept;

  /// @brief Function to check if given stream cipher algorithm Id is supported by crypto provider.
  bool IsStreamCipherAlgorithmSupported(AlgId algId);

  /// @brief Function to check if given Authentication cipher algorithm Id is supported by crypto provider.
  bool IsAuthenticationCipherAlgorithmSupported(AlgId algId);

  /// @brief vsomeip instance for crypto provider proxy.
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    vsomeipInstance_;

  /// @brief shared handle for crypto provider proxy.
  std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>
    cryptoProviderHandler_;

  /// @brief Logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYPTO_PROVIDER_PROXY_IMPL_H_
