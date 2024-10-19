// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : updates_observer.h
//
// Purpose     : This file contains Updates Observer interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEY_UPDATES_OBSERVER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEY_UPDATES_OBSERVER_H_

#include "ara/crypto/keys/elementary_types.h"

namespace ara
{
namespace crypto
{
namespace keys
{
/// @brief Definition of an "updates observer" interface.
/// The "updates observer" interface should be implemented by a consumer application, if a software
/// developer would like to get notifications about the slots' content update events.
/// @trace SWS_CRYPT_30200
class UpdatesObserver
{
 public:
  /// @brief Shared smart pointer of the interface.
  /// @trace SWS_CRYPT_30201
  using Uptr = std::unique_ptr<UpdatesObserver>;

  /// @brief Copy-assign another UpdatesObserver to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30224
  UpdatesObserver& operator=(const UpdatesObserver& other) = default;

  /// @brief Move-assign another UpdatesObserver to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30225
  UpdatesObserver& operator=(UpdatesObserver&& other) = default;

  /// @brief Destructor.
  /// @trace SWS_CRYPT_30210
  virtual ~UpdatesObserver() noexcept = default;

  /// @brief Notification method that should be called if content of specified slots was changed.
  /// Key Storage engine should call this method in a dedicated thread.
  /// The provided list may include only slots subscribed for observing (during openning with the "User"
  /// permissions, i.e. for "reading" via a call of the method @c OpenAsUser()).
  /// Each slot number may present in the provided list only one time!
  /// @param[in] updatedSlots  List of monitored slots that were updated after opening (for reading)
  /// @trace SWS_CRYPT_30211
  virtual void OnUpdate(const TransactionScope& updatedSlots) noexcept = 0;
};

} // namespace keys
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_UPDATES_OBSERVER_H_
