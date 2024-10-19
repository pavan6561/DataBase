// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : skeleton_data_storage_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_COMMON_SKELETON_DATA_STORAGE_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_COMMON_SKELETON_DATA_STORAGE_CONTAINER_H_

#include <ara/core/map.h>
#include <memory>
#include <mutex>

#include "impl_type_cryptoserviceinstanceid.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for common.
namespace common
{
/// @brief Skeleton implementation for Data storage container.
template <class contanierType>
class SkeletonDataStorageContanier
{
 public:
  /// @brief Constructor
  SkeletonDataStorageContanier() = default;

  /// @brief Destructor
  ~SkeletonDataStorageContanier() = default;

  /// @brief Operator overloading for assignment operator.
  void operator=(const SkeletonDataStorageContanier&) = delete;

  /// @brief Getter method to get the instance Id of crypto service.
  CryptoServiceInstanceId GetCryptoServiceInstanceId()
  {
    // Generate new instance Id for crypto service.
    std::lock_guard<std::mutex> const lock{ mutex_instanceId_ };
    instanceId_ = instanceId_ + 1;
    CryptoServiceInstanceId strInstance{ "SOME/IP:" };
    CryptoServiceInstanceId const tempStr{ std::to_string(static_cast<uint64_t>(instanceId_)) };
    strInstance.append(tempStr);
    return strInstance;
  }

  /// @brief StoreObjectToContanier() method for store the object into container.
  void StoreObjectToContanier(
    CryptoServiceInstanceId const& serviceInstanceId, std::unique_ptr<contanierType> uptrObject)
  {
    storageContainer_.emplace(serviceInstanceId, std::move(uptrObject));
  }

  /// @brief GetObjectFromContanier() method for insert the object from container.
  std::unique_ptr<contanierType>& GetObjectFromContanier(CryptoServiceInstanceId const& serviceInstanceId)
  {
    auto obj{ storageContainer_.find(serviceInstanceId) };
    return obj->second;
  }

  /// @brief DeleteInstanceFromContainer() method for delete the object from container
  bool DeleteInstanceFromContainer(CryptoServiceInstanceId const& serviceInstanceId)
  {
    bool isSuccess{ false };
    if (storageContainer_.erase(serviceInstanceId))
    {
      isSuccess = true;
    }
    return isSuccess;
  }

 private:
  /// @brief Map for instance Id and container data type
  ara::core::Map<CryptoServiceInstanceId, std::unique_ptr<contanierType>> storageContainer_;

  /// @brief mutex for instance Id.
  std::mutex mutex_instanceId_{};

  /// @brief Crypto service instance Id for crypto objects.
  std::uint16_t instanceId_{ 0 };
};

} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_COMMON_SKELETON_DATA_STORAGE_CONTAINER_H_
