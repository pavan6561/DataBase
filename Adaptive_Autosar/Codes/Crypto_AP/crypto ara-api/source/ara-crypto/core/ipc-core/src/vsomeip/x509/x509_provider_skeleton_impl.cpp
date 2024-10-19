// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_provider_skeleton_impl.cpp
//
// Purpose     : X509 service skeleton implementation to forward functional calls to crypto daemon.
// -----------------------------------------------------------------------------------------------------------

#include "x509_provider_skeleton_impl.h"
#include "x509dn_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace x509
{
X509ProviderSkeletonImpl::X509ProviderSkeletonImpl(CryptoServiceInstanceId instance_id,
    std::shared_ptr<ara::crypto::config::CryptoConfig>& data,
    ara::crypto::x509::X509Provider::Uptr x509ProPtr) noexcept(false) :
    ara::crypto::ipccom::x509::skeleton::X509ProviderSkeleton(ara::com::InstanceIdentifier(instance_id)),
    configData_{data}, x509ProviderPtr_{std::move(x509ProPtr)}, serviceInstanceId_{0}
{
  logger_.LogDebug() << "X509ProviderSkeletonImpl constructor";
}

CryptoServiceInstanceId X509ProviderSkeletonImpl::GenerateCryptoServiceInstanceId() noexcept
{
  // Generate new instance Id for crypto service.
  logger_.LogDebug() << "In X509ProviderSkeletonImpl::GenerateCryptoServiceInstanceId()";
  std::lock_guard<std::mutex> const lock{mutex_instanceId_};
  serviceInstanceId_ = serviceInstanceId_ + 1;
  CryptoServiceInstanceId strInstance{"SOME/IP:"};
  CryptoServiceInstanceId const tempStr{std::to_string(static_cast<uint32_t>(serviceInstanceId_))};
  strInstance.append(tempStr);
  return strInstance;
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::BuildDnOutput> X509ProviderSkeletonImpl::BuildDn(
    const String& dn)
{
  ara::core::Future<BuildDnOutput>::PromiseType promise;
  static_cast<void>(dn);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CheckCertStatusOutput>
    X509ProviderSkeletonImpl::CheckCertStatus(
        const CryptoServiceInstanceId& certInstanceId, const CryptoServiceInstanceId& ocspResponseInstanceId)
{
  ara::core::Future<CheckCertStatusOutput>::PromiseType promise;
  static_cast<void>(certInstanceId);
  static_cast<void>(ocspResponseInstanceId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CheckCertStatusWithCertListOutput>
    X509ProviderSkeletonImpl::CheckCertStatusWithCertList(
        const ByteVectorCertType& certInstanceId, const CryptoServiceInstanceId& ocspResponseInstanceId)
{
  ara::core::Future<CheckCertStatusWithCertListOutput>::PromiseType promise;
  static_cast<void>(certInstanceId);
  static_cast<void>(ocspResponseInstanceId);
  return promise.get_future();
}

ara::core::Future<void> X509ProviderSkeletonImpl::CleanupVolatileStorage()
{
  logger_.LogDebug() << "IPC-CORE:X509-Provider CleanupVolatileStorage()";
  ara::core::Future<void>::PromiseType promise;
  x509ProviderPtr_->CleanupVolatileStorage();
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CountCertsInChainOutput>
    X509ProviderSkeletonImpl::CountCertsInChain(
        const CryptoReadOnlyMemRegion& certChain, const uint32& formatId)
{
  logger_.LogDebug() << "IPC-CORE:X509-Provider CountCertsInChain()";
  ara::core::Future<CountCertsInChainOutput>::PromiseType promise;
  ReadOnlyMemRegion const certReadOnlyMem{certChain};
  ara::core::Result<std::size_t> const certUptrResult{
      x509ProviderPtr_->CountCertsInChain(certReadOnlyMem, formatId)};
  if (certUptrResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:X509-Provider CountCertsInChain() -> successful";
    ara::crypto::ipccom::x509::X509Provider::CountCertsInChainOutput certOut;
    certOut.size_t = certUptrResult.Value();
    promise.set_value(certOut);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:X509-Provider ParseCert() Error: " << certUptrResult.Error().Message();
    promise.SetError(
        static_cast<ipccom::common::error::CryptoErrorDomainErrc>(certUptrResult.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateCertSignRequestOutput>
    X509ProviderSkeletonImpl::CreateCertSignRequest(const CryptoServiceInstanceId& signerCtx,
        const CryptoReadOnlyMemRegion& derSubjectDN, const CryptoReadOnlyMemRegion& x509Extensions,
        const uint64& version)
{
  static_cast<void>(signerCtx);
  static_cast<void>(derSubjectDN);
  static_cast<void>(x509Extensions);
  static_cast<void>(version);
  ara::core::Future<CreateCertSignRequestOutput>::PromiseType promise;
  // signerCtx
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateEmptyDnOutput>
    X509ProviderSkeletonImpl::CreateEmptyDn(const uint64& capacity)
{
  logger_.LogDebug() << "IPC-CORE:X509-Provider CreateEmptyDn()";
  ara::core::Future<CreateEmptyDnOutput>::PromiseType promise;
  ara::core::Result<X509DN::Uptr> dnUptr{x509ProviderPtr_->CreateEmptyDn(capacity)};
  std::shared_ptr<ara::crypto::common::datastorage::X509DnContainer> dnContainer{
      ara::crypto::common::datastorage::X509DnContainer::GetInstance()};
  CryptoServiceInstanceId localInstId{dnContainer->GetCryptoServiceInstanceId()};
  std::unique_ptr<ara::crypto::x509::X509DnSkeletonImpl> x509DnSkelImplUptr{
      std::make_unique<ara::crypto::x509::X509DnSkeletonImpl>(localInstId, std::move(dnUptr).Value())};
  x509DnSkelImplUptr->OfferService();
  dnContainer->StoreObjectToContanier(localInstId, std::move(x509DnSkelImplUptr));
  CreateEmptyDnOutput dnOutput;
  dnOutput.instanceId = localInstId;
  promise.set_value(dnOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateEmptyExtensionsOutput>
    X509ProviderSkeletonImpl::CreateEmptyExtensions(const uint64& capacity)
{
  ara::core::Future<CreateEmptyExtensionsOutput>::PromiseType promise;
  static_cast<void>(capacity);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateOcspRequestOutput>
    X509ProviderSkeletonImpl::CreateOcspRequest(
        const CryptoServiceInstanceId& certInstanceId, const CryptoServiceInstanceId& signerInstanceId)
{
  ara::core::Future<CreateOcspRequestOutput>::PromiseType promise;
  static_cast<void>(certInstanceId);
  static_cast<void>(signerInstanceId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::CreateOcspRequestWithCertListOutput>
    X509ProviderSkeletonImpl::CreateOcspRequestWithCertList(
        const ByteVectorCertType& certListInstanceId, const CryptoServiceInstanceId& signerInstanceId)
{
  ara::core::Future<CreateOcspRequestWithCertListOutput>::PromiseType promise;
  static_cast<void>(certListInstanceId);
  static_cast<void>(signerInstanceId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::DecodeDnOutput> X509ProviderSkeletonImpl::DecodeDn(
    const CryptoReadOnlyMemRegion& dn, const uint32& formatId)
{
  ara::core::Future<DecodeDnOutput>::PromiseType promise;
  static_cast<void>(dn);
  static_cast<void>(formatId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::FindCertByDnOutput>
    X509ProviderSkeletonImpl::FindCertByDn(const CryptoServiceInstanceId& subjectDnInstanceId,
        const CryptoServiceInstanceId& issuerDnInstanceId, const Int64& validityTimePoint)
{
  logger_.LogDebug() << "IPC-CORE:X509-Provider FindCertByDn()";
  ara::core::Future<FindCertByDnOutput>::PromiseType promise;
  std::shared_ptr<ara::crypto::common::datastorage::X509DnContainer> dnContainer{
      ara::crypto::common::datastorage::X509DnContainer::GetInstance()};

  // logic to get X509Dn::uptr for subjectDN
  std::unique_ptr<X509DnSkeletonImpl>& subjectDnSkelRef{
      dnContainer->GetObjectFromContanier(subjectDnInstanceId)};
  ara::crypto::x509::X509DN::Uptr x509DnSubjectDnRef{subjectDnSkelRef->GetX509DnUptrObject()};

  // logic to get x509Dn::uptr for IssuerDN
  std::unique_ptr<X509DnSkeletonImpl>& issuerDnSkelRef{
      dnContainer->GetObjectFromContanier(issuerDnInstanceId)};
  ara::crypto::x509::X509DN::Uptr x509DnIssuerDnRef{issuerDnSkelRef->GetX509DnUptrObject()};

  std::size_t timePoint{0};
  Certificate::Uptrc certUptr{
      x509ProviderPtr_->FindCertByDn(*x509DnSubjectDnRef, *x509DnIssuerDnRef, validityTimePoint, timePoint)};
  FindCertByDnOutput dnResult;
  if (certUptr != nullptr)
  {
    CryptoServiceInstanceId const localCertInstanceId{GenerateCryptoServiceInstanceId()};
    logger_.LogDebug() << "IPC-CORE:X509-Provider FindCertByDn(), Certificate::Uptr is not nullptr";
    logger_.LogDebug() << "IPC-CORE:X509-Provider FindCertByDn(), Crypto Service InstanceId = "
                       << localCertInstanceId;
    // Note: need further analysis
    dnResult.reslutCertInstanceId = localCertInstanceId;
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE:X509-Provider FindCertByDn(), Certificate::Uptr is nullptr";
  }
  promise.set_value(dnResult);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::FindCertByKeyIdsOutput>
    X509ProviderSkeletonImpl::FindCertByKeyIds(
        const CryptoReadOnlyMemRegion& subjectKeyId, const CryptoReadOnlyMemRegion& authorityKeyId)
{
  ara::core::Future<FindCertByKeyIdsOutput>::PromiseType promise;
  FindCertByKeyIdsOutput findCertByKeyIdOutput;

  Certificate::Uptrc certUptr{x509ProviderPtr_->FindCertByKeyIds(subjectKeyId, authorityKeyId)};
  if (certUptr != nullptr)
  {
    CryptoServiceInstanceId localCertInstanceId{GenerateCryptoServiceInstanceId()};
    // Note: need further analysis
    findCertByKeyIdOutput.reslutCertInstanceId = localCertInstanceId;
  }
  promise.set_value(findCertByKeyIdOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::FindCertBySnOutput>
    X509ProviderSkeletonImpl::FindCertBySn(
        const CryptoReadOnlyMemRegion& sn, const CryptoServiceInstanceId& issuerDnInstanceId)
{
  ara::core::Future<FindCertBySnOutput>::PromiseType promise;
  FindCertBySnOutput findCertBysnOutput;

  std::shared_ptr<ara::crypto::common::datastorage::X509DnContainer> dnContainer{
      ara::crypto::common::datastorage::X509DnContainer::GetInstance()};
  // logic to get X509Dn::uptr for subjectDN
  std::unique_ptr<X509DnSkeletonImpl>& x509SklImpluptr
      {dnContainer->GetObjectFromContanier(issuerDnInstanceId)};
  ara::crypto::x509::X509DN::Uptr x509DnSubjectDnRef{x509SklImpluptr->GetX509DnUptrObject()};

  Certificate::Uptrc certUptr{x509ProviderPtr_->FindCertBySn(sn, *x509DnSubjectDnRef)};
  if (certUptr != nullptr)
  {
    logger_.LogDebug() << "IPC-CORE:X509-Provider Import() -> successful";
    CryptoServiceInstanceId localCertInstanceId{GenerateCryptoServiceInstanceId()};

    // Note: need further analysis
    findCertBysnOutput.reslutCertInstanceId = localCertInstanceId;
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE:X509-Provider Import() -> nullptr";
  }
  return promise.get_future();
}

ara::core::Future<void> X509ProviderSkeletonImpl::Import(
    const CryptoServiceInstanceId& certInstanceId, const String& iSpecify)
{
  ara::core::Future<void>::PromiseType promise;
  Certificate::Uptr& certUptr{x509CertificateContainer_[certInstanceId]->GetCertificateUptrInstance()};
  ara::core::Result<void> importResult;
  ara::core::InstanceSpecifier const instSpecify{iSpecify};
  importResult = x509ProviderPtr_->Import(*certUptr, instSpecify);

  if (importResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:X509-Provider Import() -> successful";
    promise.set_value();
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE:X509-Provider Import() Error: " << importResult.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(importResult.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ImportCrlOutput>
    X509ProviderSkeletonImpl::ImportCrl(const CryptoReadOnlyMemRegion& crl)
{
  ara::core::Future<ImportCrlOutput>::PromiseType promise;
  static_cast<void>(crl);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::LoadCertificateOutput>
    X509ProviderSkeletonImpl::LoadCertificate(const String& iSpecifyInstanceId)
{
  ara::core::Future<LoadCertificateOutput>::PromiseType promise;
  static_cast<void>(iSpecifyInstanceId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseCertOutput>
    X509ProviderSkeletonImpl::ParseCert(const CryptoReadOnlyMemRegion& cert, const uint32& formatId)
{
  ara::core::Future<ParseCertOutput>::PromiseType promise;
  ReadOnlyMemRegion const certReadOnlyMem{cert};
  ara::core::Result<Certificate::Uptr> certUptrResult{x509ProviderPtr_->ParseCert(certReadOnlyMem, formatId)};
  if (certUptrResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:X509-Provider ParseCert() -> successful";
    ara::crypto::ipccom::x509::X509Provider::ParseCertOutput certOut;
    CryptoServiceInstanceId const certLocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<CertificateSkeletonImpl> localCertUptr{
        std::make_unique<CertificateSkeletonImpl>(certLocalInstanceId, std::move(certUptrResult).Value())};
    localCertUptr->OfferService();
    x509CertificateContainer_.emplace(certLocalInstanceId, std::move(localCertUptr));
    certOut.instanceId = certLocalInstanceId;
    promise.set_value(certOut);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:X509-Provider ParseCert() Error: " << certUptrResult.Error().Message();
    promise.SetError(
        static_cast<ipccom::common::error::CryptoErrorDomainErrc>(certUptrResult.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseCertChainOutput>
    X509ProviderSkeletonImpl::ParseCertChain(const uint32& formatId, const CryptoReadOnlyMemRegion& certChain)
{
  logger_.LogDebug() << "IPC-CORE:X509-Provider ParseCertChain()";
  ara::core::Future<ParseCertChainOutput>::PromiseType promise;
  ara::core::Vector<Certificate::Uptr> vtrCertUptr;
  ReadOnlyMemRegion const certChainReadOnlyMem{certChain};

  ara::core::Result<void> const certUptrResult{
      x509ProviderPtr_->ParseCertChain(vtrCertUptr, certChainReadOnlyMem, formatId)};
  if (certUptrResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:X509-Provider ParseCertChain() -> successful";
    ara::crypto::ipccom::x509::X509Provider::ParseCertChainOutput certChainOut;
    logger_.LogDebug() << "IPC-CORE: vtrCertUptr size -> " << vtrCertUptr.size();
    ByteVectorCertType vtrInstanceId;
    for (uint8_t i{0}; i < vtrCertUptr.size(); i++)
    {
      CryptoServiceInstanceId certLocalInstanceId{GenerateCryptoServiceInstanceId()};
      logger_.LogDebug() << "IPC-CORE: ServiceInstance ID -> " << certLocalInstanceId;
      Certificate::Uptr localCert = std::move(vtrCertUptr[i]);
      std::unique_ptr<CertificateSkeletonImpl> localCertUptr{
          std::make_unique<CertificateSkeletonImpl>(certLocalInstanceId, std::move(localCert))};
      localCertUptr->OfferService();
      x509CertificateContainer_.emplace(certLocalInstanceId, std::move(localCertUptr));
      vtrInstanceId.emplace_back(certLocalInstanceId);
    }
    // Todo :: As ara::com having issue with communication of vector<String> datatypes, using concatinated
    // string variable itself
    uint8_t temp = 1;
    for (auto &it: vtrInstanceId)
    {
      logger_.LogDebug() << "IPC-CORE: ServiceInstanceId: " << it;
      certChainOut.outcome.append(it);
      if (temp < vtrInstanceId.size())
      {
        certChainOut.outcome += "at";
      }
      temp++;
    }
    logger_.LogDebug() << "IPC-CORE: ParseCertChain(), Concatinated CryptoServiceInstanceId = "
        << certChainOut.outcome;
    promise.set_value(certChainOut);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:X509-Provider ParseCertChain() Error: "
                       << certUptrResult.Error().Message();
    promise.SetError(
        static_cast<ipccom::common::error::CryptoErrorDomainErrc>(certUptrResult.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseCertChainVectorReadOnlyMemRegionOutput>
    X509ProviderSkeletonImpl::ParseCertChainVectorReadOnlyMemRegion(
        const ByteVectorReadOnlyMemRegion& certChain, const uint32& formatId)
{
  logger_.LogDebug() << "IPC-CORE:X509-Provider ParseCertChainVectorReadOnlyMemRegion()";
  ara::core::Future<ParseCertChainVectorReadOnlyMemRegionOutput>::PromiseType promise;
  ara::core::Vector<Certificate::Uptr> vtrCertUptr;
  ara::core::Vector<ReadOnlyMemRegion> vtrReadOnlyMem;
  logger_.LogDebug() << "IPC-CORE::ParseCertChainVectorReadOnlyMemRegion() cert size = " << certChain.size();
  for (uint8_t i{0}; i < certChain.size(); i++)
  {
    vtrReadOnlyMem.push_back(certChain[i]);
  }
  ara::core::Result<void> const certUptrResult{
      x509ProviderPtr_->ParseCertChain(vtrCertUptr, vtrReadOnlyMem, formatId)};
  if (certUptrResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:X509-Provider ParseCertChainVectorReadOnlyMemRegion() -> successful";
    ara::crypto::ipccom::x509::X509Provider::ParseCertChainVectorReadOnlyMemRegionOutput certChainOut;
    ByteVectorCertType vtrInstanceId;
    for (uint8_t i{0}; i < vtrCertUptr.size(); i++)
    {
      CryptoServiceInstanceId certLocalInstanceId{GenerateCryptoServiceInstanceId()};
      std::unique_ptr<CertificateSkeletonImpl> localCertUptr{
          std::make_unique<CertificateSkeletonImpl>(certLocalInstanceId, std::move(vtrCertUptr[i]))};
      localCertUptr->OfferService();
      x509CertificateContainer_.emplace(certLocalInstanceId, std::move(localCertUptr));
      vtrInstanceId.emplace_back(certLocalInstanceId);
    }
    // Todo :: As ara::com having issue with communication of vector<String> datatypes, using concatinated
    // string variable itself
    uint8_t temp = 1;
    for (auto &it: vtrInstanceId)
    {
      logger_.LogDebug() << "IPC-CORE: ServiceInstanceId: " << it;
      certChainOut.outcome.append(it);
      if (temp < vtrInstanceId.size())
      {
        certChainOut.outcome += "at";
      }
      temp++;
    }
    logger_.LogDebug() << "IPC-CORE: ParseCertChainVectorReadOnlyMemRegion(), Concatinated InstanceId = " 
        << certChainOut.outcome;
    promise.set_value(certChainOut);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:X509-Provider ParseCertChainVectorReadOnlyMemRegion() Error: "
                       << certUptrResult.Error().Message();
    promise.SetError(
        static_cast<ipccom::common::error::CryptoErrorDomainErrc>(certUptrResult.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseCertSignRequestOutput>
    X509ProviderSkeletonImpl::ParseCertSignRequest(
        const CryptoReadOnlyMemRegion& csr, const Boolean& withMetaData)
{
  ara::core::Future<ParseCertSignRequestOutput>::PromiseType promise;
  ParseCertSignRequestOutput parseCertReqOutput;

  ara::core::Result<CertSignRequest::Uptrc> const certUptrResult{
      x509ProviderPtr_->ParseCertSignRequest(csr, withMetaData)};
  if (certUptrResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:Provider ParseCertSignRequest() -> successful";
    CryptoServiceInstanceId certLocalInstanceId{GenerateCryptoServiceInstanceId()};
    // Note: need further implementation in future
    parseCertReqOutput.instanceId = certLocalInstanceId;
    promise.set_value(parseCertReqOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:X509-Provider ParseCertSignRequest() Error: "
                       << certUptrResult.Error().Message();
    promise.SetError(
        static_cast<ipccom::common::error::CryptoErrorDomainErrc>(certUptrResult.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::ParseOcspResponseOutput>
    X509ProviderSkeletonImpl::ParseOcspResponse(const CryptoReadOnlyMemRegion& response)
{
  static_cast<void>(response);
  ara::core::Future<ParseOcspResponseOutput>::PromiseType promise;
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::RemoveOutput> X509ProviderSkeletonImpl::Remove(
    const CryptoServiceInstanceId& certInstanceId)
{
  logger_.LogDebug() << "IPC-CORE:Provider Remove() -> successful";
  ara::core::Future<RemoveOutput>::PromiseType promise;
  Certificate::Uptr certUptr{x509CertificateContainer_[certInstanceId]->GetCertificateUptr()};
  x509CertificateContainer_[certInstanceId]->StopOfferService();
  x509CertificateContainer_.erase(certInstanceId);
  RemoveOutput removeOutputRet;
  removeOutputRet.result = x509ProviderPtr_->Remove(std::move(certUptr));
  promise.set_value(removeOutputRet);
  return promise.get_future();
}

ara::core::Future<void> X509ProviderSkeletonImpl::SetAsRootOfTrust(
    const CryptoServiceInstanceId& caCertInstanceId)
{
  ara::core::Future<void>::PromiseType promise;
  Certificate::Uptr const certUptr{x509CertificateContainer_[caCertInstanceId]->GetCertificateUptr()};

  ara::core::Result<void> const retValue{x509ProviderPtr_->SetAsRootOfTrust(*certUptr)};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:Provider SetAsRootOfTrust() -> successful";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE:X509-Provider SetAsRootOfTrust() Error: " << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<void> X509ProviderSkeletonImpl::SetPendingStatus(
    const CryptoServiceInstanceId& csrInstanceId)
{
  ara::core::Future<void>::PromiseType promise;
  static_cast<void>(csrInstanceId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::VerifyCertOutput>
    X509ProviderSkeletonImpl::VerifyCert(
        const CryptoServiceInstanceId& certInstanceId, const CryptoServiceInstanceId& myRootInstanceId)
{
  logger_.LogDebug() << "IPC-CORE:X509-Provider VerifyCert()";
  ara::core::Future<VerifyCertOutput>::PromiseType promise;
  logger_.LogDebug() << "IPC-CORE:VerifyCert(), certificate instance Id = " << certInstanceId;
  Certificate::Uptr& cert{x509CertificateContainer_[certInstanceId]->GetCertificateUptrInstance()};
  // Certificate::Uptr to be collected from container, offered service for that instance od certificate need
  // to be stoped and its instance need to erased.
  Certificate::Status certStatus;
  if (myRootInstanceId.empty() == false)
  {
    logger_.LogDebug() << "IPC-CORE:VerifyCert(), root certificate instance Id = " << myRootInstanceId;
    Certificate::Uptr rootCertUptr{x509CertificateContainer_[myRootInstanceId]->GetCertificateUptr()};
    x509CertificateContainer_[myRootInstanceId]->StopOfferService();
    x509CertificateContainer_.erase(myRootInstanceId);
    certStatus = x509ProviderPtr_->VerifyCert(*cert, std::move(rootCertUptr));
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE:VerifyCert(), root certificate is nullptr";
    certStatus = x509ProviderPtr_->VerifyCert(*cert);
  }

  logger_.LogDebug() << "IPC-CORE:X509-Provider VerifyCert() -> status: "
                     << static_cast<uint32_t>(certStatus);
  VerifyCertOutput verifyResult;
  verifyResult.result = static_cast<uint32_t>(certStatus);
  promise.set_value(verifyResult);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::X509Provider::VerifyCertChainOutput>
    X509ProviderSkeletonImpl::VerifyCertChain(
        const ByteVectorCertType& chain, const CryptoServiceInstanceId& myRootInstanceId)
{
  logger_.LogDebug() << "IPC-CORE:X509-Provider VerifyCertChain()";
  ara::core::Future<VerifyCertChainOutput>::PromiseType promise;
  ara::core::Vector<Certificate::Uptr> spanCert;
  for (uint8_t i{0}; i < chain.size(); i++)
  {
    logger_.LogDebug() << "IPC-CORE:VerifyCertChain() SerivceImstanceId = " << chain[i];
    // Certificate::Uptr to be collected from container, offered service for that instance od certificate need
    // to be stoped and its instance need to erased.
    spanCert.push_back(std::move(x509CertificateContainer_[chain[i]]->GetCertificateUptr()));
    x509CertificateContainer_[chain[i]]->StopOfferService();
    x509CertificateContainer_.erase(chain[i]);
  }

  Certificate::Status certStatus;
  if ( myRootInstanceId.empty() == false)
  {
    Certificate::Uptr rootCertUptr{
      std::move(x509CertificateContainer_[myRootInstanceId]->GetCertificateUptr())};
    x509CertificateContainer_[myRootInstanceId]->StopOfferService();
    x509CertificateContainer_.erase(myRootInstanceId);
    certStatus = x509ProviderPtr_->VerifyCertChain(spanCert, std::move(rootCertUptr));
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE:VerifyCertChain(), rootcertificate parameter is nullptr";
    certStatus = x509ProviderPtr_->VerifyCertChain(spanCert);
  }

  logger_.LogDebug() << "IPC-CORE:X509-Provider VerifyCertChain() -> status: "
                     << static_cast<uint32_t>(certStatus);
  VerifyCertChainOutput verifyResult;
  verifyResult.result = static_cast<uint32_t>(certStatus);
  promise.set_value(verifyResult);
  return promise.get_future();
}

} // namespace x509
} // namespace crypto
} // namespace ara
