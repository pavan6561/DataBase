// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : restricted_used_object_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_RESTRICTED_USED_OBJECT_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_RESTRICTED_USED_OBJECT_CONTAINER_H_

#include "restricted_use_object_skeleton_impl.h"
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
/// @brief RestrictedUsedObjectContainer class
class RestrictedUsedObjectContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryobj::RestrictedUseObjectSkeletonImpl>
{
 public:
  /// @briefRestrictedUsedObjectContainer should not be cloneable.
  RestrictedUsedObjectContainer(RestrictedUsedObjectContainer& other) = delete;

  /// @brief RestrictedUsedObjectContainer should not be assignable.
  void operator=(const RestrictedUsedObjectContainer&) = delete;

  /// @brief GetInstance for RestrictedUsedObjectContainer.
  static std::shared_ptr<RestrictedUsedObjectContainer> GetInstance()
  {
    if (restrictedUseObjPtr_ == nullptr)
    {
      restrictedUseObjPtr_ = std::make_shared<RestrictedUsedObjectContainer>();
    }
    return restrictedUseObjPtr_;
  }

  /// @brief RestrictedUsedObjectContainer Constructor.
  RestrictedUsedObjectContainer() = default;

 private:
  /// @brief shared pointer of RestrictedUsedObjectContainer.
  static std::shared_ptr<RestrictedUsedObjectContainer> restrictedUseObjPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_RESTRICTED_USED_OBJECT_CONTAINER_H_