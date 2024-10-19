// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : extensionservice_proxy_impl.cpp
//
// Purpose     : Crypto object proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "extensionservice_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for crypto object.
namespace cryservice
{
ExtensionServiceProxyImpl::ExtensionServiceProxyImpl(
    const CryptoServiceInstanceId serviceInstanceID) noexcept :
    cryptoObjServiceInstanceID_{serviceInstanceID},
    logger_{ara::log::CreateLogger("COPI", "Crypto Object Proxy Impl", ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void ExtensionServiceProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  // To get the proxy handler.
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for ExtensionServiceProxyImpl : Successful";
    extensionServiceHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for ExtensionServiceProxyImpl : Failed";
  }
}

bool ExtensionServiceProxyImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  logger_.LogDebug() << "IPC-APP::ExtensionService -> " << __FUNCTION__;
  bool isKeyLengthSupported{false};
  // Forward call to the IsKeyBitLengthSupported method.
  auto objResult = extensionServiceHandler_->IsKeyBitLengthSupported(keyBitLength).GetResult();
  if (objResult.HasValue())
  {
    isKeyLengthSupported = objResult.Value().response;
    logger_.LogDebug() << "IPC-APP::ExtensionService IsKeyBitLengthSupported() result =  "
        << isKeyLengthSupported;
  }
  else
  {
    logger_.LogError() << "IPC-APP::ExtensionService -> " << __FUNCTION__ << " : Failed";
  }
  return isKeyLengthSupported;
}

std::size_t ExtensionServiceProxyImpl::GetMinKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ExtensionService -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward call to the GetMinKeyBitLength method.
  auto retValue = extensionServiceHandler_->GetMinKeyBitLength().GetResult();
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::ExtensionService GetMinKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::ExtensionService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t ExtensionServiceProxyImpl::GetMaxKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ExtensionService -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward call to the GetMaxKeyBitLength method.
  auto retValue{extensionServiceHandler_->GetMaxKeyBitLength().GetResult()};
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::ExtensionService GetMaxKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::ExtensionService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t ExtensionServiceProxyImpl::GetActualKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ExtensionService -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward call to the GetActualKeyBitLength method.
  auto retValue{extensionServiceHandler_->GetActualKeyBitLength().GetResult()};
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::ExtensionService GetActualKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::ExtensionService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

AllowedUsageFlags ExtensionServiceProxyImpl::GetAllowedUsage() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ExtensionService -> " << __FUNCTION__;
  AllowedUsageFlags result{0};
  // Forward call to the GetAllowedUsage method.
  auto retValue = extensionServiceHandler_->GetAllowedUsage().GetResult();
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::ExtensionService GetAllowedUsage() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::ExtensionService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

CryptoObjectUid ExtensionServiceProxyImpl::GetActualKeyCOUID() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ExtensionService -> " << __FUNCTION__;
  CryptoObjectUid result{0};
  // Forward call to the GetActualKeyCOUID method.
  auto retValue = extensionServiceHandler_->GetActualKeyCOUID().GetResult();
  if (retValue.HasValue())
  {
    result.mGeneratorUid.mQwordMs = retValue.Value().response.mGeneratorUid.mQwordMs;
    logger_.LogDebug() << "IPC-APP::ExtensionService GetActualKeyCOUID() mQwordMs = "
        << result.mGeneratorUid.mQwordMs;
    result.mGeneratorUid.mQwordLs = retValue.Value().response.mGeneratorUid.mQwordLs;
    logger_.LogDebug() << "IPC-APP::ExtensionService GetActualKeyCOUID() mQwordLs = "
        << result.mGeneratorUid.mQwordLs;
    result.mVersionStamp = retValue.Value().response.mVersionStamp;
    logger_.LogDebug() << "IPC-APP::ExtensionService GetActualKeyCOUID() mVersionStamp = "
        << result.mVersionStamp;
  }
  else
  {
    logger_.LogError() << "IPC-APP::ExtensionService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool ExtensionServiceProxyImpl::IsKeyAvailable() const noexcept
{
  logger_.LogDebug() << "IPC-APP::ExtensionService -> " << __FUNCTION__;
  bool result{0};
  // Forward call to the IsKeyAvailable method.
  auto retValue{extensionServiceHandler_->IsKeyAvailable().GetResult()};
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::ExtensionService IsKeyAvailable() result =  " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::ExtensionService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
