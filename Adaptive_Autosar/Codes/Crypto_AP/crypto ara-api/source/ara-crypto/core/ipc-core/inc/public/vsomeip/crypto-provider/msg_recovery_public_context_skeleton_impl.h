// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : msg_recovery_public_context_skeleton_impl.h
//
// Purpose     : Message recovery public context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_MSG_RECOVERY_PUBLIC_CONTEXT_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_MSG_RECOVERY_PUBLIC_CONTEXT_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/msg_recovery_public_ctx.h"
#include "msgrecoverypubliccontext_skeleton.h"
#include "public_key_container.h"
#include "extension_service_container.h"

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
class MsgRecoveryPublicContextSkeletonImpl
  : public ara::crypto::ipccom::cryp::msgrecoverypubliccontext::skeleton::MsgRecoveryPublicContextSkeleton
{
 public:
  /// @brief Constructor for MsgRecoveryPublicContextSkeletonImpl class.
  MsgRecoveryPublicContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, MsgRecoveryPublicCtx::Uptr msgRecPubUptr) noexcept;

  /// @brief Definition for DecodeAndVerify.
  ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::DecodeAndVerifyOutput>
    DecodeAndVerify(const CryptoReadOnlyMemRegion& inputReadOnly) override;

  /// @brief Definition for GetMaxInputSize.
  ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::GetMaxInputSizeOutput>
    GetMaxInputSize(const Boolean& suppressPadding) override;

  /// @brief Definition for GetMaxOutputSize.
  ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::GetMaxOutputSizeOutput>
    GetMaxOutputSize(const Boolean& suppressPadding) override;

  /// @brief Skeleton implementation for Reset API.
  ara::core::Future<void> Reset() override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::msgrecoverypubliccontext::MsgRecoveryPublicContext::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

  /// @brief Definition for SetKey.
  ara::core::Future<void> SetKey(const CryptoServiceInstanceId& publicKeyInstanceId) override;

  ara::core::Future<GetExtensionServiceOutput>GetExtensionService()override;
 private:
  /// @brief Unique pointer to DecryptorPrivateCtx.
  MsgRecoveryPublicCtx::Uptr msgRecPubUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "MRPB", "MSG RECOVERY PUBLIC Context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_MSG_RECOVERY_PUBLIC_CONTEXT_SKELETON_IMPL_H_
