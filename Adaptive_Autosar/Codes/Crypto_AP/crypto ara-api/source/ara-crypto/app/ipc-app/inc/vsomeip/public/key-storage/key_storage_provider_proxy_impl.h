// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_storage_provider_proxy_impl.h
//
// Purpose     : Key storage provider proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_KEY_STORAGE_KEY_STORAGE_PROVIDER_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_KEY_STORAGE_KEY_STORAGE_PROVIDER_PROXY_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/keys/key_storage_provider.h"
#include "key_slot_proxy_impl.h"
#include "keystorageprovider_proxy.h"
#include "vsomeip_interface.h"

namespace ara
{
namespace crypto
{
namespace keys
{
class KeyStorageProviderProxyImpl : public ara::crypto::keys::KeyStorageProvider
{
 public:
  KeyStorageProviderProxyImpl(const CryptoServiceInstanceId serviceInstanceId) noexcept;
  virtual ~KeyStorageProviderProxyImpl() noexcept = default;

  ara::core::Result<KeySlot::Uptr> LoadKeySlot(ara::core::String& iSpecify) noexcept override;

 private:
  void Initialize(const CryptoServiceInstanceId serviceInstanceId);
  ara::log::Logger& logger_;
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::keys::keystorageprovider::proxy::KeyStorageProviderProxy>
    vsomeipInstance_;
  std::shared_ptr<ara::crypto::ipccom::keys::keystorageprovider::proxy::KeyStorageProviderProxy>
    keyStorageProviderHandler_;
};

} // namespace keys
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_KEY_STORAGE_KEY_STORAGE_PROVIDER_PROXY_IMPL_H_
