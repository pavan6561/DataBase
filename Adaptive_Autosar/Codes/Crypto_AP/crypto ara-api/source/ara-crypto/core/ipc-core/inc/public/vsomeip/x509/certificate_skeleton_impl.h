// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certificate_skeleton_impl.h
//
// Purpose     : Certificate skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_CERTIFICATE_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_CERTIFICATE_SKELETON_IMPL_H_

#include <ara/log/logging.h>
#include "certificate_skeleton.h"
#include "ara/crypto/x509/certificate.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
/// @brief Class definition for certificate skeleton implementation.
class CertificateSkeletonImpl : public ara::crypto::ipccom::x509::certificate::skeleton::CertificateSkeleton
{
 public:
  /// @brief Constructor for X509DnSkeletonImpl class.
  CertificateSkeletonImpl(
      CryptoServiceInstanceId const instanceId, ara::crypto::x509::Certificate::Uptr certificateUptr) noexcept;

  /// @brief Definition for AuthorityKeyId.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::AuthorityKeyIdOutput>
      AuthorityKeyId() override;

  /// @brief Definition for EndTime.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::EndTimeOutput> EndTime() override;

  /// @brief Definition for ExportPublicly.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::ExportPubliclyOutput> ExportPublicly(
      const uint32& formatId) override;

  /// @brief Definition for GetConstraints.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::GetConstraintsOutput>
      GetConstraints() override;

  /// @brief Definition for GetFingerprint.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::GetFingerprintOutput> GetFingerprint(
      const CryptoServiceInstanceId& hashCtxInstanceId,const uint32& fingerPrintSize) override;

  /// @brief Definition for GetPathLimit.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::GetPathLimitOutput>
      GetPathLimit() override;

  /// @brief Definition for GetStatus.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::GetStatusOutput>
      GetStatus() override;

  /// @brief Definition for IsCa.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::IsCaOutput> IsCa() override;

  /// @brief Definition for IsRoot.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::IsRootOutput> IsRoot() override;

  /// @brief Definition for IssuerDn.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::IssuerDnOutput> IssuerDn() override;

  /// @brief Definition for MyProvider.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::MyProviderOutput>
      MyProvider() override;

  /// @brief Definition for SerialNumber.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::SerialNumberOutput>
      SerialNumber() override;

  /// @brief Definition for StartTime.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::StartTimeOutput>
      StartTime() override;

  /// @brief Definition for SubjectDn.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::SubjectDnOutput>
      SubjectDn() override;

  /// @brief Definition for SubjectKeyId.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::SubjectKeyIdOutput>
      SubjectKeyId() override;

  /// @brief Definition for SubjectPubKey.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::SubjectPubKeyOutput> SubjectPubKey(
      const CryptoServiceInstanceId& cryptoProviderInstanceId) override;

  /// @brief Definition for VerifyMe.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::VerifyMeOutput> VerifyMe(
      const CryptoServiceInstanceId& caCertInstanceId) override;

  /// @brief Definition for X509Version.
  ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::X509VersionOutput>
      X509Version() override;

  /// @brief Definition for GetCertificateUptrInstance.
  ara::crypto::x509::Certificate::Uptr& GetCertificateUptrInstance()
  {
    return certUptr_;
  }

  /// @brief Definition for GetCertificateUptr.
  ara::crypto::x509::Certificate::Uptr GetCertificateUptr()
  {
    return std::move(certUptr_);
  }

 private:
  /// @brief To store the cryptoInstanceId_.
  CryptoServiceInstanceId cryptoInstanceId_;

  /// @brief To store the Uptr.
  ara::crypto::x509::Certificate::Uptr certUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{
      ara::log::CreateLogger("CERT", "Certificate Skeleton Impl", ara::log::LogLevel::kVerbose)};
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_CERTIFICATE_SKELETON_IMPL_H_
