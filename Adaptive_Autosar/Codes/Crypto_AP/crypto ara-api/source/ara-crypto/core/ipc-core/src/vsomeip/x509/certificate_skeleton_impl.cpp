// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certificate_skeleton_impl.cpp
//
// Purpose     : Certificate skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "certificate_skeleton_impl.h"
#include "hash_function_container.h"
#include "x509dn_container.h"
#include "x509publickeyinfo_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
CertificateSkeletonImpl::CertificateSkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::x509::Certificate::Uptr certificateUptr) noexcept :
    ara::crypto::ipccom::x509::certificate::skeleton::CertificateSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    cryptoInstanceId_{instanceId}, certUptr_(std::move(certificateUptr))
{
  logger_.LogDebug() << "IPC-CORE::certificate CertificateSkeletonImpl Constructor";
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::AuthorityKeyIdOutput>
    CertificateSkeletonImpl::AuthorityKeyId()
{
  logger_.LogDebug() << "IPC-CORE::certificate AuthorityKeyId()";
  ara::core::Future<AuthorityKeyIdOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const retValue{certUptr_->AuthorityKeyId()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::certificate AuthorityKeyId() -> success";
    ara::core::Vector<ara::core::Byte> tempVtr{retValue.Value()};
    AuthorityKeyIdOutput authorityKeyId;
    std::for_each(tempVtr.begin(), tempVtr.end(), [&authorityKeyId](ara::core::Byte i) {
      authorityKeyId.result.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(authorityKeyId);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::certificate AuthorityKeyId() Error ->" << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::EndTimeOutput>
    CertificateSkeletonImpl::EndTime()
{
  logger_.LogDebug() << "IPC-CORE::certificate EndTime()";
  ara::core::Future<EndTimeOutput>::PromiseType promise;
  EndTimeOutput endTime;
  endTime.time_t = certUptr_->EndTime();
  promise.set_value(endTime);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::ExportPubliclyOutput>
    CertificateSkeletonImpl::ExportPublicly(const uint32& formatId)
{
  logger_.LogDebug() << "IPC-CORE::certificate ExportPublicly()";
  ara::core::Future<ExportPubliclyOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const retValue{certUptr_->ExportPublicly(formatId)};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::certificate ExportPublicly() -> success";
    ara::core::Vector<ara::core::Byte> tempVtr{retValue.Value()};
    ExportPubliclyOutput exportOutput;
    std::for_each(tempVtr.begin(), tempVtr.end(), [&exportOutput](ara::core::Byte i) {
      exportOutput.resultByteVector.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(exportOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::certificate ExportPublicly() Error ->" << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::GetConstraintsOutput>
    CertificateSkeletonImpl::GetConstraints()
{
  logger_.LogDebug() << "IPC-CORE::certificate GetConstraints()";
  ara::core::Future<GetConstraintsOutput>::PromiseType promise;
  GetConstraintsOutput getConst;
  getConst.keyConstraints = certUptr_->GetConstraints();
  promise.set_value(getConst);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::GetFingerprintOutput>
    CertificateSkeletonImpl::GetFingerprint(const CryptoServiceInstanceId& hashCtxInstanceId,
                                            const uint32& fingerPrintSize)
{
  logger_.LogDebug() << "IPC-CORE::certificate GetFingerprint()";
  ara::core::Future<GetFingerprintOutput>::PromiseType promise;
  // Read write span variable to pass to daemon
  ara::core::Vector<uint8_t> tempFingerPrintVtr;
  logger_.LogDebug() << "IPC-CORE::certificate fingerPrint buffer size = " << fingerPrintSize;
  tempFingerPrintVtr.resize(fingerPrintSize);
  ReadWriteMemRegion roFprint{tempFingerPrintVtr};

  std::shared_ptr<ara::crypto::common::crypdatastore::HashFunctionContainer> hashStorageContainer{
      ara::crypto::common::crypdatastore::HashFunctionContainer::GetInstance()};
  std::unique_ptr<ara::crypto::cryp::HashFunctionSkeletonImpl>& hashUptr{
      hashStorageContainer->GetObjectFromContanier(hashCtxInstanceId)};
  ara::crypto::cryp::HashFunctionCtx::Uptr& hashFunctionCtxPtr{hashUptr->GetHashFunctionUptr()};

  ara::core::Result<std::size_t> const retValue{certUptr_->GetFingerprint(roFprint, *hashFunctionCtxPtr)};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::certificate GetFingerprint() -> success";
    GetFingerprintOutput fingerPrintOutpout;

    std::stringstream fingerprintValue;

    // Store the calculated fingreprint in inout buffer until the inout buffer gets completely occupied
    // or fingerprint value gets completely copied.
    for (uint8_t index = 0; index < roFprint.size(); index++)
    {
      fingerprintValue << std::setw(2) << std::setfill('0') << std::hex
                       << static_cast<uint16_t>(roFprint[index]);
      fingerPrintOutpout.fingerprint.push_back(roFprint[index]);
    }
    logger_.LogDebug() << "IPC-CORE:: GetFingerprint() Hash Value =  " << fingerprintValue.str();

    logger_.LogDebug() << "IPC-CORE:: GetFingerprint() size value = " << retValue.Value();
    fingerPrintOutpout.size_t = retValue.Value();
    promise.set_value(fingerPrintOutpout);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::certificate GetFingerprint() Error ->" << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::GetPathLimitOutput>
    CertificateSkeletonImpl::GetPathLimit()
{
  logger_.LogDebug() << "IPC-CORE::certificate GetPathLimit()";
  // Note: need further analysis to check on interface GetPathLimit()
  ara::core::Future<GetPathLimitOutput>::PromiseType promise;
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::GetStatusOutput>
    CertificateSkeletonImpl::GetStatus()
{
  logger_.LogDebug() << "IPC-CORE::certificate GetStatus()";
  ara::core::Future<GetStatusOutput>::PromiseType promise;
  GetStatusOutput getStatus;
  getStatus.status = static_cast<uint32_t>(certUptr_->GetStatus());
  promise.set_value(getStatus);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::IsCaOutput>
    CertificateSkeletonImpl::IsCa()
{
  logger_.LogDebug() << "IPC-CORE::certificate IsCa()";
  ara::core::Future<IsCaOutput>::PromiseType promise;
  IsCaOutput isTrue;
  isTrue.result = certUptr_->IsCa();
  promise.set_value(isTrue);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::IsRootOutput>
    CertificateSkeletonImpl::IsRoot()
{
  logger_.LogDebug() << "IPC-CORE::certificate IsRoot()";
  ara::core::Future<IsRootOutput>::PromiseType promise;
  IsRootOutput isTrue;
  isTrue.response = certUptr_->IsRoot();
  promise.set_value(isTrue);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::IssuerDnOutput>
    CertificateSkeletonImpl::IssuerDn()
{
  logger_.LogDebug() << "IPC-CORE::certificate IssuerDn()";
  ara::core::Future<IssuerDnOutput>::PromiseType promise;
  IssuerDnOutput issuerDnResult;
  X509DN::Uptr x509DnUptr{certUptr_->IssuerDn()};
  std::shared_ptr<ara::crypto::common::datastorage::X509DnContainer> dnContainer{
      ara::crypto::common::datastorage::X509DnContainer::GetInstance()};
  CryptoServiceInstanceId localInstId{dnContainer->GetCryptoServiceInstanceId()};
  std::unique_ptr<ara::crypto::x509::X509DnSkeletonImpl> x509DnSkelImplUptr{
      std::make_unique<ara::crypto::x509::X509DnSkeletonImpl>(localInstId, std::move(x509DnUptr))};
  x509DnSkelImplUptr->OfferService();
  dnContainer->StoreObjectToContanier(localInstId, std::move(x509DnSkelImplUptr));
  issuerDnResult.response = localInstId;
  promise.set_value(issuerDnResult);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::MyProviderOutput>
    CertificateSkeletonImpl::MyProvider()
{
  // Not implemented.
  logger_.LogDebug() << "IPC-CORE::certificate MyProvider()";
  ara::core::Future<MyProviderOutput>::PromiseType promise;
  MyProviderOutput myProvider;
  myProvider.myProviderInstanceId = cryptoInstanceId_;
  promise.set_value(myProvider);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::SerialNumberOutput>
    CertificateSkeletonImpl::SerialNumber()
{
  logger_.LogDebug() << "IPC-CORE::certificate SerialNumber()";
  ara::core::Future<SerialNumberOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const retValue{certUptr_->SerialNumber()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::certificate SerialNumber() -> success";
    SerialNumberOutput serialno;
    ara::core::Vector<ara::core::Byte> const tempVtr{retValue.Value()};
    std::for_each(tempVtr.begin(), tempVtr.end(),
        [&serialno](ara::core::Byte i) { serialno.result.emplace_back(static_cast<uint8_t>(i)); });
    promise.set_value(serialno);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::certificate SerialNumber() Error ->" << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::StartTimeOutput>
    CertificateSkeletonImpl::StartTime()
{
  logger_.LogDebug() << "IPC-CORE::certificate StartTime()";
  ara::core::Future<StartTimeOutput>::PromiseType promise;
  StartTimeOutput startTime;
  startTime.time_t = certUptr_->StartTime();
  promise.set_value(startTime);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::SubjectDnOutput>
    CertificateSkeletonImpl::SubjectDn()
{
  logger_.LogDebug() << "IPC-CORE::certificate SubjectDn()";
  ara::core::Future<SubjectDnOutput>::PromiseType promise;
  SubjectDnOutput subDnResult;
  X509DN::Uptr x509DnUptr{certUptr_->SubjectDn()};
  std::shared_ptr<ara::crypto::common::datastorage::X509DnContainer> dnContainer{
      ara::crypto::common::datastorage::X509DnContainer::GetInstance()};
  CryptoServiceInstanceId localInstId{dnContainer->GetCryptoServiceInstanceId()};
  logger_.LogDebug() << "IPC-CORE::SubjectDn() ServiceInstanceId = " << localInstId;
  std::unique_ptr<ara::crypto::x509::X509DnSkeletonImpl> x509DnSkelImplUptr{
      std::make_unique<ara::crypto::x509::X509DnSkeletonImpl>(localInstId, std::move(x509DnUptr))};
  x509DnSkelImplUptr->OfferService();
  dnContainer->StoreObjectToContanier(localInstId, std::move(x509DnSkelImplUptr));
  subDnResult.x509DnInstanceId = localInstId;
  promise.set_value(subDnResult);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::SubjectKeyIdOutput>
    CertificateSkeletonImpl::SubjectKeyId()
{
  logger_.LogDebug() << "IPC-CORE::certificate SubjectKeyId()";
  ara::core::Future<SubjectKeyIdOutput>::PromiseType promise;

  ara::core::Result<ara::core::Vector<ara::core::Byte>> const retValue{certUptr_->SubjectKeyId()};

  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::certificate SubjectKeyId() -> success";
    SubjectKeyIdOutput subKeyId;
    ara::core::Vector<ara::core::Byte> tempVtr{retValue.Value()};
    std::for_each(tempVtr.begin(), tempVtr.end(),
        [&subKeyId](ara::core::Byte i) { subKeyId.result.emplace_back(static_cast<uint8_t>(i)); });
    promise.set_value(subKeyId);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::certificate SubjectKeyId() Error ->" << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::SubjectPubKeyOutput>
    CertificateSkeletonImpl::SubjectPubKey(const CryptoServiceInstanceId& cryptoProviderInstanceId)
{
  logger_.LogDebug() << "IPC-CORE::certificate SubjectPubKey()";
  ara::core::Future<SubjectPubKeyOutput>::PromiseType promise;

  // Note: cryp::CryptoProvider::Uptr object as argument is not supported for now
  static_cast<void>(cryptoProviderInstanceId);

  X509PublicKeyInfo::Uptrc pkiUptrc{certUptr_->SubjectPubKey()};

  std::shared_ptr<ara::crypto::common::datastorage::X509PublicKeyInfoContainer> pkiContainer{
      ara::crypto::common::datastorage::X509PublicKeyInfoContainer::GetInstance()};
  CryptoServiceInstanceId localPkiInstanceId{pkiContainer->GetCryptoServiceInstanceId()};
  logger_.LogDebug() << "IPC-CORE::SubjectPubKey(), Service InstanceId -> " << localPkiInstanceId;

  std::unique_ptr<X509PublicKeyInfoSkeletonImpl> localPkiSkeletonUptr{
      std::make_unique<X509PublicKeyInfoSkeletonImpl>(localPkiInstanceId, std::move(pkiUptrc))};
  localPkiSkeletonUptr->OfferService();
  pkiContainer->StoreObjectToContanier(localPkiInstanceId, std::move(localPkiSkeletonUptr));
  SubjectPubKeyOutput subPublicKey;
  subPublicKey.x509PublicKeyInfoInstanceId = localPkiInstanceId;
  promise.set_value(subPublicKey);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::VerifyMeOutput>
    CertificateSkeletonImpl::VerifyMe(const CryptoServiceInstanceId& caCertInstanceId)
{
  // VerifyMe method is not present in autosar file.
  static_cast<void>(caCertInstanceId);
  logger_.LogDebug() << "IPC-CORE::certificate VerifyMe()";
  ara::core::Future<VerifyMeOutput>::PromiseType promise;
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::certificate::Certificate::X509VersionOutput>
    CertificateSkeletonImpl::X509Version()
{
  logger_.LogDebug() << "IPC-CORE::certificate X509Version()";
  ara::core::Future<X509VersionOutput>::PromiseType promise;
  X509VersionOutput x509Version;
  x509Version.response = certUptr_->X509Version();
  promise.set_value(x509Version);
  return promise.get_future();
}

} // namespace x509
} // namespace crypto
} // namespace ara
