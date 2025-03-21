// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : keyderivationfunction_proxy.h
//
// Purpose     : Implementation of file keyderivationfunction_proxy.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_KEYDERIVATIONFUNCTION_KEYDERIVATIONFUNCTION_PROXY_H_
#define CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_KEYDERIVATIONFUNCTION_KEYDERIVATIONFUNCTION_PROXY_H_

#include <ara/com/internal/proxy/ara_proxy_base.h>
#include "ara/crypto/ipccom/cryp/keyderivationfunction/keyderivationfunction_common.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace keyderivationfunction
{
namespace proxy
{

namespace methods
{
  /// @brief Using for AddSaltReadOnlyMem.
  using AddSaltReadOnlyMem = ara::com::internal::proxy::Method<void(const CryptoReadOnlyMemRegion& )>;
  /// @brief Using for AddSecretSalt.
  using AddSecretSalt = ara::com::internal::proxy::Method<void(const CryptoServiceInstanceId& )>;
  /// @brief Using for ConfigIterations.
  using ConfigIterations = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::ConfigIterationsOutput(const uint32& )>;
  /// @brief Using for DeriveKey.
  using DeriveKey = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveKeyOutput(const Boolean& ,const Boolean& )>;
  /// @brief Using for DeriveSeed.
  using DeriveSeed = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::DeriveSeedOutput(const Boolean& ,const Boolean& )>;
  /// @brief Using for GetCryptoPrimitiveId.
  using GetCryptoPrimitiveId = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetCryptoPrimitiveIdOutput()>;
  /// @brief Using for GetExtensionService.
  using GetExtensionService = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetExtensionServiceOutput()>;
  /// @brief Using for GetKeyIdSize.
  using GetKeyIdSize = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetKeyIdSizeOutput()>;
  /// @brief Using for GetTargetAlgId.
  using GetTargetAlgId = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetAlgIdOutput()>;
  /// @brief Using for GetTargetAllowedUsage.
  using GetTargetAllowedUsage = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetAllowedUsageOutput()>;
  /// @brief Using for GetTargetKeyBitLength.
  using GetTargetKeyBitLength = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::GetTargetKeyBitLengthOutput()>;
  /// @brief Using for Init.
  using Init = ara::com::internal::proxy::Method<void(const CryptoReadOnlyMemRegion& ,const CryptoAlgId& ,const CryptoAllowedUsageFlags& ,const CryptoReadOnlyMemRegion& )>;
  /// @brief Using for IsInitialized.
  using IsInitialized = ara::com::internal::proxy::Method<ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction::IsInitializedOutput()>;
  /// @brief Using for Reset.
  using Reset = ara::com::internal::proxy::Method<void()>;
  /// @brief Using for SetSourceKeyMaterial.
  using SetSourceKeyMaterial = ara::com::internal::proxy::Method<void(const RestrictedUseObjectStructure& )>;
} // namespace methods



/// @brief Class for KeyDerivationFunctionProxyBase.
class KeyDerivationFunctionProxyBase : public ara::com::internal::proxy::ProxyBindingBase, public ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction {
 public:
  /// @brief Method for AddSaltReadOnlyMem.
  virtual methods::AddSaltReadOnlyMem& GetAddSaltReadOnlyMem() = 0;
  /// @brief Method for AddSecretSalt.
  virtual methods::AddSecretSalt& GetAddSecretSalt() = 0;
  /// @brief Method for ConfigIterations.
  virtual methods::ConfigIterations& GetConfigIterations() = 0;
  /// @brief Method for DeriveKey.
  virtual methods::DeriveKey& GetDeriveKey() = 0;
  /// @brief Method for DeriveSeed.
  virtual methods::DeriveSeed& GetDeriveSeed() = 0;
  /// @brief Method for GetCryptoPrimitiveId.
  virtual methods::GetCryptoPrimitiveId& GetGetCryptoPrimitiveId() = 0;
  /// @brief Method for GetExtensionService.
  virtual methods::GetExtensionService& GetGetExtensionService() = 0;
  /// @brief Method for GetKeyIdSize.
  virtual methods::GetKeyIdSize& GetGetKeyIdSize() = 0;
  /// @brief Method for GetTargetAlgId.
  virtual methods::GetTargetAlgId& GetGetTargetAlgId() = 0;
  /// @brief Method for GetTargetAllowedUsage.
  virtual methods::GetTargetAllowedUsage& GetGetTargetAllowedUsage() = 0;
  /// @brief Method for GetTargetKeyBitLength.
  virtual methods::GetTargetKeyBitLength& GetGetTargetKeyBitLength() = 0;
  /// @brief Method for Init.
  virtual methods::Init& GetInit() = 0;
  /// @brief Method for IsInitialized.
  virtual methods::IsInitialized& GetIsInitialized() = 0;
  /// @brief Method for Reset.
  virtual methods::Reset& GetReset() = 0;
  /// @brief Method for SetSourceKeyMaterial.
  virtual methods::SetSourceKeyMaterial& GetSetSourceKeyMaterial() = 0;
};

/// @brief Class for KeyDerivationFunctionProxy.
class KeyDerivationFunctionProxy : public ara::com::internal::proxy::ProxyBase<KeyDerivationFunctionProxyBase>, public ara::crypto::ipccom::cryp::keyderivationfunction::KeyDerivationFunction {
 public:
  /// @brief Definition for KeyDerivationFunctionProxy.
  explicit KeyDerivationFunctionProxy(const HandleType& proxy_base_factory) :
    ara::com::internal::proxy::ProxyBase<KeyDerivationFunctionProxyBase>(proxy_base_factory),
    /// @brief Function GetAddSaltReadOnlyMem.
    AddSaltReadOnlyMem(proxy_base_->GetAddSaltReadOnlyMem()),
    /// @brief Function GetAddSecretSalt.
    AddSecretSalt(proxy_base_->GetAddSecretSalt()),
    /// @brief Function GetConfigIterations.
    ConfigIterations(proxy_base_->GetConfigIterations()),
    /// @brief Function GetDeriveKey.
    DeriveKey(proxy_base_->GetDeriveKey()),
    /// @brief Function GetDeriveSeed.
    DeriveSeed(proxy_base_->GetDeriveSeed()),
    /// @brief Function GetGetCryptoPrimitiveId.
    GetCryptoPrimitiveId(proxy_base_->GetGetCryptoPrimitiveId()),
    /// @brief Function GetGetExtensionService.
    GetExtensionService(proxy_base_->GetGetExtensionService()),
    /// @brief Function GetGetKeyIdSize.
    GetKeyIdSize(proxy_base_->GetGetKeyIdSize()),
    /// @brief Function GetGetTargetAlgId.
    GetTargetAlgId(proxy_base_->GetGetTargetAlgId()),
    /// @brief Function GetGetTargetAllowedUsage.
    GetTargetAllowedUsage(proxy_base_->GetGetTargetAllowedUsage()),
    /// @brief Function GetGetTargetKeyBitLength.
    GetTargetKeyBitLength(proxy_base_->GetGetTargetKeyBitLength()),
    /// @brief Function GetInit.
    Init(proxy_base_->GetInit()),
    /// @brief Function GetIsInitialized.
    IsInitialized(proxy_base_->GetIsInitialized()),
    /// @brief Function GetReset.
    Reset(proxy_base_->GetReset()),
    /// @brief Function GetSetSourceKeyMaterial.
    SetSourceKeyMaterial(proxy_base_->GetSetSourceKeyMaterial())
  {}

  /// @brief Proxy shall be move constructable.
  explicit KeyDerivationFunctionProxy(KeyDerivationFunctionProxy&&) = default;

  /// @brief Proxy shall be move assignable.
  KeyDerivationFunctionProxy& operator=(KeyDerivationFunctionProxy&&) = default;

  /// @brief Proxy shall not be copy constructable.
  explicit KeyDerivationFunctionProxy(const KeyDerivationFunctionProxy&) = delete;

  /// @brief Proxy shall not be copy assignable.
  KeyDerivationFunctionProxy& operator=(const KeyDerivationFunctionProxy&) = delete;

  /// @brief Method for AddSaltReadOnlyMem.
  methods::AddSaltReadOnlyMem& AddSaltReadOnlyMem;
  /// @brief Method for AddSecretSalt.
  methods::AddSecretSalt& AddSecretSalt;
  /// @brief Method for ConfigIterations.
  methods::ConfigIterations& ConfigIterations;
  /// @brief Method for DeriveKey.
  methods::DeriveKey& DeriveKey;
  /// @brief Method for DeriveSeed.
  methods::DeriveSeed& DeriveSeed;
  /// @brief Method for GetCryptoPrimitiveId.
  methods::GetCryptoPrimitiveId& GetCryptoPrimitiveId;
  /// @brief Method for GetExtensionService.
  methods::GetExtensionService& GetExtensionService;
  /// @brief Method for GetKeyIdSize.
  methods::GetKeyIdSize& GetKeyIdSize;
  /// @brief Method for GetTargetAlgId.
  methods::GetTargetAlgId& GetTargetAlgId;
  /// @brief Method for GetTargetAllowedUsage.
  methods::GetTargetAllowedUsage& GetTargetAllowedUsage;
  /// @brief Method for GetTargetKeyBitLength.
  methods::GetTargetKeyBitLength& GetTargetKeyBitLength;
  /// @brief Method for Init.
  methods::Init& Init;
  /// @brief Method for IsInitialized.
  methods::IsInitialized& IsInitialized;
  /// @brief Method for Reset.
  methods::Reset& Reset;
  /// @brief Method for SetSourceKeyMaterial.
  methods::SetSourceKeyMaterial& SetSourceKeyMaterial;
};
} // namespace proxy

} // namespace keyderivationfunction
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_KEYDERIVATIONFUNCTION_KEYDERIVATIONFUNCTION_PROXY_H_

