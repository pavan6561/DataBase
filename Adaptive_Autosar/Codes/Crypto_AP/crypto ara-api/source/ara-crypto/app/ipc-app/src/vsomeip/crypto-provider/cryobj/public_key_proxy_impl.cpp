// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : public_key_proxy_impl.cpp
//
// Purpose     : Public key proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "public_key_proxy_impl.h"

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
PublicKeyProxyImpl::PublicKeyProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    publicKeyServiceInstanceID_{serviceInstanceID}, logger_{ara::log::CreateLogger("PUPI",
                                                        "PublicKey Proxy Impl", ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void PublicKeyProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for PublicKey Proxy Impl : Successful";
    publicKeyHandler_ = vsomeipInstance_.GetHandlerInstance();
    cryObjType = CryptoObjectType::kPublicKey;
  }
  else
  {
    logger_.LogError() << "Server Client connection for PublicKey Proxy Impl : Failed";
  }
}

bool PublicKeyProxyImpl::CheckKey(bool strongCheck) const noexcept
{
  bool isResult{false};
  auto objResult = publicKeyHandler_->CheckKey(strongCheck).GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PUPI -> " << __FUNCTION__ << " : Success";
    isResult = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PUPI -> " << __FUNCTION__ << " : Failed";
  }
  return isResult;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> PublicKeyProxyImpl::HashPublicKey(
    HashFunctionCtx& hashFunc) const noexcept
{
  logger_.LogDebug() << "IPC-APP::PUPI HashPublicKey()";
  const ara::crypto::cryp::HashFunctionCtxProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::HashFunctionCtxProxyImpl*>(&hashFunc)};
  CryptoServiceInstanceId const serviceInstanceId{obj->GetHashFunctionCtxServiceInstanceId()};
  logger_.LogDebug() << "IPC-APP::PUPI HashPublicKey() HashFunctionCtx CryptoServiceInstanceId : "
                     << serviceInstanceId;
  auto tempResult = publicKeyHandler_->HashPublicKey(serviceInstanceId).GetResult();
  if (tempResult.HasValue())
  {
    ara::core::Vector<ara::core::Byte> vtrbyte(
        tempResult.Value().VectorByte.begin(), tempResult.Value().VectorByte.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::PUPI HashPublicKey() -> Error: " << tempResult.Error().Message();
    if (tempResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInsufficientCapacity);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kIncompleteArgState);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> PublicKeyProxyImpl::ExportPublicly(
    FormatId formatId) const noexcept
{
  // ToDo: This interface is required for serialization the public key, it will be supported in future release
  ara::core::Vector<ara::core::Byte> data;
  static_cast<void>(formatId);
  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(data);
}

ara::crypto::cryp::RestrictedUseObject::Usage PublicKeyProxyImpl::GetAllowedUsage() const noexcept
{
  ara::crypto::cryp::RestrictedUseObject::Usage symmtericKeyUsages{0};
  auto objResult = publicKeyHandler_->GetAllowedUsage().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    symmtericKeyUsages = objResult.Value().UsageFlag;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return symmtericKeyUsages;
}

bool PublicKeyProxyImpl::IsSession() const noexcept
{
  bool isSession{false};
  auto objResult = publicKeyHandler_->IsSession().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    isSession = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isSession;
}

bool PublicKeyProxyImpl::IsExportable() const noexcept
{
  bool isExportable{false};
  auto objResult = publicKeyHandler_->IsExportable().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    isExportable = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isExportable;
}

ara::crypto::cryp::CryptoObject::COIdentifier PublicKeyProxyImpl::GetObjectId() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = publicKeyHandler_->GetObjectId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogError() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

ara::crypto::cryp::CryptoObject::COIdentifier PublicKeyProxyImpl::HasDependence() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = publicKeyHandler_->HasDependence().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogError() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

std::size_t PublicKeyProxyImpl::GetPayloadSize() const noexcept
{
  std::size_t size{0};
  auto objResult = publicKeyHandler_->GetPayloadSize().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().PayloadSize;
  }
  else
  {
    logger_.LogError() << "IPC-APP::PublicKeyProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

ara::crypto::cryp::CryptoPrimitiveId::Uptr PublicKeyProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  ara::crypto::cryp::CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptr{nullptr};
  auto objResult = publicKeyHandler_->GetCryptoPrimitiveId().GetResult();
  if (objResult.HasValue())
  {
    const ara::core::StringView primitiveName{objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName};
    const CryptoPrimitiveId::AlgId algorithmId{objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId};
    cryptoPrimitiveIdUptr
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::PublicKeyProxyImpl -> GetCryptoPrimitiveId() Error: "
                       << objResult.Error().Message();
  }
  return cryptoPrimitiveIdUptr;
}

ara::core::Result<void> PublicKeyProxyImpl::Save(IOInterface& container) const noexcept
{
  static_cast<void>(container);
  return ara::core::Result<void>::FromValue();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara
