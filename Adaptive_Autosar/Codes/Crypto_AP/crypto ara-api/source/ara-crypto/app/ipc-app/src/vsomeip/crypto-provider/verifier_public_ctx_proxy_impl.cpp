// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : verifier_public_ctx_proxy_impl.cpp
//
// Purpose     : Verifier public context proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "verifier_public_ctx_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
VerifierPublicCtxProxyImpl::VerifierPublicCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        verPubCryptoProviderHandler,
    CryptoProvider& cp) noexcept :
    verPubCryptoProviderHandler_{verPubCryptoProviderHandler},
    cpUptr_{cp}, logger_{
                     ara::log::CreateLogger("VRPB", "VerifierPublicCtx Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "VerifierPublicCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "server client connection for VerifierPublicCtx -> success";
  }
  else
  {
    logger_.LogError() << "server client connection for VerifierPublicCtx -> fail";
  }
}

ara::core::Result<void> VerifierPublicCtxProxyImpl::SetKey(const PublicKey& key) noexcept
{
  logger_.LogDebug() << "IPC-APP::VRPB SetKey()";
  const ara::crypto::cryp::cryobj::PublicKeyProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::cryobj::PublicKeyProxyImpl*>(&key)};
  CryptoServiceInstanceId const serviceInstanceId{obj->GetPublicKeyServiceInstanceId()};
  logger_.LogDebug() << "IPC::APP VerifierPublicCtxProxyImpl Method -> SetKey(const PublicKey& key)";
  logger_.LogDebug() << "SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = verPubProxyHandler_->SetKey(serviceInstanceId).GetResult();
  if (keyResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::VRPB SetKey() -> success ";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::VRPB SetKey() Error: " << keyResult.Error().Message();
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

ara::core::Result<bool> VerifierPublicCtxProxyImpl::VerifyPrehashed(
    const HashFunctionCtx& hashFn, const Signature& signature, ReadOnlyMemRegion context) const noexcept
{
  logger_.LogDebug() << "IPC-APP::VRPB VerifyPrehashedHashFunctionSignature()";
  const ara::crypto::cryp::HashFunctionCtxProxyImpl* hashObj{
      dynamic_cast<const ara::crypto::cryp::HashFunctionCtxProxyImpl*>(&hashFn)};
  CryptoServiceInstanceId const hashFunctionServiceInstanceId{hashObj->GetHashFunctionCtxServiceInstanceId()};
  logger_.LogDebug() << "SignPreHashed() HashFunctionCtx CryptoServiceInstanceId : "
                     << hashFunctionServiceInstanceId;
  const ara::crypto::cryp::cryobj::SignatureProxyImpl* sigObj{
      dynamic_cast<const ara::crypto::cryp::cryobj::SignatureProxyImpl*>(&signature)};
  CryptoServiceInstanceId const signatureServiceInstanceId{sigObj->GetSignatureServiceInstanceId()};
  logger_.LogDebug() << "IPC-APP::VRPB Signature CryptoServiceInstanceId : " << signatureServiceInstanceId;
  CryptoReadOnlyMemRegion vtrContext(context.begin(), context.end());
  auto resultConfData = verPubProxyHandler_
                            ->VerifyPrehashedHashFunctionSignature(
                                hashFunctionServiceInstanceId, signatureServiceInstanceId, vtrContext)
                            .GetResult();
  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::VRPB VerifyPrehashedHashFunctionSignature() -> success";
    bool isResult{resultConfData.Value().result};
    return ara::core::Result<bool>::FromValue(isResult);
  }
  else
  {
    logger_.LogError() << "IPC-APP::VRPB VerifyPrehashedHashFunctionSignature() -> Error: "
                       << resultConfData.Error().Message();
    if (resultConfData.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished)
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kProcessingNotFinished);
    }
    else
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kInvalidArgument);
    }
  }
}

ara::core::Result<bool> VerifierPublicCtxProxyImpl::Verify(
    ReadOnlyMemRegion value, ReadOnlyMemRegion signature, ReadOnlyMemRegion context) const noexcept
{
  logger_.LogDebug() << "IPC-APP::VRPB VerifyReadOnlyMemRegion()";
  CryptoReadOnlyMemRegion vtrValue(value.begin(), value.end());
  CryptoReadOnlyMemRegion vtrSignature(signature.begin(), signature.end());
  CryptoReadOnlyMemRegion vtrContext(context.begin(), context.end());
  auto resultConfData = verPubProxyHandler_->VerifyReadOnly(vtrValue, vtrSignature, vtrContext).GetResult();
  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::VRPB VerifyReadOnlyMemRegion() -> success";
    bool isResult{resultConfData.Value().result};
    return ara::core::Result<bool>::FromValue(isResult);
  }
  else
  {
    logger_.LogError() << "IPC-APP::VRPB VerifyReadOnlyMemRegion() -> Error: "
                       << resultConfData.Error().Message();
    if (resultConfData.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize)
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    else
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
}

ara::core::Result<bool> VerifierPublicCtxProxyImpl::VerifyPrehashed(CryptoAlgId hashAlgId,
    ReadOnlyMemRegion hashValue, const Signature& signature, ReadOnlyMemRegion context) const noexcept
{
  logger_.LogDebug() << "IPC-APP::VRPB VerifyReadOnlyMemRegion()";
  const ara::crypto::cryp::cryobj::SignatureProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::cryobj::SignatureProxyImpl*>(&signature)};
  CryptoServiceInstanceId const serviceInstanceId{obj->GetSignatureServiceInstanceId()};
  logger_.LogDebug() << "IPC-APP::VRPB Check() CryptoServiceInstanceId : " << serviceInstanceId;
  CryptoReadOnlyMemRegion vtrHashValue(hashValue.begin(), hashValue.end());
  CryptoReadOnlyMemRegion vtrContext(context.begin(), context.end());
  auto resultConfData
      = verPubProxyHandler_->VerifyPrehashedReadOnly(hashAlgId, vtrHashValue, serviceInstanceId, vtrContext)
            .GetResult();
  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::VRPB VerifyReadOnlyMemRegion() -> success";
    bool isResult{resultConfData.Value().result};
    return ara::core::Result<bool>::FromValue(isResult);
  }
  else
  {
    logger_.LogError() << "IPC-APP::VRPB VerifyReadOnlyMemRegion() -> Error: "
                       << resultConfData.Error().Message();
    if (resultConfData.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject)
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kIncompatibleObject);
    }
    else if (resultConfData.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments)
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    else
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kBadObjectReference);
    }
  }
}

ara::core::Result<bool> VerifierPublicCtxProxyImpl::VerifyPrehashed(
    const HashFunctionCtx& hashFn, ReadOnlyMemRegion signature, ReadOnlyMemRegion context) const noexcept
{
  logger_.LogDebug() << "IPC-APP::VRPB VerifyPrehashedHashFunctionSignature()";
  const ara::crypto::cryp::HashFunctionCtxProxyImpl* hashObj{
      dynamic_cast<const ara::crypto::cryp::HashFunctionCtxProxyImpl*>(&hashFn)};
  CryptoServiceInstanceId const hashFunctionServiceInstanceId{hashObj->GetHashFunctionCtxServiceInstanceId()};
  logger_.LogDebug() << "SignPreHashed() HashFunctionCtx CryptoServiceInstanceId : "
                     << hashFunctionServiceInstanceId;
  CryptoReadOnlyMemRegion vtrSignature(signature.begin(), signature.end());
  CryptoReadOnlyMemRegion vtrContext(context.begin(), context.end());
  auto resultConfData
      = verPubProxyHandler_
            ->VerifyPrehashedHashFunction(hashFunctionServiceInstanceId, vtrSignature, vtrContext)
            .GetResult();
  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::VRPB VerifyPrehashedHashFunctionSignature() -> success";
    bool isResult{resultConfData.Value().result};
    return ara::core::Result<bool>::FromValue(isResult);
  }
  else
  {
    logger_.LogError() << "IPC-APP::VRPB VerifyPrehashedHashFunctionSignature() -> Error: "
                       << resultConfData.Error().Message();
    if (resultConfData.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished)
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kProcessingNotFinished);
    }
    else
    {
      return ara::core::Result<bool>::FromError(SecurityErrorDomain::Errc::kInvalidArgument);
    }
  }
}

ara::core::Result<void> VerifierPublicCtxProxyImpl::Reset() noexcept
{
  auto resReset = verPubProxyHandler_->Reset().GetResult();
  if (resReset.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::VRPB Reset() -> success";
  }
  return ara::core::Result<void>();
}

CryptoPrimitiveId::Uptr VerifierPublicCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::VRPB -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = verPubProxyHandler_->GetCryptoPrimitiveId().GetResult();
  if (primitiveResult.HasValue())
  {
    const ara::core::StringView primitiveName{
        primitiveResult.Value().cryptoPrimitiveIdUptrStruct.primitiveName};
    const AlgId algorithmId{primitiveResult.Value().cryptoPrimitiveIdUptrStruct.primitiveAlgId};
    cryptoPrimitiveIdUptrLocal
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::VRPB -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
  }
  return cryptoPrimitiveIdUptrLocal;
}

bool VerifierPublicCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = verPubProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::VRPB -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::VRPB -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& VerifierPublicCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::VRPB -> MyProvider()";
  return cpUptr_;
}

SignatureService::Uptr VerifierPublicCtxProxyImpl::GetSignatureService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::VRPB -> GetSignatureService()";
  auto signatureService = verPubProxyHandler_->GetSignatureService().GetResult();
  if (!signatureService.Value().signatureServiceInstanceId.empty())
  {
    CryptoServiceInstanceId instanceId = signatureService.Value().signatureServiceInstanceId;
    logger_.LogDebug() << "IPC-APP::VRPB Service InstanceId for GetSignatureService : " << instanceId;
    SignatureService::Uptr signatureServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::SignatureServiceProxyImpl>(instanceId);
    return signatureServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::VRPB Service InstanceId for GetSignatureService is null";
    return nullptr;
  }
}

bool VerifierPublicCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipVpcInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for VerifierPublicCtx Function: Successful";
    verPubProxyHandler_ = vsomeipVpcInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for VerifierPublicCtx Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
