// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : secret_seed_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_SECRET_SEED_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_SECRET_SEED_CONTAINER_H_

#include "secret_seed_skeleton_impl.h"
#include "skeleton_data_storage_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for common.
namespace common
{
/// @brief namespace for datastorage
namespace datastorage
{
/// @brief SecretSeedContainer class
class SecretSeedContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>
{
 public:
  /// @brief SecretSeedContainer should not be cloneable.
  SecretSeedContainer(SecretSeedContainer& other) = delete;

  /// @brief SecretSeedContainer should not be assignable.
  void operator=(const SecretSeedContainer&) = delete;

  /// @brief GetInstance for SecretSeedContainer.
  static std::shared_ptr<SecretSeedContainer> GetInstance()
  {
    if (secretSeedPtr_ == nullptr)
    {
      secretSeedPtr_ = std::make_shared<SecretSeedContainer>();
    }
    return secretSeedPtr_;
  }

  /// @brief SecretSeedContainer Constructor.
  SecretSeedContainer() = default;

 private:
  /// @brief shared pointer of SecretSeedContainer.
  static std::shared_ptr<SecretSeedContainer> secretSeedPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_SECRET_SEED_CONTAINER_H_