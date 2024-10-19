// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : keyslot.h
//
// Purpose     : This file contains KeySlot interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEYSLOT_PORT_PROTOTYPE_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEYSLOT_PORT_PROTOTYPE_H_

#include "ara/core/result.h"
#include "ara/crypto/common/io_interface.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"

namespace ara
{
namespace crypto
{
namespace keys
{
/// @brief Key slot port-prototype interface.
/// This class enables access to a physicl key-slot
/// @trace SWS_CRYPT_30400
class KeySlot
{
 public:
  /// @brief Destructor.
  /// @trace SWS_CRYPT_30401
  virtual ~KeySlot() noexcept = default;

  /// @brief Copy-assign another KeySlot to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30220
  KeySlot& operator=(const KeySlot& other) = default;

  /// @brief Move-assign another KeySlot to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30221
  KeySlot& operator=(KeySlot&& other) = default;

  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_30402
  using Uptr = std::unique_ptr<KeySlot>;

  /// @brief Retrieve an instance of the CryptoProvider that owns this KeySlot.
  /// Any key slot always has an associated default Crypto Provider that can serve this key slot.
  /// In the simplest case all key slots can be served by a single Crypto Provider installed on the Adaptive
  /// Platform. But in a more complicated case a few different Crypto Providers may coexist in the system,
  /// for example if ECU has one or a few HSMs and software cryptography implementation too, and each of them
  /// has own physical key storage. In such case different dedicated Crypto Providers may serve mentioned
  /// HSMs and the software implementation.
  /// @threadsafety{Thread-safe}
  /// @returns a unique_pointer to the CryptoProvider to be used with this KeySlot
  /// @trace SWS_CRYPT_30403
  //  virtual ara::core::Result<cryp::CryptoProvider::Uptr> MyProvider() const noexcept = 0;

  /// @brief Check the slot for emptiness.
  /// @returns @c true if the slot is empty or @c false otherwise
  /// @trace SWS_CRYPT_30404
  //  virtual bool IsEmpty() const noexcept = 0;

  /// @brief Clear the content of this key-slot.
  /// This method must perform a secure cleanup without the ability to restore the object data!
  /// This method may be used for atomic update of a key slot scoped to some transaction. In such case the
  /// the slot will be updated only after correspondent call of @c CommitTransaction().
  /// @error SecurityErrorDomain::kUnreservedResource  if the target slot is not opened writeable.
  /// @trace SWS_CRYPT_30405
  //  virtual ara::core::Result<void> Clear() noexcept = 0;

  /// @brief Save the content of a provided source IOInterface to this key-slot.
  /// The source container may represent a volatile trusted container or another KeySlot
  /// This method may be used for atomic update of a key slot scoped to some transaction. In such case the
  /// the slot will be updated only after correspondent call of @c CommitTransaction().
  /// @param[in] container  the source IOInterface
  /// @returns @c true if successfully saved
  /// @error SecurityErrorDomain::kIncompatibleObject  if the source object has property "session" or if the
  /// source IOInterface references a KeySlot from a different CryptoProvider
  /// @error SecurityErrorDomain::kEmptyContainer  if the source IOInterface is empty
  /// @error SecurityErrorDomain::kContentRestrictions  if the source object doesn't satisfy
  /// the slot restrictions (including version control)
  /// @error SecurityErrorDomain::kUnreservedResource  if the target slot is not opened writeable.
  /// @trace SWS_CRYPT_30406
  //  virtual ara::core::Result<void> SaveCopy(const IOInterface& container) noexcept = 0;

  /// @brief Get the prototyped properties of the key slot.
  /// @param[out] props  the output buffer for storing the prototype properties of the key slot
  /// @trace SWS_CRYPT_30407
  virtual ara::core::Result<KeySlotPrototypeProps> GetPrototypedProps() const noexcept = 0;

  /// @brief Get an actual properties of a content in the key slot.
  /// If this method called by a "User" Actor then always: <tt>props.exportability == false</tt>.
  /// @param[out] props  the output buffer for storing an actual properties of a content in the key slot
  /// @error SecurityErrorDomain::kEmptyContainer  if the slot is empty
  /// @error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which has no any
  /// ("Owner" or "User") access rights to the key slot
  /// @trace SWS_CRYPT_30408
  virtual ara::core::Result<KeySlotContentProps> GetContentProps() const noexcept = 0;

  /// @brief Open this key slot and return an IOInterface to its content.
  /// If the @c UpdatesObserver interface was provided to the call of @c RegisterObserver() then
  /// the @c UpdatesObserver::OnUpdate() method should be called by %Key Storage engine (in a dedicated
  /// thread) every time when this slot is updated (and become visible for "Users").
  /// Monitoring of the opened key slot will be continued even after destruction of the returned
  /// @c TrustedContainer, because content of the slot may be loaded to volatile memory (as a @c CryptoObject
  /// or to a @c CryptoContext of a crypto primitive), but the @c TrustedContainer may be destroyed after
  /// this. Therefore if you need to terminate monitoring of the key slot then you should directly call
  /// method @c UnsubscribeObserver(SlotNumber).
  /// @param[in] subscribeForUpdates  if this flag is @c true then the @c UpdatesObserver instance
  /// (previously registered by a call of the method @c RegisterObserver()) will be subscribed for updates of
  /// the opened key slot
  /// @param[in] writeable indicates whether the key-slot shall be opened read-only (default) or with write
  /// access
  /// @returns an unique smart pointer to the IOInterface associated with the slot content
  /// @error SecurityErrorDomain::kInvalidUsageOrder  if <tt>(true == subscribeForUpdates)</tt>,
  /// but there is no registered instance of the @c UpdatesObserver in the %Key Storage Provider
  /// context
  /// @error SecurityErrorDomain::kBusyResource if the specified slot is busy because writeable == true but
  /// (a) the keyslot is already opened writable, and/or
  /// (b) the keyslot is in scope of another ongoing transaction
  /// @error SecurityErrorDomain::kModifiedResource if the specified slot has been modified after the KeySlot
  /// has been opened
  /// @trace SWS_CRYPT_30409
  virtual ara::core::Result<IOInterface::Uptr> Open(
    bool subscribeForUpdates = false, bool writeable = false) const noexcept
    = 0;
};

} // namespace keys
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEYSLOT_PORT_PROTOTYPE_H_
