// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : msg_recovery_public_ctx_proxy_impl.cpp
//
// Purpose     : Message recovery public context proxy implementation to forward functional calls from
//               application to ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "msg_recovery_public_ctx_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
MsgRecoveryPublicCtxProxyImpl::MsgRecoveryPublicCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        msgRecCryptoProviderHandler,
    CryptoProvider& cp) noexcept :
    msgRecCryptoProviderHandler_{msgRecCryptoProviderHandler},
    cpUptr_{cp}, logger_{ara::log::CreateLogger(
                     "MRPB", "MsgRecoveryPublicCtx Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "MsgRecoveryPublicCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "server client connection for MsgRecoveryPublicCtx -> success";
  }
  else
  {
    logger_.LogError() << "server client connection for MsgRecoveryPublicCtxss -> fail";
  }
}

ara::core::Result<void> MsgRecoveryPublicCtxProxyImpl::SetKey(const PublicKey& key) noexcept
{
  logger_.LogDebug() << "IPC-APP::MRPB MsgRecoveryPublicCtxProxyImpl Method -> SetKey(const PublicKey& key)";
  const ara::crypto::cryp::cryobj::PublicKeyProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::cryobj::PublicKeyProxyImpl*>(&key)};
  CryptoServiceInstanceId const serviceInstanceId{obj->GetPublicKeyServiceInstanceId()};
  logger_.LogDebug() << "SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = msgRecProxyHandler_->SetKey(serviceInstanceId).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MRPB SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::MRPB SetKey() Error: " << keyResult.Error().Message();
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

std::size_t MsgRecoveryPublicCtxProxyImpl::GetMaxInputSize(bool suppressPadding) const noexcept
{
  std::size_t size{0};
  auto objResult = msgRecProxyHandler_->GetMaxInputSize(suppressPadding).GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MRPB -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().inputSize;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::MRPB -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

std::size_t MsgRecoveryPublicCtxProxyImpl::GetMaxOutputSize(bool suppressPadding) const noexcept
{
  std::size_t size{0};
  auto objResult = msgRecProxyHandler_->GetMaxOutputSize(suppressPadding).GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MRPB -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().outputSize;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::MRPB -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> MsgRecoveryPublicCtxProxyImpl::DecodeAndVerify(
    ReadOnlyMemRegion in) const noexcept
{
  logger_.LogDebug() << "IPC-APP::MRPB DecodeAndVerify() with ReadOnlyMemRegion as argument";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto msgResult = msgRecProxyHandler_->DecodeAndVerify(vtrUint8t).GetResult();
  if (msgResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MRPB DecodeAndVerify() -> success";
    auto tempVtr = msgResult.Value().vectorBytes;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::MRPB DecodeAndVerify() -> Error: " << msgResult.Error().Message();
    if (msgResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
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

ara::core::Result<void> MsgRecoveryPublicCtxProxyImpl::Reset() noexcept
{
  auto resReset = msgRecProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MRPB Reset() -> success";
  }
  return ara::core::Result<void>();
}

CryptoPrimitiveId::Uptr MsgRecoveryPublicCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::MRPB -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = msgRecProxyHandler_->GetCryptoPrimitiveId().GetResult();
  if (primitiveResult.HasValue())
  {
    const ara::core::StringView primitiveName{
        primitiveResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName};
    const AlgId algorithmId{primitiveResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId};
    cryptoPrimitiveIdUptrLocal
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::MRPB -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
  }
  return cryptoPrimitiveIdUptrLocal;
}

bool MsgRecoveryPublicCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = msgRecProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MRPB -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::MRPB -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& MsgRecoveryPublicCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::MRPB -> MyProvider()";
  return cpUptr_;
}

ExtensionService::Uptr MsgRecoveryPublicCtxProxyImpl::GetExtensionService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::MRPB -> GetExtensionService()";
  auto extensionService = msgRecProxyHandler_->GetExtensionService().GetResult();
  if (!extensionService.Value().extensionServiceInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = extensionService.Value().extensionServiceInstanceId;
    logger_.LogDebug() << "IPC-MRPB Service InstanceId for GetExtensionService : " << instanceId;
    ExtensionService::Uptr extensionServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::ExtensionServiceProxyImpl>(instanceId);
    return extensionServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-MRPB Service InstanceId for GetExtensionService is null";
    return nullptr;
  }
}

bool MsgRecoveryPublicCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipMrpcInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for MsgRecoveryPublicCtx Function: Successful";
    msgRecProxyHandler_ = vsomeipMrpcInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for MsgRecoveryPublicCtx Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
