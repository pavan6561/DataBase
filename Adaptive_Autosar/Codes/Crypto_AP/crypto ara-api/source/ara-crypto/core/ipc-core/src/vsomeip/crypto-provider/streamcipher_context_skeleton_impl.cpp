// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : streamcipher_context_skeleton_impl.cpp
//
// Purpose     : Stream cipher context skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "streamcipher_context_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
StreamCipherContextSkeletonImpl::StreamCipherContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, StreamCipherCtx::Uptr sccUptr) noexcept :
    ara::crypto::ipccom::cryp::streamcipher::skeleton::StreamCipherContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    sccCtx_{std::move(sccUptr)}
{
  logger_.LogDebug() << "IPC-CORE::SCC StreamCipherContextSkeletonImpl Constructor";
}

ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::IsInitializedOutput>
    StreamCipherContextSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::SCC IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = sccCtx_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::GetCryptoPrimitiveIdOutput>
    StreamCipherContextSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::SCC GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{sccCtx_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::CountBytesInCacheOutput>
    StreamCipherContextSkeletonImpl::CountBytesInCache()
{
  logger_.LogDebug() << "IPC-CORE::SCC CountBytesInCache()";
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::CountBytesInCacheOutput>::
      PromiseType promise;
  std::size_t const cacheOutput{sccCtx_->CountBytesInCache()};
  CountBytesInCacheOutput countBytesOutput;
  countBytesOutput.countSize = static_cast<uint64>(cacheOutput);
  promise.set_value(countBytesOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::FinishBytesOutput>
    StreamCipherContextSkeletonImpl::FinishBytes(const CryptoReadOnlyMemRegion& in)
{
  logger_.LogDebug() << "IPC-CORE::SCC FinishBytes()";
  ara::core::Future<FinishBytesOutput>::PromiseType promise;
  ReadOnlyMemRegion const inFinish{in};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const result{sccCtx_->FinishBytes(inFinish)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SCC FinishBytes() success";
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    FinishBytesOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(),
        [&vtrByteResult](ara::core::Byte i) { vtrByteResult.vtrByte.emplace_back(static_cast<uint8_t>(i)); });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SCC FinishBytes() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kInsufficientCapacity)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kInOutBuffersIntersect)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInOutBuffersIntersect);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::GetTransformationOutput>
    StreamCipherContextSkeletonImpl::GetTransformation()
{
  logger_.LogDebug() << "IPC-CORE::SCC GetTransformation()";
  ara::core::Future<GetTransformationOutput>::PromiseType promise;
  ara::core::Result<CryptoTransform> const tempResult{sccCtx_->GetTransformation()};
  if (tempResult.HasValue())
  {
    GetTransformationOutput tranOutput;
    switch (tempResult.Value())
    {
    case CryptoTransform::kEncrypt:
      tranOutput.transfor = CryptoTransformDataType::kEncrypt;
      break;
    case CryptoTransform::kDecrypt:
      tranOutput.transfor = CryptoTransformDataType::kDecrypt;
      break;
    case CryptoTransform::kMacVerify:
      tranOutput.transfor = CryptoTransformDataType::kMacVerify;
      break;
    case CryptoTransform::kMacGenerate:
      tranOutput.transfor = CryptoTransformDataType::kMacGenerate;
      break;
    case CryptoTransform::kWrap:
      tranOutput.transfor = CryptoTransformDataType::kWrap;
      break;
    case CryptoTransform::kUnwrap:
      tranOutput.transfor = CryptoTransformDataType::kUnwrap;
      break;
    case CryptoTransform::kSigVerify:
      tranOutput.transfor = CryptoTransformDataType::kSigVerify;
      break;
    default:
      tranOutput.transfor = CryptoTransformDataType::kSigGenerate;
      break;
    }
    promise.set_value(tranOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SCC GetTransformation() Error: " << tempResult.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::IsBytewiseModeOutput>
    StreamCipherContextSkeletonImpl::IsBytewiseMode()
{
  logger_.LogDebug() << "IPC-CORE::SCC IsBytewiseMode()";
  ara::core::Future<IsBytewiseModeOutput>::PromiseType promise;
  bool const isTrue{sccCtx_->IsBytewiseMode()};
  IsBytewiseModeOutput modeOutput;
  modeOutput.isSuccess = isTrue;
  promise.set_value(modeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::IsSeekableModeOutput>
    StreamCipherContextSkeletonImpl::IsSeekableMode()
{
  logger_.LogDebug() << "IPC-CORE::SCC IsSeekableMode()";
  ara::core::Future<IsSeekableModeOutput>::PromiseType promise;
  bool const isTrue{sccCtx_->IsSeekableMode()};
  IsSeekableModeOutput modeOutput;
  modeOutput.isSuccess = isTrue;
  promise.set_value(modeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::ProcessBlocksOutput>
    StreamCipherContextSkeletonImpl::ProcessBlocks(const CryptoReadOnlyMemRegion& in)
{
  logger_.LogDebug() << "IPC-CORE::SCC ProcessBlocks()";
  ara::core::Future<ProcessBlocksOutput>::PromiseType promise;
  ReadOnlyMemRegion const proBlocks{in};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const result{sccCtx_->ProcessBlocks(proBlocks)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SCC ProcessBlocks() success";
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    ProcessBlocksOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(),
        [&vtrByteResult](ara::core::Byte i) { vtrByteResult.vtrByte.emplace_back(static_cast<uint8_t>(i)); });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SCC ProcessBlocks() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kIncompatibleArguments)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kInOutBuffersIntersect)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInOutBuffersIntersect);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kInvalidUsageOrder)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidUsageOrder);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::ProcessByteOutput>
    StreamCipherContextSkeletonImpl::ProcessByte(const CryptoReadOnlyMemRegion& in)
{
  logger_.LogDebug() << "IPC-CORE::SCC ProcessByte()";
  ara::core::Future<ProcessByteOutput>::PromiseType promise;
  ReadOnlyMemRegion const proByte{in};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const result{sccCtx_->ProcessBytes(proByte)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SCC ProcessByte() success";
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    ProcessByteOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(),
        [&vtrByteResult](ara::core::Byte i) { vtrByteResult.vtrByte.emplace_back(static_cast<uint8_t>(i)); });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SCC ProcessByte() Error:: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kInsufficientCapacity)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kInOutBuffersIntersect)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInOutBuffersIntersect);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> StreamCipherContextSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::SCC Reset()";
  sccCtx_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> StreamCipherContextSkeletonImpl::Seek(const Int64& offset, const Boolean& fromBegin)
{
  logger_.LogDebug() << "IPC-CORE::SCC Seek()";
  ara::core::Future<void>::PromiseType promise;
  ara::core::Result<void> const result{sccCtx_->Seek(offset, fromBegin)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SCC Seek() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SCC Seek() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kUnsupported)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kProcessingNotStarted)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kBelowBoundary)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kBelowBoundary);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> StreamCipherContextSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& instanceId, const CryptoTransformDataType& transform)
{
  logger_.LogDebug() << "IPC-CORE::SCC SetKey()";
  ara::core::Future<void>::PromiseType promise;
  CryptoTransform localTransform{static_cast<CryptoTransform>(transform)};
  std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::SCC Crypto Service Instance Id : " << instanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmtericKeySkkeleton{
      dataStorageContainer->GetObjectFromContanier(instanceId)};
  auto& symmtericKeyUptr{symmtericKeySkkeleton->GetSymmetricKeyUptrInstance()};
  ara::core::Result<void> tempResult{sccCtx_->SetKey(*symmtericKeyUptr, localTransform)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SCC SetKey() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SCC SetKey() Error: " << tempResult.Error().Message();
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

ara::core::Future<void> StreamCipherContextSkeletonImpl::StartReadOnlyMem(const CryptoReadOnlyMemRegion& iv)
{
  logger_.LogDebug() << "IPC-CORE::SCC StartReadOnlyMem()";
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion const startResult{iv};
  ara::core::Result<void> const result{sccCtx_->Start(startResult)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SCC Start() with ReadOnlyMem as parameter -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SCC ProcessByte() Error: " << result.Error().Message();
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

ara::core::Future<void> StreamCipherContextSkeletonImpl::StartSecretSeed(const CryptoServiceInstanceId& iv)
{
  logger_.LogDebug() << "IPC-CORE::SCC StartSecretSeed()";
  ara::core::Future<void>::PromiseType promise;
  CryptoServiceInstanceId& secretSeedInstanceIdLocal{const_cast<CryptoServiceInstanceId&>(iv)};
  std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::SCC Crypto Service Instance Id : " << secretSeedInstanceIdLocal;
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
      dataStorageContainer->GetObjectFromContanier(secretSeedInstanceIdLocal)};
  SecretSeed::Uptrc secretSeedUptr;
  ara::core::Result<void> startResult;
  if (secretSeedSkeletonObj->IsConstUptrSet() == true)
  {
    logger_.LogDebug() << "IPC-CORE::SCC StartSecretSeed() -> GetSecretSeedConstUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
    startResult = sccCtx_->Start(*secretSeedUptr);
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE::SCC StartSecretSeed() -> GetSecretSeedUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
    startResult = sccCtx_->Start(*secretSeedUptr);
  }

  if (startResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SCC StartSecretSeed() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SCC StartSecretSeed() Error: " << startResult.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(startResult.Error().Value()));
  }
  return promise.get_future();
}
ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::GetBlockServiceOutput>
    StreamCipherContextSkeletonImpl::GetBlockService()
{
  logger_.LogDebug() << "In StreamCipherContextSkeletonImpl::GetBlockService()";
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::GetBlockServiceOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::GetBlockServiceOutput localBlockServiceOutput;
  BlockService::Uptr blockUptr{sccCtx_->GetBlockService()};
  if (blockUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:StreamCipherContextSkeletonImpl::GetBlockService() successful: ";
    std::shared_ptr<ara::crypto::common::datastorage::BlockServiceContainer> blockServiceContainer{
        ara::crypto::common::datastorage::BlockServiceContainer::GetInstance()};
    CryptoServiceInstanceId blocklocalInstanceId{blockServiceContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryservice::BlockServiceSkeletonImpl> localBlockService{
        std::make_unique<ara::crypto::cryp::cryservice::BlockServiceSkeletonImpl>(
            blocklocalInstanceId, std::move(blockUptr))};
    localBlockService->OfferService();
    blockServiceContainer->StoreObjectToContanier(blocklocalInstanceId, std::move(localBlockService));
    localBlockServiceOutput.blockServiceInstanceId = blocklocalInstanceId;
    promise.set_value(localBlockServiceOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:StreamCipherContextSkeletonImpl::GetBlockService() Uptr is nullptr: ";
    promise.set_value(localBlockServiceOutput);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
