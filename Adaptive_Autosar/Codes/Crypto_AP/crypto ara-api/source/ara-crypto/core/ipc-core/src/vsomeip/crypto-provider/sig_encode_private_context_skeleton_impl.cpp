// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sig_encode_private_context_skeleton_impl.cpp
//
// Purpose     : Sign encode private context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "sig_encode_private_context_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
SignEncodePrivateContextSkeletonImpl::SignEncodePrivateContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, SigEncodePrivateCtx::Uptr signEncPrUptr) noexcept :
    ara::crypto::ipccom::cryp::signencodeprivatecontext::skeleton::SigEncodePrivateContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    signEncPrUptr_{std::move(signEncPrUptr)}
{
  logger_.LogDebug() << "SignEncodePrivateContextSkeletonImpl Constructor";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::SignAndEncodeOutput>
    SignEncodePrivateContextSkeletonImpl::SignAndEncode(const CryptoReadOnlyMemRegion& inputReadOnly)
{
  logger_.LogDebug() << "IPC-CORE::SEPR SignAndEncode()";
  ara::core::Future<SignAndEncodeOutput>::PromiseType promise;
  ReadOnlyMemRegion const signEncode{inputReadOnly};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const result{
      signEncPrUptr_->SignAndEncode(signEncode)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SEPR SignAndEncode() success";
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    SignAndEncodeOutput signAndEncodeOutput;
    for_each(tempVtr.begin(), tempVtr.end(), [&signAndEncodeOutput](ara::core::Byte i) {
      signAndEncodeOutput.vectorBytes.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(signAndEncodeOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SEPR SignAndEncode() Error: " << result.Error().Message();
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
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetMaxInputSizeOutput>
    SignEncodePrivateContextSkeletonImpl::GetMaxInputSize(const Boolean& suppressPadding)
{
  logger_.LogDebug() << "IPC-CORE::SEPR GetMaxInputSize()";
  ara::core::Future<GetMaxInputSizeOutput>::PromiseType promise;
  GetMaxInputSizeOutput maxInputOutput;
  std::size_t localMaxInputSize{signEncPrUptr_->GetMaxInputSize(suppressPadding)};
  maxInputOutput.inputSize = localMaxInputSize;
  promise.set_value(maxInputOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetMaxOutputSizeOutput>
    SignEncodePrivateContextSkeletonImpl::GetMaxOutputSize(const Boolean& suppressPadding)
{
  logger_.LogDebug() << "IPC-CORE::SEPR GetMaxOutputSizess()";
  ara::core::Future<GetMaxOutputSizeOutput>::PromiseType promise;
  GetMaxOutputSizeOutput maxOutput;
  std::size_t localMaxOutputSize{signEncPrUptr_->GetMaxOutputSize(suppressPadding)};
  maxOutput.outputSize = localMaxOutputSize;
  promise.set_value(maxOutput);
  return promise.get_future();
}

ara::core::Future<void> SignEncodePrivateContextSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& privateKeyInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::SEPR SetKey()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::PrivateKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::PrivateKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::DEPR  Crypto Service Instance Id : " << privateKeyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::PrivateKeySkeletonImpl>& privateKeySkeleton{
      dataStorageContainer->GetObjectFromContanier(privateKeyInstanceId)};
  auto& privateKeyUptr{privateKeySkeleton->GetPrivateKeyUptrInstance()};
  ara::core::Result<void> tempResult{signEncPrUptr_->SetKey(*privateKeyUptr)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SEPR SetKey() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE:SEPR SetKey() Error: " << tempResult.Error().Message();
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

ara::core::Future<void> SignEncodePrivateContextSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::SEPR Reset()";
  signEncPrUptr_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::IsInitializedOutput>
    SignEncodePrivateContextSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::SEPR IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = signEncPrUptr_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetCryptoPrimitiveIdOutput>
    SignEncodePrivateContextSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::SEPR GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{signEncPrUptr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}
ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetExtensionServiceOutput>
    SignEncodePrivateContextSkeletonImpl::GetExtensionService()
{
  logger_.LogDebug() << "In SignEncodePrivateContextSkeletonImpl::GetExtensionService()";
  ara::core::Future<ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::
          GetExtensionServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetExtensionServiceOutput
      localExtensionServiceOutput;
  ExtensionService::Uptr extensionUptr{signEncPrUptr_->GetExtensionService()};
  if (extensionUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:SignEncodePrivateContextSkeletonImpl::GetExtensionService() successful: ";
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
        << "IPC-CORE:SignEncodePrivateContextSkeletonImpl::GetExtensionService() Uptr is nullptr: ";
    promise.set_value(localExtensionServiceOutput);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
