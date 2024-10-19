// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : decryptor_private_context_skeleton_impl.cpp
//
// Purpose     : Decryptor private context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "decryptor_private_context_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
DecryptorPrivateContextSkeletonImpl::DecryptorPrivateContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, DecryptorPrivateCtx::Uptr decPrUptr) noexcept :
    ara::crypto::ipccom::cryp::decryptorprivatecontext::skeleton::DecryptorPrivateContextSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    decPrUptr_{std::move(decPrUptr)}
{
  logger_.LogDebug() << "DecryptorPrivateContextSkeletonImpl Constructor";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::ProcessBlockReadOnlyOutput>
    DecryptorPrivateContextSkeletonImpl::ProcessBlockReadOnly(
        const CryptoReadOnlyMemRegion& readOnlyMemoryRegion, const Boolean& suppressPadding)
{
  logger_.LogDebug() << "IPC-CORE::DEPR ProcessBlocks()";
  ara::core::Future<ProcessBlockReadOnlyOutput>::PromiseType promise;
  ReadOnlyMemRegion const proBlocks{readOnlyMemoryRegion};
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const result{
      decPrUptr_->ProcessBlock(proBlocks, suppressPadding)};
  if (result.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::DEPR ProcessBlocks() success";
    ara::core::Vector<ara::core::Byte> tempVtr{result.Value()};
    ProcessBlockReadOnlyOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(), [&vtrByteResult](ara::core::Byte i) {
      vtrByteResult.vectorBytes.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::DEPR ProcessBlocks() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kInvalidInputSize)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
    }
    else if (result.Error() == SecurityErrorDomain::Errc::kInsufficientCapacity)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInsufficientCapacity);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> DecryptorPrivateContextSkeletonImpl::SetKey(
    const CryptoServiceInstanceId& privateKeyInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::DEPR SetKey()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::PrivateKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::PrivateKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::DEPR  Crypto Service Instance Id : " << privateKeyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::PrivateKeySkeletonImpl>& privateKeySkeleton{
      dataStorageContainer->GetObjectFromContanier(privateKeyInstanceId)};
  auto& privateKeyUptr{privateKeySkeleton->GetPrivateKeyUptrInstance()};
  ara::core::Result<void> const tempResult{decPrUptr_->SetKey(*privateKeyUptr)};
  if (tempResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::DEPR SetKey() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE:DEPR SetKey() Error: " << tempResult.Error().Message();
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

ara::core::Future<void> DecryptorPrivateContextSkeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::DRPR Reset()";
  decPrUptr_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::IsInitializedOutput>
    DecryptorPrivateContextSkeletonImpl::IsInitialized()

{
  logger_.LogDebug() << "IPC-CORE::DRPR IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = decPrUptr_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::GetCryptoPrimitiveIdOutput>
    DecryptorPrivateContextSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::DRPR GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{decPrUptr_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}
ara::core::Future<
    ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::GetCryptoServiceOutput>
    DecryptorPrivateContextSkeletonImpl::GetCryptoService()
{
  logger_.LogDebug() << "In DecryptorPrivateContextSkeletonImpl::GetCryptoService()";
  ara::core::Future<ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::
          GetCryptoServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::GetCryptoServiceOutput
      localCryptoServiceOutput;
  CryptoService::Uptr cryptoUptr{decPrUptr_->GetCryptoService()};
  if (cryptoUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:DecryptorPrivateContextSkeletonImpl::GetCryptoService() successful: ";
    std::shared_ptr<ara::crypto::common::datastorage::CryptoServiceContainer> cryptoServiceContainer{
        ara::crypto::common::datastorage::CryptoServiceContainer::GetInstance()};
    CryptoServiceInstanceId cryptolocalInstanceId{cryptoServiceContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryservice::CryptoServiceSkeletonImpl> localCryptoService{
        std::make_unique<ara::crypto::cryp::cryservice::CryptoServiceSkeletonImpl>(
            cryptolocalInstanceId, std::move(cryptoUptr))};
    localCryptoService->OfferService();
    cryptoServiceContainer->StoreObjectToContanier(cryptolocalInstanceId, std::move(localCryptoService));
    localCryptoServiceOutput.cryptoInstanceId = cryptolocalInstanceId;
    promise.set_value(localCryptoServiceOutput);
  }
  else
  {
    logger_.LogError()
        << "IPC-CORE:DecryptorPrivateContextSkeletonImpl::GetCryptoService() Uptr is nullptr: ";
    promise.set_value(localCryptoServiceOutput);
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
