// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_object_proxy_impl.cpp
//
// Purpose     : Crypto object proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "crypto_object_proxy_impl.h"

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
CryptoObjectProxyImpl::CryptoObjectProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    cryptoObjServiceInstanceID_{serviceInstanceID}, logger_{ara::log::CreateLogger("COPI",
                                                        "Crypto Object Proxy Impl",
                                                        ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void CryptoObjectProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for Restricted Use Object Proxy Impl : Successful";
    cryptoObjectHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for Restricted Use Object Proxy Impl : Failed";
  }
}

bool CryptoObjectProxyImpl::IsSession() const noexcept
{
  bool isSession{false};
  auto objResult = cryptoObjectHandler_->IsSession().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Success";
    isSession = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isSession;
}

bool CryptoObjectProxyImpl::IsExportable() const noexcept
{
  bool isExportable{false};
  auto objResult = cryptoObjectHandler_->IsExportable().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Success";
    isExportable = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isExportable;
}

ara::crypto::cryp::CryptoObject::COIdentifier CryptoObjectProxyImpl::GetObjectId() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = cryptoObjectHandler_->GetObjectId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogError() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

ara::crypto::cryp::CryptoObject::COIdentifier CryptoObjectProxyImpl::HasDependence() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = cryptoObjectHandler_->HasDependence().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogError() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

std::size_t CryptoObjectProxyImpl::GetPayloadSize() const noexcept
{
  std::size_t size{0};
  auto objResult = cryptoObjectHandler_->GetPayloadSize().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().size;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

ara::crypto::cryp::CryptoPrimitiveId::Uptr CryptoObjectProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  ara::crypto::cryp::CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptr{nullptr};
  auto objResult = cryptoObjectHandler_->GetCryptoPrimitiveId().GetResult();
  if (objResult.HasValue())
  {
    const ara::core::StringView primitiveName = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName;
    const CryptoPrimitiveId::AlgId algorithmId = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId;
    cryptoPrimitiveIdUptr
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoObjectProxyImpl -> GetCryptoPrimitiveId() Error: "
                       << objResult.Error().Message();
  }
  return cryptoPrimitiveIdUptr;
}

ara::core::Result<void> CryptoObjectProxyImpl::Save(IOInterface& container) const noexcept
{
  logger_.LogError() << "IPC-APP::CryptoObjectProxyImpl -> " << __FUNCTION__
                     << "Not Supported for this release";
  static_cast<void>(container);
  return ara::core::Result<void>::FromValue();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara
