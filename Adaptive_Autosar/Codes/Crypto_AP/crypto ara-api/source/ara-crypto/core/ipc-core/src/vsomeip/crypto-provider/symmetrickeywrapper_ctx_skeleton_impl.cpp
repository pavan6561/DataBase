// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetrickeywrapper_ctx_proxy_skeleton_impl.cpp
//
// Purpose     : Symmetric key wrapper context cipher skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "symmetrickeywrapper_ctx_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
SymmetricKeyWrapperCtxSkeletonImpl::SymmetricKeyWrapperCtxSkeletonImpl(
    CryptoServiceInstanceId const instanceId, SymmetricKeyWrapperCtx::Uptr skwcUptr) noexcept :
    ara::crypto::ipccom::cryp::symmetrickeywrapper::skeleton::SymmetricKeyWrapperContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    skwcCtx_{std::move(skwcUptr)}
{
  logger_.LogDebug() << "IPC-CORE::SKWC SymmetricKeyWrapperCtxSkeletonImpl constructor";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::IsInitializedOutput>
    SymmetricKeyWrapperCtxSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::SKWC IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = skwcCtx_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetCryptoPrimitiveIdOutput>
    SymmetricKeyWrapperCtxSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::SKWC GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{skwcCtx_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

CryptoServiceInstanceId SymmetricKeyWrapperCtxSkeletonImpl::GenerateCryptoServiceInstanceId() noexcept
{
  std::lock_guard<std::mutex> lock(mutex_instanceId_);
  serviceInstanceId_ = serviceInstanceId_ + 1;
  CryptoServiceInstanceId strInstance{"SOME/IP:"};
  CryptoServiceInstanceId tempStr{std::to_string(static_cast<unsigned>(serviceInstanceId_))};
  strInstance.append(tempStr);
  return strInstance;
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::CalculateWrappedKeySizeOutput>
    SymmetricKeyWrapperCtxSkeletonImpl::CalculateWrappedKeySize(const uint64& keyLength)
{
  logger_.LogDebug() << "IPC-CORE::SKWC CalculateWrappedKeySize()";
  ara::core::Future<CalculateWrappedKeySizeOutput>::PromiseType promise;
  std::size_t keyLengthLocal{skwcCtx_->CalculateWrappedKeySize(keyLength)};
  CalculateWrappedKeySizeOutput wrappedKeyOutput;
  wrappedKeyOutput.calculatedSize = keyLengthLocal;
  promise.set_value(wrappedKeyOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetMaxTargetKeyLengthOutput>
    SymmetricKeyWrapperCtxSkeletonImpl::GetMaxTargetKeyLength()
{
  logger_.LogDebug() << "IPC-CORE::SKWC GetMaxTargetKeyLength()";
  ara::core::Future<GetMaxTargetKeyLengthOutput>::PromiseType promise;
  GetMaxTargetKeyLengthOutput lengthOutput;
  std::size_t targetKeyLengthLocal{skwcCtx_->GetMaxTargetKeyLength()};
  lengthOutput.keyLengthSize = targetKeyLengthLocal;
  promise.set_value(lengthOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetTargetKeyGranularityOutput>
    SymmetricKeyWrapperCtxSkeletonImpl::GetTargetKeyGranularity()
{
  logger_.LogDebug() << "IPC-CORE::SKWC GetTargetKeyGranularity()";
  ara::core::Future<GetTargetKeyGranularityOutput>::PromiseType promise;
  GetTargetKeyGranularityOutput granularityOutput;
  std::size_t targetKeygranularityLocal{skwcCtx_->GetTargetKeyGranularity()};
  granularityOutput.keySize = targetKeygranularityLocal;
  promise.set_value(granularityOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::UnwrapSeedOutput>
    SymmetricKeyWrapperCtxSkeletonImpl::UnwrapSeed(const CryptoReadOnlyMemRegion& wrappedSeed,
        const CryptoAlgId& targetAlgId, const CryptoAllowedUsageFlags& allowedUsage)
{
  logger_.LogDebug() << "IPC-CORE::SKWC UnwrapSeed()";
  ara::core::Future<UnwrapSeedOutput>::PromiseType promise;
  ReadOnlyMemRegion const seedWrap{wrappedSeed};
  ara::core::Result<SecretSeed::Uptrc> secretSeedUptrc{
      skwcCtx_->UnwrapSeed(seedWrap, targetAlgId, allowedUsage)};
  if (secretSeedUptrc.HasValue())
  {
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::UnwrapSeedOutput
        secretSeedOutput;
    std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
    CryptoServiceInstanceId secretSeedlocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl> localSecretSeedUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>(
            secretSeedlocalInstanceId, std::move(secretSeedUptrc).Value())};
    localSecretSeedUtr->OfferService();
    dataStorageContainer->StoreObjectToContanier(secretSeedlocalInstanceId, std::move(localSecretSeedUtr));
    secretSeedOutput.secretSeedUptrcInstanceId = secretSeedlocalInstanceId;
    promise.set_value(secretSeedOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider UnwrapSeed() Error: "
                       << secretSeedUptrc.Error().Message();

    if (secretSeedUptrc.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> SymmetricKeyWrapperCtxSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::SKWC Reset()";
  ara::core::Future<void>::PromiseType promise;
  skwcCtx_->Reset();
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> SymmetricKeyWrapperCtxSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& keyInstanceId, const CryptoTransformDataType& transform)
{
  logger_.LogDebug() << "IPC-CORE::SKWC SetKey()";
  ara::core::Future<void>::PromiseType promise;
  CryptoTransform localTransform{static_cast<CryptoTransform>(transform)};
  std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::SKWC Crypto Service Instance Id : " << keyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmtericKeySkkeleton{
      dataStorageContainer->GetObjectFromContanier(keyInstanceId)};
  auto& symmtericKeyUptr{symmtericKeySkkeleton->GetSymmetricKeyUptrInstance()};
  ara::core::Result<void> tempResult{skwcCtx_->SetKey(*symmtericKeyUptr, localTransform)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SKWC SetKey() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SKWC SetKey() Error: " << tempResult.Error().Message();
    if (tempResult.Error() == SecurityErrorDomain::Errc::kIncompatibleObject)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::UnwrapKeyOutput>
    SymmetricKeyWrapperCtxSkeletonImpl::UnwrapKey(const CryptoReadOnlyMemRegion& wrappedKey,
        const CryptoAlgId& algId, const CryptoAllowedUsageFlags& allowedUsage)
{
  logger_.LogDebug() << "IPC-CORE::SKWC UnwrapKey()";
  ara::core::Future<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::
          UnwrapKeyOutput>::PromiseType promise;
  ReadOnlyMemRegion const keyWrap{wrappedKey};
  ara::core::Result<RestrictedUseObject::Uptrc> localRestrictedUptrc{
      skwcCtx_->UnwrapKey(keyWrap, algId, allowedUsage)};
  if (localRestrictedUptrc.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SKWC UnwrapKey() -> success";
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::UnwrapKeyOutput output;
    std::shared_ptr<ara::crypto::common::datastorage::RestrictedUsedObjectContainer> dataStorageContainer{
        ara::crypto::common::datastorage::RestrictedUsedObjectContainer::GetInstance()};
    CryptoServiceInstanceId restrictedlocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    logger_.LogDebug() << "IPC-CORE::SKWC restrictedlocalInstanceId = " << restrictedlocalInstanceId;
    std::unique_ptr<ara::crypto::cryp::cryobj::RestrictedUseObjectSkeletonImpl> localRestrictedUseObjectUtr{
        std::make_unique<ara::crypto::cryp::cryobj::RestrictedUseObjectSkeletonImpl>(
            restrictedlocalInstanceId, std::move(localRestrictedUptrc).Value())};
    localRestrictedUseObjectUtr->OfferService();
    dataStorageContainer->StoreObjectToContanier(
        restrictedlocalInstanceId, std::move(localRestrictedUseObjectUtr));
    output.restrictedUseObjInstanceId = restrictedlocalInstanceId;
    promise.set_value(output);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SBCC UnwrapKey() Error: " << localRestrictedUptrc.Error().Message();
    if (localRestrictedUptrc.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::WrapKeyMaterialOutput>
    SymmetricKeyWrapperCtxSkeletonImpl::WrapKeyMaterial(
        const RestrictedUseObjectStructure& restrictedUseObjStruct)
{
  logger_.LogDebug() << "IPC-CORE::SKWC WrapKeyMaterial()";
  ara::core::Future<WrapKeyMaterialOutput>::PromiseType promise;
  CryptoServiceInstanceId& restrictedUseObjectInstanceIdLocal{
      const_cast<CryptoServiceInstanceId&>(restrictedUseObjStruct.RestrictedUseInstanceId)};
  logger_.LogInfo() << "IPC-CORE::SKWC Crypto Service Instance Id: " << restrictedUseObjectInstanceIdLocal;
  CryptoObjectTypeIPC objType{restrictedUseObjStruct.RestrictedUseObjectType};
  if (objType == CryptoObjectTypeIPC::SecretSeed)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedUseObjectInstanceIdLocal)};
    ara::core::Vector<ara::core::Byte> tempVtr;

    if (secretSeedSkeletonObj->IsConstUptrSet() == true)
    {
      logger_.LogDebug() << "IPC-CORE::SKWC WrapKeyMaterial() -> GetSecretSeedConstUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
      ara::core::Result<ara::core::Vector<ara::core::Byte>> tempResult
          = skwcCtx_->WrapKeyMaterial(*secretSeedUptr);
      if (tempResult.HasValue())
      {
        logger_.LogDebug() << "IPC-CORE::SKWC WrapKeyMaterial() -> success";
        tempVtr = tempResult.Value();
      }
      else
      {
        logger_.LogError() << "IPC-CORE::SKWC WrapKeyMaterial() Error: " << tempResult.Error().Message();
        promise.SetError(
            static_cast<ipccom::common::error::CryptoErrorDomainErrc>(tempResult.Error().Value()));
      }
    }
    else
    {
      logger_.LogDebug() << "IPC-CORE::SKWC WrapKeyMaterial() -> GetSecretSeedUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
      ara::core::Result<ara::core::Vector<ara::core::Byte>> tempResult
          = skwcCtx_->WrapKeyMaterial(*secretSeedUptr);
      if (tempResult.HasValue())
      {
        logger_.LogDebug() << "IPC-CORE::SKWC WrapKeyMaterial()->success";
        tempVtr = tempResult.Value();
      }
      else
      {
        logger_.LogError() << "IPC-CORE::SKWC WrapKeyMaterial() Error: " << tempResult.Error().Message();
        promise.SetError(
            static_cast<ipccom::common::error::CryptoErrorDomainErrc>(tempResult.Error().Value()));
      }
    }

    if (tempVtr.empty() == false)
    {
      WrapKeyMaterialOutput keyOutput;
      for_each(tempVtr.begin(), tempVtr.end(),
          [&](ara::core::Byte i) { keyOutput.vtrByte.emplace_back(static_cast<uint8_t>(i)); });
      promise.set_value(keyOutput);
    }
  }
  else if (objType == CryptoObjectTypeIPC::SymmetricKey)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmetricKeySkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedUseObjectInstanceIdLocal)};
    auto& symmetricKeyUptr{symmetricKeySkeletonObj->GetSymmetricKeyUptrInstance()};
    ara::core::Result<ara::core::Vector<ara::core::Byte>> tempResult{
        skwcCtx_->WrapKeyMaterial(*symmetricKeyUptr)};
    if (tempResult.HasValue())
    {
      logger_.LogDebug() << "IPC-CORE:SKWC WrapKeyMaterial() success";
      ara::core::Vector<ara::core::Byte> tempVtr{tempResult.Value()};
      WrapKeyMaterialOutput keyOutput;
      for_each(tempVtr.begin(), tempVtr.end(),
          [&](ara::core::Byte i) { keyOutput.vtrByte.emplace_back(static_cast<uint8_t>(i)); });
      promise.set_value(keyOutput);
    }
    else
    {
      logger_.LogError() << "IPC-CORE:SKWC WrapKeyMaterial() Error: " << tempResult.Error().Message();
      if (tempResult.Error() == SecurityErrorDomain::Errc::kInsufficientCapacity)
      {
        promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity);
      }
      else if (tempResult.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
      {
        promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
      }
      else
      {
        promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
      }
    }
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
  return promise.get_future();
}
ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetExtensionServiceOutput>
    SymmetricKeyWrapperCtxSkeletonImpl::GetExtensionService()
{
  logger_.LogDebug() << "In SymmetricKeyWrapperCtxSkeletonImpl::GetExtensionService()";
  ara::core::Future<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::
          GetExtensionServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetExtensionServiceOutput
      localExtensionServiceOutput;
  ExtensionService::Uptr extensionUptr{skwcCtx_->GetExtensionService()};
  if (extensionUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:SymmetricKeyWrapperCtxSkeletonImpl::GetExtensionService() successful: ";
    std::shared_ptr<ara::crypto::common::datastorage::ExtensionServiceContainer> extensionServiceContainerPtr{
        ara::crypto::common::datastorage::ExtensionServiceContainer::GetInstance()};
    CryptoServiceInstanceId extensionlocalInstanceId{
        extensionServiceContainerPtr->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryservice::ExtensionServiceSkeletonImpl> localBlockService{
        std::make_unique<ara::crypto::cryp::cryservice::ExtensionServiceSkeletonImpl>(
            extensionlocalInstanceId, std::move(extensionUptr))};
    localBlockService->OfferService();
    extensionServiceContainerPtr->StoreObjectToContanier(
        extensionlocalInstanceId, std::move(localBlockService));
    localExtensionServiceOutput.extensionServiceInstanceId = extensionlocalInstanceId;
    promise.set_value(localExtensionServiceOutput);
  }
  else
  {
    logger_.LogError()
        << "IPC-CORE:SymmetricKeyWrapperCtxSkeletonImpl::GetExtensionService() Uptr is nullptr: ";
    promise.set_value(localExtensionServiceOutput);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
