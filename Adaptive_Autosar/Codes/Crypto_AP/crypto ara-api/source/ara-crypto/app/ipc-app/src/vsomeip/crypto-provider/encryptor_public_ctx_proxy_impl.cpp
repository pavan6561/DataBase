// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : encryptor_public_ctx_proxy_impl.cpp
//
// Purpose     : Encryptor public context proxy implementation to forward functional calls from application to
//               ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "encryptor_public_ctx_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
EncryptorPublicCtxProxyImpl::EncryptorPublicCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        encPubCryptoProviderHandler,
    CryptoProvider& cp) noexcept :
    encPubCryptoProviderHandler_{encPubCryptoProviderHandler},
    cpUptr_{cp}, logger_{ara::log::CreateLogger("ENPB", "Authn Cipher Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "EncryptorPublicCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "server client connection for EncryptorPublicCtx -> success";
  }
  else
  {
    logger_.LogError() << "server client connection for EncryptorPublicCtx -> fail";
  }
}

ara::core::Result<void> EncryptorPublicCtxProxyImpl::SetKey(const PublicKey& key) noexcept
{
  logger_.LogDebug() << "IPC-APP::ENPB EncryptorPublicCtxProxyImpl Method -> SetKey(const PublicKey& key)";
  const ara::crypto::cryp::cryobj::PublicKeyProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::cryobj::PublicKeyProxyImpl*>(&key)};
  logger_.LogDebug() << "IPC-APP::ENPB CHECK1 address of key -> " << &key;
  CryptoServiceInstanceId const serviceInstanceId{obj->GetPublicKeyServiceInstanceId()};
  logger_.LogDebug() << "IPC-APP::ENPB SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = encPubProxyHandler_->SetKey(serviceInstanceId).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ENPB SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::ENPB SetKey() Error: " << keyResult.Error().Message();
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

ara::core::Result<ara::core::Vector<ara::core::Byte>> EncryptorPublicCtxProxyImpl::ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding) const noexcept
{
  logger_.LogDebug() << "IPC-APP::ENPB ProcessBlocks() with ReadOnlyMemRegion as argument";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto processResult = encPubProxyHandler_->ProcessBlockReadOnly(vtrUint8t, suppressPadding).GetResult();
  if (processResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ENPB ProcessBlocks() -> success";
    auto tempVtr = processResult.Value().vectorBytes;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::ENPB ProcessBlocks() -> Error: " << processResult.Error().Message();
    if (processResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
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

ara::core::Result<void> EncryptorPublicCtxProxyImpl::Reset() noexcept
{
  auto resReset = encPubProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ENPB Reset() -> success";
  }
  return ara::core::Result<void>();
}

CryptoPrimitiveId::Uptr EncryptorPublicCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ENPB -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = encPubProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::ENPB -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

bool EncryptorPublicCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = encPubProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ENPB -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::ENPB -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& EncryptorPublicCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ENPB -> MyProvider()";
  return cpUptr_;
}

CryptoService::Uptr EncryptorPublicCtxProxyImpl::GetCryptoService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ENPB -> GetCryptoService()";
  auto cryptoService = encPubProxyHandler_->GetCryptoService().GetResult();
  if (!cryptoService.Value().cryptoServiceInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = cryptoService.Value().cryptoServiceInstanceId;
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

bool EncryptorPublicCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipEpcInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for AuthCipherContext Function: Successful";
    encPubProxyHandler_ = vsomeipEpcInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for AuthCipherContext Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}
} // namespace cryp
} // namespace crypto
} // namespace ara
