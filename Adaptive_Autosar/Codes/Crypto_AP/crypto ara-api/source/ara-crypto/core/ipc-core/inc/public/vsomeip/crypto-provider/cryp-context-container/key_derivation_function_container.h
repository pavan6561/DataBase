// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_derivation_function_container.h
//
// Purpose     : key derivation function container implementation to store the objects of key derivation
//               function context.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYP_CONTEXT_CONTAINER_KEY_DERIVATION_FUNCTION_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYP_CONTEXT_CONTAINER_KEY_DERIVATION_FUNCTION_CONTAINER_H_

#include "key_derivation_function_skeleton_impl.h"
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
/// @brief KeyDerivationFunctionContainer class
class KeyDerivationFunctionContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::KeyDerivationFunctionskeletonImpl>
{
 public:
  /// @brief KeyDerivationFunctionContainer should not be cloneable.
  KeyDerivationFunctionContainer(KeyDerivationFunctionContainer& other) = delete;

  /// @brief KeyDerivationFunctionContainer should not be assignable.
  void operator=(const KeyDerivationFunctionContainer&) = delete;

  /// @brief GetInstance for KeyDerivationFunctionContainer.
  static std::shared_ptr<KeyDerivationFunctionContainer> GetInstance()
  {
    if (kdfPtr_ == nullptr)
    {
      kdfPtr_ = std::make_shared<KeyDerivationFunctionContainer>();
    }
    return kdfPtr_;
  }

  /// @brief KeyDerivationFunctionContainer Constructor.
  KeyDerivationFunctionContainer() = default;

 private:
  /// @brief shared pointer of KeyDerivationFunctionContainer.
  static std::shared_ptr<KeyDerivationFunctionContainer> kdfPtr_;
};

} // namespace crypdatastore
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYP_CONTEXT_CONTAINER_KEY_DERIVATION_FUNCTION_CONTAINER_H_