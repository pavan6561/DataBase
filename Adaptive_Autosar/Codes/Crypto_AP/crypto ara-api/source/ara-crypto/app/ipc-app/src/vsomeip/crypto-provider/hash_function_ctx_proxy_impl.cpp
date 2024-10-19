// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : hash_function_ctx_proxy_impl.cpp
//
// Purpose     : Hash function proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "hash_function_ctx_proxy_impl.h"

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
HashFunctionCtxProxyImpl::HashFunctionCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        hashCPHandler,
    CryptoProvider& cp) noexcept :
    hashCryptoProviderHandler_{hashCPHandler},
    hashFunctionServiceInstanceID_{instanceValue}, cpUptr_{cp}, logger_{ara::log::CreateLogger("HASH",
                                                                    "Hash Function Ctx Proxy",
                                                                    ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "HashFunctionCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "server client connection for HashFunctionCtx -> success";
  }
  else
  {
    logger_.LogError() << "server client connection for HashFunctionCtx -> fail";
  }
}

bool HashFunctionCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = hashProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::Hash -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::Hash -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& HashFunctionCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::Hash -> MyProvider()";
  return cpUptr_;
}

DigestService::Uptr HashFunctionCtxProxyImpl::GetDigestService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::Hash -> GetDigestService()";
  auto digestService = hashProxyHandler_->GetDigestService().GetResult();
  if (!digestService.Value().instanceId.empty())
  {
    CryptoServiceInstanceId instanceId = digestService.Value().instanceId;
    logger_.LogDebug() << "IPC-APP::Hash Service InstanceId for GetDigestService : " << instanceId;
    DigestService::Uptr digestServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::DigestServiceProxyImpl>(instanceId);
    return digestServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::Hash Service InstanceId for GetDigestService is null";
    return nullptr;
  }
}

CryptoPrimitiveId::Uptr HashFunctionCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::Hash -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = hashProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::Hash -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

ara::core::Result<void> HashFunctionCtxProxyImpl::Start(ReadOnlyMemRegion iv) noexcept
{
  logger_.LogDebug() << "IPC-APP HashFunctionCtxProxyImpl::" << __FUNCTION__;
  CryptoReadOnlyMemRegion vtrUint8t(iv.begin(), iv.end());
  auto resultStart = hashProxyHandler_->StartReadOnlyMem(vtrUint8t).GetResult();
  if (resultStart.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::Hash StartReadOnlyMem() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::Hash StartReadOnlyMem() Error: " << resultStart.Error().Message();
    if (resultStart.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (resultStart.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUnsupported);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUnsupported);
    }
  }
}

ara::core::Result<void> HashFunctionCtxProxyImpl::Start() noexcept
{
  logger_.LogDebug() << "IPC-APP::Hash StartNoParam() called";
  auto resultStart = hashProxyHandler_->StartNoParam().GetResult();
  if (resultStart.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::Hash StartNoParam() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::Hash StartNoParam() Error: " << resultStart.Error().Message();
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kMissingArgument);
  }
}

ara::core::Result<void> HashFunctionCtxProxyImpl::Start(const SecretSeed& iv) noexcept
{
  logger_.LogDebug() << "IPC-APP::Hash Method -> Start(const SecretSeed& iv)";
  const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&iv);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  logger_.LogDebug() << "Start() CryptoServiceInstanceId : " << serviceInstanceId;
  auto startResult = hashProxyHandler_->StartSecretSeed(serviceInstanceId).GetResult();
  if (startResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::Hash StartSecretSeed() -> Success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::Hash StartSecretSeed() -> Error: " << startResult.Error().Message();
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

ara::core::Result<void> HashFunctionCtxProxyImpl::Update(const RestrictedUseObject& in) noexcept
{
  logger_.LogDebug() << "IPC-APP::HASH HashFunctionCtxProxyImpl::" << __FUNCTION__;
  CryptoServiceInstanceId serviceInstanceId;
  CryptoObjectType cryptoObjType = in.cryObjType;
  logger_.LogDebug() << "IPC-APP::HASH  cryptoObjectType =" << static_cast<uint32_t>(cryptoObjType);
  if (cryptoObjType == CryptoObjectType::kSecretSeed)
  {
    const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&in);
    serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kSymmetricKey)
  {
    const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&in);
    serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kPublicKey)
  {
    const ara::crypto::cryp::cryobj::PublicKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::PublicKeyProxyImpl*>(&in);
    serviceInstanceId = obj->GetPublicKeyServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kPrivateKey)
  {
    const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl*>(&in);
    serviceInstanceId = obj->GetPrivateKeyServiceInstanceId();
  }

  RestrictedUseObjectStructure restrictedUseObj;
  restrictedUseObj.RestrictedUseInstanceId = serviceInstanceId;
  restrictedUseObj.RestrictedUseObjectType = static_cast<CryptoObjectTypeIPC>(cryptoObjType);
  logger_.LogDebug() << "IPC-APP::HASH RestrictedUseObject serviceInstanceId = " << serviceInstanceId;
  // Invoke Update with RestrictedUseObject as parameter using established proxy handler
  auto resultUpdate = hashProxyHandler_->UpdateRestrictedUseObject(restrictedUseObj).GetResult();
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::Hash UpdateRestrictedUseObject() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::Hash UpdateRestrictedUseObject() Error: "
                       << resultUpdate.Error().Message();
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
  }
}

ara::core::Result<void> HashFunctionCtxProxyImpl::Update(ReadOnlyMemRegion in) noexcept
{
  logger_.LogDebug() << "IPC-APP::Hash UpdateReadOnlyMem() called";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto resultUpdate = hashProxyHandler_->UpdateReadOnlyMem(vtrUint8t).GetResult();
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::Hash UpdateReadOnlyMem() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::Hash UpdateReadOnlyMem() Error: " << resultUpdate.Error().Message();
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
  }
}

ara::core::Result<void> HashFunctionCtxProxyImpl::Update(std::uint8_t in) noexcept
{
  auto resultUpdate = hashProxyHandler_->Update(in).GetResult();
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::Hash Update() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::Hash Update() Error: " << resultUpdate.Error().Message();
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> HashFunctionCtxProxyImpl::Finish() noexcept
{
  logger_.LogDebug() << "IPC-APP::Hash Finish()";
  auto tempResult = hashProxyHandler_->Finish().GetResult();
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::Hash Finish() -> success";
    auto tempVtr = tempResult.Value().finishResult;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::Hash Finish() -> Error: " << tempResult.Error().Message();
    if (tempResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
    else if (tempResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidUsageOrder)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInvalidUsageOrder);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInvalidUsageOrder);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> HashFunctionCtxProxyImpl::GetDigest(
    std::size_t offset) const noexcept
{
  logger_.LogDebug() << "IPC-APP::Hash GetDigest()";
  auto resDigest = hashProxyHandler_->GetDigest(static_cast<uint64>(offset)).GetResult();
  if (resDigest.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::Hash GetDigest() -> success";
    auto tempVtr = resDigest.Value().digestResult;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::Hash GetDigest() Error: " << resDigest.Error().Message();
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        SecurityErrorDomain::Errc::kProcessingNotFinished);
  }
}

bool HashFunctionCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipHashInstance.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for Hash Function : Successful";
    hashProxyHandler_ = vsomeipHashInstance.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for Hash Function : Failed";
    isSuccess = false;
  }
  return isSuccess;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
