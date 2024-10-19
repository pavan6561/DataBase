// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_key_skeleton_impl.h
//
// Purpose     : symmetric key skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_SYMMETRIC_KEY_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_SYMMETRIC_KEY_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/promise.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "symmetrickey_skeleton.h"

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
/// @brief This class contains the methods for skeleton implementation for symmetric key object.
class SymmetrickeySkeletonImpl
  : public ara::crypto::ipccom::cryp::cryobj::symmetrickey::skeleton::SymmetricKeySkeleton
{
 public:
  /// @brief Constructor for SymmetrickeySkeletonImpl class.
  SymmetrickeySkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::SymmetricKey::Uptrc symmetricUtr) noexcept;

  /// @brief Constructor for SymmetrickeySkeletonImpl class.
  SymmetrickeySkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::RestrictedUseObject::Uptrc restrictedUtr) noexcept;

  /// @brief Destructor for SymmetrickeySkeletonImpl class.
  ~SymmetrickeySkeletonImpl() override = default;

  /// @brief Getter method for constant unique pointer for symmetric key.
  ara::crypto::cryp::SymmetricKey::Uptrc& GetSymmetricKeyUptrInstance()
  {
    return symmetricUtr_;
  }

  /// @brief Getter method for constant unique pointer for restricted used object.
  ara::crypto::cryp::RestrictedUseObject::Uptrc& GetRestrictedUptrInstance()
  {
    return restrictedUptr_;
  }

  /// @brief Definition for GetAllowedUsage
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetAllowedUsageOutput>
    GetAllowedUsage() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

  /// @brief Definition for GetObjectId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetObjectIdOutput>
    GetObjectId() override;

  /// @brief Definition for GetPayloadSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetPayloadSizeOutput>
    GetPayloadSize() override;

  /// @brief Definition for HasDependence.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::HasDependenceOutput>
    HasDependence() override;

  /// @brief Definition for IsExportable.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::IsExportableOutput>
    IsExportable() override;

  /// @brief Definition for IsSession.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::IsSessionOutput>
    IsSession() override;

  /// @brief Definition for Save.
  ara::core::Future<void> Save(const CryptoServiceInstanceId& IOInterfaceInstanceID) override;

 private:
  /// @brief Smart unique pointer for symmetric key.
  ara::crypto::cryp::SymmetricKey::Uptrc symmetricUtr_;

  /// @brief Smart unique pointer for restricted used object.
  ara::crypto::cryp::RestrictedUseObject::Uptrc restrictedUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "SYMK", "SYMMETRIC KEY Object", ara::log::LogLevel::kVerbose) };
};

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // _ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_SKELETON_IMPL_H_
