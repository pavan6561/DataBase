// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_provider_ssl.h
//
// Purpose     : This file contains definitions for CryptoProviderSSL class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYPTO_PROVIDER_SSL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYPTO_PROVIDER_SSL_H_

#include <ara/core/result.h>
#include <ara/core/string.h>
#include <ara/log/logging.h>
#include <memory>

#include "algorithm_identification.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "auth_cipher_ctx_ssl_aes_gcm.h"
#include "decryptor_private_ctx_ssl_rsa.h"
#include "encryptor_public_ctx_ssl_rsa.h"
#include "hash_function_ctx_default_ssl_sha256.h"
#include "io_interface_sqlite.h"
#include "io_interface_volatile.h"
#include "key_agreement_private_ctx_default_ssl.h"
#include "key_decapsulator_private_ctx_default_ssl.h"
#include "key_derivation_function_ctx_ssl_default.h"
#include "key_encapsulator_public_ctx_default_ssl.h"
#include "message_authentication_code_ctx_ssl.h"
#include "msg_recovery_public_ctx_default_ssl.h"
#include "random_generator_ctx_ssl_ctr_drbg.h"
#include "sig_encode_private_ctx_default_ssl.h"
#include "signer_private_ctx_default_ssl.h"
#include "stream_cipher_ctx_ssl_aes_ctr.h"
#include "symmetric_block_cipher_ctx_ssl_aes.h"
#include "symmetric_key_wrapper_ctx_ssl.h"
#include "uuid_impl.h"
#include "verifier_public_ctx_default_ssl.h"
#include "volatile_trusted_container_impl.h"

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
class CryptoProviderSSL : public CryptoProvider
{
 public:
  /// @brief Constructor for CryptoProviderSSL class.
  CryptoProviderSSL() noexcept :
    CryptoProvider{}, aralog_{ ara::log::CreateLogger(
                        "CRPC", "CRYPTO provider context", ara::log::LogLevel::kVerbose) }
  {
  }

  /// @brief Destructor.
  /// @trace SWS_CRYPT_20710
  ~CryptoProviderSSL() noexcept override = default;

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
  ara::core::Result<SymmetricKey::Uptrc> GenerateSymmetricKey(AlgId algId, AllowedUsageFlags allowedUsage,
    bool isSession = true, bool isExportable = false) noexcept override;

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
  ara::core::Result<SecretSeed::Uptrc> GenerateSeed(AlgId algId, SecretSeed::Usage allowedUsage,
    bool isSession = true, bool isExportable = false) noexcept override;

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
  /// target algorithm @c algId (note: it is an optional error condition for this method)
  /// @trace SWS_CRYPT_20722
  ara::core::Result<PrivateKey::Uptrc> GeneratePrivateKey(AlgId algId, AllowedUsageFlags allowedUsage,
    bool isSession = false, bool isExportable = false) noexcept override;

  /// @brief Allocate a Volatile (virtual) Trusted Container according to directly specified capacity.
  /// The Volatile Trusted Container can be used for execution of the import operations.
  /// Current process obtains the "Owner" rights for allocated Container.
  /// If <tt>(capacity == 0)</tt> then the capacity of the container will be selected automatically according
  /// to a maximal size of supported crypto objects.
  /// A few volatile (temporary) containers can coexist at same time without any affecting each-other.
  /// @param[in] capacity  the capacity required for this volatile trusted container (in bytes)
  /// @returns unique smart pointer to an allocated volatile trusted container
  /// @trace SWS_CRYPT_20726
  ara::core::Result<VolatileTrustedContainer::Uptr> AllocVolatileContainer(
    std::size_t capacity = 0) noexcept override;

  /// @brief Allocate a Volatile (virtual) Trusted Container according to indirect specification of a minimal
  /// required capacity for hosting of any listed object.
  /// The Volatile Trusted Container can be used for execution of the import operations.
  /// Current process obtains the "Owner" rights for allocated Container.
  /// Real container capacity is calculated as a maximal storage size of all listed objects.
  /// @param[in] theObjectDef  the list of objects that can be stored to this volatile trusted container
  /// @returns unique smart pointer to an allocated volatile trusted container
  /// @error SecurityErrorDomain::kInvalidArgument  if unsupported combination of object type and
  /// algorithm ID presents in the list
  /// @trace SWS_CRYPT_20727
  ara::core::Result<VolatileTrustedContainer::Uptr> AllocVolatileContainer(
    std::pair<AlgId, CryptoObjectType> theObjectDef) noexcept override;

  /// @brief Create a Random Number Generator (RNG) context.
  /// @param[in] algId  identifier of target RNG algorithm. If no algId is given, the default RNG is returned
  /// @param[in] initialize  indicates whether the returned context shall be initialized (i.e., seeded) by
  /// the stack
  /// @returns unique smart pointer to the created RNG context
  /// @trace SWS_CRYPT_20741, SWS_CRYPT_00500, SWS_CRYPT_00501, SWS_CRYPT_00504, SWS_CRYPT_00505,
  /// SWS_CRYPT_00506
  ara::core::Result<RandomGeneratorCtx::Uptr> CreateRandomGeneratorCtx(
    AlgId algId, bool initialize = true) noexcept override;

  /// @brief Create a hash function context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20747, SWS_CRYPT_00901
  ara::core::Result<HashFunctionCtx::Uptr> CreateHashFunctionCtx(AlgId algId) noexcept override;

  /// @brief Create a symmetric block cipher context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20742_001, SWS_CRYPT_20742_002, SWS_CRYPT_01501
  ara::core::Result<SymmetricBlockCipherCtx::Uptr> CreateSymmetricBlockCipherCtx(
    AlgId algId) noexcept override;

  /// @brief Create a symmetric stream cipher context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20744_001, SWS_CRYPT_20744_002, SWS_CRYPT_20744_003, SWS_CRYPT_01651
  ara::core::Result<StreamCipherCtx::Uptr> CreateStreamCipherCtx(AlgId algId) noexcept override;

  /// @brief Create a symmetric authenticated cipher context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20745_001, SWS_CRYPT_20745_002, SWS_CRYPT_20745_003, SWS_CRYPT_01806
  ara::core::Result<AuthCipherCtx::Uptr> CreateAuthCipherCtx(AlgId algId) noexcept override;

  /// @brief Create a key derivation function context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20748_001, SWS_CRYPT_20748_002, SWS_CRYPT_20748_003, SWS_CRYPT_00601
  ara::core::Result<KeyDerivationFunctionCtx::Uptr> CreateKeyDerivationFunctionCtx(
    AlgId algId) noexcept override;

  /// @brief Create a symmetric message authentication code context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20746_001, SWS_CRYPT_20746_002, SWS_CRYPT_20746_003, SWS_CRYPT_01200
  ara::core::Result<MessageAuthnCodeCtx::Uptr> CreateMessageAuthCodeCtx(AlgId algId) noexcept override;

  /// @brief Create a symmetric key-wrap algorithm context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20743_001, SWS_CRYPT_20743_002, SWS_CRYPT_20743_003, SWS_CRYPT_02100
  ara::core::Result<SymmetricKeyWrapperCtx::Uptr> CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept override;

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

  /// @brief Create a key-encapsulator public key context of a Key Encapsulation Mechanism (KEM).
  /// @param[in] algId  identifier of the target KEM crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric KEM
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20752
  ara::core::Result<KeyEncapsulatorPublicCtx::Uptr> CreateKeyEncapsulatorPublicCtx(
    AlgId algId) noexcept override;

  /// @brief Create a key-decapsulator private key context of a Key Encapsulation Mechanism (KEM).
  /// @param[in] algId  identifier of the target KEM crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric KEM
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20753
  ara::core::Result<KeyDecapsulatorPrivateCtx::Uptr> CreateKeyDecapsulatorPrivateCtx(
    AlgId algId) noexcept override;

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

  /// @brief Create a key-agreement private key context.
  /// @param[in] algId  identifier of the target key-agreement crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from key-agreement
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20758
  ara::core::Result<KeyAgreementPrivateCtx::Uptr> CreateKeyAgreementPrivateCtx(AlgId algId) noexcept override;

  /// @brief Load any crypto object from the IOInterface provided.
  /// @param[in] container  the IOInterface that contains the crypto object for loading
  /// @returns unique smart pointer to the created object
  /// @error SecurityErrorDomain::kEmptyContainer  if the container is empty
  /// @error SecurityErrorDomain::kResourceFault  if the container content is damaged
  /// @error SecurityErrorDomain::kModifiedResource if the underlying resource has been modified after
  /// the IOInterface has been opened, i.e., the IOInterface has been invalidated.
  /// @error SecurityErrorDomain::kIncompatibleObject  if the underlying resource belongs to another,
  /// incompatible CryptoProvider
  /// @note This method is one of the "binding" methods between a CryptoProvider and the %Key Storage Provider.
  /// @trace SWS_CRYPT_20733
  ara::core::Result<CryptoObject::Uptrc> LoadObject(const IOInterface& container) noexcept override;

  /// @brief Load a symmetric key from the IOInterface provided.
  /// @param[in] container  the IOInterface that contains the crypto object for loading
  /// @returns unique smart pointer to the SymmetricKey
  /// @error SecurityErrorDomain::kEmptyContainer  if the container is empty
  /// @error SecurityErrorDomain::kResourceFault  if the container content is damaged
  /// @error SecurityErrorDomain::kModifiedResource if the underlying resource has been modified after
  /// the IOInterface has been opened, i.e., the IOInterface has been invalidated.
  /// @error SecurityErrorDomain::kIncompatibleObject  if the underlying resource belongs to another,
  /// incompatible CryptoProvider
  /// @trace SWS_CRYPT_20762
  ara::core::Result<SymmetricKey::Uptrc> LoadSymmetricKey(const IOInterface& container) noexcept override;

  /// @brief Load a public key from the IOInterface provided.
  /// @param[in] container  the IOInterface that contains the crypto object for loading
  /// @returns unique smart pointer to the PublicKey
  /// @error SecurityErrorDomain::kEmptyContainer  if the container is empty
  /// @error SecurityErrorDomain::kResourceFault  if the container content is damaged
  /// @error SecurityErrorDomain::kModifiedResource if the underlying resource has been modified after
  /// the IOInterface has been opened, i.e., the IOInterface has been invalidated.
  /// @error SecurityErrorDomain::kIncompatibleObject  if the underlying resource belongs to another,
  /// incompatible CryptoProvider
  /// @trace SWS_CRYPT_20763
  ara::core::Result<PublicKey::Uptrc> LoadPublicKey(const IOInterface& container) noexcept override;

  /// @brief Load a private key from the IOInterface provided.
  /// @param[in] container  the IOInterface that contains the crypto object for loading
  /// @returns unique smart pointer to the PrivateKey
  /// @error SecurityErrorDomain::kEmptyContainer  if the container is empty
  /// @error SecurityErrorDomain::kResourceFault  if the container content is damaged
  /// @error SecurityErrorDomain::kModifiedResource if the underlying resource has been modified after
  /// the IOInterface has been opened, i.e., the IOInterface has been invalidated.
  /// @error SecurityErrorDomain::kIncompatibleObject  if the underlying resource belongs to another,
  /// incompatible CryptoProvider
  /// @trace SWS_CRYPT_20764
  ara::core::Result<PrivateKey::Uptrc> LoadPrivateKey(const IOInterface& container) noexcept override;

  /// @brief Load secret seed from the IOInterface provided.
  /// @param[in] container  the IOInterface that contains the crypto object for loading
  /// @returns unique smart pointer to the SecretSeed
  /// @error SecurityErrorDomain::kEmptyContainer  if the container is empty
  /// @error SecurityErrorDomain::kResourceFault  if the container content is damaged
  /// @error SecurityErrorDomain::kModifiedResource if the underlying resource has been modified after
  /// the IOInterface has been opened, i.e., the IOInterface has been invalidated.
  /// @error SecurityErrorDomain::kIncompatibleObject  if the underlying resource belongs to another,
  /// incompatible CryptoProvider
  /// @trace SWS_CRYPT_20765
  ara::core::Result<SecretSeed::Uptrc> LoadSecretSeed(const IOInterface& container) noexcept override;

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

 private:
  /// @brief generate random data of given number of bytes.
  /// @param[in] count required size of random data.
  /// @return Vector of random data bytes of size specified by @count.
  ara::core::Vector<uint8_t> GenerateRandomData(std::uint32_t count);

  /// @brief Logger variable for ara::log
  ara::log::Logger& aralog_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYPTO_PROVIDER_SSL_H_
