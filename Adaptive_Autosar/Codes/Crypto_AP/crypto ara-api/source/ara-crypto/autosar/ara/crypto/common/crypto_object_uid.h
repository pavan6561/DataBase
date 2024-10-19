// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_object_uid.h
//
// Purpose     : Implementation for CryptoObjectUid class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_CRYPTO_OBJECT_UID_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_CRYPTO_OBJECT_UID_H_

#include <cstdint>

#include "ara/crypto/common/uuid.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief Definition of Crypto Object Unique Identifier (@b COUID) type.
/// @trace SWS_CRYPT_10100
struct CryptoObjectUid
{
  /// @brief UUID of a generator that has produced this COUID. This UUID can be associated with HSM,
  /// physical host/ECU or VM.
  /// @trace SWS_CRYPT_10101
  Uuid mGeneratorUid;

  /// @brief Sequential value of a steady timer or simple counter, representing version of correspondent
  /// Crypto Object.
  /// @trace SWS_CRYPT_10102
  std::uint64_t mVersionStamp = 0u;

  /// @brief Check whether this identifier has a common source with the one provided by the argument.
  /// @param[in] anotherId  another identifier for the comparison
  /// @returns @c true if both identifiers has common source (identical value of the @c mGeneratorUid field)
  /// @trace SWS_CRYPT_10111
  constexpr bool HasSameSourceAs(const CryptoObjectUid& anotherId) const noexcept;

  /// @brief Check whether this identifier was generated earlier than the one provided by the argument.
  /// @param[in] anotherId  another identifier for the comparison
  /// @returns @c true if this identifier was generated earlier than the @c anotherId
  /// @trace SWS_CRYPT_10112
  constexpr bool HasEarlierVersionThan(const CryptoObjectUid& anotherId) const noexcept;

  /// @brief Check whether this identifier was generated later than the one provided by the argument.
  /// @param[in] anotherId  another identifier for the comparison
  /// @returns @c true if this identifier was generated later than the @c anotherId
  /// @trace SWS_CRYPT_10113
  constexpr bool HasLaterVersionThan(const CryptoObjectUid& anotherId) const noexcept;

  /// @brief Check whether this identifier is "Nil".
  /// @returns @c true if this identifier is "Nil" and @c false otherwise
  /// @trace SWS_CRYPT_10114
  bool IsNil() const noexcept;

  /// @brief Check whether this object's generator identifier is "Nil".
  /// @returns @c true if this identifier is "Nil" and @c false otherwise
  /// @trace SWS_CRYPT_10115
  bool SourceIsNil() const noexcept;
};

inline constexpr bool CryptoObjectUid::HasSameSourceAs(const CryptoObjectUid& anotherId) const noexcept
{
  return mGeneratorUid == anotherId.mGeneratorUid;
}

inline constexpr bool CryptoObjectUid::HasEarlierVersionThan(const CryptoObjectUid& anotherId) const noexcept
{
  return HasSameSourceAs(anotherId) && (mVersionStamp < anotherId.mVersionStamp);
}

inline constexpr bool CryptoObjectUid::HasLaterVersionThan(const CryptoObjectUid& anotherId) const noexcept
{
  return HasSameSourceAs(anotherId) && (mVersionStamp > anotherId.mVersionStamp);
}

inline bool CryptoObjectUid::IsNil() const noexcept
{
  return mGeneratorUid.IsNil() && (0u == mVersionStamp);
}

inline bool CryptoObjectUid::SourceIsNil() const noexcept
{
  return mGeneratorUid.IsNil() && (0u == mVersionStamp);
}

/// @brief Comparison operator "equal" for @c CryptoObjectUid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if all members' values of @c lhs is equal to @c rhs, and @c false otherwise
/// @trace SWS_CRYPT_10150
inline constexpr bool operator==(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept
{
  return lhs.HasSameSourceAs(rhs) && (lhs.mVersionStamp == rhs.mVersionStamp);
}

/// @brief Comparison operator "less than" for @c CryptoObjectUid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is less than @c rhs, and @c false otherwise
/// @trace SWS_CRYPT_10151
inline constexpr bool operator<(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept
{
  return lhs.HasEarlierVersionThan(rhs);
}

/// @brief Comparison operator "greater than" for @c CryptoObjectUid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is greater than @c rhs, and @c false otherwise
/// @trace SWS_CRYPT_10152
inline constexpr bool operator>(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept
{
  return lhs.HasLaterVersionThan(rhs);
}

/// @brief Comparison operator "not equal" for @c CryptoObjectUid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if at least one member of @c lhs has a value not equal to correspondent member of @c rhs,
/// and @c false otherwise
/// @trace SWS_CRYPT_10153
inline constexpr bool operator!=(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept
{
  return !(lhs == rhs);
}

/// @brief Comparison operator "less than or equal" for @c CryptoObjectUid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is less than or equal to @c rhs, and @c false
/// otherwise
/// @trace SWS_CRYPT_10154
inline constexpr bool operator<=(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept
{
  return lhs.HasSameSourceAs(rhs) && (lhs.mVersionStamp <= rhs.mVersionStamp);
}

/// @brief Comparison operator "greater than or equal" for @c CryptoObjectUid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is greater than or equal to @c rhs, and @c false
/// otherwise
/// @trace SWS_CRYPT_10155
inline constexpr bool operator>=(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept
{
  return lhs.HasSameSourceAs(rhs) && (lhs.mVersionStamp >= rhs.mVersionStamp);
}

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_CRYPTO_OBJECT_UID_H_
