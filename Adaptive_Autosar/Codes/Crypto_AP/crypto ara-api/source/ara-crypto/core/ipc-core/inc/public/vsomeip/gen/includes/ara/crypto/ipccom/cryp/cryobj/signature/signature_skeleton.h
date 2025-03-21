// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature_skeleton.h
//
// Purpose     : Implementation of file signature_skeleton.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_CRYOBJ_SIGNATURE_SIGNATURE_SKELETON_H_
#define CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_CRYOBJ_SIGNATURE_SIGNATURE_SKELETON_H_

#include <ara/com/internal/skeleton/ara_skeleton_base.h>
#include <ara/com/illegal_state_exception.h>
#include "ara/crypto/ipccom/cryp/cryobj/signature/signature_common.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace cryobj
{
namespace signature
{
namespace skeleton
{



/// @brief Class for SignatureSkeleton.
class SignatureSkeleton : public ara::crypto::ipccom::cryp::cryobj::signature::Signature, public ara::com::internal::skeleton::TypedServiceImplBase<SignatureSkeleton> {
 public:
  /// @uptrace{SWS_CM_00130}
  SignatureSkeleton(ara::com::InstanceIdentifier instance_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<SignatureSkeleton>(instance_id, mode) {}
  /// @brief SignatureSkeleton Destructor.

  /// @uptrace{SWS_CM_00152, 4ff7c3aec9fce6d3819bb320c152ce9c410cbe74}
  SignatureSkeleton(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<SignatureSkeleton>(std::move(instanceSpec), mode) {}

  /// @uptrace{SWS_CM_00153, 791854b84a101bd8d4a5e0e90bb3fb34c1fafd6f}
  SignatureSkeleton(ara::com::InstanceIdentifierContainer instanceIDs, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<SignatureSkeleton>(std::move(instanceIDs), mode) {}

  /// @brief Skeleton shall be move constructable.
  explicit SignatureSkeleton(SignatureSkeleton&&) = default;

  /// @brief Skeleton shall be move assignable.
  SignatureSkeleton& operator=(SignatureSkeleton&&) = default;

  /// @brief Skeleton shall not be copy constructable.
  explicit SignatureSkeleton(const SignatureSkeleton&) = delete;

  /// @brief Virtual function.
virtual ~SignatureSkeleton() noexcept override;

  /// @brief Skeleton shall not be copy assignable.
  SignatureSkeleton& operator=(const SignatureSkeleton&) = delete;

  /// @brief Definition for OfferService.
  void OfferService();
  using ara::crypto::ipccom::cryp::cryobj::signature::Signature::ExportPubliclyOutput;
  /// @brief Definition for ExportPublicly.
  virtual ara::core::Future<ExportPubliclyOutput>ExportPublicly(const CryptoFormatId& FormatId)=0 ;
  using ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetCryptoPrimitiveIdOutput;
  /// @brief Definition for GetCryptoPrimitiveId.
  virtual ara::core::Future<GetCryptoPrimitiveIdOutput>GetCryptoPrimitiveId()=0 ;
  using ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetHashAlgIdOutput;
  /// @brief Definition for GetHashAlgId.
  virtual ara::core::Future<GetHashAlgIdOutput>GetHashAlgId()=0 ;
  using ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetObjectIdOutput;
  /// @brief Definition for GetObjectId.
  virtual ara::core::Future<GetObjectIdOutput>GetObjectId()=0 ;
  using ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetPayloadSizeOutput;
  /// @brief Definition for GetPayloadSize.
  virtual ara::core::Future<GetPayloadSizeOutput>GetPayloadSize()=0 ;
  using ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetRequiredHashSizeOutput;
  /// @brief Definition for GetRequiredHashSize.
  virtual ara::core::Future<GetRequiredHashSizeOutput>GetRequiredHashSize()=0 ;
  using ara::crypto::ipccom::cryp::cryobj::signature::Signature::HasDependenceOutput;
  /// @brief Definition for HasDependence.
  virtual ara::core::Future<HasDependenceOutput>HasDependence()=0 ;
  using ara::crypto::ipccom::cryp::cryobj::signature::Signature::IsExportableOutput;
  /// @brief Definition for IsExportable.
  virtual ara::core::Future<IsExportableOutput>IsExportable()=0 ;
  using ara::crypto::ipccom::cryp::cryobj::signature::Signature::IsSessionOutput;
  /// @brief Definition for IsSession.
  virtual ara::core::Future<IsSessionOutput>IsSession()=0 ;
  /// @brief Definition for Save.
  virtual ara::core::Future<void> Save(const CryptoServiceInstanceId& IOInterfaceInstanceID) = 0;
};

} // namespace skeleton
} // namespace signature
} // namespace cryobj
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_CRYOBJ_SIGNATURE_SIGNATURE_SKELETON_H_

