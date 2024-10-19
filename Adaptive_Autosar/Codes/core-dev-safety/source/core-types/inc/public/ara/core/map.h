// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : map.h
//
// Purpose     : This file provides the implementation of class map.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_MAP_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_MAP_H_

#include <map>
#include <memory>
#include <type_traits>
#include <utility>
#include "ara/core/polymorphic_allocator.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief A container that contains key-value pairs with unique keys.
/// @tparam Key the type of keys in this Map.
/// @tparam T the type of values in this Map.
/// @tparam Compare the type of comparison Callable.
/// @tparam Allocator the type of Allocator to use for this container.
/// @trace SWS_CORE_00040
/// @archtrace 609888
/// @design 614039
template <typename Key, typename T, typename Compare = std::less<Key>,
    typename Allocator = std::allocator<std::pair<const Key, T>>>
using Map = std::map<Key, T, Compare, Allocator>;

/// @brief namespace of pmr.
namespace pmr
{
template <class Key, class T, class Compare = std::less<Key>>
using Map = ara::core::Map<Key, T, Compare, ara::core::pmr::polymorphic_allocator<std::pair<const Key, T>>>;
}
/// @brief Add overload of std::swap for Map.
/// We actually don't need this overload at all, because our implementation is just
/// a type alias and thus can simply use the overload for the std:: type.
/// However, we need this symbol in order to provide up-tracing.
/// @tparam Key The type of keys in the Maps.
/// @tparam T The type of values in the Maps.
/// @tparam Compare The type of comparison Callable in the Maps.
/// @tparam Allocator The type of Allocators in the Maps.
/// @param[in] lhs The first argument of the swap invocation.
/// @param[in] rhs The second argument of the swap invocation.
/// @trace SWS_CORE_00040
/// @archtrace 609888
/// @design 611847
template <typename Key, typename T, typename Compare, typename Allocator,
    typename = typename std::enable_if<
        !std::is_same<Map<Key, T, Compare, Allocator>, std::map<Key, T, Compare, Allocator>>::value>::type>
void swap(Map<Key, T, Compare, Allocator>& lhs, Map<Key, T, Compare, Allocator>& rhs) noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  lhs.swap(rhs);
}

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_MAP_H_
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
