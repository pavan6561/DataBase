// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : block_service_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_BLOCK_SERVICE_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_BLOCK_SERVICE_CONTAINER_H_

#include "blockservice_skeleton_impl.h"
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
/// @brief BlockServiceContainer class
class BlockServiceContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryservice::BlockServiceSkeletonImpl>
{
 public:
  /// @brief BlockServiceContainer should not be cloneable.
  BlockServiceContainer(BlockServiceContainer& other) = delete;

  /// @brief BlockServiceContainer should not be assignable.
  void operator=(const BlockServiceContainer&) = delete;

  /// @brief GetInstance for BlockServiceContainer.
  static std::shared_ptr<BlockServiceContainer> GetInstance()
  {
    if (blockServiceContainerPtr_ == nullptr)
    {
      blockServiceContainerPtr_ = std::make_shared<BlockServiceContainer>();
    }
    return blockServiceContainerPtr_;
  }

  /// @brief BlockServiceContainer Constructor.
  BlockServiceContainer() = default;

 private:
  /// @brief shared pointer of BlockServiceContainer.
  static std::shared_ptr<BlockServiceContainer> blockServiceContainerPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_BLOCK_SERVICE_CONTAINER_H_
