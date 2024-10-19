// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : random_generator_skeleton_impl.cpp
//
// Purpose     : Random Generator skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "random_generator_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
using ara::crypto::ipccom::cryp::randomgenerator::skeleton::RandomGeneratorSkeleton;

RandomGeneratorSkeletonImpl::RandomGeneratorSkeletonImpl(
    CryptoServiceInstanceId const instanceId, RandomGeneratorCtx::Uptr rngUptr) noexcept :
    ara::crypto::ipccom::cryp::randomgenerator::skeleton::RandomGeneratorSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    rngctx_{std::move(rngUptr)}, logger_{ara::log::CreateLogger(
                                     "RNGS", "Random Generator Context", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "IPC-CORE::RNG RandomGeneratorImpl Constructor";
}

ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::IsInitializedOutput>
    RandomGeneratorSkeletonImpl::IsInitialized()
{
  logger_.LogDebug() << "IPC-CORE::RNG IsInitialized()";
  ara::core::Future<IsInitializedOutput>::PromiseType promise;
  IsInitializedOutput initializeOutput;
  initializeOutput.result = rngctx_->IsInitialized();
  promise.set_value(initializeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GetCryptoPrimitiveIdOutput>
    RandomGeneratorSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::RNG GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{rngctx_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::AddEntropyOutput>
    RandomGeneratorSkeletonImpl::AddEntropy(const CryptoReadOnlyMemRegion& entropy)
{
  logger_.LogDebug() << "IPC-CORE::RNG AddEntropy()";
  ReadOnlyMemRegion spanEntropy{entropy};
  bool addEntrophyResult = rngctx_->AddEntropy(spanEntropy);
  AddEntropyOutput tempResult;
  tempResult.result = addEntrophyResult;
  ara::core::Future<AddEntropyOutput>::PromiseType promise;
  promise.set_value(tempResult);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GenerateOutput>
    RandomGeneratorSkeletonImpl::Generate(const uint32& count)
{
  logger_.LogDebug() << "IPC-CORE::RNG Generate()";
  ara::core::Future<GenerateOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> result = rngctx_->Generate(count);
  if (result.HasValue())
  {
    ara::core::Vector<ara::core::Byte> tempVtr = result.Value();
    GenerateOutput vtrByteResult;
    for_each(tempVtr.begin(), tempVtr.end(),
        [&](ara::core::Byte i) { vtrByteResult.genResult.emplace_back(static_cast<uint8_t>(i)); });
    promise.set_value(vtrByteResult);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::RNG Generate() Error: " << result.Error().Message();
    if (result.Error() == SecurityErrorDomain::Errc::kBusyResource)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kBusyResource);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GetExtensionServiceOutput>
    RandomGeneratorSkeletonImpl::GetExtensionService()
{
  logger_.LogDebug() << "In RandomGeneratorSkeletonImpl::GetExtensionService()";
  ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GetExtensionServiceOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GetExtensionServiceOutput
      localExtensionServiceOutput;
  ExtensionService::Uptr extensionUptr{rngctx_->GetExtensionService()};
  if (extensionUptr != nullptr)
  {
    logger_.LogError() << "IPC-CORE:RandomGeneratorSkeletonImpl::GetExtensionService() successful: ";
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
    localExtensionServiceOutput.instanceID = extensionlocalInstanceId;
    promise.set_value(localExtensionServiceOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:RandomGeneratorSkeletonImpl::GetExtensionService() Uptr is nullptr: ";
    promise.set_value(localExtensionServiceOutput);
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::SeedReadOnlyMemOutput>
    RandomGeneratorSkeletonImpl::SeedReadOnlyMem(const CryptoReadOnlyMemRegion& seed)
{
  // Perform the seed operation with ReadOnlyMemRegion and return it's output to application.
  logger_.LogDebug() << "IPC-CORE::RNG SeedReadOnlyMem()";
  ReadOnlyMemRegion spanSeed{seed};
  bool tempResult = rngctx_->Seed(spanSeed);
  ara::core::Future<SeedReadOnlyMemOutput>::PromiseType promise;
  SeedReadOnlyMemOutput resSeed;
  resSeed.result = tempResult;
  promise.set_value(resSeed);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::SeedSecretSeedOutput>
    RandomGeneratorSkeletonImpl::SeedSecretSeed(const CryptoServiceInstanceId& secretSeedInstance)
{
  logger_.LogDebug() << "IPC-CORE::RNG SeedSecretSeed()";
  ara::core::Future<SeedSecretSeedOutput>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::RNG Crypto Service Instance Id : " << secretSeedInstance;
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj
      = dataStorageContainer->GetObjectFromContanier(secretSeedInstance);
  SeedSecretSeedOutput secretSeedOutput;
  if (secretSeedSkeletonObj->IsConstUptrSet() == true)
  {
    logger_.LogDebug() << "IPC-CORE::RNG SeedSecretSeed() -> GetSecretSeedConstUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
    secretSeedOutput.result = rngctx_->Seed(*secretSeedUptr);
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE::RNG SeedSecretSeed() -> GetSecretSeedUptrInstance()";
    auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
    secretSeedOutput.result = rngctx_->Seed(*secretSeedUptr);
  }
  promise.set_value(secretSeedOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::SetKeyOutput>
    RandomGeneratorSkeletonImpl::SetKey(const CryptoServiceInstanceId& symmetricKeyInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::RNG SetKey()";
  ara::core::Future<SetKeyOutput>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
  logger_.LogInfo() << "IPC-CORE::RNG Crypto Service Instance Id : " << symmetricKeyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmtericKeySkeleton
      = dataStorageContainer->GetObjectFromContanier(symmetricKeyInstanceId);
  auto& symmtericKeyUptr = symmtericKeySkeleton->GetSymmetricKeyUptrInstance();
  bool tempResult = rngctx_->SetKey(*symmtericKeyUptr);
  SetKeyOutput setKeyResult;
  setKeyResult.result = tempResult;
  promise.set_value(setKeyResult);
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
