// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : public_key_skeleton_impl.h
//
// Purpose     : public key skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_PUBLIC_KEY_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_PUBLIC_KEY_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/promise.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/cryobj/public_key.h"
#include "publickey_skeleton.h"

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
/// @brief This class contains the methods for skeleton implementation for public key object.
class PublicKeySkeletonImpl : public ara::crypto::ipccom::cryp::cryobj::pubilckey::skeleton::PublicKeySkeleton
{
 public:
  /// @brief Constructor for PublicKeySkeletonImpl class.
  PublicKeySkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::PublicKey::Uptrc publicKeyUtr) noexcept;

  /// @brief Destructor for PublicKeySkeletonImpl class.
  ~PublicKeySkeletonImpl() override = default;

  /// @brief Getter method for constant unique pointer for symmetric key.
  ara::crypto::cryp::PublicKey::Uptrc& GetPublicKeyUptrInstance()
  {
    return publicKeyUtr_;
  }

  /// @brief Definition for CheckKey.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::CheckKeyOutput> CheckKey(
    const Boolean& strongCheck) override;

  /// @brief Definition for ExportPublicly.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::ExportPubliclyOutput>
    ExportPublicly(const CryptoFormatId& FormatId) override;

  /// @brief Definition for GetAllowedUsage.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::GetAllowedUsageOutput>
    GetAllowedUsage() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

  /// @brief Definition for GetObjectId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::GetObjectIdOutput>
    GetObjectId() override;

  /// @brief Definition for GetPayloadSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::GetPayloadSizeOutput>
    GetPayloadSize() override;

  /// @brief Definition for HasDependence.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::HasDependenceOutput>
    HasDependence() override;

  /// @brief Definition for HashPublicKey.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::HashPublicKeyOutput>
    HashPublicKey(const CryptoServiceInstanceId& HashFunctionCtxInstanceId) override;

  /// @brief Definition for IsExportable.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::IsExportableOutput>
    IsExportable() override;

  /// @brief Definition for IsSession.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::pubilckey::PublicKey::IsSessionOutput>
    IsSession() override;

  /// @brief Definition for Save.
  ara::core::Future<void> Save(const CryptoServiceInstanceId& IOInterfaceInstanceId) override;

 private:
  /// @brief Smart unique pointer for symmetric key.
  ara::crypto::cryp::PublicKey::Uptrc publicKeyUtr_;

  /// @brief Smart unique pointer for restricted used object.
  ara::crypto::cryp::RestrictedUseObject::Uptrc restrictedUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "PBKY", "PUBLIC KEY OBJECT", ara::log::LogLevel::kVerbose) };
};

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_PUBLIC_KEY_SKELETON_IMPL_H_
