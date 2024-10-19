// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : digest_service_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_DIGEST_SERVICE_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_DIGEST_SERVICE_CONTAINER_H_

#include "digestservice_skeleton_impl.h"
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
/// @brief DigestServiceContainer class
class DigestServiceContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryservice::DigestServiceSkeletonImpl>
{
 public:
  /// @brief DigestServiceContainer should not be cloneable.
  DigestServiceContainer(DigestServiceContainer& other) = delete;

  /// @brief DigestServiceContainer should not be assignable.
  void operator=(const DigestServiceContainer&) = delete;

  /// @brief GetInstance for DigestServiceContainer.
  static std::shared_ptr<DigestServiceContainer> GetInstance()
  {
    if (digestServiceContainerPtr_ == nullptr)
    {
      digestServiceContainerPtr_ = std::make_shared<DigestServiceContainer>();
    }
    return digestServiceContainerPtr_;
  }

  /// @brief DigestServiceContainer Constructor.
  DigestServiceContainer() = default;

 private:
  /// @brief shared pointer of DigestServiceContainer.
  static std::shared_ptr<DigestServiceContainer> digestServiceContainerPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_DIGEST_SERVICE_CONTAINER_H_
