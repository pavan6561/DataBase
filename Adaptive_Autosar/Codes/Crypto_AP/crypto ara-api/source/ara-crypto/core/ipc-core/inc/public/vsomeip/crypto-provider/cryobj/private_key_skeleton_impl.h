// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : private_key_skeleton_impl.h
//
// Purpose     : private key skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_PRIVATE_KEY_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_PRIVATE_KEY_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/promise.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/cryobj/private_key.h"
#include "privatekey_skeleton.h"
#include "public_key_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for crypto objects.
namespace cryobj
{
/// @brief This class contains the methods for skeleton implementation for private key object.
class PrivateKeySkeletonImpl
  : public ara::crypto::ipccom::cryp::cryobj::privatekey::skeleton::PrivateKeySkeleton
{
 public:
  /// @brief Constructor for PrivateKeySkeletonImpl class.
  PrivateKeySkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::PrivateKey::Uptrc privateKeyUtr) noexcept;

  /// @brief Destructor for PrivateKeySkeletonImpl class.
  ~PrivateKeySkeletonImpl() override = default;

  /// @brief Getter method for constant unique pointer for symmetric key.
  ara::crypto::cryp::PrivateKey::Uptrc& GetPrivateKeyUptrInstance()
  {
    return privateKeyUtr_;
  }
  /// @brief Definition for GetAllowedUsage.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetAllowedUsageOutput>
    GetAllowedUsage() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

  /// @brief Definition for GetObjectId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetObjectIdOutput>
    GetObjectId() override;

  /// @brief Definition for GetPayloadSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetPayloadSizeOutput>
    GetPayloadSize() override;

  /// @brief Definition for GetPublicKey.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::GetPublicKeyOutput>
    GetPublicKey() override;

  /// @brief Definition for HasDependence.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::HasDependenceOutput>
    HasDependence() override;

  /// @brief Definition for IsExportable.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::IsExportableOutput>
    IsExportable() override;

  /// @brief Definition for IsSession.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::privatekey::PrivateKey::IsSessionOutput>
    IsSession() override;

  /// @brief Definition for Save.
  ara::core::Future<void> Save(const CryptoServiceInstanceId& IOInterfaceInstanceId) override;

 private:
  /// @brief Smart unique pointer for symmetric key.
  ara::crypto::cryp::PrivateKey::Uptrc privateKeyUtr_;

  /// @brief Smart unique pointer for restricted used object.
  ara::crypto::cryp::RestrictedUseObject::Uptrc restrictedUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "PRKY", "PRIVATE KEY OBJECT", ara::log::LogLevel::kVerbose) };
};

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_PRIVATE_KEY_SKELETON_IMPL_H_
