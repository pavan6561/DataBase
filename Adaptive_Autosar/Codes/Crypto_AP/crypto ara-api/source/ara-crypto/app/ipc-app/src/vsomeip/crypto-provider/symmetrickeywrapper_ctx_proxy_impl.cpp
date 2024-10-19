// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetrickeywrapper_ctx_proxy_impl.cpp
//
// Purpose     : Symmetric key wrapper proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "symmetrickeywrapper_ctx_proxy_impl.h"

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
SymmetrickeyWrapperCtxProxyImpl::SymmetrickeyWrapperCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        skwcCPHandler,
    CryptoProvider& cp) noexcept :
    skwcCryptoProviderHandler_{skwcCPHandler},
    cpUptr_{cp}, logger_{ara::log::CreateLogger(
                     "SKWC", "Symmetric Key Wrapper Context Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "IPC-APP::SKWC SymmetricBlockCipherCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug()
        << "IPC-APP::SKWC server client connection for SymmetrickeyWrapperCtxProxyImpl -> success";
  }
  else
  {
    logger_.LogError()
        << "IPC-APP::SKWC server client connection for SymmetrickeyWrapperCtxProxyImpl -> fail";
  }
}

bool SymmetrickeyWrapperCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipSkwcInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug()
        << "IPC-APP::SKWC Server Client connection for SymmetricBlockCipherCtx Function: Successful";
    skwcProxyHandler_ = vsomeipSkwcInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError()
        << "IPC-APP::SKWC Server Client connection for SymmetricBlockCipherCtx Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

bool SymmetrickeyWrapperCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = skwcProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SKWC -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SKWC -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& SymmetrickeyWrapperCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SKWC -> MyProvider()";
  return cpUptr_;
}

ExtensionService::Uptr SymmetrickeyWrapperCtxProxyImpl::GetExtensionService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SKWC -> GetExtensionService()";
  auto extensionService = skwcProxyHandler_->GetExtensionService().GetResult();
  if (!extensionService.Value().extensionServiceInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = extensionService.Value().extensionServiceInstanceId;
    logger_.LogDebug() << "IPC-SKWC Service InstanceId for GetExtensionService : " << instanceId;
    ExtensionService::Uptr extensionServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::ExtensionServiceProxyImpl>(instanceId);
    return extensionServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-SKWC Service InstanceId for GetExtensionService is null";
    return nullptr;
  }
}

CryptoPrimitiveId::Uptr SymmetrickeyWrapperCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SKWC -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = skwcProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::SKWC -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

std::size_t SymmetrickeyWrapperCtxProxyImpl::GetTargetKeyGranularity() const noexcept
{
  std::size_t size{0};
  auto objResult = skwcProxyHandler_->GetTargetKeyGranularity().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SKWC -> GetTargetKeyGranularity() : Success";
    size = objResult.Value().keySize;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SKWC -> GetTargetKeyGranularity() : Failed";
  }
  return size;
}

std::size_t SymmetrickeyWrapperCtxProxyImpl::GetMaxTargetKeyLength() const noexcept
{
  std::size_t size{0};
  auto objResult = skwcProxyHandler_->GetMaxTargetKeyLength().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SKWC -> GetMaxTargetKeyLength() : Success";
    size = objResult.Value().keyLengthSize;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SKWC -> GetMaxTargetKeyLength() : Failed";
  }
  return size;
}

std::size_t SymmetrickeyWrapperCtxProxyImpl::CalculateWrappedKeySize(std::size_t keyLength) const noexcept
{
  std::size_t size{0};
  auto objResult = skwcProxyHandler_->CalculateWrappedKeySize(keyLength).GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SKWC -> CalculateWrappedKeySize() : Success";
    size = objResult.Value().calculatedSize;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SKWC -> CalculateWrappedKeySize() : Failed";
  }
  return size;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SymmetrickeyWrapperCtxProxyImpl::WrapKeyMaterial(
    const RestrictedUseObject& key) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SKWC WrapKeyMaterial(const RestrictedUseObject& key)";
  CryptoServiceInstanceId serviceInstanceId;
  CryptoObjectType cryptoObjType = key.cryObjType;
  logger_.LogDebug() << "IPC-APP::SKWC  cryptoObjectType = " << static_cast<uint32_t>(cryptoObjType);
  if (cryptoObjType == CryptoObjectType::kSecretSeed)
  {
    const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&key);
    serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kSymmetricKey)
  {
    const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&key);
    serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kPublicKey)
  {
    // ToDo: Public key is not implemented for now as underlying crypto daemon doesn't
    // support, below implementation to be considered in future.
  }
  else if (cryptoObjType == CryptoObjectType::kPrivateKey)
  {
    // ToDo: Private key is not implemented for now as underlying crypto daemon
    // doesn't support, below implementation to be considered in future.
  }

  RestrictedUseObjectStructure restrictedUseObj;
  restrictedUseObj.RestrictedUseInstanceId = serviceInstanceId;
  restrictedUseObj.RestrictedUseObjectType = static_cast<CryptoObjectTypeIPC>(cryptoObjType);
  logger_.LogDebug() << "IPC-APP::SKWC RestrictedUseObject serviceInstanceId = " << serviceInstanceId;
  // Invoke of WrapKeyMaterial for established proxy handler
  auto keyResult = skwcProxyHandler_->WrapKeyMaterial(restrictedUseObj).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SKWC WrapKeyMaterial() -> success ";
    auto tempVtr = keyResult.Value().vtrByte;
    ara::core::Vector<ara::core::Byte> vectorbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vectorbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SKWC WrapKeyMaterial() Error: " << keyResult.Error().Message();
    if (keyResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInsufficientCapacity);
    }
    else if (keyResult.Error()
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

ara::core::Result<RestrictedUseObject::Uptrc> SymmetrickeyWrapperCtxProxyImpl::UnwrapKey(
    ReadOnlyMemRegion wrappedKey, AlgId algId, AllowedUsageFlags allowedUsage) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SKWC UnwrapKey()";
  RestrictedUseObject::Uptrc restrictedUseObjUptrc{nullptr};
  CryptoReadOnlyMemRegion vtrUint8t(wrappedKey.begin(), wrappedKey.end());
  ara::core::Result<
      ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::UnwrapKeyOutput>
      unwrapResult = skwcProxyHandler_->UnwrapKey(vtrUint8t, algId, allowedUsage).GetResult();
  if (unwrapResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SKWC UnwrapKey() -> success";
    CryptoServiceInstanceId instanceId = unwrapResult.Value().restrictedUseObjInstanceId;
    logger_.LogDebug() << "IPC::APP Service InstanceId for UnwrapKey() : " << instanceId;
    restrictedUseObjUptrc
        = std::make_unique<ara::crypto::cryp::cryobj::RestrictedUseObjectProxyImpl>(instanceId);
    return ara::core::Result<RestrictedUseObject::Uptrc>::FromValue(std::move(restrictedUseObjUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP::SKWC UnwrapKey() -> Error: " << unwrapResult.Error().Message();
    if (unwrapResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<RestrictedUseObject::Uptrc>::FromError(
          SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else
    {
      return ara::core::Result<RestrictedUseObject::Uptrc>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
}

ara::core::Result<SecretSeed::Uptrc> SymmetrickeyWrapperCtxProxyImpl::UnwrapSeed(
    ReadOnlyMemRegion wrappedSeed, AlgId targetAlgId, SecretSeed::Usage allowedUsage) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SKWC UnwrapSeed()";
  SecretSeed::Uptrc secretSeedUptrc{nullptr};
  CryptoReadOnlyMemRegion localSeed(wrappedSeed.begin(), wrappedSeed.end());
  auto secretSeedResult = skwcProxyHandler_->UnwrapSeed(localSeed, targetAlgId, allowedUsage).GetResult();
  if (secretSeedResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = secretSeedResult.Value().secretSeedUptrcInstanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for GenerateSeed : " << instanceId;
    secretSeedUptrc = std::make_unique<ara::crypto::cryp::cryobj::SecretSeedProxyImpl>(instanceId);
    return ara::core::Result<SecretSeed::Uptrc>(std::move(secretSeedUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP GenerateSeed() Error: " << secretSeedResult.Error().Message();
    if (secretSeedResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<SecretSeed::Uptrc>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else
    {
      return ara::core::Result<SecretSeed::Uptrc>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
}

ara::core::Result<void> SymmetrickeyWrapperCtxProxyImpl::Reset() noexcept
{
  auto resReset = skwcProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SKWC Reset() -> success";
  }
  return ara::core::Result<void>();
}

ara::core::Result<void> SymmetrickeyWrapperCtxProxyImpl::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  logger_.LogDebug() << "IPC-APP::SKWC  SetKey(const SymmetricKey& key)";
  const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&key);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  CryptoTransformDataType const transformDataType = static_cast<CryptoTransformDataType>(transform);
  logger_.LogDebug() << "SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = skwcProxyHandler_->SetKey(serviceInstanceId, transformDataType).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SKWC SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SKWC SetKey() Error: " << keyResult.Error().Message();
    if (keyResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kIncompatibleObject);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
    }
  }
}

} // namespace cryp
} // namespace crypto
} // namespace ara
