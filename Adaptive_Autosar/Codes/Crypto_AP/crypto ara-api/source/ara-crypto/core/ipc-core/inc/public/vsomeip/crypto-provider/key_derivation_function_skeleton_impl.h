// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_derivation_function_skeleton_impl.h
//
// Purpose     : Key derivation function skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_KEY_DERIVATION_FUNCTION_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_KEY_DERIVATION_FUNCTION_SKELETON_IMPL_H_

#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "keyderivationfunction_skeleton.h"
#include "secret_seed_container.h"
#include "symmetrickey_container.h"
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
/// @brief Class definition for KDF skeleton.
class KeyDerivationFunctionskeletonImpl
  : public ara::crypto::ipccom::cryp::keyderivationfunction::skeleton::KeyDerivationFunctionSkeleton
{
 public:
  /// @brief Constructor for KeyDerivationFunctionskeletonImpl class.
  KeyDerivationFunctionskeletonImpl(
    CryptoServiceInstanceId const instanceId, KeyDerivationFunctionCtx::Uptr kdfUptr) noexcept;

  /// @brief Skeleton implementation for AddSalt method with ReadOnlyMemRegion.
  /// @param[in] salt value of salt.
  ara::core::Future<void> AddSaltReadOnlyMem(const CryptoReadOnlyMemRegion& salt) override;

  /// @brief Skeleton implementation for AddSalt method with SecretSeed.
  /// @param[in] InstanceId for SecretSeed object.
  ara::core::Future<void> AddSecretSalt(const CryptoServiceInstanceId& InstanceIdsecretSeed) override;

  /// @brief Skeleton implementation for ConfigIterations method.
  /// @param[in] iterations for Key Derivation.
  ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::ConfigIterationsOutput>
    ConfigIterations(const uint32& iterations) override;

  /// @brief Skeleton implementation for DeriveKey method.
  /// @param[in] isSession flag to indicate derive a session's key.
  /// @param[in] isExportable flag to indicate derive a exportable key.
  ara::core::Future<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveKeyOutput>
    DeriveKey(const Boolean& isSession, const Boolean& isExportable) override;

  /// @brief Skeleton implementation for DeriveSeed method.
  /// @param[in] isSession flag to indicate derive a session's seed.
  /// @param[in] isExportable flag to indicate derive a exportable seed.
  ara::core::Future<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveSeedOutput>
    DeriveSeed(const Boolean& isSession, const Boolean& isExportable) override;

  /// @brief Skeleton implementation for GetExtensionService method.
  ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetExtensionServiceOutput>
    GetExtensionService() override;

  /// @brief Skeleton implementation for GetExtensionService method.
  ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetKeyIdSizeOutput>
    GetKeyIdSize() override;

  /// @brief Skeleton implementation for GetTargetAlgId method.
  ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetAlgIdOutput>
    GetTargetAlgId() override;

  /// @brief Skeleton implementation for GetTargetAllowedUsage method.
  ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetAllowedUsageOutput>
    GetTargetAllowedUsage() override;

  /// @brief Skeleton implementation for GetTargetKeyBitLength method.
  ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetKeyBitLengthOutput>
    GetTargetKeyBitLength() override;

  /// @brief Skeleton implementation for Init method.
  /// @param[in] targetKeyId  ID of the target key
  /// @param[in] targetAlgId  the identifier of the target symmetric crypto algorithm
  /// @param[in] allowedUsage  bit-flags that define a list of allowed transformations' types in which the
  /// target key may be used
  /// @param[in] ctxLabel  an optional application specific "context label" (this can identify the purpose of
  /// the target key and/or communication parties)
  ara::core::Future<void> Init(const CryptoReadOnlyMemRegion& targetKeyId, const CryptoAlgId& targetAlgId,
    const CryptoAllowedUsageFlags& allowedUsage, const CryptoReadOnlyMemRegion& ctxLabel) override;

  /// @brief Skeleton implementation for Reset method.
  ara::core::Future<void> Reset() override;

  /// @brief Skeleton implementation for SetSourceKeyMaterial method.
  /// @param[in] restrictedObjectInstanceId instance Id of the restricted used object.
  ara::core::Future<void> SetSourceKeyMaterial(
    const RestrictedUseObjectStructure& restrictedUseObjStruct) override;

  /// @brief Definition for IsInitialized.
  ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::IsInitializedOutput>
    IsInitialized() override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<
    ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

 private:
  /// @brief unique pointer to KDF ctx.
  KeyDerivationFunctionCtx::Uptr kdfCtx_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "KDFI", "HASH FUNCTION Context", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_KEY_DERIVATION_FUNCTION_SKELETON_IMPL_H_
