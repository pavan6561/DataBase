// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signer_private_container.h
//
// Purpose     : Signer private container implementation to store the hash function context objects.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYP_CONTEXT_CONTAINER_SIGNER_PRIVATE_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYP_CONTEXT_CONTAINER_SIGNER_PRIVATE_CONTAINER_H_

#include "signer_private_context_skeleton_impl.h"
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
/// @brief SignerPrivateContainer class
class SignerPrivateContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::SignerPrivateContextSkeletonImpl>
{
 public:
  /// @brief SignerPrivateContainer should not be cloneable.
  SignerPrivateContainer(SignerPrivateContainer& other) = delete;

  /// @brief SignerPrivateContainer should not be assignable.
  void operator=(const SignerPrivateContainer&) = delete;

  /// @brief GetInstance for SignerPrivateContainer.
  static std::shared_ptr<SignerPrivateContainer> GetInstance()
  {
    if (signerPrivatePtr_ == nullptr)
    {
      signerPrivatePtr_ = std::make_shared<SignerPrivateContainer>();
    }
    return signerPrivatePtr_;
  }

  /// @brief SignerPrivateContainer Constructor.
  SignerPrivateContainer() = default;

 private:
  /// @brief shared pointer of SignerPrivateContainer.
  static std::shared_ptr<SignerPrivateContainer> signerPrivatePtr_;
};

} // namespace crypdatastore
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYP_CONTEXT_CONTAINER_HASH_FUNCTION_CONTAINER_H_