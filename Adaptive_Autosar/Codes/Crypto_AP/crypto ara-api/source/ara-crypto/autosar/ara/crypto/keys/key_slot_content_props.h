// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_slot_content_props.h
//
// Purpose     : This file contains Key Slot Content Properties structure definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEY_SLOT_CONTENT_PROPS_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEY_SLOT_CONTENT_PROPS_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"

namespace ara
{
namespace crypto
{
namespace keys
{
/// @brief Properties of current Key Slot Content, i.e. of a current instance stored to the Key Slot.
/// A value of the @c mAllowedUsage field is bitwise AND of the common usage flags defined at run-time and
/// the usage flags defined by the @c UserPermissions prototype for current "Actor".
/// @trace SWS_CRYPT_30500
struct KeySlotContentProps
{
  /// @brief set content properties
  /// @trace SWS_CRYPT_30510
  KeySlotContentProps() = default;

  /// @brief shared pointer of interface
  /// @trace SWS_CRYPT_30511
  using Uptr = std::unique_ptr<KeySlotContentProps>;

  /// @brief UID of a Crypto Object stored to the slot.
  /// @trace SWS_CRYPT_30501
  CryptoObjectUid mObjectUid;

  /// @brief Cryptoalgorithm of actual object stored to the slot.
  /// @trace SWS_CRYPT_30503
  CryptoAlgId mAlgId;

  /// @brief Actual size of an object currently stored to the slot.
  /// @trace SWS_CRYPT_30505
  std::size_t mObjectSize;

  /// @brief Actual usage restriction flags of an object stored to the slot for the current "Actor".
  /// @trace SWS_CRYPT_30506
  AllowedUsageFlags mContentAllowedUsage;

  /// @brief Actual type of an object stored to the slot.
  /// @trace SWS_CRYPT_30508
  CryptoObjectType mObjectType;
};

/// @brief Comparison operator "equal" for @c KeySlotContentProps operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if all members' values of @c lhs is equal to @c rhs, and @c false otherwise
/// @trace SWS_CRYPT_30550
inline constexpr bool operator==(const KeySlotContentProps& lhs, const KeySlotContentProps& rhs) noexcept
{
  return (lhs.mObjectUid == rhs.mObjectUid) && (lhs.mContentAllowedUsage == rhs.mContentAllowedUsage)
    && (lhs.mAlgId == rhs.mAlgId) && (lhs.mObjectSize == rhs.mObjectSize)
    && (lhs.mObjectType == rhs.mObjectType);
}

/// @brief Comparison operator "not equal" for @c KeySlotContentProps operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if at least one member of @c lhs has a value not equal to correspondent member of @c rhs,
/// and @c false otherwise
/// @trace SWS_CRYPT_30551
inline constexpr bool operator!=(const KeySlotContentProps& lhs, const KeySlotContentProps& rhs) noexcept
{
  return !(lhs == rhs);
}

} // namespace keys
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_KEY_SLOT_CONTENT_PROPS_H_
