// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : message_authn_code_skeleton_impl.h
//
// Purpose     : Message authentication code skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_MESSAGE_AUTHN_CODE_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_MESSAGE_AUTHN_CODE_SKELETON_IMPL_H_

#include <ara/core/span.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/message_authn_code_ctx.h"
#include "messageauthncode_skeleton.h"
#include "secret_seed_container.h"
#include "signature_container.h"
#include "symmetrickey_container.h" 
#include "digest_service_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Class definition for MAC skeleton.
class MessageAuthnCodeSkeletonImpl
  : public ara::crypto::ipccom::cryp::messageauthenticationcode::skeleton::MessageAuthnCodeSkeleton
{
 public:
  /// @brief Constructor for MessageAuthnCodeSkeletonImpl class.
  MessageAuthnCodeSkeletonImpl(
    CryptoServiceInstanceId const instanceId, MessageAuthnCodeCtx::Uptr macUptr) noexcept;

  /// @brief Skeleton implementation for Check method.
  /// @param[in] instanceId instance Id of the signatrue object.
  ara::core::Future<ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::CheckOutput>
    Check(const CryptoServiceInstanceId& instanceId) override;

  /// @brief Skeleton implementation for Finish method.
  /// @param[in] makeSignatureObject flag to produce signature object at the end of MAC operatio.
  ara::core::Future<ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::FinishOutput>
    Finish(const Boolean& makeSignatureObject) override;

  /// @brief Skeleton implementation for GetDigest method.
  /// @param[in] offset  position of the first byte of digest that should be placed to the output buffer
  ara::core::Future<ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetDigestOutput>
    GetDigest(const uint64& offset) override;

  /// @brief Skeleton implementation for GetDigestService method.
  ara::core::Future<
    ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetDigestServiceOutput>
    GetDigestService() override;

  /// @brief Skeleton implementation for Reset method.
  ara::core::Future<void> Reset() override;

  /// @brief Skeleton implementation for SetKey method.
  /// @param[in] symmetricKeyInstanceId instance id of the symmetric key object.
  /// @param[in] transform the "direction" indicator: deploy the key for direct transformation (if @c
  /// true) or for reverse one (if @c false)
  ara::core::Future<void> SetKey(
    const CryptoServiceInstanceId& symmetricKeyInstanceId, const CryptoTransformDataType& transform) override;

  /// @brief Skeleton implementation for Start method with ReadOnlyMemRegion.
  /// @param iv value of the initialization vector.
  ara::core::Future<void> StartReadOnlyMem(const CryptoReadOnlyMemRegion& iv) override;

  /// @brief Skeleton implementation for Start method with SecretSeed.
  /// @param secretSeedInstanceId instance Id of the SecretSeed Object.
  ara::core::Future<void> StartSecretSeed(const CryptoServiceInstanceId& secretSeedInstanceId) override;

  /// @brief Skeleton implementation for Update method with uint8_t.
  /// @param[in] in data for the calculation of MAC.
  ara::core::Future<void> Update(const uint8& in) override;

  /// @brief Skeleton implementation for Update method with ReadOnlyMemRegion.
  /// @param[in] in buffer containing data for MAC calculation.
  ara::core::Future<void> UpdateReadOnlyMem(const CryptoReadOnlyMemRegion& in) override;

  /// @brief Skeleton implementation for Update method with RestrictedUseObject.
  /// @param[in] restrictedObjectInstanceId instanceId for the resetricted used object.
  ara::core::Future<void> UpdateRestrictedUseObject(
    const RestrictedUseObjectStructure& restrictedUseObjStruct) override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<
    ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

 private:
  /// @brief Unique pointer to MAC.
  MessageAuthnCodeCtx::Uptr macCtx_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "MACS", "Message Authn Code Context", ara::log::LogLevel::kVerbose) };

  /// @brief mutex for instance Id.
  std::mutex mutex_instanceId_;

  /// @brief Instance Id for crypto provider service.
  std::uint16_t serviceInstanceId_{ 0 };

  /// @brief Method to generate crypto service instance Id.
  CryptoServiceInstanceId GenerateCryptoServiceInstanceId() noexcept;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_MESSAGE_AUTHN_CODE_SKELETON_IMPL_H_
