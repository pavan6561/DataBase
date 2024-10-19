// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_provider.h
//
// Purpose     : This file contains definitions for CryptoProvider abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYPTO_PROVIDER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYPTO_PROVIDER_H_

#include <ara/core/result.h>
#include <ara/core/string.h>

#include "ara/crypto/common/security_error_domain.h"
#include "ara/crypto/cryp/auth_cipher_ctx.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"
#include "ara/crypto/cryp/decryptor_private_ctx.h"
#include "ara/crypto/cryp/encryptor_public_ctx.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "ara/crypto/cryp/message_authn_code_ctx.h"
#include "ara/crypto/cryp/msg_recovery_public_ctx.h"
#include "ara/crypto/cryp/random_generator_ctx.h"
#include "ara/crypto/cryp/sig_encode_private_ctx.h"
#include "ara/crypto/cryp/signer_private_ctx.h"
#include "ara/crypto/cryp/stream_cipher_ctx.h"
#include "ara/crypto/cryp/symmetric_block_cipher_ctx.h"
#include "ara/crypto/cryp/symmetric_key_wrapper_ctx.h"
#include "ara/crypto/cryp/verifier_public_ctx.h"
#include "ara/crypto/cryp/cryobj/signature.h"

#include "ara/core/result.h"
#include "ara/core/string.h"

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
class CryptoProvider
{
 public:
  /// @brief Shared smart pointer of the interface.
  /// @trace SWS_CRYPT_20701
  using Uptr = std::unique_ptr<CryptoProvider>;

  /// @brief A short alias for Algorithm ID type definition.
  /// @trace SWS_CRYPT_20703
  using AlgId = CryptoPrimitiveId::AlgId;

  /// @brief Destructor.
  /// @trace SWS_CRYPT_20710
  virtual ~CryptoProvider() noexcept = default;

  /// @brief Copy-assign another CryptoProvider to this instance.
  /// @param other  the other instance.
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30216
  CryptoProvider& operator=(const CryptoProvider& other) = default;

  /// @brief Move-assign another CryptoProvider to this instance.
  /// @param other  the other instance.
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30217
  CryptoProvider& operator=(CryptoProvider&& other) = default;

  /// @brief Create a Random Number Generator (RNG) context.
  /// @param[in] algId  identifier of target RNG algorithm. If no algId is given, the default RNG is returned
  /// @param[in] initialize  indicates whether the returned context shall be initialized (i.e., seeded) by
  /// the stack.
  /// @returns unique smart pointer to the created RNG context.
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value or
  /// if <tt>(algId == kAlgIdDefault)</tt> and the CryptoProvider does not provide any
  /// RandomGeneratorCtx
  /// @error SecurityErrorDomain::kBusyResource  if <tt>(initialize == true)</tt> but the context currently
  /// cannot be seeded (e.g., due to a lack of entropy)
  /// @trace SWS_CRYPT_20741
  virtual ara::core::Result<RandomGeneratorCtx::Uptr> CreateRandomGeneratorCtx(
    AlgId algId = kAlgIdDefault, bool initialize = true) noexcept
    = 0;

  /// @brief Create a hash function context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from hash function
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_20747
  virtual ara::core::Result<HashFunctionCtx::Uptr> CreateHashFunctionCtx(AlgId algId) noexcept = 0;

  /// @brief Create a symmetric block cipher context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20742_001, SWS_CRYPT_20742_002, SWS_CRYPT_01501
  virtual ara::core::Result<SymmetricBlockCipherCtx::Uptr> CreateSymmetricBlockCipherCtx(AlgId algId) noexcept
    = 0;

  /// @brief Create a symmetric stream cipher context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20744_001, SWS_CRYPT_20744_002, SWS_CRYPT_20744_003, SWS_CRYPT_01651
  virtual ara::core::Result<StreamCipherCtx::Uptr> CreateStreamCipherCtx(AlgId algId) noexcept = 0;

  /// @brief Create a symmetric authenticated cipher context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20745_001, SWS_CRYPT_20745_002, SWS_CRYPT_20745_003, SWS_CRYPT_01806
  virtual ara::core::Result<AuthCipherCtx::Uptr> CreateAuthCipherCtx(AlgId algId) noexcept = 0;

  /// @brief Create a key derivation function context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20748_001, SWS_CRYPT_20748_002, SWS_CRYPT_20748_003, SWS_CRYPT_00601
  virtual ara::core::Result<KeyDerivationFunctionCtx::Uptr> CreateKeyDerivationFunctionCtx(
    AlgId algId) noexcept
    = 0;

  /// @brief Create a symmetric message authentication code context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20746_001, SWS_CRYPT_20746_002, SWS_CRYPT_20746_003, SWS_CRYPT_01200
  virtual ara::core::Result<MessageAuthnCodeCtx::Uptr> CreateMessageAuthCodeCtx(AlgId algId) noexcept = 0;

  /// @brief Create a symmetric key-wrap algorithm context.
  /// @param[in] algId  identifier of the target crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @trace SWS_CRYPT_20743_001, SWS_CRYPT_20743_002, SWS_CRYPT_20743_003, SWS_CRYPT_02100
  virtual ara::core::Result<SymmetricKeyWrapperCtx::Uptr> CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept
    = 0;

  /// @brief Create an encryption public key context.
  /// @param[in] algId  identifier of the target asymmetric encryption/decryption algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric encryption/decryption
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20750
  virtual ara::core::Result<EncryptorPublicCtx::Uptr> CreateEncryptorPublicCtx(AlgId algId) noexcept = 0;

  /// @brief Create a decryption private key context.
  /// @param[in] algId  identifier of the target asymmetric encryption/decryption algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric encryption/decryption
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20751
  virtual ara::core::Result<DecryptorPrivateCtx::Uptr> CreateDecryptorPrivateCtx(AlgId algId) noexcept = 0;

  /// @brief Create a signature encoding private key context.
  /// @param[in] algId  identifier of the target asymmetric crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric signature encoding with message recovery
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20754
  virtual ara::core::Result<SigEncodePrivateCtx::Uptr> CreateSigEncodePrivateCtx(AlgId algId) noexcept = 0;

  /// @brief Create a message recovery public key context.
  /// @param[in] algId  identifier of the target asymmetric crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from asymmetric signature encoding with message recovery
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20755
  virtual ara::core::Result<MsgRecoveryPublicCtx::Uptr> CreateMsgRecoveryPublicCtx(AlgId algId) noexcept = 0;

  /// @brief Create a signature private key context.
  /// @param[in] algId  identifier of the target signature crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from private key signature
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20756
  virtual ara::core::Result<SignerPrivateCtx::Uptr> CreateSignerPrivateCtx(AlgId algId) noexcept = 0;

  /// @brief Create a signature verification public key context.
  /// @param[in] algId  identifier of the target signature crypto algorithm
  /// @returns unique smart pointer to the created context
  /// @error SecurityErrorDomain::kInvalidArgument  if @c algId argument specifies a crypto algorithm
  /// different from public key signature verification
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20757
  virtual ara::core::Result<VerifierPublicCtx::Uptr> CreateVerifierPublicCtx(AlgId algId) noexcept = 0;

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
  virtual ara::core::Result<SymmetricKey::Uptrc> GenerateSymmetricKey(
    AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = true, bool isExportable = false) noexcept
    = 0;

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
  virtual ara::core::Result<PrivateKey::Uptrc> GeneratePrivateKey(
    AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = false, bool isExportable = false) noexcept
    = 0;

  /// @uptrace{RS_CRYPTO_02007}
  /// @brief Generate a random Secret Seed object of requested algorithm.
  /// @param[in] algId  the identifier of target crypto algorithm
  /// @param[in] allowedUsage  the lags that define a list of allowed transformations' types in which the
  /// target seed can be used (see constants in scope of @c RestrictedUseObject).
  /// @param[in] isSession  the "session" (or "temporary") attribute of the target seed (if @c true)
  /// @param[in] isExportable  the exportability attribute of the target seed (if @c true)
  /// @returns unique smart pointer to generated @c SecretSeed object
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId has an unsupported value
  /// @error SecurityErrorDomain::kIncompatibleArguments  if @c allowedUsage argument is incompatible with
  /// target algorithm @c algId (note: it is an optional error condition for this method)
  /// @trace SWS_CRYPT_20723
  virtual ara::core::Result<SecretSeed::Uptrc> GenerateSeed(
    AlgId algId, SecretSeed::Usage allowedUsage, bool isSession = true, bool isExportable = false) noexcept
    = 0;

  /// @brief Load any crypto object from the IOInterface provided.
  /// @param[in] container  the IOInterface that contains the crypto object for loading
  /// @returns unique smart pointer to the created object
  /// @error SecurityErrorDomain::kEmptyContainer  if the container is empty
  /// @error SecurityErrorDomain::kResourceFault  if the container content is damaged
  /// @error SecurityErrorDomain::kModifiedResource if the underlying resource has been modified after
  /// the IOInterface has been opened, i.e., the IOInterface has been invalidated.
  /// @error SecurityErrorDomain::kIncompatibleObject  if the underlying resource belongs to another,
  /// incompatible CryptoProvider
  /// @note This method is one of the "binding" methods between a CryptoProvider and the Key Storage Provider.
  /// @trace SWS_CRYPT_20733
  // virtual ara::core::Result<CryptoObject::Uptrc> LoadObject(const IOInterface& container
  // ) noexcept = 0;

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
  // virtual ara::core::Result<SymmetricKey::Uptrc> LoadSymmetricKey(const IOInterface& container
  // ) noexcept = 0;

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
  // virtual ara::core::Result<PublicKey::Uptrc> LoadPublicKey(const IOInterface& container
  // ) noexcept = 0;

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
  // virtual ara::core::Result<PrivateKey::Uptrc> LoadPrivateKey(const IOInterface& container
  // ) noexcept = 0;

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
  // virtual ara::core::Result<SecretSeed::Uptrc> LoadSecretSeed(const IOInterface& container
  // ) noexcept = 0;

  /// @brief Convert a common name of crypto algorithm to a correspondent vendor specific binary algorithm ID.
  /// @param[in] primitiveName  the unified name of the crypto primitive (see "Crypto Primitives Naming
  /// Convention" for more details)
  /// @returns vendor specific binary algorithm ID or @c kAlgIdUndefined if a primitive with provided name
  /// is not supported
  /// @trace SWS_CRYPT_20711
  virtual AlgId ConvertToAlgId(ara::core::StringView primitiveName) const noexcept = 0;

  /// @brief Convert a vendor specific binary algorithm ID to a correspondent common name of the crypto
  /// algorithm.
  /// @param[in] algId  the vendor specific binary algorithm ID
  /// @returns the common name of the crypto algorithm (see "Crypto Primitives Naming Convention" for more
  /// details)
  /// @error SecurityErrorDomain::kUnknownIdentifier  if @c algId argument has an unsupported value
  /// @trace SWS_CRYPT_20712
  virtual ara::core::Result<ara::core::String> ConvertToAlgName(AlgId algId) const noexcept = 0;

  /// @brief Construct @c Signature object from directly provided components of a digital signature/MAC or
  /// authenticated encryption (AE/AEAD).
  /// All integers inside a digital signature BLOB value are always presented in Big Endian bytes order
  /// (i.e. MSF - Most Significant byte First).
  /// @param[in] signAlgId  identifier of an applied signature/MAC/AE/AEAD crypto algorithm
  /// @param[in] value  raw BLOB value of the signature/MAC
  /// @param[in] key  symmetric or asymmetric key (according to @c signAlgId) applied for the sign or
  /// MAC/AE/AEAD operation.
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
  /// signAlgId or @c hashAlgId arguments
  /// @error SecurityErrorDomain::kInvalidInputSize  if the @c value argument has invalid size
  /// (i.e. incompatible with the @c signAlgId argument)
  /// @trace SWS_CRYPT_20760
  virtual ara::core::Result<Signature::Uptrc> CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value,
    const RestrictedUseObject& key, AlgId hashAlgId = kAlgIdNone) noexcept
    = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYPTO_PROVIDER_H_
