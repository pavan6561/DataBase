// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : digestservice_proxy_impl.cpp
//
// Purpose     : Crypto object proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "digestservice_proxy_impl.h"

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
DigestServiceProxyImpl::DigestServiceProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    cryptoObjServiceInstanceID_{serviceInstanceID}, logger_{ara::log::CreateLogger("COPI",
                                                        "Crypto Object Proxy Impl",
                                                        ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void DigestServiceProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  // To get the proxy handler.
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for DigestServiceProxyImpl : Successful";
    digestServiceHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for DigestServiceProxyImpl : Failed";
  }
}

std::size_t DigestServiceProxyImpl::GetDigestSize() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward the call to the GetDigestSize method.
  auto response {digestServiceHandler_->GetDigestSize().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService GetDigestSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

ara::core::Result<bool> DigestServiceProxyImpl::Compare(ReadOnlyMemRegion expected, std::size_t offset) const
    noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  std::size_t result{0};
  CryptoReadOnlyMemRegion readOnlyMemUint8t(expected.begin(), expected.end());
  // Forward the call to the Compare method.
  auto response {digestServiceHandler_->Compare(readOnlyMemUint8t, offset).GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> Compare() result = " << result;
    return ara::core::Result<bool>::FromValue(result);
  }
  else
  {
    logger_.LogError() << "IPC-APP:: Compare() Error: " << response.Error().Message();
    return ara::core::Result<bool>::FromError(
        static_cast<SecurityErrorDomain::Errc>(response.Error().Value()));
  }
}

bool DigestServiceProxyImpl::IsStarted() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  bool result{0};
  // Forward the call to the IsStarted method.
  auto response {digestServiceHandler_->IsStarted().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> IsStarted() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool DigestServiceProxyImpl::IsFinished() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  bool result{0};
  // Forward the call to the IsFinished method.
  auto response {digestServiceHandler_->IsFinished().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> IsFinished result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t DigestServiceProxyImpl::GetIvSize() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward the call to the GetIvSize method.
  auto response{digestServiceHandler_->GetIvSize().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> GetIvSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t DigestServiceProxyImpl::GetBlockSize() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward the call to the GetBlockSize method.
  auto response = digestServiceHandler_->GetBlockSize().GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> GetBlockSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t DigestServiceProxyImpl::GetActualIvBitLength(ara::core::Optional<CryptoObjectUid> ivUid) const
    noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  std::size_t result{0};
  CryptoObjectUidIPC uidIpcObj;
  Boolean isOptional;

  if (ivUid.has_value())
  {
    CryptoObjectUid& cryObj = *ivUid;
    uidIpcObj.mGeneratorUid.mQwordMs = cryObj.mGeneratorUid.mQwordMs;
    uidIpcObj.mGeneratorUid.mQwordLs = cryObj.mGeneratorUid.mQwordLs;
    uidIpcObj.mVersionStamp = cryObj.mVersionStamp;
    isOptional = false;
  }
  else
  {
    isOptional = true;
  }
  // Forward the call to the GetActualIvBitLength method.
  auto response = digestServiceHandler_->GetActualIvBitLength(uidIpcObj, isOptional).GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> GetActualIvBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool DigestServiceProxyImpl::IsValidIvSize(std::size_t ivSize) const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  bool result{false};
  // Forward the call to the IsValidIvSize method.
  auto response = digestServiceHandler_->IsValidIvSize(ivSize).GetResult();
  if (response.HasValue())
  {
    result = response.Value().result;
    logger_.LogDebug() << "IPC-APP::DigestService -> IsValidIvSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool DigestServiceProxyImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  bool result{false};
  // Forward the call to the IsKeyBitLengthSupported method.
  auto response = digestServiceHandler_->IsKeyBitLengthSupported(keyBitLength).GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> IsKeyBitLengthSupported() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t DigestServiceProxyImpl::GetMinKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward the call to the GetMinKeyBitLength method.
  auto response = digestServiceHandler_->GetMinKeyBitLength().GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> GetMinKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t DigestServiceProxyImpl::GetMaxKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward the call to the GetMaxKeyBitLength method.
  auto response = digestServiceHandler_->GetMaxKeyBitLength().GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> GetMaxKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t DigestServiceProxyImpl::GetActualKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward the call to the GetActualKeyBitLength method.
  auto response{digestServiceHandler_->GetActualKeyBitLength().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> GetActualKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

AllowedUsageFlags DigestServiceProxyImpl::GetAllowedUsage() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  AllowedUsageFlags result{0};
  // Forward the call to the GetAllowedUsage method.
  auto response{digestServiceHandler_->GetAllowedUsage().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> GetAllowedUsage() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

CryptoObjectUid DigestServiceProxyImpl::GetActualKeyCOUID() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  CryptoObjectUid result{0};
  // Forward the call to the GetActualKeyCOUID method.
  auto retValue {digestServiceHandler_->GetActualKeyCOUID().GetResult()};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Success";
    result.mGeneratorUid.mQwordMs = retValue.Value().response.mGeneratorUid.mQwordMs;
    logger_.LogDebug() << "IPC-APP::GetActualKeyCOUID() -> mQwordMs =  " << result.mGeneratorUid.mQwordMs;
    result.mGeneratorUid.mQwordLs = retValue.Value().response.mGeneratorUid.mQwordLs;
    logger_.LogDebug() << "IPC-APP::GetActualKeyCOUID() -> mQwordMs =  " << result.mGeneratorUid.mQwordLs;
    result.mVersionStamp = retValue.Value().response.mVersionStamp;
    logger_.LogDebug() << "IPC-APP::GetActualKeyCOUID() -> mVersionStamp =  " << result.mVersionStamp;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool DigestServiceProxyImpl::IsKeyAvailable() const noexcept
{
  logger_.LogDebug() << "IPC-APP::DigestService -> " << __FUNCTION__;
  bool result{0};
  // Forward the call to the IsKeyAvailable method.
  auto response {digestServiceHandler_->IsKeyAvailable().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::DigestService -> IsKeyAvailable() result =  " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::DigestService -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
