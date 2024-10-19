// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : hash_function_skeleton_impl.cpp
//
// Purpose     : Random Generator skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "hash_function_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
using ara::crypto::ipccom::cryp::hashfunction::skeleton::HashFunctionSkeleton;

HashFunctionSkeletonImpl::HashFunctionSkeletonImpl(
    CryptoServiceInstanceId const instanceId, HashFunctionCtx::Uptr hashUptr) noexcept :
    ara::crypto::ipccom::cryp::hashfunction::skeleton::HashFunctionSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    hashCtx_{std::move(hashUptr)}, logger_{ara::log::CreateLogger(
                                       "HASH", "HASH FUNCTION Context", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "IPC-CORE::Hash HashFunctionImpl Constructor";
}

ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::IsInitializedOutput>
    HashFunctionSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::Hash IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = hashCtx_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::GetCryptoPrimitiveIdOutput>
    HashFunctionSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::Hash GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{hashCtx_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::FinishOutput>
    HashFunctionSkeletonImpl::Finish()
{
  // Perform Finish operation of Hash function ctx in crypto daemon. If Finish operation is successful
  // then return the final Hash value else return the error code.
  logger_.LogDebug() << "IPC-CORE::Hash Finish()";
  ara::core::Future<FinishOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const result{hashCtx_->Finish()};
  if (result.HasValue())
  {
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    FinishOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(), [&vtrByteResult](ara::core::Byte i) {
      vtrByteResult.finishResult.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::Hash Finish() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kProcessingNotStarted)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidUsageOrder);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::GetDigestOutput>
    HashFunctionSkeletonImpl::GetDigest(const uint64& offset)
{
  // Perform GetDigest operation of Hash function ctx in crypto daemon. If GetDigest operation is successful
  // then return the final Hash value else return the error code.
  logger_.LogDebug() << "IPC-CORE::Hash GetDigest()";
  ara::core::Future<GetDigestOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const resultGetDigest{
      hashCtx_->GetDigest(static_cast<std::size_t>(offset))};
  if (resultGetDigest.HasValue())
  {
    ara::core::Vector<ara::core::Byte> tempVtr{resultGetDigest.Value()};
    GetDigestOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(), [&vtrByteResult](ara::core::Byte i) {
      vtrByteResult.digestResult.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::Hash GetDigest() Error: " << resultGetDigest.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished);
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::GetDigestServiceOutput>
    HashFunctionSkeletonImpl::GetDigestService()
{
  logger_.LogDebug() << "In HashFunctionSkeletonImpl::GetDigestService()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::hashfunction::HashFunction::GetDigestServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::hashfunction::HashFunction::GetDigestServiceOutput localDigestServiceOutput;
  DigestService::Uptr digestServiceUptr{hashCtx_->GetDigestService()};
  if (digestServiceUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:HashFunctionSkeletonImpl::GetDigestService() successful: ";
    std::shared_ptr<ara::crypto::common::datastorage::DigestServiceContainer> digestServiceContainer{
        ara::crypto::common::datastorage::DigestServiceContainer::GetInstance()};
    CryptoServiceInstanceId cryptolocalInstanceId{digestServiceContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryservice::DigestServiceSkeletonImpl> digestCryptoService{
        std::make_unique<ara::crypto::cryp::cryservice::DigestServiceSkeletonImpl>(
            cryptolocalInstanceId, std::move(digestServiceUptr))};
    digestCryptoService->OfferService();
    digestServiceContainer->StoreObjectToContanier(cryptolocalInstanceId, std::move(digestCryptoService));
    localDigestServiceOutput.instanceId = cryptolocalInstanceId;
    promise.set_value(localDigestServiceOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:HashFunctionSkeletonImpl::GetDigestService() Uptr is nullptr: ";
    promise.set_value(localDigestServiceOutput);
  }
  return promise.get_future();
}

ara::core::Future<void> HashFunctionSkeletonImpl::StartNoParam()
{
  // Call Start without any parameters in crypto daemon and return error if any.
  logger_.LogDebug() << "IPC-CORE::Hash StartNoParam()";
  ara::core::Future<void>::PromiseType promise;
  ara::core::Result<void> const resultStart{hashCtx_->Start()};
  if (resultStart.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::Hash StartNoParam() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::Hash StartNoParam() Error: " << resultStart.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kMissingArgument);
  }
  return promise.get_future();
}

ara::core::Future<void> HashFunctionSkeletonImpl::StartReadOnlyMem(const CryptoReadOnlyMemRegion& iv)
{
  // Call Start with ReadOnlyMemRegion parameter in crypto daemon and return error if any.
  logger_.LogDebug() << "IPC-CORE::Hash StartReadOnlyMem()";
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion const ivStart{iv};
  ara::core::Result<void> const result{hashCtx_->Start(ivStart)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::Hash StartReadOnlyMem() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::Hash StartReadOnlyMem() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> HashFunctionSkeletonImpl::StartSecretSeed(
    const CryptoServiceInstanceId& secretSeedInstanceId)
{
  logger_.LogDebug() << "In HashFunctionImpl::StartSecretSeed()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
  logger_.LogInfo() << " IPC-CORE::Hash Crypto Service Instance Id : " << secretSeedInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
      dataStorageContainer->GetObjectFromContanier(secretSeedInstanceId)};
  ara::core::Result<void> startResult;
  if (secretSeedSkeletonObj->IsConstUptrSet() == true)
  {
    logger_.LogDebug() << "IPC-CORE::Hash StartSecretSeed() -> GetSecretSeedConstUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
    startResult = hashCtx_->Start(*secretSeedUptr);
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE::Hash StartSecretSeed() -> GetSecretSeedUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
    startResult = hashCtx_->Start(*secretSeedUptr);
  }

  if (startResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::Hash StartSecretSeed() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::Hash StartSecretSeed() Error: " << startResult.Error().Message();
    if (startResult.Error() == SecurityErrorDomain::Errc::kUninitializedContext)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
    else if (startResult.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidUsageOrder);
    }
    else if (startResult.Error() == SecurityErrorDomain::Errc::kUnsupported)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> HashFunctionSkeletonImpl::Update(const uint8& in)
{
  // Call Update with uint8_t parameter in crypto daemon and return error if any.
  logger_.LogDebug() << "In HashFunctionImpl::Update()";
  ara::core::Future<void>::PromiseType promise;
  ara::core::Result<void> const resultUpdate{hashCtx_->Update(in)};
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::Hash StartNoParam() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::Hash Update() Error: " << resultUpdate.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kMissingArgument);
  }
  return promise.get_future();
}

ara::core::Future<void> HashFunctionSkeletonImpl::UpdateReadOnlyMem(const CryptoReadOnlyMemRegion& in)
{
  // Call Update with ReadOnlyMemRegion parameter in crypto daemon and return error if any.
  logger_.LogDebug() << "IPC-CORE::Hash UpdateReadOnlyMem()";
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion const inUpdate{in};
  ara::core::Result<void> const result{hashCtx_->Update(inUpdate)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::Hash UpdateReadOnlyMem() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::Hash UpdateReadOnlyMem() Error: " << result.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
  }
  return promise.get_future();
}

ara::core::Future<void> HashFunctionSkeletonImpl::UpdateRestrictedUseObject(
    const RestrictedUseObjectStructure& restrictedUseObjStruct)
{
  logger_.LogDebug() << "IPC-CORE::HASH UpdateRestrictedUseObject()";
  ara::core::Future<void>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE:HASH Crypto Service Instance Id : "
                    << restrictedUseObjStruct.RestrictedUseInstanceId;

  ara::core::Result<void> updateResult;
  CryptoObjectTypeIPC const objType{restrictedUseObjStruct.RestrictedUseObjectType};
  if (objType == CryptoObjectTypeIPC::SecretSeed)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedUseObjStruct.RestrictedUseInstanceId)};

    if (secretSeedSkeletonObj->IsConstUptrSet() == true)
    {
      logger_.LogDebug() << "IPC-CORE::Hash UpdateRestrictedUseObject() -> GetSecretSeedConstUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
      updateResult = hashCtx_->Update(*secretSeedUptr);
    }
    else
    {
      logger_.LogDebug() << "IPC-CORE::Hash UpdateRestrictedUseObject() -> GetSecretSeedUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
      updateResult = hashCtx_->Update(*secretSeedUptr);
    }
  }
  else if (objType == CryptoObjectTypeIPC::SymmetricKey)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmetricKeySkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedUseObjStruct.RestrictedUseInstanceId)};
    auto& symmetricKeyUptr{symmetricKeySkeletonObj->GetSymmetricKeyUptrInstance()};
    updateResult = hashCtx_->Update(*symmetricKeyUptr);
  }
  else if (objType == CryptoObjectTypeIPC::PublicKey)
  {
    // ToDo: Public key and Private key are not implemented for now as
    // underlying crypto daemon doesn't support.
  }
  else if (objType == CryptoObjectTypeIPC::PrivateKey)
  {
    // ToDo: Public key and Private key are not implemented for now as
    // underlying crypto daemon doesn't support.
  }
  if (updateResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::Hash UpdateReadOnlyMem() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::Hash UpdateReadOnlyMem() Error: " << updateResult.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
