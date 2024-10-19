// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : streamcipher_context_proxy_impl.cpp
//
// Purpose     : Stream cipher proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "streamcipher_context_proxy_impl.h"

#include <algorithm>
#include <iostream>
#include <unistd.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
StreamCipherContextProxyImpl::StreamCipherContextProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        sccCPHandler,
    CryptoProvider& cp) noexcept :
    sccCryptoProviderHandler_{sccCPHandler},
    cpUptr_{cp}, logger_{ara::log::CreateLogger(
                     "SCCP", "Symmetric Cipher Context Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "IPC-APP::SCC StreamCipherContextProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "IPC-APP::SCC Server client connection for StreamCipherContext -> success";
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC Server client connection for StreamCipherContext -> fail";
  }
}

bool StreamCipherContextProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = sccProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SCC -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& StreamCipherContextProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC -> MyProvider()";
  return cpUptr_;
}

BlockService::Uptr StreamCipherContextProxyImpl::GetBlockService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC -> GetBlockService()";
  auto blockService = sccProxyHandler_->GetBlockService().GetResult();
  if (!blockService.Value().blockServiceInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = blockService.Value().blockServiceInstanceId;
    logger_.LogDebug() << "IPC-APP::SCC Service InstanceId for GetBlockService : " << instanceId;
    BlockService::Uptr blockServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::BlockServiceProxyImpl>(instanceId);
    return blockServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SCC Service InstanceId for GetBlockService is null";
    return nullptr;
  }
}

CryptoPrimitiveId::Uptr StreamCipherContextProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = sccProxyHandler_->GetCryptoPrimitiveId().GetResult();
  if (primitiveResult.HasValue())
  {
    const ara::core::StringView primitiveName
        = primitiveResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName;
    const AlgId algorithmId = primitiveResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId;
    cryptoPrimitiveIdUptrLocal
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
    return cryptoPrimitiveIdUptrLocal;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

bool StreamCipherContextProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipSccInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug()
        << "IPC-APP::SCC Server Client connection for StreamCipherContext Function: Successful";
    sccProxyHandler_ = vsomeipSccInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC Server Client connection for StreamCipherContext Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

bool StreamCipherContextProxyImpl::IsBytewiseMode() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC IsBytewiseMode()";
  auto result = sccProxyHandler_->IsBytewiseMode().GetResult();
  return result.Value().isSuccess;
}

bool StreamCipherContextProxyImpl::IsSeekableMode() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC IsSeekableMode()";
  auto result = sccProxyHandler_->IsSeekableMode().GetResult();
  return result.Value().isSuccess;
}

ara::core::Result<void> StreamCipherContextProxyImpl::Seek(std::int64_t offset, bool fromBegin) noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC Seek()";
  auto seekResult = sccProxyHandler_->Seek(offset, fromBegin).GetResult();
  if (seekResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC Seek() -> success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC Seek() Error: " << seekResult.Error().Message();
    if (seekResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
    else if (seekResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUnsupported);
    }
    else if (seekResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kBelowBoundary)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kBelowBoundary);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidArgument);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> StreamCipherContextProxyImpl::ProcessBlocks(
    ReadOnlyMemRegion in) noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC ProcessBlocks() with ReadOnlyMemRegion as argument";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto processResult = sccProxyHandler_->ProcessBlocks(vtrUint8t).GetResult();
  if (processResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC ProcessBlocks() -> success";
    auto tempVtr = processResult.Value().vtrByte;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC ProcessBlocks() -> Error: " << processResult.Error().Message();
    if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    else if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInOutBuffersIntersect)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInOutBuffersIntersect);
    }
    else if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidUsageOrder)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInvalidUsageOrder);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
  }
}

ara::core::Result<void> StreamCipherContextProxyImpl::ProcessBlocks(ReadWriteMemRegion inOut) noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC ProcessConfidentialData() with ReadWriteMemRegion as argument";
  CryptoReadOnlyMemRegion vtrUint8t(inOut.begin(), inOut.end());
  auto tempResult = sccProxyHandler_->ProcessBlocks(vtrUint8t).GetResult();
  if (tempResult.HasValue())
  {
    logger_.LogDebug()
        << "IPC-APP::SCC ProcessConfidentialData() with ReadWriteMemRegion arguement -> success";
    for (uint16_t i = 0; i < inOut.size(); i++)
    {
      inOut[i] = tempResult.Value().vtrByte[i];
    }
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC ProcessBlocks -> Error: " << tempResult.Error().Message();
    if (tempResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (tempResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidUsageOrder)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidUsageOrder);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> StreamCipherContextProxyImpl::ProcessBytes(
    ReadOnlyMemRegion in) noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC ProcessBytes()";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto processResult = sccProxyHandler_->ProcessByte(vtrUint8t).GetResult();
  if (processResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC ProcessBytes() -> success";
    auto tempVtr = processResult.Value().vtrByte;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC ProcessBytes() -> Error: " << processResult.Error().Message();
    if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInsufficientCapacity);
    }
    else if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInOutBuffersIntersect)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInOutBuffersIntersect);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> StreamCipherContextProxyImpl::FinishBytes(
    ReadOnlyMemRegion in) noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC FinishBytes()";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto finishResult = sccProxyHandler_->FinishBytes(vtrUint8t).GetResult();
  if (finishResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC ProcessBytes() -> success";
    auto tempVtr = finishResult.Value().vtrByte;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC ProcessBytes() -> Error: " << finishResult.Error().Message();
    if (finishResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInsufficientCapacity);
    }
    else if (finishResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInOutBuffersIntersect)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInOutBuffersIntersect);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
  }
}

ara::core::Result<void> StreamCipherContextProxyImpl::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC SetKey()";
  const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&key);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  CryptoTransformDataType const transformDataType = static_cast<CryptoTransformDataType>(transform);
  logger_.LogDebug() << "IPC::APP StreamCipherContextProxyImpl Method -> SetKey(const SymmetricKey& key)";
  logger_.LogDebug() << "SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = sccProxyHandler_->SetKey(serviceInstanceId, transformDataType).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC SetKey() Error: " << keyResult.Error().Message();
    if (keyResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kIncompatibleObject);
    }
    else if (keyResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
    }
  }
}

std::size_t StreamCipherContextProxyImpl::CountBytesInCache() const noexcept
{
  auto result = sccProxyHandler_->CountBytesInCache().GetResult();
  return static_cast<std::size_t>(result.Value().countSize);
}

ara::core::Result<CryptoTransform> StreamCipherContextProxyImpl::GetTransformation() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC GetTransformation()";
  auto transform = sccProxyHandler_->GetTransformation().GetResult();
  if (transform.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC GetTransformation(), received value = "
                       << static_cast<uint32_t>(transform.Value().transfor);
    if (transform.Value().transfor == CryptoTransformDataType::kEncrypt)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kEncrypt);
    else if (transform.Value().transfor == CryptoTransformDataType::kDecrypt)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kDecrypt);
    else if (transform.Value().transfor == CryptoTransformDataType::kMacVerify)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kMacVerify);
    else if (transform.Value().transfor == CryptoTransformDataType::kMacGenerate)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kMacGenerate);
    else if (transform.Value().transfor == CryptoTransformDataType::kWrap)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kWrap);
    else if (transform.Value().transfor == CryptoTransformDataType::kUnwrap)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kUnwrap);
    else if (transform.Value().transfor == CryptoTransformDataType::kSigVerify)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kSigVerify);
    else
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kSigGenerate);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC GetTransformation() Error: " << transform.Error().Message();
    return ara::core::Result<CryptoTransform>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
  }
}

ara::core::Result<void> StreamCipherContextProxyImpl::Start(ReadOnlyMemRegion iv) noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC Start(ReadOnlyMemRegion iv)";
  CryptoReadOnlyMemRegion vtrUint8t(iv.begin(), iv.end());
  auto startResult = sccProxyHandler_->StartReadOnlyMem(vtrUint8t).GetResult();
  if (startResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC Start() -> success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC Start() -> Error: " << startResult.Error().Message();
    if (startResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
    }
    else if (startResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUnsupported);
    }
  }
}

ara::core::Result<void> StreamCipherContextProxyImpl::Start(const SecretSeed& iv) noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC Start(const SecretSeed& iv)";
  const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&iv);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  logger_.LogDebug() << "IPC-APP::SCC Start() CryptoServiceInstanceId : " << serviceInstanceId;
  auto startResult = sccProxyHandler_->StartSecretSeed(serviceInstanceId).GetResult();
  if (startResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC StartSecretSeed() -> Success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SCC StartSecretSeed() -> Error: " << startResult.Error().Message();
    if (startResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
    }
    else if (startResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (startResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUnsupported);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
    }
  }
}

ara::core::Result<void> StreamCipherContextProxyImpl::Reset() noexcept
{
  logger_.LogDebug() << "IPC-APP::SCC Reset";
  auto resReset = sccProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SCC Reset() -> success";
  }
  return ara::core::Result<void>();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
