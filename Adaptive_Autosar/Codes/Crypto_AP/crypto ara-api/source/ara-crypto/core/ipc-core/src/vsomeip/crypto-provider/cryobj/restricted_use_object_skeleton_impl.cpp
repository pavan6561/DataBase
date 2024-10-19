// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : restricted_use_object_skeleton_impl.cpp
//
// Purpose     : restricted use object implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "restricted_use_object_skeleton_impl.h"

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
RestrictedUseObjectSkeletonImpl::RestrictedUseObjectSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::RestrictedUseObject::Uptrc restrictedUtr) noexcept :
    ara::crypto::ipccom::cryp::cryobj::restricteduseobject::skeleton::RestrictedUseObjectSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    restrictedUptr_{std::move(restrictedUtr)}
{
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryobj::restricteduseobject::RestrictedUseObject::GetAllowedUsageOutput>
    RestrictedUseObjectSkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::Restricted Used Object GetAllowedUsage()";
  ara::core::Future<GetAllowedUsageOutput>::PromiseType promise;
  GetAllowedUsageOutput allowdUsagesOutput;
  ara::crypto::cryp::RestrictedUseObject::Usage restrictedUsages{0};
  restrictedUsages = restrictedUptr_->GetAllowedUsage();
  allowdUsagesOutput.Usages = restrictedUsages;
  promise.set_value(allowdUsagesOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryobj::restricteduseobject::RestrictedUseObject::GetCryptoPrimitiveIdOutput>
    RestrictedUseObjectSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::Restricted Usedsss Object GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{restrictedUptr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryobj::restricteduseobject::RestrictedUseObject::GetObjectIdOutput>
    RestrictedUseObjectSkeletonImpl::GetObjectId()
{
  logger_.LogDebug() << "IPC-CORE::Restricted Used Object GetObjectId()";
  ara::core::Future<GetObjectIdOutput>::PromiseType promise;
  GetObjectIdOutput objectIdOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier const localCryptoObjIdentifier{
      restrictedUptr_->GetObjectId()};
  objectIdOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  objectIdOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(objectIdOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryobj::restricteduseobject::RestrictedUseObject::GetPayloadSizeOutput>
    RestrictedUseObjectSkeletonImpl::GetPayloadSize()
{
  logger_.LogDebug() << "IPC-CORE::Restricted Used Object GetPayloadSize()";
  ara::core::Future<GetPayloadSizeOutput>::PromiseType promise;
  GetPayloadSizeOutput payloadSizeeOutput;
  payloadSizeeOutput.size = restrictedUptr_->GetPayloadSize();
  promise.set_value(payloadSizeeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryobj::restricteduseobject::RestrictedUseObject::HasDependenceOutput>
    RestrictedUseObjectSkeletonImpl::HasDependence()
{
  logger_.LogDebug() << "IPC-CORE::Restricted Used Object HasDependence()";
  ara::core::Future<HasDependenceOutput>::PromiseType promise;
  HasDependenceOutput hasLocalOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier const localCryptoObjIdentifier{
      restrictedUptr_->HasDependence()};
  hasLocalOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  hasLocalOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(hasLocalOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryobj::restricteduseobject::RestrictedUseObject::IsExportableOutput>
    RestrictedUseObjectSkeletonImpl::IsExportable()
{
  logger_.LogDebug() << "IPC-CORE::Restricted Used Object IsExportable()";
  ara::core::Future<IsExportableOutput>::PromiseType promise;
  IsExportableOutput isexportableOutput;
  isexportableOutput.Bool = restrictedUptr_->IsExportable();
  promise.set_value(isexportableOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryobj::restricteduseobject::RestrictedUseObject::IsSessionOutput>
    RestrictedUseObjectSkeletonImpl::IsSession()
{
  logger_.LogDebug() << "IPC-CORE::Restricted Used Object IsSession()";
  ara::core::Future<IsSessionOutput>::PromiseType promise;
  IsSessionOutput issessionOutput;
  issessionOutput.Bool = restrictedUptr_->IsSession();
  promise.set_value(issessionOutput);
  return promise.get_future();
}

ara::core::Future<void> RestrictedUseObjectSkeletonImpl::Save(
    const CryptoServiceInstanceId& IOInterfaceInstanceID)
{
  static_cast<void>(IOInterfaceInstanceID);
  return ara::core::Future<void>();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara