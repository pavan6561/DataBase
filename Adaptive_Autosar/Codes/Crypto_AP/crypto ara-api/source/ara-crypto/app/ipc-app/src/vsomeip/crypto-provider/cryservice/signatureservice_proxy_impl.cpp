// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signatureservice_proxy_impl.cpp
//
// Purpose     : Crypto object proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "signatureservice_proxy_impl.h"

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
SignatureServiceProxyImpl::SignatureServiceProxyImpl(
    const CryptoServiceInstanceId serviceInstanceID) noexcept :
    cryptoObjServiceInstanceID_{serviceInstanceID},
    logger_{ara::log::CreateLogger("COPI", "Crypto Object Proxy Impl", ara::log::LogLevel::kVerbose)}
{
  Initialize(serviceInstanceID);
}

void SignatureServiceProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  // To get the proxy handler.
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for SignatureServiceProxyImpl : Successful";
    signatureServiceHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for SignatureServiceProxyImpl : Failed";
  }
}

std::size_t SignatureServiceProxyImpl::GetRequiredHashSize() const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward call to the GetRequiredHashSize method.
  auto response = signatureServiceHandler_->GetRequiredHashSize().GetResult();
  if (response.HasValue())
  {
    result = response.Value().result;
    logger_.LogDebug() << "IPC-App::SignatureService GetRequiredHashSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

CryptoPrimitiveId::AlgId SignatureServiceProxyImpl::GetRequiredHashAlgId() const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  CryptoPrimitiveId::AlgId result{0};
  // Forward call to the GetRequiredHashAlgId method.
  auto response = signatureServiceHandler_->GetRequiredHashAlgId().GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-App::SignatureService GetRequiredHashAlgId() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t SignatureServiceProxyImpl::GetSignatureSize() const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward call to the GetSignatureSize method.
  auto response = signatureServiceHandler_->GetSignatureSize().GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-App::SignatureService GetSignatureSize() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool SignatureServiceProxyImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward call to the IsKeyBitLengthSupported method.
  auto response = signatureServiceHandler_->IsKeyBitLengthSupported(keyBitLength).GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-App::SignatureService IsKeyBitLengthSupported() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t SignatureServiceProxyImpl::GetMinKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward call to the GetMinKeyBitLength method.
  auto response = signatureServiceHandler_->GetMinKeyBitLength().GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-App::SignatureService GetMinKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t SignatureServiceProxyImpl::GetMaxKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward call to the GetMaxKeyBitLength method.
  auto response = signatureServiceHandler_->GetMaxKeyBitLength().GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-App::SignatureService GetMaxKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

std::size_t SignatureServiceProxyImpl::GetActualKeyBitLength() const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  std::size_t result{0};
  // Forward call to the GetActualKeyBitLength method.
  auto response = signatureServiceHandler_->GetActualKeyBitLength().GetResult();
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-App::SignatureService GetActualKeyBitLength() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

AllowedUsageFlags SignatureServiceProxyImpl::GetAllowedUsage() const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  AllowedUsageFlags result{0};
  // Forward call to the GetAllowedUsage method.
  auto retValue = signatureServiceHandler_->GetAllowedUsage().GetResult();
  if (retValue.HasValue())
  {
    result = retValue.Value().response;
    logger_.LogDebug() << "IPC-App::SignatureService GetAllowedUsage() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

CryptoObjectUid SignatureServiceProxyImpl::GetActualKeyCOUID() const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  CryptoObjectUid result{0};
  // Forward call to the GetActualKeyCOUID method.
  auto retValue{signatureServiceHandler_->GetActualKeyCOUID().GetResult()};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Success";
    result.mGeneratorUid.mQwordMs = retValue.Value().response.mGeneratorUid.mQwordMs;
    logger_.LogDebug() << "IPC-App::SignatureService GetActualKeyCOUID() mQwordMs = "
        << result.mGeneratorUid.mQwordMs;
    result.mGeneratorUid.mQwordLs = retValue.Value().response.mGeneratorUid.mQwordLs;
    logger_.LogDebug() << "IPC-App::SignatureService GetActualKeyCOUID() mQwordLs = "
        << result.mGeneratorUid.mQwordLs;
    result.mVersionStamp = retValue.Value().response.mVersionStamp;
    logger_.LogDebug() << "IPC-App::SignatureService GetActualKeyCOUID() mVersionStamp = "
        << result.mVersionStamp;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

bool SignatureServiceProxyImpl::IsKeyAvailable() const noexcept
{
  logger_.LogDebug() << "IPC-App::SignatureService  -> " << __FUNCTION__;
  bool result{0};
  // Forward call to the IsKeyAvailable method.
  auto response {signatureServiceHandler_->IsKeyAvailable().GetResult()};
  if (response.HasValue())
  {
    result = response.Value().response;
    logger_.LogDebug() << "IPC-App::SignatureService IsKeyAvailable() result = " << result;
  }
  else
  {
    logger_.LogError() << "IPC-App::SignatureService  -> " << __FUNCTION__ << " : Failed";
  }
  return result;
}

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara
