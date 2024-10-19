// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_derivation_function_skeleton_impl.cpp
//
// Purpose     : Key derivation function skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "key_derivation_function_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
using ara::crypto::ipccom::cryp::keyderivationfunction::skeleton::KeyDerivationFunctionSkeleton;

KeyDerivationFunctionskeletonImpl::KeyDerivationFunctionskeletonImpl(
    CryptoServiceInstanceId const instanceId, KeyDerivationFunctionCtx::Uptr kdfUptr) noexcept :
    ara::crypto::ipccom::cryp::keyderivationfunction::skeleton::KeyDerivationFunctionSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    kdfCtx_{std::move(kdfUptr)}
{
  logger_.LogDebug() << "IPC-CORE::KDF KeyDerivationFunctionskeletonImpl Constructor";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::IsInitializedOutput>
    KeyDerivationFunctionskeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::KDF IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = kdfCtx_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetCryptoPrimitiveIdOutput>
    KeyDerivationFunctionskeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::KDF GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{kdfCtx_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<void> KeyDerivationFunctionskeletonImpl::AddSaltReadOnlyMem(
    const CryptoReadOnlyMemRegion& salt)
{
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion const spanSalt{salt};
  ara::core::Result<void> const resultAddSalt{kdfCtx_->AddSalt(spanSalt)};
  if (resultAddSalt.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::KDF KDF AddSaltReadOnlyMem() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::KDF KDF AddSaltReadOnlyMem() Error: " << resultAddSalt.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize);
  }
  return promise.get_future();
}

ara::core::Future<void> KeyDerivationFunctionskeletonImpl::AddSecretSalt(
    const CryptoServiceInstanceId& InstanceIdsecretSeed)
{
  logger_.LogDebug() << "IPC-CORE::KDF AddSecretSalt()";
  ara::core::Future<void>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::KDF Crypto Service Instance Id : " << InstanceIdsecretSeed;
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
      dataStorageContainer->GetObjectFromContanier(InstanceIdsecretSeed)};
  ara::core::Result<void> secretSeedResult;
  if (secretSeedSkeletonObj->IsConstUptrSet() == true)
  {
    logger_.LogDebug() << "IPC-CORE::KDF AddSecretSalt() -> GetSecretSeedConstUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
    secretSeedResult = kdfCtx_->AddSecretSalt(*secretSeedUptr);
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE::KDF AddSecretSalt() -> GetSecretSeedUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
    secretSeedResult = kdfCtx_->AddSecretSalt(*secretSeedUptr);
  }
  if (secretSeedResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::KDF StartSecretSeed() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::KDF StartSecretSeed() Error: " << secretSeedResult.Error().Message();
    promise.SetError(
        static_cast<ipccom::common::error::CryptoErrorDomainErrc>(secretSeedResult.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::ConfigIterationsOutput>
    KeyDerivationFunctionskeletonImpl::ConfigIterations(const uint32& iterations)
{
  logger_.LogDebug() << "IPC-CORE::KDF ConfigIterations()";
  ara::core::Future<ConfigIterationsOutput>::PromiseType promise;
  std::uint32_t const resultCt{kdfCtx_->ConfigIterations(iterations)};
  ConfigIterationsOutput ctOutput;
  ctOutput.result = resultCt;
  promise.set_value(ctOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveKeyOutput>
    KeyDerivationFunctionskeletonImpl::DeriveKey(const Boolean& isSession, const Boolean& isExportable)
{
  logger_.LogDebug() << "IPC-CORE::KDF DeriveKey()";
  ara::core::Future<DeriveKeyOutput>::PromiseType promise;
  ara::core::Result<SymmetricKey::Uptrc> sKeyUptrc{kdfCtx_->DeriveKey(isSession, isExportable)};
  if (sKeyUptrc.HasValue())
  {
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveKeyOutput keyOutput;
    // Get data container instance
    std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
    CryptoServiceInstanceId symmetricKeylocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl> localSymmetricUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>(
            symmetricKeylocalInstanceId, std::move(sKeyUptrc).Value())};
    localSymmetricUtr->OfferService();
    dataStorageContainer->StoreObjectToContanier(symmetricKeylocalInstanceId, std::move(localSymmetricUtr));
    keyOutput.instanceId = symmetricKeylocalInstanceId;
    promise.set_value(keyOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::KDF KDF DeriveKey() Error: " << sKeyUptrc.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveSeedOutput>
    KeyDerivationFunctionskeletonImpl::DeriveSeed(const Boolean& isSession, const Boolean& isExportable)
{
  logger_.LogDebug() << "In KeyDerivationFunctionskeletonImpl::DeriveSeed()";
  ara::core::Future<DeriveSeedOutput>::PromiseType promise;
  ara::core::Result<SecretSeed::Uptrc> seedUptrc{kdfCtx_->DeriveSeed(isSession, isExportable)};
  if (seedUptrc.HasValue())
  {
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveSeedOutput seedOutput;
    std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
    CryptoServiceInstanceId secretSeedlocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl> localSecretSeedUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>(
            secretSeedlocalInstanceId, std::move(seedUptrc).Value())};
    localSecretSeedUtr->OfferService();
    dataStorageContainer->StoreObjectToContanier(secretSeedlocalInstanceId, std::move(localSecretSeedUtr));
    seedOutput.instanceId = secretSeedlocalInstanceId;
    promise.set_value(seedOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::KDF KDF DeriveKey() Error: " << seedUptrc.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetExtensionServiceOutput>
    KeyDerivationFunctionskeletonImpl::GetExtensionService()
{
  logger_.LogDebug() << "In KeyDerivationFunctionskeletonImpl::GetExtensionService()";
  ara::core::Future<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::
          GetExtensionServiceOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetExtensionServiceOutput
      localExtensionServiceOutput;
  ExtensionService::Uptr extensionUptr{kdfCtx_->GetExtensionService()};
  if (extensionUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:KeyDerivationFunctionskeletonImpl::GetExtensionService() successful: ";
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
    localExtensionServiceOutput.instanceId = extensionlocalInstanceId;
    promise.set_value(localExtensionServiceOutput);
  }
  else
  {
    logger_.LogError()
        << "IPC-CORE:KeyDerivationFunctionskeletonImpl::GetExtensionService() Uptr is nullptr: ";
    promise.set_value(localExtensionServiceOutput);
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetKeyIdSizeOutput>
    KeyDerivationFunctionskeletonImpl::GetKeyIdSize()
{
  logger_.LogDebug() << "IPC-CORE::KDF GetKeyIdSize()";
  ara::core::Future<GetKeyIdSizeOutput>::PromiseType promise;
  GetKeyIdSizeOutput keyIdSize;
  keyIdSize.result = kdfCtx_->GetKeyIdSize();
  promise.set_value(keyIdSize);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetAlgIdOutput>
    KeyDerivationFunctionskeletonImpl::GetTargetAlgId()
{
  ara::core::Future<GetTargetAlgIdOutput>::PromiseType promise;
  GetTargetAlgIdOutput tempOutput;
  tempOutput.algId = kdfCtx_->GetTargetAlgId();
  promise.set_value(tempOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetAllowedUsageOutput>
    KeyDerivationFunctionskeletonImpl::GetTargetAllowedUsage()
{
  ara::core::Future<GetTargetAllowedUsageOutput>::PromiseType promise;
  GetTargetAllowedUsageOutput tempOutput;
  tempOutput.allowedUsageFlags = kdfCtx_->GetTargetAllowedUsage();
  promise.set_value(tempOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetKeyBitLengthOutput>
    KeyDerivationFunctionskeletonImpl::GetTargetKeyBitLength()
{
  ara::core::Future<GetTargetKeyBitLengthOutput>::PromiseType promise;
  GetTargetKeyBitLengthOutput tempOutput;
  tempOutput.result = kdfCtx_->GetTargetKeyBitLength();
  promise.set_value(tempOutput);
  return promise.get_future();
}

ara::core::Future<void> KeyDerivationFunctionskeletonImpl::Init(const CryptoReadOnlyMemRegion& targetKeyId,
    const CryptoAlgId& targetAlgId, const CryptoAllowedUsageFlags& allowedUsage,
    const CryptoReadOnlyMemRegion& ctxLabel)
{
  ara::core::Future<void>::PromiseType promise;
  ReadOnlyMemRegion const keyId{targetKeyId};
  ReadOnlyMemRegion const cLabel{ctxLabel};
  ara::core::Result<void> const resultInit{
      kdfCtx_->Init(keyId, targetAlgId, static_cast<AllowedUsageFlags>(allowedUsage), cLabel)};
  if (resultInit.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::KDF Init() success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::KDF Init() Error: " << resultInit.Error().Message();
    if (resultInit.Error() == SecurityErrorDomain::Errc::kIncompatibleArguments)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation);
    }
  }
  return promise.get_future();
}

ara::core::Future<void> KeyDerivationFunctionskeletonImpl::Reset()
{
  logger_.LogDebug() << "IPC-CORE::KDF Reset()";
  kdfCtx_->Reset();
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

ara::core::Future<void> KeyDerivationFunctionskeletonImpl::SetSourceKeyMaterial(
    const RestrictedUseObjectStructure& restrictedUseObjStruct)
{
  logger_.LogDebug() << "IPC-CORE::KDF SetSourceKeyMaterial()";
  ara::core::Future<void>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::KDF Crypto Service Instance Id : "
                    << restrictedUseObjStruct.RestrictedUseInstanceId;
  ara::core::Result<void> setSourceResult;
  CryptoObjectTypeIPC objType{restrictedUseObjStruct.RestrictedUseObjectType};
  if (objType == CryptoObjectTypeIPC::SecretSeed)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedUseObjStruct.RestrictedUseInstanceId)};
    if (secretSeedSkeletonObj->IsConstUptrSet() == true)
    {
      logger_.LogDebug() << "IPC-CORE::KDF SetSourceKeyMaterial() -> GetSecretSeedConstUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
      setSourceResult = kdfCtx_->SetSourceKeyMaterial(*secretSeedUptr);
    }
    else
    {
      logger_.LogDebug() << "IPC-CORE::KDF SetSourceKeyMaterial() -> GetSecretSeedUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
      setSourceResult = kdfCtx_->SetSourceKeyMaterial(*secretSeedUptr);
    }
  }
  else if (objType == CryptoObjectTypeIPC::SymmetricKey)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmetricKeySkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedUseObjStruct.RestrictedUseInstanceId)};
    auto& symmetricKeyUptr{symmetricKeySkeletonObj->GetSymmetricKeyUptrInstance()};
    setSourceResult = kdfCtx_->SetSourceKeyMaterial(*symmetricKeyUptr);
  }
  else if (objType == CryptoObjectTypeIPC::PublicKey)
  {
    // ToDo: Public key and Private key are not implemented for now as
    // underlying crypto daemon doesn't support.
  }
  else if (objType == CryptoObjectTypeIPC::PrivateKey)
  {
    // ToDo: Public key and Private key are not implemented for now as
    // underlying crypto daemon doesn't support.
  }
  if (setSourceResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::KDF SetSourceKeyMaterial() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::ACC SetSourceKeyMaterial() Error: " << setSourceResult.Error().Message();
    if (setSourceResult.Error() == SecurityErrorDomain::Errc::kIncompatibleObject)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject);
    }
    else if (setSourceResult.Error() == SecurityErrorDomain::Errc::kUsageViolation)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation);
    }
    else
    {
      // kBruteForceRisk error type is not configured, so using other for temporary
      // promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kBruteForceRisk);
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation);
    }
  }
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
