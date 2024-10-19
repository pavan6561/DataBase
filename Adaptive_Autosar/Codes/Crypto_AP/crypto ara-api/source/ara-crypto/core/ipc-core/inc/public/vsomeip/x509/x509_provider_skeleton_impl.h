// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_provider_skeleton_impl.h
//
// Purpose     : X509 provider skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_PROVIDER_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_PROVIDER_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/map.h>
#include <ara/log/logging.h>
#include <mutex>

#include "ara/crypto/x509/x509_provider.h"
#include "crypto_config.h"
#include "x509_dn_skeleton_impl.h"
#include "x509provider_skeleton.h"
#include "certificate_skeleton_impl.h"

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
class X509ProviderSkeletonImpl : public ara::crypto::ipccom::x509::skeleton::X509ProviderSkeleton
{
 public:
  /// @brief to Skeleton for easy of usage.
  using Skeleton = ara::crypto::ipccom::x509::skeleton::X509ProviderSkeleton;

  /// @brief X509ProviderSkeletonImpl class constructor.
  explicit X509ProviderSkeletonImpl(CryptoServiceInstanceId instance_id,
      std::shared_ptr<ara::crypto::config::CryptoConfig>& data,
      ara::crypto::x509::X509Provider::Uptr x509ProPtr) noexcept(false);

  /// @brief X509ProviderSkeletonImpl class distructor.
  virtual ~X509ProviderSkeletonImpl() override = default;

  /// @brief Definition for BuildDn.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::BuildDnOutput> BuildDn(
      const String& dn) override;

  /// @brief Definition for CheckCertStatus.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CheckCertStatusOutput> CheckCertStatus(
      const CryptoServiceInstanceId& certInstanceId,
      const CryptoServiceInstanceId& ocspResponseInstanceId) override;

  /// @brief Definition for CheckCertStatusWithCertList.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CheckCertStatusWithCertListOutput>
      CheckCertStatusWithCertList(const ByteVectorCertType& certInstanceId,
          const CryptoServiceInstanceId& ocspResponseInstanceId) override;

  /// @brief Definition for CleanupVolatileStorage.
  ara::core::Future<void> CleanupVolatileStorage() override;

  /// @brief Definition for CountCertsInChain.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CountCertsInChainOutput> CountCertsInChain(
      const CryptoReadOnlyMemRegion& certChain, const uint32& formatId) override;

  /// @brief Definition for CreateCertSignRequest.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateCertSignRequestOutput>
      CreateCertSignRequest(const CryptoServiceInstanceId& signerCtx,
          const CryptoReadOnlyMemRegion& derSubjectDN, const CryptoReadOnlyMemRegion& x509Extensions,
          const uint64& version) override;

  /// @brief Definition for CreateEmptyDn.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateEmptyDnOutput> CreateEmptyDn(
      const uint64& capacity) override;

  /// @brief Definition for CreateEmptyExtensions.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateEmptyExtensionsOutput>
      CreateEmptyExtensions(const uint64& capacity) override;

  /// @brief Definition for CreateOcspRequest.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateOcspRequestOutput> CreateOcspRequest(
      const CryptoServiceInstanceId& certInstanceId,
      const CryptoServiceInstanceId& signerInstanceId) override;

  /// @brief Definition for CreateOcspRequestWithCertList.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateOcspRequestWithCertListOutput>
      CreateOcspRequestWithCertList(const ByteVectorCertType& certListInstanceId,
          const CryptoServiceInstanceId& signerInstanceId) override;

  /// @brief Definition for DecodeDn.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::DecodeDnOutput> DecodeDn(
      const CryptoReadOnlyMemRegion& dn, const uint32& formatId) override;

  /// @brief Definition for FindCertByDn.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::FindCertByDnOutput> FindCertByDn(
      const CryptoServiceInstanceId& subjectDnInstanceId, const CryptoServiceInstanceId& issuerDnInstanceId,
      const Int64& validityTimePoint) override;

  /// @brief Definition for FindCertByKeyIds.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::FindCertByKeyIdsOutput> FindCertByKeyIds(
      const CryptoReadOnlyMemRegion& subjectKeyId, const CryptoReadOnlyMemRegion& authorityKeyId) override;

  /// @brief Definition for FindCertBySn.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::FindCertBySnOutput> FindCertBySn(
      const CryptoReadOnlyMemRegion& sn, const CryptoServiceInstanceId& issuerDnInstanceId) override;

  /// @brief Definition for Import.
  ara::core::Future<void> Import(
      const CryptoServiceInstanceId& certInstanceId, const String& iSpecify) override;

  /// @brief Definition for ImportCrl.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ImportCrlOutput> ImportCrl(
      const CryptoReadOnlyMemRegion& crl) override;

  /// @brief Definition for LoadCertificate.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::LoadCertificateOutput> LoadCertificate(
      const String& iSpecifyInstanceId) override;

  /// @brief Definition for ParseCert.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseCertOutput> ParseCert(
      const CryptoReadOnlyMemRegion& cert, const uint32& formatId) override;

  /// @brief Definition for ParseCertChain.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseCertChainOutput> ParseCertChain(
      const uint32& formatId, const CryptoReadOnlyMemRegion& certChain) override;

  /// @brief Definition for ParseCertChainConstReadOnlyMemRegion.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseCertChainVectorReadOnlyMemRegionOutput>
      ParseCertChainVectorReadOnlyMemRegion(const ByteVectorReadOnlyMemRegion&, const uint32&) override;

  /// @brief Definition for ParseCertSignRequest.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseCertSignRequestOutput> ParseCertSignRequest(
      const CryptoReadOnlyMemRegion& csr, const Boolean& withMetaData) override;

  /// @brief Definition for ParseOcspResponse.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseOcspResponseOutput> ParseOcspResponse(
      const CryptoReadOnlyMemRegion& response) override;

  /// @brief Definition for Remove.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::RemoveOutput> Remove(
      const CryptoServiceInstanceId& certInstanceId) override;

  /// @brief Definition for SetAsRootOfTrust.
  ara::core::Future<void> SetAsRootOfTrust(const CryptoServiceInstanceId& caCertInstanceId) override;

  /// @brief Definition for SetPendingStatus.
  ara::core::Future<void> SetPendingStatus(const CryptoServiceInstanceId& csrInstanceId) override;

  /// @brief Definition for VerifyCert.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::VerifyCertOutput> VerifyCert(
      const CryptoServiceInstanceId& certInstanceId,
      const CryptoServiceInstanceId& myRootInstanceId) override;

  /// @brief Definition for VerifyCertChain.
  ara::core::Future<ara::crypto::ipccom::x509::X509Provider::VerifyCertChainOutput> VerifyCertChain(
      const ByteVectorCertType& chain, const CryptoServiceInstanceId& myRootInstanceId) override;

 private:
  /// @brief shared pointer to the configuration.
  std::shared_ptr<ara::crypto::config::CryptoConfig> const configData_;

  /// @brief Pointer to hold the crypto provider instance.
  ara::crypto::x509::X509Provider::Uptr x509ProviderPtr_;

  /// @brief map for instance id to X509Dn instance mapping.
  ara::core::Map<CryptoServiceInstanceId, std::unique_ptr<ara::crypto::x509::CertificateSkeletonImpl>>
      x509CertificateContainer_{};

  /// @brief mutex for instance Id.
  std::mutex mutex_instanceId_{};

  /// @brief Instance Id for crypto provider service.
  std::uint16_t serviceInstanceId_{0};

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{
      ara::log::CreateLogger("XPSI", "X509 PROVIDER Context", ara::log::LogLevel::kVerbose)};

  /// @brief Method to generate crypto service instance Id.
  CryptoServiceInstanceId GenerateCryptoServiceInstanceId() noexcept;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_X509_PROVIDER_SKELETON_IMPL_H_
