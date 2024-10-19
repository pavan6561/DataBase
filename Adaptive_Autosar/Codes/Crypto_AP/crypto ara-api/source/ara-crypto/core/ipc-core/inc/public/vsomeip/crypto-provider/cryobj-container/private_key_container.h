// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : private_key_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_PRIVATE_KEY_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_PRIVATE_KEY_CONTAINER_H_

#include "private_key_skeleton_impl.h"
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
/// @brief PrivateKeyContainer class
class PrivateKeyContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryobj::PrivateKeySkeletonImpl>
{
 public:
  /// @brief PrivateKeyContainer should not be cloneable.
  PrivateKeyContainer(PrivateKeyContainer& other) = delete;

  /// @brief PrivateKeyContainer should not be assignable.
  void operator=(const PrivateKeyContainer&) = delete;

  /// @brief GetInstance for PrivateKeyContainer.
  static std::shared_ptr<PrivateKeyContainer> GetInstance()
  {
    if (privateKeyPtr_ == nullptr)
    {
      privateKeyPtr_ = std::make_shared<PrivateKeyContainer>();
    }
    return privateKeyPtr_;
  }

  /// @brief PrivateKeyContainer Constructor.
  PrivateKeyContainer() = default;

 private:
  /// @brief shared pointer of PrivateKeyContainer.
  static std::shared_ptr<PrivateKeyContainer> privateKeyPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_PRIVATE_KEY_CONTAINER_H_