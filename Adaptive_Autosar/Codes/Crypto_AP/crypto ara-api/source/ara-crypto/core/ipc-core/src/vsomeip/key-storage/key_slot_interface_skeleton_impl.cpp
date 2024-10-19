// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_slot_interface_skeleton_impl.cpp
//
// Purpose     : Key slot interface skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "key_slot_interface_skeleton_impl.h"

namespace ara
{
namespace crypto
{
namespace keys
{
KeySlotInterfaceSkeletonImpl::KeySlotInterfaceSkeletonImpl(
  CryptoServiceInstanceId const instanceId, KeySlot::Uptr keySlotUptr) noexcept :
  ara::crypto::ipccom::keys::keyslot::skeleton::KeySlotInterfaceSkeleton(
    ara::com::InstanceIdentifier(instanceId)),
  keySlotUptr_{ std::move(keySlotUptr) }, serviceInstanceId_{ 0 }, logger_{
    ara::log::CreateLogger("KSLT", "Key Slot Interface Skeleton", ara::log::LogLevel::kVerbose)
  }
{
  logger_.LogDebug() << "KeySlotInterfaceSkeletonImpl Constructor";
}

CryptoServiceInstanceId KeySlotInterfaceSkeletonImpl::GenerateCryptoServiceInstanceId() noexcept
{
  std::lock_guard<std::mutex> lock(mutex_instanceId_);
  serviceInstanceId_ = serviceInstanceId_ + 1;
  CryptoServiceInstanceId strInstance{ "SOME/IP:" };
  CryptoServiceInstanceId tempStr{ std::to_string(static_cast<unsigned>(serviceInstanceId_)) };
  strInstance.append(tempStr);
  return strInstance;
}

ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::GetContentPropsOutput>
  KeySlotInterfaceSkeletonImpl::GetContentProps()
{
  ara::core::Future<GetContentPropsOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::KeySlotInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::core::Result<KeySlotContentProps> keySlotContentPrototype = keySlotUptr_->GetContentProps();
  if (keySlotContentPrototype.HasValue())
  {
    KeySlotContentProps keySlotProObj = keySlotContentPrototype.Value();
    GetContentPropsOutput contentPropsOutput;
    ara::crypto::CryptoObjectType objectType = keySlotProObj.mObjectType;
    contentPropsOutput.KeySlotContentProps.mAlgId = keySlotProObj.mAlgId;
    contentPropsOutput.KeySlotContentProps.mObjectSize = keySlotProObj.mObjectSize;
    contentPropsOutput.KeySlotContentProps.mContentAllowedUsage = keySlotProObj.mContentAllowedUsage;
    contentPropsOutput.KeySlotContentProps.mObjectUid.mGeneratorUid.mQwordLs
      = keySlotProObj.mObjectUid.mGeneratorUid.mQwordLs;
    contentPropsOutput.KeySlotContentProps.mObjectUid.mGeneratorUid.mQwordMs
      = keySlotProObj.mObjectUid.mGeneratorUid.mQwordMs;
    contentPropsOutput.KeySlotContentProps.mObjectUid.mVersionStamp = keySlotProObj.mObjectUid.mVersionStamp;
    if (objectType == ara::crypto::CryptoObjectType::kUndefined)
    {
      contentPropsOutput.KeySlotContentProps.mObjectType = ObjectTypeIPC::Undefined;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kSymmetricKey)
    {
      contentPropsOutput.KeySlotContentProps.mObjectType = ObjectTypeIPC::SymmetricKey;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kSecretSeed)
    {
      contentPropsOutput.KeySlotContentProps.mObjectType = ObjectTypeIPC::SecretSeed;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kSignature)
    {
      contentPropsOutput.KeySlotContentProps.mObjectType = ObjectTypeIPC::Signature;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kPublicKey)
    {
      contentPropsOutput.KeySlotContentProps.mObjectType = ObjectTypeIPC::PublicKey;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kPrivateKey)
    {
      contentPropsOutput.KeySlotContentProps.mObjectType = ObjectTypeIPC::PrivateKey;
    }
    promise.set_value(contentPropsOutput);
  }
  else
  {
    // error implementation
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::GetPrototypedPropsOutput>
  KeySlotInterfaceSkeletonImpl::GetPrototypedProps()
{
  ara::core::Future<GetPrototypedPropsOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::KeySlotInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::core::Result<KeySlotPrototypeProps> keySlotPrototype = keySlotUptr_->GetPrototypedProps();
  if (keySlotPrototype.HasValue())
  {
    KeySlotPrototypeProps keySlotPropObj = keySlotPrototype.Value();
    GetPrototypedPropsOutput prototypePropOutput;
    ara::crypto::CryptoObjectType objectType = keySlotPropObj.mObjectType;
    ara::crypto::KeySlotType keySlotType = keySlotPropObj.mSlotType;
    prototypePropOutput.KeySlotPrototypeProps.mAlgId = keySlotPropObj.mAlgId;
    prototypePropOutput.KeySlotPrototypeProps.mAllocateSpareSlot = keySlotPropObj.mAllocateSpareSlot;
    prototypePropOutput.KeySlotPrototypeProps.mAllowCtTypeChange = keySlotPropObj.mAllowContentTypeChange;
    prototypePropOutput.KeySlotPrototypeProps.mContentAllowedUsage = keySlotPropObj.mContentAllowedUsage;
    prototypePropOutput.KeySlotPrototypeProps.mExportAllowed = keySlotPropObj.mExportAllowed;
    prototypePropOutput.KeySlotPrototypeProps.mMaxUpdateAllowed = keySlotPropObj.mMaxUpdateAllowed;
    prototypePropOutput.KeySlotPrototypeProps.mSlotCapacity = keySlotPropObj.mSlotCapacity;
    if (keySlotType == ara::crypto::KeySlotType::kMachine)
    {
      prototypePropOutput.KeySlotPrototypeProps.mSlotType = SlotTypeProps::Machine;
    }
    else if (keySlotType == ara::crypto::KeySlotType::kApplication)
    {
      prototypePropOutput.KeySlotPrototypeProps.mSlotType = SlotTypeProps::Application;
    }
    if (objectType == ara::crypto::CryptoObjectType::kUndefined)
    {
      prototypePropOutput.KeySlotPrototypeProps.mObjectType = ObjectTypeIPC::Undefined;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kSymmetricKey)
    {
      prototypePropOutput.KeySlotPrototypeProps.mObjectType = ObjectTypeIPC::SymmetricKey;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kSecretSeed)
    {
      prototypePropOutput.KeySlotPrototypeProps.mObjectType = ObjectTypeIPC::SecretSeed;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kSignature)
    {
      prototypePropOutput.KeySlotPrototypeProps.mObjectType = ObjectTypeIPC::Signature;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kPublicKey)
    {
      prototypePropOutput.KeySlotPrototypeProps.mObjectType = ObjectTypeIPC::PublicKey;
    }
    else if (objectType == ara::crypto::CryptoObjectType::kPrivateKey)
    {
      prototypePropOutput.KeySlotPrototypeProps.mObjectType = ObjectTypeIPC::PrivateKey;
    }
    promise.set_value(prototypePropOutput);
  }
  else
  {
    // error implementation
  }
  return promise.get_future();
}

ara::core::Future<void> KeySlotInterfaceSkeletonImpl::SaveCopy(const CryptoServiceInstanceId& instanceId)
{
  // std::shared_ptr<ara::crypto::common::SkeletonDataStorageContainer> dataStorageContainer
  // = ara::crypto::common::SkeletonDataStorageContainer::GetDataContainerInstance();
  // dataStorageContainer->GetIOInterfaceSkeletonContainer(instanceId);
  static_cast<void>(instanceId);
  return ara::core::Future<void>();
}

ara::core::Future<void> KeySlotInterfaceSkeletonImpl::Clear()
{
  logger_.LogInfo() << "IPC-CORE::KeySlotInterfaceSkeletonImpl : " << __FUNCTION__
                    << "-> Not supported for now";
  return ara::core::Future<void>();
}

ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::IsEmptyOutput>
  KeySlotInterfaceSkeletonImpl::IsEmpty()
{
  ara::core::Future<IsEmptyOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::KeySlotInterfaceSkeletonImpl : " << __FUNCTION__
                    << "-> Not supported for now";
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::MyProviderOutput>
  KeySlotInterfaceSkeletonImpl::MyProvider()
{
  ara::core::Future<MyProviderOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::KeySlotInterfaceSkeletonImpl : " << __FUNCTION__
                    << "-> Not supported for now";
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::keys::keyslot::KeySlotInterface::OpenOutput>
  KeySlotInterfaceSkeletonImpl::Open(const boolean& subscribeForUpdates, const boolean& writeable)
{
  ara::core::Future<OpenOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::KeySlotInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::core::Result<ara::crypto::IOInterface::Uptr> ioInterfaceUptr
    = keySlotUptr_->Open(subscribeForUpdates, writeable);
  if (ioInterfaceUptr.HasValue())
  {
    ara::crypto::ipccom::keys::keyslot::KeySlotInterface::OpenOutput openOutputObj;
    CryptoServiceInstanceId localInstanceId = GenerateCryptoServiceInstanceId();
    logger_.LogInfo() << " IPC-CORE::KeySlotInterfaceSkeletonImpl : " << __FUNCTION__
                      << " Instance Id :" << localInstanceId;
    std::unique_ptr<ara::crypto::common::IOInterfaceSkeletonImpl> localUptr
      = std::make_unique<ara::crypto::common::IOInterfaceSkeletonImpl>(
        localInstanceId, std::move(ioInterfaceUptr).Value());
    localUptr->OfferService();
    ioInterfaceContainer_.emplace(localInstanceId, std::move(localUptr));
    std::shared_ptr<ara::crypto::common::SkeletonDataStorageContainer> dataStorageContainer
      = ara::crypto::common::SkeletonDataStorageContainer::GetDataContainerInstance();
    dataStorageContainer->UpdateIOInterfaceContainer(localInstanceId, std::move(localUptr));
    static_cast<void>(dataStorageContainer);
    openOutputObj.IoInterfaceInstanceId = localInstanceId;
    promise.set_value(openOutputObj);
  }
  return promise.get_future();
}

} // namespace keys
} // namespace crypto
} // namespace ara
