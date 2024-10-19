// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : hash_function_skeleton_impl.h
//
// Purpose     : Hash function skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_HASH_FUNCTION_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_HASH_FUNCTION_SKELETON_IMPL_H_

#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/hash_function_ctx.h"
#include "hashfunction_skeleton.h"
#include "secret_seed_container.h"
#include "symmetrickey_container.h"
#include "digest_service_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Class definition for Hash function skeleton.
class HashFunctionSkeletonImpl
  : public ara::crypto::ipccom::cryp::hashfunction::skeleton::HashFunctionSkeleton
{
 public:
  /// @brief Constructor for HashFunctionSkeletonImpl class.
  /// @param[in] instance Id of the hash function context P port.
  /// @param[in] unique pointer to the hash function ctx instance.
  HashFunctionSkeletonImpl(CryptoServiceInstanceId const instanceId, HashFunctionCtx::Uptr hashUptr) noexcept;

  /// @brief Skeleton implementation for Finish method.
  ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::FinishOutput> Finish() override;

  /// @brief Skeleton implementation for GetDigest method.
  /// @param[in] offset location from which digest needs to be read.
  ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::GetDigestOutput> GetDigest(
    const uint64& offset) override;

  /// @brief Skeleton implementation for GetDigestService method.
  ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::GetDigestServiceOutput>
    GetDigestService() override;

  /// @brief Skeleton implementation for Start method with no arguments.
  ara::core::Future<void> StartNoParam() override;

  /// @brief Skeleton implementation for Start method with read only memory region.
  /// @param[in] Initialiation vector for the Hash function ctx.
  ara::core::Future<void> StartReadOnlyMem(const CryptoReadOnlyMemRegion& iv) override;

  /// @brief Skeleton implementation for Start method with read only memory region.
  ara::core::Future<void> StartSecretSeed(const CryptoServiceInstanceId& secretSeedInstanceId) override;

  /// @brief Skeleton implementation for Update method with uint8_t.
  ara::core::Future<void> Update(const uint8& in) override;

  /// @brief Skeleton implementation for Update method with ReadOnlyMemRegion.
  ara::core::Future<void> UpdateReadOnlyMem(const CryptoReadOnlyMemRegion& in) override;

  /// @brief Skeleton implementation for Update method with RestrictedUsedObject.
  ara::core::Future<void> UpdateRestrictedUseObject(
    const RestrictedUseObjectStructure& restrictedUseObjStruct) override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::cryp::hashfunction::HashFunction::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

  /// @brief Definition for GetHashFunctionUptr.
  HashFunctionCtx::Uptr& GetHashFunctionUptr() noexcept
  {
    return hashCtx_;
  }

 private:
  /// @brief unique pointer to hash function ctx.
  HashFunctionCtx::Uptr hashCtx_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_HASH_FUNCTION_SKELETON_IMPL_H_
