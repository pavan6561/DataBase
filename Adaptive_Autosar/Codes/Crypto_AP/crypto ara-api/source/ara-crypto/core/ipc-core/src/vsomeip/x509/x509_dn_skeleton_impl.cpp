// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_dn_skeleton_impl.cpp
//
// Purpose     : X509 Domain Number skeleton implementation
// -----------------------------------------------------------------------------------------------------------

#include "x509_dn_skeleton_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
X509DnSkeletonImpl::X509DnSkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::x509::X509DN::Uptr uptr) noexcept :
    ara::crypto::ipccom::x509::x509dn::skeleton::X509DNSkeleton(ara::com::InstanceIdentifier(instanceId)),
    x509DnUptr_{std::move(uptr)}
{
  logger_.LogDebug() << "X509DnSkeletonImpl Constructor with X509DN::Uptr object";
}

ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::ExportPubliclyOutput>
    X509DnSkeletonImpl::ExportPublicly(const uint32& formatId)
{
  logger_.LogDebug() << "IPC-CORE::X509DN ExportPublicly()";
  ara::core::Future<ExportPubliclyOutput>::PromiseType promise;
  ara::core::Result<ara::core::Vector<ara::core::Byte>> const retValue{x509DnUptr_->ExportPublicly(formatId)};
  if (retValue.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::X509DN ExportPublicly() -> success";
    ara::core::Vector<ara::core::Byte> tempVtr{retValue.Value()};
    ExportPubliclyOutput exportOutput;
    std::for_each(tempVtr.begin(), tempVtr.end(), [&exportOutput](ara::core::Byte i) {
      exportOutput.result.emplace_back(static_cast<uint8_t>(i));
    });
    promise.set_value(exportOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::X509DN ExportPublicly() Error ->" << retValue.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(retValue.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::GetAttributeOutput>
    X509DnSkeletonImpl::GetAttribute(const uint64& id)
{
  ara::core::Future<GetAttributeOutput>::PromiseType promise;
  ara::core::Result<ara::core::StringView> const dnStr{
      x509DnUptr_->GetAttribute(static_cast<ara::crypto::x509::X509DN::AttributeId>(id))};
  if (dnStr.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::X509DN GetAttribute() -> success";
    GetAttributeOutput dnValue;
    dnValue.attributeResult = dnStr.Value();
    promise.set_value(dnValue);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::X509DN GetAttribute() Error ->" << dnStr.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(dnStr.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::GetAttributeWithIndexOutput>
    X509DnSkeletonImpl::GetAttributeWithIndex(const uint32& id, const uint64& index)
{
  ara::core::Future<GetAttributeWithIndexOutput>::PromiseType promise;

  ara::core::Result<ara::core::StringView> const dnStr{
      x509DnUptr_->GetAttribute(static_cast<ara::crypto::x509::X509DN::AttributeId>(id), index)};
  if (dnStr.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::X509DN GetAttribute() -> success";
    GetAttributeWithIndexOutput dnValue;
    dnValue.attributeResult = dnStr.Value();
    promise.set_value(dnValue);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::X509DN GetAttribute() Error ->" << dnStr.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(dnStr.Error().Value()));
  }

  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::GetDnStringOutput>
    X509DnSkeletonImpl::GetDnString()
{
  logger_.LogDebug() << "IPC-CORE::X509DN GetDnString()";
  ara::core::Future<GetDnStringOutput>::PromiseType promise;
  ara::core::Result<ara::core::StringView> const dnStr{x509DnUptr_->GetDnString()};
  if (dnStr.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::X509DN GetDnString() -> success";
    GetDnStringOutput dnValue;
    dnValue.response = dnStr.Value();
    promise.set_value(dnValue);
  }
  else
  {
    logger_.LogError() << "IPC-CORE::X509DN GetDnString() Error ->" << dnStr.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(dnStr.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::x509::x509dn::X509DN::MyProviderOutput>
    X509DnSkeletonImpl::MyProvider()
{
  ara::core::Future<MyProviderOutput>::PromiseType promise;
  return promise.get_future();
}

ara::core::Future<void> X509DnSkeletonImpl::SetAttribute(const uint32& attributeId, const String& attribute)
{
  ara::core::Future<void>::PromiseType promise;
  ara::core::Result<void> const dnStr{
      x509DnUptr_->SetAttribute(static_cast<ara::crypto::x509::X509DN::AttributeId>(attributeId), attribute)};
  if (dnStr.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::X509DN SetAttribute() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::X509DN SetAttribute() Error ->" << dnStr.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(dnStr.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<void> X509DnSkeletonImpl::SetAttributeWithIndex(
    const uint32& attributeId, const uint64& index, const String& attribute)
{
  ara::core::Future<void>::PromiseType promise;
  ara::core::Result<void> const dnStr{x509DnUptr_->SetAttribute(
      static_cast<ara::crypto::x509::X509DN::AttributeId>(attributeId), index, attribute)};
  if (dnStr.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::X509DN SetAttributeWithIndex() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::X509DN SetAttributeWithIndex() Error ->" << dnStr.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(dnStr.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<void> X509DnSkeletonImpl::SetDn(const String& dn)
{
  ara::core::Future<void>::PromiseType promise;
  ara::core::Result<void> const setDnResult{x509DnUptr_->SetDn(dn.c_str())};
  if (setDnResult.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE::X509DN SetDn() -> success";
    promise.set_value();
  }
  else
  {
    logger_.LogError() << "IPC-CORE::X509DN SetDn() Error ->" << setDnResult.Error().Message();
    promise.SetError(static_cast<ipccom::common::error::CryptoErrorDomainErrc>(setDnResult.Error().Value()));
  }
  return promise.get_future();
}

} // namespace x509
} // namespace crypto
} // namespace ara
