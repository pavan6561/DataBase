// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : extension_service_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_EXTENSION_SERVICE_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_EXTENSION_SERVICE_CONTAINER_H_

#include "extensionservice_skeleton_impl.h"
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
/// @brief ExtensionServiceContainer class
class ExtensionServiceContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryservice::ExtensionServiceSkeletonImpl>
{
 public:
  /// @brief ExtensionServiceContainer should not be cloneable.
  ExtensionServiceContainer(ExtensionServiceContainer& other) = delete;

  /// @brief ExtensionServiceContainer should not be assignable.
  void operator=(const ExtensionServiceContainer&) = delete;

  /// @brief GetInstance for ExtensionServiceContainer.
  static std::shared_ptr<ExtensionServiceContainer> GetInstance()
  {
    if (extensionServiceContainerPtr_ == nullptr)
    {
      extensionServiceContainerPtr_ = std::make_shared<ExtensionServiceContainer>();
    }
    return extensionServiceContainerPtr_;
  }

  /// @brief ExtensionServiceContainer Constructor.
  ExtensionServiceContainer() = default;

 private:
  /// @brief shared pointer of ExtensionServiceContainer.
  static std::shared_ptr<ExtensionServiceContainer> extensionServiceContainerPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_EXTENSION_SERVICE_CONTAINER_H_
