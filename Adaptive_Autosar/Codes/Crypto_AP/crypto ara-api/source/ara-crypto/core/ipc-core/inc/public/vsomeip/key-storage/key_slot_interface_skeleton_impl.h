// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_slot_interface_skeleton_impl.h
//
// Purpose     : Key slot interface skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef _ARA_CRYPTO_KEYS_KEY_SLOT_INTERFACE_SKELETON_IMPL_H_
#define _ARA_CRYPTO_KEYS_KEY_SLOT_INTERFACE_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/promise.h>
#include <ara/log/logging.h>

#include "impl_type_cryptoserviceinstanceid.h"
#include "iointerface_skeleton_impl.h"
#include "keyslot.h"
#include "keyslotinterface_skeleton.h"
#include "skeleton_data_storage_container.h"

namespace ara
{
namespace crypto
{
namespace keys
{
class KeySlotInterfaceSkeletonImpl
  : public ara::crypto::ipccom::keys::keyslot::skeleton::KeySlotInterfaceSkeleton
{
 public:
  KeySlotInterfaceSkeletonImpl(CryptoServiceInstanceId const instanceId, KeySlot::Uptr keySlotUptr) noexcept;
  virtual ~KeySlotInterfaceSkeletonImpl() = default;

  ara::core::Future<void> Clear() override;
  ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::IsEmptyOutput> IsEmpty() override;
  ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::MyProviderOutput>
    MyProvider() override;

  ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::GetContentPropsOutput>
    GetContentProps() override;
  ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::GetPrototypedPropsOutput>
    GetPrototypedProps() override;
  ara::core::Future<void> SaveCopy(const CryptoServiceInstanceId& instanceId) override;

  ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::OpenOutput> Open(
    const boolean& subscribeForUpdates, const boolean& writeable) override;

 private:
  CryptoServiceInstanceId GenerateCryptoServiceInstanceId() noexcept;
  KeySlot::Uptr keySlotUptr_;
  std::mutex mutex_instanceId_;
  std::uint16_t serviceInstanceId_;
  ara::core::Map<CryptoServiceInstanceId, std::unique_ptr<ara::crypto::common::IOInterfaceSkeletonImpl>>
    ioInterfaceContainer_;
  ara::log::Logger& logger_;
};

} // namespace keys
} // namespace crypto
} // namespace ara

#endif // _ARA_CRYPTO_KEYS_KEY_SLOT_INTERFACE_SKELETON_IMPL_H_