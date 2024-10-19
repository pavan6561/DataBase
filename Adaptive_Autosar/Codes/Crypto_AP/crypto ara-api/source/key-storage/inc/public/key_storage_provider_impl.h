// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_storage_provider_impl.h
//
// Purpose     : This file provides KeyStorageProviderImpl class declaration
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_KEY_STORAGE_INC_PUBLIC_KEY_STORAGE_PROVIDER_IMPL_H_
#define CRYPTO_SOURCE_KEY_STORAGE_INC_PUBLIC_KEY_STORAGE_PROVIDER_IMPL_H_

// NOTE:- This file is out of scope for MS5-IR1 plese do not consider this file for review or static analysis.

#include "ara/crypto/keys/key_storage_provider.h"
#include "keyslot_impl.h"

namespace ara
{
namespace crypto
{
namespace keys
{
/// @brief Key Storage Provider interface.
/// Any object is uniquely identified by the combination of its UUID and type.
/// HSMs/TPMs implementing the concept of "non-extractable keys" should use own copies of externally supplied
/// crypto objects.
/// A few software Crypto Providers can share single key slot if they support same format.
/// @trace SWS_CRYPT_30100
class KeyStorageProviderImpl : public KeyStorageProvider
{
 public:
  /// @brief Load a key slot.
  /// The functions loads the information associated with a KeySlot into a KeySlot object.
  /// @param[in] iSpecify  the target key-slot instance specifier
  /// @returns an unique smart pointer to allocated key slot
  /// @error SecurityErrorDomain::kUnreservedResource  if the InstanceSpecifier is incorrect (the slot is not
  /// allocated)
  /// @trace SWS_CRYPT_30115
  ara::core::Result<KeySlot::Uptr> LoadKeySlot(ara::core::String& iSpecify) noexcept;

  /// @brief Begin new transaction for key slots update.
  /// In order for a keyslot to be part of a transaction scope, the reserveSpareSlot model parameter of the
  /// keyslot has to be set to true.
  /// A transaction is dedicated for updating related key slots simultaneously (in an atomic, all-or-nothing,
  /// way). All key slots that should be updated by the transaction have to be opened and provided to this
  /// function. Any changes to the slots in scope are executed by calling commit().
  /// @param[in] targetSlots  a list of KeySlots that should be updated during this transaction.
  /// @returns a unique ID assigned to this transaction
  /// @error SecurityErrorDomain::kUnreservedResource if @c targetSlots list has a slot that has not been
  /// configured with the reserveSpareSlot parameter in the manifest
  /// @error SecurityErrorDomain::kBusyResource  if @c targetSlots list has key slots that are already
  /// involved to another pending transaction or opened in writing mode
  /// @trace SWS_CRYPT_30123
  //ara::core::Result<TransactionId> BeginTransaction(const TransactionScope& targetSlots)
  //    noexcept;

  /// @brief Commit changes of the transaction to Key Storage.
  /// Any changes of key slots made during a transaction are invisible up to the commit execution.
  /// The commit command permanently saves all changes made during the transaction in Key Storage
  /// @param[in] id  an ID of a transaction that should be commited
  /// @error SecurityErrorDomain::kInvalidArgument  if provided @c id is invalid, i.e. this ID is unknown
  /// or correspondent transaction already was finished (commited or rolled back)
  /// @trace SWS_CRYPT_30124
  //ara::core::Result<void> CommitTransaction(TransactionId id) noexcept;

  /// @brief Rollback all changes executed during the transaction in Key Storage.
  /// The rollback command permanently cancels all changes made during the transaction in Key Storage.
  /// A rolled back transaction is completely invisible for all applications.
  /// @param[in] id  an ID of a transaction that should be rolled back
  /// @error SecurityErrorDomain::kInvalidArgument  if provided @c id is invalid, i.e. this ID is unknown
  /// or correspondent transaction already was finished (commited or rolled back)
  /// @trace SWS_CRYPT_30125
  //ara::core::Result<void> RollbackTransaction(TransactionId id) noexcept;

  /// @brief Unsubscribe the Update Observer from changes monitoring of the specified slot.
  /// @param[in] slot  number of a slot that should be unsubscribed from the updates observing
  /// @error SecurityErrorDomain::kInvalidArgument  if the specified slot is not monitored now (i.e. if it
  /// was not successfully opened via @c OpenAsUser() or it was already unsubscribed by this method)
  /// @trace SWS_CRYPT_30126
  //ara::core::Result<void> UnsubscribeObserver(KeySlot& slot) noexcept;

  /// @brief Register consumer Updates Observer.
  /// Only one instance of the @c UpdatesObserver may be registered by an application process, therefore this
  /// method always unregister previous observer and return its unique pointer.
  /// If <tt>(nullptr == observer)</tt> then the method only unregister the previous observer!
  /// The method returns @c nullptr if no observers have been registered yet!
  /// @param[in] observer  optional pointer to a client-supplied @c UpdatesObserver instance that should be
  /// registered inside %Key Storage implementation and called every time, when an opened for usage/loading
  /// key slot is updated externally (by its "Owner" application)
  /// @returns unique pointer to previously registered Updates Observer interface (the pointer ownership is
  /// "moved out" to the caller code)
  /// @trace SWS_CRYPT_30130
  //UpdatesObserver::Uptr RegisterObserver(UpdatesObserver::Uptr observer = nullptr) noexcept;

  /// @brief Get pointer of registered Updates Observer.
  /// The method returns @c nullptr if no observers have been registered yet!
  /// @returns unique pointer to the registered Updates Observer interface (copy of an internal unique pointer
  /// is returned, i.e. the %Key Storage provider continues to keep the ownership)
  /// @trace SWS_CRYPT_30131
  //UpdatesObserver::Uptr GetRegisteredObserver() const noexcept;
};

} // namespace keys
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_KEY_STORAGE_INC_PUBLIC_KEY_STORAGE_PROVIDER_IMPL_H_
