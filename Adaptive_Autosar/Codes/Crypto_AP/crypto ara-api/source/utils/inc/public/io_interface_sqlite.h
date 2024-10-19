// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : io_interface_sqlite.h
//
// Purpose     : This file contains definitions related to IOInterfaceSqlite class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_UTILS_INC_PUBLIC_IO_INTERFACE_SQLITE_H_
#define CRYPTO_SOURCE_UTILS_INC_PUBLIC_IO_INTERFACE_SQLITE_H_

#include <cstring>

#include <ara/log/logging.h>

#include "ara/crypto/common/io_interface.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief Formal interface of an IOInterface is used for saving and loading of security objects.
/// Actual saving and loading should be implemented by internal methods known to a trusted pair of Crypto
/// Provider and Storage Provider.
/// Each object should be uniquely identified by its type and Crypto Object Unique Identifier (@b COUID).
/// This interface suppose that objects in the container are compressed i.e. have a minimal size optimized for
/// @brief SWS_CRYPT_10800
class IOInterfaceSqlite : public IOInterface
{
 public:
  /// @brief Constructor for IOInterfaceSqlite class.
  IOInterfaceSqlite(CryptoObjectUid objectIdVal, ara::core::String uuidVal, std::size_t capacity,
    std::size_t size, AllowedUsageFlags usage, CryptoAlgId algIdVal, CryptoObjectType objectTypeVal,
    CryptoObjectType restrictionsVal, bool volatileVal, bool sessionVal, bool exportableVal,
    bool writableVal) :
    objectId_{ objectIdVal },
    uuid_{ uuidVal }, objectCapcity_{ capacity }, payloadSize_{ size },
    allowedUsage_{ usage }, algId_{ algIdVal }, objectType_{ objectTypeVal },
    restrictions_{ restrictionsVal }, isVolatile_{ volatileVal }, isSession_{ sessionVal },
    isExprotable_{ exportableVal }, isWritable_{ writableVal }
  {
    if (!volatileVal && !sessionVal)
    {
      if (cryptoDbPtr == nullptr)
      {
        int rc = sqlite3_open("/opt/crypto-core/var/keys.db", &cryptoDbPtr);
        if (rc)
        {
          aralog_.LogError() << "Can't open database: " << sqlite3_errmsg(cryptoDbPtr);
        }
        else
        {
          aralog_.LogDebug() << "Opened database successfully";
          GetValueFromDB(objectValue_);
        }
      }
    }
    objectMemRegion_ = objectValue_;
  }

  /// @brief Return @a COUID of an object stored to this IOInterface.
  /// If the container is empty then this method returns @c CryptoObjectType::KUndefined.
  /// Unambiguous identification of a crypto object requires both components: @c CryptoObjectUid and
  /// @c CryptoObjectType.
  /// @param[out] objectUid @a COUID of an object stored in the container
  /// @returns type of the content stored in the container
  /// @trace SWS_CRYPT_10811
  CryptoObjectUid GetObjectId() const noexcept;

  /// @brief Return the CryptoObjectType of the object referenced by this IOInterface.
  /// @returns the CryptoObjectType stored inside the referenced resource
  /// @trace SWS_CRYPT_10812
  CryptoObjectType GetCryptoObjectType() const noexcept;

  /// @brief Return capacity of the underlying resource.
  /// @returns capacity of the underlying buffer of this IOInterface (in bytes)
  /// @trace SWS_CRYPT_10813
  std::size_t GetCapacity() const noexcept;

  /// @brief Return volatility of the the underlying buffer of this IOInterface.
  /// A "session" object can be stored to a "volatile" container only.
  /// A content of a "volatile" container will be destroyed together with the interface instance.
  /// @returns @c true if the container has a volatile nature (i.e. "temporary" or "in RAM") or @c false
  /// otherwise
  /// @trace SWS_CRYPT_10814
  bool IsVolatile() const noexcept;

  /// @brief Return the "session" (or "temporary") attribute of an object as set e.g. by
  /// KeyDerivationFunctionCtx::DeriveKey().
  /// A "session" object can be stored to a VolatileTrustedContainer only!
  /// If this IOInterface is linked to a KeySlot this returns always false.
  /// @returns @c true if the object referenced by this IOInterface has set the "session" attribute
  /// @trace SWS_CRYPT_10815
  bool IsObjectSession() const noexcept;

  /// @brief Return the "exportable" attribute of an object stored to the container.
  /// The exportability of an object doesn't depend from the volatility of its container.
  /// @returns @c true if an object stored to the container has set the "exportable" attribute
  /// @trace SWS_CRYPT_10816
  bool IsObjectExportable() const noexcept;

  /// @brief Return size of an object payload stored in the underlying buffer of this IOInterface.
  /// If the container is empty then this method returns 0.
  /// Returned value does not take into account the object's meta-information properties,
  /// but their size is fixed and common for all crypto objects independently from their actual type.
  /// space for an object's meta-information automatically, according to their implementation details.
  /// @returns size of an object payload stored in the underlying buffer of this IOInterface (in bytes)
  /// @trace SWS_CRYPT_10817
  std::size_t GetPayloadSize() const noexcept;

  /// @brief Return content type restriction of this IOInterface. If
  /// KeySlotPrototypeProps::mAllowContentTypeChange==TRUE, then kUndefined shall be returned.
  /// If a container has a type restriction different from @c CryptoObjectType::kUndefined then only objects
  /// of the mentioned type can be saved to this container.
  /// Volatile containers don't have any content type restrictions.
  /// @returns an object type of allowed content (@c CryptoObjectType::kUndefined means without restriction)
  /// @trace SWS_CRYPT_10818
  CryptoObjectType GetTypeRestriction() const noexcept;

  /// @brief Return actual allowed key/seed usage flags defined by the key slot prototype for this "Actor"
  /// and current content of the container.
  /// Volatile containers don't have any prototyped restrictions, but can have restrictions defined at
  /// run-time for a current instance of object.
  /// A value returned by this method is bitwise AND of the common usage flags defined at run-time and the
  /// usage flags defined by the UserPermissions prototype for current "Actor".
  /// This method is especially useful for empty permanent prototyped containers.
  /// @returns allowed key/seed usage flags
  /// @trace SWS_CRYPT_10819
  AllowedUsageFlags GetAllowedUsage() const noexcept;

  /// @brief Get whether the underlying KeySlot is writable - if this IOInterface is linked to a
  /// VolatileTrustedContainer always return true.
  /// @returns true if the underlying resource can be written
  /// @trace SWS_CRYPT_10821
  bool IsWritable() const noexcept;

  /// @brief Get vendor specific ID of the primitive.
  /// @returns the binary Crypto Primitive ID
  /// @trace SWS_CRYPT_10822
  CryptoAlgId GetPrimitiveId() const noexcept;

  /// @brief Get whether the underlying KeySlot is valid. An IOInterface is invalidated if the underlying
  /// resource has been modified after the IOInterface has been opened.
  /// @returns true if the underlying resource can be valid, false otherwise
  /// @trace SWS_CRYPT_10823
  bool IsValid() const noexcept;

  /// @brief call back for sqlite read operation.
  static int callback(void* data, int argc, char** argv, char** ColName) noexcept
  {
    ara::core::String coloumnName{ ColName[0] };
    char* tempPtr = nullptr;
    for (int index = 0; index < argc; index++)
    {
      tempPtr = argv[index];
    }

    if (coloumnName == "KEYVALUE")
    {
      char* ptr{ reinterpret_cast<char*>(data) };
      size_t index;
      for (index = 0; index < strlen(tempPtr); index++)
      {
        ptr[index] = tempPtr[index];
      }
    }
    else
    {
      size_t* ptr{ reinterpret_cast<size_t*>(data) };
      *ptr = 0;
      for (size_t index = 0; index < strlen(tempPtr); index++)
      {
        *ptr = ((*ptr) * 10) + (tempPtr[index] - 48);
      }
    }
    readComplete = true;
    return 0;
  }

  /// @brief Method to read value from the data base.
  void GetValueFromDB(ara::core::Vector<uint8_t>& dataValue) const noexcept;

  /// @brief Method to read the length of the data in the DB.
  size_t GetDBContentLength() const noexcept;

  /// @brief Getter method for the crypto object value.
  ReadWriteMemRegion GetValue() const noexcept;

  /// @brief Setter method for the crypto object value.
  bool SetObjectValue(ara::core::Vector<uint8_t> value);

  /// @brief Setter method for the crypto object.
  bool SetCryptoObject(CryptoObjectUid id, CryptoObjectType type, CryptoAlgId algoId,
    AllowedUsageFlags usagflags, ara::core::Vector<uint8_t> value);

 private:
  /// @brief variable to store the crypto object id.
  CryptoObjectUid objectId_;

  /// @brief variable to store the uuid in string format.
  ara::core::String uuid_;

  /// @brief variable to store the crypto object capacity.
  std::size_t objectCapcity_;

  /// @brief variable to store the sqlite DB pointer.
  static sqlite3* cryptoDbPtr;

  /// @brief variable to store the crypto object payload size.
  std::size_t payloadSize_;

  /// @brief variable to store the crypto object allowed usage flags.
  AllowedUsageFlags allowedUsage_;

  /// @brief variable to store the crypto object algId.
  CryptoAlgId algId_;

  /// @brief variable to store the crypto object type.
  CryptoObjectType objectType_;

  /// @brief variable to store the crypto object restrictions.
  CryptoObjectType restrictions_;

  /// @brief variable to store the volatile flag.
  bool isVolatile_;

  /// @brief variable to store the session flag.
  bool isSession_;

  /// @brief variable to store the exportable flag.
  bool isExprotable_;

  /// @brief variable to store the writable flag.
  bool isWritable_;

  /// @brief variable to store the flag to indecate completion of read operation.
  static bool readComplete;

  /// @brief variable to store the buffer size of data.
  static size_t buffersize;

  /// @brief variable to store the crypto object value.
  ara::core::Vector<uint8_t> objectValue_;

  /// @brief ReadWriteMemRegion to store the crypto object value.
  ReadWriteMemRegion objectMemRegion_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "IOIS", "IOInterface sqlite", ara::log::LogLevel::kVerbose) };
};

} // namespace crypto
} // namespace ara

#endif // ARA_CRYPTO_COMMON_IO_INTERFACE_SQLITE_H_
