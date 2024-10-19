// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : verifier_public_context_skeleton_impl.h
//
// Purpose     : Verifier public context skeleton implementation to forward functional calls to
//               crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_VERIFIER_PUBLIC_CONTEXT_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_VERIFIER_PUBLIC_CONTEXT_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/verifier_public_ctx.h"
#include "public_key_container.h"
#include "signature_container.h"
#include "signature_service_container.h"
#include "skeleton_data_storage_container.h"
#include "verifierpubliccontext_skeleton.h" 

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
class VerifierPublicContextSkeletonImpl
  : public ara::crypto::ipccom::cryp::verifierpubliccontext::skeleton::VerifierPublicContextSkeleton
{
 public:
  /// @brief Constructor for VerifierPublicContextSkeletonImpl class.
  VerifierPublicContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, VerifierPublicCtx::Uptr verifierPubUptr) noexcept;

  /// @brief Definition for VerifyPrehashedHashFunction.
  ara::core::Future<ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::
      VerifyPrehashedHashFunctionOutput>
    VerifyPrehashedHashFunction(const CryptoServiceInstanceId& hashFunctionInstanceId,
      const CryptoReadOnlyMemRegion& signatureReadOnly,
      const CryptoReadOnlyMemRegion& contextReadOnly) override;

  /// @brief Definition for VerifyPrehashedHashFunctionSignature.
  ara::core::Future<ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::
      VerifyPrehashedHashFunctionSignatureOutput>
    VerifyPrehashedHashFunctionSignature(const CryptoServiceInstanceId& hashFunctionInstanceId,
      const CryptoServiceInstanceId& signatureInstanceId,
      const CryptoReadOnlyMemRegion& contextReadOnly) override;

  /// @brief Definition for VerifyPrehashedReadOnly.
  ara::core::Future<
    ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::VerifyPrehashedReadOnlyOutput>
    VerifyPrehashedReadOnly(const CryptoAlgId& hashAlgId, const CryptoReadOnlyMemRegion& hashValueReadOnly,
      const CryptoServiceInstanceId& signatureInstanceId,
      const CryptoReadOnlyMemRegion& contextReadOnly) override;

  /// @brief Definition for VerifyReadOnly.
  ara::core::Future<
    ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::VerifyReadOnlyOutput>
    VerifyReadOnly(const CryptoReadOnlyMemRegion& valueReadOnly,
      const CryptoReadOnlyMemRegion& signatureReadOnly,
      const CryptoReadOnlyMemRegion& contextReadOnly) override;

  /// @brief Definition for SetKey.
  ara::core::Future<void> SetKey(const CryptoServiceInstanceId& publicKeyInstanceId) override;

  /// @brief Skeleton implementation for Reset API.
  ara::core::Future<void> Reset() override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<
    ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::verifierpubliccontext::VerifierPublicContext::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;
 ara::core::Future<GetSignatureServiceOutput>GetSignatureService()override;
 private:
  /// @brief Unique pointer to DecryptorPrivateCtx.
  VerifierPublicCtx::Uptr verifierPubUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "VPCS", "VERIFIER PUBLIC CONTEXT SKELETON", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_VERIFIER_PUBLIC_CONTEXT_SKELETON_IMPL_H_
