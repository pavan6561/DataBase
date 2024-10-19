// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name : unordered_map.h
//
// Purpose : This file provides the implementation of class Unordered_Map.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_UNORDERED_MAP_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_UNORDERED_MAP_H_

#include <unordered_map>
// #include <type_traits>
// #include <utility>
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

template <typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>,
    typename Allocator = std::allocator<std::pair<const Key, T>>>
using Unordered_Map = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

namespace pmr
{
template <typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
using Unordered_Map = ara::core::Unordered_Map<Key, T, Hash, KeyEqual,
    ara::core::pmr::polymorphic_allocator<std::pair<const Key, T>>>;
}

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_UNORDERED_MAP_H_
