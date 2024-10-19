// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509dn_container.h
//
// Purpose     : X509Dn skeleton data storage container implementation to store the object into skeleton 
//               container.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509_DATA_STORAGE_X509DN_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509_DATA_STORAGE_X509DN_CONTAINER_H_

#include "x509_dn_skeleton_impl.h"
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
/// @brief X509DnContainer class
class X509DnContainer final : public SkeletonDataStorageContanier<ara::crypto::x509::X509DnSkeletonImpl>
{
 public:
  /// @brief X509DnContainer should not be cloneable.
  X509DnContainer(X509DnContainer& other) = delete;

  /// @brief X509DnContainer should not be assignable.
  void operator=(const X509DnContainer&) = delete;

  /// @brief GetInstance for X509DnContainer.
  static std::shared_ptr<X509DnContainer> GetInstance()
  {
    if (x509DnPtr_ == nullptr)
    {
      x509DnPtr_ = std::make_shared<X509DnContainer>();
    }
    return x509DnPtr_;
  }

  /// @brief X509DnContainer Constructor.
  X509DnContainer() = default;

 private:
  /// @brief shared pointer of X509DnContainer.
  static std::shared_ptr<X509DnContainer> x509DnPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509_DATA_STORAGE_X509DN_CONTAINER_H_
