// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : adapter_symmetrickey.h
//
// Purpose     : Implementation of file adapter_symmetrickey.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOSERVEREXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_CRYOBJ_SYMMETRICKEY_ADAPTER_SYMMETRICKEY_H_
#define CRYPTOSERVEREXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_CRYOBJ_SYMMETRICKEY_ADAPTER_SYMMETRICKEY_H_

#include <ara/com/internal/vsomeip/skeleton/vsomeip_skeleton_base.h>
#include "service_desc_symmetrickey.h"

#include "ara/crypto/ipccom/cryp/cryobj/symmetrickey/symmetrickey_skeleton.h"

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
namespace symmetrickey
{
namespace symmetrickey_binding
{
namespace vsomeip
{

/// @brief Class for SymmetricKeyServiceAdapter.
class SymmetricKeyServiceAdapter : public
  ::ara::com::internal::vsomeip::skeleton::ServiceImplBase {
 public:
  /// @brief Using for ServiceInterface.
  using ServiceInterface = skeleton::SymmetricKeySkeleton;
  /// @brief Using for ServiceDescriptor.
  using ServiceDescriptor = descriptors::internal::Service;

  /// @brief description on SymmetricKeyServiceAdapter.
  SymmetricKeyServiceAdapter(ServiceInterface& service, ara::com::internal::vsomeip::types::InstanceId instance) :
    ::ara::com::internal::vsomeip::skeleton::ServiceImplBase(service, instance){
    Connect(service);
    RegisterMethodDispatcher(ServiceDescriptor::service_id,
      [this](const std::shared_ptr<::vsomeip::message>& msg) { DispatchMethodCall(msg); });
    OfferService(ServiceDescriptor::service_id, GetInstanceId(), ServiceDescriptor::service_version_major, ServiceDescriptor::service_version_minor);
    SendInitialNotification(service);
  }

  /// @brief SymmetricKeyServiceAdapter Destructor.
  virtual ~SymmetricKeyServiceAdapter() noexcept override;

  /// @brief Definition for Connect.
  void Connect(ServiceInterface& service);
  /// @brief Definition for Disconnect.
  void Disconnect(ServiceInterface& service);
  
  /// @brief Definition for SendInitialNotification.
  void SendInitialNotification(ServiceInterface& service);

 protected:
  /// @brief Definition for DispatchMethodCall.
  void DispatchMethodCall(const std::shared_ptr<::vsomeip::message>& msg);

 private:
};

} // namespace vsomeip
} // namespace symmetrickey_binding
} // namespace symmetrickey
} // namespace cryobj
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOSERVEREXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_CRYOBJ_SYMMETRICKEY_ADAPTER_SYMMETRICKEY_H_


