// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : blockservice_proxy_impl.cpp
//
// Purpose     : Crypto object proxy implementation to forward functional calls from application to ipc-APP.
// -----------------------------------------------------------------------------------------------------------

#include "blockservice_proxy_impl.h"

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
BlockServiceProxyImpl::BlockServiceProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    cryptoObjServiceInstanceID_{serviceInstanceID}, logger_{ara::log::CreateLogger("COPI",
                                                        "Crypto BlockService Proxy Impl",
                                                        ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void BlockServiceProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  // To get the proxy handler.
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for BlockServiceProxyImpl : Successful";
    blockServiceHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for BlockServiceProxyImpl : Failed";
  }
}

std::size_t BlockServiceProxyImpl::GetIvSize() const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  std::size_t result{0};
  // To call the GetIvSize ara::com method.
  auto response{blockServiceHandler_->GetIvSize().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService GetIvSize() result = "<< result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t BlockServiceProxyImpl::GetBlockSize() const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  std::size_t result{0};
  // To call the GetBlockSize ara::com method.
  auto response{blockServiceHandler_->GetBlockSize().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService GetBlockSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool BlockServiceProxyImpl::IsValidIvSize(std::size_t ivSize) const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  bool result{0};
  // To call the IsValidIvSize ara::com method.
  auto response {blockServiceHandler_->IsValidIvSize(ivSize).GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService IsValidIvSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t BlockServiceProxyImpl::GetActualIvBitLength(ara::core::Optional<CryptoObjectUid> ivUid) const
    noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
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
  // To call the GetActualIvBitLength ara::com method.
  auto response = blockServiceHandler_->GetActualIvBitLength(uidIpcObj, isOptional).GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService GetActualIvBitLength() result = " << result;    
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool BlockServiceProxyImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  bool result{0};
  // To call the IsKeyBitLengthSupported ara::com method.
  auto response{blockServiceHandler_->IsKeyBitLengthSupported(keyBitLength).GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService IsKeyBitLengthSupported() result = " << result;    
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t BlockServiceProxyImpl::GetMinKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  std::size_t result{0};
  // To call the GetMinKeyBitLength ara::com method.
  auto response{blockServiceHandler_->GetMinKeyBitLength().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService GetMinKeyBitLength() result = " << result;    
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t BlockServiceProxyImpl::GetMaxKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  std::size_t result{0};
  // To call the GetMaxKeyBitLength ara::com method.
  auto response{blockServiceHandler_->GetMaxKeyBitLength().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService GetMaxKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t BlockServiceProxyImpl::GetActualKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  std::size_t result{0};
  // To call the GetActualKeyBitLength ara::com method.
  auto response{blockServiceHandler_->GetActualKeyBitLength().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService GetActualKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

AllowedUsageFlags BlockServiceProxyImpl::GetAllowedUsage() const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  AllowedUsageFlags result{0};
  // To call the GetAllowedUsage ara::com method.
  auto retValue{blockServiceHandler_->GetAllowedUsage().GetResult()};
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService GetAllowedUsage() result = " << result;    
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

CryptoObjectUid BlockServiceProxyImpl::GetActualKeyCOUID() const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  CryptoObjectUid result{0};
  // To call the GetActualKeyCOUID ara::com method.
  auto retValue{blockServiceHandler_->GetActualKeyCOUID().GetResult()};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Success";
    result.mGeneratorUid.mQwordMs = retValue.Value().response.mGeneratorUid.mQwordMs;
    logger_.LogDebug() << "IPC-APP::BlockService GetActualKeyCOUID() mQwordMs = "
        << result.mGeneratorUid.mQwordMs;
    result.mGeneratorUid.mQwordLs = retValue.Value().response.mGeneratorUid.mQwordLs;
    logger_.LogDebug() << "IPC-APP::BlockService GetActualKeyCOUID() mQwordLs = "
        << result.mGeneratorUid.mQwordLs;
    result.mVersionStamp = retValue.Value().response.mVersionStamp;
    logger_.LogDebug() << "IPC-APP::BlockService GetActualKeyCOUID() mVersionStamp = "
        << result.mVersionStamp;
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool BlockServiceProxyImpl::IsKeyAvailable() const noexcept
{
  logger_.LogDebug() << "IPC-APP::BlockService  -> " << __FUNCTION__;
  bool result{0};
  // To call the IsKeyAvailable ara::com method.
  auto response = blockServiceHandler_->IsKeyAvailable().GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-APP::BlockService IsKeyAvailable() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::BlockService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
