// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : utilities.h
//
// Purpose     : Contains some utility functions for PMR.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_UTILITIES_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_UTILITIES_H_

#include <climits>
namespace ara
{
namespace core
{
namespace pmr
{
namespace internal
{
/// @brief The maximum value.
/// @tparam T The type of class.
/// @param[in] a The T class of object.
/// @param[in] b The T class of object.
/// @returns Returns the T object.
/// @archtrace 633881
/// @design 645109
template <class T>
const T& max_value(const T& a, const T& b)
{
  return a > b ? a : b;
}
/// @brief The minimum value.
/// @tparam T The type of class.
/// @param[in] a The T class of object.
/// @param[in] b The T class of object.
/// @returns Returns the T object.
/// @archtrace 633881
/// @design 645110
template <class T>
const T& min_value(const T& a, const T& b)
{
  return a < b ? a : b;
}
/// @brief The floor log.
/// @param[in] v The size of log.
/// @param[in] integral_constant The size of log.
/// @returns Returns the size of log.
/// @archtrace 633881
/// @design 645111
inline std::size_t floor_log2(std::size_t v, std::integral_constant<std::size_t, 32>)
{
  static const int MultiplyDeBruijnBitPosition[32] = {0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3,
      30, 8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31};

  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;

  return MultiplyDeBruijnBitPosition[(std::size_t)(v * 0x07C4ACDDU) >> 27];
}
/// @brief The floor log.
/// @param[in] v The size of log.
/// @param[in] integral_constant The size of log.
/// @returns Returns the size of log.
/// @archtrace 633881
/// @design 645112
inline std::size_t floor_log2(std::size_t v, std::integral_constant<std::size_t, 64>)
{
  static const std::size_t MultiplyDeBruijnBitPosition[64] = {63, 0, 58, 1, 59, 47, 53, 2, 60, 39, 48, 27, 54,
      33, 42, 3, 61, 51, 37, 40, 49, 18, 28, 20, 55, 30, 34, 11, 43, 14, 22, 4, 62, 57, 46, 52, 38, 26, 32,
      41, 50, 36, 17, 19, 29, 10, 13, 21, 56, 45, 25, 31, 35, 16, 9, 12, 44, 24, 15, 8, 23, 7, 6, 5};

  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  return MultiplyDeBruijnBitPosition[((std::size_t)((v - (v >> 1)) * 0x07EDD5E59A4E28C2ULL)) >> 58];
}
/// @brief The floor log.
/// @param[in] x The size of log.
/// @returns Returns the size of log.
/// @archtrace 633881
/// @design 645113
inline std::size_t floor_log2(std::size_t x)
{
  const std::size_t Bits = sizeof(std::size_t) * CHAR_BIT;
  return floor_log2(x, std::integral_constant<std::size_t, Bits>());
}
/// @brief The is pow2.
/// @param[in] x The size of pow.
/// @returns Returns the true or false.
/// @archtrace 633881
/// @design 645114
inline bool is_pow2(std::size_t x)
{
  return (x & (x - 1)) == 0;
}
/// @brief The ceil log.
/// @param[in] x The size of log.
/// @returns Returns the size of ceil log.
/// @archtrace 633881
/// @design 645115
inline std::size_t ceil_log2(std::size_t x)
{
  return static_cast<std::size_t>(!(is_pow2)(x)) + floor_log2(x);
}
/// @brief The ceil pow.
/// @param[in] x The size of pow.
/// @returns Returns the size of ceil pow.
/// @archtrace 633881
/// @design 645116
inline std::size_t ceil_pow2(std::size_t x)
{
  return std::size_t(1u) << (ceil_log2)(x);
}
/// @brief The previous or equal pow.
/// @param[in] x The size of pow.
/// @returns Returns the size of previous or equal pow.
/// @archtrace 633881
/// @design 645117
inline std::size_t previous_or_equal_pow2(std::size_t x)
{
  return std::size_t(1u) << floor_log2(x);
}

template <std::size_t N>
/// @brief creates a structure of static_is_pow2.
struct static_is_pow2
{
  static const bool value = (N & (N - 1)) == 0;
};
} // namespace internal
} // namespace pmr
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_UTILITIES_H_
