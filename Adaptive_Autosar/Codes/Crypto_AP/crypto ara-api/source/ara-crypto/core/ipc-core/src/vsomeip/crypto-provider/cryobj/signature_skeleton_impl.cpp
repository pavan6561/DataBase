// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature_skeleton_impl.cpp
//
// Purpose     : signature implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "signature_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for crypto objects.
namespace cryobj
{
SignatureSkeletonImpl::SignatureSkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::Signature::Uptrc signatureUtrc) noexcept :
    ara::crypto::ipccom::cryp::cryobj::signature::skeleton::SignatureSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    signatureUptrc_{std::move(signatureUtrc)}
{
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::ExportPubliclyOutput>
    SignatureSkeletonImpl::ExportPublicly(const CryptoFormatId& FormatId)
{
  logger_.LogDebug() << "IPC-CORE::Signature Object ExportPublicly()";
  ara::core::Future<ExportPubliclyOutput>::PromiseType promise;
  static_cast<void>(FormatId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetCryptoPrimitiveIdOutput>
    SignatureSkeletonImpl::GetCryptoPrimitiveId()
{
  logger_.LogDebug() << "IPC-CORE::Signature Object GetCryptoPrimitiveId()";
  ara::core::Future<GetCryptoPrimitiveIdOutput>::PromiseType promise;
  GetCryptoPrimitiveIdOutput primitiveOutput;
  ara::crypto::cryp::CryptoPrimitiveId::Uptr const primitiveUptr{signatureUptrc_->GetCryptoPrimitiveId()};
  ara::core::StringView const primitiveNameLocal{primitiveUptr->GetPrimitiveName()};
  ara::crypto::cryp::CryptoPrimitiveId::AlgId const primitiveAlgIdLocal{primitiveUptr->GetPrimitiveId()};
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveName = primitiveNameLocal;
  primitiveOutput.cryptoPrimitiveIdUptrStruct.primitiveAlgId = primitiveAlgIdLocal;
  promise.set_value(primitiveOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetHashAlgIdOutput>
    SignatureSkeletonImpl::GetHashAlgId()
{
  logger_.LogDebug() << "IPC-CORE::Signature Object GetHashAlgId()";
  ara::core::Future<GetHashAlgIdOutput>::PromiseType promise;
  GetHashAlgIdOutput hashId;
  hashId.CryptoPrimitiveIdAlgId = signatureUptrc_->GetHashAlgId();
  promise.set_value(hashId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetObjectIdOutput>
    SignatureSkeletonImpl::GetObjectId()
{
  logger_.LogDebug() << "IPC-CORE::Signature Object GetObjectId()";
  ara::core::Future<GetObjectIdOutput>::PromiseType promise;
  GetObjectIdOutput objectIdOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier const localCryptoObjIdentifier{
      signatureUptrc_->GetObjectId()};
  objectIdOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  objectIdOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  objectIdOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(objectIdOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetPayloadSizeOutput>
    SignatureSkeletonImpl::GetPayloadSize()
{
  logger_.LogDebug() << "IPC-CORE::Signature Object GetPayloadSize()";
  ara::core::Future<GetPayloadSizeOutput>::PromiseType promise;
  GetPayloadSizeOutput payloadSizeeOutput;
  payloadSizeeOutput.PayloadSize = signatureUptrc_->GetPayloadSize();
  promise.set_value(payloadSizeeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::GetRequiredHashSizeOutput>
    SignatureSkeletonImpl::GetRequiredHashSize()
{
  logger_.LogDebug() << "IPC-CORE::Signature Object GetRequiredHashSize()";
  ara::core::Future<GetRequiredHashSizeOutput>::PromiseType promise;
  GetRequiredHashSizeOutput hashsizeOutput;
  hashsizeOutput.HashSize = signatureUptrc_->GetRequiredHashSize();
  promise.set_value(hashsizeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::HasDependenceOutput>
    SignatureSkeletonImpl::HasDependence()
{
  logger_.LogDebug() << "IPC-CORE::Signature Object HasDependence()";
  ara::core::Future<HasDependenceOutput>::PromiseType promise;
  HasDependenceOutput hasLocalOutput;
  ara::crypto::cryp::CryptoObject::COIdentifier const localCryptoObjIdentifier{
      signatureUptrc_->HasDependence()};
  hasLocalOutput.COIdentifier.mCOType = static_cast<CryptoObjectTypeIPC>(localCryptoObjIdentifier.mCOType);
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordLs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordLs;
  hasLocalOutput.COIdentifier.mCouid.mGeneratorUid.mQwordMs
      = localCryptoObjIdentifier.mCouid.mGeneratorUid.mQwordMs;
  hasLocalOutput.COIdentifier.mCouid.mVersionStamp = localCryptoObjIdentifier.mCouid.mVersionStamp;
  promise.set_value(hasLocalOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::IsExportableOutput>
    SignatureSkeletonImpl::IsExportable()
{
  logger_.LogDebug() << "IPC-CORE::Signature Object IsExportable()";
  ara::core::Future<IsExportableOutput>::PromiseType promise;
  IsExportableOutput isexportableOutput;
  isexportableOutput.Bool = signatureUptrc_->IsExportable();
  promise.set_value(isexportableOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryobj::signature::Signature::IsSessionOutput>
    SignatureSkeletonImpl::IsSession()
{
  logger_.LogDebug() << "IPC-CORE::Signature Object IsSession()";
  ara::core::Future<IsSessionOutput>::PromiseType promise;
  IsSessionOutput issessionOutput;
  issessionOutput.Bool = signatureUptrc_->IsSession();
  promise.set_value(issessionOutput);
  return promise.get_future();
}

ara::core::Future<void> SignatureSkeletonImpl::Save(const CryptoServiceInstanceId& IOInterfaceInstanceID)
{
  ara::core::Future<void>::PromiseType promise;
  static_cast<void>(IOInterfaceInstanceID);
  return promise.get_future();
}

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara