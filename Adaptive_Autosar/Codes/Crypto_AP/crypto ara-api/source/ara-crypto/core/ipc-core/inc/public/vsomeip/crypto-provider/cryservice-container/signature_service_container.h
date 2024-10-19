// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature_service_container.h
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_SIGNATURE_SERVICE_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_SIGNATURE_SERVICE_CONTAINER_H_

#include "signatureservice_skeleton_impl.h"
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
/// @brief SignatureServiceContainer class
class SignatureServiceContainer final
  : public SkeletonDataStorageContanier<ara::crypto::cryp::cryservice::SignatureServiceSkeletonImpl>
{
 public:
  /// @brief SignatureServiceContainer should not be cloneable.
  SignatureServiceContainer(SignatureServiceContainer& other) = delete;

  /// @brief SignatureServiceContainer should not be assignable.
  void operator=(const SignatureServiceContainer&) = delete;

  /// @brief GetInstance for SignatureServiceContainer.
  static std::shared_ptr<SignatureServiceContainer> GetInstance()
  {
    if (signatureServiceContainerPtr_ == nullptr)
    {
      signatureServiceContainerPtr_ = std::make_shared<SignatureServiceContainer>();
    }
    return signatureServiceContainerPtr_;
  }

  /// @brief SignatureServiceContainer Constructor.
  SignatureServiceContainer() = default;

 private:
  /// @brief shared pointer of SignatureServiceContainer.
  static std::shared_ptr<SignatureServiceContainer> signatureServiceContainerPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CONTAINER_SIGNATURE_SERVICE_CONTAINER_H_
