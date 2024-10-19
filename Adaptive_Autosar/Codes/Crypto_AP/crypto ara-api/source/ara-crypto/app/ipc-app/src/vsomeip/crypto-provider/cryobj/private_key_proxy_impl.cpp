// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : private_key_proxy_impl.cpp
//
// Purpose     : Private key proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "private_key_proxy_impl.h"

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
PrivateKeyProxyImpl::PrivateKeyProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    privateKeyServiceInstanceID_{serviceInstanceID}, logger_{ara::log::CreateLogger("PPPI",
                                                         "PrivateKey Proxy Impl",
                                                         ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

ara::core::Result<PublicKey::Uptrc> PrivateKeyProxyImpl::GetPublicKey() const noexcept
{
  logger_.LogDebug() << "IPC-APP::PPPI PrivateKeyProxyImpl::" << __FUNCTION__;

  PublicKey::Uptrc publicKeyUptrc{nullptr};
  auto publicKeyResult = privateKeyHandler_->GetPublicKey().GetResult();
  if (publicKeyResult.HasValue())
  {
    CryptoServiceInstanceId instanceId{publicKeyResult.Value().PublicKeyUptrInstanceId};
    logger_.LogDebug() << "IPC-APP::PPPI Service InstanceId for GetPublicKey : " << instanceId;
    publicKeyUptrc = std::make_unique<ara::crypto::cryp::cryobj::PublicKeyProxyImpl>(instanceId);
  }
  return ara::core::Result<PublicKey::Uptrc>(std::move(publicKeyUptrc));
}

void PrivateKeyProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for PrivateKey Proxy Impl : Successful";
    privateKeyHandler_ = vsomeipInstance_.GetHandlerInstance();
    cryObjType = CryptoObjectType::kPrivateKey;
  }
  else
  {
    logger_.LogError() << "Server Client connection for PrivateKey Proxy Impl : Failed";
  }
}

ara::crypto::cryp::RestrictedUseObject::Usage PrivateKeyProxyImpl::GetAllowedUsage() const noexcept
{
  ara::crypto::cryp::RestrictedUseObject::Usage symmtericKeyUsages{0};
  auto objResult = privateKeyHandler_->GetAllowedUsage().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    symmtericKeyUsages = objResult.Value().UsageFlag;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return symmtericKeyUsages;
}

bool PrivateKeyProxyImpl::IsSession() const noexcept
{
  bool isSession{false};
  auto objResult = privateKeyHandler_->IsSession().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    isSession = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isSession;
}

bool PrivateKeyProxyImpl::IsExportable() const noexcept
{
  bool isExportable{false};
  auto objResult = privateKeyHandler_->IsExportable().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    isExportable = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isExportable;
}

ara::crypto::cryp::CryptoObject::COIdentifier PrivateKeyProxyImpl::GetObjectId() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = privateKeyHandler_->GetObjectId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    CryptoObjectTypeIPC objType{objResult.Value().COIdentifier.mCOType};
    localCoIdentifier.mCOType = static_cast<ara::crypto::CryptoObjectType>(objType);
    localCoIdentifier.mCouid.mGeneratorUid.mQwordLs
        = objResult.Value().COIdentifier.mCouid.mGeneratorUid.mQwordLs;
    localCoIdentifier.mCouid.mGeneratorUid.mQwordMs
        = objResult.Value().COIdentifier.mCouid.mGeneratorUid.mQwordMs;
    localCoIdentifier.mCouid.mVersionStamp = objResult.Value().COIdentifier.mCouid.mVersionStamp;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

ara::crypto::cryp::CryptoObject::COIdentifier PrivateKeyProxyImpl::HasDependence() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = privateKeyHandler_->HasDependence().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    CryptoObjectTypeIPC objType{objResult.Value().COIdentifier.mCOType};
    localCoIdentifier.mCOType = static_cast<ara::crypto::CryptoObjectType>(objType);
    localCoIdentifier.mCouid.mGeneratorUid.mQwordLs
        = objResult.Value().COIdentifier.mCouid.mGeneratorUid.mQwordLs;
    localCoIdentifier.mCouid.mGeneratorUid.mQwordMs
        = objResult.Value().COIdentifier.mCouid.mGeneratorUid.mQwordMs;
    localCoIdentifier.mCouid.mVersionStamp = objResult.Value().COIdentifier.mCouid.mVersionStamp;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

std::size_t PrivateKeyProxyImpl::GetPayloadSize() const noexcept
{
  std::size_t size{0};
  auto objResult = privateKeyHandler_->GetPayloadSize().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().PayloadSize;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PrivateKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

ara::crypto::cryp::CryptoPrimitiveId::Uptr PrivateKeyProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  ara::crypto::cryp::CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptr{nullptr};
  auto objResult = privateKeyHandler_->GetCryptoPrimitiveId().GetResult();
  if (objResult.HasValue())
  {
    const ara::core::StringView primitiveName{objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName};
    const CryptoPrimitiveId::AlgId algorithmId{objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId};
    cryptoPrimitiveIdUptr
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::PrivateKeyProxyImpl -> GetCryptoPrimitiveId() Error: "
                       << objResult.Error().Message();
  }
  return cryptoPrimitiveIdUptr;
}

ara::core::Result<void> PrivateKeyProxyImpl::Save(IOInterface& container) const noexcept
{
  static_cast<void>(container);
  return ara::core::Result<void>::FromValue();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara
