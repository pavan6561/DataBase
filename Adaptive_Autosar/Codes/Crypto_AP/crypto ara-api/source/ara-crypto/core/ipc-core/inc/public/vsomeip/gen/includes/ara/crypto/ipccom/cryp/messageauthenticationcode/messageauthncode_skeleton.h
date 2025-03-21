// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : messageauthncode_skeleton.h
//
// Purpose     : Implementation of file messageauthncode_skeleton.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-03-14T12:58:58+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_MESSAGEAUTHENTICATIONCODE_MESSAGEAUTHNCODE_SKELETON_H_
#define CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_MESSAGEAUTHENTICATIONCODE_MESSAGEAUTHNCODE_SKELETON_H_

#include <ara/com/internal/skeleton/ara_skeleton_base.h>
#include <ara/com/illegal_state_exception.h>
#include "ara/crypto/ipccom/cryp/messageauthenticationcode/messageauthncode_common.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace messageauthenticationcode
{
namespace skeleton
{



/// @brief Class for MessageAuthnCodeSkeleton.
class MessageAuthnCodeSkeleton : public ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode, public ara::com::internal::skeleton::TypedServiceImplBase<MessageAuthnCodeSkeleton> {
 public:
  /// @uptrace{SWS_CM_00130}
  MessageAuthnCodeSkeleton(ara::com::InstanceIdentifier instance_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<MessageAuthnCodeSkeleton>(instance_id, mode) {}
  /// @brief MessageAuthnCodeSkeleton Destructor.

  /// @uptrace{SWS_CM_00152, 4ff7c3aec9fce6d3819bb320c152ce9c410cbe74}
  MessageAuthnCodeSkeleton(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<MessageAuthnCodeSkeleton>(std::move(instanceSpec), mode) {}

  /// @uptrace{SWS_CM_00153, 791854b84a101bd8d4a5e0e90bb3fb34c1fafd6f}
  MessageAuthnCodeSkeleton(ara::com::InstanceIdentifierContainer instanceIDs, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<MessageAuthnCodeSkeleton>(std::move(instanceIDs), mode) {}

  /// @brief Skeleton shall be move constructable.
  explicit MessageAuthnCodeSkeleton(MessageAuthnCodeSkeleton&&) = default;

  /// @brief Skeleton shall be move assignable.
  MessageAuthnCodeSkeleton& operator=(MessageAuthnCodeSkeleton&&) = default;

  /// @brief Skeleton shall not be copy constructable.
  explicit MessageAuthnCodeSkeleton(const MessageAuthnCodeSkeleton&) = delete;

  /// @brief Virtual function.
virtual ~MessageAuthnCodeSkeleton() noexcept override;

  /// @brief Skeleton shall not be copy assignable.
  MessageAuthnCodeSkeleton& operator=(const MessageAuthnCodeSkeleton&) = delete;

  /// @brief Definition for OfferService.
  void OfferService();
  using ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::CheckOutput;
  /// @brief Definition for Check.
  virtual ara::core::Future<CheckOutput>Check(const CryptoServiceInstanceId& instanceId)=0 ;
  using ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::FinishOutput;
  /// @brief Definition for Finish.
  virtual ara::core::Future<FinishOutput>Finish(const Boolean& makeSignatureObject)=0 ;
  using ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetCryptoPrimitiveIdOutput;
  /// @brief Definition for GetCryptoPrimitiveId.
  virtual ara::core::Future<GetCryptoPrimitiveIdOutput>GetCryptoPrimitiveId()=0 ;
  using ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetDigestOutput;
  /// @brief Definition for GetDigest.
  virtual ara::core::Future<GetDigestOutput>GetDigest(const uint64& offset)=0 ;
  using ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetDigestServiceOutput;
  /// @brief Definition for GetDigestService.
  virtual ara::core::Future<GetDigestServiceOutput>GetDigestService()=0 ;
  using ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::IsInitializedOutput;
  /// @brief Definition for IsInitialized.
  virtual ara::core::Future<IsInitializedOutput>IsInitialized()=0 ;
  /// @brief Definition for Reset.
  virtual ara::core::Future<void> Reset() = 0;
  /// @brief Definition for SetKey.
  virtual ara::core::Future<void> SetKey(const CryptoServiceInstanceId& symmetricKeyInstanceId,const CryptoTransformDataType& transform) = 0;
  /// @brief Definition for StartReadOnlyMem.
  virtual ara::core::Future<void> StartReadOnlyMem(const CryptoReadOnlyMemRegion& iv) = 0;
  /// @brief Definition for StartSecretSeed.
  virtual ara::core::Future<void> StartSecretSeed(const CryptoServiceInstanceId& secretSeedInstanceId) = 0;
  /// @brief Definition for Update.
  virtual ara::core::Future<void> Update(const uint8& in) = 0;
  /// @brief Definition for UpdateReadOnlyMem.
  virtual ara::core::Future<void> UpdateReadOnlyMem(const CryptoReadOnlyMemRegion& in) = 0;
  /// @brief Definition for UpdateRestrictedUseObject.
  virtual ara::core::Future<void> UpdateRestrictedUseObject(const RestrictedUseObjectStructure& restrictedUseObjStruct) = 0;
};

} // namespace skeleton
} // namespace messageauthenticationcode
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_MESSAGEAUTHENTICATIONCODE_MESSAGEAUTHNCODE_SKELETON_H_

