// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetickey_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_SYMMETRIC_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_SYMMETRIC_CONTAINER_H_

#include "skeleton_data_storage_container.h"
#include "symmetric_key_skeleton_impl.h"

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
/// @brief SymmetricKeyContainer class
class SymmetricKeyContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>
{
 public:
  /// @brief SymmetricKeyContainer should not be cloneable.
  SymmetricKeyContainer(SymmetricKeyContainer& other) = delete;

  /// @brief SymmetricKeyContainer should not be assignable.
  void operator=(const SymmetricKeyContainer&) = delete;

  /// @brief GetInstance for SymmetricKeyContainer.
  static std::shared_ptr<SymmetricKeyContainer> GetInstance()
  {
    if (symmetricKeyPtr_ == nullptr)
    {
      symmetricKeyPtr_ = std::make_shared<SymmetricKeyContainer>();
    }
    return symmetricKeyPtr_;
  }

  /// @brief SymmetricKeyContainer Constructor.
  SymmetricKeyContainer() = default;

 private:
  /// @brief shared pointer of SymmetricKeyContainer.
  static std::shared_ptr<SymmetricKeyContainer> symmetricKeyPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_SYMMETRIC_CONTAINER_H_