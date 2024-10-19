// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : msg_recovery_public_context_skeleton_impl.cpp
//
// Purpose     : Message recovery public context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "msg_recovery_public_context_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
MsgRecoveryPublicContextSkeletonImpl::MsgRecoveryPublicContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, MsgRecoveryPublicCtx::Uptr msgRecPubUptr) noexcept :
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::skeleton::MsgRecoveryPublicContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    msgRecPubUptr_{std::move(msgRecPubUptr)}
{
  logger_.LogDebug() << "MsgRecoveryPublicContextSkeletonImpl Constructor";
}

ara::core::Future<void> MsgRecoveryPublicContextSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& publicKeyInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::MRPB SetKey()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::PublicKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::PublicKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::MRPB  Crypto Service Instance Id : " << publicKeyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl>& publicKeySkeleton{
      dataStorageContainer->GetObjectFromContanier(publicKeyInstanceId)};
  auto& publicKeyUptr{publicKeySkeleton->GetPublicKeyUptrInstance()};
  ara::core::Result<void> tempResult{msgRecPubUptr_->SetKey(*publicKeyUptr)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::MRPB SetKey() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE:MRPB SetKey() Error: " << tempResult.Error().Message();
    if (tempResult.Error() == SecurityErrorDomain::Errc::kIncompatibleObject)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::DecodeAndVerifyOutput>
    MsgRecoveryPublicContextSkeletonImpl::DecodeAndVerify(const CryptoReadOnlyMemRegion& inputReadOnly)
{
  logger_.LogDebug() << "IPC-CORE::MRPB DecodeAndVerify()";
  ara::core::Future<DecodeAndVerifyOutput>::PromiseType promise;
  ReadOnlyMemRegion const decodeVerify{inputReadOnly};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const result{
      msgRecPubUptr_->DecodeAndVerify(decodeVerify)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::MRPB DecodeAndVerify() success";
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    DecodeAndVerifyOutput decodeAndVerifyOutput;
    for_each(tempVtr.begin(), tempVtr.end(), [&decodeAndVerifyOutput](ara::core::Byte i) {
      decodeAndVerifyOutput.vectorBytes.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(decodeAndVerifyOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::MRPB DecodeAndVerify() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::GetMaxInputSizeOutput>
    MsgRecoveryPublicContextSkeletonImpl::GetMaxInputSize(const Boolean& suppressPadding)
{
  logger_.LogDebug() << "IPC-CORE::MRPB GetMaxInputSize()";
  ara::core::Future<GetMaxInputSizeOutput>::PromiseType promise;
  GetMaxInputSizeOutput maxInputOutput;
  std::size_t localMaxInputSize{msgRecPubUptr_->GetMaxInputSize(suppressPadding)};
  maxInputOutput.inputSize = localMaxInputSize;
  promise.set_value(maxInputOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::GetMaxOutputSizeOutput>
    MsgRecoveryPublicContextSkeletonImpl::GetMaxOutputSize(const Boolean& suppressPadding)
{
  logger_.LogDebug() << "IPC-CORE::MRPB GetMaxOutputSize()";
  ara::core::Future<GetMaxOutputSizeOutput>::PromiseType promise;
  GetMaxOutputSizeOutput maxOutput;
  std::size_t localMaxOutputSize{msgRecPubUptr_->GetMaxOutputSize(suppressPadding)};
  maxOutput.outputSize = localMaxOutputSize;
  promise.set_value(maxOutput);
  return promise.get_future();
}

ara::core::Future<void> MsgRecoveryPublicContextSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::MRPB Reset()";
  msgRecPubUptr_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::IsInitializedOutput>
    MsgRecoveryPublicContextSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::MRPB IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = msgRecPubUptr_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::GetCryptoPrimitiveIdOutput>
    MsgRecoveryPublicContextSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::MRPB GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{msgRecPubUptr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::GetExtensionServiceOutput>
    MsgRecoveryPublicContextSkeletonImpl::GetExtensionService()
{
  logger_.LogDebug() << "In MsgRecoveryPublicContextSkeletonImpl::GetExtensionService()";
  ara::core::Future<ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::
          GetExtensionServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::GetExtensionServiceOutput
      localExtensionServiceOutput;
  ExtensionService::Uptr extensionUptr{msgRecPubUptr_->GetExtensionService()};
  if (extensionUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:MsgRecoveryPublicContextSkeletonImpl::GetExtensionService() successful: ";
    std::shared_ptr<ara::crypto::common::datastorage::ExtensionServiceContainer> extensionServiceContainerPtr{
        ara::crypto::common::datastorage::ExtensionServiceContainer::GetInstance()};
    CryptoServiceInstanceId extensionlocalInstanceId{
        extensionServiceContainerPtr->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryservice::ExtensionServiceSkeletonImpl> localBlockService{
        std::make_unique<ara::crypto::cryp::cryservice::ExtensionServiceSkeletonImpl>(
            extensionlocalInstanceId, std::move(extensionUptr))};
    localBlockService->OfferService();
    extensionServiceContainerPtr->StoreObjectToContanier(
        extensionlocalInstanceId, std::move(localBlockService));
    localExtensionServiceOutput.extensionServiceInstanceId = extensionlocalInstanceId;
    promise.set_value(localExtensionServiceOutput);
  }
  else
  {
    logger_.LogError()
        << "IPC-CORE:MsgRecoveryPublicContextSkeletonImpl::GetExtensionService() Uptr is nullptr: ";
    promise.set_value(localExtensionServiceOutput);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
