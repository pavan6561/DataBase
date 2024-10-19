// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : authcipher_context_proxy_impl.cpp
//
// Purpose     : Authentic cipher proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "authcipher_context_proxy_impl.h"

#include <algorithm>
#include <iostream>
#include <unistd.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
AuthCipherContextProxyImpl::AuthCipherContextProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        accCPHandler,
    CryptoProvider& cp) noexcept :
    accCryptoProviderHandler_{accCPHandler},
    cpUptr_{cp}, logger_{ara::log::CreateLogger("ACCP", "Authn Cipher Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "AuthCipherContextProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "server client connection for AuthCipherContext -> success";
  }
  else
  {
    logger_.LogError() << "server client connection for AuthCipherContext -> fail";
  }
}

CryptoPrimitiveId::Uptr AuthCipherContextProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = accProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::ACC -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

bool AuthCipherContextProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = accProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::ACC -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& AuthCipherContextProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC -> MyProvider()";
  return cpUptr_;
}

BlockService::Uptr AuthCipherContextProxyImpl::GetBlockService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC -> GetBlockService()";
  auto blockService = accProxyHandler_->GetBlockService().GetResult();
  if (!blockService.Value().blockInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = blockService.Value().blockInstanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for GetBlockService : " << instanceId;
    BlockService::Uptr blockServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::BlockServiceProxyImpl>(instanceId);
    return blockServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP Service InstanceId for GetBlockService is null";
    return nullptr;
  }
}

std::uint64_t AuthCipherContextProxyImpl::GetMaxAssociatedDataSize() const noexcept
{
  auto dataSize = accProxyHandler_->GetMaxAssociatedDataSize().GetResult();
  return dataSize.Value().maxDataSize;
}

ara::core::Result<CryptoTransform> AuthCipherContextProxyImpl::GetTransformation() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC GetTransformation()";
  auto transform = accProxyHandler_->GetTransformation().GetResult();
  if (transform.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC GetTransformation(), received value = "
                       << static_cast<uint32_t>(transform.Value().CryptoTransform);
    if (transform.Value().CryptoTransform == CryptoTransformDataType::kEncrypt)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kEncrypt);
    else if (transform.Value().CryptoTransform == CryptoTransformDataType::kDecrypt)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kDecrypt);
    else if (transform.Value().CryptoTransform == CryptoTransformDataType::kMacVerify)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kMacVerify);
    else if (transform.Value().CryptoTransform == CryptoTransformDataType::kMacGenerate)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kMacGenerate);
    else if (transform.Value().CryptoTransform == CryptoTransformDataType::kWrap)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kWrap);
    else if (transform.Value().CryptoTransform == CryptoTransformDataType::kUnwrap)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kUnwrap);
    else if (transform.Value().CryptoTransform == CryptoTransformDataType::kSigVerify)
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kSigVerify);
    else
      return ara::core::Result<CryptoTransform>::FromValue(CryptoTransform::kSigGenerate);
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC GetTransformation() Error: " << transform.Error().Message();
    return ara::core::Result<CryptoTransform>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
  }
}

ara::core::Result<void> AuthCipherContextProxyImpl::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  logger_.LogDebug() << "IPC::APP SetKey(const SymmetricKey& key)";
  const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&key);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  CryptoTransformDataType const transformDataType = static_cast<CryptoTransformDataType>(transform);
  logger_.LogDebug() << "SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = accProxyHandler_->SetKey(serviceInstanceId, transformDataType).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC SetKey() Error: " << keyResult.Error().Message();
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

ara::core::Result<void> AuthCipherContextProxyImpl::Start(ReadOnlyMemRegion iv) noexcept
{
  CryptoReadOnlyMemRegion vtrUint8t(iv.begin(), iv.end());
  auto resultStart = accProxyHandler_->StartReadOnlyMem(vtrUint8t).GetResult();
  if (resultStart.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC StartReadOnlyMem() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC StartReadOnlyMem() Error: " << resultStart.Error().Message();
    if (resultStart.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
    }
    else if (resultStart.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUnsupported);
    }
  }
}

ara::core::Result<void> AuthCipherContextProxyImpl::Start(const SecretSeed& iv) noexcept
{
  const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&iv);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  logger_.LogDebug() << "IPC::APP AuthCipherContextProxyImpl Method -> Start(const SecretSeed& iv)";
  logger_.LogDebug() << "Start() CryptoServiceInstanceId : " << serviceInstanceId;
  auto startResult = accProxyHandler_->StartSecretSeed(serviceInstanceId).GetResult();
  if (startResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC StartSecretSeed() -> Success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC StartSecretSeed() -> Error: " << startResult.Error().Message();
    if (startResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
    }
    else if (startResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (startResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUnsupported);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
    }
  }
}

ara::core::Result<void> AuthCipherContextProxyImpl::UpdateAssociatedData(
    const RestrictedUseObject& in) noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC  UpdateAssociatedData(const RestrictedUseObject& in)";
  CryptoServiceInstanceId serviceInstanceId;
  CryptoObjectType cryptoObjType = in.cryObjType;
  logger_.LogDebug() << "IPC-APP::ACC  cryptoObjectType =" << static_cast<uint32_t>(cryptoObjType);
  if (cryptoObjType == CryptoObjectType::kSecretSeed)
  {
    const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&in);
    serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kSymmetricKey)
  {
    const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&in);
    serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kPublicKey)
  {
    const ara::crypto::cryp::cryobj::PublicKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::PublicKeyProxyImpl*>(&in);
    serviceInstanceId = obj->GetPublicKeyServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kPrivateKey)
  {
    const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl*>(&in);
    serviceInstanceId = obj->GetPrivateKeyServiceInstanceId();
  }

  RestrictedUseObjectStructure restrictedUseObj;
  restrictedUseObj.RestrictedUseInstanceId = serviceInstanceId;
  restrictedUseObj.RestrictedUseObjectType = static_cast<CryptoObjectTypeIPC>(cryptoObjType);
  logger_.LogDebug() << "IPC-APP::ACC RestrictedUseObject serviceInstanceId = " << serviceInstanceId;
  auto updateResult = accProxyHandler_->UpdateAssociatedDataResUseObj(restrictedUseObj).GetResult();
  if (updateResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC UpdateAssociatedData() -> Success";
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC UpdateAssociatedData() -> Error: " << updateResult.Error().Message();
    if (updateResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidUsageOrder);
    }
  }
  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> AuthCipherContextProxyImpl::UpdateAssociatedData(ReadOnlyMemRegion in) noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC  UpdateAssociatedData(ReadOnlyMemRegion in)";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto resultUpdate = accProxyHandler_->UpdateAssociatedDataReadOnlyMem(vtrUint8t).GetResult();
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC UpdateAssociatedData() with ReadOnlyMemRegion parameter -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC UpdateAssociatedData() with ReadOnlyMemRegion parameter Error: "
                       << resultUpdate.Error().Message();
    if (resultUpdate.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidUsageOrder);
    }
  }
}

ara::core::Result<void> AuthCipherContextProxyImpl::UpdateAssociatedData(std::uint8_t in) noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC  UpdateAssociatedData(std::uint8_t in)";
  auto resultUpdate = accProxyHandler_->UpdateAssociatedData(in).GetResult();
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC UpdateAssociatedData() with uint8_t as parameter -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC UpdateAssociatedData() with uint8_t as parameter Error: "
                       << resultUpdate.Error().Message();
    if (resultUpdate.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidUsageOrder);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> AuthCipherContextProxyImpl::ProcessConfidentialData(
    ReadOnlyMemRegion in, ara::core::Optional<ReadOnlyMemRegion> expectedTag) noexcept
{
  logger_.LogDebug()
      << "IPC-APP::ACC ProcessConfidentialData(ReadOnlyMemRegion in, ReadOnlyMemRegion* expectedTag)";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  CryptoReadOnlyMemRegion vtrTag(expectedTag->begin(), expectedTag->end());
  auto resultConfData = accProxyHandler_->ProcessConfidentialDataReadOnlyMem(vtrUint8t, vtrTag).GetResult();
  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC ProcessConfidentialData() -> success";
    auto tempVtr = resultConfData.Value().vtrByte;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC ProcessConfidentialData() -> Error: "
                       << resultConfData.Error().Message();
    if (resultConfData.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (resultConfData.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kAuthTagNotValid);
    }
  }
}

ara::core::Result<void> AuthCipherContextProxyImpl::ProcessConfidentialData(
    ReadWriteMemRegion inOut, ara::core::Optional<ReadOnlyMemRegion> expectedTag) noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC ProcessConfidentialData() with ReadWriteMemRegion as argument";
  CryptoReadOnlyMemRegion vtrUint8t(inOut.begin(), inOut.end());
  CryptoReadOnlyMemRegion vtrTag(expectedTag->begin(), expectedTag->end());
  auto tempResult = accProxyHandler_->ProcessConfidentialDataReadOnlyMem(vtrUint8t, vtrTag).GetResult();
  if (tempResult.HasValue())
  {
    logger_.LogDebug()
        << "IPC-APP::ACC ProcessConfidentialData() with ReadWriteMemRegion arguement -> success";
    for (uint16_t i = 0; i < inOut.size(); i++)
    {
      inOut[i] = tempResult.Value().vtrByte[i];
    }
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError()
        << "IPC-APP::ACC ProcessConfidentialData() with ReadWriteMemRegion arguement -> Error: "
        << tempResult.Error().Message();
    if (tempResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else if (tempResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kAuthTagNotValid);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> AuthCipherContextProxyImpl::GetDigest(
    std::size_t offset) const noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC GetDigest()";
  auto resDigest = accProxyHandler_->GetDigest(static_cast<uint64>(offset)).GetResult();
  if (resDigest.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC GetDigest() -> success";
    auto tempVtr = resDigest.Value().vtrResult;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC GetDigest() Error: " << resDigest.Error().Message();
    if (resDigest.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kProcessingNotFinished);
    }
    else if (resDigest.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUsageViolation);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUsageViolation);
    }
  }
}

ara::core::Result<void> AuthCipherContextProxyImpl::Reset() noexcept
{
  auto resReset = accProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC Reset() -> success";
  }
  return ara::core::Result<void>();
}

ara::core::Result<bool> AuthCipherContextProxyImpl::Check(const Signature& expected) const noexcept
{
  logger_.LogDebug() << "IPC-APP::ACC Check(const Signature& expected)";
  const ara::crypto::cryp::cryobj::SignatureProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SignatureProxyImpl*>(&expected);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSignatureServiceInstanceId();
  logger_.LogDebug() << "IPC-APP::ACC Check() CryptoServiceInstanceId : " << serviceInstanceId;
  auto checkResult = accProxyHandler_->Check(serviceInstanceId).GetResult();
  if (checkResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC Check() -> Success";
    return checkResult.Value().isSuccess;
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC Check() -> Error: " << checkResult.Error().Message();
    if (checkResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished)
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kProcessingNotFinished);
    }
    else
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kIncompatibleObject);
    }
  }
}

bool AuthCipherContextProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipAccInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for AuthCipherContext Function: Successful";
    accProxyHandler_ = vsomeipAccInstance_.GetHandlerInstance();
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
