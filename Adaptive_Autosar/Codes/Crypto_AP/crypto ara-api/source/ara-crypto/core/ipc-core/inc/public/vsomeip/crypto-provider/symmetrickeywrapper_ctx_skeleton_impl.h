// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetrickeywrapper_ctx_skeleton_impl.h
//
// Purpose     : Symmetric key wrapper skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SYMMETRICKEYWRAPPER_CTX_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SYMMETRICKEYWRAPPER_CTX_SKELETON_IMPL_H_

#include <ara/core/result.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/symmetric_key_wrapper_ctx.h"
#include "restricted_used_object_container.h"
#include "secret_seed_container.h"
#include "symmetrickey_container.h" 
#include "extension_service_container.h"
#include "symmetrickeywrappercontext_skeleton.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Class definition for Symmetric Key wrapper skeleton.
class SymmetricKeyWrapperCtxSkeletonImpl
  : public ara::crypto::ipccom::cryp::symmetrickeywrapper::skeleton::SymmetricKeyWrapperContextSkeleton
{
 public:
  /// @brief Constructor for SymmetricKeyWrapperCtxSkeletonImpl class.
  SymmetricKeyWrapperCtxSkeletonImpl(
    CryptoServiceInstanceId const instanceId, SymmetricKeyWrapperCtx::Uptr skwcUptr) noexcept;

  /// @brief Skeleton implementation for SetKey method.
  /// @param[in] instanceId instance id of the symmetric key object.
  /// @param[in] transform the "direction" indicator: deploy the key for direct transformation (if @c
  /// true) or for reverse one (if @c false)
  ara::core::Future<void> SetKey(
    const CryptoServiceInstanceId& keyInstanceId, const CryptoTransformDataType& transform) override;

  /// @brief Skeleton implementation for UnwrapKey method.
  /// @param[in] wrappedKey wrapped key value.
  /// @param[in] algId  an identifier of the target symmetric crypto algorithm
  /// @param[in] allowedUsage  bit-flags that define a list of allowed transformations' types in which the
  /// target key can be used
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::UnwrapKeyOutput>
    UnwrapKey(const CryptoReadOnlyMemRegion& wrappedKey, const CryptoAlgId& algId,
      const CryptoAllowedUsageFlags& allowedUsage) override;

  /// @brief Skeleton implementation for WrapKeyMaterial method.
  /// @param instanceId instance Id of the symmetric key object.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::WrapKeyMaterialOutput>
    WrapKeyMaterial(const RestrictedUseObjectStructure& restrictedUseObjStruct) override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for CalculateWrappedKeySize.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::CalculateWrappedKeySizeOutput>
    CalculateWrappedKeySize(const uint64& keyLength) override;

  /// @brief Definition for GetMaxTargetKeyLength.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetMaxTargetKeyLengthOutput>
    GetMaxTargetKeyLength() override;

  /// @brief Definition for GetTargetKeyGranularity.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetTargetKeyGranularityOutput>
    GetTargetKeyGranularity() override;

  /// @brief Definition for Reset.
  ara::core::Future<void> Reset() override;

  /// @brief Definition for UnwrapSeed.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::UnwrapSeedOutput>
    UnwrapSeed(const CryptoReadOnlyMemRegion& wrappedSeed, const CryptoAlgId& targetAlgId,
      const CryptoAllowedUsageFlags& allowedUsage) override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;
 ara::core::Future<GetExtensionServiceOutput>GetExtensionService()override ;
 private:
  /// @brief Unique pointer to the Symmetric key wrapper ctx.
  SymmetricKeyWrapperCtx::Uptr skwcCtx_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "SKWC", "Symmetric Key Wrapper Cipher Context", ara::log::LogLevel::kVerbose) };

  /// @brief mutex for instance Id.
  std::mutex mutex_instanceId_;

  /// @brief variable to store the instance Id.
  std::uint16_t serviceInstanceId_;

  /// @brief method to generate the instance Id.
  CryptoServiceInstanceId GenerateCryptoServiceInstanceId() noexcept;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_SYMMETRICKEYWRAPPER_CTX_SKELETON_IMPL_H_
