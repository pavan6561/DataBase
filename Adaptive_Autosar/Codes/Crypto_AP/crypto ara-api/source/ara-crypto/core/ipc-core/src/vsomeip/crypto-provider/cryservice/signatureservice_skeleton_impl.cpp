// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signatureservice_skeleton_impl.cpp
//
// Purpose     : crypto object implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "signatureservice_skeleton_impl.h"

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
namespace cryservice
{
SignatureServiceSkeletonImpl::SignatureServiceSkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::SignatureService::Uptr uptr) noexcept :
    ara::crypto::ipccom::cryp::cryservice::signatureservice::skeleton::SignatureServiceSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    signatureUptr_{std::move(uptr)}
{
  logger_.LogDebug() << "IPC-CORE::SignatureService SignatureServiceSkeletonImpl()";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetActualKeyBitLengthOutput>
    SignatureServiceSkeletonImpl::GetActualKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::SignatureService GetActualKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          GetActualKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetActualKeyBitLengthOutput
      actualKeyBitLengthOutput;
 // Forward call to the GetActualKeyBitLength function.
  actualKeyBitLengthOutput.response = signatureUptr_->GetActualKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::SignatureService GetActualKeyBitLength() result = "
      << actualKeyBitLengthOutput.response;
  promise.set_value(actualKeyBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetActualKeyCOUIDOutput>
    SignatureServiceSkeletonImpl::GetActualKeyCOUID()
{
  logger_.LogDebug() << "IPC-CORE::SignatureService GetActualKeyCOUID()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          GetActualKeyCOUIDOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetActualKeyCOUIDOutput
      actualKeyCOUIDOutput;
 // Forward call to the GetActualKeyCOUID function.
  CryptoObjectUid const retValue{signatureUptr_->GetActualKeyCOUID()};
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordMs = retValue.mGeneratorUid.mQwordMs;
  logger_.LogDebug() << "IPC-CORE::SignatureService GetActualKeyCOUID() mQwordMs = "
      << actualKeyCOUIDOutput.response.mGeneratorUid.mQwordMs;
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordLs = retValue.mGeneratorUid.mQwordLs;
  logger_.LogDebug() << "IPC-CORE::SignatureService GetActualKeyCOUID() mQwordLs = "
      << actualKeyCOUIDOutput.response.mGeneratorUid.mQwordLs;
  actualKeyCOUIDOutput.response.mVersionStamp = retValue.mVersionStamp;
  logger_.LogDebug() << "IPC-CORE::SignatureService GetActualKeyCOUID() mVersionStamp = "
      << actualKeyCOUIDOutput.response.mVersionStamp;

  promise.set_value(actualKeyCOUIDOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetAllowedUsageOutput>
    SignatureServiceSkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::SignatureService GetAllowedUsage()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          GetAllowedUsageOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetAllowedUsageOutput
      allowedUsageOutput;
 // Forward call to the GetAllowedUsage function.
  allowedUsageOutput.response = signatureUptr_->GetAllowedUsage();
  logger_.LogDebug() << "IPC-CORE::SignatureService GetAllowedUsage() result = "
      << allowedUsageOutput.response;
  promise.set_value(allowedUsageOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetMaxKeyBitLengthOutput>
    SignatureServiceSkeletonImpl::GetMaxKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::SignatureService GetMaxKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          GetMaxKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetMaxKeyBitLengthOutput
      maxKeyBitLengthOutput;
 // Forward call to the GetMaxKeyBitLength function.
  maxKeyBitLengthOutput.response = signatureUptr_->GetMaxKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::SignatureService GetMaxKeyBitLength() result = "
      << maxKeyBitLengthOutput.response;
  promise.set_value(maxKeyBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetMinKeyBitLengthOutput>
    SignatureServiceSkeletonImpl::GetMinKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::SignatureService GetMinKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          GetMinKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetMinKeyBitLengthOutput
      minKeyBitLengthOutput;
 // Forward call to the GetMinKeyBitLength function.
  minKeyBitLengthOutput.response = signatureUptr_->GetMinKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::SignatureService GetMinKeyBitLength() result = "
      << minKeyBitLengthOutput.response;
  promise.set_value(minKeyBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetRequiredHashAlgIdOutput>
    SignatureServiceSkeletonImpl::GetRequiredHashAlgId()
{
  logger_.LogDebug() << "IPC-CORE::SignatureService GetRequiredHashAlgId()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          GetRequiredHashAlgIdOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetRequiredHashAlgIdOutput
      requiredHashAlgOutput;
 // Forward call to the GetRequiredHashAlgId function.
  requiredHashAlgOutput.response = signatureUptr_->GetRequiredHashAlgId();
  logger_.LogDebug() << "IPC-CORE::SignatureService GetRequiredHashAlgId() result = "
      << requiredHashAlgOutput.response;
  promise.set_value(requiredHashAlgOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetSignatureSizeOutput>
    SignatureServiceSkeletonImpl::GetSignatureSize()
{
  logger_.LogDebug() << "IPC-CORE::SignatureService GetSignatureSize()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          GetSignatureSizeOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetSignatureSizeOutput
      getSignatureSizeOutputObj;
 // Forward call to the GetSignatureSize function.
  getSignatureSizeOutputObj.response = signatureUptr_->GetSignatureSize();
  logger_.LogDebug() << "IPC-CORE::SignatureService GetSignatureSize() result = "
      <<  getSignatureSizeOutputObj.response;
  promise.set_value(getSignatureSizeOutputObj);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::IsKeyAvailableOutput>
    SignatureServiceSkeletonImpl::IsKeyAvailable()
{
  logger_.LogDebug() << "IPC-CORE::SignatureService IsKeyAvailable()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          IsKeyAvailableOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::IsKeyAvailableOutput
      keyAvailableOutput;
 // Forward call to the IsKeyAvailable function.
  keyAvailableOutput.response = signatureUptr_->IsKeyAvailable();
  logger_.LogDebug() << "IPC-CORE::SignatureService IsKeyAvailable() result = "
      << keyAvailableOutput.response;
  promise.set_value(keyAvailableOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::IsKeyBitLengthSupportedOutput>
    SignatureServiceSkeletonImpl::IsKeyBitLengthSupported(const uint64& keyBitLength)
{
  logger_.LogDebug() << "IPC-CORE::SignatureService IsKeyBitLengthSupported()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          IsKeyBitLengthSupportedOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::IsKeyBitLengthSupportedOutput
      isKeyBitLengthSupportedOutputObj;
 // Forward call to the IsKeyBitLengthSupported function.
  isKeyBitLengthSupportedOutputObj.response = signatureUptr_->IsKeyBitLengthSupported(keyBitLength);
  logger_.LogDebug() << "IPC-CORE::SignatureService IsKeyBitLengthSupported() result = "
      << isKeyBitLengthSupportedOutputObj.response;
  promise.set_value(isKeyBitLengthSupportedOutputObj);
  return promise.get_future();
}
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetRequiredHashSizeOutput>
      SignatureServiceSkeletonImpl::GetRequiredHashSize()
{
  logger_.LogDebug() << "IPC-CORE::SignatureService GetRequiredHashSize()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::
          GetRequiredHashSizeOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::signatureservice::SignatureService::GetRequiredHashSizeOutput
      getRequiredHashSizeOutputObj;
 // Forward call to the GetRequiredHashSize function.
  getRequiredHashSizeOutputObj.result = signatureUptr_->GetRequiredHashSize();
  logger_.LogDebug() << "IPC-CORE::SignatureService GetRequiredHashSize() result = "
      << getRequiredHashSizeOutputObj.result;
  promise.set_value(getRequiredHashSizeOutputObj);
  return promise.get_future();

}
} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
