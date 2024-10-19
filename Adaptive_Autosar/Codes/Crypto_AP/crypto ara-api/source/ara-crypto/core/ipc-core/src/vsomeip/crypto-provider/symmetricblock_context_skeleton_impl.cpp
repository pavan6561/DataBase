// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetricblock_context_skeleton_impl.cpp
//
// Purpose     : Symmetric block context cipher skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "symmetricblock_context_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
SymmetricBlockCipherContextSkeletonImpl::SymmetricBlockCipherContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, SymmetricBlockCipherCtx::Uptr sbccUptr) noexcept :
    ara::crypto::ipccom::cryp::symmetricblockcipher::skeleton::SymmetricBlockCipherContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    sbccCtx_{std::move(sbccUptr)}
{
  logger_.LogDebug() << "IPC-CORE::SBCC StreamCipherContextSkeletonImpl Constructor";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::IsInitializedOutput>
    SymmetricBlockCipherContextSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::SBCC IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = sbccCtx_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::GetCryptoPrimitiveIdOutput>
    SymmetricBlockCipherContextSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::SBCC GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{sbccCtx_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::GetTransformationOutput>
    SymmetricBlockCipherContextSkeletonImpl::GetTransformation()
{
  logger_.LogDebug() << "IPC-CORE::SBCC GetTransformation()";
  ara::core::Future<GetTransformationOutput>::PromiseType promise;
  ara::core::Result<CryptoTransform> const tempResult{sbccCtx_->GetTransformation()};
  if (tempResult.HasValue())
  {
    GetTransformationOutput tranOutput;
    switch (tempResult.Value())
    {
    case CryptoTransform::kEncrypt:
      tranOutput.transform = CryptoTransformDataType::kEncrypt;
      break;
    case CryptoTransform::kDecrypt:
      tranOutput.transform = CryptoTransformDataType::kDecrypt;
      break;
    case CryptoTransform::kMacVerify:
      tranOutput.transform = CryptoTransformDataType::kMacVerify;
      break;
    case CryptoTransform::kMacGenerate:
      tranOutput.transform = CryptoTransformDataType::kMacGenerate;
      break;
    case CryptoTransform::kWrap:
      tranOutput.transform = CryptoTransformDataType::kWrap;
      break;
    case CryptoTransform::kUnwrap:
      tranOutput.transform = CryptoTransformDataType::kUnwrap;
      break;
    case CryptoTransform::kSigVerify:
      tranOutput.transform = CryptoTransformDataType::kSigVerify;
      break;
    default:
      tranOutput.transform = CryptoTransformDataType::kSigGenerate;
      break;
    }
    promise.set_value(tranOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SBCC GetTransformation() Error: " << tempResult.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::ProcessBlockOutput>
    SymmetricBlockCipherContextSkeletonImpl::ProcessBlock(
        const CryptoReadOnlyMemRegion& in, const Boolean& suppressPadding)
{
  logger_.LogDebug() << "IPC-CORE::SBCC ProcessBlock()";
  ara::core::Future<ProcessBlockOutput>::PromiseType promise;
  ReadOnlyMemRegion const proBlock{in};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> result{
      sbccCtx_->ProcessBlock(proBlock, suppressPadding)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SBCC ProcessBlock() success";
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    ProcessBlockOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(),
        [&vtrByteResult](ara::core::Byte i) { vtrByteResult.vtrByte.emplace_back(static_cast<uint8_t>(i)); });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SBCC ProcessBlock() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kInsufficientCapacity)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
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
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::ProcessBlocksOutput>
    SymmetricBlockCipherContextSkeletonImpl::ProcessBlocks(const CryptoReadOnlyMemRegion& in)
{
  logger_.LogDebug() << "IPC-CORE::SBCC ProcessBlocks()";
  ara::core::Future<ProcessBlocksOutput>::PromiseType promise;
  ReadOnlyMemRegion const proBlock{in};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const result{sbccCtx_->ProcessBlocks(proBlock)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SBCC ProcessBlocks() success";
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    ProcessBlocksOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(),
        [&vtrByteResult](ara::core::Byte i) { vtrByteResult.vtrByte.emplace_back(static_cast<uint8_t>(i)); });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SBCC ProcessBlocks() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kUninitializedContext)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kIncompatibleArguments)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInOutBuffersIntersect);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> SymmetricBlockCipherContextSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::SBCC Reset()";
  sbccCtx_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> SymmetricBlockCipherContextSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& instanceId, const CryptoTransformDataType& transform)
{
  logger_.LogDebug() << "IPC-CORE::SBCC SetKey()";
  ara::core::Future<void>::PromiseType promise;
  CryptoServiceInstanceId& symmetricKeyInstanceIdLocal{const_cast<CryptoServiceInstanceId&>(instanceId)};
  CryptoTransform localTransform{static_cast<CryptoTransform>(transform)};
  std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::SBCC Crypto Service Instance Id : " << symmetricKeyInstanceIdLocal;
  std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmtericKeySkkeleton{
      dataStorageContainer->GetObjectFromContanier(symmetricKeyInstanceIdLocal)};
  auto& symmtericKeyUptr{symmtericKeySkkeleton->GetSymmetricKeyUptrInstance()};
  ara::core::Result<void> tempResult{sbccCtx_->SetKey(*symmtericKeyUptr, localTransform)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SBCC SetKey() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SBCC SetKey() Error: " << tempResult.Error().Message();
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
ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::GetCryptoServiceOutput>
    SymmetricBlockCipherContextSkeletonImpl::GetCryptoService()
{
  logger_.LogDebug() << "In SymmetricBlockCipherContextSkeletonImpl::GetCryptoService()";
  ara::core::Future<ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::
          GetCryptoServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::GetCryptoServiceOutput
      localCryptoServiceOutput;
  CryptoService::Uptr cryptoUptr{sbccCtx_->GetCryptoService()};
  if (cryptoUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:SymmetricBlockCipherContextSkeletonImpl::GetCryptoService() successful: ";
    std::shared_ptr<ara::crypto::common::datastorage::CryptoServiceContainer> cryptoServiceContainer{
        ara::crypto::common::datastorage::CryptoServiceContainer::GetInstance()};
    CryptoServiceInstanceId cryptolocalInstanceId{cryptoServiceContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryservice::CryptoServiceSkeletonImpl> localCryptoService{
        std::make_unique<ara::crypto::cryp::cryservice::CryptoServiceSkeletonImpl>(
            cryptolocalInstanceId, std::move(cryptoUptr))};
    localCryptoService->OfferService();
    cryptoServiceContainer->StoreObjectToContanier(cryptolocalInstanceId, std::move(localCryptoService));
    localCryptoServiceOutput.cryptoServiceInstanceId = cryptolocalInstanceId;
    promise.set_value(localCryptoServiceOutput);
  }
  else
  {
    logger_.LogError()
        << "IPC-CORE:SymmetricBlockCipherContextSkeletonImpl::GetCryptoService() Uptr is nullptr: ";
    promise.set_value(localCryptoServiceOutput);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
