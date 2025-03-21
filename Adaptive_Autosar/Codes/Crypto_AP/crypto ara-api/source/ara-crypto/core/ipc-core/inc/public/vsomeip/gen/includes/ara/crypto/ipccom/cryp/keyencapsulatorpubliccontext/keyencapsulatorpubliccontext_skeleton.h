// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : keyencapsulatorpubliccontext_skeleton.h
//
// Purpose     : Implementation of file keyencapsulatorpubliccontext_skeleton.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-03-14T12:58:58+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_KEYENCAPSULATORPUBLICCONTEXT_KEYENCAPSULATORPUBLICCONTEXT_SKELETON_H_
#define CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_KEYENCAPSULATORPUBLICCONTEXT_KEYENCAPSULATORPUBLICCONTEXT_SKELETON_H_

#include <ara/com/internal/skeleton/ara_skeleton_base.h>
#include <ara/com/illegal_state_exception.h>
#include "ara/crypto/ipccom/cryp/keyencapsulatorpubliccontext/keyencapsulatorpubliccontext_common.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace keyencapsulatorpubliccontext
{
namespace skeleton
{



/// @brief Class for KeyEncapsulatorPublicContextSkeleton.
class KeyEncapsulatorPublicContextSkeleton : public ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::KeyEncapsulatorPublicContext, public ara::com::internal::skeleton::TypedServiceImplBase<KeyEncapsulatorPublicContextSkeleton> {
 public:
  /// @uptrace{SWS_CM_00130}
  KeyEncapsulatorPublicContextSkeleton(ara::com::InstanceIdentifier instance_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<KeyEncapsulatorPublicContextSkeleton>(instance_id, mode) {}
  /// @brief KeyEncapsulatorPublicContextSkeleton Destructor.

  /// @uptrace{SWS_CM_00152, 4ff7c3aec9fce6d3819bb320c152ce9c410cbe74}
  KeyEncapsulatorPublicContextSkeleton(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<KeyEncapsulatorPublicContextSkeleton>(std::move(instanceSpec), mode) {}

  /// @uptrace{SWS_CM_00153, 791854b84a101bd8d4a5e0e90bb3fb34c1fafd6f}
  KeyEncapsulatorPublicContextSkeleton(ara::com::InstanceIdentifierContainer instanceIDs, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<KeyEncapsulatorPublicContextSkeleton>(std::move(instanceIDs), mode) {}

  /// @brief Skeleton shall be move constructable.
  explicit KeyEncapsulatorPublicContextSkeleton(KeyEncapsulatorPublicContextSkeleton&&) = default;

  /// @brief Skeleton shall be move assignable.
  KeyEncapsulatorPublicContextSkeleton& operator=(KeyEncapsulatorPublicContextSkeleton&&) = default;

  /// @brief Skeleton shall not be copy constructable.
  explicit KeyEncapsulatorPublicContextSkeleton(const KeyEncapsulatorPublicContextSkeleton&) = delete;

  /// @brief Virtual function.
virtual ~KeyEncapsulatorPublicContextSkeleton() noexcept override;

  /// @brief Skeleton shall not be copy assignable.
  KeyEncapsulatorPublicContextSkeleton& operator=(const KeyEncapsulatorPublicContextSkeleton&) = delete;

  /// @brief Definition for OfferService.
  void OfferService();
  /// @brief Definition for AddKeyingData.
  virtual ara::core::Future<void> AddKeyingData(const CryptoServiceInstanceId& restrictedUseObjInstanceId) = 0;
  using ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::KeyEncapsulatorPublicContext::EncapsulateOutput;
  /// @brief Definition for Encapsulate.
  virtual ara::core::Future<EncapsulateOutput>Encapsulate(const CryptoServiceInstanceId& kdfInstanceId,const CryptoAlgId& kekAlgId,const CryptoReadOnlyMemRegion& saltReadOnly,const CryptoReadOnlyMemRegion& ctxLabelReadOnly)=0 ;
  using ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::KeyEncapsulatorPublicContext::GetCryptoPrimitiveIdOutput;
  /// @brief Definition for GetCryptoPrimitiveId.
  virtual ara::core::Future<GetCryptoPrimitiveIdOutput>GetCryptoPrimitiveId()=0 ;
  using ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::KeyEncapsulatorPublicContext::GetEncapsulatedSizeOutput;
  /// @brief Definition for GetEncapsulatedSize.
  virtual ara::core::Future<GetEncapsulatedSizeOutput>GetEncapsulatedSize()=0 ;
  using ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::KeyEncapsulatorPublicContext::GetExtensionServiceOutput;
  /// @brief Definition for GetExtensionService.
  virtual ara::core::Future<GetExtensionServiceOutput>GetExtensionService()=0 ;
  using ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::KeyEncapsulatorPublicContext::GetKekEntropyOutput;
  /// @brief Definition for GetKekEntropy.
  virtual ara::core::Future<GetKekEntropyOutput>GetKekEntropy()=0 ;
  using ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::KeyEncapsulatorPublicContext::IsInitializedOutput;
  /// @brief Definition for IsInitialized.
  virtual ara::core::Future<IsInitializedOutput>IsInitialized()=0 ;
  /// @brief Definition for Reset.
  virtual ara::core::Future<void> Reset() = 0;
  /// @brief Definition for SetKey.
  virtual ara::core::Future<void> SetKey(const CryptoServiceInstanceId& publicKeyInstanceId) = 0;
};

} // namespace skeleton
} // namespace keyencapsulatorpubliccontext
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_KEYENCAPSULATORPUBLICCONTEXT_KEYENCAPSULATORPUBLICCONTEXT_SKELETON_H_

