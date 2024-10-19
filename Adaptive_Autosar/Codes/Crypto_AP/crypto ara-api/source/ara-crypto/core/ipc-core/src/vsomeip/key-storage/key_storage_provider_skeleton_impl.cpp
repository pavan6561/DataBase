// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_storage_provider_skeleton_impl.cpp
//
// Purpose     : Key storage provider skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "key_storage_provider_skeleton_impl.h"

namespace ara
{
namespace crypto
{
namespace keys
{
KeyStorageProviderSkeletonImpl::KeyStorageProviderSkeletonImpl(CryptoServiceInstanceId instance_id,
  ara::crypto::keys::KeyStorageProvider::Uptr keyStorgeUptr) noexcept(false) :
  ara::crypto::ipccom::keys::keystorageprovider::skeleton::KeyStorageProviderSkeleton(
    ara::com::InstanceIdentifier(instance_id)),
  keyStorageUptr_{ std::move(keyStorgeUptr) }, serviceInstanceId_{ 0 }, logger_{
    ara::log::CreateLogger("KSSI", "Key Storage Provider Skeleton", ara::log::LogLevel::kVerbose)
  }
{
  logger_.LogDebug() << "KeyStorageProviderSkeletonImpl Constructor";
}

CryptoServiceInstanceId KeyStorageProviderSkeletonImpl::GenerateCryptoServiceInstanceId() noexcept
{
  std::lock_guard<std::mutex> lock(mutex_instanceId_);
  serviceInstanceId_ = serviceInstanceId_ + 1;
  CryptoServiceInstanceId strInstance{ "SOME/IP:" };
  CryptoServiceInstanceId tempStr{ std::to_string(static_cast<unsigned>(serviceInstanceId_)) };
  strInstance.append(tempStr);
  return strInstance;
}

ara::core::Future<ara::crypto::ipccom::keys::keystorageprovider::KeyStorageProvider::LoadKeySlotOutput>
  KeyStorageProviderSkeletonImpl::LoadKeySlot(const String& iSpecify)
{
  logger_.LogInfo() << "IPC-CORE::KeyStorageProviderSkeletonImpl : " << __FUNCTION__;
  ara::core::Future<LoadKeySlotOutput>::PromiseType promise;
  ara::core::String instanceSpecifire{ iSpecify };
  ara::core::Result<KeySlot::Uptr> keySlotUptr = keyStorageUptr_->LoadKeySlot(instanceSpecifire);
  if (keySlotUptr.HasValue())
  {
    ara::crypto::ipccom::keys::keystorageprovider::KeyStorageProvider::LoadKeySlotOutput
      localLoadKeySlotOutput;
    CryptoServiceInstanceId keyStoragelocalInstanceId{ iSpecify };
    logger_.LogInfo() << "IPC-CORE::KeyStorageProviderSkeletonImpl InstanceId : "
                      << keyStoragelocalInstanceId;
    std::unique_ptr<KeySlotInterfaceSkeletonImpl> keySlotSkeleton
      = std::make_unique<KeySlotInterfaceSkeletonImpl>(
        keyStoragelocalInstanceId, std::move(keySlotUptr).Value());
    keySlotSkeleton->OfferService();
    keySlotContainer_.emplace(keyStoragelocalInstanceId, std::move(keySlotSkeleton));
    localLoadKeySlotOutput.keyStorageInstanceId = keyStoragelocalInstanceId;
    promise.set_value(localLoadKeySlotOutput);
  }
  else
  {
    // error implementation
  }
  return promise.get_future();
}

ara::core::Future<void> KeyStorageProviderSkeletonImpl::BeginTransaction(const uint64& id)
{
  static_cast<void>(id);
  logger_.LogInfo() << "IPC-CORE::KeyStorageProviderSkeletonImpl : " << __FUNCTION__
                    << "-> Not supported for now";
  return ara::core::Future<void>();
}

ara::core::Future<
  ara::crypto::ipccom::keys::keystorageprovider::KeyStorageProvider::GetRegisteredObserverOutput>
  KeyStorageProviderSkeletonImpl::GetRegisteredObserver()
{
  ara::core::Future<GetRegisteredObserverOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::KeyStorageProviderSkeletonImpl : " << __FUNCTION__
                    << "-> Not supported for now";
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::keys::keystorageprovider::KeyStorageProvider::RegisterObserverOutput>
  KeyStorageProviderSkeletonImpl::RegisterObserver(const CryptoServiceInstanceId& inputInstanceId)
{
  static_cast<void>(inputInstanceId);
  ara::core::Future<RegisterObserverOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::KeyStorageProviderSkeletonImpl : " << __FUNCTION__
                    << "-> Not supported for now";
  return promise.get_future();
}

ara::core::Future<void> KeyStorageProviderSkeletonImpl::RollbackTransaction(const uint64& id)
{
  static_cast<void>(id);
  logger_.LogInfo() << "IPC-CORE::KeyStorageProviderSkeletonImpl : " << __FUNCTION__
                    << "-> Not supported for now";
  return ara::core::Future<void>();
}

ara::core::Future<void> KeyStorageProviderSkeletonImpl::UnsubscribeObserver(
  const CryptoServiceInstanceId& instanceId)
{
  static_cast<void>(instanceId);
  logger_.LogInfo() << "IPC-CORE::KeyStorageProviderSkeletonImpl : " << __FUNCTION__
                    << "-> Not supported for now";
  return ara::core::Future<void>();
}

} // namespace keys
} // namespace crypto
} // namespace ara
