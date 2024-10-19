// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : secret_seed_skeleton_impl.cpp
//
// Purpose     : secret seed implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "secret_seed_skeleton_impl.h"

#include "secret_seed_container.h"

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
SecretSeedSkeletonImpl::SecretSeedSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::SecretSeed::Uptrc secretSeedConstUtr) noexcept :
    ara::crypto::ipccom::cryp::cryobj::secretseed::skeleton::SecretSeedSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    secretSeedConstUptr_{std::move(secretSeedConstUtr)}, isConstUptr_{true}
{
}

SecretSeedSkeletonImpl::SecretSeedSkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::SecretSeed::Uptr secretSeedUtr) noexcept :
    ara::crypto::ipccom::cryp::cryobj::secretseed::skeleton::SecretSeedSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    secretSeedUptr_{std::move(secretSeedUtr)}, isConstUptr_{false}
{
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::CloneOutput>
    SecretSeedSkeletonImpl::Clone(const CryptoReadOnlyMemRegion& xorDelta)
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object Clone()";
  ara::core::Future<CloneOutput>::PromiseType promise;
  ReadOnlyMemRegion const delta{xorDelta};
  ara::crypto::cryp::SecretSeed::Uptr seedUptr;
  if (isConstUptr_ == true)
  {
    ara::core::Result<ara::crypto::cryp::SecretSeed::Uptr> seedResult = secretSeedConstUptr_->Clone(delta);
    seedUptr = std::move(seedResult).Value();
  }
  else
  {
    ara::core::Result<ara::crypto::cryp::SecretSeed::Uptr> seedResult = secretSeedUptr_->Clone(delta);
    seedUptr = std::move(seedResult).Value();
  }
  ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::CloneOutput seedOutput;
  std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
  CryptoServiceInstanceId secretSeedlocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl> localSecretSeedUtr{
      std::make_unique<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>(
          secretSeedlocalInstanceId, std::move(seedUptr))};
  localSecretSeedUtr->OfferService();
  dataStorageContainer->StoreObjectToContanier(secretSeedlocalInstanceId, std::move(localSecretSeedUtr));
  seedOutput.SecretSeedUptr = secretSeedlocalInstanceId;
  promise.set_value(seedOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::GetAllowedUsageOutput>
    SecretSeedSkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object GetAllowedUsage()";
  ara::core::Future<GetAllowedUsageOutput>::PromiseType promise;
  GetAllowedUsageOutput allowdUsagesOutput;
  ara::crypto::cryp::RestrictedUseObject::Usage secretSeedUsages{0};
  if (isConstUptr_ == true)
  {
    secretSeedUsages = secretSeedConstUptr_->GetAllowedUsage();
  }
  else
  {
    secretSeedUsages = secretSeedUptr_->GetAllowedUsage();
  }
  allowdUsagesOutput.Usages = secretSeedUsages;
  promise.set_value(allowdUsagesOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::GetCryptoPrimitiveIdOutput>
    SecretSeedSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr primitiveUptr;
  if (isConstUptr_ == true)
  {
    primitiveUptr = secretSeedConstUptr_->GetCryptoPrimitiveId();
  }
  else
  {
    primitiveUptr = secretSeedUptr_->GetCryptoPrimitiveId();
  }
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::GetObjectIdOutput>
    SecretSeedSkeletonImpl::GetObjectId()
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object GetObjectId()";
  ara::core::Future<GetObjectIdOutput>::PromiseType promise;
  GetObjectIdOutput objectIdOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier localCryptoObjIdentifier;
  if (isConstUptr_ == true)
  {
    localCryptoObjIdentifier = secretSeedConstUptr_->GetObjectId();
  }
  else
  {
    localCryptoObjIdentifier = secretSeedUptr_->GetObjectId();
  }
  objectIdOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  objectIdOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(objectIdOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::GetPayloadSizeOutput>
    SecretSeedSkeletonImpl::GetPayloadSize()
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object GetPayloadSize()";
  ara::core::Future<GetPayloadSizeOutput>::PromiseType promise;
  GetPayloadSizeOutput payloadSizeOutput;
  if (isConstUptr_ == true)
  {
    payloadSizeOutput.Size = secretSeedConstUptr_->GetPayloadSize();
  }
  else
  {
    payloadSizeOutput.Size = secretSeedUptr_->GetPayloadSize();
  }
  promise.set_value(payloadSizeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::HasDependenceOutput>
    SecretSeedSkeletonImpl::HasDependence()
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object HasDependence()";
  ara::core::Future<HasDependenceOutput>::PromiseType promise;
  HasDependenceOutput hasLocalOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier localCryptoObjIdentifier;
  if (isConstUptr_ == true)
  {
    localCryptoObjIdentifier = secretSeedConstUptr_->HasDependence();
  }
  else
  {
    localCryptoObjIdentifier = secretSeedUptr_->HasDependence();
  }
  hasLocalOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  hasLocalOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(hasLocalOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::IsExportableOutput>
    SecretSeedSkeletonImpl::IsExportable()
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object IsExportable()";
  ara::core::Future<IsExportableOutput>::PromiseType promise;
  IsExportableOutput isexportableOutput;
  if (isConstUptr_ == true)
  {
    isexportableOutput.Bool = secretSeedConstUptr_->IsExportable();
  }
  else
  {
    isexportableOutput.Bool = secretSeedUptr_->IsExportable();
  }
  promise.set_value(isexportableOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::IsSessionOutput>
    SecretSeedSkeletonImpl::IsSession()
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object IsSession()";
  ara::core::Future<IsSessionOutput>::PromiseType promise;
  IsSessionOutput issessionOutput;
  if (isConstUptr_ == true)
  {
    issessionOutput.Bool = secretSeedConstUptr_->IsSession();
  }
  else
  {
    issessionOutput.Bool = secretSeedUptr_->IsSession();
  }
  promise.set_value(issessionOutput);
  return promise.get_future();
}

ara::core::Future<void> SecretSeedSkeletonImpl::Jump(const Int64& Steps)
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object Jump()";
  ara::core::Future<void>::PromiseType promise;
  static_cast<void>(secretSeedUptr_->Jump(Steps));
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> SecretSeedSkeletonImpl::JumpFrom(
    const CryptoServiceInstanceId& SecretSeedInstanceId, const Int64& steps)
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object JumpFrom()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
  logger_.LogInfo() << " IPC-CORE::SecretSeed Object Crypto ServiceInstanceId : " << SecretSeedInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
      dataStorageContainer->GetObjectFromContanier(SecretSeedInstanceId)};
  auto& secretSeedUptr{secretSeedSkeletonObj->GetSecretSeedConstUptrInstance()};
  ara::core::Result<void> jmpResult = secretSeedUptr_->JumpFrom(*secretSeedUptr, steps);
  if (jmpResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SecretSeed Object JumpFrom() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SecretSeed Object JumpFrom() Error: " << jmpResult.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(jmpResult.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<void> SecretSeedSkeletonImpl::Next()
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object Next()";
  ara::core::Future<void>::PromiseType promise;
  static_cast<void>(secretSeedUptr_->Next());
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> SecretSeedSkeletonImpl::XorEqualReadOnlyMemRegion(
    const CryptoReadOnlyMemRegion& source)
{
  logger_.LogDebug() << "IPC-CORE::SecretSeed Object XorEqualReadOnlyMemRegion()";
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion const srcReadOnly{source};
  *secretSeedUptr_ ^= srcReadOnly;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> SecretSeedSkeletonImpl::XorEqualSecretSeed(
    const CryptoServiceInstanceId& seedInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::SecretSeedObject XorEqualSecretSeed()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
  logger_.LogInfo() << " IPC-CORE::SecretSeedObject XorEqualSecretSeed() -> Crypto ServiceInstanceId : "
                    << seedInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
      dataStorageContainer->GetObjectFromContanier(seedInstanceId)};
  if (secretSeedSkeletonObj->IsConstUptrSet() == true)
  {
    auto& seedConstUptr{secretSeedSkeletonObj->GetSecretSeedConstUptrInstance()};
    *secretSeedUptr_ ^= *seedConstUptr;
  }
  else
  {
    auto& seedUptr{secretSeedSkeletonObj->GetSecretSeedUptrInstance()};
    *secretSeedUptr_ ^= *seedUptr;
  }
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> SecretSeedSkeletonImpl::Save(const CryptoServiceInstanceId& IOInterfaceInstanceId)
{
  static_cast<void>(IOInterfaceInstanceId);
  return ara::core::Future<void>();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara