// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signer_private_context_skeleton_impl.h
//
// Purpose     : Signer private context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SIGNER_PRIVATE_CONTEXT_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SIGNER_PRIVATE_CONTEXT_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/signer_private_ctx.h"
#include "private_key_container.h"
#include "signature_container.h"
#include "signature_service_container.h"
#include "signerprivatecontext_skeleton.h" 
#include "signatureservice_skeleton_impl.h"

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
class SignerPrivateContextSkeletonImpl
  : public ara::crypto::ipccom::cryp::signerprivatecontext::skeleton::SignerPrivateContextSkeleton
{
 public:
  /// @brief Constructor for SignerPrivateContextSkeletonImpl class.
  SignerPrivateContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, SignerPrivateCtx::Uptr signerPrUptr) noexcept;

  /// @brief Definition for SignPreHashedHashFunction.
  ara::core::Future<
    ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::SignPreHashedHashFunctionOutput>
    SignPreHashedHashFunction(const CryptoServiceInstanceId& hashFunctionInstanceId,
      const CryptoReadOnlyMemRegion& contextReadOnly) override;

  /// @brief Definition for SignPreHashedReadOnly.
  ara::core::Future<
    ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::SignPreHashedReadOnlyOutput>
    SignPreHashedReadOnly(const CryptoAlgId& hashAlgId, const CryptoReadOnlyMemRegion& hashValueReadOnly,
      const CryptoReadOnlyMemRegion& contextReadOnly) override;

  /// @brief Definition for SignReadOnly.
  ara::core::Future<ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::SignReadOnlyOutput>
    SignReadOnly(
      const CryptoReadOnlyMemRegion& valueReadOnly, const CryptoReadOnlyMemRegion& contextReadOnly) override;

  /// @brief Definition for SetKey.
  ara::core::Future<void> SetKey(const CryptoServiceInstanceId& privateKeyInstanceId) override;

  /// @brief Skeleton implementation for Reset API.
  ara::core::Future<void> Reset() override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<
    ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::signerprivatecontext::SignerPrivateContext::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

 ara::core::Future<GetSignatureServiceOutput>GetSignatureService()override;
 private:
  /// @brief Unique pointer to DecryptorPrivateCtx.
  SignerPrivateCtx::Uptr signerPrUptr_;

  // std::unique_ptr<ara::crypto::cryp::CryptoProviderSkeletonImpl> cryptoSkeletonImpl_;
  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "SRPR", "SIGNER PRIVATE CONTEXT", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SIGNER_PRIVATE_CONTEXT_SKELETON_IMPL_H_
