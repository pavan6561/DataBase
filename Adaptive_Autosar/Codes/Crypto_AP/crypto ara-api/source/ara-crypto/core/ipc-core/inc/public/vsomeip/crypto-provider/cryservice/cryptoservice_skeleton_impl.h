// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : cryptoservice_skeleton_impl.h
//
// Purpose     : Authentication cipher skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CRYPTOSERVICE_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CRYPTOSERVICE_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/crypto_service.h"
#include "cryptoservice_skeleton.h"
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
class CryptoServiceSkeletonImpl
    : public ara::crypto::ipccom::cryp::cryservice::cryptoservice::skeleton::CryptoServiceSkeleton
{
 public:
  /// @brief Constructor for CryptoServiceSkeletonImpl class.
  CryptoServiceSkeletonImpl(CryptoServiceInstanceId const instanceId,
      ara::crypto::cryp::CryptoService::Uptr cryptoServiceUptr) noexcept;

  /// @brief Definition for GetActualKeyBitLength.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetActualKeyBitLengthOutput>
      GetActualKeyBitLength() override;

  /// @brief Definition for GetActualKeyCOUID.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetActualKeyCOUIDOutput>
      GetActualKeyCOUID() override;

  /// @brief Definition for GetAllowedUsage.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetAllowedUsageOutput>
      GetAllowedUsage() override;

  /// @brief Definition for GetBlockSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetBlockSizeOutput>
      GetBlockSize() override;

  /// @brief Definition for GetMaxInputSize.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxInputSizeOutput>
      GetMaxInputSize(const boolean& suppressPadding) override;

  /// @brief Definition for GetMaxKeyBitLength.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxKeyBitLengthOutput>
      GetMaxKeyBitLength() override;

  /// @brief Definition for GetMaxOutputSize.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxOutputSizeOutput>
      GetMaxOutputSize(const Boolean& suppressPadding) override;

  /// @brief Definition for GetMinKeyBitLength.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMinKeyBitLengthOutput>
      GetMinKeyBitLength() override;

  /// @brief Definition for IsKeyAvailable.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::IsKeyAvailableOutput>
      IsKeyAvailable() override;

  /// @brief Definition for IsKeyBitLengthSupported.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::IsKeyBitLengthSupportedOutput>
      IsKeyBitLengthSupported(const uint64& keyBitLength) override;

 private:
  /// @brief Unique pointer to CryptoService cipher ctx.
  ara::crypto::cryp::CryptoService::Uptr cryptoUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ara::log::CreateLogger(
      "cryservice", "cryservice ExtensionService Context", ara::log::LogLevel::kVerbose)};
};

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_CRYPTOSERVICE_SKELETON_IMPL_H_
