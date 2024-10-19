// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sig_encode_private_ctx_proxy_impl.cpp
//
// Purpose     : Sign encode private context proxy implementation to forward functional calls from application
//               to ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "sig_encode_private_ctx_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
SigEncodePrivateCtxProxyImpl::SigEncodePrivateCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        sigEncCryptoProviderHandler,
    CryptoProvider& cp) noexcept :
    sigEncCryptoProviderHandler_{sigEncCryptoProviderHandler},
    cpUptr_{cp}, logger_{ara::log::CreateLogger(
                     "SEPR", "Sign Encode Private Ctx Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "SigEncodePrivateCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "server client connection for SigEncodePrivateCtx -> success";
  }
  else
  {
    logger_.LogError() << "server client connection for SigEncodePrivateCtx -> fail";
  }
}

ara::core::Result<void> SigEncodePrivateCtxProxyImpl::SetKey(const PrivateKey& key) noexcept
{
  logger_.LogDebug() << "IPC-APP::SEPR SigEncodePrivateCtxProxyImpl Method -> SetKey(const PrivateKey& key)";
  const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl*>(&key)};
  CryptoServiceInstanceId const serviceInstanceId{obj->GetPrivateKeyServiceInstanceId()};
  logger_.LogDebug() << "IPC-APP::SEPR SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = sigEncProxyHandler_->SetKey(serviceInstanceId).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SEPR SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SEPR SetKey() Error: " << keyResult.Error().Message();
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

std::size_t SigEncodePrivateCtxProxyImpl::GetMaxInputSize(bool suppressPadding) const noexcept
{
  std::size_t size{0};
  auto objResult = sigEncProxyHandler_->GetMaxInputSize(suppressPadding).GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SEPR GetMaxInputSize() -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().inputSize;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SigEncodePrivateCtxProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

std::size_t SigEncodePrivateCtxProxyImpl::GetMaxOutputSize(bool suppressPadding) const noexcept
{
  std::size_t size{0};
  auto objResult = sigEncProxyHandler_->GetMaxOutputSize(suppressPadding).GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SigEncodePrivateCtxProxyImpl -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().outputSize;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SigEncodePrivateCtxProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SigEncodePrivateCtxProxyImpl::SignAndEncode(
    ReadOnlyMemRegion in) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SEPR SignAndEncode() with ReadOnlyMemRegion as argument";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto SEPRResult = sigEncProxyHandler_->SignAndEncode(vtrUint8t).GetResult();
  if (SEPRResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SEPR SignAndEncode() -> success";
    auto tempVtr = SEPRResult.Value().vectorBytes;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SEPR SignAndEncode() -> Error: " << SEPRResult.Error().Message();
    if (SEPRResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
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

ara::core::Result<void> SigEncodePrivateCtxProxyImpl::Reset() noexcept
{
  auto resReset = sigEncProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SEPR Reset() -> success";
  }
  return ara::core::Result<void>();
}

CryptoPrimitiveId::Uptr SigEncodePrivateCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SEPR -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = sigEncProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::SEPR -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

bool SigEncodePrivateCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = sigEncProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SEPR -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SEPR -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& SigEncodePrivateCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SEPR -> MyProvider()";
  return cpUptr_;
}

ExtensionService::Uptr SigEncodePrivateCtxProxyImpl::GetExtensionService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SEPR -> GetExtensionService()";
  auto extensionService = sigEncProxyHandler_->GetExtensionService().GetResult();
  if (!extensionService.Value().extensionServiceInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = extensionService.Value().extensionServiceInstanceId;
    logger_.LogDebug() << "IPC-SEPR Service InstanceId for GetExtensionService : " << instanceId;
    ExtensionService::Uptr extensionServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::ExtensionServiceProxyImpl>(instanceId);
    return extensionServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-SEPR Service InstanceId for GetExtensionService is null";
    return nullptr;
  }
}

bool SigEncodePrivateCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipSeprInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for SigEncodePrivateCtx Function: Successful";
    sigEncProxyHandler_ = vsomeipSeprInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for SigEncodePrivateCtx Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
