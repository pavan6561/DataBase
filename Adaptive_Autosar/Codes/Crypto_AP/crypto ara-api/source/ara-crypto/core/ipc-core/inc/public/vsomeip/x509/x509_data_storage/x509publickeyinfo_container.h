// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509publickeyinfo_container.h
//
// Purpose     : x509 public key info data container implementation to store the X509 PKI skeleton objects
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509_DATA_STORAGE_X509PUBLICKEYINFO_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509_DATA_STORAGE_X509PUBLICKEYINFO_CONTAINER_H_

#include "x509publickeyinfo_skeleton_impl.h"
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
/// @brief X509PublicKeyInfoContainer class
class X509PublicKeyInfoContainer final
    : public SkeletonDataStorageContanier<ara::crypto::x509::X509PublicKeyInfoSkeletonImpl>
{
 public:
  /// @brief X509PublicKeyInfoContainer should not be cloneable.
  X509PublicKeyInfoContainer(X509PublicKeyInfoContainer& other) = delete;

  /// @brief X509PublicKeyInfoContainer should not be assignable.
  void operator=(const X509PublicKeyInfoContainer&) = delete;

  /// @brief GetInstance for X509PublicKeyInfoContainer.
  static std::shared_ptr<X509PublicKeyInfoContainer> GetInstance()
  {
    if (x509PkiPtr_ == nullptr)
    {
      x509PkiPtr_ = std::make_shared<X509PublicKeyInfoContainer>();
    }
    return x509PkiPtr_;
  }

  /// @brief X509PublicKeyInfoContainer Constructor.
  X509PublicKeyInfoContainer() = default;

 private:
  /// @brief shared pointer of X509PublicKeyInfoContainer.
  static std::shared_ptr<X509PublicKeyInfoContainer> x509PkiPtr_;
};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509_DATA_STORAGE_X509DN_CONTAINER_H_