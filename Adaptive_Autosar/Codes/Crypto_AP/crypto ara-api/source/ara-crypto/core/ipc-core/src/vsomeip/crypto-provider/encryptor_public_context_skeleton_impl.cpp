// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : encryptor_public_context_skeleton_impl.cpp
//
// Purpose     : Encryptor public context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "encryptor_public_context_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
EncryptorPrivateContextSkeletonImpl::EncryptorPrivateContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, EncryptorPublicCtx::Uptr enPubUptr) noexcept :
    ara::crypto::ipccom::cryp::encryptorpubliccontext::skeleton::EncryptorPublicContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    enPubUptr_{std::move(enPubUptr)}
{
  logger_.LogDebug() << "EncryptorPrivateContextSkeletonImpl Constructor";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::encryptorpubliccontext::EncryptorPublicContext::ProcessBlockReadOnlyOutput>
    EncryptorPrivateContextSkeletonImpl::ProcessBlockReadOnly(
        const CryptoReadOnlyMemRegion& readOnlyMemoryRegion, const Boolean& suppressPadding)
{
  logger_.LogDebug() << "IPC-CORE::ENPB ProcessBlocks()";
  ara::core::Future<ProcessBlockReadOnlyOutput>::PromiseType promise;
  ReadOnlyMemRegion const proBlocks{readOnlyMemoryRegion};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const result{
      enPubUptr_->ProcessBlock(proBlocks, suppressPadding)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ENPB ProcessBlocks() -> success";
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    ProcessBlockReadOnlyOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(), [&vtrByteResult](ara::core::Byte i) {
      vtrByteResult.vectorBytes.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ENPB ProcessBlocks() Error: " << result.Error().Message();
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

ara::core::Future<void> EncryptorPrivateContextSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& publicKeyInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::ENPB SetKey()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::PublicKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::PublicKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::ENPB  Crypto Service Instance Id : " << publicKeyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl>& publicKeySkeleton{
      dataStorageContainer->GetObjectFromContanier(publicKeyInstanceId)};
  auto& publicKeyUptr{publicKeySkeleton->GetPublicKeyUptrInstance()};
  ara::core::Result<void> const tempResult{enPubUptr_->SetKey(*publicKeyUptr)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::ENPB SetKey() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE:ENPB SetKey() Error: " << tempResult.Error().Message();
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

ara::core::Future<void> EncryptorPrivateContextSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::DRPR Reset()";
  enPubUptr_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::encryptorpubliccontext::EncryptorPublicContext::IsInitializedOutput>
    EncryptorPrivateContextSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::DRPR IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = enPubUptr_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::encryptorpubliccontext::EncryptorPublicContext::GetCryptoPrimitiveIdOutput>
    EncryptorPrivateContextSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::DRPR GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{enPubUptr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}
ara::core::Future<
    ara::crypto::ipccom::cryp::encryptorpubliccontext::EncryptorPublicContext::GetCryptoServiceOutput>
    EncryptorPrivateContextSkeletonImpl::GetCryptoService()
{
  logger_.LogDebug() << "In EncryptorPrivateContextSkeletonImpl::GetCryptoService()";
  ara::core::Future<ara::crypto::ipccom::cryp::encryptorpubliccontext::EncryptorPublicContext::
          GetCryptoServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::encryptorpubliccontext::EncryptorPublicContext::GetCryptoServiceOutput
      localCryptoServiceOutput;
  CryptoService::Uptr cryptoUptr{enPubUptr_->GetCryptoService()};
  if (cryptoUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:EncryptorPrivateContextSkeletonImpl::GetCryptoService() successful: ";
    std::shared_ptr<ara::crypto::common::datastorage::CryptoServiceContainer> cryptoServiceContainer{
        ara::crypto::common::datastorage::CryptoServiceContainer::GetInstance()};
    CryptoServiceInstanceId cryptolocalInstanceId{cryptoServiceContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryservice::CryptoServiceSkeletonImpl> localCryptoService{
        std::make_unique<ara::crypto::cryp::cryservice::CryptoServiceSkeletonImpl>(
            cryptolocalInstanceId, std::move(cryptoUptr))};
    localCryptoService->OfferService();
    cryptoServiceContainer->StoreObjectToContanier(cryptolocalInstanceId, std::move(localCryptoService));
    localCryptoServiceOutput.cryptoServiceInstanceId = cryptolocalInstanceId;
    promise.set_value(localCryptoServiceOutput);
  }
  else
  {
    logger_.LogError()
        << "IPC-CORE:EncryptorPrivateContextSkeletonImpl::GetCryptoService() Uptr is nullptr: ";
    promise.set_value(localCryptoServiceOutput);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
