// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : public_key_skeleton_impl.cpp
//
// Purpose     : public key implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "public_key_skeleton_impl.h"

#include "hash_function_container.h"

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
PublicKeySkeletonImpl::PublicKeySkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::PublicKey::Uptrc publicKeyUtr) noexcept :
    ara::crypto::ipccom::cryp::cryobj::pubilckey::skeleton::PublicKeySkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    publicKeyUtr_{std::move(publicKeyUtr)}
{
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::CheckKeyOutput>
    PublicKeySkeletonImpl::CheckKey(const Boolean& strongCheck)
{
  logger_.LogDebug() << "IPC-CORE::PublicKey CheckKey()";
  ara::core::Future<CheckKeyOutput>::PromiseType promise;
  CheckKeyOutput keyOutput;
  static_cast<void>(strongCheck);
  // ToDO: Currently not supported by crypto daemon, to be considered in future release
  // keyOutput.Bool = publicKeyUtr_->CheckKey(strongCheck);
  promise.set_value(keyOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::ExportPubliclyOutput>
    PublicKeySkeletonImpl::ExportPublicly(const CryptoFormatId& FormatId)
{
  // ToDo: implementation containing Serializable part, this interface will be considered in future.
  ara::core::Future<ExportPubliclyOutput>::PromiseType promise;
  static_cast<void>(FormatId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::GetAllowedUsageOutput>
    PublicKeySkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::PublicKey GetAllowedUsage()";
  ara::core::Future<GetAllowedUsageOutput>::PromiseType promise;
  GetAllowedUsageOutput allowdUsagesOutput;
  ara::crypto::cryp::RestrictedUseObject::Usage KeyUsages{0};
  KeyUsages = publicKeyUtr_->GetAllowedUsage();
  allowdUsagesOutput.UsageFlag = KeyUsages;
  promise.set_value(allowdUsagesOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::GetCryptoPrimitiveIdOutput>
    PublicKeySkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::PublicKey GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{publicKeyUtr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::GetObjectIdOutput>
    PublicKeySkeletonImpl::GetObjectId()
{
  logger_.LogDebug() << "IPC-CORE::PublicKey GetObjectId()";
  ara::core::Future<GetObjectIdOutput>::PromiseType promise;
  GetObjectIdOutput objectIdOutput;
  const ara::crypto::cryp::CryptoObject::COIdentifier localCryptoObjIdentifier{publicKeyUtr_->GetObjectId()};
  objectIdOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  objectIdOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(objectIdOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::GetPayloadSizeOutput>
    PublicKeySkeletonImpl::GetPayloadSize()
{
  logger_.LogDebug() << "IPC-CORE::PublicKey GetPayloadSize()";
  ara::core::Future<GetPayloadSizeOutput>::PromiseType promise;
  GetPayloadSizeOutput payloadSizeOutput;
  payloadSizeOutput.PayloadSize = publicKeyUtr_->GetPayloadSize();
  promise.set_value(payloadSizeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::HasDependenceOutput>
    PublicKeySkeletonImpl::HasDependence()
{
  logger_.LogDebug() << "IPC-CORE::PublicKey HasDependence()";
  ara::core::Future<HasDependenceOutput>::PromiseType promise;
  HasDependenceOutput hasLocalOutput;
  const ara::crypto::cryp::CryptoObject::COIdentifier localCryptoObjIdentifier{
      publicKeyUtr_->HasDependence()};
  hasLocalOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  hasLocalOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(hasLocalOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::HashPublicKeyOutput>
    PublicKeySkeletonImpl::HashPublicKey(const CryptoServiceInstanceId& HashFunctionCtxInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::PublicKey HashPublicKey()";
  std::shared_ptr<ara::crypto::common::crypdatastore::HashFunctionContainer> hashStorageContainer{
      ara::crypto::common::crypdatastore::HashFunctionContainer::GetInstance()};
  std::unique_ptr<HashFunctionSkeletonImpl>& hashUptr{
      hashStorageContainer->GetObjectFromContanier(HashFunctionCtxInstanceId)};
  HashFunctionCtx::Uptr& hashFunctionCtxPtr{hashUptr->GetHashFunctionUptr()};
  ara::core::Future<HashPublicKeyOutput>::PromiseType promise;
  const ara::core::Result<ara::core::Vector<ara::core::Byte>> hashOutput{
      publicKeyUtr_->HashPublicKey(*hashFunctionCtxPtr)};
  if (hashOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::PublicKey HashPublicKey()";
    ara::core::Vector<ara::core::Byte> tempVtr{hashOutput.Value()};
    HashPublicKeyOutput vtrUint8Result;
    for_each(tempVtr.begin(), tempVtr.end(), [&vtrUint8Result](ara::core::Byte i) {
      vtrUint8Result.VectorByte.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(vtrUint8Result);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::PublicKey HashPublicKey() Error: " << hashOutput.Error().Message();
    if (hashOutput.Error() == SecurityErrorDomain::Errc::kInsufficientCapacity)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompleteArgState);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::IsExportableOutput>
    PublicKeySkeletonImpl::IsExportable()
{
  logger_.LogDebug() << "IPC-CORE::PublicKey IsExportable()";
  ara::core::Future<IsExportableOutput>::PromiseType promise;
  IsExportableOutput isexportableOutput;
  isexportableOutput.Bool = publicKeyUtr_->IsExportable();
  promise.set_value(isexportableOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::IsSessionOutput>
    PublicKeySkeletonImpl::IsSession()
{
  logger_.LogDebug() << "IPC-CORE::PublicKey IsSession()";
  ara::core::Future<IsSessionOutput>::PromiseType promise;
  IsSessionOutput issessionOutput;
  issessionOutput.Bool = publicKeyUtr_->IsSession();
  promise.set_value(issessionOutput);
  return promise.get_future();
}

ara::core::Future<void> PublicKeySkeletonImpl::Save(const CryptoServiceInstanceId& IOInterfaceInstanceId)
{
  // ToDO: IOInterface to be implemented in future releases
  static_cast<void>(IOInterfaceInstanceId);
  return ara::core::Future<void>();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara