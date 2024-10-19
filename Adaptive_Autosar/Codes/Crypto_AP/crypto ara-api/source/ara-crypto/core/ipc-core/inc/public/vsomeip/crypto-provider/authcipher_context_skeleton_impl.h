// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : authcipher_context_skeleton_impl.h
//
// Purpose     : Authentication cipher skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_AUTHCIPHER_CONTEXT_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_AUTHCIPHER_CONTEXT_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/auth_cipher_ctx.h"
#include "authciphercontext_skeleton.h"
#include "secret_seed_container.h"
#include "signature_container.h"
#include "symmetrickey_container.h"
#include "block_service_container.h"

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
class AuthCipherContextSkeletonImpl
  : public ara::crypto::ipccom::cryp::authenticationcipher::skeleton::AuthCipherContextSkeleton
{
 public:
  /// @brief Constructor for AuthCipherContextSkeletonImpl class.
  AuthCipherContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, AuthCipherCtx::Uptr accUptr) noexcept;

  /// @brief Skeleton implementation for Check API.
  /// @param[in] instanceId of the crypto object.
  ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::CheckOutput> Check(
    const CryptoServiceInstanceId& instanceId) override;

  /// @brief Skeleton implementation for GetDigest API.
  /// @param[in] offset  position of the first byte of digest that should be placed to the output buffer
  ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetDigestOutput>
    GetDigest(const uint64& offset) override;

  /// @brief Skeleton implementation for GetMaxAssociatedDataSize API.
  ara::core::Future<
    ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetMaxAssociatedDataSizeOutput>
    GetMaxAssociatedDataSize() override;

  /// @brief Skeleton implementation for GetTransformation API.
  ara::core::Future<
    ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetTransformationOutput>
    GetTransformation() override;

  /// @brief Skeleton implementation for ProcessConfidentialData API.
  /// @param[in] in buffer with input data.
  /// @param[in] expectedTag expected mac value for decryption.
  ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::
      ProcessConfidentialDataReadOnlyMemOutput>
    ProcessConfidentialDataReadOnlyMem(
      const CryptoReadOnlyMemRegion& in, const CryptoReadOnlyMemRegion& expectedTag) override;

  /// @brief Skeleton implementation for Reset API.
  ara::core::Future<void> Reset() override;

  /// @brief Skeleton implementation for SetKey API.
  /// @param[in] key instance id of crypto object
  /// @param[in] transform transformation value for encryption and decryption.
  ara::core::Future<void> SetKey(
    const CryptoServiceInstanceId& key, const CryptoTransformDataType& transform) override;

  /// @brief Skeleton implementation for Start with ReadOnlyMemRegion API.
  /// @param[in] IV initialization vector for authentication cipher.
  ara::core::Future<void> StartReadOnlyMem(const CryptoReadOnlyMemRegion& iv) override;

  /// @brief Skeleton implementation for Start with SecretSeed API.
  /// @param[in] instanceId of the secret seed object.
  ara::core::Future<void> StartSecretSeed(const CryptoServiceInstanceId& instanceId) override;

  /// @brief Skeleton implementation for UpdateAssociatedData with uint8_t API.
  /// @param[in] in buffer with input data.
  ara::core::Future<void> UpdateAssociatedData(const uint8& in) override;

  /// @brief Skeleton implementation for UpdateAssociatedData with ReadOnlyMemRegion API.
  /// @param[in] in buffer with input data.
  ara::core::Future<void> UpdateAssociatedDataReadOnlyMem(const CryptoReadOnlyMemRegion& in) override;

  /// @brief Skeleton implementation for UpdateAssociatedData with RestrictedUsedObject API.
  /// @param[in] instanceId of the RestrictedUsedObject.
  ara::core::Future<void> UpdateAssociatedDataResUseObj(
    const RestrictedUseObjectStructure& restrictedUseObjStruct) override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::authenticationcipher::AuthCipherContext::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

 ara::core::Future<GetBlockServiceOutput>GetBlockService() override;
 private:
  /// @brief Unique pointer to authentication cipher ctx.
  AuthCipherCtx::Uptr accCtx_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "ACSI", "AUTHN CIPHER Context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_AUTHCIPHER_CONTEXT_SKELETON_IMPL_H_
