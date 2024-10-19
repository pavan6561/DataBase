// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certsignrequest_proxy_impl.cpp
//
// Purpose     : X509 domain number proxy implementation to forward functional calls from application to
//               ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "certsignrequest_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
CertSignRequestProxyImpl::CertSignRequestProxyImpl(
    CryptoServiceInstanceId const instanceValue, X509Provider& x509ProRef) noexcept :
    x509ProviderRef_{x509ProRef},
    logger_{ara::log::CreateLogger("csr", "csr Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "IPC-APP:: server client connection for csr -> success";
  }
  else
  {
    logger_.LogError() << "IPC-APP:: server client connection for csr -> fail";
  }
}

bool CertSignRequestProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess{false};
  if (vsomeipCertInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for csr Function: Successful";
    csrHandler_ = vsomeipCertInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for csr Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}
bool CertSignRequestProxyImpl::Verify() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> Verify()";
  bool ret{false};
  auto const retOutput{csrHandler_->Version().GetResult()};
  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> Verify() : Success";
    ret = retOutput.Value().result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertSignRequestProxyImpl -> Verify() Failed: ";
  }
  return ret;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>>
    CertSignRequestProxyImpl::ExportASN1CertSignRequest() noexcept
{
  logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> ExportASN1CertSignRequest()";
  auto const resultConfData{csrHandler_->ExportASN1CertSignRequest().GetResult()};
  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> "
                          "ExportASN1CertSignRequest() : Success";
    auto const tempVtr{resultConfData.Value().result};
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertSignRequestProxyImpl::"
                          "ExportASN1CertSignRequest() -> Failed:";
    logger_.LogError() << "IPC-APP::ExportASN1CertSignRequest() -> Error: "
                       << resultConfData.Error().Message();
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        static_cast<SecurityErrorDomain::Errc>(resultConfData.Error().Value()));
  }
}

unsigned CertSignRequestProxyImpl::Version() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> Version()";
  unsigned retData{0};
  auto const resultConfData{csrHandler_->Version().GetResult()};

  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> Version() : Success";
    retData = resultConfData.Value().result;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> Version() : Failed";
  }
  return retData;
}

const ara::crypto::cryp::Signature& CertSignRequestProxyImpl::GetSignature() const noexcept
{
}

const X509DN& CertSignRequestProxyImpl::SubjectDn() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> SubjectDn()";
  auto const retValue{csrHandler_->SubjectDn().GetResult()};
  logger_.LogDebug() << "IPC-APP::SubjectDn(), CryptoserviceInstanceId: "
                     << retValue.Value().x509DnInstanceId;
  CryptoServiceInstanceId const localX509DnServiceInstance{retValue.Value().x509DnInstanceId};
  // Unique pointer of x509DN class
  static std::unique_ptr<ara::crypto::x509::X509DN> x509DNProxyUptr
      = std::make_unique<ara::crypto::x509::X509DNProxyImpl>(localX509DnServiceInstance, x509ProviderRef_);
  return *x509DNProxyUptr;
}

bool CertSignRequestProxyImpl::IsCa() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> IsCa()";
  bool ret{false};
  auto const retValue{csrHandler_->IsCa().GetResult()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> IsCa() : Success";
    ret = retValue.Value().result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertSignRequestProxyImpl::IsCa() -> Failed: ";
  }
  return ret;
}

std::uint32_t CertSignRequestProxyImpl::GetPathLimit() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> GetPathLimit()";
  uint32_t ret{0};
  auto const retValue{csrHandler_->GetPathLimit().GetResult()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> GetPathLimit() : Success";
    ret = retValue.Value().pathLimit;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertSignRequestProxyImpl::GetPathLimit() -> Failed: ";
  }
  return ret;
}

ara::crypto::x509::BasicCertInfo::KeyConstraints CertSignRequestProxyImpl::GetConstraints() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> GetConstraints()";
  uint32_t ret{0};
  auto const retValue{csrHandler_->GetConstraints().GetResult()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> GetConstraints() : Success";
    ret = retValue.Value().keyConstraints;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertSignRequestProxyImpl::GetConstraints() -> Failed:";
  }
  return static_cast<KeyConstraints>(ret);
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertSignRequestProxyImpl::ExportPublicly(
    FormatId formatId) const noexcept
{
  static_cast<void>(formatId);
}

X509Provider& CertSignRequestProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertSignRequestProxyImpl -> MyProvider()";
  return x509ProviderRef_;
}

} // namespace x509
} // namespace crypto
} // namespace ara
