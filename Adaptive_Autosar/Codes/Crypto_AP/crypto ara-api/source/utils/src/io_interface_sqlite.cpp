// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : io_interface_sqlite.cpp
//
// Purpose     : This file contains implementation for IOInterfaceSqlite class.
// -----------------------------------------------------------------------------------------------------------

#include "io_interface_sqlite.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
sqlite3* IOInterfaceSqlite::cryptoDbPtr{ nullptr };
bool IOInterfaceSqlite::readComplete{ false };
size_t IOInterfaceSqlite::buffersize{ false };

CryptoObjectUid IOInterfaceSqlite::GetObjectId() const noexcept
{
  return objectId_;
}

CryptoObjectType IOInterfaceSqlite::GetCryptoObjectType() const noexcept
{
  return objectType_;
}

std::size_t IOInterfaceSqlite::GetCapacity() const noexcept
{
  return objectCapcity_;
}

bool IOInterfaceSqlite::IsVolatile() const noexcept
{
  return isVolatile_;
}

bool IOInterfaceSqlite::IsObjectSession() const noexcept
{
  return isSession_;
}

bool IOInterfaceSqlite::IsObjectExportable() const noexcept
{
  return isExprotable_;
}

std::size_t IOInterfaceSqlite::GetPayloadSize() const noexcept
{
  return payloadSize_;
}

CryptoObjectType IOInterfaceSqlite::GetTypeRestriction() const noexcept
{
  return restrictions_;
}

AllowedUsageFlags IOInterfaceSqlite::GetAllowedUsage() const noexcept
{
  return allowedUsage_;
}

bool IOInterfaceSqlite::IsWritable() const noexcept
{
  return isWritable_;
}

CryptoAlgId IOInterfaceSqlite::GetPrimitiveId() const noexcept
{
  return algId_;
}

ReadWriteMemRegion IOInterfaceSqlite::GetValue() const noexcept
{
  return objectMemRegion_;
}

bool IOInterfaceSqlite::IsValid() const noexcept
{
  if (!isSession_ && !isVolatile_)
  {
    ara::core::Vector<uint8_t> objectValueInStorage;

    GetValueFromDB(objectValueInStorage);

    if (objectValueInStorage == objectValue_)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return true;
  }
}

bool IOInterfaceSqlite::SetCryptoObject(CryptoObjectUid id, CryptoObjectType type, CryptoAlgId algoId,
  AllowedUsageFlags usagflags, ara::core::Vector<uint8_t> value)
{
  if (isWritable_ && isSession_ && isVolatile_)
  {
    objectId_ = id;
    objectType_ = type;
    algId_ = algoId;
    allowedUsage_ = usagflags;
    objectValue_ = value;
    objectMemRegion_ = objectValue_;
    return true;
  }
  return false;
}

bool IOInterfaceSqlite::SetObjectValue(ara::core::Vector<uint8_t> value)
{
  if (isWritable_)
  {
    objectValue_ = value;

    objectMemRegion_ = objectValue_;
    if (!isSession_ && !isVolatile_)
    {
      aralog_.LogDebug() << "Writing data";
      ara::core::String command{ "UPDATE " };
      ara::core::String subCommand1{ " SET KEYVALUE = " };
      ara::core::String subCommand2{ "WHERE UUID = '" };
      ara::core::String argumentEnd{ "';" };
      ara::core::String keyData{ "'" };
      ara::core::String keyEnd{ "' " };
      char* errorMsg;

      for (size_t index = 0; index < objectValue_.size(); index++)
      {
        keyData.push_back(objectValue_[index]);
      }

      ara::core::String sqlquery(
        command + "cryptoobjects" + subCommand1 + keyData + keyEnd + subCommand2 + uuid_ + argumentEnd);

      int rc = sqlite3_exec(cryptoDbPtr, sqlquery.data(), NULL, 0, &errorMsg);

      if (rc)
      {
        aralog_.LogError() << "Can't update " << errorMsg;
      }
    }
    return true;
  }
  return false;
}

void IOInterfaceSqlite::GetValueFromDB(ara::core::Vector<uint8_t>& dataValue) const noexcept
{
  ara::core::String command{ "SELECT KEYVALUE FROM cryptoobjects WHERE UUID = '" };
  ara::core::String argumentEnd{ "';" };
  char* errorMsg;
  size_t length = GetDBContentLength();

  ara::core::String sqlquery(command + uuid_ + argumentEnd);

  readComplete = false;

  dataValue.clear();

  dataValue.resize(length);

  buffersize = length;

  int rc = sqlite3_exec(cryptoDbPtr, sqlquery.data(), this->callback, dataValue.data(), &errorMsg);

  if (rc)
  {
    aralog_.LogError() << "Can't select Value " << errorMsg;
  }

  while (!readComplete)
  {
  }
}

size_t IOInterfaceSqlite::GetDBContentLength() const noexcept
{
  ara::core::String command{ "SELECT LENGTH(KEYVALUE) FROM cryptoobjects WHERE UUID = '" };
  ara::core::String argumentEnd{ "';" };
  char* errorMsg;
  size_t length;

  ara::core::String sqlquery(command + uuid_ + argumentEnd);

  readComplete = false;

  int rc = sqlite3_exec(cryptoDbPtr, sqlquery.data(), this->callback, &length, &errorMsg);

  if (rc)
  {
    aralog_.LogError() << "Can't select Value " << errorMsg;
  }

  while (!readComplete)
  {
  }
  return length;
}

} // namespace crypto
} // namespace ara