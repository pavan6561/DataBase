// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetrickeywrappercontext_proxy.h
//
// Purpose     : Implementation of file symmetrickeywrappercontext_proxy.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_SYMMETRICKEYWRAPPER_SYMMETRICKEYWRAPPERCONTEXT_PROXY_H_
#define CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_SYMMETRICKEYWRAPPER_SYMMETRICKEYWRAPPERCONTEXT_PROXY_H_

#include <ara/com/internal/proxy/ara_proxy_base.h>
#include "ara/crypto/ipccom/cryp/symmetrickeywrapper/symmetrickeywrappercontext_common.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace symmetrickeywrapper
{
namespace proxy
{

namespace methods
{
  /// @brief Using for CalculateWrappedKeySize.
  using CalculateWrappedKeySize = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::CalculateWrappedKeySizeOutput(const uint64& )>;
  /// @brief Using for GetCryptoPrimitiveId.
  using GetCryptoPrimitiveId = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetCryptoPrimitiveIdOutput()>;
  /// @brief Using for GetExtensionService.
  using GetExtensionService = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetExtensionServiceOutput()>;
  /// @brief Using for GetMaxTargetKeyLength.
  using GetMaxTargetKeyLength = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetMaxTargetKeyLengthOutput()>;
  /// @brief Using for GetTargetKeyGranularity.
  using GetTargetKeyGranularity = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::GetTargetKeyGranularityOutput()>;
  /// @brief Using for IsInitialized.
  using IsInitialized = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::IsInitializedOutput()>;
  /// @brief Using for Reset.
  using Reset = ara::com::internal::proxy::Method<void()>;
  /// @brief Using for SetKey.
  using SetKey = ara::com::internal::proxy::Method<void(const CryptoServiceInstanceId& ,const CryptoTransformDataType& )>;
  /// @brief Using for UnwrapKey.
  using UnwrapKey = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::UnwrapKeyOutput(const CryptoReadOnlyMemRegion& ,const CryptoAlgId& ,const CryptoAllowedUsageFlags& )>;
  /// @brief Using for UnwrapSeed.
  using UnwrapSeed = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::UnwrapSeedOutput(const CryptoReadOnlyMemRegion& ,const CryptoAlgId& ,const CryptoAllowedUsageFlags& )>;
  /// @brief Using for WrapKeyMaterial.
  using WrapKeyMaterial = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext::WrapKeyMaterialOutput(const RestrictedUseObjectStructure& )>;
} // namespace methods



/// @brief Class for SymmetricKeyWrapperContextProxyBase.
class SymmetricKeyWrapperContextProxyBase : public ara::com::internal::proxy::ProxyBindingBase, public ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext {
 public:
  /// @brief Method for CalculateWrappedKeySize.
  virtual methods::CalculateWrappedKeySize& GetCalculateWrappedKeySize() = 0;
  /// @brief Method for GetCryptoPrimitiveId.
  virtual methods::GetCryptoPrimitiveId& GetGetCryptoPrimitiveId() = 0;
  /// @brief Method for GetExtensionService.
  virtual methods::GetExtensionService& GetGetExtensionService() = 0;
  /// @brief Method for GetMaxTargetKeyLength.
  virtual methods::GetMaxTargetKeyLength& GetGetMaxTargetKeyLength() = 0;
  /// @brief Method for GetTargetKeyGranularity.
  virtual methods::GetTargetKeyGranularity& GetGetTargetKeyGranularity() = 0;
  /// @brief Method for IsInitialized.
  virtual methods::IsInitialized& GetIsInitialized() = 0;
  /// @brief Method for Reset.
  virtual methods::Reset& GetReset() = 0;
  /// @brief Method for SetKey.
  virtual methods::SetKey& GetSetKey() = 0;
  /// @brief Method for UnwrapKey.
  virtual methods::UnwrapKey& GetUnwrapKey() = 0;
  /// @brief Method for UnwrapSeed.
  virtual methods::UnwrapSeed& GetUnwrapSeed() = 0;
  /// @brief Method for WrapKeyMaterial.
  virtual methods::WrapKeyMaterial& GetWrapKeyMaterial() = 0;
};

/// @brief Class for SymmetricKeyWrapperContextProxy.
class SymmetricKeyWrapperContextProxy : public ara::com::internal::proxy::ProxyBase<SymmetricKeyWrapperContextProxyBase>, public ara::crypto::ipccom::cryp::symmetrickeywrapper::SymmetricKeyWrapperContext {
 public:
  /// @brief Definition for SymmetricKeyWrapperContextProxy.
  explicit SymmetricKeyWrapperContextProxy(const HandleType& proxy_base_factory) :
    ara::com::internal::proxy::ProxyBase<SymmetricKeyWrapperContextProxyBase>(proxy_base_factory),
    /// @brief Function GetCalculateWrappedKeySize.
    CalculateWrappedKeySize(proxy_base_->GetCalculateWrappedKeySize()),
    /// @brief Function GetGetCryptoPrimitiveId.
    GetCryptoPrimitiveId(proxy_base_->GetGetCryptoPrimitiveId()),
    /// @brief Function GetGetExtensionService.
    GetExtensionService(proxy_base_->GetGetExtensionService()),
    /// @brief Function GetGetMaxTargetKeyLength.
    GetMaxTargetKeyLength(proxy_base_->GetGetMaxTargetKeyLength()),
    /// @brief Function GetGetTargetKeyGranularity.
    GetTargetKeyGranularity(proxy_base_->GetGetTargetKeyGranularity()),
    /// @brief Function GetIsInitialized.
    IsInitialized(proxy_base_->GetIsInitialized()),
    /// @brief Function GetReset.
    Reset(proxy_base_->GetReset()),
    /// @brief Function GetSetKey.
    SetKey(proxy_base_->GetSetKey()),
    /// @brief Function GetUnwrapKey.
    UnwrapKey(proxy_base_->GetUnwrapKey()),
    /// @brief Function GetUnwrapSeed.
    UnwrapSeed(proxy_base_->GetUnwrapSeed()),
    /// @brief Function GetWrapKeyMaterial.
    WrapKeyMaterial(proxy_base_->GetWrapKeyMaterial())
  {}

  /// @brief Proxy shall be move constructable.
  explicit SymmetricKeyWrapperContextProxy(SymmetricKeyWrapperContextProxy&&) = default;

  /// @brief Proxy shall be move assignable.
  SymmetricKeyWrapperContextProxy& operator=(SymmetricKeyWrapperContextProxy&&) = default;

  /// @brief Proxy shall not be copy constructable.
  explicit SymmetricKeyWrapperContextProxy(const SymmetricKeyWrapperContextProxy&) = delete;

  /// @brief Proxy shall not be copy assignable.
  SymmetricKeyWrapperContextProxy& operator=(const SymmetricKeyWrapperContextProxy&) = delete;

  /// @brief Method for CalculateWrappedKeySize.
  methods::CalculateWrappedKeySize& CalculateWrappedKeySize;
  /// @brief Method for GetCryptoPrimitiveId.
  methods::GetCryptoPrimitiveId& GetCryptoPrimitiveId;
  /// @brief Method for GetExtensionService.
  methods::GetExtensionService& GetExtensionService;
  /// @brief Method for GetMaxTargetKeyLength.
  methods::GetMaxTargetKeyLength& GetMaxTargetKeyLength;
  /// @brief Method for GetTargetKeyGranularity.
  methods::GetTargetKeyGranularity& GetTargetKeyGranularity;
  /// @brief Method for IsInitialized.
  methods::IsInitialized& IsInitialized;
  /// @brief Method for Reset.
  methods::Reset& Reset;
  /// @brief Method for SetKey.
  methods::SetKey& SetKey;
  /// @brief Method for UnwrapKey.
  methods::UnwrapKey& UnwrapKey;
  /// @brief Method for UnwrapSeed.
  methods::UnwrapSeed& UnwrapSeed;
  /// @brief Method for WrapKeyMaterial.
  methods::WrapKeyMaterial& WrapKeyMaterial;
};
} // namespace proxy

} // namespace symmetrickeywrapper
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_SYMMETRICKEYWRAPPER_SYMMETRICKEYWRAPPERCONTEXT_PROXY_H_

