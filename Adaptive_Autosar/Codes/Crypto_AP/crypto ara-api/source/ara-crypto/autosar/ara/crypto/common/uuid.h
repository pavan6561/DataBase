// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : uuid.h
//
// Purpose     : This file contains definitions for Uuid class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_UUID_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_UUID_H_

#include <cstdint>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief Definition of Universally Unique Identifier (@b UUID) type.
/// Independently from internal definition details of this structure, it's size @b must be 16 bytes and
/// entropy of this ID should be close to 128 bit!
/// @trace SWS_CRYPT_10400
struct Uuid
{
  /// @uptrace{RS_CRYPTO_02005}
  /// @brief Most significant QWORD.
  /// @trace SWS_CRYPT_10413
  std::uint64_t mQwordMs = 0u;

  /// @brief Less significant QWORD.
  /// @trace SWS_CRYPT_10412
  std::uint64_t mQwordLs = 0u;

  /// @brief Check whether this identifier is the "Nil UUID" (according to RFC4122).
  /// @returns @c true if this identifier is "Nil" and @c false otherwise
  /// @trace SWS_CRYPT_10411
  bool IsNil() const noexcept;
};

inline bool Uuid::IsNil() const noexcept
{
  return (0u == mQwordMs) && (0u == mQwordLs);
}

/// @brief Comparison operator "equal" for @c Uuid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is equal to @c rhs, and @c false otherwise
/// @trace SWS_CRYPT_10451
inline constexpr bool operator==(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return (lhs.mQwordMs == rhs.mQwordMs) && (lhs.mQwordLs == rhs.mQwordLs);
}

/// @brief Comparison operator "less than" for @c Uuid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is less than @c rhs, and @c false otherwise
/// @trace SWS_CRYPT_10452
inline constexpr bool operator<(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return (lhs.mQwordMs < rhs.mQwordMs) || ((lhs.mQwordMs == rhs.mQwordMs) && (lhs.mQwordLs < rhs.mQwordLs));
}

/// @brief Comparison operator "greater than" for @c Uuid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is greater than @c rhs, and @c false otherwise
/// @trace SWS_CRYPT_10453
inline constexpr bool operator>(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return (lhs.mQwordMs > rhs.mQwordMs) || ((lhs.mQwordMs == rhs.mQwordMs) && (lhs.mQwordLs > rhs.mQwordLs));
}

/// @brief Comparison operator "not equal" for @c Uuid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is not equal to @c rhs, and @c false otherwise
/// @trace SWS_CRYPT_10454
inline constexpr bool operator!=(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return !(lhs == rhs);
}

/// @brief Comparison operator "less than or equal" for @c Uuid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is less than or equal to @c rhs, and @c false
/// otherwise
/// @trace SWS_CRYPT_10455
inline constexpr bool operator<=(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return !(lhs > rhs);
}

/// @brief Comparison operator "greater than or equal" for @c Uuid operands.
/// @param[in] lhs  left-hand side operand
/// @param[in] rhs  right-hand side operand
/// @returns @c true if a binary representation of @c lhs is greater than or equal to @c rhs, and @c false
/// otherwise
/// @trace SWS_CRYPT_10456
inline constexpr bool operator>=(const Uuid& lhs, const Uuid& rhs) noexcept
{
  return !(lhs < rhs);
}

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_UUID_H_
