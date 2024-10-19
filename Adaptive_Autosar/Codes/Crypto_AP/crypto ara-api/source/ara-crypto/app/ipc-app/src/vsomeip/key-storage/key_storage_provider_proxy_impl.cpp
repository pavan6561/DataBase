// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_storage_provider_proxy_impl.cpp
//
// Purpose     : Key storage provider proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "key_storage_provider_proxy_impl.h"

namespace ara
{
namespace crypto
{
namespace keys
{
KeyStorageProviderProxyImpl::KeyStorageProviderProxyImpl(
  const CryptoServiceInstanceId serviceInstanceId) noexcept :
  logger_{ ara::log::CreateLogger("KSTR", "Key Storage Provider Proxy", ara::log::LogLevel::kVerbose) }
{
  logger_.LogDebug() << "Initialize the server client connection for Key Storage Provider";
  Initialize(serviceInstanceId);
}

void KeyStorageProviderProxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceId)
{
  if (vsomeipInstance_.CreateInstance(serviceInstanceId) == true)
  {
    logger_.LogDebug() << "Server Client connection for Key Storage Provider : Successfull";
    keyStorageProviderHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for Key Storage Provider : Failed";
  }
}

ara::core::Result<KeySlot::Uptr> KeyStorageProviderProxyImpl::LoadKeySlot(
  ara::core::String& iSpecify) noexcept
{
  KeySlot::Uptr keySlotUptr;
  auto keyStorageResult = keyStorageProviderHandler_->LoadKeySlot(iSpecify).GetResult();
  if (keyStorageResult.HasValue())
  {
    CryptoServiceInstanceId keyStorageInstanceId = keyStorageResult.Value().keyStorageInstanceId;
    logger_.LogDebug() << "Key Storge Provider Instance Id  : " << keyStorageInstanceId;
    keySlotUptr = std::make_unique<KeySlotProxyImpl>(keyStorageInstanceId, keyStorageProviderHandler_);
  }
  return ara::core::Result<KeySlot::Uptr>(std::move(keySlotUptr));
}

} // namespace keys
} // namespace crypto
} // namespace ara