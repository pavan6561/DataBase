// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : authcipher_context_skeleton_impl.cpp
//
// Purpose     : Authenticate cipher context skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "authcipher_context_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
AuthCipherContextSkeletonImpl::AuthCipherContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, AuthCipherCtx::Uptr accUptr) noexcept :
    ara::crypto::ipccom::cryp::authenticationcipher::skeleton::AuthCipherContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    accCtx_{std::move(accUptr)}
{
  logger_.LogDebug() << "AuthCipherContextSkeletonImpl Constructor";
}

ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::IsInitializedOutput>
    AuthCipherContextSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::ACC IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = accCtx_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetCryptoPrimitiveIdOutput>
    AuthCipherContextSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::ACC GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{accCtx_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::CheckOutput>
    AuthCipherContextSkeletonImpl::Check(const CryptoServiceInstanceId& instanceId)
{
  logger_.LogDebug() << "IPC-CORE::ACC Check()";
  ara::core::Future<CheckOutput>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SignatureContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SignatureContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::ACC Crypto Service Instance Id : " << instanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SignatureSkeletonImpl>& signatureSkeletonObj{
      dataStorageContainer->GetObjectFromContanier(instanceId)};
  auto& signatureUptrc{signatureSkeletonObj->GetSignatureUptrInstance()};
  const ara::core::Result<bool> checkResult{accCtx_->Check(*signatureUptrc)};
  if (checkResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ACC Check() success";
    CheckOutput localCheckOutput;
    localCheckOutput.isSuccess = checkResult.Value();
    promise.set_value(localCheckOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ACC Check() Error: " << checkResult.Error().Message();
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

ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetDigestOutput>
    AuthCipherContextSkeletonImpl::GetDigest(const uint64& offset)
{
  logger_.LogDebug() << "IPC-CORE::ACC GetDigest()";
  ara::core::Future<GetDigestOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const resultGetDigest{
      accCtx_->GetDigest(static_cast<std::size_t>(offset))};
  if (resultGetDigest.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ACC GetDigest() success";
    ara::core::Vector<ara::core::Byte> tempVtr{resultGetDigest.Value()};
    GetDigestOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(), [&vtrByteResult](ara::core::Byte i) {
      vtrByteResult.vtrResult.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:ACC GetDigest() Error: " << resultGetDigest.Error().Message();
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
    ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetMaxAssociatedDataSizeOutput>
    AuthCipherContextSkeletonImpl::GetMaxAssociatedDataSize()
{
  logger_.LogDebug() << "IPC-CORE::ACC GetMaxAssociatedDataSize()";
  ara::core::Future<GetMaxAssociatedDataSizeOutput>::PromiseType promise;
  GetMaxAssociatedDataSizeOutput dataSizeOutput;
  dataSizeOutput.maxDataSize = accCtx_->GetMaxAssociatedDataSize();
  promise.set_value(dataSizeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetTransformationOutput>
    AuthCipherContextSkeletonImpl::GetTransformation()
{
  logger_.LogDebug() << "IPC-CORE::ACC GetTransformation()";
  ara::core::Future<GetTransformationOutput>::PromiseType promise;
  ara::core::Result<CryptoTransform> const tempResult{accCtx_->GetTransformation()};
  if (tempResult.HasValue())
  {
    GetTransformationOutput tranOutput;
    switch (tempResult.Value())
    {
    case CryptoTransform::kEncrypt:
      tranOutput.CryptoTransform = CryptoTransformDataType::kEncrypt;
      break;
    case CryptoTransform::kDecrypt:
      tranOutput.CryptoTransform = CryptoTransformDataType::kDecrypt;
      break;
    case CryptoTransform::kMacVerify:
      tranOutput.CryptoTransform = CryptoTransformDataType::kMacVerify;
      break;
    case CryptoTransform::kMacGenerate:
      tranOutput.CryptoTransform = CryptoTransformDataType::kMacGenerate;
      break;
    case CryptoTransform::kWrap:
      tranOutput.CryptoTransform = CryptoTransformDataType::kWrap;
      break;
    case CryptoTransform::kUnwrap:
      tranOutput.CryptoTransform = CryptoTransformDataType::kUnwrap;
      break;
    case CryptoTransform::kSigVerify:
      tranOutput.CryptoTransform = CryptoTransformDataType::kSigVerify;
      break;
    default:
      tranOutput.CryptoTransform = CryptoTransformDataType::kSigGenerate;
      break;
    }
    promise.set_value(tranOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ACC GetTransformation() Error: " << tempResult.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::
        ProcessConfidentialDataReadOnlyMemOutput>
    AuthCipherContextSkeletonImpl::ProcessConfidentialDataReadOnlyMem(
        const CryptoReadOnlyMemRegion& in, const CryptoReadOnlyMemRegion& expectedTag)
{
  logger_.LogDebug() << "IPC-CORE::ACC ProcessConfidentialDataReadOnlyMem()";
  ara::core::Future<ProcessConfidentialDataReadOnlyMemOutput>::PromiseType promise;
  ReadOnlyMemRegion const inTemp{in};
  ReadOnlyMemRegion const tagTemp{expectedTag};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const resultData{
      accCtx_->ProcessConfidentialData(inTemp, tagTemp)};
  if (resultData.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ACC ProcessConfidentialDataReadOnlyMem() -> success";
    ara::core::Vector<ara::core::Byte> tempVtr{resultData.Value()};
    ProcessConfidentialDataReadOnlyMemOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(),
        [&vtrByteResult](ara::core::Byte i) { vtrByteResult.vtrByte.emplace_back(static_cast<uint8_t>(i)); });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE ProcessConfidentialDataReadOnlyMem() Error: "
                       << resultData.Error().Message();

    if (resultData.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else if (resultData.Error() == SecurityErrorDomain::Errc::kProcessingNotStarted)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kAuthTagNotValid);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> AuthCipherContextSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::ACC Reset()";
  accCtx_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> AuthCipherContextSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& key, const CryptoTransformDataType& transform)
{
  logger_.LogDebug() << "IPC-CORE::ACC SetKey()";
  ara::core::Future<void>::PromiseType promise;
  const CryptoTransform localTransform{static_cast<CryptoTransform>(transform)};
  std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::ACC  Crypto Service Instance Id : " << key;
  std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmtericKeySkkeleton{
      dataStorageContainer->GetObjectFromContanier(key)};
  auto& symmtericKeyUptr{symmtericKeySkkeleton->GetSymmetricKeyUptrInstance()};
  const ara::core::Result<void> tempResult{accCtx_->SetKey(*symmtericKeyUptr, localTransform)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ACC SetKey() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE:ACC SetKey() Error: " << tempResult.Error().Message();
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

ara::core::Future<void> AuthCipherContextSkeletonImpl::StartReadOnlyMem(const CryptoReadOnlyMemRegion& iv)
{
  logger_.LogDebug() << "IPC-CORE::ACC StartReadOnlyMem()";
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion const ivStart{iv};
  ara::core::Result<void> const result{accCtx_->Start(ivStart)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ACC StartReadOnlyMem() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ACC StartReadOnlyMem() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kUninitializedContext)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
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

ara::core::Future<void> AuthCipherContextSkeletonImpl::StartSecretSeed(
    const CryptoServiceInstanceId& instanceId)
{
  logger_.LogDebug() << "IPC-CORE::ACC StartSecretSeed()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
  logger_.LogInfo() << " IPC-CORE::ACC Crypto Service Instance Id : " << instanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
      dataStorageContainer->GetObjectFromContanier(instanceId)};
  ara::core::Result<void> startResult;

  if (secretSeedSkeletonObj->IsConstUptrSet() == true)
  {
    logger_.LogDebug() << "IPC-CORE::ACC StartSecretSeed() -> GetSecretSeedConstUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
    startResult = accCtx_->Start(*secretSeedUptr);
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE::ACC StartSecretSeed() -> GetSecretSeedUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
    startResult = accCtx_->Start(*secretSeedUptr);
  }

  if (startResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ACC StartSecretSeed() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ACC StartSecretSeed() Error: " << startResult.Error().Message();
    if (startResult.Error() == SecurityErrorDomain::Errc::kUninitializedContext)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
    else if (startResult.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
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

ara::core::Future<void> AuthCipherContextSkeletonImpl::UpdateAssociatedDataReadOnlyMem(
    const CryptoReadOnlyMemRegion& in)
{
  logger_.LogDebug() << "IPC-CORE::ACC UpdateAssociatedDataReadOnlyMem()";
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion const inUpdate{in};
  ara::core::Result<void> const result{accCtx_->UpdateAssociatedData(inUpdate)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ACC UpdateAssociatedData() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ACC UpdateAssociatedData() Error: " << result.Error().Message();
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

ara::core::Future<void> AuthCipherContextSkeletonImpl::UpdateAssociatedData(const uint8& in)
{
  logger_.LogDebug() << "IPC-CORE::ACC UpdateAssociatedData() with uint8 as argument";
  ara::core::Future<void>::PromiseType promise;
  const ara::core::Result<void> result{accCtx_->UpdateAssociatedData(in)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ACC UpdateAssociatedData() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ACC UpdateAssociatedData() Error: " << result.Error().Message();
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

ara::core::Future<void> AuthCipherContextSkeletonImpl::UpdateAssociatedDataResUseObj(
    const RestrictedUseObjectStructure& restrictedUseObjStruct)
{
  logger_.LogDebug() << "IPC-CORE::ACC UpdateAssociatedDataResUseObj()";
  ara::core::Future<void>::PromiseType promise;
  CryptoServiceInstanceId& restrictedInstanceIdLocal{
      const_cast<CryptoServiceInstanceId&>(restrictedUseObjStruct.RestrictedUseInstanceId)};
  logger_.LogInfo() << "IPC-CORE::ACC Crypto Service Instance Id : " << restrictedInstanceIdLocal;

  ara::core::Result<void> updateResult;
  const CryptoObjectTypeIPC objType{restrictedUseObjStruct.RestrictedUseObjectType};
  if (objType == CryptoObjectTypeIPC::SecretSeed)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedInstanceIdLocal)};

    if (secretSeedSkeletonObj->IsConstUptrSet() == true)
    {
      logger_.LogDebug()
          << "IPC-CORE::ACC UpdateAssociatedDataResUseObj() -> GetSecretSeedConstUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
      updateResult = accCtx_->UpdateAssociatedData(*secretSeedUptr);
    }
    else
    {
      logger_.LogDebug() << "IPC-CORE::ACC UpdateAssociatedDataResUseObj() -> GetSecretSeedUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
      updateResult = accCtx_->UpdateAssociatedData(*secretSeedUptr);
    }
  }
  else if (objType == CryptoObjectTypeIPC::SymmetricKey)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmetricKeySkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedInstanceIdLocal)};
    auto& symmetricKeyUptr{symmetricKeySkeletonObj->GetSymmetricKeyUptrInstance()};
    updateResult = accCtx_->UpdateAssociatedData(*symmetricKeyUptr);
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
    logger_.LogDebug() << "IPC-CORE::ACC UpdateAssociatedDataResUseObj() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ACC UpdateAssociatedDataResUseObj() Error: "
                       << updateResult.Error().Message();
    if (updateResult.Error() == SecurityErrorDomain::Errc::kProcessingNotStarted)
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
ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetBlockServiceOutput>
    AuthCipherContextSkeletonImpl::GetBlockService()
{
  logger_.LogDebug() << "In AuthCipherContextSkeletonImpl::GetBlockService()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetBlockServiceOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetBlockServiceOutput
      localBlockServiceOutput;
  BlockService::Uptr blockUptr{accCtx_->GetBlockService()};
  if (blockUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:AuthCipherContextSkeletonImpl::GetBlockService() successful: ";
    std::shared_ptr<ara::crypto::common::datastorage::BlockServiceContainer> blockServiceContainer{
        ara::crypto::common::datastorage::BlockServiceContainer::GetInstance()};
    CryptoServiceInstanceId blocklocalInstanceId{blockServiceContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryservice::BlockServiceSkeletonImpl> localBlockService{
        std::make_unique<ara::crypto::cryp::cryservice::BlockServiceSkeletonImpl>(
            blocklocalInstanceId, std::move(blockUptr))};
    localBlockService->OfferService();
    blockServiceContainer->StoreObjectToContanier(blocklocalInstanceId, std::move(localBlockService));
    localBlockServiceOutput.blockInstanceId = blocklocalInstanceId;
    promise.set_value(localBlockServiceOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:AuthCipherContextSkeletonImpl::GetBlockService() Uptr is nullptr: ";
    promise.set_value(localBlockServiceOutput);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
