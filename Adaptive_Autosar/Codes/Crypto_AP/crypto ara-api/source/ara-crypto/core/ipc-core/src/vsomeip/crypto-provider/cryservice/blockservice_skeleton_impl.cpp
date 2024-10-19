// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : blockservice_skeleton_impl.cpp
//
// Purpose     : crypto object implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "blockservice_skeleton_impl.h"

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
BlockServiceSkeletonImpl::BlockServiceSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::BlockService::Uptr blockServicePtr) noexcept :
    ara::crypto::ipccom::cryp::cryservice::blockservice::skeleton::BlockServiceSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    blockServiceUptr_{std::move(blockServicePtr)}
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl() constructor";
}
ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetActualIvBitLengthOutput>
    BlockServiceSkeletonImpl::GetActualIvBitLength(const CryptoObjectUidIPC& ivUid, const Boolean& isOptional)
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> GetActualIvBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::
          GetActualIvBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetActualIvBitLengthOutput
      actualIvBitLengthOutput;

  CryptoObjectUid cryptoObjUid;

  if (isOptional == false)
  {
    cryptoObjUid.mGeneratorUid.mQwordMs = ivUid.mGeneratorUid.mQwordMs;
    cryptoObjUid.mGeneratorUid.mQwordLs = ivUid.mGeneratorUid.mQwordLs;
    cryptoObjUid.mVersionStamp = ivUid.mVersionStamp;

    ara::core::Optional<CryptoObjectUid> cryptoObjOptional{cryptoObjUid};
 // Forward call to the GetActualIvBitLength function.
    std::size_t const result{blockServiceUptr_->GetActualIvBitLength(cryptoObjOptional)};
    logger_.LogDebug() << "IPC-CORE:: ActualIvBitLength(isOptional == false) length = " << result;
    actualIvBitLengthOutput.response = result;
  }
  else
  {
    ara::core::Optional<CryptoObjectUid> cryptoObjOptional{cryptoObjUid};
    std::size_t const result{blockServiceUptr_->GetActualIvBitLength(cryptoObjOptional)};
    logger_.LogDebug() << "IPC-CORE:: ActualIvBitLength(isOptional == true) length = " << result;
    actualIvBitLengthOutput.response = result;
  }
  promise.set_value(actualIvBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetActualKeyBitLengthOutput>
    BlockServiceSkeletonImpl::GetActualKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> GetActualKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::
          GetActualKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetActualKeyBitLengthOutput
      actualKeyBitLengthOutput;
 // Forward call to the GetActualKeyBitLength function.
  std::size_t keyBitLength{blockServiceUptr_->GetActualKeyBitLength()};
  logger_.LogDebug() << "IPC-CORE::GetActualKeyBitLength = " << keyBitLength;
  actualKeyBitLengthOutput.response = keyBitLength;
  promise.set_value(actualKeyBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetActualKeyCOUIDOutput>
    BlockServiceSkeletonImpl::GetActualKeyCOUID()
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> GetActualKeyCOUID()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetActualKeyCOUIDOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetActualKeyCOUIDOutput
      actualKeyCOUIDOutput;
 // Forward call to the GetActualKeyCOUID function.
  CryptoObjectUid retValue = blockServiceUptr_->GetActualKeyCOUID();
  logger_.LogDebug() << "IPC-CORE:: GetActualKeyCOUID(), mQwordMs = " << retValue.mGeneratorUid.mQwordMs;
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordMs = retValue.mGeneratorUid.mQwordMs;
  logger_.LogDebug() << "IPC-CORE:: GetActualKeyCOUID(), mQwordLs = " << retValue.mGeneratorUid.mQwordLs;
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordLs = retValue.mGeneratorUid.mQwordLs;
  logger_.LogDebug() << "IPC-CORE:: GetActualKeyCOUID(), mVersionStamp = " << retValue.mVersionStamp;
  actualKeyCOUIDOutput.response.mVersionStamp = retValue.mVersionStamp;
  promise.set_value(actualKeyCOUIDOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetAllowedUsageOutput>
    BlockServiceSkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> GetAllowedUsage()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetAllowedUsageOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetAllowedUsageOutput
      getAllowedUsageOutput;
 // Forward call to the GetAllowedUsage function.
  getAllowedUsageOutput.response = blockServiceUptr_->GetAllowedUsage();
  logger_.LogDebug() << "IPC-CORE:: GetAllowedUsage() flag = " << getAllowedUsageOutput.response;
  promise.set_value(getAllowedUsageOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetBlockSizeOutput>
    BlockServiceSkeletonImpl::GetBlockSize()
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> GetBlockSize()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetBlockSizeOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetBlockSizeOutput blockSizeOutput;
 // Forward call to the GetBlockSize function.
  std::size_t blockSize{blockServiceUptr_->GetBlockSize()};
  logger_.LogDebug() << "IPC-CORE:: BlockSize = " << blockSize;  
  blockSizeOutput.response = blockSize;
  promise.set_value(blockSizeOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetIvSizeOutput>
    BlockServiceSkeletonImpl::GetIvSize()
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> GetIvSize()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetIvSizeOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetIvSizeOutput getIvSizeOutputObj;
 // Forward call to the GetIvSize function.
  std::size_t ivSize{blockServiceUptr_->GetIvSize()};
  logger_.LogDebug() << "IPC-CORE:: IvSize = " << ivSize;
  getIvSizeOutputObj.response = ivSize;
  promise.set_value(getIvSizeOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetMaxKeyBitLengthOutput>
    BlockServiceSkeletonImpl::GetMaxKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> GetMaxKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::
          GetMaxKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetMaxKeyBitLengthOutput
      maxKeyBitLengthOutput;
 // Forward call to the GetMaxKeyBitLength function.
  std::size_t maxKeyBitLength{blockServiceUptr_->GetMaxKeyBitLength()};
  logger_.LogDebug() << "IPC-CORE:: MaxKeyBitLength = " << maxKeyBitLength;
  maxKeyBitLengthOutput.response = maxKeyBitLength;
  promise.set_value(maxKeyBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetMinKeyBitLengthOutput>
    BlockServiceSkeletonImpl::GetMinKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> GetMinKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::
          GetMinKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::GetMinKeyBitLengthOutput
      minKeyBitLengthOutput;
 // Forward call to the GetMinKeyBitLength function.
  std::size_t minKeyBitLength{blockServiceUptr_->GetMinKeyBitLength()};
  logger_.LogDebug() << "IPC-CORE:: MinKeyBitLength = " << minKeyBitLength;
  minKeyBitLengthOutput.response = minKeyBitLength;
  promise.set_value(minKeyBitLengthOutput);
  return promise.get_future();
}
ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::IsKeyAvailableOutput>
    BlockServiceSkeletonImpl::IsKeyAvailable()
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> IsKeyAvailable()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::IsKeyAvailableOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::IsKeyAvailableOutput keyAvailableOutput;
 // Forward call to the IsKeyAvailable function.
  bool isSuccess{blockServiceUptr_->IsKeyAvailable()};
  logger_.LogDebug() << "IPC-CORE:: IsKeyAvailable() result = " << isSuccess;
  keyAvailableOutput.response = isSuccess;
  promise.set_value(keyAvailableOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::IsKeyBitLengthSupportedOutput>
    BlockServiceSkeletonImpl::IsKeyBitLengthSupported(const uint64& keyBitLength)
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> IsKeyBitLengthSupported()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::
          IsKeyBitLengthSupportedOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::IsKeyBitLengthSupportedOutput
      keyBitLengthSupportedOutput;
 // Forward call to the IsKeyBitLengthSupported function.
  bool result{blockServiceUptr_->IsKeyBitLengthSupported(keyBitLength)};
  logger_.LogDebug() << "IPC-CORE::IsKeyBitLengthSupported() result = " << result;
  keyBitLengthSupportedOutput.response = result;
  promise.set_value(keyBitLengthSupportedOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::IsValidIvSizeOutput>
    BlockServiceSkeletonImpl::IsValidIvSize(const uint64& ivSize)
{
  logger_.LogDebug() << "IPC-CORE::BlockServiceSkeletonImpl -> IsValidIvSize()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::IsValidIvSizeOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::blockservice::BlockService::IsValidIvSizeOutput validIvSizeOutput;
 // Forward call to the IsValidIvSize function.
  bool result{blockServiceUptr_->IsValidIvSize(ivSize)};
  logger_.LogDebug() << "IPC-CORE:: ValidIvSize = " << result;
  validIvSizeOutput.response = result;
  promise.set_value(validIvSizeOutput);
  return promise.get_future();
}

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
