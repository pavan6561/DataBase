// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : randomgenerator_proxy.h
//
// Purpose     : Implementation of file randomgenerator_proxy.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_RANDOMGENERATOR_RANDOMGENERATOR_PROXY_H_
#define CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_RANDOMGENERATOR_RANDOMGENERATOR_PROXY_H_

#include <ara/com/internal/proxy/ara_proxy_base.h>
#include "ara/crypto/ipccom/cryp/randomgenerator/randomgenerator_common.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace randomgenerator
{
namespace proxy
{

namespace methods
{
  /// @brief Using for AddEntropy.
  using AddEntropy = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::AddEntropyOutput(const CryptoReadOnlyMemRegion& )>;
  /// @brief Using for Generate.
  using Generate = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GenerateOutput(const uint32& )>;
  /// @brief Using for GetCryptoPrimitiveId.
  using GetCryptoPrimitiveId = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GetCryptoPrimitiveIdOutput()>;
  /// @brief Using for GetExtensionService.
  using GetExtensionService = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GetExtensionServiceOutput()>;
  /// @brief Using for IsInitialized.
  using IsInitialized = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::IsInitializedOutput()>;
  /// @brief Using for SeedReadOnlyMem.
  using SeedReadOnlyMem = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::SeedReadOnlyMemOutput(const CryptoReadOnlyMemRegion& )>;
  /// @brief Using for SeedSecretSeed.
  using SeedSecretSeed = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::SeedSecretSeedOutput(const CryptoServiceInstanceId& )>;
  /// @brief Using for SetKey.
  using SetKey = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::SetKeyOutput(const CryptoServiceInstanceId& )>;
} // namespace methods



/// @brief Class for RandomGeneratorProxyBase.
class RandomGeneratorProxyBase : public ara::com::internal::proxy::ProxyBindingBase, public ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator {
 public:
  /// @brief Method for AddEntropy.
  virtual methods::AddEntropy& GetAddEntropy() = 0;
  /// @brief Method for Generate.
  virtual methods::Generate& GetGenerate() = 0;
  /// @brief Method for GetCryptoPrimitiveId.
  virtual methods::GetCryptoPrimitiveId& GetGetCryptoPrimitiveId() = 0;
  /// @brief Method for GetExtensionService.
  virtual methods::GetExtensionService& GetGetExtensionService() = 0;
  /// @brief Method for IsInitialized.
  virtual methods::IsInitialized& GetIsInitialized() = 0;
  /// @brief Method for SeedReadOnlyMem.
  virtual methods::SeedReadOnlyMem& GetSeedReadOnlyMem() = 0;
  /// @brief Method for SeedSecretSeed.
  virtual methods::SeedSecretSeed& GetSeedSecretSeed() = 0;
  /// @brief Method for SetKey.
  virtual methods::SetKey& GetSetKey() = 0;
};

/// @brief Class for RandomGeneratorProxy.
class RandomGeneratorProxy : public ara::com::internal::proxy::ProxyBase<RandomGeneratorProxyBase>, public ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator {
 public:
  /// @brief Definition for RandomGeneratorProxy.
  explicit RandomGeneratorProxy(const HandleType& proxy_base_factory) :
    ara::com::internal::proxy::ProxyBase<RandomGeneratorProxyBase>(proxy_base_factory),
    /// @brief Function GetAddEntropy.
    AddEntropy(proxy_base_->GetAddEntropy()),
    /// @brief Function GetGenerate.
    Generate(proxy_base_->GetGenerate()),
    /// @brief Function GetGetCryptoPrimitiveId.
    GetCryptoPrimitiveId(proxy_base_->GetGetCryptoPrimitiveId()),
    /// @brief Function GetGetExtensionService.
    GetExtensionService(proxy_base_->GetGetExtensionService()),
    /// @brief Function GetIsInitialized.
    IsInitialized(proxy_base_->GetIsInitialized()),
    /// @brief Function GetSeedReadOnlyMem.
    SeedReadOnlyMem(proxy_base_->GetSeedReadOnlyMem()),
    /// @brief Function GetSeedSecretSeed.
    SeedSecretSeed(proxy_base_->GetSeedSecretSeed()),
    /// @brief Function GetSetKey.
    SetKey(proxy_base_->GetSetKey())
  {}

  /// @brief Proxy shall be move constructable.
  explicit RandomGeneratorProxy(RandomGeneratorProxy&&) = default;

  /// @brief Proxy shall be move assignable.
  RandomGeneratorProxy& operator=(RandomGeneratorProxy&&) = default;

  /// @brief Proxy shall not be copy constructable.
  explicit RandomGeneratorProxy(const RandomGeneratorProxy&) = delete;

  /// @brief Proxy shall not be copy assignable.
  RandomGeneratorProxy& operator=(const RandomGeneratorProxy&) = delete;

  /// @brief Method for AddEntropy.
  methods::AddEntropy& AddEntropy;
  /// @brief Method for Generate.
  methods::Generate& Generate;
  /// @brief Method for GetCryptoPrimitiveId.
  methods::GetCryptoPrimitiveId& GetCryptoPrimitiveId;
  /// @brief Method for GetExtensionService.
  methods::GetExtensionService& GetExtensionService;
  /// @brief Method for IsInitialized.
  methods::IsInitialized& IsInitialized;
  /// @brief Method for SeedReadOnlyMem.
  methods::SeedReadOnlyMem& SeedReadOnlyMem;
  /// @brief Method for SeedSecretSeed.
  methods::SeedSecretSeed& SeedSecretSeed;
  /// @brief Method for SetKey.
  methods::SetKey& SetKey;
};
} // namespace proxy

} // namespace randomgenerator
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_RANDOMGENERATOR_RANDOMGENERATOR_PROXY_H_

