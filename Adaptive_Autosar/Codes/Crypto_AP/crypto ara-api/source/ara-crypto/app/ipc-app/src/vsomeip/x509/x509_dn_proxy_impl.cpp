// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_dn_proxy_impl.cpp
//
// Purpose     : X509 domain number proxy implementation to forward functional calls from application to
//               ipc-core of crypto service daemon.
// -----------------------------------------------------------------------------------------------------------

#include "x509_dn_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
X509DNProxyImpl::X509DNProxyImpl(
    CryptoServiceInstanceId const instanceValue, X509Provider& x509ProRef) noexcept :
    x509ProviderRef_{x509ProRef},
    logger_{ara::log::CreateLogger("X9DN", "X509 DN Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "X509DNProxyImpl constructor";
  if (Initialize(instanceValue) == true)
  {
    logger_.LogDebug() << "server client connection for X509 DN -> success";
  }
  else
  {
    logger_.LogError() << "server client connection for X509 DN -> fail";
  }
}

bool X509DNProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess{false};
  if (vsomeipX509DnInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for X509 DN Function: Successful";
    x509DnProxyHandler_ = vsomeipX509DnInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server Client connection for X509 DN Function: Failed";
    isSuccess = false;
  }
  return isSuccess;
}

CryptoServiceInstanceId X509DNProxyImpl::GetX509DnServiceInstanceId() const
{
  return vsomeipX509DnInstance_.serviceInstanceID_;
}

ara::core::Result<ara::core::StringView> X509DNProxyImpl::GetDnString() const noexcept
{
  logger_.LogDebug() << "IPC-APP X509DNProxyImpl::" << __FUNCTION__;

  auto const retOutput{x509DnProxyHandler_->GetDnString().GetResult()};

  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP GetDnString() Data -> " << retOutput.Value().response;
    getDnString_ = retOutput.Value().response;
    return ara::core::Result<ara::core::StringView>::FromValue(getDnString_);
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509DNProxyImpl::GetDnString() Error: " << retOutput.Error().Message();
    return ara::core::Result<ara::core::StringView>::FromError(
        static_cast<SecurityErrorDomain::Errc>(retOutput.Error().Value()));
  }
}

ara::core::Result<void> X509DNProxyImpl::SetDn(ara::core::StringView dn) noexcept
{
  logger_.LogDebug() << "IPC-APP X509DNProxyImpl::" << __FUNCTION__;
  auto const retOutput{x509DnProxyHandler_->SetDn(dn.data()).GetResult()};
  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP X509DNProxyImpl:: SetDn() -> success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509DNProxyImpl::SetDn() Error: " << retOutput.Error().Message();
    return ara::core::Result<void>::FromError(
        static_cast<SecurityErrorDomain::Errc>(retOutput.Error().Value()));
  }
}

ara::core::Result<ara::core::StringView> X509DNProxyImpl::GetAttribute(AttributeId id) const noexcept
{
  logger_.LogDebug() << "IPC-APP X509DNProxyImpl::" << __FUNCTION__;
  auto const retOutput{x509DnProxyHandler_->GetAttribute(static_cast<uint32_t>(id)).GetResult()};
  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP X509DNProxyImpl:: GetAttribute() data : "
                       << retOutput.Value().attributeResult;
    getAttribute_ = retOutput.Value().attributeResult;
    return ara::core::Result<ara::core::StringView>::FromValue(getAttribute_);
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509DNProxyImpl::GetAttribute() Error: " << retOutput.Error().Message();
    return ara::core::Result<ara::core::StringView>::FromError(
        static_cast<SecurityErrorDomain::Errc>(retOutput.Error().Value()));
  }
}

ara::core::Result<void> X509DNProxyImpl::SetAttribute(AttributeId id, ara::core::StringView attribute) const
    noexcept
{
  logger_.LogDebug() << "IPC-APP X509DNProxyImpl::" << __FUNCTION__;
  auto const retOutput{
      x509DnProxyHandler_->SetAttribute(static_cast<uint32_t>(id), attribute.data()).GetResult()};
  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP X509DNProxyImpl:: SetAttribute() -> success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509DNProxyImpl::SetAttribute() Error: " << retOutput.Error().Message();
    return ara::core::Result<void>::FromError(
        static_cast<SecurityErrorDomain::Errc>(retOutput.Error().Value()));
  }
}

ara::core::Result<ara::core::StringView> X509DNProxyImpl::GetAttribute(AttributeId id, unsigned index) const
    noexcept
{
  logger_.LogDebug() << "IPC-APP X509DNProxyImpl::" << __FUNCTION__;
  auto const retOutput{
      x509DnProxyHandler_->GetAttributeWithIndex(static_cast<uint32_t>(id), index).GetResult()};

  if (retOutput.HasValue())
  {
    getAttributeIndex_ = retOutput.Value().attributeResult;
    logger_.LogDebug() << "IPC-APP X509DNProxyImpl:: GetAttribute() -> data = " << getAttributeIndex_;
    return ara::core::Result<ara::core::StringView>::FromValue(getAttributeIndex_);
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509DNProxyImpl::GetAttribute() Error: " << retOutput.Error().Message();
    return ara::core::Result<ara::core::StringView>::FromError(
        static_cast<SecurityErrorDomain::Errc>(retOutput.Error().Value()));
  }
}

ara::core::Result<void> X509DNProxyImpl::SetAttribute(
    AttributeId id, unsigned index, ara::core::StringView attribute) const noexcept
{
  logger_.LogDebug() << "IPC-APP X509DNProxyImpl::" << __FUNCTION__;
  auto const retOutput{
      x509DnProxyHandler_->SetAttributeWithIndex(static_cast<uint32_t>(id), index, attribute.data())
          .GetResult()};

  if (retOutput.HasValue())
  {
    logger_.LogDebug() << "IPC-APP X509DNProxyImpl:: SetAttribute() -> success";
    return ara::core::Result<void>::FromValue();
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509DNProxyImpl::SetAttribute() Error: " << retOutput.Error().Message();
    return ara::core::Result<void>::FromError(
        static_cast<SecurityErrorDomain::Errc>(retOutput.Error().Value()));
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> X509DNProxyImpl::ExportPublicly(FormatId formatId) const
    noexcept
{
  auto const objResult{x509DnProxyHandler_->ExportPublicly(formatId).GetResult()};
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::X509DNProxyImpl -> " << __FUNCTION__ << " : Success";
    auto const byteResult{objResult.Value().result};
    ara::core::Vector<ara::core::Byte> localByteVector(byteResult.begin(), byteResult.end());
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(localByteVector);
  }
  else
  {
    logger_.LogError() << "IPC-APP::X509DNProxyImpl -> " << __FUNCTION__ << " : Failed";
    logger_.LogError() << "IPC-APP::ExportPublicly() -> Error: " << objResult.Error().Message();
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        static_cast<SecurityErrorDomain::Errc>(objResult.Error().Value()));
  }
}

X509Provider& X509DNProxyImpl::MyProvider() const noexcept
{
  logger_.LogDebug() << "IPC-APP::X509DNProxyImpl -> MyProvider()";
  return x509ProviderRef_;
}

} // namespace x509
} // namespace crypto
} // namespace ara
