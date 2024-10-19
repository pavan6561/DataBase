// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certsignrequest_skeleton_impl.cpp
//
// Purpose     : X509 Domain Number skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "certsignrequest_skeleton_impl.h"
#include "x509dn_container.h"
#include "signature_container.h"
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
CertSignRequestSkeletonImpl::CertSignRequestSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::x509::CertSignRequest::Uptr certSignUptr) noexcept :
    ara::crypto::ipccom::x509::csr::skeleton::CertSignRequestSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    cryptoInstanceId_{instanceId}, certSignUptr_{std::move(certSignUptr)}
{
  logger_.LogDebug() << "X509DnSkeletonImpl Constructor";
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::ExportASN1CertSignRequestOutput>
    CertSignRequestSkeletonImpl::ExportASN1CertSignRequest()
{
  ara::core::Future<
      ara::crypto::ipccom::x509::csr::CertSignRequest::ExportASN1CertSignRequestOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const retValue{
      certSignUptr_->ExportASN1CertSignRequest()};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl ExportASN1CertSignRequest() -> success";
    ara::crypto::ipccom::x509::csr::CertSignRequest::ExportASN1CertSignRequestOutput
        exportASN1certSignReqOutput;
    ara::core::Vector<ara::core::Byte> tempVtr{retValue.Value()};
    std::for_each(tempVtr.begin(), tempVtr.end(), [&exportASN1certSignReqOutput](ara::core::Byte i) {
      exportASN1certSignReqOutput.result.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(exportASN1certSignReqOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::CertSignRequestSkeletonImpl ExportASN1CertSignRequest() Error ->"
                       << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::ExportPubliclyOutput>
    CertSignRequestSkeletonImpl::ExportPublicly(const CryptoServiceInstanceId& formatId)
{
  static_cast<void>(formatId);
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::ExportPubliclyOutput>::PromiseType
      promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const retValue{certSignUptr_->ExportPublicly()};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl ExportPublicly() -> success";
    ara::crypto::ipccom::x509::csr::CertSignRequest::ExportPubliclyOutput exportPubliclyOutputObj;
    ara::core::Vector<ara::core::Byte> tempVtr{retValue.Value()};
    std::for_each(tempVtr.begin(), tempVtr.end(), [&exportPubliclyOutputObj](ara::core::Byte i) {
      exportPubliclyOutputObj.resultByteVector.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(exportPubliclyOutputObj);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::CertSignRequestSkeletonImpl ExportPublicly() Error ->"
                       << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::GetConstraintsOutput>
    CertSignRequestSkeletonImpl::GetConstraints()
{
  logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl GetConstraints() -> success";
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::GetConstraintsOutput>::PromiseType
      promise;
  ara::crypto::ipccom::x509::csr::CertSignRequest::GetConstraintsOutput getConstraintsOutput;
  getConstraintsOutput.keyConstraints = certSignUptr_->GetConstraints();
  promise.set_value(getConstraintsOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::GetPathLimitOutput>
    CertSignRequestSkeletonImpl::GetPathLimit()
{
  // GetPathLimit is not present in autosar
  logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl GetPathLimit() -> success";
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::GetPathLimitOutput>::PromiseType promise;
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::GetSignatureOutput>
    CertSignRequestSkeletonImpl::GetSignature()
{
  logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl GetSignature() -> success";
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::GetSignatureOutput>::PromiseType promise;
  ara::crypto::ipccom::x509::csr::CertSignRequest::GetSignatureOutput getsignOutput;
  ara::crypto::cryp::Signature::Uptrc signatureUptrc{certSignUptr_->GetSignature()};
  if (signatureUptrc != nullptr)
  {
    logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl SubjectDn() pointer is not Null";
    std::shared_ptr<ara::crypto::common::datastorage::SignatureContainer> dnContainer{
        ara::crypto::common::datastorage::SignatureContainer::GetInstance()};
    CryptoServiceInstanceId signatureLocalInstanceId{dnContainer->GetCryptoServiceInstanceId()};
    getsignOutput.signatureInstanceId = signatureLocalInstanceId;

    std::unique_ptr<ara::crypto::cryp::cryobj::SignatureSkeletonImpl> localSignatureUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SignatureSkeletonImpl>(
            signatureLocalInstanceId, std::move(signatureUptrc))};
    dnContainer->StoreObjectToContanier(signatureLocalInstanceId, std::move(localSignatureUtr));
    getsignOutput.signatureInstanceId = signatureLocalInstanceId;
  }
  else
  {
  }
  promise.set_value(getsignOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::IsCaOutput>
    CertSignRequestSkeletonImpl::IsCa()
{
  logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl IsCa() -> success";
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::IsCaOutput>::PromiseType promise;
  ara::crypto::ipccom::x509::csr::CertSignRequest::IsCaOutput isCaOutput;
  isCaOutput.result = certSignUptr_->IsCa();
  promise.set_value(isCaOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::MyProviderOutput>
    CertSignRequestSkeletonImpl::MyProvider()
{
  // Not present in autosar file
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::MyProviderOutput>::PromiseType promise;
  ara::crypto::ipccom::x509::csr::CertSignRequest::MyProviderOutput myProviderOutput;
  promise.set_value(myProviderOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::SubjectDnOutput>
    CertSignRequestSkeletonImpl::SubjectDn()
{
  logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl SubjectDn()";
  ara::core::Future<SubjectDnOutput>::PromiseType promise;
  SubjectDnOutput subDnResult;
  X509DN::Uptr x509DnUptr{certSignUptr_->SubjectDn()};

  if (x509DnUptr != nullptr)
  {
    logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl SubjectDn() pointer is not Null";
    std::shared_ptr<ara::crypto::common::datastorage::X509DnContainer> dnContainer{
        ara::crypto::common::datastorage::X509DnContainer::GetInstance()};
    CryptoServiceInstanceId localInstId{dnContainer->GetCryptoServiceInstanceId()};
    logger_.LogDebug() << "IPC-CORE::SubjectDn() ServiceInstanceId = " << localInstId;
    std::unique_ptr<ara::crypto::x509::X509DnSkeletonImpl> x509DnSkelImplUptr{
        std::make_unique<ara::crypto::x509::X509DnSkeletonImpl>(localInstId, std::move(x509DnUptr))};
    x509DnSkelImplUptr->OfferService();
    dnContainer->StoreObjectToContanier(localInstId, std::move(x509DnSkelImplUptr));
    subDnResult.x509DnInstanceId = localInstId;
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl SubjectDn() pointer is Null";
  }

  promise.set_value(subDnResult);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::SubjectPubKeyOutput>
    CertSignRequestSkeletonImpl::SubjectPubKey(const CryptoServiceInstanceId& cryptoProviderInstanceId)
{
  static_cast<void>(cryptoProviderInstanceId);
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::SubjectPubKeyOutput>::PromiseType
      promise;
  ara::crypto::ipccom::x509::csr::CertSignRequest::SubjectPubKeyOutput subjectKeyOutputObj;

  X509PublicKeyInfo::Uptrc publicKeyUptrc = certSignUptr_->SubjectPubKey();
  if (publicKeyUptrc != nullptr)
  {
    logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl SubjectPubKey() pointer is not Null";
    std::shared_ptr<ara::crypto::common::datastorage::X509PublicKeyInfoContainer> pubKeyContainer{
        ara::crypto::common::datastorage::X509PublicKeyInfoContainer::GetInstance()};
    CryptoServiceInstanceId publicKeylocalInstanceId{pubKeyContainer->GetCryptoServiceInstanceId()};

    std::unique_ptr<ara::crypto::x509::X509PublicKeyInfoSkeletonImpl> localPublicKeyUtr{
        std::make_unique<ara::crypto::x509::X509PublicKeyInfoSkeletonImpl>(
            publicKeylocalInstanceId, std::move(publicKeyUptrc))};
     localPublicKeyUtr->OfferService();
     pubKeyContainer->StoreObjectToContanier(publicKeylocalInstanceId, std::move(localPublicKeyUtr));
     subjectKeyOutputObj.x509PublicKeyInfoInstanceId = publicKeylocalInstanceId;
  }
  else
  {
    logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl SubjectPubKey() pointer is Null";
  }
  promise.set_value(subjectKeyOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::VerifyOutput>
    CertSignRequestSkeletonImpl::Verify()
{
  logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl Verify() -> success";
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::VerifyOutput>::PromiseType promise;
  ara::crypto::ipccom::x509::csr::CertSignRequest::VerifyOutput verifyOutputObj;
  verifyOutputObj.result = certSignUptr_->Verify();
  promise.set_value(verifyOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::VersionOutput>
    CertSignRequestSkeletonImpl::Version()
{
  logger_.LogDebug() << "IPC-CORE::CertSignRequestSkeletonImpl Version() -> success";
  ara::core::Future<ara::crypto::ipccom::x509::csr::CertSignRequest::VersionOutput>::PromiseType promise;
  ara::crypto::ipccom::x509::csr::CertSignRequest::VersionOutput versionOutputObj;
  versionOutputObj.result = certSignUptr_->Verify();
  promise.set_value(versionOutputObj);
  return promise.get_future();
}

} // namespace x509
} // namespace crypto
} // namespace ara
