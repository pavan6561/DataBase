// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : adapter_randomgenerator.h
//
// Purpose     : Implementation of file adapter_randomgenerator.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-03-14T12:58:58+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOSERVEREXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_RANDOMGENERATOR_ADAPTER_RANDOMGENERATOR_H_
#define CRYPTOSERVEREXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_RANDOMGENERATOR_ADAPTER_RANDOMGENERATOR_H_

#include <ara/com/internal/vsomeip/skeleton/vsomeip_skeleton_base.h>
#include "service_desc_randomgenerator.h"

#include "ara/crypto/ipccom/cryp/randomgenerator/randomgenerator_skeleton.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace randomgenerator
{
namespace randomgenerator_binding
{
namespace vsomeip
{

/// @brief Class for RandomGeneratorServiceAdapter.
class RandomGeneratorServiceAdapter : public
  ::ara::com::internal::vsomeip::skeleton::ServiceImplBase {
 public:
  /// @brief Using for ServiceInterface.
  using ServiceInterface = skeleton::RandomGeneratorSkeleton;
  /// @brief Using for ServiceDescriptor.
  using ServiceDescriptor = descriptors::internal::Service;

  /// @brief description on RandomGeneratorServiceAdapter.
  RandomGeneratorServiceAdapter(ServiceInterface& service, ara::com::internal::vsomeip::types::InstanceId instance) :
    ::ara::com::internal::vsomeip::skeleton::ServiceImplBase(service, instance){
    Connect(service);
    RegisterMethodDispatcher(ServiceDescriptor::service_id,
      [this](const std::shared_ptr<::vsomeip::message>& msg) { DispatchMethodCall(msg); });
    OfferService(ServiceDescriptor::service_id, GetInstanceId(), ServiceDescriptor::service_version_major, ServiceDescriptor::service_version_minor);
    SendInitialNotification(service);
  }

  /// @brief RandomGeneratorServiceAdapter Destructor.
  virtual ~RandomGeneratorServiceAdapter() noexcept override;

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
} // namespace randomgenerator_binding
} // namespace randomgenerator
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOSERVEREXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_RANDOMGENERATOR_ADAPTER_RANDOMGENERATOR_H_


