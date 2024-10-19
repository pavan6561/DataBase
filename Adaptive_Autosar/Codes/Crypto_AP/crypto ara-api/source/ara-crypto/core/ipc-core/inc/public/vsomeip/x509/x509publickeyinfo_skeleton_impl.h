// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509publickeyinfo_skeleton_impl.h
//
// Purpose     : X509 domain number skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509PUBLICKEYINFO_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509PUBLICKEYINFO_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/x509/x509_public_key_info.h"
#include "x509publickeyinfo_skeleton.h"
#include "public_key_container.h"
/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
/// @brief Class definition for X509 Domain Number skeleton.
class X509PublicKeyInfoSkeletonImpl
    : public ara::crypto::ipccom::x509::pki::skeleton::X509PublicKeyInfoSkeleton
{
  using ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetAlgorithmIdOutput;

 public:
  /// @brief Constructor for X509PublicKeyInfoSkeletonImpl class.
  X509PublicKeyInfoSkeletonImpl(
      CryptoServiceInstanceId const instanceId, ara::crypto::x509::X509PublicKeyInfo::Uptrc publicKeyInfoUptr) noexcept;

  /// @brief Definition for ExportPublicly.
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::ExportPubliclyOutput> ExportPublicly(
      const uint32& formatId) override;

  /// @brief Definition for GetAlgorithmId
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetAlgorithmIdOutput>
      GetAlgorithmId() override;

  /// @brief Definition for GetPublicKey
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetPublicKeyOutput>
      GetPublicKey() override;

  /// @brief Definition for GetRequiredHashAlgId.
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetRequiredHashAlgIdOutput>
      GetRequiredHashAlgId() override;

  /// @brief Definition for GetRequiredHashSize.
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetRequiredHashSizeOutput>
      GetRequiredHashSize() override;

  /// @brief Definition for GetRequiredHashSize.
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetSignatureSizeOutput>
      GetSignatureSize() override;

  /// @brief Definition for IsSameKey.
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::IsSameKeyOutput> IsSameKey(
      const CryptoServiceInstanceId& publicKeyInstanceId) override;

 private:
  /// @brief Instance Id for crypto provider service.
  CryptoServiceInstanceId cryptoServiceInstanceId_;

  /// @brief Unique pointer to X509 Domain Number ctx.
  ara::crypto::x509::X509PublicKeyInfo::Uptrc pkiUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{
      ara::log::CreateLogger("X9DN", "X509 Domain Number", ara::log::LogLevel::kVerbose)};
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509PUBLICKEYINFO_SKELETON_IMPL_H_
