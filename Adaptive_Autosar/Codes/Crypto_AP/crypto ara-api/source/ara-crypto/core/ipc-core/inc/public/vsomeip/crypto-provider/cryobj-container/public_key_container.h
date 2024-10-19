// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : public_key_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_PUBLIC_KEY_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_PUBLIC_KEY_CONTAINER_H_

#include "public_key_skeleton_impl.h"
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
/// @brief PublicKeyContainer class
class PublicKeyContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl>
{
 public:
  /// @brief PublicKeyContainer should not be cloneable.
  PublicKeyContainer(PublicKeyContainer& other) = delete;

  /// @brief PublicKeyContainer should not be assignable.
  void operator=(const PublicKeyContainer&) = delete;

  /// @brief GetInstance for PublicKeyContainer.
  static std::shared_ptr<PublicKeyContainer> GetInstance()
  {
    if (publicKeyPtr_ == nullptr)
    {
      publicKeyPtr_ = std::make_shared<PublicKeyContainer>();
    }
    return publicKeyPtr_;
  }

  /// @brief PublicKeyContainer Constructor.
  PublicKeyContainer() = default;

 private:
  /// @brief shared pointer of PublicKeyContainer.
  static std::shared_ptr<PublicKeyContainer> publicKeyPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_PUBLIC_KEY_CONTAINER_H_