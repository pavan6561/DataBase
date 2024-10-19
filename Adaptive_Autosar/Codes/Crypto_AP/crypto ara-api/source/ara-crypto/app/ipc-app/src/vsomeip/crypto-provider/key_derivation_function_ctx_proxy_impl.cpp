// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_derivation_function_ctx_impl.cpp
//
// Purpose     : Key derivation function proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "key_derivation_function_ctx_proxy_impl.h"

#include <algorithm>
#include <iostream>
#include <unistd.h>

#include "ara/crypto/cryp/cryobj/symmetric_key.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
KeyDerivationFunctionCtxProxyImpl::KeyDerivationFunctionCtxProxyImpl(
    CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        kdfCPHandler,
    CryptoProvider& cp) noexcept :
    kdfCryptoProviderHandler_{kdfCPHandler},
    kdfServiceInstanceID_{instanceValue}, cpUptr_{cp}, logger_{ara::log::CreateLogger("KDFP",
                                                           "KDF Context Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "KeyDerivationFunctionCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "IPC-APP::KDF server client connection for KeyDerivationFunctionCtx -> success";
  }
  else
  {
    logger_.LogError() << "IPC-APP::KDF server client connection for KeyDerivationFunctionCtx -> fail";
  }
}

bool KeyDerivationFunctionCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = kdfProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::KDF -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::KDF -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& KeyDerivationFunctionCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF -> MyProvider()";
  return cpUptr_;
}

ExtensionService::Uptr KeyDerivationFunctionCtxProxyImpl::GetExtensionService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF -> GetExtensionService()";
  auto extensionService = kdfProxyHandler_->GetExtensionService().GetResult();
  if (!extensionService.Value().instanceId.empty())
  {
    CryptoServiceInstanceId instanceId = extensionService.Value().instanceId;
    logger_.LogDebug() << "IPC-KDF Service InstanceId for GetExtensionService : " << instanceId;
    ExtensionService::Uptr extensionServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::ExtensionServiceProxyImpl>(instanceId);
    return extensionServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-KDF Service InstanceId for GetExtensionService is null";
    return nullptr;
  }
}

CryptoPrimitiveId::Uptr KeyDerivationFunctionCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = kdfProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::KDF -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

ara::core::Result<void> KeyDerivationFunctionCtxProxyImpl::AddSalt(ReadOnlyMemRegion salt) noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF AddSaltReadOnlyMem()";
  CryptoReadOnlyMemRegion vtrUint8t(salt.begin(), salt.end());
  auto resultAddSalt = kdfProxyHandler_->AddSaltReadOnlyMem(vtrUint8t).GetResult();
  if (resultAddSalt.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::KDF AddSaltReadOnlyMem() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::KDF AddSaltReadOnlyMem() Error: " << resultAddSalt.Error().Message();
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
  }
}

ara::core::Result<void> KeyDerivationFunctionCtxProxyImpl::AddSecretSalt(const SecretSeed& salt) noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF AddSecretSalt()";
  const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&salt);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  logger_.LogDebug() << "IPC-APP::KDF AddSecretSalt() CryptoServiceInstanceId : " << serviceInstanceId;
  auto saltResult = kdfProxyHandler_->AddSecretSalt(serviceInstanceId).GetResult();
  if (saltResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::KDF AddSecretSalt() -> Success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::KDF AddSecretSalt() -> Error: " << saltResult.Error().Message();
    return ara::core::Result<void>::FromValue();
  }
}

std::uint32_t KeyDerivationFunctionCtxProxyImpl::ConfigIterations(std::uint32_t iterations) noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF ConfigIterations()";
  auto resultIterations = kdfProxyHandler_->ConfigIterations(iterations).GetResult();
  return resultIterations.Value().result;
}

std::size_t KeyDerivationFunctionCtxProxyImpl::GetKeyIdSize() const noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF GetKeyIdSize()";
  auto keyIdResult = kdfProxyHandler_->GetKeyIdSize().GetResult();
  return keyIdResult.Value().result;
}

CryptoAlgId KeyDerivationFunctionCtxProxyImpl::GetTargetAlgId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF GetTargetAlgId()";
  auto targetAlgId = kdfProxyHandler_->GetTargetAlgId().GetResult();
  return targetAlgId.Value().algId;
}

AllowedUsageFlags KeyDerivationFunctionCtxProxyImpl::GetTargetAllowedUsage() const noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF GetTargetAllowedUsage()";
  auto targetAllowUsageFlag = kdfProxyHandler_->GetTargetAllowedUsage().GetResult();
  return targetAllowUsageFlag.Value().allowedUsageFlags;
}

std::size_t KeyDerivationFunctionCtxProxyImpl::GetTargetKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF GetTargetKeyBitLength()";
  auto targetKeyBitLength = kdfProxyHandler_->GetTargetKeyBitLength().GetResult();
  return targetKeyBitLength.Value().result;
}

ara::core::Result<void> KeyDerivationFunctionCtxProxyImpl::Init(ReadOnlyMemRegion targetKeyId,
    CryptoAlgId targetAlgId, AllowedUsageFlags allowedUsage, ReadOnlyMemRegion ctxLabel) noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF Init()";
  CryptoReadOnlyMemRegion localtargetKeyId(targetKeyId.begin(), targetKeyId.end());
  CryptoReadOnlyMemRegion localctxLabel(ctxLabel.begin(), ctxLabel.end());
  auto resultInit
      = kdfProxyHandler_->Init(localtargetKeyId, targetAlgId, allowedUsage, localctxLabel).GetResult();
  if (resultInit.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::KDF Init() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::KDF Init() Error: " << resultInit.Error().Message();
    if (resultInit.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
    }
  }
}

ara::core::Result<void> KeyDerivationFunctionCtxProxyImpl::Reset() noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF Reset()";
  auto resReset = kdfProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::KDF Reset() -> success";
  }
  return ara::core::Result<void>();
}

ara::core::Result<SymmetricKey::Uptrc> KeyDerivationFunctionCtxProxyImpl::DeriveKey(
    bool isSession, bool isExportable) const noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF DeriveKey()";
  auto deriveKeyResult = kdfProxyHandler_->DeriveKey(isSession, isExportable).GetResult();
  if (deriveKeyResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = deriveKeyResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP::KDF Service InstanceId : " << instanceId;
    SymmetricKey::Uptrc symmetricKeyUptrc
        = std::make_unique<ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl>(instanceId);
    return ara::core::Result<SymmetricKey::Uptrc>(std::move(symmetricKeyUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP::KDF DeriveKey() Error: " << deriveKeyResult.Error().Message();
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kUninitializedContext);
  }
}

ara::core::Result<SecretSeed::Uptrc> KeyDerivationFunctionCtxProxyImpl::DeriveSeed(
    bool isSession, bool isExportable) const noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF DeriveSeed()";
  auto secretSeedResult = kdfProxyHandler_->DeriveSeed(isSession, isExportable).GetResult();
  if (secretSeedResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = secretSeedResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for DeriveSeed : " << instanceId;
    SecretSeed::Uptrc secretSeedUptrc
        = std::make_unique<ara::crypto::cryp::cryobj::SecretSeedProxyImpl>(instanceId);
    return ara::core::Result<SecretSeed::Uptrc>(std::move(secretSeedUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP DeriveSeed() Error: " << secretSeedResult.Error().Message();
    return ara::core::Result<SecretSeed::Uptrc>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
  }
}

ara::core::Result<void> KeyDerivationFunctionCtxProxyImpl::SetSourceKeyMaterial(
    const RestrictedUseObject& sourceKM) noexcept
{
  logger_.LogDebug() << "IPC-APP::KDF SetSourceKeyMaterial()";
  CryptoServiceInstanceId serviceInstanceId;
  CryptoObjectType cryptoObjType = sourceKM.cryObjType;
  logger_.LogDebug() << "IPC-APP::KDF  cryptoObjectType =" << static_cast<uint32_t>(cryptoObjType);
  if (cryptoObjType == CryptoObjectType::kSecretSeed)
  {
    const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&sourceKM);
    serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kSymmetricKey)
  {
    const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&sourceKM);
    serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kPublicKey)
  {
    // ToDo: Public key and Private key are not implemented for now as
    // underlying crypto daemon doesn't support.
  }
  else if (cryptoObjType == CryptoObjectType::kPrivateKey)
  {
    // ToDo: Public key and Private key are not implemented for now as
    // underlying crypto daemon doesn't support.
  }

  RestrictedUseObjectStructure restrictedUseObj;
  restrictedUseObj.RestrictedUseInstanceId = serviceInstanceId;
  restrictedUseObj.RestrictedUseObjectType = static_cast<CryptoObjectTypeIPC>(cryptoObjType);
  logger_.LogDebug() << "IPC-APP::KDF CryptoServiceInstanceId : " << serviceInstanceId;
  auto updateResult = kdfProxyHandler_->SetSourceKeyMaterial(restrictedUseObj).GetResult();
  if (updateResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::KDF SetSourceKeyMaterial() -> Success";
  }
  else
  {
    logger_.LogError() << "IPC-APP::KDF SetSourceKeyMaterial() -> Error: " << updateResult.Error().Message();
    if (updateResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kIncompatibleObject);
    }
    else if (updateResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kBruteForceRisk);
    }
  }
  return ara::core::Result<void>::FromValue();
}

bool KeyDerivationFunctionCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipKdfInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "IPC-APP::KDF Server Client connection for Hash Function : Successful";
    kdfProxyHandler_ = vsomeipKdfInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "IPC-APP::KDF Server Client connection for Hash Function : Failed";
    isSuccess = false;
  }
  return isSuccess;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
