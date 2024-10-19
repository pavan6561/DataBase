// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : secret_seed_proxy_impl.cpp
//
// Purpose     : Secret seed proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "secret_seed_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for crypto object.
namespace cryobj
{
SecretSeedProxyImpl::SecretSeedProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    secretSeedServiceInstanceID_{serviceInstanceID}, logger_{ara::log::CreateLogger("SSPI",
                                                         "SecretSeed Proxy Impl",
                                                         ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void SecretSeedProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug()
        << "IPC-APP::SecretSeedProxyImpl -> Server Client connection for SecretSeed Proxy Impl : Successful";
    secretSeedKeyHandler_ = vsomeipInstance_.GetHandlerInstance();
    cryObjType = CryptoObjectType::kSecretSeed;
  }
  else
  {
    logger_.LogError()
        << "IPC-APP::SecretSeedProxyImpl -> Server Client connection for SecretSeed Proxy Impl : Failed";
  }
}

ara::core::Result<SecretSeed::Uptr> SecretSeedProxyImpl::Clone(ReadOnlyMemRegion xorDelta) const noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__;
  CryptoReadOnlyMemRegion vtrUint8t(xorDelta.begin(), xorDelta.end());
  SecretSeed::Uptr secretSeedUptr{nullptr};
  auto seedResult = secretSeedKeyHandler_->Clone(vtrUint8t).GetResult();
  if (seedResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = seedResult.Value().SecretSeedUptr;
    logger_.LogDebug() << "IPC-APP Service InstanceId for Clone() : " << instanceId;
    secretSeedUptr = std::make_unique<ara::crypto::cryp::cryobj::SecretSeedProxyImpl>(instanceId);
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> Clone():: undefined error";
  }
  return ara::core::Result<SecretSeed::Uptr>::FromValue(std::move(secretSeedUptr));
}

ara::crypto::cryp::RestrictedUseObject::Usage SecretSeedProxyImpl::GetAllowedUsage() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__;
  ara::crypto::cryp::RestrictedUseObject::Usage symmtericKeyUsages{0};
  auto objResult = secretSeedKeyHandler_->GetAllowedUsage().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Success";
    symmtericKeyUsages = objResult.Value().Usages;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return symmtericKeyUsages;
}

bool SecretSeedProxyImpl::IsSession() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__;
  bool isSession{false};
  auto objResult = secretSeedKeyHandler_->IsSession().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Success";
    isSession = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isSession;
}

bool SecretSeedProxyImpl::IsExportable() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__;
  bool isExportable{false};
  auto objResult = secretSeedKeyHandler_->IsExportable().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Success";
    isExportable = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isExportable;
}

ara::crypto::cryp::CryptoObject::COIdentifier SecretSeedProxyImpl::GetObjectId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__;
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = secretSeedKeyHandler_->GetObjectId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Success";
    CryptoObjectTypeIPC objType = objResult.Value().COIdentifier.mCOType;
    localCoIdentifier.mCOType = static_cast<ara::crypto::CryptoObjectType>(objType);
    localCoIdentifier.mCouid.mGeneratorUid.mQwordLs
        = objResult.Value().COIdentifier.mCouid.mGeneratorUid.mQwordLs;
    localCoIdentifier.mCouid.mGeneratorUid.mQwordMs
        = objResult.Value().COIdentifier.mCouid.mGeneratorUid.mQwordMs;
    localCoIdentifier.mCouid.mVersionStamp = objResult.Value().COIdentifier.mCouid.mVersionStamp;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

ara::crypto::cryp::CryptoObject::COIdentifier SecretSeedProxyImpl::HasDependence() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__;
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = secretSeedKeyHandler_->HasDependence().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Success";
    CryptoObjectTypeIPC objType = objResult.Value().COIdentifier.mCOType;
    localCoIdentifier.mCOType = static_cast<ara::crypto::CryptoObjectType>(objType);
    localCoIdentifier.mCouid.mGeneratorUid.mQwordLs
        = objResult.Value().COIdentifier.mCouid.mGeneratorUid.mQwordLs;
    localCoIdentifier.mCouid.mGeneratorUid.mQwordMs
        = objResult.Value().COIdentifier.mCouid.mGeneratorUid.mQwordMs;
    localCoIdentifier.mCouid.mVersionStamp = objResult.Value().COIdentifier.mCouid.mVersionStamp;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

std::size_t SecretSeedProxyImpl::GetPayloadSize() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> GetPayloadSize()";
  std::size_t size{0};
  auto objResult = secretSeedKeyHandler_->GetPayloadSize().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().Size;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

ara::crypto::cryp::CryptoPrimitiveId::Uptr SecretSeedProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> GetCryptoPrimitiveId()";
  ara::crypto::cryp::CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptr{nullptr};
  auto objResult = secretSeedKeyHandler_->GetCryptoPrimitiveId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl, GetCryptoPrimitiveId() -> success";
    const ara::core::StringView primitiveName = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName;
    const CryptoPrimitiveId::AlgId algorithmId = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId;
    cryptoPrimitiveIdUptr
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SecretSeedProxyImpl -> GetCryptoPrimitiveId() Error: "
                       << objResult.Error().Message();
  }
  return cryptoPrimitiveIdUptr;
}

ara::core::Result<void> SecretSeedProxyImpl::JumpFrom(const SecretSeed& from, std::int64_t steps) noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> JumpFrom()";
  const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&from);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl, CryptoServiceInstanceId : " << serviceInstanceId;
  auto jmpResult = secretSeedKeyHandler_->JumpFrom(serviceInstanceId, steps).GetResult();
  if (jmpResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl, JumpFrom() -> "
                       << " : Success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::SecretSeedProxyImpl -> JumpFrom() Error: " << jmpResult.Error().Message();
    return ara::core::Result<void>::FromError(
        static_cast<SecurityErrorDomain::Errc>(jmpResult.Error().Value()));
  }
}

SecretSeed& SecretSeedProxyImpl::Next() noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> Next()";
  auto seedResult = secretSeedKeyHandler_->Next().GetResult();
  static_cast<void>(seedResult);
  return *this;
}

SecretSeed& SecretSeedProxyImpl::Jump(std::int64_t steps) noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> Jump()";
  auto seedResult = secretSeedKeyHandler_->Jump(steps).GetResult();
  static_cast<void>(seedResult);
  return *this;
}

SecretSeed& SecretSeedProxyImpl::operator^=(const SecretSeed& source) noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> operator ^=(SecretSeed&)";
  const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&source);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  logger_.LogDebug() << "IPC-APP::operator ^=() CryptoServiceInstanceId : " << serviceInstanceId;
  auto seedResult = secretSeedKeyHandler_->XorEqualSecretSeed(serviceInstanceId).GetResult();
  static_cast<void>(seedResult);
  return *this;
}

SecretSeed& SecretSeedProxyImpl::operator^=(ReadOnlyMemRegion source) noexcept
{
  logger_.LogDebug() << "IPC-APP::SecretSeedProxyImpl -> operator ^=(ReadOnlyMemRegion)";
  CryptoReadOnlyMemRegion vtrUint8t(source.begin(), source.end());
  auto seedResult = secretSeedKeyHandler_->XorEqualReadOnlyMemRegion(vtrUint8t).GetResult();
  static_cast<void>(seedResult);
  return *this;
}

ara::core::Result<void> SecretSeedProxyImpl::Save(IOInterface& container) const noexcept
{
  logger_.LogError() << "IPC-APP::SecretSeedProxyImpl -> " << __FUNCTION__
                     << "Not Supported for this release";
  static_cast<void>(container);
  return ara::core::Result<void>::FromValue();
}
} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara
