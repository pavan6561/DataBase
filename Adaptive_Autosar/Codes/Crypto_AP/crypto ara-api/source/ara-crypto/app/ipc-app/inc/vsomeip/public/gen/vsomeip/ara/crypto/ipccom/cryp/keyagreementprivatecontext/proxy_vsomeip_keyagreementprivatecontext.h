// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : proxy_vsomeip_keyagreementprivatecontext.h
//
// Purpose     : Implementation of file proxy_vsomeip_keyagreementprivatecontext.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOCLIENTEXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_KEYAGREEMENTPRIVATECONTEXT_PROXY_VSOMEIP_KEYAGREEMENTPRIVATECONTEXT_H_
#define CRYPTOCLIENTEXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_KEYAGREEMENTPRIVATECONTEXT_PROXY_VSOMEIP_KEYAGREEMENTPRIVATECONTEXT_H_

#include "service_desc_keyagreementprivatecontext.h"
#include "ara/crypto/ipccom/cryp/keyagreementprivatecontext/keyagreementprivatecontext_proxy.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace keyagreementprivatecontext
{
namespace keyagreementprivatecontext_binding
{
namespace vsomeip
{

/// @brief Class for KeyAgreementPrivateContextImpl.
class KeyAgreementPrivateContextImpl : public proxy::KeyAgreementPrivateContextProxyBase, public ::ara::com::internal::vsomeip::proxy::ProxyImplBase {
 public :
  /// @brief Using for ProxyInterface.
  using ProxyInterface = proxy::KeyAgreementPrivateContextProxy;
  /// @brief Using for ServiceDescriptor.
  using ServiceDescriptor = descriptors::internal::Service;

  /// @brief Definition for KeyAgreementPrivateContextImpl.
  explicit KeyAgreementPrivateContextImpl(ara::com::internal::vsomeip::types::InstanceId instance) :
  ::ara::com::internal::vsomeip::proxy::ProxyImplBase(ServiceDescriptor::service_id, instance, ServiceDescriptor::service_version_major, ServiceDescriptor::service_version_minor),
  /// @brief Definition for AgreeKey(instance).
    AgreeKey(instance),
  /// @brief Definition for AgreeSeed(instance).
    AgreeSeed(instance),
  /// @brief Definition for GetCryptoPrimitiveId(instance).
    GetCryptoPrimitiveId(instance),
  /// @brief Definition for GetExtensionService(instance).
    GetExtensionService(instance),
  /// @brief Definition for IsInitialized(instance).
    IsInitialized(instance),
  /// @brief Definition for Reset(instance).
    Reset(instance),
  /// @brief Definition for SetKey(instance).
    SetKey(instance)
  {}
  /// @brief Definition for KeyAgreementPrivateContextImpl.
  ~KeyAgreementPrivateContextImpl() noexcept override {}

  /// @brief Definition for GetAgreeKey.
        ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::AgreeKey& GetAgreeKey() noexcept override { return AgreeKey; }
  /// @brief Definition for GetAgreeSeed.
        ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::AgreeSeed& GetAgreeSeed() noexcept override { return AgreeSeed; }
  /// @brief Definition for GetGetCryptoPrimitiveId.
        ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::GetCryptoPrimitiveId& GetGetCryptoPrimitiveId() noexcept override { return GetCryptoPrimitiveId; }
  /// @brief Definition for GetGetExtensionService.
        ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::GetExtensionService& GetGetExtensionService() noexcept override { return GetExtensionService; }
  /// @brief Definition for GetIsInitialized.
        ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::IsInitialized& GetIsInitialized() noexcept override { return IsInitialized; }
  /// @brief Definition for GetReset.
        ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::Reset& GetReset() noexcept override { return Reset; }
  /// @brief Definition for GetSetKey.
        ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::SetKey& GetSetKey() noexcept override { return SetKey; }

 private:
  /// @brief Method for AgreeKey is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::AgreeKey, ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::AgreeKey::signature_type> AgreeKey;
  /// @brief Method for AgreeSeed is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::AgreeSeed, ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::AgreeSeed::signature_type> AgreeSeed;
  /// @brief Method for GetCryptoPrimitiveId is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::GetCryptoPrimitiveId, ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::GetCryptoPrimitiveId::signature_type> GetCryptoPrimitiveId;
  /// @brief Method for GetExtensionService is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::GetExtensionService, ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::GetExtensionService::signature_type> GetExtensionService;
  /// @brief Method for IsInitialized is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::IsInitialized, ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::IsInitialized::signature_type> IsInitialized;
  /// @brief Method for Reset is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::Reset, ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::Reset::signature_type> Reset;
  /// @brief Method for SetKey is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::SetKey, ara::crypto::ipccom::cryp::keyagreementprivatecontext::proxy::methods::SetKey::signature_type> SetKey;
};

} // namespace vsomeip
} // namespace keyagreementprivatecontext_binding
} // namespace keyagreementprivatecontext
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOCLIENTEXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_KEYAGREEMENTPRIVATECONTEXT_PROXY_VSOMEIP_KEYAGREEMENTPRIVATECONTEXT_H_

