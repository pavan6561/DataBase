// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name : set.h
//
// Purpose : This file provides the implementation of class Set.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_SET_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_SET_H_

#include <set>
#include <memory>
#include <type_traits>
#include <utility>
#include "ara/core/polymorphic_allocator.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.x
namespace core
{
template <typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
using Set = std::set<Key, Compare, Allocator>;

template <typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
using MultiSet = std::multiset<Key, Compare, Allocator>;

namespace pmr
{
template <typename Key, typename Compare = std::less<Key>>
using Set = ara::core::Set<Key, Compare, ara::core::pmr::polymorphic_allocator<Key>>;

template <typename Key, typename Compare = std::less<Key>>
using MultiSet = MultiSet<Key, Compare, ara::core::pmr::polymorphic_allocator<Key>>;

} // namespace pmr

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_SET_H_
