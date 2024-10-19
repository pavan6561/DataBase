// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : cryptoservice_proxy_impl.cpp
//
// Purpose     : Crypto object proxy implementation to forward functional calls from application to ipc-APP.
// -----------------------------------------------------------------------------------------------------------

#include "cryptoservice_proxy_impl.h"

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
CryptoServiceProxyImpl::CryptoServiceProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    cryptoObjServiceInstanceID_{serviceInstanceID}, logger_{ara::log::CreateLogger("COPI",
                                                        "Crypto Object Proxy Impl",
                                                        ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void CryptoServiceProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  // To get the proxy handler.
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for CryptoServiceProxyImpl : Successful";
    cryptoServiceHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for CryptoServiceProxyImpl : Failed";
  }
}

std::size_t CryptoServiceProxyImpl::GetMaxInputSize(bool suppressPadding) const noexcept
{
  // Get maximum expected size of the input data block.
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  std::size_t result{0};
  auto retValue{cryptoServiceHandler_->GetMaxInputSize(suppressPadding).GetResult()};
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::CryptoService GetMaxInputSize() result = " << result;    
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t CryptoServiceProxyImpl::GetMaxOutputSize(bool suppressPadding) const noexcept
{
  // To Get maximum possible size of the output data block.
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  std::size_t result{0};
  auto retValue{cryptoServiceHandler_->GetMaxOutputSize(suppressPadding).GetResult()};
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::CryptoService GetMaxOutputSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t CryptoServiceProxyImpl::GetBlockSize() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  std::size_t result{0};
  // To call the GetBlockSize ara::com method.
  auto retValue {cryptoServiceHandler_->GetBlockSize().GetResult()};
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::CryptoService GetBlockSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool CryptoServiceProxyImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  // To Verify supportness of specific key length by the context.
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  bool result{0};
  auto retValue{cryptoServiceHandler_->IsKeyBitLengthSupported(keyBitLength).GetResult()};
  // To check the return value.
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::CryptoService IsKeyBitLengthSupported() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t CryptoServiceProxyImpl::GetMinKeyBitLength() const noexcept
{
  // Get minimal supported key length in bits.
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  std::size_t result{0};
  auto retValue{cryptoServiceHandler_->GetMinKeyBitLength().GetResult()};
  // To check the return value.
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::CryptoService GetMinKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t CryptoServiceProxyImpl::GetMaxKeyBitLength() const noexcept
{
  // To Get actual bit-length of a key loaded to the context.
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  std::size_t result{0};
  auto retValue{cryptoServiceHandler_->GetMaxKeyBitLength().GetResult()};
  // To check the return value.
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::CryptoService GetMaxKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t CryptoServiceProxyImpl::GetActualKeyBitLength() const noexcept
{
  // Get actual bit-length of a key loaded to the context.
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  std::size_t result{0};
  auto retValue{cryptoServiceHandler_->GetActualKeyBitLength().GetResult()};
  // To check the return value.
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::CryptoService GetActualKeyBitLength() result = " << result;    
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

AllowedUsageFlags CryptoServiceProxyImpl::GetAllowedUsage() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  AllowedUsageFlags result{0};
  auto retValue{cryptoServiceHandler_->GetAllowedUsage().GetResult()};
  // To check the return value.
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::CryptoService GetAllowedUsage() result = " << result;    
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

CryptoObjectUid CryptoServiceProxyImpl::GetActualKeyCOUID() const noexcept
{
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  CryptoObjectUid result{0};
  auto retValue{cryptoServiceHandler_->GetActualKeyCOUID().GetResult()};
  if (retValue.HasValue())
  {
    // To fill the return type object parameters.
    logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Success";
    result.mGeneratorUid.mQwordMs = retValue.Value().response.mGeneratorUid.mQwordMs;
    logger_.LogDebug() << "IPC-APP::CryptoService GetActualKeyCOUID() mQwordMs = "
        << result.mGeneratorUid.mQwordMs;
    result.mGeneratorUid.mQwordLs = retValue.Value().response.mGeneratorUid.mQwordLs;
    logger_.LogDebug() << "IPC-APP::CryptoService GetActualKeyCOUID() mQwordLs = "
        << result.mGeneratorUid.mQwordLs;
    result.mVersionStamp = retValue.Value().response.mVersionStamp;
    logger_.LogDebug() << "IPC-APP::CryptoService GetActualKeyCOUID() mVersionStamp = "
        << result.mVersionStamp;    
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool CryptoServiceProxyImpl::IsKeyAvailable() const noexcept
{
  // To Check if a key has been set to this context.
  logger_.LogDebug() << "IPC-APP::CryptoService  -> " << __FUNCTION__;
  bool result{0};
  auto const retValue{cryptoServiceHandler_->IsKeyAvailable().GetResult()};
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-APP::CryptoService IsKeyAvailable() result = " << result;    
  }
  else
  {
    logger_.LogError() << "IPC-APP::CryptoService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
