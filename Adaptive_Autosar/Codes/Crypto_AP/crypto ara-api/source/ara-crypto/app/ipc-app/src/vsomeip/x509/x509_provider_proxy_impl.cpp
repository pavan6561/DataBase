// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_provider_proxy_impl.cpp
//
// Purpose     : X509 provider proxy implementation to forward functional calls from application to
//               ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "x509_provider_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
X509ProviderProxyImpl::X509ProviderProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    logger_{ara::log::CreateLogger("CPPI", "X509 Provider Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "X509ProviderProxyImpl Constructor";
  logger_.LogDebug() << "Initialize server client connection for X509 Provider";
  Initialize(serviceInstanceID);
}

void X509ProviderProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for X509 Provider : Successful";
    x509ProviderHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for X509 Provider : Failed";
  }
}

ara::core::Result<X509DN::Uptr> X509ProviderProxyImpl::CreateEmptyDn(std::size_t capacity) noexcept
{
  logger_.LogDebug() << "IPC-APP: X509ProviderProxyImpl::" << __FUNCTION__;
  X509DN::Uptr dnUptr;
  auto const dnUptrResult{x509ProviderHandler_->CreateEmptyDn(capacity).GetResult()};
  if (dnUptrResult.HasValue())
  {
    CryptoServiceInstanceId instanceId{dnUptrResult.Value().instanceId};
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateEmptyDn : " << instanceId;
    dnUptr = std::make_unique<X509DNProxyImpl>(instanceId, *this);
  }
  else
  {
    logger_.LogError() << "IPC-APP: CreateEmptyDn() Error: " << dnUptrResult.Error().Message();
  }
  return ara::core::Result<X509DN::Uptr>::FromValue(std::move(dnUptr));
}

ara::core::Result<X509DN::Uptrc> X509ProviderProxyImpl::BuildDn(ara::core::StringView dn) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  ara::core::String dnStr{dn.data()};
  auto const dnUptrResult{x509ProviderHandler_->BuildDn(dnStr).GetResult()};
  if (dnUptrResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = dnUptrResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateEmptyDn : " << instanceId;
    X509DN::Uptrc dnUptrc{std::make_unique<X509DNProxyImpl>(instanceId, *this)};
    return ara::core::Result<X509DN::Uptrc>::FromValue(std::move(dnUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP: CreateEmptyDn() Error: " << dnUptrResult.Error().Message();
    return ara::core::Result<X509DN::Uptrc>::FromValue(nullptr);
  }
}

ara::core::Result<X509DN::Uptrc> X509ProviderProxyImpl::DecodeDn(
    ReadOnlyMemRegion dn, Serializable::FormatId formatId) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  CryptoReadOnlyMemRegion dnUint8t(dn.begin(), dn.end());
  auto const dnUptrResult{x509ProviderHandler_->DecodeDn(dnUint8t, formatId).GetResult()};
  if (dnUptrResult.HasValue())
  {
    CryptoServiceInstanceId const instanceId{dnUptrResult.Value().instanceId};
    logger_.LogDebug() << "IPC-APP Service InstanceId for DecodeDn : " << instanceId;
    X509DN::Uptrc dnUptrc{std::make_unique<X509DNProxyImpl>(instanceId, *this)};
    return ara::core::Result<X509DN::Uptrc>::FromValue(std::move(dnUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP: CreateEmptyDn() Error: " << dnUptrResult.Error().Message();
    return ara::core::Result<X509DN::Uptrc>::FromError(
        static_cast<SecurityErrorDomain::Errc>(dnUptrResult.Error().Value()));
  }
}

ara::core::Result<Certificate::Uptr> X509ProviderProxyImpl::ParseCert(
    ReadOnlyMemRegion cert, Serializable::FormatId formatId) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  CryptoReadOnlyMemRegion certUint8t(cert.begin(), cert.end());
  auto const certUptrResult{x509ProviderHandler_->ParseCert(certUint8t, formatId).GetResult()};
  if (certUptrResult.HasValue())
  {
    CryptoServiceInstanceId const instanceId{certUptrResult.Value().instanceId};
    logger_.LogDebug() << "IPC-APP Service InstanceId for DecodeDn : " << instanceId;
    Certificate::Uptr certUptr{std::make_unique<CertificateProxyImpl>(instanceId, *this)};
    return ara::core::Result<Certificate::Uptr>::FromValue(std::move(certUptr));
  }
  else
  {
    logger_.LogError() << "IPC-APP: CreateEmptyDn() Error: " << certUptrResult.Error().Message();
    return ara::core::Result<Certificate::Uptr>::FromError(
        static_cast<SecurityErrorDomain::Errc>(certUptrResult.Error().Value()));
  }
}

ara::core::Result<std::size_t> X509ProviderProxyImpl::CountCertsInChain(
    ReadOnlyMemRegion certChain, Serializable::FormatId formatId) const noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  CryptoReadOnlyMemRegion certUint8t(certChain.begin(), certChain.end());
  auto const certChainResult{x509ProviderHandler_->CountCertsInChain(certUint8t, formatId).GetResult()};
  if (certChainResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP CountCertsInChain() -> success";
    return ara::core::Result<std::size_t>::FromValue(certChainResult.Value().size_t);
  }
  else
  {
    logger_.LogError() << "IPC-APP: CountCertsInChain() Error: " << certChainResult.Error().Message();
    return ara::core::Result<std::size_t>::FromError(
        static_cast<SecurityErrorDomain::Errc>(certChainResult.Error().Value()));
  }
}

ara::core::Result<void> X509ProviderProxyImpl::ParseCertChain(ara::core::Vector<Certificate::Uptr>& outcome,
    ReadOnlyMemRegion certChain, Serializable::FormatId formatId) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  CryptoReadOnlyMemRegion certUint8t(certChain.begin(), certChain.end());
  auto const certChainResult{x509ProviderHandler_->ParseCertChain(formatId, certUint8t).GetResult()};
  if (certChainResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP ParseCertChain() -> success";
    CryptoServiceInstanceId serviceInstanceId{certChainResult.Value().outcome};
    logger_.LogDebug() << "IPC-APP ParseCertChain() -> CryptoServiceInstanceId's : " << serviceInstanceId;
    // Note : Temporary solution as vector<String> is not supported in ara::com communication
    ByteVectorCertType vrtCert;
    while (serviceInstanceId.empty() == false)
    {
      std::size_t pos = serviceInstanceId.find("at");
      if (pos != String::npos)
      {
        ara::core::String sub = serviceInstanceId.substr(0, pos);
        vrtCert.emplace_back(sub);
        serviceInstanceId = serviceInstanceId.substr(pos + 2);
      }
      else
      {
        vrtCert.emplace_back(serviceInstanceId);
        serviceInstanceId.clear();
      }
    }

    for (auto& it : vrtCert)
    {
      logger_.LogDebug() << "IPC-APP ParseCertChain() -> ServiceId = " << it;
      Certificate::Uptr certUptr{std::make_unique<CertificateProxyImpl>(it, *this)};
      outcome.emplace_back(std::move(certUptr));
    }
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP: ParseCertChain() Error: " << certChainResult.Error().Message();
    return ara::core::Result<void>::FromError(
        static_cast<SecurityErrorDomain::Errc>(certChainResult.Error().Value()));
  }
}

ara::core::Result<void> X509ProviderProxyImpl::ParseCertChain(ara::core::Vector<Certificate::Uptr>& outcome,
    const ara::core::Vector<ReadOnlyMemRegion>& certChain, Serializable::FormatId formatId) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  ara::core::Vector<CryptoReadOnlyMemRegion> vtrReadOnlyMem;

  logger_.LogDebug() << "IPC-APP::ParseCertChainVector() certChain size = " << certChain.size();
  for (auto& it : certChain)
  {
    CryptoReadOnlyMemRegion roMemory(it.begin(), it.end());
    vtrReadOnlyMem.emplace_back(roMemory);
  }
  auto const certChainResult{
      x509ProviderHandler_->ParseCertChainVectorReadOnlyMemRegion(vtrReadOnlyMem, formatId).GetResult()};
  if (certChainResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP ParseCertChain() -> success";
    CryptoServiceInstanceId ServiceInstanceId = certChainResult.Value().outcome;
    logger_.LogDebug() << "IPC-APP ParseCertChain() -> CryptoServiceInstanceId's : " << ServiceInstanceId;
    // Note : Temporary solution as vector<String> is not supported in ara::com communication
    ByteVectorCertType vrtCert;
    while (ServiceInstanceId.empty() == false)
    {
      std::size_t pos = ServiceInstanceId.find("at");
      if (pos != String::npos)
      {
        ara::core::String sub = ServiceInstanceId.substr(0, pos);
        vrtCert.emplace_back(sub);
        ServiceInstanceId = ServiceInstanceId.substr(pos + 2);
      }
      else
      {
        vrtCert.emplace_back(ServiceInstanceId);
        ServiceInstanceId.clear();
      }
    }

    for (auto& it : vrtCert)
    {
      logger_.LogDebug() << "IPC-APP ParseCertChain() -> ServiceId = " << it;
      Certificate::Uptr certUptr{std::make_unique<CertificateProxyImpl>(it, *this)};
      outcome.emplace_back(std::move(certUptr));
    }
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP: ParseCertChain() Error: " << certChainResult.Error().Message();
    return ara::core::Result<void>::FromError(
        static_cast<SecurityErrorDomain::Errc>(certChainResult.Error().Value()));
  }
}

Certificate::Status X509ProviderProxyImpl::VerifyCert(Certificate& cert, Certificate::Uptr myRoot) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  CertificateProxyImpl* obj{dynamic_cast<ara::crypto::x509::CertificateProxyImpl*>(&cert)};
  CryptoServiceInstanceId const certInstanceId{obj->GetCertificateServiceInstanceId()};
  CryptoServiceInstanceId myRootInstanceId;
  if (myRoot != nullptr)
  {
    myRootInstanceId = dynamic_cast<CertificateProxyImpl*>(&(*myRoot))->GetCertificateServiceInstanceId();
  }

  auto verifyOutput{x509ProviderHandler_->VerifyCert(certInstanceId, myRootInstanceId).GetResult()};
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl:: VerifyCert() result = "
                     << verifyOutput.Value().result;
  return static_cast<Certificate::Status>(verifyOutput.Value().result);
}

Certificate::Status X509ProviderProxyImpl::VerifyCertChain(
    ara::core::Span<const Certificate::Uptr> chain, Certificate::Uptr myRoot) const noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  // logic to convert variable from Span<const Certificate::Uptr> type to Vector<CryptoServiceInstanceId>
  ara::core::Vector<CryptoServiceInstanceId> vtrServiceInstanceId;
  for (auto& it : chain)
  {
    CertificateProxyImpl* obj = dynamic_cast<ara::crypto::x509::CertificateProxyImpl*>(&(*it));
    CryptoServiceInstanceId const certInstanceId{obj->GetCertificateServiceInstanceId()};
    vtrServiceInstanceId.emplace_back(certInstanceId);
  }

  CryptoServiceInstanceId myRootInstanceId;
  if (myRoot != nullptr)
  {
    myRootInstanceId = dynamic_cast<CertificateProxyImpl*>(&(*myRoot))->GetCertificateServiceInstanceId();
  }

  auto const verifyOutput{
      x509ProviderHandler_->VerifyCertChain(vtrServiceInstanceId, myRootInstanceId).GetResult()};
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl:: VerifyCert() result = "
                     << verifyOutput.Value().result;
  return static_cast<Certificate::Status>(verifyOutput.Value().result);
}

ara::core::Result<bool> X509ProviderProxyImpl::ImportCrl(ReadOnlyMemRegion crl) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  CryptoReadOnlyMemRegion crlUint8t(crl.begin(), crl.end());
  auto const importCrlResult{x509ProviderHandler_->ImportCrl(crlUint8t).GetResult()};
  if (importCrlResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP ImportCrl() -> success";
    return ara::core::Result<bool>::FromValue(importCrlResult.Value().result);
  }
  else
  {
    logger_.LogError() << "IPC-APP: ImportCrl() Error: " << importCrlResult.Error().Message();
    return ara::core::Result<bool>::FromError(
        static_cast<SecurityErrorDomain::Errc>(importCrlResult.Error().Value()));
  }
}

ara::core::Result<void> X509ProviderProxyImpl::Import(
    const Certificate& cert, ara::core::Optional<ara::core::InstanceSpecifier> iSpecify) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  const CertificateProxyImpl* obj{dynamic_cast<const ara::crypto::x509::CertificateProxyImpl*>(&cert)};
  CryptoServiceInstanceId const certInstanceId{obj->GetCertificateServiceInstanceId()};

  ara::core::String strInstSpecify;
  if (iSpecify.has_value() == true)
  {
    ara::core::InstanceSpecifier instSpecifier{*iSpecify};
    ara::core::StringView const strInst{instSpecifier.ToString()};
    ara::core::String strTemp{strInst.data()};
    strInstSpecify = strTemp;
  }

  auto const importResult{x509ProviderHandler_->Import(certInstanceId, strInstSpecify).GetResult()};
  if (importResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP Import() -> success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP: Import() Error: " << importResult.Error().Message();
    return ara::core::Result<void>::FromError(
        static_cast<SecurityErrorDomain::Errc>(importResult.Error().Value()));
  }
}

bool X509ProviderProxyImpl::Remove(Certificate::Uptrc&& cert) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  CryptoServiceInstanceId const certInstanceId{
      dynamic_cast<const CertificateProxyImpl*>(&(*cert))->GetCertificateServiceInstanceId()};
  auto importResult = x509ProviderHandler_->Remove(certInstanceId).GetResult();
  return importResult.Value().result;
}

ara::core::Result<void> X509ProviderProxyImpl::SetPendingStatus(const CertSignRequest& request) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  static_cast<void>(request);
  return ara::core::Result<void>();
}

ara::core::Result<void> X509ProviderProxyImpl::SetAsRootOfTrust(const Certificate& caCert) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  const CertificateProxyImpl* obj{dynamic_cast<const ara::crypto::x509::CertificateProxyImpl*>(&caCert)};
  CryptoServiceInstanceId caCertInstanceId{obj->GetCertificateServiceInstanceId()};
  auto trustResult{x509ProviderHandler_->SetAsRootOfTrust(caCertInstanceId).GetResult()};
  if (trustResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::SetAsRootOfTrust() -> success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP: X509ProviderProxyImpl::SetAsRootOfTrust() Error: "
                       << trustResult.Error().Message();
    return ara::core::Result<void>::FromError(
        static_cast<SecurityErrorDomain::Errc>(trustResult.Error().Value()));
  }
}

OcspResponse::Uptrc X509ProviderProxyImpl::ParseOcspResponse(ReadOnlyMemRegion response) const noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  static_cast<void>(response);
  OcspResponse::Uptrc localUtr{nullptr};
  return localUtr;
}

ara::core::Result<bool> X509ProviderProxyImpl::CheckCertStatus(
    Certificate& cert, const OcspResponse& ocspResponse) const noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  static_cast<void>(cert);
  static_cast<void>(ocspResponse);
  bool localStatus{false};
  return ara::core::Result<bool>(localStatus);
}

ara::core::Result<bool> X509ProviderProxyImpl::CheckCertStatus(
    const ara::core::Vector<Certificate*>& certList, const OcspResponse& ocspResponse) const noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  static_cast<void>(certList);
  static_cast<void>(ocspResponse);
  bool localStatus{false};
  return ara::core::Result<bool>(localStatus);
}

Certificate::Uptrc X509ProviderProxyImpl::FindCertByDn(const X509DN& subjectDn, const X509DN& issuerDn,
    time_t validityTimePoint, StorageIndex& certIndex) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  logger_.LogInfo() << __FUNCTION__
                    << "Parameter StorageIndex& certIndex not considered in current implementation";

  const X509DNProxyImpl* subjectDnObj{dynamic_cast<const ara::crypto::x509::X509DNProxyImpl*>(&subjectDn)};
  CryptoServiceInstanceId subjectDnInstanceId{subjectDnObj->GetX509DnServiceInstanceId()};

  const X509DNProxyImpl* issuerDnObj{dynamic_cast<const ara::crypto::x509::X509DNProxyImpl*>(&issuerDn)};
  CryptoServiceInstanceId const issuerDnInstanceId{issuerDnObj->GetX509DnServiceInstanceId()};

  int64_t const localValidityTime{validityTimePoint};

  static_cast<void>(certIndex);

  auto trustResult{
      x509ProviderHandler_->FindCertByDn(subjectDnInstanceId, issuerDnInstanceId, localValidityTime)
          .GetResult()};
  CryptoServiceInstanceId certByDnInstanceId{trustResult.Value().reslutCertInstanceId};
  if (certByDnInstanceId.empty() == false)
  {
    logger_.LogDebug() << "IPC-APP Service InstanceId for FindCertByDn() : " << certByDnInstanceId;
    Certificate::Uptrc certUptrc{std::make_unique<const CertificateProxyImpl>(certByDnInstanceId, *this)};
    return certUptrc;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP FindCertByDn(), ServiceInstaneID is empty,so returning nullptr";
    return nullptr;
  }
}

Certificate::Uptrc X509ProviderProxyImpl::FindCertByKeyIds(
    ReadOnlyMemRegion subjectKeyId, ReadOnlyMemRegion authorityKeyId) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  CryptoReadOnlyMemRegion roMemSubjectKeyId{subjectKeyId.begin(), subjectKeyId.end()};
  CryptoReadOnlyMemRegion roMemauthorityKeyId{authorityKeyId.begin(), authorityKeyId.end()};
  auto const keyIdsResult{
      x509ProviderHandler_->FindCertByKeyIds(roMemSubjectKeyId, roMemauthorityKeyId).GetResult()};
  CryptoServiceInstanceId const certBykeyIdsInstanceId{keyIdsResult.Value().reslutCertInstanceId};
  if (certBykeyIdsInstanceId.empty() == false)
  {
    logger_.LogDebug() << "IPC-APP Service InstanceId for FindCertByKeyIds() : " << certBykeyIdsInstanceId;
    Certificate::Uptrc certUptrc{std::make_unique<const CertificateProxyImpl>(certBykeyIdsInstanceId, *this)};
    return certUptrc;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP FindCertByKeyIds(), ServiceInstaneID is empty,so returning nullptr";
    return nullptr;
  }
}

Certificate::Uptrc X509ProviderProxyImpl::FindCertBySn(ReadOnlyMemRegion sn, const X509DN& issuerDn) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  CryptoReadOnlyMemRegion const roMemSN{sn.begin(), sn.end()};
  const X509DNProxyImpl* issuerDnObj{dynamic_cast<const ara::crypto::x509::X509DNProxyImpl*>(&issuerDn)};
  CryptoServiceInstanceId const issuerDnInstanceId{issuerDnObj->GetX509DnServiceInstanceId()};
  auto const certBySnResult{x509ProviderHandler_->FindCertBySn(roMemSN, issuerDnInstanceId).GetResult()};
  CryptoServiceInstanceId const certBySnIdsInstanceId = certBySnResult.Value().reslutCertInstanceId;
  if (certBySnIdsInstanceId.empty() == false)
  {
    logger_.LogDebug() << "IPC-APP Service InstanceId for FindCertBySn() : " << certBySnIdsInstanceId;
    Certificate::Uptrc certUptrc{std::make_unique<const CertificateProxyImpl>(certBySnIdsInstanceId, *this)};
    return certUptrc;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP FindCertBySn(), ServiceInstaneID is empty,so returning nullptr";
    return nullptr;
  }
}

ara::core::Result<CertSignRequest::Uptrc> X509ProviderProxyImpl::ParseCertSignRequest(
    ReadOnlyMemRegion csr, bool withMetaData) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  static_cast<void>(csr);
  static_cast<void>(withMetaData);
  return ara::core::Result<CertSignRequest::Uptrc>(nullptr);
}

void X509ProviderProxyImpl::CleanupVolatileStorage() noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  auto cleanUpResult{x509ProviderHandler_->CleanupVolatileStorage().GetResult()};
  static_cast<void>(cleanUpResult);
  return;
}

ara::core::Result<X509Extensions::Uptr> X509ProviderProxyImpl::CreateEmptyExtensions(
    std::size_t capacity) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  static_cast<void>(capacity);
  return ara::core::Result<X509Extensions::Uptr>(nullptr);
}

ara::core::Result<Certificate::Uptr> X509ProviderProxyImpl::LoadCertificate(
    ara::core::InstanceSpecifier& iSpecify) noexcept
{
  logger_.LogDebug() << "IPC-APP X509ProviderProxyImpl::" << __FUNCTION__;
  ara::core::String const strIspecify{iSpecify.ToString()};
  auto const loadCertResult{x509ProviderHandler_->LoadCertificate(strIspecify).GetResult()};
  if (loadCertResult.HasValue())
  {
    CryptoServiceInstanceId instanceId{loadCertResult.Value().resultInstanceId};
    logger_.LogDebug() << "IPC-APP Service InstanceId for LoadCertificate : " << instanceId;
    Certificate::Uptr certUptr{std::make_unique<CertificateProxyImpl>(instanceId, *this)};
    return ara::core::Result<Certificate::Uptr>::FromValue(std::move(certUptr));
  }
  else
  {
    logger_.LogError() << "IPC-APP: CreateEmptyDn() Error: " << loadCertResult.Error().Message();
    return ara::core::Result<Certificate::Uptr>::FromError(
        static_cast<SecurityErrorDomain::Errc>(loadCertResult.Error().Value()));
  }
}

} // namespace x509
} // namespace crypto
} // namespace ara
