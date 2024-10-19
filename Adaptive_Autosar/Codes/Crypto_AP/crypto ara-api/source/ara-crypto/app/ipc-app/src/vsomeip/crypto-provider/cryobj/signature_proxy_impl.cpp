// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature_proxy_impl.cpp
//
// Purpose     : Signature proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "signature_proxy_impl.h"

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
SignatureProxyImpl::SignatureProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    signatureServiceInstanceID_{serviceInstanceID}, logger_{ara::log::CreateLogger("SGPI",
                                                        "Signature Proxy Impl", ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void SignatureProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug()
        << "IPC-APP::SignatureProxyImpl -> Server Client connection for Signature Proxy Impl : Successful";
    signatureHandler_ = vsomeipInstance_.GetHandlerInstance();
    cryObjType = CryptoObjectType::kSignature;
  }
  else
  {
    logger_.LogDebug()
        << "IPC-APP::SignatureProxyImpl -> Server Client connection for Signature Proxy Impl : Failed";
  }
}

ara::crypto::cryp::CryptoPrimitiveId::AlgId SignatureProxyImpl::GetHashAlgId() const noexcept
{
  ara::crypto::cryp::CryptoPrimitiveId::AlgId localAlgId{0};
  auto objResult = signatureHandler_->GetHashAlgId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Success";
    localAlgId = objResult.Value().CryptoPrimitiveIdAlgId;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localAlgId;
}

std::size_t SignatureProxyImpl::GetRequiredHashSize() const noexcept
{
  std::size_t hashSize{0};
  auto objResult = signatureHandler_->GetRequiredHashSize().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Success";
    hashSize = objResult.Value().HashSize;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return hashSize;
}

bool SignatureProxyImpl::IsSession() const noexcept
{
  bool isSession{false};
  auto objResult = signatureHandler_->IsSession().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Success";
    isSession = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isSession;
}

bool SignatureProxyImpl::IsExportable() const noexcept
{
  bool isExportable{false};
  auto objResult = signatureHandler_->IsExportable().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Success";
    isExportable = objResult.Value().Bool;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return isExportable;
}

ara::crypto::cryp::CryptoObject::COIdentifier SignatureProxyImpl::GetObjectId() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = signatureHandler_->GetObjectId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogError() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

ara::crypto::cryp::CryptoObject::COIdentifier SignatureProxyImpl::HasDependence() const noexcept
{
  ara::crypto::cryp::CryptoObject::COIdentifier localCoIdentifier;
  auto objResult = signatureHandler_->HasDependence().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Success";
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
    logger_.LogError() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return localCoIdentifier;
}

std::size_t SignatureProxyImpl::GetPayloadSize() const noexcept
{
  std::size_t size{0};
  auto objResult = signatureHandler_->GetPayloadSize().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Success";
    size = objResult.Value().PayloadSize;
  }
  else
  {
    logger_.LogError() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Failed";
  }
  return size;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SignatureProxyImpl::ExportPublicly(
    FormatId formatId) const noexcept
{
  auto objResult = signatureHandler_->ExportPublicly(formatId).GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Success";
    auto byteResult = objResult.Value().ByteVector;
    ara::core::Vector<ara::core::Byte> localByteVector(byteResult.begin(), byteResult.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(localByteVector);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << " : Failed";
    if (objResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUnknownIdentifier);
    }
    else if (objResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUnsupportedFormat);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUnsupportedFormat);
    }
  }
}

ara::crypto::cryp::CryptoPrimitiveId::Uptr SignatureProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  ara::crypto::cryp::CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptr{nullptr};
  auto objResult = signatureHandler_->GetCryptoPrimitiveId().GetResult();
  if (objResult.HasValue())
  {
    const ara::core::StringView primitiveName = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName;
    const CryptoPrimitiveId::AlgId algorithmId = objResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId;
    cryptoPrimitiveIdUptr
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::SignatureProxyImpl -> GetCryptoPrimitiveId() Error: "
                       << objResult.Error().Message();
  }
  return cryptoPrimitiveIdUptr;
}

ara::core::Result<void> SignatureProxyImpl::Save(IOInterface& container) const noexcept
{
  logger_.LogError() << "IPC-APP::SignatureProxyImpl -> " << __FUNCTION__ << "Not Supported for this release";
  static_cast<void>(container);
  return ara::core::Result<void>::FromValue();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara
