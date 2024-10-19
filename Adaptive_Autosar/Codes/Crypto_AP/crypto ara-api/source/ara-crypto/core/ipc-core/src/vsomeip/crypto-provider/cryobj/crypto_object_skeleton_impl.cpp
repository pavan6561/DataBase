// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_object_skeleton_impl.cpp
//
// Purpose     : crypto object implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "crypto_object_skeleton_impl.h"

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
CryptoObjectSkeletonImpl::CryptoObjectSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::CryptoObject::Uptrc cryptoObjectUtr) noexcept :
    ara::crypto::ipccom::cryp::cryobj::cryptoobject::skeleton::CryptoObjectSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    cryptoObjectUptr_{std::move(cryptoObjectUtr)}
{
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::GetCryptoPrimitiveIdOutput>
    CryptoObjectSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::Crypto Object GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr primitiveUptr{cryptoObjectUptr_->GetCryptoPrimitiveId()};
  const ara::core::StringView primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::GetObjectIdOutput>
    CryptoObjectSkeletonImpl::GetObjectId()
{
  logger_.LogDebug() << "IPC-CORE::Crypto Object GetObjectId()";
  ara::core::Future<GetObjectIdOutput>::PromiseType promise;
  GetObjectIdOutput objectIdOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier localCryptoObjIdentifier{cryptoObjectUptr_->GetObjectId()};
  objectIdOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  objectIdOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(objectIdOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::GetPayloadSizeOutput>
    CryptoObjectSkeletonImpl::GetPayloadSize()
{
  logger_.LogDebug() << "IPC-CORE::Crypto Object GetPayloadSize()";
  ara::core::Future<GetPayloadSizeOutput>::PromiseType promise;
  GetPayloadSizeOutput payloadSizeeOutput;
  payloadSizeeOutput.size = cryptoObjectUptr_->GetPayloadSize();
  promise.set_value(payloadSizeeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::HasDependenceOutput>
    CryptoObjectSkeletonImpl::HasDependence()
{
  logger_.LogDebug() << "IPC-CORE::Crypto Object HasDependence()";
  ara::core::Future<HasDependenceOutput>::PromiseType promise;
  HasDependenceOutput hasLocalOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier localCryptoObjIdentifier{cryptoObjectUptr_->HasDependence()};
  hasLocalOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  hasLocalOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(hasLocalOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::IsExportableOutput>
    CryptoObjectSkeletonImpl::IsExportable()
{
  logger_.LogDebug() << "IPC-CORE::Crypto Object IsExportable()";
  ara::core::Future<IsExportableOutput>::PromiseType promise;
  IsExportableOutput isexportableOutput;
  isexportableOutput.Bool = cryptoObjectUptr_->IsExportable();
  promise.set_value(isexportableOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::IsSessionOutput>
    CryptoObjectSkeletonImpl::IsSession()
{
  logger_.LogDebug() << "IPC-CORE::Crypto Object IsSession()";
  ara::core::Future<IsSessionOutput>::PromiseType promise;
  IsSessionOutput issessionOutput;
  issessionOutput.Bool = cryptoObjectUptr_->IsSession();
  promise.set_value(issessionOutput);
  return promise.get_future();
}

ara::core::Future<void> CryptoObjectSkeletonImpl::Save(const CryptoServiceInstanceId& IOInterfaceInstanceID)
{
  static_cast<void>(IOInterfaceInstanceID);
  return ara::core::Future<void>();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara