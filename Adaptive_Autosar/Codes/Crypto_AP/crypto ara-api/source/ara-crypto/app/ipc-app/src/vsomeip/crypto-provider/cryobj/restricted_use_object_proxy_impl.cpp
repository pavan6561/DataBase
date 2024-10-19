// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : restricted_use_object_proxy_impl.cpp
//
// Purpose     : Restricted use object proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "restricted_use_object_proxy_impl.h"

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
RestrictedUseObjectProxyImpl::RestrictedUseObjectProxyImpl(
    const CryptoServiceInstanceId serviceInstanceID) noexcept :
    restrictedUseObjServiceInstanceID_{serviceInstanceID},
    logger_{ara::log::CreateLogger("RUPI", "Restricted UseObject Proxy Impl", ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void RestrictedUseObjectProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug()
        << "IPC-APP::RestrictedUseObjectProxyImpl -> Server Client connection for Restricted Use Object Proxy Impl : Successful";
    restricteduseObjectHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError()
        << "IPC-APP::RestrictedUseObjectProxyImpl -> Server Client connection for Restricted Use Object Proxy Impl : Failed";
  }
}

ara::crypto::cryp::RestrictedUseObject::Usage RestrictedUseObjectProxyImpl::GetAllowedUsage() const noexcept
{
  ara::crypto::cryp::RestrictedUseObject::Usage keyUsages{0};
  auto objResult = restricteduseObjectHandler_->GetAllowedUsage().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Success";
    keyUsages = objResult.Value().Usages;
  }
  else
  {
    logger_.LogError() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return keyUsages;
}

bool RestrictedUseObjectProxyImpl::IsSession() const noexcept
{
  bool isSession{false};
  auto objResult = restricteduseObjectHandler_->IsSession().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Success";
    isSession = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isSession;
}

bool RestrictedUseObjectProxyImpl::IsExportable() const noexcept
{
  bool isExportable{false};
  auto objResult = restricteduseObjectHandler_->IsExportable().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Success";
    isExportable = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isExportable;
}

ara::crypto::cryp::CryptoObject::COIdentifier RestrictedUseObjectProxyImpl::GetObjectId() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = restricteduseObjectHandler_->GetObjectId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogError() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

ara::crypto::cryp::CryptoObject::COIdentifier RestrictedUseObjectProxyImpl::HasDependence() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = restricteduseObjectHandler_->HasDependence().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogError() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

std::size_t RestrictedUseObjectProxyImpl::GetPayloadSize() const noexcept
{
  std::size_t size{0};
  auto objResult = restricteduseObjectHandler_->GetPayloadSize().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().size;
  }
  else
  {
    logger_.LogError() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

ara::crypto::cryp::CryptoPrimitiveId::Uptr RestrictedUseObjectProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  ara::crypto::cryp::CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptr{nullptr};
  auto objResult = restricteduseObjectHandler_->GetCryptoPrimitiveId().GetResult();
  if (objResult.HasValue())
  {
    const ara::core::StringView primitiveName = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName;
    const CryptoPrimitiveId::AlgId algorithmId = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId;
    cryptoPrimitiveIdUptr
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::RestrictedUseObjectProxyImpl -> GetCryptoPrimitiveId() Error: "
                       << objResult.Error().Message();
  }
  return cryptoPrimitiveIdUptr;
}

ara::core::Result<void> RestrictedUseObjectProxyImpl::Save(IOInterface& container) const noexcept
{
  logger_.LogError() << "IPC-APP::RestrictedUseObjectProxyImpl -> " << __FUNCTION__
                     << "Not Supported for this release";
  static_cast<void>(container);
  return ara::core::Result<void>::FromValue();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara
