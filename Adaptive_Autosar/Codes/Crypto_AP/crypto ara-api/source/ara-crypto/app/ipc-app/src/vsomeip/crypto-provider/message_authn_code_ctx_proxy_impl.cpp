// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : message_authn_code_ctx_proxy_impl.cpp
//
// Purpose     : Message authentication code proxy implementation to forward functional calls from application
//               to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "message_authn_code_ctx_proxy_impl.h"

#include <algorithm>
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
MessageAuthnCodeCtxProxyImpl::MessageAuthnCodeCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        macCPHandler,
    CryptoProvider& cp) noexcept :
    macCryptoProviderHandler_{macCPHandler},
    cpUptr_{cp}, logger_{
                     ara::log::CreateLogger("MACP", "Message Authn Code Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "IPC-APP::MAC MessageAuthnCodeCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "IPC-APP::MAC Server client connection for MessageAuthnCodeCtx -> success";
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC Server client connection for MessageAuthnCodeCtx -> fail";
  }
}

bool MessageAuthnCodeCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = macProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MAC -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::MAC -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& MessageAuthnCodeCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC -> MyProvider()";
  return cpUptr_;
}

DigestService::Uptr MessageAuthnCodeCtxProxyImpl::GetDigestService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC -> GetDigestService()";
  auto digestService = macProxyHandler_->GetDigestService().GetResult();
  if (!digestService.Value().instanceId.empty())
  {
    CryptoServiceInstanceId instanceId = digestService.Value().instanceId;
    logger_.LogDebug() << "IPC-APP::MAC Service InstanceId for GetDigestService : " << instanceId;
    DigestService::Uptr digestServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::DigestServiceProxyImpl>(instanceId);
    return digestServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::MAC Service InstanceId for GetDigestService is null";
    return nullptr;
  }
}

CryptoPrimitiveId::Uptr MessageAuthnCodeCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = macProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::MAC -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

ara::core::Result<void> MessageAuthnCodeCtxProxyImpl::Start(ReadOnlyMemRegion iv) noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC StartReadOnlyMem()";
  CryptoReadOnlyMemRegion vtrUint8t(iv.begin(), iv.end());
  auto resultStart = macProxyHandler_->StartReadOnlyMem(vtrUint8t).GetResult();
  if (resultStart.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MAC StartReadOnlyMem() -> success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC StartReadOnlyMem() Error: " << resultStart.Error().Message();
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

ara::core::Result<void> MessageAuthnCodeCtxProxyImpl::Start(const SecretSeed& iv) noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC StartSecretSeed()";
  const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&iv);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  logger_.LogDebug() << "Start() CryptoServiceInstanceId : " << serviceInstanceId;
  auto startResult = macProxyHandler_->StartSecretSeed(serviceInstanceId).GetResult();
  if (startResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MAC StartSecretSeed() -> Success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC StartSecretSeed() -> Error: " << startResult.Error().Message();
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
    else if (startResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation)
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
    }
    else
    {
      return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUnsupported);
    }
  }
}

ara::core::Result<void> MessageAuthnCodeCtxProxyImpl::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC SetKey()";
  const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&key);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  CryptoTransformDataType const transformDataType = static_cast<CryptoTransformDataType>(transform);
  logger_.LogDebug() << "SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = macProxyHandler_->SetKey(serviceInstanceId, transformDataType).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MAC SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC SetKey() Error: " << keyResult.Error().Message();
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

ara::core::Result<void> MessageAuthnCodeCtxProxyImpl::Update(ReadOnlyMemRegion in) noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC UpdateReadOnlyMem() called";
  CryptoReadOnlyMemRegion vtrUint8t(in.begin(), in.end());
  auto resultUpdate = macProxyHandler_->UpdateReadOnlyMem(vtrUint8t).GetResult();
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-APP:MAC UpdateReadOnlyMem() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC UpdateReadOnlyMem() Error: " << resultUpdate.Error().Message();
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
  }
}

ara::core::Result<void> MessageAuthnCodeCtxProxyImpl::Update(std::uint8_t in) noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC UpdateReadOnlyMem() called";
  auto resultUpdate = macProxyHandler_->Update(in).GetResult();
  if (resultUpdate.HasValue())
  {
    logger_.LogDebug() << "IPC-APP:MAC UpdateReadOnlyMem() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC UpdateReadOnlyMem() Error: " << resultUpdate.Error().Message();
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
  }
}

ara::core::Result<void> MessageAuthnCodeCtxProxyImpl::Update(const RestrictedUseObject& in) noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC  Update(const RestrictedUseObject& in)";
  CryptoServiceInstanceId serviceInstanceId;
  CryptoObjectType cryptoObjType = in.cryObjType;
  logger_.LogDebug() << "IPC-APP::MAC  cryptoObjectType =" << static_cast<uint32_t>(cryptoObjType);
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

  RestrictedUseObjectStructure restrictedUseObj;
  restrictedUseObj.RestrictedUseInstanceId = serviceInstanceId;
  restrictedUseObj.RestrictedUseObjectType = static_cast<CryptoObjectTypeIPC>(cryptoObjType);
  logger_.LogDebug() << "IPC-APP::MAC RestrictedUseObject serviceInstanceId = " << serviceInstanceId;
  auto updateResult = macProxyHandler_->UpdateRestrictedUseObject(restrictedUseObj).GetResult();
  if (updateResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::ACC UpdateAssociatedData() -> Success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::ACC UpdateAssociatedData() -> Error: " << updateResult.Error().Message();
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
  }
}

ara::core::Result<Signature::Uptrc> MessageAuthnCodeCtxProxyImpl::Finish(bool makeSignatureObject) noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC Finish()";
  auto tempResult = macProxyHandler_->Finish(makeSignatureObject).GetResult();
  if (tempResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = tempResult.Value().finishInstanceId;
    logger_.LogDebug() << "IPC-APP::MAC Service InstanceId for Signature Object : " << instanceId;
    Signature::Uptrc signatureUptrc
        = std::make_unique<ara::crypto::cryp::cryobj::SignatureProxyImpl>(instanceId);
    return ara::core::Result<Signature::Uptrc>(std::move(signatureUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC Finish() -> Error: " << tempResult.Error().Message();
    if (tempResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted)
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
    else
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
    }
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> MessageAuthnCodeCtxProxyImpl::GetDigest(
    std::size_t offset) const noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC GetDigest()";
  auto resDigest = macProxyHandler_->GetDigest(static_cast<uint64>(offset)).GetResult();
  if (resDigest.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MAC GetDigest() -> success";
    auto tempVtr = resDigest.Value().digestResult;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC GetDigest() Error: " << resDigest.Error().Message();
    if (resDigest.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kProcessingNotFinished);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUsageViolation);
    }
  }
}

ara::core::Result<bool> MessageAuthnCodeCtxProxyImpl::Check(const Signature& expected) const noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC Check()";
  const ara::crypto::cryp::cryobj::SignatureProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SignatureProxyImpl*>(&expected);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSignatureServiceInstanceId();
  logger_.LogDebug() << "IPC-APP::MAC Check() CryptoServiceInstanceId : " << serviceInstanceId;
  auto checkResult = macProxyHandler_->Check(serviceInstanceId).GetResult();
  if (checkResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MAC Check() -> Success";
    return checkResult.Value().result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC Check() -> Error: " << checkResult.Error().Message();
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

ara::core::Result<void> MessageAuthnCodeCtxProxyImpl::Reset() noexcept
{
  logger_.LogDebug() << "IPC-APP::MAC Reset()";
  auto resReset = macProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::MAC Reset() -> success";
  }
  return ara::core::Result<void>();
}

bool MessageAuthnCodeCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipMacInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "IPC-APP::MAC Server Client connection for Random Generator : Successful";
    macProxyHandler_ = vsomeipMacInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "IPC-APP::MAC Server Client connection for Random Generator : Failed";
    isSuccess = false;
  }
  return isSuccess;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
