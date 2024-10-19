// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signer_private_context_skeleton_impl.cpp
//
// Purpose     : Signer private context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "signer_private_context_skeleton_impl.h"

#include "hash_function_container.h"
#include "signature_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
SignerPrivateContextSkeletonImpl::SignerPrivateContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, SignerPrivateCtx::Uptr signerPrUptr) noexcept :
    ara::crypto::ipccom::cryp::signerprivatecontext::skeleton::SignerPrivateContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    signerPrUptr_{std::move(signerPrUptr)}
{
  logger_.LogDebug() << "SignerPrivateContextSkeletonImpl Constructor";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::SignPreHashedHashFunctionOutput>
    SignerPrivateContextSkeletonImpl::SignPreHashedHashFunction(
        const CryptoServiceInstanceId& hashFunctionInstanceId, const CryptoReadOnlyMemRegion& contextReadOnly)
{
  std::shared_ptr<ara::crypto::common::crypdatastore::HashFunctionContainer> hashStorageContainer{
      ara::crypto::common::crypdatastore::HashFunctionContainer::GetInstance()};
  std::unique_ptr<HashFunctionSkeletonImpl>& hashUptr{
      hashStorageContainer->GetObjectFromContanier(hashFunctionInstanceId)};
  HashFunctionCtx::Uptr& hashFunctionCtxPtr{hashUptr->GetHashFunctionUptr()};
  logger_.LogDebug() << "IPC-CORE::SRPR SignPreHashedHashFunction()";
  ara::core::Future<SignPreHashedHashFunctionOutput>::PromiseType promise;
  ReadOnlyMemRegion const cLabel{contextReadOnly};
  ara::core::Result<Signature::Uptrc> resultSignPre{
      signerPrUptr_->SignPreHashed(*hashFunctionCtxPtr, cLabel)};
  if (resultSignPre.HasValue())
  {
    SignPreHashedHashFunctionOutput tempOutput;
    std::shared_ptr<ara::crypto::common::datastorage::SignatureContainer> signatureDataContainer{
        ara::crypto::common::datastorage::SignatureContainer::GetInstance()};
    CryptoServiceInstanceId signaturelocalInstanceId{signatureDataContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SignatureSkeletonImpl> localSignatureUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SignatureSkeletonImpl>(
            signaturelocalInstanceId, std::move(resultSignPre).Value())};
    localSignatureUtr->OfferService();
    signatureDataContainer->StoreObjectToContanier(signaturelocalInstanceId, std::move(localSignatureUtr));
    tempOutput.signatureInstanceId = signaturelocalInstanceId;
    promise.set_value(tempOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SRPR SignPreHashedHashFunction() Error: "
                       << resultSignPre.Error().Message();
    if (resultSignPre.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else if (resultSignPre.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else if (resultSignPre.Error() == SecurityErrorDomain::Errc::kProcessingNotFinished)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::SignPreHashedReadOnlyOutput>
    SignerPrivateContextSkeletonImpl::SignPreHashedReadOnly(const CryptoAlgId& hashAlgId,
        const CryptoReadOnlyMemRegion& hashValueReadOnly, const CryptoReadOnlyMemRegion& contextReadOnly)
{
  logger_.LogDebug() << "IPC-CORE::SRPR SignPreHashedReadOnly()";
  ara::core::Future<SignPreHashedReadOnlyOutput>::PromiseType promise;
  ReadOnlyMemRegion const valueData{hashValueReadOnly};
  ReadOnlyMemRegion const cLabel{contextReadOnly};
  ara::core::Result<Signature::Uptrc> resultSignPre{
      signerPrUptr_->SignPreHashed(hashAlgId, valueData, cLabel)};
  if (resultSignPre.HasValue())
  {
    SignPreHashedReadOnlyOutput tempOutput;
    std::shared_ptr<ara::crypto::common::datastorage::SignatureContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SignatureContainer::GetInstance()};
    CryptoServiceInstanceId signaturelocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SignatureSkeletonImpl> localSignatureUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SignatureSkeletonImpl>(
            signaturelocalInstanceId, std::move(resultSignPre).Value())};
    localSignatureUtr->OfferService();
    dataStorageContainer->StoreObjectToContanier(signaturelocalInstanceId, std::move(localSignatureUtr));
    tempOutput.signatureInstanceId = signaturelocalInstanceId;
    promise.set_value(tempOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SRPR SignPreHashedReadOnly() Error: " << resultSignPre.Error().Message();
    if (resultSignPre.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else if (resultSignPre.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
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

ara::core::Future<ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::SignReadOnlyOutput>
    SignerPrivateContextSkeletonImpl::SignReadOnly(
        const CryptoReadOnlyMemRegion& valueReadOnly, const CryptoReadOnlyMemRegion& contextReadOnly)
{
  ara::core::Future<SignReadOnlyOutput>::PromiseType promise;
  ReadOnlyMemRegion const valueData{valueReadOnly};
  ReadOnlyMemRegion const cLabel{contextReadOnly};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> resultSign{signerPrUptr_->Sign(valueData, cLabel)};
  if (resultSign.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SRPR Sign() success";
    ara::core::Vector<ara::core::Byte> tempVtr{resultSign.Value()};
    SignReadOnlyOutput signReadOnlyOutput;
    for_each(tempVtr.begin(), tempVtr.end(), [&signReadOnlyOutput](ara::core::Byte i) {
      signReadOnlyOutput.vectorBytes.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(signReadOnlyOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::SRPR Sign() Error: " << resultSign.Error().Message();
    if (resultSign.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
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

ara::core::Future<void> SignerPrivateContextSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& privateKeyInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::SRPR SetKey()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::PrivateKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::PrivateKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::DEPR  Crypto Service Instance Id : " << privateKeyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::PrivateKeySkeletonImpl>& privateKeySkeleton{
      dataStorageContainer->GetObjectFromContanier(privateKeyInstanceId)};
  auto& privateKeyUptr{privateKeySkeleton->GetPrivateKeyUptrInstance()};
  ara::core::Result<void> tempResult{signerPrUptr_->SetKey(*privateKeyUptr)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::SRPR SetKey() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE:SRPR SetKey() Error: " << tempResult.Error().Message();
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

ara::core::Future<void> SignerPrivateContextSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::SRPR Reset()";
  signerPrUptr_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::IsInitializedOutput>
    SignerPrivateContextSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::SRPR IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = signerPrUptr_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::GetCryptoPrimitiveIdOutput>
    SignerPrivateContextSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::SRPR GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{signerPrUptr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}
ara::core::Future<
    ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::GetSignatureServiceOutput>
    SignerPrivateContextSkeletonImpl::GetSignatureService()
{
  logger_.LogDebug() << "In SignerPrivateContextSkeletonImpl::GetSignatureService()";
  ara::core::Future<ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::
          GetSignatureServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::GetSignatureServiceOutput
      getSignatureServiceOutputObj;
  SignatureService::Uptr extensionUptr{signerPrUptr_->GetSignatureService()};
  if (extensionUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:SignerPrivateContextSkeletonImpl::GetSignatureService() successful: ";
    std::shared_ptr<ara::crypto::common::datastorage::SignatureServiceContainer> signatureServiceContainerPtr{
        ara::crypto::common::datastorage::SignatureServiceContainer::GetInstance()};
    CryptoServiceInstanceId localInstanceId{
        signatureServiceContainerPtr->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryservice::SignatureServiceSkeletonImpl> localBlockService{
        std::make_unique<ara::crypto::cryp::cryservice::SignatureServiceSkeletonImpl>(
            localInstanceId, std::move(extensionUptr))};
    localBlockService->OfferService();
    signatureServiceContainerPtr->StoreObjectToContanier(
        localInstanceId, std::move(localBlockService));
    getSignatureServiceOutputObj.signatureServiceInstanceId = localInstanceId;
    promise.set_value(getSignatureServiceOutputObj);
  }
  else
  {
    logger_.LogError()
        << "IPC-CORE:SignerPrivateContextSkeletonImpl::GetSignatureService() Uptr is nullptr: ";
    promise.set_value(getSignatureServiceOutputObj);
  }
  return promise.get_future();



}
} // namespace cryp
} // namespace crypto
} // namespace ara
