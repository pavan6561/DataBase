// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : cryptoservice_skeleton_impl.cpp
//
// Purpose     : crypto object implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "cryptoservice_skeleton_impl.h"

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
CryptoServiceSkeletonImpl::CryptoServiceSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::CryptoService::Uptr cryptoServiceUptr) noexcept :
    ara::crypto::ipccom::cryp::cryservice::cryptoservice::skeleton::CryptoServiceSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    cryptoUptr_{std::move(cryptoServiceUptr)}
{
  logger_.LogDebug() << "IPC-CORE::CryptoService CryptoServiceSkeletonImpl()";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetActualKeyBitLengthOutput>
    CryptoServiceSkeletonImpl::GetActualKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::CryptoService GetActualKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::
          GetActualKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetActualKeyBitLengthOutput
      actualKeyBitLengthOutput;
 // Forward call to the GetActualKeyBitLength function.
  actualKeyBitLengthOutput.response = cryptoUptr_->GetActualKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::CryptoService GetActualKeyBitLength() result = "
      << actualKeyBitLengthOutput.response;
  promise.set_value(actualKeyBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetActualKeyCOUIDOutput>
    CryptoServiceSkeletonImpl::GetActualKeyCOUID()
{
  logger_.LogDebug() << "IPC-CORE::CryptoService GetActualKeyCOUID()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::
          GetActualKeyCOUIDOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetActualKeyCOUIDOutput
      actualKeyCOUIDOutput;
 // Forward call to the GetActualKeyCOUID function.
  CryptoObjectUid retValue = cryptoUptr_->GetActualKeyCOUID();
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordMs = retValue.mGeneratorUid.mQwordMs;
  logger_.LogDebug() << "IPC-CORE::CryptoService GetActualKeyCOUID() mQwordMs = "
      << actualKeyCOUIDOutput.response.mGeneratorUid.mQwordMs;
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordLs = retValue.mGeneratorUid.mQwordLs;
  logger_.LogDebug() << "IPC-CORE::CryptoService GetActualKeyCOUID() mQwordLs = "
      << actualKeyCOUIDOutput.response.mGeneratorUid.mQwordLs;
  actualKeyCOUIDOutput.response.mVersionStamp = retValue.mVersionStamp;
  logger_.LogDebug() << "IPC-CORE::CryptoService GetActualKeyCOUID() mVersionStamp = "
      << actualKeyCOUIDOutput.response.mVersionStamp;
  promise.set_value(actualKeyCOUIDOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetAllowedUsageOutput>
    CryptoServiceSkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::CryptoService GetAllowedUsage()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetAllowedUsageOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetAllowedUsageOutput
      getAllowedUsageOutputObj;
 // Forward call to the GetAllowedUsage function.
  getAllowedUsageOutputObj.response = cryptoUptr_->GetAllowedUsage();
  logger_.LogDebug() << "IPC-CORE::CryptoService GetAllowedUsage() result = "
      << getAllowedUsageOutputObj.response;
  promise.set_value(getAllowedUsageOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetBlockSizeOutput>
    CryptoServiceSkeletonImpl::GetBlockSize()
{
  logger_.LogDebug() << "IPC-CORE::CryptoService GetBlockSize()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetBlockSizeOutput>::
      PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetBlockSizeOutput
      getBlockSizeOutputObj;
 // Forward call to the GetBlockSize function.
  getBlockSizeOutputObj.response = cryptoUptr_->GetBlockSize();
  logger_.LogDebug() << "IPC-CORE::CryptoService GetBlockSize() result = "
      << getBlockSizeOutputObj.response;
  promise.set_value(getBlockSizeOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxInputSizeOutput>
    CryptoServiceSkeletonImpl::GetMaxInputSize(const boolean& suppressPadding)
{
  logger_.LogDebug() << "IPC-CORE::CryptoService GetMaxInputSize()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxInputSizeOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxInputSizeOutput
      maxInputSizeOutput;
 // Forward call to the GetMaxInputSize function.
  maxInputSizeOutput.response = cryptoUptr_->GetMaxInputSize(suppressPadding);
  logger_.LogDebug() << "IPC-CORE::CryptoService GetMaxInputSize() result = "
      << maxInputSizeOutput.response;
  promise.set_value(maxInputSizeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxKeyBitLengthOutput>
    CryptoServiceSkeletonImpl::GetMaxKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::CryptoService GetMaxKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::
          GetMaxKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxKeyBitLengthOutput
      getMaxKeyBitLengthOutputObj;
 // Forward call to the GetMaxKeyBitLength function.
  getMaxKeyBitLengthOutputObj.response = cryptoUptr_->GetMaxKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::CryptoService GetMaxKeyBitLength() result = "
      << getMaxKeyBitLengthOutputObj.response;
  promise.set_value(getMaxKeyBitLengthOutputObj);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxOutputSizeOutput>
    CryptoServiceSkeletonImpl::GetMaxOutputSize(const Boolean& suppressPadding)
{
  logger_.LogDebug() << "IPC-CORE::CryptoService GetMaxOutputSize()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::
          GetMaxOutputSizeOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMaxOutputSizeOutput
      maxOutputSizeOutput;
 // Forward call to the GetMaxOutputSize function.
  maxOutputSizeOutput.response = cryptoUptr_->GetMaxOutputSize();
  logger_.LogDebug() << "IPC-CORE::CryptoService GetMaxOutputSize() result = "
      << maxOutputSizeOutput.response;
  promise.set_value(maxOutputSizeOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMinKeyBitLengthOutput>
    CryptoServiceSkeletonImpl::GetMinKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::CryptoService GetMinKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::
          GetMinKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::GetMinKeyBitLengthOutput
      minKeyBitLengthOutput;
 // Forward call to the GetMinKeyBitLength function.
  minKeyBitLengthOutput.response = cryptoUptr_->GetMinKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::CryptoService GetMinKeyBitLength() result = "
      << minKeyBitLengthOutput.response;
  promise.set_value(minKeyBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::IsKeyAvailableOutput>
    CryptoServiceSkeletonImpl::IsKeyAvailable()
{
  logger_.LogDebug() << "IPC-CORE::CryptoService IsKeyAvailable()";
  ara::core::Future<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::IsKeyAvailableOutput>::PromiseType
      promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::IsKeyAvailableOutput
      isKeyAvailableOutput;
 // Forward call to the IsKeyAvailable function.
  isKeyAvailableOutput.response = cryptoUptr_->IsKeyAvailable();
  logger_.LogDebug() << "IPC-CORE::CryptoService IsKeyAvailable() result = "
      << isKeyAvailableOutput.response;
  promise.set_value(isKeyAvailableOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::IsKeyBitLengthSupportedOutput>
    CryptoServiceSkeletonImpl::IsKeyBitLengthSupported(const uint64& keyBitLength)
{
  logger_.LogDebug() << "IPC-CORE::CryptoService IsKeyBitLengthSupported()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::
          IsKeyBitLengthSupportedOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::cryptoservice::CryptoService::IsKeyBitLengthSupportedOutput
      isKeyBitLengthSupported;
 // Forward call to the IsKeyBitLengthSupported function.
  isKeyBitLengthSupported.response = cryptoUptr_->IsKeyBitLengthSupported(keyBitLength);
  logger_.LogDebug() << "IPC-CORE::CryptoService IsKeyBitLengthSupported() result = "
      << isKeyBitLengthSupported.response;
  promise.set_value(isKeyBitLengthSupported);
  return promise.get_future();
}

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
