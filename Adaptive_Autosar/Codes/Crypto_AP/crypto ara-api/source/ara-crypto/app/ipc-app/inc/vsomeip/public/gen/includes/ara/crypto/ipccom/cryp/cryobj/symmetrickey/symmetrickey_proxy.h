// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetrickey_proxy.h
//
// Purpose     : Implementation of file symmetrickey_proxy.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_CRYOBJ_SYMMETRICKEY_SYMMETRICKEY_PROXY_H_
#define CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_CRYOBJ_SYMMETRICKEY_SYMMETRICKEY_PROXY_H_

#include <ara/com/internal/proxy/ara_proxy_base.h>
#include "ara/crypto/ipccom/cryp/cryobj/symmetrickey/symmetrickey_common.h"

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
namespace proxy
{

namespace methods
{
  /// @brief Using for GetAllowedUsage.
  using GetAllowedUsage = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetAllowedUsageOutput()>;
  /// @brief Using for GetCryptoPrimitiveId.
  using GetCryptoPrimitiveId = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetCryptoPrimitiveIdOutput()>;
  /// @brief Using for GetObjectId.
  using GetObjectId = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetObjectIdOutput()>;
  /// @brief Using for GetPayloadSize.
  using GetPayloadSize = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::GetPayloadSizeOutput()>;
  /// @brief Using for HasDependence.
  using HasDependence = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::HasDependenceOutput()>;
  /// @brief Using for IsExportable.
  using IsExportable = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::IsExportableOutput()>;
  /// @brief Using for IsSession.
  using IsSession = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey::IsSessionOutput()>;
  /// @brief Using for Save.
  using Save = ara::com::internal::proxy::Method<void(const CryptoServiceInstanceId& )>;
} // namespace methods



/// @brief Class for SymmetricKeyProxyBase.
class SymmetricKeyProxyBase : public ara::com::internal::proxy::ProxyBindingBase, public ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey {
 public:
  /// @brief Method for GetAllowedUsage.
  virtual methods::GetAllowedUsage& GetGetAllowedUsage() = 0;
  /// @brief Method for GetCryptoPrimitiveId.
  virtual methods::GetCryptoPrimitiveId& GetGetCryptoPrimitiveId() = 0;
  /// @brief Method for GetObjectId.
  virtual methods::GetObjectId& GetGetObjectId() = 0;
  /// @brief Method for GetPayloadSize.
  virtual methods::GetPayloadSize& GetGetPayloadSize() = 0;
  /// @brief Method for HasDependence.
  virtual methods::HasDependence& GetHasDependence() = 0;
  /// @brief Method for IsExportable.
  virtual methods::IsExportable& GetIsExportable() = 0;
  /// @brief Method for IsSession.
  virtual methods::IsSession& GetIsSession() = 0;
  /// @brief Method for Save.
  virtual methods::Save& GetSave() = 0;
};

/// @brief Class for SymmetricKeyProxy.
class SymmetricKeyProxy : public ara::com::internal::proxy::ProxyBase<SymmetricKeyProxyBase>, public ara::crypto::ipccom::cryp::cryobj::symmetrickey::SymmetricKey {
 public:
  /// @brief Definition for SymmetricKeyProxy.
  explicit SymmetricKeyProxy(const HandleType& proxy_base_factory) :
    ara::com::internal::proxy::ProxyBase<SymmetricKeyProxyBase>(proxy_base_factory),
    /// @brief Function GetGetAllowedUsage.
    GetAllowedUsage(proxy_base_->GetGetAllowedUsage()),
    /// @brief Function GetGetCryptoPrimitiveId.
    GetCryptoPrimitiveId(proxy_base_->GetGetCryptoPrimitiveId()),
    /// @brief Function GetGetObjectId.
    GetObjectId(proxy_base_->GetGetObjectId()),
    /// @brief Function GetGetPayloadSize.
    GetPayloadSize(proxy_base_->GetGetPayloadSize()),
    /// @brief Function GetHasDependence.
    HasDependence(proxy_base_->GetHasDependence()),
    /// @brief Function GetIsExportable.
    IsExportable(proxy_base_->GetIsExportable()),
    /// @brief Function GetIsSession.
    IsSession(proxy_base_->GetIsSession()),
    /// @brief Function GetSave.
    Save(proxy_base_->GetSave())
  {}

  /// @brief Proxy shall be move constructable.
  explicit SymmetricKeyProxy(SymmetricKeyProxy&&) = default;

  /// @brief Proxy shall be move assignable.
  SymmetricKeyProxy& operator=(SymmetricKeyProxy&&) = default;

  /// @brief Proxy shall not be copy constructable.
  explicit SymmetricKeyProxy(const SymmetricKeyProxy&) = delete;

  /// @brief Proxy shall not be copy assignable.
  SymmetricKeyProxy& operator=(const SymmetricKeyProxy&) = delete;

  /// @brief Method for GetAllowedUsage.
  methods::GetAllowedUsage& GetAllowedUsage;
  /// @brief Method for GetCryptoPrimitiveId.
  methods::GetCryptoPrimitiveId& GetCryptoPrimitiveId;
  /// @brief Method for GetObjectId.
  methods::GetObjectId& GetObjectId;
  /// @brief Method for GetPayloadSize.
  methods::GetPayloadSize& GetPayloadSize;
  /// @brief Method for HasDependence.
  methods::HasDependence& HasDependence;
  /// @brief Method for IsExportable.
  methods::IsExportable& IsExportable;
  /// @brief Method for IsSession.
  methods::IsSession& IsSession;
  /// @brief Method for Save.
  methods::Save& Save;
};
} // namespace proxy

} // namespace symmetrickey
} // namespace cryobj
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_CRYOBJ_SYMMETRICKEY_SYMMETRICKEY_PROXY_H_

