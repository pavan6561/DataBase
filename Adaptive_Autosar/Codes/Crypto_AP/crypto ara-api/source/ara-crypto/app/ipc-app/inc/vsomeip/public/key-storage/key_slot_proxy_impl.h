// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_slot_proxy_impl.h
//
// Purpose     : Key slot proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_KEY_STORAGE_KEY_SLOT_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_KEY_STORAGE_KEY_SLOT_PROXY_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/map.h>
#include <ara/log/logging.h>
#include <memory>

#include "ara/crypto/keys/keyslot.h"
#include "keyslotinterface_proxy.h"
#include "keystorageprovider_proxy.h"
#include "proxy_data_storage_container.h"
#include "vsomeip_interface.h"

namespace ara
{
namespace crypto
{
namespace keys
{
class KeySlotProxyImpl : public ara::crypto::keys::KeySlot
{
 public:
  KeySlotProxyImpl(const CryptoServiceInstanceId serviceInstanceID,
    std::shared_ptr<ara::crypto::ipccom::keys::keystorageprovider::proxy::KeyStorageProviderProxy>
      keyStorageProviderHandler_) noexcept;
  virtual ~KeySlotProxyImpl() noexcept = default;

  ara::core::Result<cryp::CryptoProvider::Uptr> MyProvider() const noexcept override;
  bool IsEmpty() const noexcept;
  ara::core::Result<void> Clear() noexcept override;
  ara::core::Result<void> SaveCopy(const IOInterface& container) noexcept override;
  ara::core::Result<KeySlotPrototypeProps> GetPrototypedProps() const noexcept override;
  ara::core::Result<KeySlotContentProps> GetContentProps() const noexcept override;
  ara::core::Result<ara::crypto::IOInterface::Uptr> Open(
    bool subscribeForUpdates = false, bool writeable = false) const noexcept override;

 private:
  // Private member function
  bool Initialize(const CryptoServiceInstanceId serviceInstanceID);
  ara::log::Logger& logger_;
  ara::crypto::vsomeip::VsomeipInterface<ara::crypto::ipccom::keys::keyslot::proxy::KeySlotInterfaceProxy>
    vsomeipKeySlotInstance_;
  std::shared_ptr<ara::crypto::ipccom::keys::keystorageprovider::proxy::KeyStorageProviderProxy>
    keySlotKeyStorageProviderHandler_;
  std::shared_ptr<ara::crypto::ipccom::keys::keyslot::proxy::KeySlotInterfaceProxy> keySlotProxyHandler_;
};
} // namespace keys
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_KEY_STORAGE_KEY_SLOT_PROXY_IMPL_H_
