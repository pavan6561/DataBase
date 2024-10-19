// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certificate_proxy_impl.cpp
//
// Purpose     : X509 domain number proxy implementation to forward functional calls from application to
//               ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "certificate_proxy_impl.h"
#include "hash_function_ctx_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
CertificateProxyImpl::CertificateProxyImpl(
    CryptoServiceInstanceId const instanceValue, X509Provider& x509ProRef) noexcept :
    x509ProviderRef_{x509ProRef},
    logger_{ara::log::CreateLogger("Certificate", "Certificate Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "IPC-APP:: server client connection for Certificate -> success";
  }
  else
  {
    logger_.LogError() << "IPC-APP:: server client connection for Certificate -> fail";
  }
}

bool CertificateProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess{false};
  if (vsomeipCertInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for Certificate Function: Successful";
    certificateHandler_ = vsomeipCertInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for Certificate Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

CryptoServiceInstanceId CertificateProxyImpl::GetCertificateServiceInstanceId() const
{
  return vsomeipCertInstance_.serviceInstanceID_;
}

std::uint32_t CertificateProxyImpl::X509Version() const noexcept
{
  logger_.LogDebug() << "IPC-APP:: CertificateProxyImpl::X509Version()";
  std::uint32_t ret{0};
  auto const retOutput{certificateHandler_->X509Version().GetResult()};

  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> X509Version() : Success";
    ret = retOutput.Value().response;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl -> X509Version() Failed: ";
  }
  return ret;
}

bool CertificateProxyImpl::IsRoot() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> IsRoot()";
  bool isRoot{false};
  auto const retValue{certificateHandler_->IsRoot().GetResult()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> IsRoot() : Success";
    isRoot = retValue.Value().response;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::IsRoot() Failed: ";
  }
  return isRoot;
}

const X509DN& CertificateProxyImpl::IssuerDn() const
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> IssuerDn()";
  auto const retValue{certificateHandler_->IssuerDn().GetResult()};
  logger_.LogDebug() << "IPC-APP::IssuerDn(), CryptoserviceInstanceId: " << retValue.Value().response;
  CryptoServiceInstanceId localX509DnServiceInstance{retValue.Value().response};
  // Unique pointer of x509DN class
  static std::unique_ptr<ara::crypto::x509::X509DN> x509DNProxyUptr{
      std::make_unique<ara::crypto::x509::X509DNProxyImpl>(localX509DnServiceInstance, x509ProviderRef_)};
  return *x509DNProxyUptr;
}

time_t CertificateProxyImpl::StartTime() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> StartTime()";
  time_t ret{0};
  auto const retValue{certificateHandler_->StartTime().GetResult()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> StartTime() : Success";
    ret = retValue.Value().time_t;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::StartTime() Failed: ";
  }
  return ret;
}

time_t CertificateProxyImpl::EndTime() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> EndTime()";
  time_t ret{0};
  auto const retValue{certificateHandler_->EndTime().GetResult()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> EndTime() : Success";
    ret = retValue.Value().time_t;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::EndTime() Failed: ";
  }
  return ret;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertificateProxyImpl::SerialNumber() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> SerialNumber()";
  auto const resultConfData{certificateHandler_->SerialNumber().GetResult()};
  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> SerialNumber() : Success";
    auto tempVtr{resultConfData.Value().result};
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::SerialNumber() -> Failed:";
    logger_.LogError() << "IPC-APP::SerialNumber() -> Error: " << resultConfData.Error().Message();
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        static_cast<SecurityErrorDomain::Errc>(resultConfData.Error().Value()));
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertificateProxyImpl::AuthorityKeyId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> AuthorityKeyId()";
  auto const resultConfData{certificateHandler_->AuthorityKeyId().GetResult()};
  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> AuthorityKeyId() : Success";
    auto tempVtr{resultConfData.Value().result};
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::AuthorityKeyId() -> Failed:";
    logger_.LogError() << "IPC-APP::SerialNumber() -> Error: " << resultConfData.Error().Message();
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        static_cast<SecurityErrorDomain::Errc>(resultConfData.Error().Value()));
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertificateProxyImpl::SubjectKeyId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> SubjectKeyId()";
  auto const resultConfData{certificateHandler_->SubjectKeyId().GetResult()};
  if (resultConfData.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> SubjectKeyId() : Success";
    auto tempVtr{resultConfData.Value().result};
    ara::core::Vector<ara::core::Byte> vtrbyte(tempVtr.begin(), tempVtr.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(vtrbyte);
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::SubjectKeyId() -> Failed:";
    logger_.LogError() << "IPC-APP::SerialNumber() -> Error: " << resultConfData.Error().Message();
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        static_cast<SecurityErrorDomain::Errc>(resultConfData.Error().Value()));
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertificateProxyImpl::ExportPublicly(
    FormatId formatId) const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> ExportPublicly()";
  auto const objResult{certificateHandler_->ExportPublicly(formatId).GetResult()};
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> " << __FUNCTION__ << " : Success";
    auto const byteResult{objResult.Value().resultByteVector};
    ara::core::Vector<ara::core::Byte> localByteVector(byteResult.begin(), byteResult.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(localByteVector);
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl -> " << __FUNCTION__ << " : Failed";
    logger_.LogError() << "IPC-APP::ExportPublicly() Error: " << objResult.Error().Message();
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        static_cast<SecurityErrorDomain::Errc>(objResult.Error().Value()));
  }
}

const X509DN& CertificateProxyImpl::SubjectDn() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> SubjectDn()";
  auto const retValue{certificateHandler_->SubjectDn().GetResult()};
  logger_.LogDebug() << "IPC-APP::SubjectDn(), CryptoserviceInstanceId: "
                     << retValue.Value().x509DnInstanceId;
  CryptoServiceInstanceId localX509DnServiceInstance{retValue.Value().x509DnInstanceId};
  // Unique pointer of x509DN class
  static std::unique_ptr<ara::crypto::x509::X509DN> x509DNProxyUptr{
      std::make_unique<ara::crypto::x509::X509DNProxyImpl>(localX509DnServiceInstance, x509ProviderRef_)};
  return *x509DNProxyUptr;
}

bool CertificateProxyImpl::IsCa() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> IsCa()";
  bool ret{false};
  auto const retValue{certificateHandler_->IsCa().GetResult()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> IsCa() : Success";
    ret = retValue.Value().result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::IsCa() -> Failed: ";
  }
  return ret;
}

std::uint32_t CertificateProxyImpl::GetPathLimit() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> GetPathLimit()";
  uint32_t ret{0};
  auto const retValue{certificateHandler_->GetPathLimit().GetResult()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> GetPathLimit() : Success";
    ret = retValue.Value().pathLimit;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::GetPathLimit() -> Failed: ";
  }
  return ret;
}

BasicCertInfo::KeyConstraints CertificateProxyImpl::GetConstraints() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> GetConstraints()";
  uint32_t ret{0};
  auto const retValue{certificateHandler_->GetConstraints().GetResult()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> GetConstraints() : Success";
    ret = retValue.Value().keyConstraints;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::GetConstraints() -> Failed:";
  }
  return static_cast<KeyConstraints>(ret);
}

ara::core::Result<std::size_t> CertificateProxyImpl::GetFingerprint(
    ReadWriteMemRegion fingerprint, cryp::HashFunctionCtx& hashCtx) const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> GetFingerprint()";

  const ara::crypto::cryp::HashFunctionCtxProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::HashFunctionCtxProxyImpl*>(&hashCtx)};
  CryptoServiceInstanceId const hashServiceInstanceId{obj->GetHashFunctionCtxServiceInstanceId()};
  logger_.LogDebug() << "IPC-APP:X509 GetFingerprint() HashFunctionCtx -> CryptoServiceInstanceId : "
                     << hashServiceInstanceId;

  auto const fingerResult{
      certificateHandler_->GetFingerprint(hashServiceInstanceId, fingerprint.size()).GetResult()};
  if (fingerResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> GetFingerprint() : Success";

    std::stringstream tempStr;
    for (uint16_t i = 0; i < fingerprint.size(); i++)
    {
      tempStr << std::setw(2) << std::setfill('0') << std::hex
              << static_cast<uint16_t>(fingerResult.Value().fingerprint[i]);
      fingerprint[i] = fingerResult.Value().fingerprint[i];
    }
    logger_.LogDebug() << "IPC-APP::GetFingerprint() Hash Value = " << tempStr.str();

    std::size_t const value{static_cast<std::size_t>(fingerResult.Value().size_t)};
    logger_.LogDebug() << "IPC-APP::GetFingerprint() : Size value = " << value;
    return ara::core::Result<std::size_t>::FromValue(value);
  }
  else
  {
    logger_.LogError() << "IPC-APP::CertificateProxyImpl::GetFingerprint() -> Failed:";
    logger_.LogError() << "IPC-APP::SerialNumber() -> Error: " << fingerResult.Error().Message();
    return ara::core::Result<std::size_t>::FromError(
        static_cast<SecurityErrorDomain::Errc>(fingerResult.Error().Value()));
  }
}

Certificate::Status CertificateProxyImpl::GetStatus() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> GetStatus()";
  auto const retValue{certificateHandler_->GetStatus().GetResult()};
  logger_.LogDebug() << "IPC-APP:: GetStatus() return value = " << retValue.Value().status;
  return static_cast<Certificate::Status>(retValue.Value().status);
}

X509Provider& CertificateProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> MyProvider()";
  return x509ProviderRef_;
}

const X509PublicKeyInfo& CertificateProxyImpl::SubjectPubKey(cryp::CryptoProvider::Uptr cryptoProvider) const
    noexcept
{
  logger_.LogDebug() << "IPC-APP::CertificateProxyImpl -> SubjectPubKey()";
  // Note : cryp::CryptoProvider::Uptr with default argument is only supported
  static_cast<void>(cryptoProvider);
  CryptoServiceInstanceId const tmpInstanceId;
  auto const retValue = certificateHandler_->SubjectPubKey(tmpInstanceId).GetResult();
  CryptoServiceInstanceId const localPkiInstanceId{retValue.Value().x509PublicKeyInfoInstanceId};
  logger_.LogDebug() << "IPC-APP:: GetStatus() return value = " << localPkiInstanceId;
  static std::unique_ptr<const X509PublicKeyInfo> localPkiProxyUptr{
      std::make_unique<const X509PublicKeyInfoProxyImpl>(localPkiInstanceId)};
  return *localPkiProxyUptr;
}

} // namespace x509
} // namespace crypto
} // namespace ara
