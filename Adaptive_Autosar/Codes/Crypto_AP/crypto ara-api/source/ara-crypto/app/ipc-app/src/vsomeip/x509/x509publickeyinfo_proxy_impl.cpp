// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509publickeyinfo_proxy_impl.cpp
//
// Purpose     : X509 provider proxy implementation to forward functional calls from application to
//               ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "x509publickeyinfo_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
X509PublicKeyInfoProxyImpl::X509PublicKeyInfoProxyImpl(
    const CryptoServiceInstanceId serviceInstanceID) noexcept :
    logger_{ara::log::CreateLogger("CPPI", "X509 Provider Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "X509PublicKeyInfoProxyImpl Constructor";
  logger_.LogDebug() << "Initialize server client connection for X509PublicKeyInfo";
  Initialize(serviceInstanceID);
}
void X509PublicKeyInfoProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  logger_.LogDebug() << "IPC-APP X509PublicKeyInfoProxyImpl::" << __FUNCTION__;
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for X509PublicKeyInfo : Successful";
    x509PublicKeyInfoHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for X509PublicKeyInfo : Failed";
  }
}

ara::crypto::cryp::CryptoPrimitiveId::Uptrc X509PublicKeyInfoProxyImpl::GetAlgorithmId()
{
  auto retOutput{x509PublicKeyInfoHandler_->GetAlgorithmId().GetResult()};
  ara::crypto::cryp::CryptoPrimitiveId::Uptrc cryptoPrimitiveIdUptr{nullptr};
  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::X509PublicKeyInfoProxyImpl-> GetAlgorithmId() success";
    const ara::core::StringView primitiveName{retOutput.Value().cryptoPrimitiveInstanceId.primitiveName};
    const ara::crypto::cryp::CryptoPrimitiveId::AlgId algorithmId{
        retOutput.Value().cryptoPrimitiveInstanceId.primitiveAlgId};
    cryptoPrimitiveIdUptr
        = std::make_unique<ara::crypto::cryp::CryptoPrimitiveIdProxyImpl>(primitiveName, algorithmId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509PublicKeyInfoProxyImpl::GetAlgorithmId() Error: ";
  }
  return cryptoPrimitiveIdUptr;
}

std::size_t X509PublicKeyInfoProxyImpl::GetRequiredHashSize() const noexcept
{
  auto retOutput{x509PublicKeyInfoHandler_->GetRequiredHashSize().GetResult()};
  std::size_t ret{0};

  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::X509PublicKeyInfoProxyImpl-> GetRequiredHashSize() success";
    ret = retOutput.Value().size_t;
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509PublicKeyInfoProxyImpl::GetRequiredHashSize() Error ";
  }
  return ret;
}

CryptoAlgId X509PublicKeyInfoProxyImpl::GetRequiredHashAlgId() const noexcept
{
  CryptoAlgId cryptoAlgId{0};
  auto retOutput{x509PublicKeyInfoHandler_->GetRequiredHashAlgId().GetResult()};

  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::X509PublicKeyInfoProxyImpl-> GetRequiredHashAlgId() success";
    cryptoAlgId = retOutput.Value().cryptoAlgId;
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509PublicKeyInfoProxyImpl::GetRequiredHashAlgId() Error ";
  }
  return cryptoAlgId;
}

std::size_t X509PublicKeyInfoProxyImpl::GetSignatureSize() const noexcept
{
  std::size_t ret{0};
  auto retOutput{x509PublicKeyInfoHandler_->GetSignatureSize().GetResult()};

  if (retOutput.HasValue())
  {
    logger_.LogError() << "IPC-APP::X509PublicKeyInfoProxyImpl::GetSignatureSize() Success ";
    ret = retOutput.Value().size_t;
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509PublicKeyInfoProxyImpl::GetSignatureSize() Error ";
  }
  return ret;
}

ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc> X509PublicKeyInfoProxyImpl::GetPublicKey() const
    noexcept
{
  auto retOutput{x509PublicKeyInfoHandler_->GetPublicKey().GetResult()};
  ara::crypto::cryp::PublicKey::Uptrc publicKeyUptr{nullptr};
  if (retOutput.HasValue())
  {
    CryptoServiceInstanceId instanceId{retOutput.Value().publicKeyInstanceId};
    logger_.LogDebug() << "IPC-APP::X509PublicKeyInfoProxyImpl GetPublicKey Success : " << instanceId;
    publicKeyUptr = std::make_unique<ara::crypto::cryp::cryobj::PublicKeyProxyImpl>(instanceId);
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509PublicKeyInfoProxyImpl::GetPublicKey() Error ";
  }

  return ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc>(std::move(publicKeyUptr));
}

bool X509PublicKeyInfoProxyImpl::IsSameKey(const ara::crypto::cryp::PublicKey& publicKey) const noexcept
{
  bool ret{false};

  const ara::crypto::cryp::cryobj::PublicKeyProxyImpl* obj{
      dynamic_cast<const ara::crypto::cryp::cryobj::PublicKeyProxyImpl*>(&publicKey)};

  CryptoServiceInstanceId const serviceInstanceId{obj->GetPublicKeyServiceInstanceId()};

  auto retOutput{x509PublicKeyInfoHandler_->IsSameKey(serviceInstanceId).GetResult()};

  if (retOutput.HasValue())
  {
    logger_.LogError() << "IPC-APP::X509PublicKeyInfoProxyImpl::IsSameKey() Success ";
    ret = retOutput.Value().result;
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509PublicKeyInfoProxyImpl::IsSameKey() Error ";
  }
  return ret;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> X509PublicKeyInfoProxyImpl::ExportPublicly(
    FormatId formatId) const noexcept
{
  logger_.LogDebug() << "IPC-APP::X509PublicKeyInfoProxyImpl -> ExportPublicly()";
  auto objResult{x509PublicKeyInfoHandler_->ExportPublicly(formatId).GetResult()};
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::X509PublicKeyInfoProxyImpl -> " << __FUNCTION__ << " : Success";
    auto byteResult{objResult.Value().resultByteVector};
    ara::core::Vector<ara::core::Byte> localByteVector(byteResult.begin(), byteResult.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(localByteVector);
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509PublicKeyInfoProxyImpl -> " << __FUNCTION__ << " : Failed";
    logger_.LogError() << "IPC-APP::ExportPublicly() Error: " << objResult.Error().Message();
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        static_cast<SecurityErrorDomain::Errc>(objResult.Error().Value()));
  }
}

} // namespace x509
} // namespace crypto
} // namespace ara
