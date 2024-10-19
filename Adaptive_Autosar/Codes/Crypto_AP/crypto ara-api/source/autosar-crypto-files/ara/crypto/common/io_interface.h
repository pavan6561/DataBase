// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : io_interface.h
//
// Purpose     : This file contains definitions for IOInterface class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_IO_INTERFACE_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_IO_INTERFACE_H_

#include <cstdint>

#include <sqlite3.h>

#include "ara/core/string.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"
#include "ara/crypto/common/mem_region.h"

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
class IOInterface
{
 public:
  /// @brief Destructor.
  /// @trace SWS_CRYPT_10810
  virtual ~IOInterface() noexcept = default;

  /// @brief Copy-assign another IOInterface to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30202
  IOInterface& operator=(const IOInterface& other) = default;

  /// @brief Move-assign another IOInterface to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30203
  IOInterface& operator=(IOInterface&& other) = default;

  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_10801
  using Uptr = std::unique_ptr<IOInterface>;

  /// @brief Unique smart pointer of the constant interface.
  /// @trace SWS_CRYPT_10802
  using Uptrc = std::unique_ptr<const IOInterface>;

  /// @brief Return @a COUID of an object stored to this IOInterface.
  /// If the container is empty then this method returns @c CryptoObjectType::KUndefined.
  /// Unambiguous identification of a crypto object requires both components: @c CryptoObjectUid and
  /// @c CryptoObjectType.
  /// @param[out] objectUid @a COUID of an object stored in the container
  /// @returns type of the content stored in the container
  /// @trace SWS_CRYPT_10811
  virtual CryptoObjectUid GetObjectId() const noexcept = 0;

  /// @brief Return the CryptoObjectType of the object referenced by this IOInterface.
  /// @returns the CryptoObjectType stored inside the referenced resource
  /// @trace SWS_CRYPT_10812
  virtual CryptoObjectType GetCryptoObjectType() const noexcept = 0;

  /// @brief Return capacity of the underlying resource.
  /// @returns capacity of the underlying buffer of this IOInterface (in bytes)
  /// @trace SWS_CRYPT_10813
  virtual std::size_t GetCapacity() const noexcept = 0;

  /// @brief Return volatility of the the underlying buffer of this IOInterface.
  /// A "session" object can be stored to a "volatile" container only.
  /// A content of a "volatile" container will be destroyed together with the interface instance.
  /// @returns @c true if the container has a volatile nature (i.e. "temporary" or "in RAM") or @c false
  /// otherwise
  /// @trace SWS_CRYPT_10814
  virtual bool IsVolatile() const noexcept = 0;

  /// @brief Return the "session" (or "temporary") attribute of an object as set e.g. by
  /// KeyDerivationFunctionCtx::DeriveKey().
  /// A "session" object can be stored to a VolatileTrustedContainer only!
  /// If this IOInterface is linked to a KeySlot this returns always false.
  /// @returns @c true if the object referenced by this IOInterface has set the "session" attribute
  /// @trace SWS_CRYPT_10815
  virtual bool IsObjectSession() const noexcept = 0;

  /// @brief Return the "exportable" attribute of an object stored to the container.
  /// The exportability of an object doesn't depend from the volatility of its container.
  /// @returns @c true if an object stored to the container has set the "exportable" attribute
  /// @trace SWS_CRYPT_10816
  virtual bool IsObjectExportable() const noexcept = 0;

  /// @brief Return size of an object payload stored in the underlying buffer of this IOInterface.
  /// If the container is empty then this method returns 0.
  /// Returned value does not take into account the object's meta-information properties,
  /// but their size is fixed and common for all crypto objects independently from their actual type.
  /// space for an object's meta-information automatically, according to their implementation details.
  /// @returns size of an object payload stored in the underlying buffer of this IOInterface (in bytes)
  /// @trace SWS_CRYPT_10817
  virtual std::size_t GetPayloadSize() const noexcept = 0;

  /// @brief Return content type restriction of this IOInterface. If
  /// KeySlotPrototypeProps::mAllowContentTypeChange==TRUE, then kUndefined shall be returned.
  /// If a container has a type restriction different from @c CryptoObjectType::kUndefined then only objects
  /// of the mentioned type can be saved to this container.
  /// Volatile containers don't have any content type restrictions.
  /// @returns an object type of allowed content (@c CryptoObjectType::kUndefined means without restriction)
  /// @trace SWS_CRYPT_10818
  virtual CryptoObjectType GetTypeRestriction() const noexcept = 0;

  /// @brief Return actual allowed key/seed usage flags defined by the key slot prototype for this "Actor"
  /// and current content of the container.
  /// Volatile containers don't have any prototyped restrictions, but can have restrictions defined at run-time
  /// for a current instance of object.
  /// A value returned by this method is bitwise AND of the common usage flags defined at run-time and the
  /// usage flags defined by the UserPermissions prototype for current "Actor".
  /// This method is especially useful for empty permanent prototyped containers.
  /// @returns allowed key/seed usage flags
  /// @trace SWS_CRYPT_10819
  virtual AllowedUsageFlags GetAllowedUsage() const noexcept = 0;

  /// @brief Get whether the underlying KeySlot is writable - if this IOInterface is linked to a
  /// VolatileTrustedContainer always return true.
  /// @returns true if the underlying resource can be written
  /// @trace SWS_CRYPT_10821
  virtual bool IsWritable() const noexcept = 0;

  /// @brief Get vendor specific ID of the primitive.
  /// @returns the binary Crypto Primitive ID
  /// @trace SWS_CRYPT_10822
  virtual CryptoAlgId GetPrimitiveId() const noexcept = 0;

  /// @brief Get whether the underlying KeySlot is valid. An IOInterface is invalidated if the underlying
  /// resource has been modified after the IOInterface has been opened.
  /// @returns true if the underlying resource can be valid, false otherwise
  /// @trace SWS_CRYPT_10823
  virtual bool IsValid() const noexcept = 0;

  /// @brief Get value in the data base.
  /// @return value in DB in the for of Read write memory region.
  virtual ReadWriteMemRegion GetValue() const noexcept = 0;

  /// @brief Set the crypto object value.
  /// @param[in] value which is to be set in crypto obejct.
  /// @return returns true if value is set successfully.
  virtual bool SetObjectValue(ara::core::Vector<uint8_t> value) = 0;

  /// @brief Set the crypto object parameters.
  /// @param[in] Id of the crypto object.
  /// @param[in] type crypto object type.
  /// @param[in] algoId algId for the crypto object.
  /// @param[in] usageflags usage flags for the crypto object.
  /// @param[in] Value which is to be set in crypto obejct.
  /// @return returns true if all parameters are set successfully.
  virtual bool SetCryptoObject(CryptoObjectUid id, CryptoObjectType type, CryptoAlgId algoId,
    AllowedUsageFlags usagflags, ara::core::Vector<uint8_t> value)
    = 0;
};

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_IO_INTERFACE_H_
