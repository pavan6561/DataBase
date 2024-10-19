// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : decryptor_private_context_skeleton_impl.h
//
// Purpose     : Decryptor private context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_DECRYPTOR_PRIVATE_CONTEXT_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_DECRYPTOR_PRIVATE_CONTEXT_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/decryptor_private_ctx.h"
#include "decryptorprivatecontext_skeleton.h"
#include "private_key_container.h"
#include "crypto_service_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Class definition for Authentication cipher skeleton.
class DecryptorPrivateContextSkeletonImpl
  : public ara::crypto::ipccom::cryp::decryptorprivatecontext::skeleton::DecryptorPrivateContextSkeleton
{
 public:
  /// @brief Constructor for DecryptorPrivateContextSkeletonImpl class.
  DecryptorPrivateContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, DecryptorPrivateCtx::Uptr decPrUptr) noexcept;

  /// @brief Definition for ProcessBlockReadOnly.
  ara::core::Future<
    ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::ProcessBlockReadOnlyOutput>
    ProcessBlockReadOnly(
      const CryptoReadOnlyMemRegion& readOnlyMemoryRegion, const Boolean& suppressPadding) override;

  /// @brief Definition for SetKey.
  ara::core::Future<void> SetKey(const CryptoServiceInstanceId& privateKeyInstanceId) override;

  /// @brief Skeleton implementation for Reset API.
  ara::core::Future<void> Reset() override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<
    ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;
 ara::core::Future<GetCryptoServiceOutput>GetCryptoService()override;
 private:
  /// @brief Unique pointer to DecryptorPrivateCtx.
  DecryptorPrivateCtx::Uptr decPrUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "DCPR", "DECRYPTOR PRIVATE Context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_DECRYPTOR_PRIVATE_CONTEXT_SKELETON_IMPL_H_
