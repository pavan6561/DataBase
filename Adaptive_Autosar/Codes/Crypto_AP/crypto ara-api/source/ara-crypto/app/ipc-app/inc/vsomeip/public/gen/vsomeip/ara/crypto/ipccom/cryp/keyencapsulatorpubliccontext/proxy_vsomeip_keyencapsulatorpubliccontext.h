// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : proxy_vsomeip_keyencapsulatorpubliccontext.h
//
// Purpose     : Implementation of file proxy_vsomeip_keyencapsulatorpubliccontext.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOCLIENTEXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_KEYENCAPSULATORPUBLICCONTEXT_PROXY_VSOMEIP_KEYENCAPSULATORPUBLICCONTEXT_H_
#define CRYPTOCLIENTEXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_KEYENCAPSULATORPUBLICCONTEXT_PROXY_VSOMEIP_KEYENCAPSULATORPUBLICCONTEXT_H_

#include "service_desc_keyencapsulatorpubliccontext.h"
#include "ara/crypto/ipccom/cryp/keyencapsulatorpubliccontext/keyencapsulatorpubliccontext_proxy.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace keyencapsulatorpubliccontext
{
namespace keyencapsulatorpubliccontext_binding
{
namespace vsomeip
{

/// @brief Class for KeyEncapsulatorPublicContextImpl.
class KeyEncapsulatorPublicContextImpl : public proxy::KeyEncapsulatorPublicContextProxyBase, public ::ara::com::internal::vsomeip::proxy::ProxyImplBase {
 public :
  /// @brief Using for ProxyInterface.
  using ProxyInterface = proxy::KeyEncapsulatorPublicContextProxy;
  /// @brief Using for ServiceDescriptor.
  using ServiceDescriptor = descriptors::internal::Service;

  /// @brief Definition for KeyEncapsulatorPublicContextImpl.
  explicit KeyEncapsulatorPublicContextImpl(ara::com::internal::vsomeip::types::InstanceId instance) :
  ::ara::com::internal::vsomeip::proxy::ProxyImplBase(ServiceDescriptor::service_id, instance, ServiceDescriptor::service_version_major, ServiceDescriptor::service_version_minor),
  /// @brief Definition for AddKeyingData(instance).
    AddKeyingData(instance),
  /// @brief Definition for Encapsulate(instance).
    Encapsulate(instance),
  /// @brief Definition for GetCryptoPrimitiveId(instance).
    GetCryptoPrimitiveId(instance),
  /// @brief Definition for GetEncapsulatedSize(instance).
    GetEncapsulatedSize(instance),
  /// @brief Definition for GetExtensionService(instance).
    GetExtensionService(instance),
  /// @brief Definition for GetKekEntropy(instance).
    GetKekEntropy(instance),
  /// @brief Definition for IsInitialized(instance).
    IsInitialized(instance),
  /// @brief Definition for Reset(instance).
    Reset(instance),
  /// @brief Definition for SetKey(instance).
    SetKey(instance)
  {}
  /// @brief Definition for KeyEncapsulatorPublicContextImpl.
  ~KeyEncapsulatorPublicContextImpl() noexcept override {}

  /// @brief Definition for GetAddKeyingData.
        ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::AddKeyingData& GetAddKeyingData() noexcept override { return AddKeyingData; }
  /// @brief Definition for GetEncapsulate.
        ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::Encapsulate& GetEncapsulate() noexcept override { return Encapsulate; }
  /// @brief Definition for GetGetCryptoPrimitiveId.
        ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::GetCryptoPrimitiveId& GetGetCryptoPrimitiveId() noexcept override { return GetCryptoPrimitiveId; }
  /// @brief Definition for GetGetEncapsulatedSize.
        ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::GetEncapsulatedSize& GetGetEncapsulatedSize() noexcept override { return GetEncapsulatedSize; }
  /// @brief Definition for GetGetExtensionService.
        ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::GetExtensionService& GetGetExtensionService() noexcept override { return GetExtensionService; }
  /// @brief Definition for GetGetKekEntropy.
        ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::GetKekEntropy& GetGetKekEntropy() noexcept override { return GetKekEntropy; }
  /// @brief Definition for GetIsInitialized.
        ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::IsInitialized& GetIsInitialized() noexcept override { return IsInitialized; }
  /// @brief Definition for GetReset.
        ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::Reset& GetReset() noexcept override { return Reset; }
  /// @brief Definition for GetSetKey.
        ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::SetKey& GetSetKey() noexcept override { return SetKey; }

 private:
  /// @brief Method for AddKeyingData is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::AddKeyingData, ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::AddKeyingData::signature_type> AddKeyingData;
  /// @brief Method for Encapsulate is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::Encapsulate, ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::Encapsulate::signature_type> Encapsulate;
  /// @brief Method for GetCryptoPrimitiveId is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::GetCryptoPrimitiveId, ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::GetCryptoPrimitiveId::signature_type> GetCryptoPrimitiveId;
  /// @brief Method for GetEncapsulatedSize is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::GetEncapsulatedSize, ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::GetEncapsulatedSize::signature_type> GetEncapsulatedSize;
  /// @brief Method for GetExtensionService is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::GetExtensionService, ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::GetExtensionService::signature_type> GetExtensionService;
  /// @brief Method for GetKekEntropy is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::GetKekEntropy, ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::GetKekEntropy::signature_type> GetKekEntropy;
  /// @brief Method for IsInitialized is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::IsInitialized, ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::IsInitialized::signature_type> IsInitialized;
  /// @brief Method for Reset is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::Reset, ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::Reset::signature_type> Reset;
  /// @brief Method for SetKey is fire & forget: false.
  ara::com::internal::vsomeip::proxy::MethodImpl<descriptors::SetKey, ara::crypto::ipccom::cryp::keyencapsulatorpubliccontext::proxy::methods::SetKey::signature_type> SetKey;
};

} // namespace vsomeip
} // namespace keyencapsulatorpubliccontext_binding
} // namespace keyencapsulatorpubliccontext
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOCLIENTEXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_KEYENCAPSULATORPUBLICCONTEXT_PROXY_VSOMEIP_KEYENCAPSULATORPUBLICCONTEXT_H_

