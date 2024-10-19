// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : digestservice_skeleton_impl.cpp
//
// Purpose     : crypto object implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "digestservice_skeleton_impl.h"

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
DigestServiceSkeletonImpl::DigestServiceSkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::cryp::DigestService::Uptr digestUptr) noexcept :
    ara::crypto::ipccom::cryp::cryservice::digestservice::skeleton::DigestServiceSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    digestServiceUptr_{std::move(digestUptr)}
{
  logger_.LogDebug() << "IPC-CORE::DigestService DigestServiceSkeletonImpl()";
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::CompareOutput>
    DigestServiceSkeletonImpl::Compare(const CryptoReadOnlyMemRegion& expected, const uint64& offset)
{
  logger_.LogDebug() << "IPC-CORE::DigestService Compare()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::CompareOutput>::
      PromiseType promise;
 // Forward call to the Compare function.
  ara::core::Result<bool> const retValue{digestServiceUptr_->Compare(expected, offset)};
  if (retValue.HasValue())
  {
    ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::CompareOutput compareOutputObj;
    compareOutputObj.response = retValue.Value();
    logger_.LogDebug() << "IPC-CORE::DigestService Compare() result = " << compareOutputObj.response;
    promise.set_value(compareOutputObj);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::DigestService Compare() Error: "
                       << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetActualIvBitLengthOutput>
    DigestServiceSkeletonImpl::GetActualIvBitLength(
        const CryptoObjectUidIPC& ivUid, const Boolean& isOptional)
{
  logger_.LogDebug() << "IPC-CORE::DigestService GetActualIvBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::
          GetActualIvBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetActualIvBitLengthOutput
      actualIvBitLengthOutput;

  CryptoObjectUid cryptoObjUid;

  if (isOptional == false)
  {
    cryptoObjUid.mGeneratorUid.mQwordMs = ivUid.mGeneratorUid.mQwordMs;
    cryptoObjUid.mGeneratorUid.mQwordLs = ivUid.mGeneratorUid.mQwordLs;
    cryptoObjUid.mVersionStamp = ivUid.mVersionStamp;

    ara::core::Optional<CryptoObjectUid> cryptoObjOptional{cryptoObjUid};
 // Forward call to the GetActualIvBitLength function.
    std::size_t result = digestServiceUptr_->GetActualIvBitLength(cryptoObjOptional);
    logger_.LogDebug() << "IPC-CORE::DigestService GetActualIvBitLength(optional<cryptoObjUid>) result = "
        << result;
    actualIvBitLengthOutput.response = result;
  }
  else
  {
    ara::core::Optional<CryptoObjectUid> cryptoObjOptional{cryptoObjUid};
    std::size_t const result{digestServiceUptr_->GetActualIvBitLength(cryptoObjOptional)};
    logger_.LogDebug() << "IPC-CORE::DigestService GetActualIvBitLength() result = " << result;
    actualIvBitLengthOutput.response = result;
  }
  promise.set_value(actualIvBitLengthOutput);
  promise.get_future();
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetActualKeyBitLengthOutput>
    DigestServiceSkeletonImpl::GetActualKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::DigestService GetActualKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::
          GetActualKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetActualKeyBitLengthOutput
      getActualKeyBitLengthOutputObj;
 // Forward call to the GetActualKeyBitLength function.
  getActualKeyBitLengthOutputObj.response = digestServiceUptr_->GetActualKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::DigestService GetActualKeyBitLength() result = "
      << getActualKeyBitLengthOutputObj.response;
  promise.set_value(getActualKeyBitLengthOutputObj);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetActualKeyCOUIDOutput>
    DigestServiceSkeletonImpl::GetActualKeyCOUID()
{
  logger_.LogDebug() << "IPC-CORE::DigestService GetActualKeyCOUID()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::
          GetActualKeyCOUIDOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetActualKeyCOUIDOutput
      actualKeyCOUIDOutput;
 // Forward call to the GetActualKeyCOUID function.
  CryptoObjectUid retValue = digestServiceUptr_->GetActualKeyCOUID();
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordMs = retValue.mGeneratorUid.mQwordMs;
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordLs = retValue.mGeneratorUid.mQwordLs;
  actualKeyCOUIDOutput.response.mVersionStamp = retValue.mVersionStamp;
  logger_.LogDebug() << "IPC-CORE::DigestService GetActualKeyCOUID() mQwordMs = "
      << retValue.mGeneratorUid.mQwordMs;
  logger_.LogDebug() << "IPC-CORE::DigestService GetActualKeyCOUID() mQwordLs = "
      << retValue.mGeneratorUid.mQwordLs;
  logger_.LogDebug() << "IPC-CORE::DigestService GetActualKeyCOUID() mQwordLs = "
      << retValue.mVersionStamp;

  promise.set_value(actualKeyCOUIDOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetAllowedUsageOutput>
    DigestServiceSkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::DigestService GetAllowedUsage()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetAllowedUsageOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetAllowedUsageOutput
      getAllowedUsageOutputObj;
 // Forward call to the GetAllowedUsage function.
  getAllowedUsageOutputObj.response = digestServiceUptr_->GetAllowedUsage();
  logger_.LogDebug() << "IPC-CORE::DigestService GetAllowedUsage() result" 
      << getAllowedUsageOutputObj.response;
  promise.set_value(getAllowedUsageOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetBlockSizeOutput>
    DigestServiceSkeletonImpl::GetBlockSize()
{
  logger_.LogDebug() << "IPC-CORE::DigestService GetBlockSize()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetBlockSizeOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetBlockSizeOutput
      getBlockSizeOutputObj;
 // Forward call to the GetBlockSize function.
  getBlockSizeOutputObj.response = digestServiceUptr_->GetBlockSize();
  logger_.LogDebug() << "IPC-CORE::DigestService GetBlockSize() result = " << getBlockSizeOutputObj.response;
  promise.set_value(getBlockSizeOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetDigestSizeOutput>
    DigestServiceSkeletonImpl::GetDigestSize()
{
  logger_.LogDebug() << "IPC-CORE::DigestService GetDigestSize()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetDigestSizeOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetDigestSizeOutput
      getDigestSizeOutputObj;
 // Forward call to the GetDigestSize function.
  getDigestSizeOutputObj.response = digestServiceUptr_->GetDigestSize();
  logger_.LogDebug() << "IPC-CORE::DigestService GetDigestSize() result = "
      << getDigestSizeOutputObj.response;
  promise.set_value(getDigestSizeOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetIvSizeOutput>
    DigestServiceSkeletonImpl::GetIvSize()
{
  logger_.LogDebug() << "IPC-CORE::DigestService GetIvSize()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetIvSizeOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetIvSizeOutput getIvSizeOutputObj;
 // Forward call to the GetIvSize function.
  getIvSizeOutputObj.response = digestServiceUptr_->GetIvSize();
  logger_.LogDebug() << "IPC-CORE::DigestService GetIvSize() result = " << getIvSizeOutputObj.response;
  promise.set_value(getIvSizeOutputObj);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetMaxKeyBitLengthOutput>
    DigestServiceSkeletonImpl::GetMaxKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::DigestService GetMaxKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::
          GetMaxKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetMaxKeyBitLengthOutput
      getMaxKeyBitLengthOutputObj;
 // Forward call to the GetMaxKeyBitLength function.
  getMaxKeyBitLengthOutputObj.response = digestServiceUptr_->GetMaxKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::DigestService GetMaxKeyBitLength() result = "
      << getMaxKeyBitLengthOutputObj.response;
  promise.set_value(getMaxKeyBitLengthOutputObj);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetMinKeyBitLengthOutput>
    DigestServiceSkeletonImpl::GetMinKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::DigestService GetMinKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::
          GetMinKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::GetMinKeyBitLengthOutput
      getMinKeyBitLengthOutputObj;
 // Forward call to the GetMinKeyBitLength function.
  getMinKeyBitLengthOutputObj.response = digestServiceUptr_->GetMinKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::DigestService GetMinKeyBitLength() result = "
      << getMinKeyBitLengthOutputObj.response;
  promise.set_value(getMinKeyBitLengthOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsFinishedOutput>
    DigestServiceSkeletonImpl::IsFinished()
{
  logger_.LogDebug() << "IPC-CORE::DigestService IsFinished()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsFinishedOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsFinishedOutput isFinishedObj;
 // Forward call to the IsFinished function.
  isFinishedObj.response = digestServiceUptr_->IsFinished();
  logger_.LogDebug() << "IPC-CORE::DigestService IsFinished() result = " << isFinishedObj.response;
  promise.set_value(isFinishedObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsKeyAvailableOutput>
    DigestServiceSkeletonImpl::IsKeyAvailable()
{
  logger_.LogDebug() << "IPC-CORE::DigestService IsKeyAvailable()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsKeyAvailableOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsKeyAvailableOutput
      keyAvailableOutput;
 // Forward call to the IsKeyAvailable function.
  keyAvailableOutput.response = digestServiceUptr_->IsKeyAvailable();
  logger_.LogDebug() << "IPC-CORE::DigestService IsKeyAvailable() result = " << keyAvailableOutput.response;
  promise.set_value(keyAvailableOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsKeyBitLengthSupportedOutput>
    DigestServiceSkeletonImpl::IsKeyBitLengthSupported(const uint64& keyBitLength)
{
  logger_.LogDebug() << "IPC-CORE::DigestService IsKeyBitLengthSupported()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::
          IsKeyBitLengthSupportedOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsKeyBitLengthSupportedOutput
      isKeyBitLengthSupportedOutputObj;
 // Forward call to the IsKeyBitLengthSupported function.
  isKeyBitLengthSupportedOutputObj.response = digestServiceUptr_->IsKeyBitLengthSupported(keyBitLength);
  logger_.LogDebug() << "IPC-CORE::DigestService IsKeyBitLengthSupported() result = "
      << isKeyBitLengthSupportedOutputObj.response;
  promise.set_value(isKeyBitLengthSupportedOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsStartedOutput>
    DigestServiceSkeletonImpl::IsStarted()
{
  logger_.LogDebug() << "IPC-CORE::DigestService IsStarted()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsStartedOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsStartedOutput isStartedObj;
 // Forward call to the IsStarted function.
  isStartedObj.response = digestServiceUptr_->IsStarted();
  logger_.LogDebug() << "IPC-CORE::DigestService IsStarted() result = " << isStartedObj.response;
  promise.set_value(isStartedObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsValidIvSizeOutput>
    DigestServiceSkeletonImpl::IsValidIvSize(const uint64& ivSize)
{
  logger_.LogDebug() << "IPC-CORE::DigestService IsValidIvSize()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsValidIvSizeOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::cryservice::digestservice::DigestService::IsValidIvSizeOutput validIvSizeOutput;
 // Forward call to the IsValidIvSize function.
  validIvSizeOutput.result = digestServiceUptr_->IsValidIvSize(ivSize);
  logger_.LogDebug() << "IPC-CORE::DigestService IsValidIvSize() result = " << validIvSizeOutput.result;
  promise.set_value(validIvSizeOutput);
  return promise.get_future();
}

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
