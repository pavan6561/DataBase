// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : vector.h
//
// Purpose     : This file provides the implementation of class vector.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_VECTOR_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_VECTOR_H_

#include <vector>
#include <memory>
#include <type_traits>
#include "ara/core/polymorphic_allocator.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief A sequence container that encapsulates dynamically sized arrays.
/// @tparam T The type of contained values.
/// @tparam Allocator The type of Allocator to use for this container.
/// @trace SWS_CORE_00040
/// @archtrace 609887
template <typename T, typename Allocator = std::allocator<T>>
using Vector = std::vector<T, Allocator>;

/// @brief namespace of pmr.
namespace pmr
{
template <class Tp>
using Vector = ara::core::Vector<Tp, pmr::polymorphic_allocator<Tp>>;
}
/// @brief Global operator== for Vector instances.
/// @tparam T The type of values in the Vectors.
/// @tparam Allocator The Allocator used by the Vectors.
/// @param[in] lhs The first argument of the swap invocation.
/// @param[in] rhs The second argument of the swap invocation.
/// @returns true if the contents of the vectors are equal, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609887
/// @design 611877
template <typename T, typename Allocator,
    typename
    = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
inline bool operator==(Vector<T, Allocator> const& lhs, Vector<T, Allocator> const& rhs)
{
  return std::operator==(lhs, rhs);
}

/// @brief Global operator!= for Vector instances.
/// @tparam T The type of values in the Vectors.
/// @tparam Allocator The Allocator used by the Vectors.
/// @param[in] lhs The first argument of the swap invocation.
/// @param[in] rhs The second argument of the swap invocation.
/// @returns true if the contents of the lhs are lexicographically
/// less than the contents of rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609887
/// @design 611878
template <typename T, typename Allocator,
    typename
    = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
inline bool operator!=(Vector<T, Allocator> const& lhs, Vector<T, Allocator> const& rhs)
{
  return std::operator!=(lhs, rhs);
}

/// @brief Global operator< for Vector instances.
/// @tparam T The type of values in the Vectors.
/// @tparam Allocator The Allocator used by the Vectors.
/// @param[in] lhs The first argument of the swap invocation.
/// @param[in] rhs The second argument of the swap invocation.
/// @returns true if the contents of the lhs are lexicographically
/// less than the contents of rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609887
/// @design 611879
template <typename T, typename Allocator,
    typename
    = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
inline bool operator<(Vector<T, Allocator> const& lhs, Vector<T, Allocator> const& rhs)
{
  return std::operator<(lhs, rhs);
}

/// @brief Global operator<= for Vector instances.
/// @tparam T The type of values in the Vectors.
/// @tparam Allocator The Allocator used by the Vectors.
/// @param[in] lhs The first argument of the swap invocation.
/// @param[in] rhs The second argument of the swap invocation.
/// @returns true if the contents of the lhs are lexicographically
/// less than or equal to the contents of rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609887
/// @design 611880
template <typename T, typename Allocator,
    typename
    = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
inline bool operator<=(Vector<T, Allocator> const& lhs, Vector<T, Allocator> const& rhs)
{
  return std::operator<=(lhs, rhs);
}

/// @brief Global operator> for Vector instances.
/// @tparam T The type of values in the Vectors.
/// @tparam Allocator The Allocator used by the Vectors.
/// @param[in] lhs The first argument of the swap invocation.
/// @param[in] rhs The second argument of the swap invocation.
/// @returns true if the contents of the lhs are lexicographically
/// greater than the contents of rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609887
/// @design 611881
template <typename T, typename Allocator,
    typename
    = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
inline bool operator>(Vector<T, Allocator> const& lhs, Vector<T, Allocator> const& rhs)
{
  return std::operator>(lhs, rhs);
}

/// @brief Global operator>= for Vector instances.
/// @tparam T The type of values in the Vectors.
/// @tparam Allocator The Allocator used by the Vectors.
/// @param[in] lhs The first argument of the swap invocation.
/// @param[in] rhs The second argument of the swap invocation.
/// @returns true if the contents of the lhs are lexicographically
/// greater than or equal to the contents of rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609887
/// @design 611882
template <typename T, typename Allocator,
    typename
    = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
inline bool operator>=(Vector<T, Allocator> const& lhs, Vector<T, Allocator> const& rhs)
{
  return std::operator>=(lhs, rhs);
}

/// @brief Add overload of swap for ara::core::Vector.
/// We actually don't need this overload at all, because our implementation is just
/// a type alias and thus can simply use the overload for the std:: type.
/// However, we need this symbol in order to provide uptracing.
/// @tparam T The type of values in the Vectors.
/// @tparam Allocator The Allocator used by the Vectors.
/// @param[in] lhs The first argument of the swap invocation.
/// @param[in] rhs The second argument of the swap invocation.
/// @trace SWS_CORE_00040
/// @archtrace 609887
/// @design 611883
template <typename T, typename Allocator,
    typename
    = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
void swap(Vector<T, Allocator>& lhs, Vector<T, Allocator>& rhs) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  lhs.swap(rhs);
}

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_VECTOR_H_
// -----------------------------------------------------------------------------------------------------------
// AVIN Guideline Exception(s)
// -----------------------------------------------------------------------------------------------------------
// - CodingStyle-Naming.Function
//   Guideline:           AUTOSAR CPP14 Guidelines, CodingStyle-Naming.Function required
//   Description:         Function names shall be pascal case.
//   Justification:       This will create another violation for initialization with pascal case. This has
//                        been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
