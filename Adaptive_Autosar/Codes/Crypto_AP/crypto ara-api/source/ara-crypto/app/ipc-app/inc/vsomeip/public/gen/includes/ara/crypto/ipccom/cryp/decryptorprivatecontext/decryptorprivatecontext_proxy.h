// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : decryptorprivatecontext_proxy.h
//
// Purpose     : Implementation of file decryptorprivatecontext_proxy.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_DECRYPTORPRIVATECONTEXT_DECRYPTORPRIVATECONTEXT_PROXY_H_
#define CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_DECRYPTORPRIVATECONTEXT_DECRYPTORPRIVATECONTEXT_PROXY_H_

#include <ara/com/internal/proxy/ara_proxy_base.h>
#include "ara/crypto/ipccom/cryp/decryptorprivatecontext/decryptorprivatecontext_common.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace decryptorprivatecontext
{
namespace proxy
{

namespace methods
{
  /// @brief Using for GetCryptoPrimitiveId.
  using GetCryptoPrimitiveId = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::GetCryptoPrimitiveIdOutput()>;
  /// @brief Using for GetCryptoService.
  using GetCryptoService = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::GetCryptoServiceOutput()>;
  /// @brief Using for IsInitialized.
  using IsInitialized = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::IsInitializedOutput()>;
  /// @brief Using for ProcessBlockReadOnly.
  using ProcessBlockReadOnly = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext::ProcessBlockReadOnlyOutput(const CryptoReadOnlyMemRegion& ,const Boolean& )>;
  /// @brief Using for Reset.
  using Reset = ara::com::internal::proxy::Method<void()>;
  /// @brief Using for SetKey.
  using SetKey = ara::com::internal::proxy::Method<void(const CryptoServiceInstanceId& )>;
} // namespace methods



/// @brief Class for DecryptorPrivateContextProxyBase.
class DecryptorPrivateContextProxyBase : public ara::com::internal::proxy::ProxyBindingBase, public ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext {
 public:
  /// @brief Method for GetCryptoPrimitiveId.
  virtual methods::GetCryptoPrimitiveId& GetGetCryptoPrimitiveId() = 0;
  /// @brief Method for GetCryptoService.
  virtual methods::GetCryptoService& GetGetCryptoService() = 0;
  /// @brief Method for IsInitialized.
  virtual methods::IsInitialized& GetIsInitialized() = 0;
  /// @brief Method for ProcessBlockReadOnly.
  virtual methods::ProcessBlockReadOnly& GetProcessBlockReadOnly() = 0;
  /// @brief Method for Reset.
  virtual methods::Reset& GetReset() = 0;
  /// @brief Method for SetKey.
  virtual methods::SetKey& GetSetKey() = 0;
};

/// @brief Class for DecryptorPrivateContextProxy.
class DecryptorPrivateContextProxy : public ara::com::internal::proxy::ProxyBase<DecryptorPrivateContextProxyBase>, public ara::crypto::ipccom::cryp::decryptorprivatecontext::DecryptorPrivateContext {
 public:
  /// @brief Definition for DecryptorPrivateContextProxy.
  explicit DecryptorPrivateContextProxy(const HandleType& proxy_base_factory) :
    ara::com::internal::proxy::ProxyBase<DecryptorPrivateContextProxyBase>(proxy_base_factory),
    /// @brief Function GetGetCryptoPrimitiveId.
    GetCryptoPrimitiveId(proxy_base_->GetGetCryptoPrimitiveId()),
    /// @brief Function GetGetCryptoService.
    GetCryptoService(proxy_base_->GetGetCryptoService()),
    /// @brief Function GetIsInitialized.
    IsInitialized(proxy_base_->GetIsInitialized()),
    /// @brief Function GetProcessBlockReadOnly.
    ProcessBlockReadOnly(proxy_base_->GetProcessBlockReadOnly()),
    /// @brief Function GetReset.
    Reset(proxy_base_->GetReset()),
    /// @brief Function GetSetKey.
    SetKey(proxy_base_->GetSetKey())
  {}

  /// @brief Proxy shall be move constructable.
  explicit DecryptorPrivateContextProxy(DecryptorPrivateContextProxy&&) = default;

  /// @brief Proxy shall be move assignable.
  DecryptorPrivateContextProxy& operator=(DecryptorPrivateContextProxy&&) = default;

  /// @brief Proxy shall not be copy constructable.
  explicit DecryptorPrivateContextProxy(const DecryptorPrivateContextProxy&) = delete;

  /// @brief Proxy shall not be copy assignable.
  DecryptorPrivateContextProxy& operator=(const DecryptorPrivateContextProxy&) = delete;

  /// @brief Method for GetCryptoPrimitiveId.
  methods::GetCryptoPrimitiveId& GetCryptoPrimitiveId;
  /// @brief Method for GetCryptoService.
  methods::GetCryptoService& GetCryptoService;
  /// @brief Method for IsInitialized.
  methods::IsInitialized& IsInitialized;
  /// @brief Method for ProcessBlockReadOnly.
  methods::ProcessBlockReadOnly& ProcessBlockReadOnly;
  /// @brief Method for Reset.
  methods::Reset& Reset;
  /// @brief Method for SetKey.
  methods::SetKey& SetKey;
};
} // namespace proxy

} // namespace decryptorprivatecontext
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_DECRYPTORPRIVATECONTEXT_DECRYPTORPRIVATECONTEXT_PROXY_H_

