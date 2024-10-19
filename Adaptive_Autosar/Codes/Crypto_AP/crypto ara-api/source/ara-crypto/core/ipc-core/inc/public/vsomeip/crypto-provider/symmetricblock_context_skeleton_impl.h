// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetricblock_context_skeleton_impl.h
//
// Purpose     : Symmetric block cipher skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SYMMETRICBLOCK_CONTEXT_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SYMMETRICBLOCK_CONTEXT_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/symmetric_block_cipher_ctx.h"
#include "symmetricblockciphercontext_skeleton.h"
#include "symmetrickey_container.h" 
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
/// @brief Class definition for Block Cipher skeleton.
class SymmetricBlockCipherContextSkeletonImpl
  : public ara::crypto::ipccom::cryp::symmetricblockcipher::skeleton::SymmetricBlockCipherContextSkeleton
{
 public:
  /// @brief Constructor for SymmetricBlockCipherContextSkeletonImpl class.
  SymmetricBlockCipherContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, SymmetricBlockCipherCtx::Uptr sbccUptr) noexcept;

  /// @brief Skeleton implementation for GetTransformation method.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::GetTransformationOutput>
    GetTransformation() override;

  /// @brief Skeleton implementation for ProcessBlock method.
  /// @param[in] in buffer containing input data.
  /// @param[in] suppressPadding flag to remove padding if nedded.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::ProcessBlockOutput>
    ProcessBlock(const CryptoReadOnlyMemRegion& in, const Boolean& suppressPadding) override;

  /// @brief Skeleton implementation for ProcessBlock method.
  /// @param[in] in buffer containing input data.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::ProcessBlocksOutput>
    ProcessBlocks(const CryptoReadOnlyMemRegion& in) override;

  /// @brief Skeleton implementation for Reset method.
  ara::core::Future<void> Reset() override;

  /// @brief Skeleton implementation for SetKey method.
  /// @param[in] instanceId instance id of the symmetric key object.
  /// @param[in] transform the "direction" indicator: deploy the key for direct transformation (if @c
  /// true) or for reverse one (if @c false).
  ara::core::Future<void> SetKey(
    const CryptoServiceInstanceId& instanceId, const CryptoTransformDataType& transform) override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetricblockcipher::SymmetricBlockCipherContext::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

 ara::core::Future<GetCryptoServiceOutput>GetCryptoService() override;
 private:
  /// @brief Unique pointer to symmetric block cipher ctx.
  SymmetricBlockCipherCtx::Uptr sbccCtx_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "SBCC", "Symmetric Block Cipher Context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SYMMETRICBLOCK_CONTEXT_SKELETON_IMPL_H_
