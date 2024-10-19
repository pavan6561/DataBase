// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_slot_proxy_impl.cpp
//
// Purpose     : Key slot proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "key_slot_proxy_impl.h"

#include "key_storage_provider_proxy_impl.h"

namespace ara
{
namespace crypto
{
namespace keys
{
KeySlotProxyImpl::KeySlotProxyImpl(const CryptoServiceInstanceId serviceInstanceID,
  std::shared_ptr<ara::crypto::ipccom::keys::keystorageprovider::proxy::KeyStorageProviderProxy>
    keyStorageProviderHandler_) noexcept :
  logger_{ ara::log::CreateLogger("KSLT", "Key Slot Proxy", ara::log::LogLevel::kVerbose) },
  keySlotKeyStorageProviderHandler_{ keyStorageProviderHandler_ }
{
  logger_.LogDebug() << " Initializing Key Slot Proxy Impl ";
  if (Initialize(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server-Client connection for KeySlotProxyImpl -> SUCCESS";
  }
  else
  {
    logger_.LogDebug() << "Server-Client connection for KeySlotProxyImpl -> FAIL";
  }
}

bool KeySlotProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipKeySlotInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server-Client connection for KeySlotProxyImpl Function -> SUCCESS";
    keySlotProxyHandler_ = vsomeipKeySlotInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server-Client connection for KeySlotProxyImpl Function -> FAILED";
    isSuccess = false;
  }
  return isSuccess;
}

ara::core::Result<KeySlotPrototypeProps> KeySlotProxyImpl::GetPrototypedProps() const noexcept
{
  KeySlotPrototypeProps keySlotPrototypeObj;
  auto keySlotResult = keySlotProxyHandler_->GetPrototypedProps().GetResult();
  if (keySlotResult.HasValue())
  {
    logger_.LogDebug() << "IPC::APP KeySlotProxyImpl for : " << __FUNCTION__;
    auto keySlotPrototypeOutputObj = keySlotResult.Value().KeySlotPrototypeProps;
    keySlotPrototypeObj.mAlgId = keySlotPrototypeOutputObj.mAlgId;
    keySlotPrototypeObj.mAllocateSpareSlot = keySlotPrototypeOutputObj.mAllocateSpareSlot;
    keySlotPrototypeObj.mAllowContentTypeChange = keySlotPrototypeOutputObj.mAllocateSpareSlot;
    keySlotPrototypeObj.mExportAllowed = keySlotPrototypeOutputObj.mExportAllowed;
    keySlotPrototypeObj.mMaxUpdateAllowed = keySlotPrototypeOutputObj.mMaxUpdateAllowed;
    keySlotPrototypeObj.mSlotCapacity = keySlotPrototypeOutputObj.mSlotCapacity;
    ObjectTypeIPC objType = keySlotPrototypeOutputObj.mObjectType;
    if (objType == ObjectTypeIPC::Undefined)
    {
      keySlotPrototypeObj.mObjectType = ara::crypto::CryptoObjectType::kUndefined;
    }
    else if (objType == ObjectTypeIPC::SecretSeed)
    {
      keySlotPrototypeObj.mObjectType = ara::crypto::CryptoObjectType::kSecretSeed;
    }
    else if (objType == ObjectTypeIPC::Signature)
    {
      keySlotPrototypeObj.mObjectType = ara::crypto::CryptoObjectType::kSignature;
    }
    else if (objType == ObjectTypeIPC::SymmetricKey)
    {
      keySlotPrototypeObj.mObjectType = ara::crypto::CryptoObjectType::kSymmetricKey;
    }
    else if (objType == ObjectTypeIPC::PrivateKey)
    {
      keySlotPrototypeObj.mObjectType = ara::crypto::CryptoObjectType::kPrivateKey;
    }
    else if (objType == ObjectTypeIPC::PublicKey)
    {
      keySlotPrototypeObj.mObjectType = ara::crypto::CryptoObjectType::kPublicKey;
    }
    SlotTypeProps slotType = keySlotPrototypeOutputObj.mSlotType;
    if (slotType == SlotTypeProps::Application)
    {
      keySlotPrototypeObj.mSlotType = ara::crypto::KeySlotType::kApplication;
    }
    else if (slotType == SlotTypeProps::Machine)
    {
      keySlotPrototypeObj.mSlotType = ara::crypto::KeySlotType::kMachine;
    }
  }
  else
  {
    // error implementation
  }
  return ara::core::Result<KeySlotPrototypeProps>(keySlotPrototypeObj);
}

ara::core::Result<KeySlotContentProps> KeySlotProxyImpl::GetContentProps() const noexcept
{
  KeySlotContentProps keySlotContentObj;
  auto keySlotContentResult = keySlotProxyHandler_->GetContentProps().GetResult();
  if (keySlotContentResult.HasValue())
  {
    logger_.LogDebug() << "IPC::APP KeySlotProxyImpl for : " << __FUNCTION__;
    auto keySlotContentPrototypeOutputObj = keySlotContentResult.Value().KeySlotContentProps;
    keySlotContentObj.mAlgId = keySlotContentPrototypeOutputObj.mAlgId;
    keySlotContentObj.mContentAllowedUsage = keySlotContentPrototypeOutputObj.mContentAllowedUsage;
    keySlotContentObj.mObjectSize = keySlotContentPrototypeOutputObj.mObjectSize;
    keySlotContentObj.mObjectUid.mGeneratorUid.mQwordLs
      = keySlotContentPrototypeOutputObj.mObjectUid.mGeneratorUid.mQwordLs;
    keySlotContentObj.mObjectUid.mGeneratorUid.mQwordMs
      = keySlotContentPrototypeOutputObj.mObjectUid.mGeneratorUid.mQwordMs;
    keySlotContentObj.mObjectUid.mVersionStamp = keySlotContentPrototypeOutputObj.mObjectUid.mVersionStamp;
    ObjectTypeIPC objType = keySlotContentPrototypeOutputObj.mObjectType;
    if (objType == ObjectTypeIPC::Undefined)
    {
      keySlotContentObj.mObjectType = ara::crypto::CryptoObjectType::kUndefined;
    }
    else if (objType == ObjectTypeIPC::SecretSeed)
    {
      keySlotContentObj.mObjectType = ara::crypto::CryptoObjectType::kSecretSeed;
    }
    else if (objType == ObjectTypeIPC::Signature)
    {
      keySlotContentObj.mObjectType = ara::crypto::CryptoObjectType::kSignature;
    }
    else if (objType == ObjectTypeIPC::SymmetricKey)
    {
      keySlotContentObj.mObjectType = ara::crypto::CryptoObjectType::kSymmetricKey;
    }
    else if (objType == ObjectTypeIPC::PrivateKey)
    {
      keySlotContentObj.mObjectType = ara::crypto::CryptoObjectType::kPrivateKey;
    }
    else if (objType == ObjectTypeIPC::PublicKey)
    {
      keySlotContentObj.mObjectType = ara::crypto::CryptoObjectType::kPublicKey;
    }
  }
  else
  {
    // error implementation
  }
  return ara::core::Result<KeySlotContentProps>(keySlotContentObj);
}

ara::core::Result<ara::crypto::IOInterface::Uptr> KeySlotProxyImpl::Open(
  bool subscribeForUpdates, bool writeable) const noexcept
{
  std::unique_ptr<ara::crypto::common::IoInterfaceProxyImpl> ioInterfaceUptr;
  //ara::crypto::IOInterface::Uptr ioInterfaceUptr{nullptr};
  logger_.LogInfo() << "IPC::APP KeySlotProxyImpl : " << __FUNCTION__;
  auto ioInterfaceResult = keySlotProxyHandler_->Open(subscribeForUpdates, writeable).GetResult();
  if (ioInterfaceResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = ioInterfaceResult.Value().IoInterfaceInstanceId;
    logger_.LogDebug() << "IPC::APP Service InstanceId for " << __FUNCTION__ << "KeySlot : " << instanceId;
    ioInterfaceUptr = std::make_unique<ara::crypto::common::IoInterfaceProxyImpl>(instanceId);
    //std::shared_ptr<ara::crypto::common::ProxyDataStorageContainer> ioInterfaceObjStore_
    // = ara::crypto::common::ProxyDataStorageContainer::GetIOInterfaceContainer(std::move(ioInterfaceUptr),
    // instanceId);
  }
  return ara::core::Result<ara::crypto::IOInterface::Uptr>(std::move(ioInterfaceUptr));
}

ara::core::Result<cryp::CryptoProvider::Uptr> KeySlotProxyImpl::MyProvider() const noexcept
{
  ara::crypto::cryp::CryptoProvider::Uptr crypUptr{ nullptr };
  logger_.LogInfo() << "IPC-APP::KeySlotProxyImpl : " << __FUNCTION__ << "-> Not supported for now";
  return ara::core::Result<cryp::CryptoProvider::Uptr>(std::move(crypUptr));
}

bool KeySlotProxyImpl::IsEmpty() const noexcept
{
  logger_.LogInfo() << "IPC-APP::KeySlotProxyImpl : " << __FUNCTION__ << "-> Not supported for now";
  return false;
}

ara::core::Result<void> KeySlotProxyImpl::Clear() noexcept
{
  logger_.LogInfo() << "IPC-APP::KeySlotProxyImpl : " << __FUNCTION__ << "-> Not supported for now";
  return ara::core::Result<void>();
}

ara::core::Result<void> KeySlotProxyImpl::SaveCopy(const IOInterface& container) noexcept
{
  const ara::crypto::common::IoInterfaceProxyImpl* obj
    = dynamic_cast<const ara::crypto::common::IoInterfaceProxyImpl*>(&container);
  CryptoServiceInstanceId const serviceInstanceId = obj->GetIOInterfaceInstanceId();
  auto saveCopyResult = keySlotProxyHandler_->SaveCopy(serviceInstanceId).GetResult();
  if (saveCopyResult.HasValue())
  {
    logger_.LogInfo() << " IPC-APP KeySlotProxyImpl : " << __FUNCTION__
                      << " IOInterface Instance Id : " << serviceInstanceId;
  }
  else
  {
  }
  logger_.LogInfo() << "IPC-APP::KeySlotProxyImpl : " << __FUNCTION__ << "-> Not supported for now";
  return ara::core::Result<void>();
}
} // namespace keys
} // namespace crypto
} // namespace ara