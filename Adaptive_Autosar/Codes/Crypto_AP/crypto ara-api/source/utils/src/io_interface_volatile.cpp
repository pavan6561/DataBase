// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : io_interface_volatile.cpp
//
// Purpose     : This file contains implementation for IOInterfaceVolatile class.
// -----------------------------------------------------------------------------------------------------------

#include "io_interface_volatile.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
CryptoObjectUid IOInterfaceVolatile::GetObjectId() const noexcept
{
  return objectId_;
}

CryptoObjectType IOInterfaceVolatile::GetCryptoObjectType() const noexcept
{
  return objectType_;
}

std::size_t IOInterfaceVolatile::GetCapacity() const noexcept
{
  return objectCapcity_;
}

bool IOInterfaceVolatile::IsVolatile() const noexcept
{
  return true;
}

bool IOInterfaceVolatile::IsObjectSession() const noexcept
{
  return isSession_;
}

bool IOInterfaceVolatile::IsObjectExportable() const noexcept
{
  return isExprotable_;
}

std::size_t IOInterfaceVolatile::GetPayloadSize() const noexcept
{
  return payloadSize_;
}

CryptoObjectType IOInterfaceVolatile::GetTypeRestriction() const noexcept
{
  return restrictions_;
}

AllowedUsageFlags IOInterfaceVolatile::GetAllowedUsage() const noexcept
{
  return allowedUsage_;
}

bool IOInterfaceVolatile::IsWritable() const noexcept
{
  return isWritable_;
}

CryptoAlgId IOInterfaceVolatile::GetPrimitiveId() const noexcept
{
  return algId_;
}

ReadWriteMemRegion IOInterfaceVolatile::GetValue() const noexcept
{
  return objectMemRegion_;
}

bool IOInterfaceVolatile::IsValid() const noexcept
{
  return isVolatile_;
}

bool IOInterfaceVolatile::SetCryptoObject(CryptoObjectUid id, CryptoObjectType type, CryptoAlgId algoId,
  AllowedUsageFlags usagflags, ara::core::Vector<uint8_t> value)
{
  if (isVolatile_ && isWritable_ && isSession_)
  {
    objectId_ = id;
    objectType_ = type;
    algId_ = algoId;
    allowedUsage_ = usagflags;
    objectValue_ = value;
    objectMemRegion_ = objectValue_;
    payloadSize_ = value.size();
    return true;
  }
  return false;
}

bool IOInterfaceVolatile::SetObjectValue(ara::core::Vector<uint8_t> value)
{
  if (isWritable_ && isSession_)
  {
    objectValue_ = value;
    objectMemRegion_ = objectValue_;
    return true;
  }
  return false;
}

} // namespace crypto
} // namespace ara