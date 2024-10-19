// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_storage_provider_skeleton_impl.h
//
// Purpose     : Key storage provider skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef _ARA_CRYPTO_KEYS_KEY_STORAGE_PROVIDER_SKELETON_IMPL_H_
#define _ARA_CRYPTO_KEYS_KEY_STORAGE_PROVIDER_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/map.h>
#include <ara/core/promise.h>
#include <ara/log/logging.h>

#include "impl_type_cryptoserviceinstanceid.h"
#include "key_slot_interface_skeleton_impl.h"
#include "key_storage_provider.h"
#include "keystorageprovider_skeleton.h"

// enum class KeyStorageProviderServiceType : uint8_t
// {
//     kKeySlot = 0x01U
// }

namespace ara
{
namespace crypto
{
namespace keys
{
class KeyStorageProviderSkeletonImpl
  : public ara::crypto::ipccom::keys::keystorageprovider::skeleton::KeyStorageProviderSkeleton
{
 public:
  explicit KeyStorageProviderSkeletonImpl(CryptoServiceInstanceId instance_id,
    ara::crypto::keys::KeyStorageProvider::Uptr keyStorgeUptr) noexcept(false);
  virtual ~KeyStorageProviderSkeletonImpl() = default;

  ara::core::Future<ara::crypto::ipccom::keys::keystorageprovider::KeyStorageProvider::LoadKeySlotOutput>
    LoadKeySlot(const String& iSpecify) override;

  ara::core::Future<void> BeginTransaction(const uint64& id) override;
  ara::core::Future<
    ara::crypto::ipccom::keys::keystorageprovider::KeyStorageProvider::GetRegisteredObserverOutput>
    GetRegisteredObserver() override;
  ara::core::Future<ara::crypto::ipccom::keys::keystorageprovider::KeyStorageProvider::RegisterObserverOutput>
    RegisterObserver(const CryptoServiceInstanceId& inputInstanceId) override;
  ara::core::Future<void> RollbackTransaction(const uint64& id) override;
  ara::core::Future<void> UnsubscribeObserver(const CryptoServiceInstanceId& instanceId) override;

 private:
  std::mutex mutex_instanceId_;
  ara::crypto::keys::KeyStorageProvider::Uptr keyStorageUptr_;
  ara::core::Map<CryptoServiceInstanceId, std::unique_ptr<KeySlotInterfaceSkeletonImpl>> keySlotContainer_;
  uint16_t serviceInstanceId_;
  ara::log::Logger& logger_;
  CryptoServiceInstanceId GenerateCryptoServiceInstanceId() noexcept;
};

} // namespace keys
} // namespace crypto
} // namespace ara

#endif // _ARA_CRYPTO_KEYS_KEY_STORAGE_PROVIDER_SKELETON_IMPL_H_