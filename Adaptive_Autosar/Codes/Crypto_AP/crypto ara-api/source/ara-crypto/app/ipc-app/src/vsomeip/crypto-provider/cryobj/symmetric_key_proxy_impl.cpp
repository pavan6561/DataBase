// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_key_proxy_impl.cpp
//
// Purpose     : Symmetric key proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "symmetric_key_proxy_impl.h"

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
SymmetricKeyProxyImpl::SymmetricKeyProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    symmetricKeyServiceInstanceID_{serviceInstanceID}, logger_{ara::log::CreateLogger("SKPI",
                                                           "Symmetric Key Proxy Impl",
                                                           ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void SymmetricKeyProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for SymmetricKey Proxy Impl : Successful";
    symmetricKeyHandler_ = vsomeipInstance_.GetHandlerInstance();
    cryObjType = CryptoObjectType::kSymmetricKey;
  }
  else
  {
    logger_.LogDebug() << "Server Client connection for SymmetricKey Proxy Impl : Failed";
  }
}

ara::crypto::cryp::RestrictedUseObject::Usage SymmetricKeyProxyImpl::GetAllowedUsage() const noexcept
{
  ara::crypto::cryp::RestrictedUseObject::Usage symmtericKeyUsages{0};
  auto objResult = symmetricKeyHandler_->GetAllowedUsage().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    symmtericKeyUsages = objResult.Value().Usages;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return symmtericKeyUsages;
}

bool SymmetricKeyProxyImpl::IsSession() const noexcept
{
  bool isSession{false};
  auto objResult = symmetricKeyHandler_->IsSession().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    isSession = objResult.Value().Bool;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isSession;
}

bool SymmetricKeyProxyImpl::IsExportable() const noexcept
{
  bool isExportable{false};
  auto objResult = symmetricKeyHandler_->IsExportable().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    isExportable = objResult.Value().Bool;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isExportable;
}

ara::crypto::cryp::CryptoObject::COIdentifier SymmetricKeyProxyImpl::GetObjectId() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = symmetricKeyHandler_->GetObjectId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

ara::crypto::cryp::CryptoObject::COIdentifier SymmetricKeyProxyImpl::HasDependence() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = symmetricKeyHandler_->HasDependence().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

std::size_t SymmetricKeyProxyImpl::GetPayloadSize() const noexcept
{
  std::size_t size{0};
  auto objResult = symmetricKeyHandler_->GetPayloadSize().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().size;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

ara::crypto::cryp::CryptoPrimitiveId::Uptr SymmetricKeyProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  ara::crypto::cryp::CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptr{nullptr};
  auto objResult = symmetricKeyHandler_->GetCryptoPrimitiveId().GetResult();
  if (objResult.HasValue())
  {
    const ara::core::StringView primitiveName = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName;
    const CryptoPrimitiveId::AlgId algorithmId = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId;
    cryptoPrimitiveIdUptr
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SymmetricKeyProxyImpl -> GetCryptoPrimitiveId() Error: "
                       << objResult.Error().Message();
  }
  return cryptoPrimitiveIdUptr;
}

ara::core::Result<void> SymmetricKeyProxyImpl::Save(IOInterface& container) const noexcept
{
  logger_.LogError() << "IPC-APP::SymmetricKeyProxyImpl -> " << __FUNCTION__
                     << "Not Supported for this release";
  static_cast<void>(container);
  return ara::core::Result<void>::FromValue();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara
