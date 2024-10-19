// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : extensionservice_skeleton_impl.h
//
// Purpose     : Authentication cipher skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_EXTENSIONSERVICE_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_EXTENSIONSERVICE_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/extension_service.h"
#include "extensionservice_skeleton.h"
#include "secret_seed_container.h"
#include "signature_container.h"
#include "symmetrickey_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
namespace cryservice
{
/// @brief Class definition for Authentication cipher skeleton.
class ExtensionServiceSkeletonImpl
    : public ara::crypto::ipccom::cryp::cryservice::extensionservice::skeleton::ExtensionServiceSkeleton
{
 public:
  /// @brief Constructor for ExtensionServiceSkeletonImpl class.
  ExtensionServiceSkeletonImpl(CryptoServiceInstanceId const instanceId,
      ara::crypto::cryp::ExtensionService::Uptr extensionUptr) noexcept;

  /// @brief Definition for GetActualKeyBitLength.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetActualKeyBitLengthOutput>
      GetActualKeyBitLength() override;

  /// @brief Definition for GetActualKeyCOUID.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetActualKeyCOUIDOutput>
      GetActualKeyCOUID() override;

  /// @brief Definition for GetAllowedUsage.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetAllowedUsageOutput>
      GetAllowedUsage() override;

  /// @brief Definition for GetMaxKeyBitLength.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetMaxKeyBitLengthOutput>
      GetMaxKeyBitLength() override;

  /// @brief Definition for GetMinKeyBitLength.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetMinKeyBitLengthOutput>
      GetMinKeyBitLength() override;

  /// @brief Definition for IsKeyAvailable.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::IsKeyAvailableOutput>
      IsKeyAvailable() override;

  /// @brief Definition for IsKeyBitLengthSupported.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::
          IsKeyBitLengthSupportedOutput>
      IsKeyBitLengthSupported(const uint64& keyBitLength) override;

 private:
  /// @brief Unique pointer to authentication cipher ctx.
  ara::crypto::cryp::ExtensionService::Uptr extensionServiceUptr;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ara::log::CreateLogger(
      "cryservice", "cryservice ExtensionService Context", ara::log::LogLevel::kVerbose)};
};

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_EXTENSIONSERVICE_SKELETON_IMPL_H_
