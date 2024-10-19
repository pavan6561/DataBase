// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_slot_prototype_props.h
//
// Purpose     : This file contains Key Slot Prototype Properties structure definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEY_SLOT_PROTOTYPE_PROPS_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEY_SLOT_PROTOTYPE_PROPS_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"

namespace ara
{
namespace crypto
{
namespace keys
{
/// @brief Prototyped Properties of a Key Slot.
/// @trace SWS_CRYPT_30300
struct KeySlotPrototypeProps
{
  /// @brief Constructor for KeyslotPrototypeProps
  /// @trace SWS_CRYPT_30301
  KeySlotPrototypeProps() = default;

  /// @brief Unique pointer for KeySlotPrototypeProps
  /// @trace SWS_CRYPT_30302
  using Uptr = std::unique_ptr<KeySlotPrototypeProps>;

  /// @brief Key-slot type configuration: all key-slots used by the adaptive machine to provide service such
  /// as secure communication, diagnostics, updates, secure storage etc. shall use the type kMachine.
  /// All key-slots that will be used by the adaptive user application must use kApplication.
  /// A key-manager user application may define kMachine key-slots as well; in this case the integrator
  /// must match a corresponding machine key-slot to be managed.
  /// @trace SWS_CRYPT_30305
  KeySlotType mSlotType;

  /// @brief Cryptoalgorithm restriction (@c kAlgIdAny means without restriction).
  /// The algorithm can be specified partially: family & length, mode, padding.
  /// @trace SWS_CRYPT_30306
  CryptoAlgId mAlgId;

  /// @brief Capacity of the slot in bytes.
  /// @trace SWS_CRYPT_30307
  std::size_t mSlotCapacity;

  /// @brief Restriction of an object type that can be stored the slot.
  /// If this field contains @c CryptoObjectType::kUnknown then without restriction of the type.
  /// @trace SWS_CRYPT_30308
  CryptoObjectType mObjectType;

  /// @brief Indicates whether FC Crypto shall allocate sufficient storage space for a shadow copy of this
  /// KeySlot.
  /// @trace SWS_CRYPT_30309
  bool mAllocateSpareSlot;

  /// @brief Indicates whether the content of this key-slot may be changed, e.g. from storing a symmetric key
  /// to storing an RSA key
  /// If this is set to false, then the mObjectType of this KeySlotPrototypeProps must be a) valid and b)
  /// cannot be changed (i.e. only objects of mObjectType may be stored in this key-slot).
  /// @trace SWS_CRYPT_30310
  bool mAllowContentTypeChange;

  /// @brief Specifies how many times this key-slot may be updated, e.g.:
  /// - a value of 0 means the key-slot content will be pre-set during production
  /// - a value of 1 means the key-slot content can be updated only once ("OTP")
  /// - a negative value means the key-slot content can be updated infinitely
  /// @trace SWS_CRYPT_30311
  std::int32_t mMaxUpdateAllowed;

  /// @brief Indicates whether the key-slot content may be exported.
  /// @trace SWS_CRYPT_30312
  bool mExportAllowed;

  /// @brief Indicates how the content may be used. The following use cases of this attribute are considered:
  /// - the object to be stored in this key-slot has it's AllowedUsageFlags set to kAllowPrototypedOnly.
  /// In this case this attribute must be observed when loading the content into a runtime instance
  /// (e.g. the AllowedUsageFlags of a SymmetricKey object should be set according to this attribute)
  /// - mMaxUpdatesAllowed==0, in this case the content is provided during production while the
  /// AllowedUsageFlags is modeled using this attribute
  /// - when this key-slot is flexibly updated the runtime object's AllowedUsageFlags override this attribute
  /// upon a later loading from this key-slot
  /// @trace SWS_CRYPT_30313
  AllowedUsageFlags mContentAllowedUsage;
};

/// @brief Comparison operator "equal" for @c KeySlotPrototypeProps operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if all members' values of @c lhs is equal to @c rhs, and @c false otherwise
/// @trace SWS_CRYPT_30350
inline constexpr bool operator==(const KeySlotPrototypeProps& lhs, const KeySlotPrototypeProps& rhs) noexcept
{
  return (lhs.mContentAllowedUsage == rhs.mContentAllowedUsage) && (lhs.mExportAllowed == rhs.mExportAllowed)
    && (lhs.mMaxUpdateAllowed == rhs.mMaxUpdateAllowed)
    && (lhs.mAllowContentTypeChange == rhs.mAllowContentTypeChange)
    && (lhs.mAllocateSpareSlot == rhs.mAllocateSpareSlot) && (lhs.mAlgId == rhs.mAlgId)
    && (lhs.mSlotCapacity == rhs.mSlotCapacity) && (lhs.mObjectType == rhs.mObjectType)
    && (lhs.mSlotType == rhs.mSlotType);
}

/// @brief Comparison operator "not equal" for @c KeySlotPrototypeProps operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if at least one member of @c lhs has a value not equal to correspondent member of @c rhs,
/// and @c false otherwise
/// @trace SWS_CRYPT_30351
inline constexpr bool operator!=(const KeySlotPrototypeProps& lhs, const KeySlotPrototypeProps& rhs) noexcept
{
  return !(lhs == rhs);
}

} // namespace keys
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEY_SLOT_PROTOTYPE_PROPS_H_
