// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : adapter_publickey.cpp
//
// Purpose     : Implementation of file adapter_publickey.cpp
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#include "adapter_publickey.h"
namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace cryobj
{
namespace pubilckey
{
namespace publickey_binding
{
namespace vsomeip
{

PublicKeyServiceAdapter::~PublicKeyServiceAdapter() noexcept {
  StopOfferService(ServiceDescriptor::service_id, GetInstanceId());
  UnregisterMethodDispatcher(ServiceDescriptor::service_id);
  Disconnect(dynamic_cast<ServiceInterface&>(service_));
}

void PublicKeyServiceAdapter::DispatchMethodCall(const std::shared_ptr<::vsomeip::message>& msg)
{
  ServiceInterface& service{dynamic_cast<ServiceInterface&>(service_)};
  switch(msg->get_method()) {
    case descriptors::CheckKey::method_id:
      HandleCall(service, &ServiceInterface::CheckKey, msg);
    break;
    case descriptors::ExportPublicly::method_id:
      HandleCall(service, &ServiceInterface::ExportPublicly, msg);
    break;
    case descriptors::GetAllowedUsage::method_id:
      HandleCall(service, &ServiceInterface::GetAllowedUsage, msg);
    break;
    case descriptors::GetCryptoPrimitiveId::method_id:
      HandleCall(service, &ServiceInterface::GetCryptoPrimitiveId, msg);
    break;
    case descriptors::GetObjectId::method_id:
      HandleCall(service, &ServiceInterface::GetObjectId, msg);
    break;
    case descriptors::GetPayloadSize::method_id:
      HandleCall(service, &ServiceInterface::GetPayloadSize, msg);
    break;
    case descriptors::HasDependence::method_id:
      HandleCall(service, &ServiceInterface::HasDependence, msg);
    break;
    case descriptors::HashPublicKey::method_id:
      HandleCall(service, &ServiceInterface::HashPublicKey, msg);
    break;
    case descriptors::IsExportable::method_id:
      HandleCall(service, &ServiceInterface::IsExportable, msg);
    break;
    case descriptors::IsSession::method_id:
      HandleCall(service, &ServiceInterface::IsSession, msg);
    break;
    case descriptors::Save::method_id:
      HandleCall(service, &ServiceInterface::Save, msg);
    break;
    default:
    // Avoid Warning.
    break;
  }
}

void PublicKeyServiceAdapter::Connect(ServiceInterface& service) {
  service.AddDelegate(*this);
}

void PublicKeyServiceAdapter::Disconnect(ServiceInterface& service) {
  service.RemoveDelegate(*this);
}

void PublicKeyServiceAdapter::SendInitialNotification(ServiceInterface& service) {
}

} // namespace vsomeip
} // namespace publickey_binding
} // namespace pubilckey
} // namespace cryobj
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara


