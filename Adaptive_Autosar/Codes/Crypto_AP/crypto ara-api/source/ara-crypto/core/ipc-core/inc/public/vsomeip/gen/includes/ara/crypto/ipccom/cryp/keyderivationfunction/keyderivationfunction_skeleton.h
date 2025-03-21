// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : keyderivationfunction_skeleton.h
//
// Purpose     : Implementation of file keyderivationfunction_skeleton.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-03-14T12:58:58+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_KEYDERIVATIONFUNCTION_KEYDERIVATIONFUNCTION_SKELETON_H_
#define CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_KEYDERIVATIONFUNCTION_KEYDERIVATIONFUNCTION_SKELETON_H_

#include <ara/com/internal/skeleton/ara_skeleton_base.h>
#include <ara/com/illegal_state_exception.h>
#include "ara/crypto/ipccom/cryp/keyderivationfunction/keyderivationfunction_common.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace keyderivationfunction
{
namespace skeleton
{



/// @brief Class for KeyDerivationFunctionSkeleton.
class KeyDerivationFunctionSkeleton : public ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction, public ara::com::internal::skeleton::TypedServiceImplBase<KeyDerivationFunctionSkeleton> {
 public:
  /// @uptrace{SWS_CM_00130}
  KeyDerivationFunctionSkeleton(ara::com::InstanceIdentifier instance_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<KeyDerivationFunctionSkeleton>(instance_id, mode) {}
  /// @brief KeyDerivationFunctionSkeleton Destructor.

  /// @uptrace{SWS_CM_00152, 4ff7c3aec9fce6d3819bb320c152ce9c410cbe74}
  KeyDerivationFunctionSkeleton(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<KeyDerivationFunctionSkeleton>(std::move(instanceSpec), mode) {}

  /// @uptrace{SWS_CM_00153, 791854b84a101bd8d4a5e0e90bb3fb34c1fafd6f}
  KeyDerivationFunctionSkeleton(ara::com::InstanceIdentifierContainer instanceIDs, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<KeyDerivationFunctionSkeleton>(std::move(instanceIDs), mode) {}

  /// @brief Skeleton shall be move constructable.
  explicit KeyDerivationFunctionSkeleton(KeyDerivationFunctionSkeleton&&) = default;

  /// @brief Skeleton shall be move assignable.
  KeyDerivationFunctionSkeleton& operator=(KeyDerivationFunctionSkeleton&&) = default;

  /// @brief Skeleton shall not be copy constructable.
  explicit KeyDerivationFunctionSkeleton(const KeyDerivationFunctionSkeleton&) = delete;

  /// @brief Virtual function.
virtual ~KeyDerivationFunctionSkeleton() noexcept override;

  /// @brief Skeleton shall not be copy assignable.
  KeyDerivationFunctionSkeleton& operator=(const KeyDerivationFunctionSkeleton&) = delete;

  /// @brief Definition for OfferService.
  void OfferService();
  /// @brief Definition for AddSaltReadOnlyMem.
  virtual ara::core::Future<void> AddSaltReadOnlyMem(const CryptoReadOnlyMemRegion& salt) = 0;
  /// @brief Definition for AddSecretSalt.
  virtual ara::core::Future<void> AddSecretSalt(const CryptoServiceInstanceId& InstanceIdsecretSeed) = 0;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::ConfigIterationsOutput;
  /// @brief Definition for ConfigIterations.
  virtual ara::core::Future<ConfigIterationsOutput>ConfigIterations(const uint32& iterations)=0 ;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveKeyOutput;
  /// @brief Definition for DeriveKey.
  virtual ara::core::Future<DeriveKeyOutput>DeriveKey(const Boolean& isSession,const Boolean& isExportable)=0 ;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveSeedOutput;
  /// @brief Definition for DeriveSeed.
  virtual ara::core::Future<DeriveSeedOutput>DeriveSeed(const Boolean& isSession,const Boolean& isExportable)=0 ;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetCryptoPrimitiveIdOutput;
  /// @brief Definition for GetCryptoPrimitiveId.
  virtual ara::core::Future<GetCryptoPrimitiveIdOutput>GetCryptoPrimitiveId()=0 ;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetExtensionServiceOutput;
  /// @brief Definition for GetExtensionService.
  virtual ara::core::Future<GetExtensionServiceOutput>GetExtensionService()=0 ;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetKeyIdSizeOutput;
  /// @brief Definition for GetKeyIdSize.
  virtual ara::core::Future<GetKeyIdSizeOutput>GetKeyIdSize()=0 ;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetAlgIdOutput;
  /// @brief Definition for GetTargetAlgId.
  virtual ara::core::Future<GetTargetAlgIdOutput>GetTargetAlgId()=0 ;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetAllowedUsageOutput;
  /// @brief Definition for GetTargetAllowedUsage.
  virtual ara::core::Future<GetTargetAllowedUsageOutput>GetTargetAllowedUsage()=0 ;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetKeyBitLengthOutput;
  /// @brief Definition for GetTargetKeyBitLength.
  virtual ara::core::Future<GetTargetKeyBitLengthOutput>GetTargetKeyBitLength()=0 ;
  /// @brief Definition for Init.
  virtual ara::core::Future<void> Init(const CryptoReadOnlyMemRegion& targetKeyId,const CryptoAlgId& targetAlgId,const CryptoAllowedUsageFlags& allowedUsage,const CryptoReadOnlyMemRegion& ctxLabel) = 0;
  using ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::IsInitializedOutput;
  /// @brief Definition for IsInitialized.
  virtual ara::core::Future<IsInitializedOutput>IsInitialized()=0 ;
  /// @brief Definition for Reset.
  virtual ara::core::Future<void> Reset() = 0;
  /// @brief Definition for SetSourceKeyMaterial.
  virtual ara::core::Future<void> SetSourceKeyMaterial(const RestrictedUseObjectStructure& restrictedUseObjStruct) = 0;
};

} // namespace skeleton
} // namespace keyderivationfunction
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_KEYDERIVATIONFUNCTION_KEYDERIVATIONFUNCTION_SKELETON_H_

