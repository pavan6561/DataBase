// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetricblock_cipher_ctx_proxy_impl.cpp
//
// Purpose     : Symmetric block cipher proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "symmetricblock_cipher_ctx_proxy_impl.h"

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
SymmetricBlockCipherCtxProxyImpl::SymmetricBlockCipherCtxProxyImpl(
    CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        sbccCPHandler,
    CryptoProvider& cp) noexcept :
    sbccCryptoProviderHandler_{sbccCPHandler},
    cpUptr_{cp}, logger_{ara::log::CreateLogger(
                     "SBCC", "Symmetric Block Cipher Context Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "IPC-APP::SBCC SymmetricBlockCipherCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug()
        << "IPC-APP::SBCC Server client connection for SymmetricBlockCipherCtxProxyImpl -> success";
  }
  else
  {
    logger_.LogError()
        << "IPC-APP::SBCC Server client connection for SymmetricBlockCipherCtxProxyImpl -> fail";
  }
}

bool SymmetricBlockCipherCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = sbccProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SBCC -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SBCC -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& SymmetricBlockCipherCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SBCC -> MyProvider()";
  return cpUptr_;
}

CryptoService::Uptr SymmetricBlockCipherCtxProxyImpl::GetCryptoService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SBCC -> GetCryptoService()";
  auto cryptoService = sbccProxyHandler_->GetCryptoService().GetResult();
  if (!cryptoService.Value().cryptoServiceInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = cryptoService.Value().cryptoServiceInstanceId;
    logger_.LogDebug() << "IPC-APP::SBCC Service InstanceId for GetCryptoService : " << instanceId;
    CryptoService::Uptr cryptoServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::CryptoServiceProxyImpl>(instanceId);
    return cryptoServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SBCC Service InstanceId for GetCryptoService is null";
    return nullptr;
  }
}

CryptoPrimitiveId::Uptr SymmetricBlockCipherCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SBCC -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = sbccProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::SBCC -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

bool SymmetricBlockCipherCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipSbccInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug()
        << "IPC-APP::SBCC Server Client connection for SymmetricBlockCipherCtx Function: Successful";
    sbccProxyHandler_ = vsomeipSbccInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError()
        << "IPC-APP::SBCC Server Client connection for SymmetricBlockCipherCtx Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

ara::core::Result<CryptoTransform> SymmetricBlockCipherCtxProxyImpl::GetTransformation() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SBCC GetTransformation()";
  auto transResult = sbccProxyHandler_->GetTransformation().GetResult();
  if (transResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SBCC GetTransformation(), received value = "
                       << static_cast<uint32_t>(transResult.Value().transform);
    if (transResult.Value().transform == CryptoTransformDataType::kEncrypt)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kEncrypt);
    else if (transResult.Value().transform == CryptoTransformDataType::kDecrypt)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kDecrypt);
    else if (transResult.Value().transform == CryptoTransformDataType::kMacVerify)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kMacVerify);
    else if (transResult.Value().transform == CryptoTransformDataType::kMacGenerate)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kMacGenerate);
    else if (transResult.Value().transform == CryptoTransformDataType::kWrap)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kWrap);
    else if (transResult.Value().transform == CryptoTransformDataType::kUnwrap)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kUnwrap);
    else if (transResult.Value().transform == CryptoTransformDataType::kSigVerify)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kSigVerify);
    else
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kSigGenerate);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SBCC GetTransformation() Error: " << transResult.Error().Message();
    return ara::core::Result<CryptoTransform>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
  }
}

ara::core::Result<void> SymmetricBlockCipherCtxProxyImpl::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  logger_.LogDebug() << "IPC-APP::SBCC SetKey()";
  const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&key);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  CryptoTransformDataType const transformDataType = static_cast<CryptoTransformDataType>(transform);
  logger_.LogDebug() << "IPC::APP SymmetricBlockCipherCtxProxyImpl Method -> SetKey(const SymmetricKey& key)";
  logger_.LogDebug() << "SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = sbccProxyHandler_->SetKey(serviceInstanceId, transformDataType).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SBCC SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SBCC SetKey() Error: " << keyResult.Error().Message();
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

ara::core::Result<void> SymmetricBlockCipherCtxProxyImpl::Reset() noexcept
{
  logger_.LogDebug() << "IPC-APP::SBCC Reset()";
  auto resReset = sbccProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SBCC Reset() -> success";
  }
  return ara::core::Result<void>();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SymmetricBlockCipherCtxProxyImpl::ProcessBlocks(
    ReadOnlyMemRegion in) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SBCC ProcessBlocks(ReadOnlyMemRegion in)";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto processResult = sbccProxyHandler_->ProcessBlocks(vtrUint8t).GetResult();
  if (processResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SBCC ProcessBlocks() -> success";
    auto tempVtr = processResult.Value().vtrByte;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SBCC ProcessBlocks() -> Error: " << processResult.Error().Message();
    if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }
    else if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kIncompatibleArguments);
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
          SecurityErrorDomain::Errc::kInOutBuffersIntersect);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SymmetricBlockCipherCtxProxyImpl::ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SBCC ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding)";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto blockResult = sbccProxyHandler_->ProcessBlock(vtrUint8t, suppressPadding).GetResult();
  if (blockResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SBCC ProcessBlocks() -> success";
    auto tempVtr = blockResult.Value().vtrByte;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SBCC ProcessBlocks() -> Error: " << blockResult.Error().Message();
    if (blockResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInsufficientCapacity);
    }
    else if (blockResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
}

} // namespace cryp
} // namespace crypto
} // namespace ara
