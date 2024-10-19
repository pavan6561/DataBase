// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : verifier_public_context_skeleton_impl.cpp
//
// Purpose     : Verifier public context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "verifier_public_context_skeleton_impl.h"

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
VerifierPublicContextSkeletonImpl::VerifierPublicContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, VerifierPublicCtx::Uptr verifierPubUptr) noexcept :
    ara::crypto::ipccom::cryp::verifierpubliccontext::skeleton::VerifierPublicContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    verifierPubUptr_{std::move(verifierPubUptr)}
{
  logger_.LogDebug() << "VerifierPublicContextSkeletonImpl Constructor";
}

ara::core::Future<ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::
        VerifyPrehashedHashFunctionOutput>
    VerifierPublicContextSkeletonImpl::VerifyPrehashedHashFunction(
        const CryptoServiceInstanceId& hashFunctionInstanceId,
        const CryptoReadOnlyMemRegion& signatureReadOnly, const CryptoReadOnlyMemRegion& contextReadOnly)
{
  logger_.LogDebug() << "IPC-CORE::VPCS VerifyPrehashedHashFunction()";
  std::shared_ptr<ara::crypto::common::crypdatastore::HashFunctionContainer> hashStorageContainer{
      ara::crypto::common::crypdatastore::HashFunctionContainer::GetInstance()};
  std::unique_ptr<HashFunctionSkeletonImpl>& hashUptr{
      hashStorageContainer->GetObjectFromContanier(hashFunctionInstanceId)};
  HashFunctionCtx::Uptr& hashFunctionCtxPtr{hashUptr->GetHashFunctionUptr()};

  ara::core::Future<VerifyPrehashedHashFunctionOutput>::PromiseType promise;
  ReadOnlyMemRegion const signReadOnlyMem{signatureReadOnly};
  ReadOnlyMemRegion const context{contextReadOnly};
  ara::core::Result<bool> isSuccess{
      verifierPubUptr_->VerifyPrehashed(*hashFunctionCtxPtr, signReadOnlyMem, context)};
  if (isSuccess.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::VPCS VerifyPrehashedHashFunction() -> success";
    VerifyPrehashedHashFunctionOutput verifyOutput;
    verifyOutput.result = isSuccess.Value();
    promise.set_value(verifyOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::VPCS VerifyPrehashedHashFunction() Error: "
                       << isSuccess.Error().Message();
    if (isSuccess.Error() == SecurityErrorDomain::Errc::kProcessingNotFinished)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::
        VerifyPrehashedHashFunctionSignatureOutput>
    VerifierPublicContextSkeletonImpl::VerifyPrehashedHashFunctionSignature(
        const CryptoServiceInstanceId& hashFunctionInstanceId,
        const CryptoServiceInstanceId& signatureInstanceId, const CryptoReadOnlyMemRegion& contextReadOnly)
{
  // Retrieve hashfunctionctx::uptr from data storage and use as parameter
  std::shared_ptr<ara::crypto::common::crypdatastore::HashFunctionContainer> hashStorageContainer{
      ara::crypto::common::crypdatastore::HashFunctionContainer::GetInstance()};
  std::unique_ptr<HashFunctionSkeletonImpl>& hashUptr{
      hashStorageContainer->GetObjectFromContanier(hashFunctionInstanceId)};
  HashFunctionCtx::Uptr& hashFunctionCtxPtr{hashUptr->GetHashFunctionUptr()};

  // Retrieve signature::uptr from signature data storage container and use as parameter
  std::shared_ptr<ara::crypto::common::datastorage::SignatureContainer> signContainer{
      ara::crypto::common::datastorage::SignatureContainer::GetInstance()};
  std::unique_ptr<cryobj::SignatureSkeletonImpl>& signatureUptr{
      signContainer->GetObjectFromContanier(signatureInstanceId)};
  Signature::Uptrc& signPtrc{signatureUptr->GetSignatureUptrInstance()};

  ara::core::Future<VerifyPrehashedHashFunctionSignatureOutput>::PromiseType promise;
  ReadOnlyMemRegion const contextMem{contextReadOnly};
  ara::core::Result<bool> isSuccess{
      verifierPubUptr_->VerifyPrehashed(*hashFunctionCtxPtr, *signPtrc, contextMem)};
  if (isSuccess.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::VPCS VerifyPrehashedHashFunctionSignature() -> success";
    VerifyPrehashedHashFunctionSignatureOutput verifyOutput;
    verifyOutput.result = isSuccess.Value();
    promise.set_value(verifyOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::VPCS SignPreHashedHashFunction() Error: " << isSuccess.Error().Message();
    if (isSuccess.Error() == SecurityErrorDomain::Errc::kProcessingNotFinished)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::VerifyPrehashedReadOnlyOutput>
    VerifierPublicContextSkeletonImpl::VerifyPrehashedReadOnly(const CryptoAlgId& hashAlgId,
        const CryptoReadOnlyMemRegion& hashValueReadOnly, const CryptoServiceInstanceId& signatureInstanceId,
        const CryptoReadOnlyMemRegion& contextReadOnly)
{
  logger_.LogDebug() << "IPC-CORE::VPCS VerifyPrehashedReadOnly()";
  ara::core::Future<VerifyPrehashedReadOnlyOutput>::PromiseType promise;
  ReadOnlyMemRegion const valueData{hashValueReadOnly};
  ReadOnlyMemRegion const cLabel{contextReadOnly};

  // Retrieve signature::uptr from signature data storage container and use as parameter
  std::shared_ptr<ara::crypto::common::datastorage::SignatureContainer> signContainer{
      ara::crypto::common::datastorage::SignatureContainer::GetInstance()};
  std::unique_ptr<cryobj::SignatureSkeletonImpl>& signatureUptr{
      signContainer->GetObjectFromContanier(signatureInstanceId)};
  Signature::Uptrc& signPtrc{signatureUptr->GetSignatureUptrInstance()};

  ara::core::Result<bool> resultVerPub{
      verifierPubUptr_->VerifyPrehashed(hashAlgId, valueData, *signPtrc, cLabel)};
  if (resultVerPub.HasValue())
  {
    VerifyPrehashedReadOnlyOutput tempOutput;
    logger_.LogDebug() << "IPC-CORE::VPCS VerifyPrehashedReadOnly() -> success";
    tempOutput.result = resultVerPub.Value();
    promise.set_value(tempOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::VPCS VerifyPrehashedReadOnly() Error: "
                       << resultVerPub.Error().Message();
    if (resultVerPub.Error() == SecurityErrorDomain::Errc::kIncompatibleObject)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject);
    }
    else if (resultVerPub.Error() == SecurityErrorDomain::Errc::kIncompatibleArguments)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kBadObjectReference);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::VerifyReadOnlyOutput>
    VerifierPublicContextSkeletonImpl::VerifyReadOnly(const CryptoReadOnlyMemRegion& valueReadOnly,
        const CryptoReadOnlyMemRegion& signatureReadOnly, const CryptoReadOnlyMemRegion& contextReadOnly)
{
  logger_.LogDebug() << "IPC-CORE::VPCS VerifyReadOnly()";
  ara::core::Future<VerifyReadOnlyOutput>::PromiseType promise;
  ReadOnlyMemRegion const valueData{valueReadOnly};
  ReadOnlyMemRegion const signatureData{signatureReadOnly};
  ReadOnlyMemRegion const cLabel{contextReadOnly};
  ara::core::Result<bool> resultVerPub{verifierPubUptr_->Verify(valueData, signatureData, cLabel)};
  if (resultVerPub.HasValue())
  {
    VerifyReadOnlyOutput tempOutput;
    tempOutput.result = resultVerPub.Value();
    promise.set_value(tempOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::VPCS VerifyReadOnly() Error: " << resultVerPub.Error().Message();
    if (resultVerPub.Error() == SecurityErrorDomain::Errc::kUninitializedContext)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> VerifierPublicContextSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& publicKeyInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::VPCS SetKey()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::PublicKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::PublicKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::VPCS  Crypto Service Instance Id : " << publicKeyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl>& publicKeySkeleton{
      dataStorageContainer->GetObjectFromContanier(publicKeyInstanceId)};
  auto& publicKeyUptr{publicKeySkeleton->GetPublicKeyUptrInstance()};
  ara::core::Result<void> tempResult{verifierPubUptr_->SetKey(*publicKeyUptr)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::VPCS SetKey() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE:VPCS SetKey() Error: " << tempResult.Error().Message();
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

ara::core::Future<void> VerifierPublicContextSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::VPCS Reset()";
  verifierPubUptr_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::IsInitializedOutput>
    VerifierPublicContextSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::VPCS IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = verifierPubUptr_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::GetCryptoPrimitiveIdOutput>
    VerifierPublicContextSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::VPCS GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{verifierPubUptr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}
ara::core::Future<
    ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::GetSignatureServiceOutput>
    VerifierPublicContextSkeletonImpl::GetSignatureService()
{
  logger_.LogDebug() << "In VerifierPublicContextSkeletonImpl::GetSignatureService()";
  ara::core::Future<ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::
          GetSignatureServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::GetSignatureServiceOutput
      getSignatureServiceOutputObj;
  SignatureService::Uptr extensionUptr{verifierPubUptr_->GetSignatureService()};
  if (extensionUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:VerifierPublicContextSkeletonImpl::GetSignatureService() successful: ";
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
        << "IPC-CORE:VerifierPublicContextSkeletonImpl::GetSignatureService() Uptr is nullptr: ";
    promise.set_value(getSignatureServiceOutputObj);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
