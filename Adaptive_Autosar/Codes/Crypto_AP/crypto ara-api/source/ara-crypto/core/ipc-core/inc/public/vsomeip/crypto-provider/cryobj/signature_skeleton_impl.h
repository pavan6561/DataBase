// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature_skeleton_impl.h
//
// Purpose     : signature skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_SIGNATURE_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_SIGNATURE_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/promise.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/cryobj/signature.h"
#include "signature_skeleton.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for crypto objects.
namespace cryobj
{
/// @brief This class contains the methods for skeleton implementation for signature object.
class SignatureSkeletonImpl : public ara::crypto::ipccom::cryp::cryobj::signature::skeleton::SignatureSkeleton
{
 public:
  /// @brief Constructor for SignatureSkeletonImpl class.
  SignatureSkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::Signature::Uptrc signatureUtrc) noexcept;

  /// @brief Destructor for SignatureSkeletonImpl class.
  ~SignatureSkeletonImpl() override = default;

  /// @brief Getter method for unique pointer for signature object.
  ara::crypto::cryp::Signature::Uptrc& GetSignatureUptrInstance()
  {
    return signatureUptrc_;
  }

  /// @brief Definition for ExportPublicly.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::ExportPubliclyOutput>
    ExportPublicly(const CryptoFormatId& FormatId) override;

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

  /// @brief Definition for GetHashAlgId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetHashAlgIdOutput>
    GetHashAlgId() override;

  /// @brief Definition for GetObjectId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetObjectIdOutput>
    GetObjectId() override;

  /// @brief Definition for GetPayloadSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetPayloadSizeOutput>
    GetPayloadSize() override;

  /// @brief Definition for GetRequiredHashSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetRequiredHashSizeOutput>
    GetRequiredHashSize() override;

  /// @brief Definition for HasDependence.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::HasDependenceOutput>
    HasDependence() override;

  /// @brief Definition for IsExportable.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::IsExportableOutput>
    IsExportable() override;

  /// @brief Definition for IsSession.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::IsSessionOutput>
    IsSession() override;

  /// @brief Definition for Save.
  ara::core::Future<void> Save(const CryptoServiceInstanceId& IOInterfaceInstanceID) override;

 private:
  /// @brief Smart unique pointer for signature unique pointer.
  ara::crypto::cryp::Signature::Uptrc signatureUptrc_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "SIGN", "SIGNATURE Object", ara::log::LogLevel::kVerbose) };
};

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // _ARA_CRYPTO_CRYP_CRYOBJ_SIGNATURE_SKELETON_IMPL_H_
