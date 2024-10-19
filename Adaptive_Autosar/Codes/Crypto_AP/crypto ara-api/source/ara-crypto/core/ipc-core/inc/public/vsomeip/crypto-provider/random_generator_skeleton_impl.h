// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : random_generator_skeleton_impl.h
//
// Purpose     : Random number generator skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_RANDOM_GENERATOR_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_RANDOM_GENERATOR_SKELETON_IMPL_H_

#include <algorithm>
#include <ara/core/span.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/security_error_domain.h"
#include "ara/crypto/cryp/random_generator_ctx.h"
#include "error_domain_cryptoerrordomain.h"
#include "impl_type_cryptoalgid.h"
#include "randomgenerator_skeleton.h"
#include "secret_seed_container.h"
#include "symmetrickey_container.h"
#include "extension_service_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Class definition for Random number generator skeleton.
class RandomGeneratorSkeletonImpl final
  : public ara::crypto::ipccom::cryp::randomgenerator::skeleton::RandomGeneratorSkeleton
{
 public:
  using Skeleton = ara::crypto::ipccom::cryp::randomgenerator::skeleton::RandomGeneratorSkeleton;

  /// @brief Initializes a new instance of this service with the given instance id.
  RandomGeneratorSkeletonImpl(
    CryptoServiceInstanceId const instanceId, RandomGeneratorCtx::Uptr rngUptr) noexcept;

  /// @brief Skeleton implementation for AddEntropy.
  ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::AddEntropyOutput> AddEntropy(
    const CryptoReadOnlyMemRegion& entropy) override;

  /// @brief Skeleton implementation for Generate.
  ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GenerateOutput> Generate(
    const uint32& count) override;

  /// @brief Skeleton implementation for GetExtensionServiceOutput.
  ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GetExtensionServiceOutput>
    GetExtensionService() override;

  /// @brief Skeleton implementation for SeedReadOnlyMem.
  ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::SeedReadOnlyMemOutput>
    SeedReadOnlyMem(const CryptoReadOnlyMemRegion& seed) override;

  /// @brief Skeleton implementation for SeedSecretSeed.
  ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::SeedSecretSeedOutput>
    SeedSecretSeed(const CryptoServiceInstanceId& secretSeedInstance) override;

  /// @brief Skeleton implementation for SetKey.
  ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::SetKeyOutput> SetKey(
    const CryptoServiceInstanceId& symmetricKeyInstanceId) override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::cryp::randomgenerator::RandomGenerator::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

 private:
  /// @brief Unique pointer for rng ctx.
  RandomGeneratorCtx::Uptr rngctx_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_RANDOM_GENERATOR_SKELETON_IMPL_H_
