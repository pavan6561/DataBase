// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certsignrequest_skeleton_impl.h
//
// Purpose     : X509 provider skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_CERTSIGNREQUEST_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_CERTSIGNREQUEST_SKELETON_IMPL_H_

#include <ara/log/logging.h>
#include "certsignrequest_skeleton.h"
#include "ara/crypto/x509/cert_sign_request.h" 

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
/// @brief class definition for Crypto Provider Skeleton.
class CertSignRequestSkeletonImpl : public ara::crypto::ipccom::x509::csr::skeleton::CertSignRequestSkeleton
{
 public:
  /// @brief Constructor for CertSignRequestSkeleton class.
  CertSignRequestSkeletonImpl(CryptoServiceInstanceId const instanceId,
      ara::crypto::x509::CertSignRequest::Uptr certSignUptr) noexcept;

  /// @brief CertSignRequestSkeletonImpl class distructor.
  virtual ~CertSignRequestSkeletonImpl() override = default;

  /// @brief Definition for ExportASN1CertSignRequestOutput.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::ExportASN1CertSignRequestOutput>
      ExportASN1CertSignRequest() override;

  /// @brief Definition for ExportPublicly.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::ExportPubliclyOutput> ExportPublicly(
      const CryptoServiceInstanceId& formatId) override;

  /// @brief Definition for GetConstraints.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::GetConstraintsOutput>
      GetConstraints() override;

  /// @brief Definition for GetPathLimit.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::GetPathLimitOutput>
      GetPathLimit() override;

  /// @brief Definition for GetSignature.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::GetSignatureOutput>
      GetSignature() override;

  /// @brief Definition for IsCaOutput.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::IsCaOutput> IsCa() override;

  /// @brief Definition for MyProvider.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::MyProviderOutput> MyProvider() override;

  /// @brief Definition for SubjectDn.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::SubjectDnOutput> SubjectDn() override;

  /// @brief Definition for SubjectPubKey.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::SubjectPubKeyOutput> SubjectPubKey(
      const CryptoServiceInstanceId& cryptoProviderInstanceId) override;

  /// @brief Definition for Verify.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::VerifyOutput> Verify() override;

  /// @brief Definition for Version.
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::VersionOutput> Version() override;

 private:
  /// @brief To store the cryptoInstanceId_.
  CryptoServiceInstanceId cryptoInstanceId_;

  /// @brief To store the Uptr.
  ara::crypto::x509::CertSignRequest::Uptr certSignUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{
      ara::log::CreateLogger("CSR", "CertSignRequest Skeleton Impl", ara::log::LogLevel::kVerbose)};
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_CERTSIGNREQUEST_SKELETON_IMPL_H_
