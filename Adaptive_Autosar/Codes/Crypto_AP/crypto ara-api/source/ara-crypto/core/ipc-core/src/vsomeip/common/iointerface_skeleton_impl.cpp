// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : iointerface_skeleton_impl.cpp
//
// Purpose     : IO-interface skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#include "iointerface_skeleton_impl.h"

namespace ara
{
namespace crypto
{
namespace common
{
IOInterfaceSkeletonImpl::IOInterfaceSkeletonImpl(
  CryptoServiceInstanceId const instanceId, ara::crypto::IOInterface::Uptr ioUtr) noexcept :
  ara::crypto::ipccom::common::iointerface::skeleton::IOInterfaceSkeleton(
    ara::com::InstanceIdentifier(instanceId)),
  logger_{ ara::log::CreateLogger("IOIN", "IO Interface", ara::log::LogLevel::kVerbose) },
  instanceId_{ instanceId }, ioInterfaceUptr_{ std::move(ioUtr) }
{
  logger_.LogDebug() << "IOInterface Skeleton constructor";
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetAllowedUsageOutput>
  IOInterfaceSkeletonImpl::GetAllowedUsage()
{
  ara::core::Future<GetAllowedUsageOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::GetAllowedUsageOutput objOutput;
  ara::crypto::AllowedUsageFlags allowUsage = ioInterfaceUptr_->GetAllowedUsage();
  objOutput.AllowedUsageFlags = allowUsage;
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetCapacityOutput>
  IOInterfaceSkeletonImpl::GetCapacity()
{
  ara::core::Future<GetCapacityOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::GetCapacityOutput objOutput;
  std::size_t capacity = ioInterfaceUptr_->GetCapacity();
  objOutput.Size = capacity;
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetCryptoObjectTypeOutput>
  IOInterfaceSkeletonImpl::GetCryptoObjectType()
{
  ara::core::Future<GetCryptoObjectTypeOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::GetCryptoObjectTypeOutput objOutput;
  ara::crypto::CryptoObjectType objectType = ioInterfaceUptr_->GetCryptoObjectType();
  if (objectType == ara::crypto::CryptoObjectType::kUndefined)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::Undefined;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kSymmetricKey)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::SymmetricKey;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kSecretSeed)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::SecretSeed;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kSignature)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::Signature;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kPublicKey)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::PublicKey;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kPrivateKey)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::PrivateKey;
  }
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetObjectIdOutput>
  IOInterfaceSkeletonImpl::GetObjectId()
{
  ara::core::Future<GetObjectIdOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::GetObjectIdOutput objOutput;
  ara::crypto::CryptoObjectUid crypObjUid = ioInterfaceUptr_->GetObjectId();
  objOutput.CryptoObjectUid.mVersionStamp = crypObjUid.mVersionStamp;
  objOutput.CryptoObjectUid.mGeneratorUid.mQwordMs = crypObjUid.mGeneratorUid.mQwordMs;
  objOutput.CryptoObjectUid.mGeneratorUid.mQwordLs = crypObjUid.mGeneratorUid.mQwordLs;
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetPayloadSizeOutput>
  IOInterfaceSkeletonImpl::GetPayloadSize()
{
  ara::core::Future<GetPayloadSizeOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::GetPayloadSizeOutput objOutput;
  std::size_t payloadSize = ioInterfaceUptr_->GetPayloadSize();
  objOutput.PayloadSize = payloadSize;
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetPrimitiveIdOutput>
  IOInterfaceSkeletonImpl::GetPrimitiveId()
{
  ara::core::Future<GetPrimitiveIdOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::GetPrimitiveIdOutput objOutput;
  ara::crypto::CryptoAlgId algId = ioInterfaceUptr_->GetPrimitiveId();
  objOutput.cryptoAlgId = algId;
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetTypeRestrictionOutput>
  IOInterfaceSkeletonImpl::GetTypeRestriction()
{
  ara::core::Future<GetTypeRestrictionOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::GetTypeRestrictionOutput objOutput;
  ara::crypto::CryptoObjectType objectType = ioInterfaceUptr_->GetTypeRestriction();
  if (objectType == ara::crypto::CryptoObjectType::kUndefined)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::Undefined;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kSymmetricKey)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::SymmetricKey;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kSecretSeed)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::SecretSeed;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kSignature)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::Signature;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kPublicKey)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::PublicKey;
  }
  else if (objectType == ara::crypto::CryptoObjectType::kPrivateKey)
  {
    objOutput.CryptoObjectType = ObjectTypeIPC::PrivateKey;
  }
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsObjectExportableOutput>
  IOInterfaceSkeletonImpl::IsObjectExportable()
{
  ara::core::Future<IsObjectExportableOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::IsObjectExportableOutput objOutput;
  bool isObjExportable = ioInterfaceUptr_->IsObjectExportable();
  objOutput.Bool = isObjExportable;
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsObjectSessionOutput>
  IOInterfaceSkeletonImpl::IsObjectSession()
{
  ara::core::Future<IsObjectSessionOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::IsObjectSessionOutput objOutput;
  bool isObjectSession = ioInterfaceUptr_->IsObjectSession();
  objOutput.Bool = isObjectSession;
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsValidOutput>
  IOInterfaceSkeletonImpl::IsValid()
{
  ara::core::Future<IsValidOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::IsValidOutput objOutput;
  bool isValid = ioInterfaceUptr_->IsValid();
  objOutput.Bool = isValid;
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsVolatileOutput>
  IOInterfaceSkeletonImpl::IsVolatile()
{
  ara::core::Future<IsVolatileOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::IsVolatileOutput objOutput;
  bool isVolatile = ioInterfaceUptr_->IsVolatile();
  objOutput.Bool = isVolatile;
  promise.set_value(objOutput);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsWritableOutput>
  IOInterfaceSkeletonImpl::IsWritable()
{
  ara::core::Future<IsWritableOutput>::PromiseType promise;
  logger_.LogInfo() << "IPC-CORE::IOInterfaceSkeletonImpl : " << __FUNCTION__;
  ara::crypto::ipccom::common::iointerface::IOInterface::IsWritableOutput objOutput;
  bool isWritable = ioInterfaceUptr_->IsWritable();
  objOutput.Bool = isWritable;
  promise.set_value(objOutput);
  return promise.get_future();
}

} // namespace common
} // namespace crypto
} // namespace ara