// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_storage_provider_impl.cpp
//
// Purpose     : This file contains implementation for KeyStorageProviderImpl class.
// -----------------------------------------------------------------------------------------------------------

// NOTE:- This file is out of scope for MS5-IR1 plese do not consider this file for review or static analysis.

#include "key_storage_provider_impl.h"

namespace ara
{
namespace crypto
{
namespace keys
{
ara::core::Result<KeySlot::Uptr> KeyStorageProviderImpl::LoadKeySlot(ara::core::String& iSpecify) noexcept
{
  return ara::core::Result<KeySlot::Uptr>::FromValue(std::make_unique<KeySlotImpl>(iSpecify));
}

} // namespace keys
} // namespace crypto
} // namespace ara
