// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_key_skeleton_impl.cpp
//
// Purpose     : symmetric key implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "symmetric_key_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for crypto objects.
namespace cryobj
{
SymmetrickeySkeletonImpl::SymmetrickeySkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::SymmetricKey::Uptrc symmetricUtr) noexcept :
    ara::crypto::ipccom::cryp::cryobj::symmetrickey::skeleton::SymmetricKeySkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    symmetricUtr_{std::move(symmetricUtr)}
{
}

SymmetrickeySkeletonImpl::SymmetrickeySkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::RestrictedUseObject::Uptrc restrictedUtr) noexcept :
    ara::crypto::ipccom::cryp::cryobj::symmetrickey::skeleton::SymmetricKeySkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    restrictedUptr_{std::move(restrictedUtr)}
{
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetAllowedUsageOutput>
    SymmetrickeySkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::SymmetricKey Object GetAllowedUsage()";
  ara::core::Future<GetAllowedUsageOutput>::PromiseType promise;
  GetAllowedUsageOutput allowdUsagesOutput;
  ara::crypto::cryp::RestrictedUseObject::Usage symmtericKeyUsages{0};
  symmtericKeyUsages = symmetricUtr_->GetAllowedUsage();
  allowdUsagesOutput.Usages = symmtericKeyUsages;
  promise.set_value(allowdUsagesOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetCryptoPrimitiveIdOutput>
    SymmetrickeySkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::SymmetricKey Object GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{symmetricUtr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetObjectIdOutput>
    SymmetrickeySkeletonImpl::GetObjectId()
{
  logger_.LogDebug() << "IPC-CORE::SymmetricKey Object GetObjectId()";
  ara::core::Future<GetObjectIdOutput>::PromiseType promise;
  GetObjectIdOutput objectIdOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier const localCryptoObjIdentifier{symmetricUtr_->GetObjectId()};
  objectIdOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  objectIdOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(objectIdOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetPayloadSizeOutput>
    SymmetrickeySkeletonImpl::GetPayloadSize()
{
  logger_.LogDebug() << "IPC-CORE::SymmetricKey Object GetPayloadSize()";
  ara::core::Future<GetPayloadSizeOutput>::PromiseType promise;
  GetPayloadSizeOutput payloadSizeeOutput;
  payloadSizeeOutput.size = symmetricUtr_->GetPayloadSize();
  promise.set_value(payloadSizeeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::HasDependenceOutput>
    SymmetrickeySkeletonImpl::HasDependence()
{
  logger_.LogDebug() << "IPC-CORE::SymmetricKey Object HasDependence()";
  ara::core::Future<HasDependenceOutput>::PromiseType promise;
  HasDependenceOutput hasLocalOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier const localCryptoObjIdentifier{
      symmetricUtr_->HasDependence()};
  hasLocalOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  hasLocalOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(hasLocalOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::IsExportableOutput>
    SymmetrickeySkeletonImpl::IsExportable()
{
  logger_.LogDebug() << "IPC-CORE::SymmetricKey Object IsExportable()";
  ara::core::Future<IsExportableOutput>::PromiseType promise;
  IsExportableOutput isexportableOutput;
  isexportableOutput.Bool = symmetricUtr_->IsExportable();
  promise.set_value(isexportableOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::IsSessionOutput>
    SymmetrickeySkeletonImpl::IsSession()
{
  logger_.LogDebug() << "IPC-CORE::SymmetricKey Object IsSession()";
  ara::core::Future<IsSessionOutput>::PromiseType promise;
  IsSessionOutput issessionOutput;
  issessionOutput.Bool = symmetricUtr_->IsSession();
  promise.set_value(issessionOutput);
  return promise.get_future();
}

ara::core::Future<void> SymmetrickeySkeletonImpl::Save(const CryptoServiceInstanceId& IOInterfaceInstanceID)
{
  static_cast<void>(IOInterfaceInstanceID);
  return ara::core::Future<void>();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara