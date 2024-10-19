// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_dn_skeleton_impl.h
//
// Purpose     : X509 domain number skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509_DN_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509_DN_SKELETON_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/x509/x509_dn.h"
#include "x509dn_skeleton.h"

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
class X509DnSkeletonImpl : public ara::crypto::ipccom::x509::x509dn::skeleton::X509DNSkeleton
{
 public:
  /// @brief Constructor for X509DnSkeletonImpl class.
  X509DnSkeletonImpl(
      CryptoServiceInstanceId const instanceId, ara::crypto::x509::X509DN::Uptr x509DnUptr) noexcept;

  /// @brief Definition for ExportPublicly.
  ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::ExportPubliclyOutput> ExportPublicly(
      const uint32& formatId) override;

  /// @brief Definition for GetAttribute.
  ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::GetAttributeOutput> GetAttribute(
      const uint64& id) override;

  /// @brief Definition for GetAttributeWithIndex.
  ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::GetAttributeWithIndexOutput>
      GetAttributeWithIndex(const uint32& id, const uint64& index) override;

  /// @brief Definition for GetDnString.
  ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::GetDnStringOutput> GetDnString() override;

  /// @brief Definition for MyProvider.
  ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::MyProviderOutput> MyProvider() override;

  /// @brief Definition for SetAttribute.
  ara::core::Future<void> SetAttribute(const uint32& attributeId, const String& attribute) override;

  /// @brief Definition for SetAttributeWithIndex.
  ara::core::Future<void> SetAttributeWithIndex(
      const uint32& attributeId, const uint64& index, const String& attribute) override;

  /// @brief Definition for SetDn.
  ara::core::Future<void> SetDn(const String& dn) override;

  /// @brief Definition for GetX509DnUptrObject.
  ara::crypto::x509::X509DN::Uptr GetX509DnUptrObject()
  {
    return std::move(x509DnUptr_);
  }

 private:
  /// @brief Unique pointer to X509 Domain Number ctx.
  ara::crypto::x509::X509DN::Uptr x509DnUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{
      ara::log::CreateLogger("X9DN", "X509 Domain Number", ara::log::LogLevel::kVerbose)};
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_X509_DN_SKELETON_IMPL_H_
