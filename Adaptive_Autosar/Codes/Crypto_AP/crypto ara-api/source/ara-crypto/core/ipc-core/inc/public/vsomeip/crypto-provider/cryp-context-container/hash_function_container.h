// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : hash_function_container.h
//
// Purpose     : Hash function container implementation to store the hash function context objects.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYP_CONTEXT_CONTAINER_HASH_FUNCTION_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYP_CONTEXT_CONTAINER_HASH_FUNCTION_CONTAINER_H_

#include "hash_function_skeleton_impl.h"
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
/// @brief namespace for crypdatastore
namespace crypdatastore
{
/// @brief HashFunctionContainer class
class HashFunctionContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::HashFunctionSkeletonImpl>
{
 public:
  /// @brief HashFunctionContainer should not be cloneable.
  HashFunctionContainer(HashFunctionContainer& other) = delete;

  /// @brief HashFunctionContainer should not be assignable.
  void operator=(const HashFunctionContainer&) = delete;

  /// @brief GetInstance for HashFunctionContainer.
  static std::shared_ptr<HashFunctionContainer> GetInstance()
  {
    if (hashPtr_ == nullptr)
    {
      hashPtr_ = std::make_shared<HashFunctionContainer>();
    }
    return hashPtr_;
  }

  /// @brief HashFunctionContainer Constructor.
  HashFunctionContainer() = default;

 private:
  /// @brief shared pointer of HashFunctionContainer.
  static std::shared_ptr<HashFunctionContainer> hashPtr_;
};

} // namespace crypdatastore
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYP_CONTEXT_CONTAINER_HASH_FUNCTION_CONTAINER_H_