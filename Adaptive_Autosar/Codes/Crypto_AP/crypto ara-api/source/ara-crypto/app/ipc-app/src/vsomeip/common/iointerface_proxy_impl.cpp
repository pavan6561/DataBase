// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : iointerface_proxy_impl.cpp
//
// Purpose     : IO-Interface proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "iointerface_proxy_impl.h"

namespace ara
{
namespace crypto
{
namespace common
{
IoInterfaceProxyImpl::IoInterfaceProxyImpl(CryptoServiceInstanceId const serviceInstanceID) noexcept :
  serviceInstanceID_{ serviceInstanceID }, logger_{ ara::log::CreateLogger(
                                             "IOIN", "IO Interface Proxy", ara::log::LogLevel::kVerbose) }
{
  logger_.LogDebug() << " Initializing IO Interface Proxy Impl ";
  if (Initialize(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server-Client connection for IoInterfaceProxyImpl -> SUCCESS";
  }
  else
  {
    logger_.LogDebug() << "Server-Client connection for IoInterfaceProxyImpl -> FAIL";
  }
}

bool IoInterfaceProxyImpl::Initialize(CryptoServiceInstanceId const serviceInstanceID)
{
  bool isSuccess;
  if (vsomeipIOInterfaceInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server-Client connection for KeySlotProxyImpl Function -> SUCCESS";
    ioInterfaceProxyHandler_ = vsomeipIOInterfaceInstance_.GetHandlerInstance();
    isSuccess = true;
  }
  else
  {
    logger_.LogError() << "Server-Client connection for KeySlotProxyImpl Function -> FAILED";
    isSuccess = false;
  }
  return isSuccess;
}

ara::crypto::CryptoObjectUid IoInterfaceProxyImpl::GetObjectId() const noexcept
{
  ara::crypto::CryptoObjectUid cryptoObjuid;
  auto objResult = ioInterfaceProxyHandler_->GetObjectId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    cryptoObjuid.mVersionStamp = objResult.Value().CryptoObjectUid.mVersionStamp;
    cryptoObjuid.mGeneratorUid.mQwordLs = objResult.Value().CryptoObjectUid.mGeneratorUid.mQwordLs;
    cryptoObjuid.mGeneratorUid.mQwordMs = objResult.Value().CryptoObjectUid.mGeneratorUid.mQwordMs;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return cryptoObjuid;
}

ara::crypto::CryptoObjectType IoInterfaceProxyImpl::GetCryptoObjectType() const noexcept
{
  ara::crypto::CryptoObjectType cryptObjType{ ara::crypto::CryptoObjectType::kUndefined };
  auto objResult = ioInterfaceProxyHandler_->GetCryptoObjectType().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    ObjectTypeIPC objType = objResult.Value().CryptoObjectType;
    if (objType == ObjectTypeIPC::Undefined)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kUndefined;
    }
    else if (objType == ObjectTypeIPC::SecretSeed)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kSecretSeed;
    }
    else if (objType == ObjectTypeIPC::Signature)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kSignature;
    }
    else if (objType == ObjectTypeIPC::SymmetricKey)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kSymmetricKey;
    }
    else if (objType == ObjectTypeIPC::PrivateKey)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kPrivateKey;
    }
    else if (objType == ObjectTypeIPC::PublicKey)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kPublicKey;
    }
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return cryptObjType;
}

std::size_t IoInterfaceProxyImpl::GetCapacity() const noexcept
{
  std::size_t data{ 0 };
  auto objResult = ioInterfaceProxyHandler_->GetCapacity().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    data = objResult.Value().Size;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return data;
}

bool IoInterfaceProxyImpl::IsVolatile() const noexcept
{
  bool isVolatile{ false };
  auto objResult = ioInterfaceProxyHandler_->IsVolatile().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    isVolatile = objResult.Value().Bool;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return isVolatile;
}

bool IoInterfaceProxyImpl::IsObjectSession() const noexcept
{
  bool isObjectSession{ false };
  auto objResult = ioInterfaceProxyHandler_->IsObjectSession().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    isObjectSession = objResult.Value().Bool;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return isObjectSession;
}

bool IoInterfaceProxyImpl::IsObjectExportable() const noexcept
{
  bool isObjectExport{ false };
  auto objResult = ioInterfaceProxyHandler_->IsObjectExportable().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    isObjectExport = objResult.Value().Bool;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return isObjectExport;
}

std::size_t IoInterfaceProxyImpl::GetPayloadSize() const noexcept
{
  std::size_t data{ 0 };
  auto objResult = ioInterfaceProxyHandler_->GetPayloadSize().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    data = objResult.Value().PayloadSize;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return data;
}

ara::crypto::CryptoObjectType IoInterfaceProxyImpl::GetTypeRestriction() const noexcept
{
  ara::crypto::CryptoObjectType cryptObjType{ ara::crypto::CryptoObjectType::kUndefined };
  auto objResult = ioInterfaceProxyHandler_->GetTypeRestriction().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    ObjectTypeIPC objType = objResult.Value().CryptoObjectType;
    if (objType == ObjectTypeIPC::Undefined)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kUndefined;
    }
    else if (objType == ObjectTypeIPC::SecretSeed)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kSecretSeed;
    }
    else if (objType == ObjectTypeIPC::Signature)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kSignature;
    }
    else if (objType == ObjectTypeIPC::SymmetricKey)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kSymmetricKey;
    }
    else if (objType == ObjectTypeIPC::PrivateKey)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kPrivateKey;
    }
    else if (objType == ObjectTypeIPC::PublicKey)
    {
      cryptObjType = ara::crypto::CryptoObjectType::kPublicKey;
    }
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return cryptObjType;
}

ara::crypto::AllowedUsageFlags IoInterfaceProxyImpl::GetAllowedUsage() const noexcept
{
  ara::crypto::AllowedUsageFlags flag{ 0 };
  auto objResult = ioInterfaceProxyHandler_->GetAllowedUsage().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    flag = objResult.Value().AllowedUsageFlags;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return flag;
}

bool IoInterfaceProxyImpl::IsWritable() const noexcept
{
  bool isWritable{ false };
  auto objResult = ioInterfaceProxyHandler_->IsWritable().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    isWritable = objResult.Value().Bool;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return isWritable;
}

ara::crypto::CryptoAlgId IoInterfaceProxyImpl::GetPrimitiveId() const noexcept
{
  ara::crypto::CryptoAlgId id{ 0 };
  auto objResult = ioInterfaceProxyHandler_->GetPrimitiveId().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    //id = objResult.Value().cryptoAlgId;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return id;
}

bool IoInterfaceProxyImpl::IsValid() const noexcept
{
  bool isValid{ false };
  auto objResult = ioInterfaceProxyHandler_->IsValid().GetResult();
  if (objResult.HasValue())
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Success";
    isValid = objResult.Value().Bool;
  }
  else
  {
    logger_.LogDebug() << "IPC-APP::IoInterfaceProxyImpl " << __FUNCTION__ << " : Failed";
  }
  return isValid;
}

} // namespace common
} // namespace crypto
} // namespace ara