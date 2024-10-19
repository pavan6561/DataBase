// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : random_generator_ctx_proxy_impl.cpp
//
// Purpose     : Random number generator proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "random_generator_ctx_proxy_impl.h"

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
RandomGeneratorCtxProxyImpl::RandomGeneratorCtxProxyImpl(CryptoServiceInstanceId const instanceValue,
    std::shared_ptr<ara::crypto::ipccom::cryp::cryptoprovider::proxy::CryptoProviderInterfaceProxy>&
        rngCPHandler,
    CryptoProvider& cp) noexcept :
    rngCryptoProviderHandler_{rngCPHandler},
    cpUptr_{cp}, logger_{ara::log::CreateLogger(
                     "MACP", "Random Generator Code Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "IPC-APP::RNG RandomGeneratorCtxProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "IPC-APP::RNG Server client connection for RandomGeneratorCtx -> success";
  }
  else
  {
    logger_.LogError() << "IPC-APP::RNG Server client connection for RandomGeneratorCtx -> fail";
  }
}

bool RandomGeneratorCtxProxyImpl::IsInitialized() const noexcept
{
  bool isInitialize{false};
  auto initResult = rngProxyHandler_->IsInitialized().GetResult();
  if (initResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::RNG -> IsInitialized() : Success";
    isInitialize = initResult.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::RNG -> IsInitialized() : Failed";
  }
  return isInitialize;
}

CryptoProvider& RandomGeneratorCtxProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::RNG -> MyProvider()";
  return cpUptr_;
}

ExtensionService::Uptr RandomGeneratorCtxProxyImpl::GetExtensionService() const noexcept
{
  logger_.LogDebug() << "IPC-APP::RNG -> GetExtensionService()";
  auto extensionService = rngProxyHandler_->GetExtensionService().GetResult();
  if (!extensionService.Value().instanceID.empty())
  {
    CryptoServiceInstanceId instanceId = extensionService.Value().instanceID;
    logger_.LogDebug() << "IPC-RNG Service InstanceId for GetExtensionService : " << instanceId;
    ExtensionService::Uptr extensionServiceUptr
        = std::make_unique<ara::crypto::cryp::cryservice::ExtensionServiceProxyImpl>(instanceId);
    return extensionServiceUptr;
  }
  else
  {
    logger_.LogDebug() << "IPC-RNG Service InstanceId for GetExtensionService is null";
    return nullptr;
  }
}

CryptoPrimitiveId::Uptr RandomGeneratorCtxProxyImpl::GetCryptoPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::RNG -> GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr cryptoPrimitiveIdUptrLocal{nullptr};
  auto primitiveResult = rngProxyHandler_->GetCryptoPrimitiveId().GetResult();
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
    logger_.LogError() << "IPC-APP::RNG -> GetCryptoPrimitiveId() Error: "
                       << primitiveResult.Error().Message();
    return cryptoPrimitiveIdUptrLocal;
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> RandomGeneratorCtxProxyImpl::Generate(
    std::uint32_t count) noexcept
{
  logger_.LogDebug() << "IPC-APP::RNG Generate()";
  auto generateResult = rngProxyHandler_->Generate(count).GetResult();
  if (generateResult.HasValue())
  {
    auto tempVtr = generateResult.Value().genResult;
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::RNG Generate() Error: " << generateResult.Error().Message();
    if (generateResult.Error() == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kBusyResource)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kBusyResource);
    }
    else if (generateResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext)
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }
    else
    {
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
}

bool RandomGeneratorCtxProxyImpl::Seed(ReadOnlyMemRegion seed) noexcept
{
  logger_.LogDebug() << "IPC-APP::RNG Method -> Seed(ReadOnlyMemRegion seed)";
  CryptoReadOnlyMemRegion vtrUint8t(seed.begin(), seed.end());
  auto seedResult = rngProxyHandler_->SeedReadOnlyMem(vtrUint8t).GetResult();
  return seedResult.Value().result;
}

bool RandomGeneratorCtxProxyImpl::Seed(const SecretSeed& seed) noexcept
{
  logger_.LogDebug() << "IPC-APP::RNG Method -> Seed(const SecretSeed& seed)";
  const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&seed);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  logger_.LogDebug() << "IPC-APP::RNG CryptoServiceInstanceId : " << serviceInstanceId;
  auto seedResult = rngProxyHandler_->SeedSecretSeed(serviceInstanceId).GetResult();
  return seedResult.Value().result;
}

bool RandomGeneratorCtxProxyImpl::SetKey(const SymmetricKey& key) noexcept
{
  logger_.LogDebug() << "IPC-APP::RNG Method -> SetKey(const SymmetricKey& key)";
  const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
      = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&key);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  logger_.LogDebug() << "IPC-APP::RNG SetKey() CryptoServiceInstanceId : " << serviceInstanceId;
  auto keyResult = rngProxyHandler_->SetKey(serviceInstanceId).GetResult();
  return keyResult.Value().result;
}

bool RandomGeneratorCtxProxyImpl::AddEntropy(ReadOnlyMemRegion entropy) noexcept
{
  logger_.LogDebug() << "IPC-APP::RNG AddEntropy()";
  CryptoReadOnlyMemRegion vtrUint8t(entropy.begin(), entropy.end());
  auto entropyResult = rngProxyHandler_->AddEntropy(vtrUint8t).GetResult();
  return entropyResult.Value().result;
}

bool RandomGeneratorCtxProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipRngInstance.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "IPC-APP::RNG Server Client connection for Random Generator : Successful";
    rngProxyHandler_ = vsomeipRngInstance.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "IPC-APP::RNG Server Client connection for Random Generator : Failed";
    isSuccess = false;
  }
  return isSuccess;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
