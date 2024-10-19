// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sig_encode_private_context_skeleton_impl.h
//
// Purpose     : Sign encode private context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SIGN_ENCODE_PRIVATE_CONTEXT_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SIGN_ENCODE_PRIVATE_CONTEXT_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/sig_encode_private_ctx.h"
#include "private_key_container.h"
#include "sigencodeprivatecontext_skeleton.h"
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
class SignEncodePrivateContextSkeletonImpl
  : public ara::crypto::ipccom::cryp::signencodeprivatecontext::skeleton::SigEncodePrivateContextSkeleton
{
 public:
  /// @brief Constructor for SignEncodePrivateContextSkeletonImpl class.
  SignEncodePrivateContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, SigEncodePrivateCtx::Uptr signEncPrUptr) noexcept;

  /// @brief Definition for SignAndEncode.
  ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::SignAndEncodeOutput>
    SignAndEncode(const CryptoReadOnlyMemRegion& inputReadOnly) override;

  /// @brief Definition for GetMaxInputSize.
  ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetMaxInputSizeOutput>
    GetMaxInputSize(const Boolean& suppressPadding) override;

  /// @brief Definition for GetMaxOutputSize.
  ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetMaxOutputSizeOutput>
    GetMaxOutputSize(const Boolean& suppressPadding) override;

  /// @brief Definition for SetKey.
  ara::core::Future<void> SetKey(const CryptoServiceInstanceId& privateKeyInstanceId) override;

  /// @brief Skeleton implementation for Reset API.
  ara::core::Future<void> Reset() override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::signencodeprivatecontext::SigEncodePrivateContext::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

  ara::core::Future<GetExtensionServiceOutput>GetExtensionService()override;
 private:
  /// @brief Unique pointer to DecryptorPrivateCtx.
  SigEncodePrivateCtx::Uptr signEncPrUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "SEPR", "SIGN ENCODE PRIVATE CONTEXT", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SIGN_ENCODE_PRIVATE_CONTEXT_SKELETON_IMPL_H_
