// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : decryptor_private_ctx_proxy_impl.cpp
//
// Purpose     : Decryptor private context proxy implementation to forward functional calls from application
//               to ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "decryptor_private_ctx_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
DecryptorPrivateCtxProxyImpl::DecryptorPrivateCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        decPrCryptoProviderHandler,
    CryptoProvider& cp) noexcept :
    decPrCryptoProviderHandler_{decPrCryptoProviderHandler},
    cpUptr_{cp}, logger_{ara::log::CreateLogger(
                     "DEPR", "Decryptor Private Context Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "DecryptorPrivateCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "server client connection for DecryptorPrivateCtx -> success";
  }
  else
  {
    logger_.LogError() << "server client connection for DecryptorPrivateCtx -> fail";
  }
}

CryptoService::Uptr DecryptorPrivateCtxProxyImpl::GetCryptoService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC -> GetCryptoService()";
  auto cryptoService = decPrProxyHandler_->GetCryptoService().GetResult();
  if (!cryptoService.Value().cryptoInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = cryptoService.Value().cryptoInstanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for GetCryptoService : " << instanceId;
    CryptoService::Uptr cryptoServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::CryptoServiceProxyImpl>(instanceId);
    return cryptoServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP Service InstanceId for GetCryptoService is null";
    return nullptr;
  }
}

ara::core::Result<void> DecryptorPrivateCtxProxyImpl::SetKey(const PrivateKey& key) noexcept
{
  logger_.LogDebug() << "IPC-APP::DEPR DecryptorPrivateCtxProxyImpl Method -> SetKey(const PrivateKey& key)";
  const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl*>(&key);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetPrivateKeyServiceInstanceId();
  logger_.LogDebug() << "IPC-APP::DEPR SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = decPrProxyHandler_->SetKey(serviceInstanceId).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::DEPR SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::DEPR SetKey() Error: " << keyResult.Error().Message();
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

ara::core::Result<ara::core::Vector<ara::core::Byte>> DecryptorPrivateCtxProxyImpl::ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding) const noexcept
{
  logger_.LogDebug() << "IPC-APP::DEPR ProcessBlock() with ReadOnlyMemRegion as argument";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto processResult = decPrProxyHandler_->ProcessBlockReadOnly(vtrUint8t, suppressPadding).GetResult();
  if (processResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::DEPR ProcessBlock() -> success";
    auto tempVtr = processResult.Value().vectorBytes;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::DEPR ProcessBlocks() -> Error: " << processResult.Error().Message();
    if (processResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (processResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInsufficientCapacity);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
}

ara::core::Result<void> DecryptorPrivateCtxProxyImpl::Reset() noexcept
{
  auto resReset = decPrProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::DEPR Reset() -> success";
  }
  return ara::core::Result<void>();
}

CryptoPrimitiveId::Uptr DecryptorPrivateCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DEPR -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = decPrProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::ACC -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

bool DecryptorPrivateCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = decPrProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::DEPR -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::DEPR -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& DecryptorPrivateCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DEPR -> MyProvider()";
  return cpUptr_;
}

bool DecryptorPrivateCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipDpcInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for DecryptorPrivateCtx Function: Successful";
    decPrProxyHandler_ = vsomeipDpcInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for DecryptorPrivateCtx Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
