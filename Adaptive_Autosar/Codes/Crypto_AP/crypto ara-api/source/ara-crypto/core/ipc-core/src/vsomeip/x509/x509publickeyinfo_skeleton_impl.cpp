// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509publickeyinfo_skeleton_impl.cpp
//
// Purpose     : X509PublicKeyInfoSkeleton Domain Number skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "x509publickeyinfo_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
X509PublicKeyInfoSkeletonImpl::X509PublicKeyInfoSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::x509::X509PublicKeyInfo::Uptrc publicKeyInfoUptr) noexcept :
    ara::crypto::ipccom::x509::pki::skeleton::X509PublicKeyInfoSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    cryptoServiceInstanceId_{instanceId}, pkiUptr_{std::move(publicKeyInfoUptr)}
{
  logger_.LogDebug() << "X509PublicKeyInfoSkeletonImpl Constructor";
}

ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::ExportPubliclyOutput>
    X509PublicKeyInfoSkeletonImpl::ExportPublicly(const uint32& formatId)
{
  logger_.LogDebug() << "IPC-CORE::X509PublicKeyInfoSkeletonImpl ExportPublicly()";
  ara::core::Future<ExportPubliclyOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const retValue{pkiUptr_->ExportPublicly(formatId)};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::X509PublicKeyInfoSkeletonImpl ExportPublicly() -> success";
    ara::core::Vector<ara::core::Byte> tempVtr{retValue.Value()};
    ExportPubliclyOutput exportOutput;
    std::for_each(tempVtr.begin(), tempVtr.end(), [&exportOutput](ara::core::Byte i) {
      exportOutput.resultByteVector.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(exportOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::certificate GetFingerprint() Error ->" << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetAlgorithmIdOutput>
    X509PublicKeyInfoSkeletonImpl::GetAlgorithmId()
{
  logger_.LogDebug() << "IPC-CORE:X509PublicKeyInfoSkeletonImpl GetAlgorithmId()";
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetAlgorithmIdOutput>::PromiseType
      promise;

  ara::crypto::cryp::CryptoPrimitiveId::Uptrc primitiveUptrc = pkiUptr_->GetAlgorithmId();
  ara::core::StringView const primitiveNameLocal{ primitiveUptrc->GetPrimitiveName() };
  logger_.LogDebug() << "IPC-APP::GetAlgorithmId() -> PrimitiveName = " << primitiveNameLocal;
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{ primitiveUptrc->GetPrimitiveId() };
  logger_.LogDebug() << "IPC-APP::GetAlgorithmId() -> PrimitiveName = " << primitiveAlgIdLocal;
  GetAlgorithmIdOutput primitiveOutput;
  primitiveOutput.cryptoPrimitiveInstanceId.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveInstanceId.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetPublicKeyOutput>
    X509PublicKeyInfoSkeletonImpl::GetPublicKey()
{
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetPublicKeyOutput>::PromiseType
      promise;
  ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetPublicKeyOutput publicKeyOutput;
  ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc> publicKeyUptrc{pkiUptr_->GetPublicKey()};
  if (publicKeyUptrc.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:X509PublicKeyInfoSkeletonImpl GetPublicKey() -> successful";
    std::shared_ptr<ara::crypto::common::datastorage::PublicKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::PublicKeyContainer::GetInstance()};
    // create unique crypto service instance Id from skeleton data storage class for crypto objects
    CryptoServiceInstanceId publicKeylocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl> localPublicKeyUtr{
        std::make_unique<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl>(
            publicKeylocalInstanceId, std::move(publicKeyUptrc).Value())};
    localPublicKeyUtr->OfferService();
    dataStorageContainer->StoreObjectToContanier(publicKeylocalInstanceId, std::move(localPublicKeyUtr));
    publicKeyOutput.publicKeyInstanceId = publicKeylocalInstanceId;
    promise.set_value(publicKeyOutput);
  }
  else
  {
    promise.set_value(publicKeyOutput);
    logger_.LogDebug() << "IPC-CORE:X509PublicKeyInfoSkeletonImpl GetPublicKey() -> Error";
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetRequiredHashAlgIdOutput>
    X509PublicKeyInfoSkeletonImpl::GetRequiredHashAlgId()
{
  logger_.LogDebug() << "IPC-CORE:X509PublicKeyInfoSkeletonImpl GetRequiredHashAlgId() -> successful";
  ara::core::Future<
      ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetRequiredHashAlgIdOutput>::PromiseType promise;
  ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetRequiredHashAlgIdOutput reqHashAlgId;
  reqHashAlgId.cryptoAlgId = pkiUptr_->GetRequiredHashAlgId();
  promise.set_value(reqHashAlgId);
  return promise.get_future();
}
ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetRequiredHashSizeOutput>
    X509PublicKeyInfoSkeletonImpl::GetRequiredHashSize()
{
  logger_.LogDebug() << "IPC-CORE:X509PublicKeyInfoSkeletonImpl GetRequiredHashSize() -> successful";
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetRequiredHashSizeOutput>::PromiseType
      promise;
  ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetRequiredHashSizeOutput reqHashSizeOutput;
  reqHashSizeOutput.size_t = pkiUptr_->GetRequiredHashSize();
  promise.set_value(reqHashSizeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetSignatureSizeOutput>
    X509PublicKeyInfoSkeletonImpl::GetSignatureSize()
{
  logger_.LogDebug() << "IPC-CORE:X509PublicKeyInfoSkeletonImpl GetSignatureSize() -> successful";
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetSignatureSizeOutput>::PromiseType
      promise;
  ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::GetSignatureSizeOutput signatureSizeOutput;
  signatureSizeOutput.size_t = pkiUptr_->GetSignatureSize();
  promise.set_value(signatureSizeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::IsSameKeyOutput>
    X509PublicKeyInfoSkeletonImpl::IsSameKey(const CryptoServiceInstanceId& publicKeyInstanceId)
{
  ara::core::Future<ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::IsSameKeyOutput>::PromiseType promise;
  ara::crypto::ipccom::x509::pki::X509PublicKeyInfo::IsSameKeyOutput sameKeyOutput;

  std::shared_ptr<ara::crypto::common::datastorage::PublicKeyContainer> dataStorageContainer{
      ara::crypto::common::datastorage::PublicKeyContainer::GetInstance()};

  logger_.LogInfo() << " IPC-CORE::X509PublicKeyInfoSkeletonImpl IsSameKey() -> successful "
                    << publicKeyInstanceId;
  std::unique_ptr<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl>& publicKeySklImpl{
      dataStorageContainer->GetObjectFromContanier(publicKeyInstanceId)};

  auto& publicKeyUptr{publicKeySklImpl->GetPublicKeyUptrInstance()};
  sameKeyOutput.result = pkiUptr_->IsSameKey(*publicKeyUptr);

  promise.set_value(sameKeyOutput);
  return promise.get_future();
}

} // namespace x509
} // namespace crypto
} // namespace ara
