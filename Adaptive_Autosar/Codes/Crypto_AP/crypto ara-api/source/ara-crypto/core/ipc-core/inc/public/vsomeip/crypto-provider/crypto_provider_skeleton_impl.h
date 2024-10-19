// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_provider_skeleton_impl.h
//
// Purpose     : Crypto provider skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYPTOPROVIDER_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYPTOPROVIDER_SKELETON_IMPL_H_

#include <ara/core/map.h>
#include <ara/log/logging.h>
#include <mutex>

#include "ara/crypto/cryp/crypto_provider.h"
#include "authcipher_context_skeleton_impl.h"
#include "crypto_config.h"
#include "cryptoproviderinterface_skeleton.h"
#include "decryptor_private_context_skeleton_impl.h"
#include "encryptor_public_context_skeleton_impl.h"
#include "hash_function_skeleton_impl.h"
#include "impl_type_cryptoserviceinstanceid.h"
#include "key_derivation_function_skeleton_impl.h"
#include "message_authn_code_skeleton_impl.h"
#include "msg_recovery_public_context_skeleton_impl.h"
#include "random_generator_skeleton_impl.h"
#include "sig_encode_private_context_skeleton_impl.h"
#include "signer_private_context_skeleton_impl.h"
#include "streamcipher_context_skeleton_impl.h"
#include "symmetricblock_context_skeleton_impl.h"
#include "symmetrickeywrapper_ctx_skeleton_impl.h"
#include "verifier_public_context_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief class definition for Crypto Provider Skeleton.
class CryptoProviderSkeletonImpl
  : public ara::crypto::ipccom::cryp::cryptoprovider::skeleton::CryptoProviderInterfaceSkeleton
{
 public:
  /// @brief to Skeleton for easy of usage.
  using Skeleton = ara::crypto::ipccom::cryp::cryptoprovider::skeleton::CryptoProviderInterfaceSkeleton;

  /// @brief CryptoProviderSkeletonImpl class constructor.
  explicit CryptoProviderSkeletonImpl(CryptoServiceInstanceId instance_id,
    std::shared_ptr<ara::crypto::config::CryptoConfig>& data,
    CryptoProvider::Uptr crypProviderPtr) noexcept(false);

  /// @brief CryptoProviderSkeletonImpl class distructor.
  ~CryptoProviderSkeletonImpl() override = default;

  /// @brief Method to create authentication cipher ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateAuthCipherOutput>
    CreateAuthCipher(const CryptoAlgId& algId) override;

  /// @brief Method to create decryptor private ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateDecryptorPrivateOutput>
    CreateDecryptorPrivate(const CryptoAlgId& algId) override;

  /// @brief Method to create encryptor public ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateEncryptorPublicOutput>
    CreateEncryptorPublic(const CryptoAlgId& algId) override;

  /// @brief Method to create Hash function ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateHashFunctionOutput>
    CreateHashFunction(const CryptoAlgId& algId) override;

  /// @brief Method to create key agreement private ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateKeyAgreementPrivateOutput>
    CreateKeyAgreementPrivate(const CryptoAlgId& algId) override;

  /// @brief Method to create key decapsulator private ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateKeyDecapsulatorPrivateOutput>
    CreateKeyDecapsulatorPrivate(const CryptoAlgId& algId) override;

  /// @brief Method to create key derivative function ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateKeyDerivationFunctionOutput>
    CreateKeyDerivationFunction(const CryptoAlgId& algId) override;

  /// @brief Method to create key encapsulator public ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateKeyEncapsulatorPublicOutput>
    CreateKeyEncapsulatorPublic(const CryptoAlgId& algId) override;

  /// @brief Method to create message authentication code ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateMessageAuthCodeOutput>
    CreateMessageAuthCode(const CryptoAlgId& algId) override;

  /// @brief Method to create message recovery public ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateMsgRecoveryPublicOutput>
    CreateMsgRecoveryPublic(const CryptoAlgId& algId) override;

  /// @brief Method to create random number generator ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateRandomGeneratorOutput>
    CreateRandomGenerator(const CryptoAlgId& algId, const Boolean& initialize) override;

  /// @brief Method to create sign encode private ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSigEncodePrivateOutput>
    CreateSigEncodePrivate(const CryptoAlgId& algId) override;

  /// @brief Method to create signer private ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSignerPrivateOutput>
    CreateSignerPrivate(const CryptoAlgId& algId) override;

  /// @brief Method to create stream cipher ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateStreamCipherOutput>
    CreateStreamCipher(const CryptoAlgId& algId) override;

  /// @brief Method to create symmetric block cipher ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSymmetricBlockCipherOutput>
    CreateSymmetricBlockCipher(const CryptoAlgId& algId) override;

  /// @brief Method to create symmetric key wrapper ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSymmetricKeyWrapperOutput>
    CreateSymmetricKeyWrapper(const CryptoAlgId& algId) override;

  /// @brief Method to create verifier public ctx instance.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateVerifierPublicOutput>
    CreateVerifierPublic(const CryptoAlgId& algId) override;

  /// @brief Method for skeleton implementation of GenerateSymmetricKey API.
  ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::GenSymmetricKeyOutput>
    GenSymmetricKey(const CryptoAlgId& algId, const CryptoAllowedUsageFlags& allowedUsage,
      const Boolean& isSession, const Boolean& isExportable) override;

  /// @brief Method to stop the crypto provider services.
  ara::core::Future<void> StopCryptoProviderService(
    const CryptoProviderServiceType& cpServiceId, const CryptoServiceInstanceId& instanceId) override;

  /// @brief Definition for GenPrivateKey.
  ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::GenPrivateKeyOutput>
    GenPrivateKey(const CryptoAlgId& algId, const CryptoAllowedUsageFlags& allowedUsages,
      const Boolean& isSession, const Boolean& isExportable) override;

  /// @brief Definition for GenSeed.
  ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::GenSeedOutput>
    GenSeed(const CryptoAlgId& algId, const CryptoAllowedUsageFlags& secretSeedAllowedUsages,
      const Boolean& isSession, const Boolean& isExportable) override;

  /// @brief Definition for LoadObject.
  ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadObjectOutput>
    LoadObject(const CryptoServiceInstanceId& IOInterfaceContainerInstanceId) override;

  /// @brief Definition for LoadPrivateKey.
  ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadPrivateKeyOutput>
    LoadPrivateKey(const CryptoServiceInstanceId& IOInterfaceContainerInstanceId) override;

  /// @brief Definition for LoadPublicKey.
  ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadPublicKeyOutput>
    LoadPublicKey(const CryptoServiceInstanceId& IOInterfaceContainerInstanceId) override;

  /// @brief Definition for LoadSecretSeed.
  ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadSecretSeedOutput>
    LoadSecretSeed(const CryptoServiceInstanceId& IOInterfaceContainerInstanceId) override;

  /// @brief Definition for LoadSymmetricKey.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadSymmetricKeyOutput>
    LoadSymmetricKey(const CryptoServiceInstanceId& IOInterfaceContainerInstanceId) override;

  /// @brief Definition for CreateHashDigest.
  ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateHashDigestOutput>
    CreateHashDigest(const CryptoAlgId& hashAlgId, const CryptoReadOnlyMemRegion& readOnlyValue) override;

  /// @brief Definition for CreateSignature.
  ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSignatureOutput>
    CreateSignature(const CryptoAlgId& signAlgId, const CryptoReadOnlyMemRegion& readOnlyValue,
      const RestrictedUseObjectStructure& restrictedUseObjStruct, const CryptoAlgId& hashAlgId) override;

 private:
  /// @brief shared pointer to the configuration.
  std::shared_ptr<ara::crypto::config::CryptoConfig> config_data_;

  /// @brief Pointer to hold the crypto provider instance.
  CryptoProvider::Uptr cryptoProviderPtr_;

  /// @brief map for instance id to RNG instance mapping.
  ara::core::Map<CryptoServiceInstanceId, std::unique_ptr<ara::crypto::cryp::RandomGeneratorSkeletonImpl>>
    rngContainer_{};

  /// @brief map for instance id to Hash instance mapping.
  ara::core::Map<CryptoServiceInstanceId, std::unique_ptr<ara::crypto::cryp::HashFunctionSkeletonImpl>>
    hashContainer_{};

  /// @brief map for instance id to mac instance mapping.
  ara::core::Map<CryptoServiceInstanceId, std::unique_ptr<ara::crypto::cryp::MessageAuthnCodeSkeletonImpl>>
    macContainer_{};

  /// @brief map for instance id to kdf instance mapping.
  ara::core::Map<CryptoServiceInstanceId,
    std::unique_ptr<ara::crypto::cryp::KeyDerivationFunctionskeletonImpl>>
    kdfContainer_{};

  /// @brief map for instance id to authentication cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId, std::unique_ptr<ara::crypto::cryp::AuthCipherContextSkeletonImpl>>
    accContainer_{};

  /// @brief map for instance id to stream cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId, std::unique_ptr<ara::crypto::cryp::StreamCipherContextSkeletonImpl>>
    sccContainer_{};

  /// @brief map for instance id to Symmetric block cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId,
    std::unique_ptr<ara::crypto::cryp::SymmetricBlockCipherContextSkeletonImpl>>
    sbccContainer_{};

  /// @brief map for instance id to Symmetric key wrapper cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId,
    std::unique_ptr<ara::crypto::cryp::SymmetricKeyWrapperCtxSkeletonImpl>>
    skwcContainer_{};

  /// @brief map for instance id to Symmetric key wrapper cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId,
    std::unique_ptr<ara::crypto::cryp::DecryptorPrivateContextSkeletonImpl>>
    decrypPriContainer_{};

  /// @brief map for instance id to Symmetric key wrapper cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId,
    std::unique_ptr<ara::crypto::cryp::EncryptorPrivateContextSkeletonImpl>>
    encryPubContainer_{};

  /// @brief map for instance id to Symmetric key wrapper cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId,
    std::unique_ptr<ara::crypto::cryp::MsgRecoveryPublicContextSkeletonImpl>>
    msgPubContainer_{};

  /// @brief map for instance id to Symmetric key wrapper cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId,
    std::unique_ptr<ara::crypto::cryp::SignEncodePrivateContextSkeletonImpl>>
    signEnPubContainer_{};

  /// @brief map for instance id to Symmetric key wrapper cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId,
    std::unique_ptr<ara::crypto::cryp::SignerPrivateContextSkeletonImpl>>
    signerPrivateContainer_{};

  /// @brief map for instance id to Symmetric key wrapper cipher instance mapping.
  ara::core::Map<CryptoServiceInstanceId,
    std::unique_ptr<ara::crypto::cryp::VerifierPublicContextSkeletonImpl>>
    verifyPubContainer_{};

  /// @brief mutex for instance Id.
  std::mutex mutex_instanceId_{};

  /// @brief Instance Id for crypto provider service.
  std::uint16_t serviceInstanceId_{ 0 };

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "CPSI", "CRYPTO PROVIDER Context", ara::log::LogLevel::kVerbose) };

  /// @brief Method to generate crypto service instance Id.
  CryptoServiceInstanceId GenerateCryptoServiceInstanceId() noexcept;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYPTOPROVIDER_SKELETON_IMPL_H_
