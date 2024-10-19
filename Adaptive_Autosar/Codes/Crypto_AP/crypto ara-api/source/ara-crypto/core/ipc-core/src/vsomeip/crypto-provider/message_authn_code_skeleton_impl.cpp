// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : message_authn_code_impl.cpp
//
// Purpose     : Message authentication code skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "message_authn_code_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
using ara::crypto::ipccom::cryp::messageauthenticationcode::skeleton::MessageAuthnCodeSkeleton;

MessageAuthnCodeSkeletonImpl::MessageAuthnCodeSkeletonImpl(
    CryptoServiceInstanceId const instanceId, MessageAuthnCodeCtx::Uptr macUptr) noexcept :
    ara::crypto::ipccom::cryp::messageauthenticationcode::skeleton::MessageAuthnCodeSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    macCtx_{std::move(macUptr)}
{
  logger_.LogDebug() << "IPC-CORE::MAC MessageAuthnCodeSkeletonImpl Constructor";
}

ara::core::Future<ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::IsInitializedOutput>
    MessageAuthnCodeSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::MAC IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = macCtx_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetCryptoPrimitiveIdOutput>
    MessageAuthnCodeSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::MAC GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{macCtx_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

CryptoServiceInstanceId MessageAuthnCodeSkeletonImpl::GenerateCryptoServiceInstanceId() noexcept
{
  logger_.LogDebug() << "IPC-CORE::MAC GenerateCryptoServiceInstanceId()";
  std::lock_guard<std::mutex> lock{mutex_instanceId_};
  serviceInstanceId_ = serviceInstanceId_ + 1;
  CryptoServiceInstanceId strInstance{"SOME/IP:"};
  CryptoServiceInstanceId tempStr{std::to_string(static_cast<unsigned>(serviceInstanceId_))};
  strInstance.append(tempStr);
  return strInstance;
}

ara::core::Future<ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::CheckOutput>
    MessageAuthnCodeSkeletonImpl::Check(const CryptoServiceInstanceId& instanceId)
{
  logger_.LogDebug() << "IPC-CORE::MAC Check()";
  ara::core::Future<CheckOutput>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SignatureContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SignatureContainer::GetInstance()};
  logger_.LogInfo() << " IPC-CORE::MAC Crypto Service Instance Id : " << instanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SignatureSkeletonImpl>& signatureSkeletonObj{
      dataStorageContainer->GetObjectFromContanier(instanceId)};
  auto& signatureUptrc{signatureSkeletonObj->GetSignatureUptrInstance()};
  ara::core::Result<bool> checkResult{macCtx_->Check(*signatureUptrc)};
  if (checkResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::MAC Check() success";
    CheckOutput localCheckOutput;
    localCheckOutput.result = checkResult.Value();
    promise.set_value(localCheckOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::MAC Check() Error: " << checkResult.Error().Message();
    if (checkResult.Error() == SecurityErrorDomain::Errc::kProcessingNotFinished)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::FinishOutput>
    MessageAuthnCodeSkeletonImpl::Finish(const Boolean& makeSignatureObject)
{
  logger_.LogDebug() << "IPC-CORE::MAC Finish()";
  ara::core::Future<FinishOutput>::PromiseType promise;
  ara::core::Result<Signature::Uptrc> resultFinish{macCtx_->Finish(makeSignatureObject)};
  if (resultFinish.HasValue())
  {
    FinishOutput tempOutput;
    std::shared_ptr<ara::crypto::common::datastorage::SignatureContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SignatureContainer::GetInstance()};
    CryptoServiceInstanceId signaturelocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SignatureSkeletonImpl> localSignatureUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SignatureSkeletonImpl>(
            signaturelocalInstanceId, std::move(resultFinish).Value())};
    localSignatureUtr->OfferService();
    dataStorageContainer->StoreObjectToContanier(signaturelocalInstanceId, std::move(localSignatureUtr));
    tempOutput.finishInstanceId = signaturelocalInstanceId;
    promise.set_value(tempOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::MAC Finish() Error: " << resultFinish.Error().Message();
    if (resultFinish.Error() == SecurityErrorDomain::Errc::kProcessingNotStarted)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetDigestOutput>
    MessageAuthnCodeSkeletonImpl::GetDigest(const uint64& offset)
{
  logger_.LogDebug() << "IPC-CORE::MAC GetDigest()";
  ara::core::Future<GetDigestOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const resultGetDigest{
      macCtx_->GetDigest(static_cast<std::size_t>(offset))};
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
    logger_.LogError() << "IPC-CORE::MAC GetDigest() Error: " << resultGetDigest.Error().Message();
    if (resultGetDigest.Error() == SecurityErrorDomain::Errc::kProcessingNotFinished)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetDigestServiceOutput>
    MessageAuthnCodeSkeletonImpl::GetDigestService()
{
  logger_.LogDebug() << "In MessageAuthnCodeSkeletonImpl::GetDigestService()";
  ara::core::Future<ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::
          GetDigestServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::messageauthenticationcode::MessageAuthnCode::GetDigestServiceOutput
      localDigestServiceOutput;
  DigestService::Uptr digestServiceUptr{macCtx_->GetDigestService()};
  if (digestServiceUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:MessageAuthnCodeSkeletonImpl::GetDigestService() successful: ";
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
    logger_.LogError() << "IPC-CORE:MessageAuthnCodeSkeletonImpl::GetDigestService() Uptr is nullptr: ";
    promise.set_value(localDigestServiceOutput);
  }
  return promise.get_future();
}

ara::core::Future<void> MessageAuthnCodeSkeletonImpl::Reset()
{
  ara::core::Future<void>::PromiseType promise;
  ara::core::Result<void> const result{macCtx_->Reset()};
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> MessageAuthnCodeSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& symmetricKeyInstanceId, const CryptoTransformDataType& transform)
{
  logger_.LogDebug() << "IPC-CORE::MAC SetKey()";
  ara::core::Future<void>::PromiseType promise;
  CryptoTransform localTransform{static_cast<CryptoTransform>(transform)};
  std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::MAC Crypto Service Instance Id : " << symmetricKeyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmetericKeySkeleton{
      dataStorageContainer->GetObjectFromContanier(symmetricKeyInstanceId)};
  auto& symmetericKeyUptr{symmetericKeySkeleton->GetSymmetricKeyUptrInstance()};
  ara::core::Result<void> tempResult{macCtx_->SetKey(*symmetericKeyUptr, localTransform)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ACC SetKey() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ACC SetKey() Error: " << tempResult.Error().Message();
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

ara::core::Future<void> MessageAuthnCodeSkeletonImpl::StartReadOnlyMem(const CryptoReadOnlyMemRegion& iv)
{
  logger_.LogDebug() << "IPC-CORE::MAC StartReadOnlyMem()";
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion const ivStart{iv};
  ara::core::Result<void> const resultStart{macCtx_->Start(ivStart)};
  if (resultStart.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::MAC StartReadOnlyMem() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::MAC StartReadOnlyMem() Error: " << resultStart.Error().Message();
    if (resultStart.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else if (resultStart.Error() == SecurityErrorDomain::Errc::kUninitializedContext)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> MessageAuthnCodeSkeletonImpl::StartSecretSeed(
    const CryptoServiceInstanceId& secretSeedInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::MAC StartSecretSeed()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::MAC Crypto Service Instance Id : " << secretSeedInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
      dataStorageContainer->GetObjectFromContanier(secretSeedInstanceId)};

  ara::core::Result<void> startResult;
  if (secretSeedSkeletonObj->IsConstUptrSet() == true)
  {
    logger_.LogDebug() << "IPC-CORE::MAC StartSecretSeed() -> GetSecretSeedConstUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
    startResult = macCtx_->Start(*secretSeedUptr);
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE::MAC StartSecretSeed() -> GetSecretSeedUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
    startResult = macCtx_->Start(*secretSeedUptr);
  }

  if (startResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::MAC StartSecretSeed() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::MAC StartSecretSeed() Error: " << startResult.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(startResult.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<void> MessageAuthnCodeSkeletonImpl::Update(const uint8& in)
{
  logger_.LogDebug() << "IPC-CORE::MAC Update()";
  ara::core::Future<void>::PromiseType promise;
  ara::core::Result<void> const resultUpdate{macCtx_->Update(in)};
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::MAC Update() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::MAC Update() Error: " << resultUpdate.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
  }
  return promise.get_future();
}

ara::core::Future<void> MessageAuthnCodeSkeletonImpl::UpdateReadOnlyMem(const CryptoReadOnlyMemRegion& in)
{
  logger_.LogDebug() << "IPC-CORE::MAC UpdateReadOnlyMem()";
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion inUpdate{in};
  ara::core::Result<void> resultUpdate{macCtx_->Update(inUpdate)};
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::MAC UpdateReadOnlyMem() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::MAC UpdateReadOnlyMem() Error: " << resultUpdate.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
  }
  return promise.get_future();
}

ara::core::Future<void> MessageAuthnCodeSkeletonImpl::UpdateRestrictedUseObject(
    const RestrictedUseObjectStructure& restrictedUseObjStruct)
{
  logger_.LogDebug() << "IPC-CORE::MAC UpdateRestrictedUseObject()";
  ara::core::Future<void>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::MAC Crypto Service Instance Id : "
                    << restrictedUseObjStruct.RestrictedUseInstanceId;
  ara::core::Result<void> updateResult;
  CryptoObjectTypeIPC objType{restrictedUseObjStruct.RestrictedUseObjectType};
  if (objType == CryptoObjectTypeIPC::SecretSeed)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedUseObjStruct.RestrictedUseInstanceId)};
    if (secretSeedSkeletonObj->IsConstUptrSet() == true)
    {
      logger_.LogDebug() << "IPC-CORE::MAC UpdateRestrictedUseObject() -> GetSecretSeedConstUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
      updateResult = macCtx_->Update(*secretSeedUptr);
    }
    else
    {
      logger_.LogDebug() << "IPC-CORE::MAC UpdateRestrictedUseObject() -> GetSecretSeedUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
      updateResult = macCtx_->Update(*secretSeedUptr);
    }
  }
  else if (objType == CryptoObjectTypeIPC::SymmetricKey)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmetricKeySkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedUseObjStruct.RestrictedUseInstanceId)};
    auto& symmetricKeyUptr{symmetricKeySkeletonObj->GetSymmetricKeyUptrInstance()};
    updateResult = macCtx_->Update(*symmetricKeyUptr);
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
    logger_.LogDebug() << "IPC-CORE::MAC UpdateRestrictedUseObject() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::MAC UpdateRestrictedUseObject() Error: "
                       << updateResult.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
