// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : streamcipher_context_skeleton_impl.h
//
// Purpose     : Stream cipher skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_STREAMCIPHER_CONTEXT_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_STREAMCIPHER_CONTEXT_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/stream_cipher_ctx.h"
#include "secret_seed_container.h"
#include "streamciphercontext_skeleton.h"
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
/// @brief Class definition for Stream Cipher skeleton.
class StreamCipherContextSkeletonImpl
  : public ara::crypto::ipccom::cryp::streamcipher::skeleton::StreamCipherContextSkeleton
{
 public:
  /// @brief Constructor for StreamCipherContextSkeletonImpl class.
  StreamCipherContextSkeletonImpl(
    CryptoServiceInstanceId const instanceId, StreamCipherCtx::Uptr sccUptr) noexcept;

  /// @brief Skeleton implementation for CountBytesInCache method.
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::CountBytesInCacheOutput>
    CountBytesInCache() override;

  /// @brief Skeleton implementation for FinishBytes method.
  /// @param[in] in buffer containging input data.
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::FinishBytesOutput>
    FinishBytes(const CryptoReadOnlyMemRegion& in) override;

  /// @brief Skeleton implementation for GetTransformation method.
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::GetTransformationOutput>
    GetTransformation() override;

  /// @brief Skeleton implementation for IsBytewiseMode method.
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::IsBytewiseModeOutput>
    IsBytewiseMode() override;

  /// @brief Skeleton implementation for IsSeekableMode method.
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::IsSeekableModeOutput>
    IsSeekableMode() override;

  /// @brief Skeleton implementation for ProcessBlocks method.
  /// @param[in] in buffer containging input data.
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::ProcessBlocksOutput>
    ProcessBlocks(const CryptoReadOnlyMemRegion& in) override;

  /// @brief Skeleton implementation for ProcessByte method.
  /// @param[in] in buffer containging input data.
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::ProcessByteOutput>
    ProcessByte(const CryptoReadOnlyMemRegion& in) override;

  /// @brief Skeleton implementation for Reset method.
  ara::core::Future<void> Reset() override;

  /// @brief Skeleton implementation for Seek method.
  /// @param[in] offset  the offset value in bytes, relative to begin or current position in the gamma stream
  /// @param[in] fromBegin  the starting point for positioning within the stream: from begin (if @c true) or
  /// from current position (if @c false).
  ara::core::Future<void> Seek(const Int64& offset, const Boolean& fromBegin) override;

  /// @brief Skeleton implementation for SetKey method.
  /// @param[in] instanceId instance Id of the Symmetrict Key object.
  /// @param[in] transform the "direction" indicator: deploy the key for direct transformation (if @c
  /// true) or for reverse one (if @c false)
  ara::core::Future<void> SetKey(
    const CryptoServiceInstanceId& instanceId, const CryptoTransformDataType& transform) override;

  /// @brief Skeleton implementation for Start method with ReadOnlyMemRegion.
  /// @param iv buffer containing initialization vector.
  ara::core::Future<void> StartReadOnlyMem(const CryptoReadOnlyMemRegion& iv) override;

  /// @brief Skeleton implementation for Start method with SecretSeed.
  /// @param[in] iv instance Id for the SecretSeed object.
  ara::core::Future<void> StartSecretSeed(const CryptoServiceInstanceId& iv) override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::cryp::streamcipher::StreamCipherContext::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;
  ara::core::Future<GetBlockServiceOutput>GetBlockService()override ;
 private:
  /// @brief Unique pointer to the Stream cipher ctx.
  StreamCipherCtx::Uptr sccCtx_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "RNGS", "Random Generator Context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_STREAMCIPHER_CONTEXT_SKELETON_IMPL_H_
