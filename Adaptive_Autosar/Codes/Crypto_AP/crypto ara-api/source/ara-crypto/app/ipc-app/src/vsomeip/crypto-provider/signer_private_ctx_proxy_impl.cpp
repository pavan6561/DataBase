// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signer_private_ctx_proxy_impl.cpp
//
// Purpose     : Signer private context proxy implementation to forward functional calls from application to
//               ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "signer_private_ctx_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
SignerPrivateCtxProxyImpl::SignerPrivateCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        signPrCryptoProviderHandler,
    CryptoProvider& cp) noexcept :
    signPrCryptoProviderHandler_{signPrCryptoProviderHandler},
    cpUptr_{cp}, logger_{
                     ara::log::CreateLogger("SNPR", "SignerPrivateCtx Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "SignerPrivateCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "server client connection for SignerPrivateCtx -> success";
  }
  else
  {
    logger_.LogError() << "server client connection for SignerPrivateCtx -> fail";
  }
}

ara::core::Result<void> SignerPrivateCtxProxyImpl::SetKey(const PrivateKey& key) noexcept
{
  logger_.LogDebug() << "IPC-APP::SNPR SignerPrivateCtxProxyImpl Method -> SetKey(const PrivateKey& key)";
  const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl*>(&key)};
  CryptoServiceInstanceId const serviceInstanceId{obj->GetPrivateKeyServiceInstanceId()};
  logger_.LogDebug() << "SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = signPrProxyHandler_->SetKey(serviceInstanceId).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SNPR SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SNPR SetKey() Error: " << keyResult.Error().Message();
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

ara::core::Result<Signature::Uptrc> SignerPrivateCtxProxyImpl::SignPreHashed(
    const HashFunctionCtx& hashFn, ReadOnlyMemRegion context) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SNPR SignPreHashed()";
  const ara::crypto::cryp::HashFunctionCtxProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::HashFunctionCtxProxyImpl*>(&hashFn)};
  CryptoServiceInstanceId const serviceInstanceId{obj->GetHashFunctionCtxServiceInstanceId()};
  logger_.LogDebug() << "SignPreHashed() HashFunctionCtx CryptoServiceInstanceId : " << serviceInstanceId;
  CryptoReadOnlyMemRegion vtrContext(context.begin(), context.end());
  auto tempResult = signPrProxyHandler_->SignPreHashedHashFunction(serviceInstanceId, vtrContext).GetResult();
  if (tempResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = tempResult.Value().signatureInstanceId;
    logger_.LogDebug() << "IPC-APP::SNPR Service InstanceId for Signature Object : " << instanceId;
    Signature::Uptrc signatureUptrc
        = std::make_unique<ara::crypto::cryp::cryobj::SignatureProxyImpl>(instanceId);
    return ara::core::Result<Signature::Uptrc>(std::move(signatureUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP::SNPR SignPreHashed() -> Error: " << tempResult.Error().Message();
    if (tempResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument)
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kInvalidArgument);
    }
    else if (tempResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (tempResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished)
    {
      return ara::core::Result<Signature::Uptrc>::FromError(
          SecurityErrorDomain::Errc::kProcessingNotFinished);
    }
    else
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SignerPrivateCtxProxyImpl::Sign(
    ReadOnlyMemRegion value, ReadOnlyMemRegion context) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SNPR SignReadOnly()";
  CryptoReadOnlyMemRegion vtrValue(value.begin(), value.end());
  CryptoReadOnlyMemRegion vtrContext(context.begin(), context.end());
  auto tempResult = signPrProxyHandler_->SignReadOnly(vtrValue, vtrContext).GetResult();
  if (tempResult.HasValue())
  {
    auto tempVtr = tempResult.Value().vectorBytes;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SNPR SignReadOnly() -> Error: " << tempResult.Error().Message();
    if (tempResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
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

ara::core::Result<Signature::Uptrc> SignerPrivateCtxProxyImpl::SignPreHashed(
    AlgId hashAlgId, ReadOnlyMemRegion hashValue, ReadOnlyMemRegion context) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SNPR SignPreHashed()";
  CryptoReadOnlyMemRegion vtrHash(hashValue.begin(), hashValue.end());
  CryptoReadOnlyMemRegion vtrContext(context.begin(), context.end());
  auto tempResult = signPrProxyHandler_->SignPreHashedReadOnly(hashAlgId, vtrHash, vtrContext).GetResult();
  if (tempResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = tempResult.Value().signatureInstanceId;
    logger_.LogDebug() << "IPC-APP::SNPR Service InstanceId for Signature Object : " << instanceId;
    Signature::Uptrc signatureUptrc
        = std::make_unique<ara::crypto::cryp::cryobj::SignatureProxyImpl>(instanceId);
    return ara::core::Result<Signature::Uptrc>(std::move(signatureUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP::SNPR SignPreHashed() -> Error: " << tempResult.Error().Message();
    if (tempResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument)
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kInvalidArgument);
    }
    else if (tempResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
}

ara::core::Result<void> SignerPrivateCtxProxyImpl::Reset() noexcept
{
  auto resReset = signPrProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SNPR Reset() -> success";
  }
  return ara::core::Result<void>();
}

CryptoPrimitiveId::Uptr SignerPrivateCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SNPR -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = signPrProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::SNPR -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

bool SignerPrivateCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = signPrProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SNPR -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SNPR -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& SignerPrivateCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SNPR -> MyProvider()";
  return cpUptr_;
}

SignatureService::Uptr SignerPrivateCtxProxyImpl::GetSignatureService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SNPR -> GetSignatureService()";
  auto signatureService = signPrProxyHandler_->GetSignatureService().GetResult();
  if (!signatureService.Value().signatureServiceInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = signatureService.Value().signatureServiceInstanceId;
    logger_.LogDebug() << "IPC-SNPR Service InstanceId for GetSignatureService : " << instanceId;
    SignatureService::Uptr signatureServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::SignatureServiceProxyImpl>(instanceId);
    return signatureServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-SNPR Service InstanceId for GetSignatureService is null";
    return nullptr;
  }
}

bool SignerPrivateCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipSnprInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for SignerPrivateCtx Function: Successful";
    signPrProxyHandler_ = vsomeipSnprInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for SignerPrivateCtx Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
