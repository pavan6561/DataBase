// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_SIGNATURE_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_SIGNATURE_CONTAINER_H_

#include "signature_skeleton_impl.h"
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
/// @brief SignatureContainer classs
class SignatureContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryobj::SignatureSkeletonImpl>
{
 public:
  /// @brief SignatureContainer should not be cloneable.
  SignatureContainer(SignatureContainer& other) = delete;

  /// @brief SignatureContainer should not be assignable.
  void operator=(const SignatureContainer&) = delete;

  /// @brief GetInstance for SignatureContainer.
  static std::shared_ptr<SignatureContainer> GetInstance()
  {
    if (signatureContainerPtr_ == nullptr)
    {
      signatureContainerPtr_ = std::make_shared<SignatureContainer>();
    }
    return signatureContainerPtr_;
  }

  /// @brief SignatureContainer Constructor.
  SignatureContainer() = default;

 private:
  /// @brief shared pointer of SignatureContainer.
  static std::shared_ptr<SignatureContainer> signatureContainerPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CONTAINER_SIGNATURE_CONTAINER_H_