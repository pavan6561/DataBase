// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : secret_seed_skeleton_impl.h
//
// Purpose     : secret seed skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_SECRET_SEED_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_SECRET_SEED_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/promise.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "secretseed_skeleton.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for crypto objects.
namespace cryobj
{
/// @brief This class contains the methods for skeleton implementation for secret seed object.
class SecretSeedSkeletonImpl
  : public ara::crypto::ipccom::cryp::cryobj::secretseed::skeleton::SecretSeedSkeleton
{
 public:
  /// @brief Constructor for SecretSeedSkeletonImpl.
  SecretSeedSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::SecretSeed::Uptrc secretSeedConstUtr) noexcept;

  /// @brief Constructor for SecretSeedSkeletonImpl.
  SecretSeedSkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::SecretSeed::Uptr secretSeedUtr) noexcept;

  /// @brief Constructor for SecretSeedSkeletonImpl with restricted used object.
  SecretSeedSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::RestrictedUseObject::Uptrc restrictedUtr) noexcept;

  /// @brief Distructor for SecretSeedSkeletonImpl.
  ~SecretSeedSkeletonImpl() override = default;

  /// @brief Getter method for secret seed constant unique pointer.
  ara::crypto::cryp::SecretSeed::Uptrc& GetSecretSeedConstUptrInstance()
  {
    return secretSeedConstUptr_;
  }

  /// @brief Getter method for secret seed unique pointer.
  ara::crypto::cryp::SecretSeed::Uptr& GetSecretSeedUptrInstance()
  {
    return secretSeedUptr_;
  }

  /// @brief Definition for Clone.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::CloneOutput> Clone(
    const CryptoReadOnlyMemRegion& xorDelta) override;

  /// @brief Definition for GetAllowedUsage.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::GetAllowedUsageOutput>
    GetAllowedUsage() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

  /// @brief Definition for GetObjectId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::GetObjectIdOutput>
    GetObjectId() override;

  /// @brief Definition for GetPayloadSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::GetPayloadSizeOutput>
    GetPayloadSize() override;

  /// @brief Definition for HasDependence.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::HasDependenceOutput>
    HasDependence() override;

  /// @brief Definition for IsExportable.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::IsExportableOutput>
    IsExportable() override;

  /// @brief Definition for IsSession.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::secretseed::SecretSeed::IsSessionOutput>
    IsSession() override;

  /// @brief Definition for Jump.
  ara::core::Future<void> Jump(const Int64& Steps) override;

  /// @brief Definition for JumpFrom.
  ara::core::Future<void> JumpFrom(
    const CryptoServiceInstanceId& SecretSeedInstanceId, const Int64& steps) override;

  /// @brief Definition for Next.
  ara::core::Future<void> Next() override;

  /// @brief Definition for XorEqualReadOnlyMemRegion.
  ara::core::Future<void> XorEqualReadOnlyMemRegion(const CryptoReadOnlyMemRegion& source) override;

  /// @brief Definition for XorEqualSecretSeed.
  ara::core::Future<void> XorEqualSecretSeed(const CryptoServiceInstanceId& seedInstanceId) override;

  /// @brief Definition for Save.
  ara::core::Future<void> Save(const CryptoServiceInstanceId& IOInterfaceInstanceId) override;

  bool IsConstUptrSet() const noexcept
  {
    return isConstUptr_;
  }

 private:
  /// @brief Constant smart unique pointer for secret seed object.
  ara::crypto::cryp::SecretSeed::Uptrc secretSeedConstUptr_;

  /// @brief Smart unique pointer for secret seed object.
  ara::crypto::cryp::SecretSeed::Uptr secretSeedUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "SEED", "SECRET SEED Object", ara::log::LogLevel::kVerbose) };

  /// @brief Flag to check if constant secret seed object to be fetched.
  bool isConstUptr_;
};

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_SECRET_SEED_SKELETON_IMPL_H_
