// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_service_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_CRYPTO_SERVICE_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_CRYPTO_SERVICE_CONTAINER_H_

#include "cryptoservice_skeleton_impl.h"
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
/// @brief CryptoServiceContainer class
class CryptoServiceContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryservice::CryptoServiceSkeletonImpl>
{
 public:
  /// @brief CryptoServiceContainer should not be cloneable.
  CryptoServiceContainer(CryptoServiceContainer& other) = delete;

  /// @brief CryptoServiceContainer should not be assignable.
  void operator=(const CryptoServiceContainer&) = delete;

  /// @brief GetInstance for CryptoServiceContainer.
  static std::shared_ptr<CryptoServiceContainer> GetInstance()
  {
    if (cryptoServiceContainerPtr_ == nullptr)
    {
      cryptoServiceContainerPtr_ = std::make_shared<CryptoServiceContainer>();
    }
    return cryptoServiceContainerPtr_;
  }

  /// @brief CryptoServiceContainer Constructor.
  CryptoServiceContainer() = default;

 private:
  /// @brief shared pointer of CryptoServiceContainer.
  static std::shared_ptr<CryptoServiceContainer> cryptoServiceContainerPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_CRYPTO_SERVICE_CONTAINER_H_
