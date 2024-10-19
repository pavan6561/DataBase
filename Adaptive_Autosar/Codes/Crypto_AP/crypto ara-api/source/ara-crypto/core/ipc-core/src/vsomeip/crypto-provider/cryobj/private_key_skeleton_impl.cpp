// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : private_key_skeleton_impl.cpp
//
// Purpose     : private key implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "private_key_skeleton_impl.h"

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
PrivateKeySkeletonImpl::PrivateKeySkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::PrivateKey::Uptrc privateKeyUtr) noexcept :
    ara::crypto::ipccom::cryp::cryobj::privatekey::skeleton::PrivateKeySkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    privateKeyUtr_{std::move(privateKeyUtr)}
{
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetAllowedUsageOutput>
    PrivateKeySkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::PrivateKey GetAllowedUsage()";
  ara::core::Future<GetAllowedUsageOutput>::PromiseType promise;
  GetAllowedUsageOutput allowdUsagesOutput;
  ara::crypto::cryp::RestrictedUseObject::Usage KeyUsages{0};
  KeyUsages = privateKeyUtr_->GetAllowedUsage();
  allowdUsagesOutput.UsageFlag = KeyUsages;
  promise.set_value(allowdUsagesOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetCryptoPrimitiveIdOutput>
    PrivateKeySkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::PrivateKey GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{privateKeyUtr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetObjectIdOutput>
    PrivateKeySkeletonImpl::GetObjectId()
{
  logger_.LogDebug() << "IPC-CORE::PrivateKey GetObjectId()";
  ara::core::Future<GetObjectIdOutput>::PromiseType promise;
  GetObjectIdOutput objectIdOutput;
  const ara::crypto::cryp::CryptoObject::COIdentifier localCryptoObjIdentifier{privateKeyUtr_->GetObjectId()};
  objectIdOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  objectIdOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(objectIdOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetPayloadSizeOutput>
    PrivateKeySkeletonImpl::GetPayloadSize()
{
  logger_.LogDebug() << "IPC-CORE::PrivateKey GetPayloadSize()";
  ara::core::Future<GetPayloadSizeOutput>::PromiseType promise;
  GetPayloadSizeOutput payloadSizeOutput;
  payloadSizeOutput.PayloadSize = privateKeyUtr_->GetPayloadSize();
  promise.set_value(payloadSizeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetPublicKeyOutput>
    PrivateKeySkeletonImpl::GetPublicKey()
{
  logger_.LogDebug() << "IPC-CORE::PrivateKey GetPublicKey()";
  ara::core::Future<GetPublicKeyOutput>::PromiseType promise;
  ara::core::Result<PublicKey::Uptrc> publicKeyUptrc{privateKeyUtr_->GetPublicKey()};
  if (publicKeyUptrc.HasValue())
  {
    ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetPublicKeyOutput publicKeyOutput;
    std::shared_ptr<ara::crypto::common::datastorage::PublicKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::PublicKeyContainer::GetInstance()};
    // create unique crypto service instance Id from skeleton data storage class for crypto objects
    CryptoServiceInstanceId publicKeylocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl> localPublicKeyUtr{
        std::make_unique<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl>(
            publicKeylocalInstanceId, std::move(publicKeyUptrc).Value())};
    localPublicKeyUtr->OfferService();
    dataStorageContainer->StoreObjectToContanier(publicKeylocalInstanceId, std::move(localPublicKeyUtr));
    publicKeyOutput.PublicKeyUptrInstanceId = publicKeylocalInstanceId;
    promise.set_value(publicKeyOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider GetPublicKey() Error: "
                       << publicKeyUptrc.Error().Message();
    if (publicKeyUptrc.Error() == SecurityErrorDomain::Errc::kUnknownIdentifier)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::HasDependenceOutput>
    PrivateKeySkeletonImpl::HasDependence()
{
  logger_.LogDebug() << "IPC-CORE::PrivateKey HasDependence()";
  ara::core::Future<HasDependenceOutput>::PromiseType promise;
  HasDependenceOutput hasLocalOutput;
  const ara::crypto::cryp::CryptoObject::COIdentifier localCryptoObjIdentifier{
      privateKeyUtr_->HasDependence()};
  hasLocalOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  hasLocalOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(hasLocalOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::IsExportableOutput>
    PrivateKeySkeletonImpl::IsExportable()
{
  logger_.LogDebug() << "IPC-CORE::PrivateKey IsExportable()";
  ara::core::Future<IsExportableOutput>::PromiseType promise;
  IsExportableOutput isexportableOutput;
  isexportableOutput.Bool = privateKeyUtr_->IsExportable();
  promise.set_value(isexportableOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::IsSessionOutput>
    PrivateKeySkeletonImpl::IsSession()
{
  logger_.LogDebug() << "IPC-CORE::PrivateKey IsSession()";
  ara::core::Future<IsSessionOutput>::PromiseType promise;
  IsSessionOutput issessionOutput;
  issessionOutput.Bool = privateKeyUtr_->IsSession();
  promise.set_value(issessionOutput);
  return promise.get_future();
}

ara::core::Future<void> PrivateKeySkeletonImpl::Save(const CryptoServiceInstanceId& IOInterfaceInstanceId)
{
  static_cast<void>(IOInterfaceInstanceId);
  return ara::core::Future<void>();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara