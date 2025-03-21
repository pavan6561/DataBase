// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sigencodeprivatecontext_skeleton.h
//
// Purpose     : Implementation of file sigencodeprivatecontext_skeleton.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-03-14T12:58:58+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_SIGNENCODEPRIVATECONTEXT_SIGENCODEPRIVATECONTEXT_SKELETON_H_
#define CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_SIGNENCODEPRIVATECONTEXT_SIGENCODEPRIVATECONTEXT_SKELETON_H_

#include <ara/com/internal/skeleton/ara_skeleton_base.h>
#include <ara/com/illegal_state_exception.h>
#include "ara/crypto/ipccom/cryp/signencodeprivatecontext/sigencodeprivatecontext_common.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace signencodeprivatecontext
{
namespace skeleton
{



/// @brief Class for SigEncodePrivateContextSkeleton.
class SigEncodePrivateContextSkeleton : public ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext, public ara::com::internal::skeleton::TypedServiceImplBase<SigEncodePrivateContextSkeleton> {
 public:
  /// @uptrace{SWS_CM_00130}
  SigEncodePrivateContextSkeleton(ara::com::InstanceIdentifier instance_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<SigEncodePrivateContextSkeleton>(instance_id, mode) {}
  /// @brief SigEncodePrivateContextSkeleton Destructor.

  /// @uptrace{SWS_CM_00152, 4ff7c3aec9fce6d3819bb320c152ce9c410cbe74}
  SigEncodePrivateContextSkeleton(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<SigEncodePrivateContextSkeleton>(std::move(instanceSpec), mode) {}

  /// @uptrace{SWS_CM_00153, 791854b84a101bd8d4a5e0e90bb3fb34c1fafd6f}
  SigEncodePrivateContextSkeleton(ara::com::InstanceIdentifierContainer instanceIDs, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<SigEncodePrivateContextSkeleton>(std::move(instanceIDs), mode) {}

  /// @brief Skeleton shall be move constructable.
  explicit SigEncodePrivateContextSkeleton(SigEncodePrivateContextSkeleton&&) = default;

  /// @brief Skeleton shall be move assignable.
  SigEncodePrivateContextSkeleton& operator=(SigEncodePrivateContextSkeleton&&) = default;

  /// @brief Skeleton shall not be copy constructable.
  explicit SigEncodePrivateContextSkeleton(const SigEncodePrivateContextSkeleton&) = delete;

  /// @brief Virtual function.
virtual ~SigEncodePrivateContextSkeleton() noexcept override;

  /// @brief Skeleton shall not be copy assignable.
  SigEncodePrivateContextSkeleton& operator=(const SigEncodePrivateContextSkeleton&) = delete;

  /// @brief Definition for OfferService.
  void OfferService();
  using ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetCryptoPrimitiveIdOutput;
  /// @brief Definition for GetCryptoPrimitiveId.
  virtual ara::core::Future<GetCryptoPrimitiveIdOutput>GetCryptoPrimitiveId()=0 ;
  using ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetExtensionServiceOutput;
  /// @brief Definition for GetExtensionService.
  virtual ara::core::Future<GetExtensionServiceOutput>GetExtensionService()=0 ;
  using ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetMaxInputSizeOutput;
  /// @brief Definition for GetMaxInputSize.
  virtual ara::core::Future<GetMaxInputSizeOutput>GetMaxInputSize(const Boolean& suppressPadding)=0 ;
  using ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetMaxOutputSizeOutput;
  /// @brief Definition for GetMaxOutputSize.
  virtual ara::core::Future<GetMaxOutputSizeOutput>GetMaxOutputSize(const Boolean& suppressPadding)=0 ;
  using ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::IsInitializedOutput;
  /// @brief Definition for IsInitialized.
  virtual ara::core::Future<IsInitializedOutput>IsInitialized()=0 ;
  /// @brief Definition for Reset.
  virtual ara::core::Future<void> Reset() = 0;
  /// @brief Definition for SetKey.
  virtual ara::core::Future<void> SetKey(const CryptoServiceInstanceId& privateKeyInstanceId) = 0;
  using ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::SignAndEncodeOutput;
  /// @brief Definition for SignAndEncode.
  virtual ara::core::Future<SignAndEncodeOutput>SignAndEncode(const CryptoReadOnlyMemRegion& inputReadOnly)=0 ;
};

} // namespace skeleton
} // namespace signencodeprivatecontext
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_SIGNENCODEPRIVATECONTEXT_SIGENCODEPRIVATECONTEXT_SKELETON_H_

