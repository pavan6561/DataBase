// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : digestservice_skeleton_impl.h
//
// Purpose     : Authentication cipher skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_DIGESTSERVICE_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_DIGESTSERVICE_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/digest_service.h"
#include "digestservice_skeleton.h"
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
/// @brief namespace for crypto cryservice.
namespace cryservice
{
/// @brief Class definition for Authentication cipher skeleton.
class DigestServiceSkeletonImpl
    : public ara::crypto::ipccom::cryp::cryservice::digestservice::skeleton::DigestServiceSkeleton
{
 public:
  /// @brief Constructor for ExtensionServiceSkeletonImpl class.
  DigestServiceSkeletonImpl(CryptoServiceInstanceId const instanceId, ara::crypto::cryp::DigestService::Uptr digestUptr) noexcept;

  /// @brief Definition for Compare.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::CompareOutput>
      Compare(const CryptoReadOnlyMemRegion& expected, const uint64& offset) override;

  /// @brief Definition for GetActualIvBitLength.
  ara::core::Future<GetActualIvBitLengthOutput>GetActualIvBitLength(const CryptoObjectUidIPC& ivUid,const Boolean& isOptional) override;

  /// @brief Definition for GetActualKeyBitLength.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetActualKeyBitLengthOutput>
      GetActualKeyBitLength() override;

  /// @brief Definition for GetActualKeyCOUID.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetActualKeyCOUIDOutput>
      GetActualKeyCOUID() override;

  /// @brief Definition for GetAllowedUsage.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetAllowedUsageOutput>
      GetAllowedUsage() override;

  /// @brief Definition for GetBlockSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetBlockSizeOutput>
      GetBlockSize() override;

  /// @brief Definition for GetDigestSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetDigestSizeOutput>
      GetDigestSize() override;

  /// @brief Definition for GetIvSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetIvSizeOutput>
      GetIvSize() override;

  /// @brief Definition for GetMaxKeyBitLength.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetMaxKeyBitLengthOutput>
      GetMaxKeyBitLength() override;

  /// @brief Definition for GetMinKeyBitLength.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetMinKeyBitLengthOutput>
      GetMinKeyBitLength() override;

  /// @brief Definition for IsFinished.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsFinishedOutput>
      IsFinished() override;

  /// @brief Definition for IsKeyAvailable.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsKeyAvailableOutput>
      IsKeyAvailable() override;

  /// @brief Definition for IsKeyBitLengthSupported.
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsKeyBitLengthSupportedOutput>
      IsKeyBitLengthSupported(const uint64& keyBitLength) override;

  /// @brief Definition for IsStarted.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsStartedOutput>
      IsStarted() override;

  /// @brief Definition for IsValidIvSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsValidIvSizeOutput>
      IsValidIvSize(const uint64& ivSize) override;

 private:
  /// @brief Unique pointer to authentication cipher ctx.
  ara::crypto::cryp::DigestService::Uptr digestServiceUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ara::log::CreateLogger(
      "cryservice", "cryservice ExtensionService Context", ara::log::LogLevel::kVerbose)};
};

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYSERVICE_DIGESTSERVICE_SKELETON_IMPL_H_
