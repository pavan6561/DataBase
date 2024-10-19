// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : extensionservice_skeleton_impl.cpp
//
// Purpose     : crypto object implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "extensionservice_skeleton_impl.h"

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
ExtensionServiceSkeletonImpl::ExtensionServiceSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::ExtensionService::Uptr extensionUptr) noexcept :
    ara::crypto::ipccom::cryp::cryservice::extensionservice::skeleton::ExtensionServiceSkeleton(
        ara::com::InstanceIdentifier(instanceId)),
    extensionServiceUptr(std::move(extensionUptr))
{
  logger_.LogDebug() << "IPC-CORE::ExtensionServiceSkeletonImpl()";
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetActualKeyBitLengthOutput>
    ExtensionServiceSkeletonImpl::GetActualKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::ExtensionService GetActualKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::
          GetActualKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetActualKeyBitLengthOutput
      actualKeyBitLengthOutput;
 // Forward call to the GetActualKeyBitLength function.
  actualKeyBitLengthOutput.response = extensionServiceUptr->GetActualKeyBitLength();
  logger_.LogDebug() << "IPC-CORE::GetActualKeyBitLength() result = " << actualKeyBitLengthOutput.response;
  promise.set_value(actualKeyBitLengthOutput);
  return promise.get_future();
}
ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetActualKeyCOUIDOutput>
    ExtensionServiceSkeletonImpl::GetActualKeyCOUID()
{
  logger_.LogDebug() << "IPC-CORE::ExtensionService GetActualKeyCOUID()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::
          GetActualKeyCOUIDOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetActualKeyCOUIDOutput
      actualKeyCOUIDOutput;
 // Forward call to the GetActualKeyCOUID function.
  CryptoObjectUid const retValue{extensionServiceUptr->GetActualKeyCOUID()};
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordMs = retValue.mGeneratorUid.mQwordMs;
  logger_.LogDebug() << "IPC-CORE::GetActualKeyCOUID() mQwordMs = "
      << actualKeyCOUIDOutput.response.mGeneratorUid.mQwordMs;
  actualKeyCOUIDOutput.response.mGeneratorUid.mQwordLs = retValue.mGeneratorUid.mQwordLs;
  logger_.LogDebug() << "IPC-CORE::GetActualKeyCOUID() mQwordLs = "
      << actualKeyCOUIDOutput.response.mGeneratorUid.mQwordLs;
  actualKeyCOUIDOutput.response.mVersionStamp = retValue.mVersionStamp;
  logger_.LogDebug() << "IPC-CORE::GetActualKeyCOUID() mVersionStamp = "
      << actualKeyCOUIDOutput.response.mVersionStamp;

  promise.set_value(actualKeyCOUIDOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetAllowedUsageOutput>
    ExtensionServiceSkeletonImpl::GetAllowedUsage()
{
  logger_.LogDebug() << "IPC-CORE::ExtensionService GetAllowedUsage()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::
          GetAllowedUsageOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetAllowedUsageOutput
      actualKeyCOUIDOutput;
 // Forward call to the GetAllowedUsage function.
  actualKeyCOUIDOutput.response = extensionServiceUptr->GetAllowedUsage();
  logger_.LogDebug() << "IPC-CORE:: GetAllowedUsage() result = " << actualKeyCOUIDOutput.response;
  promise.set_value(actualKeyCOUIDOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetMaxKeyBitLengthOutput>
    ExtensionServiceSkeletonImpl::GetMaxKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::ExtensionService GetMaxKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::
          GetMaxKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetMaxKeyBitLengthOutput
      maxKeyBitLengthOutput;
 // Forward call to the GetMaxKeyBitLength function.
  std::size_t retValue{extensionServiceUptr->GetMaxKeyBitLength()};
  maxKeyBitLengthOutput.response = retValue;
  logger_.LogDebug() << "IPC-CORE:: GetMaxKeyBitLength() result = " << maxKeyBitLengthOutput.response;
  promise.set_value(maxKeyBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetMinKeyBitLengthOutput>
    ExtensionServiceSkeletonImpl::GetMinKeyBitLength()
{
  logger_.LogDebug() << "IPC-CORE::ExtensionService GetMinKeyBitLength()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::
          GetMinKeyBitLengthOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::GetMinKeyBitLengthOutput
      minKeyBitLengthOutput;
 // Forward call to the GetMinKeyBitLength function.
  std::size_t retValue{extensionServiceUptr->GetMinKeyBitLength()};
  minKeyBitLengthOutput.response = retValue;
  logger_.LogDebug() << "IPC-CORE:: GetMinKeyBitLength() result = " << minKeyBitLengthOutput.response;
  promise.set_value(minKeyBitLengthOutput);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::IsKeyAvailableOutput>
    ExtensionServiceSkeletonImpl::IsKeyAvailable()
{
  logger_.LogDebug() << "IPC-CORE::ExtensionService IsKeyAvailable()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::
          IsKeyAvailableOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::IsKeyAvailableOutput
      isKeyAvailableResult;
 // Forward call to the IsKeyAvailable function.
  isKeyAvailableResult.response = extensionServiceUptr->IsKeyAvailable();
  logger_.LogDebug() << "IPC-CORE:: IsKeyAvailable() result = " << isKeyAvailableResult.response;
  promise.set_value(isKeyAvailableResult);
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::IsKeyBitLengthSupportedOutput>
    ExtensionServiceSkeletonImpl::IsKeyBitLengthSupported(const uint64& keyBitLength)
{
  logger_.LogDebug() << "IPC-CORE::ExtensionService IsKeyBitLengthSupported()";
  ara::core::Future<ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::
          IsKeyBitLengthSupportedOutput>::PromiseType promise;
  ara::crypto::ipccom::cryp::cryservice::extensionservice::ExtensionService::IsKeyBitLengthSupportedOutput
      minKeyBitLengthOutput;
 // Forward call to the IsKeyBitLengthSupported function.
  minKeyBitLengthOutput.response = extensionServiceUptr->IsKeyBitLengthSupported(keyBitLength);
  logger_.LogDebug() << "IPC-CORE:: IsKeyBitLengthSupported() result = " << minKeyBitLengthOutput.response;
  promise.set_value(minKeyBitLengthOutput);
  return promise.get_future();
}

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
