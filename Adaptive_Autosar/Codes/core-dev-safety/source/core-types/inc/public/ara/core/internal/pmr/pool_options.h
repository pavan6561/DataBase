// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : pool_options.h
//
// Purpose     : Header file contains pool_options comprise a set of constructor options for pool resources.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_POOL_OPTIONS_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_POOL_OPTIONS_H_

#include <cstddef>

namespace ara
{
namespace core
{
namespace pmr
{
//! The members of pool_options comprise a set of constructor options for pool resources.
//! The effect of each option on the pool resource behavior is described below:
//!
//! - `std::size_t max_blocks_per_chunk`: The maximum number of blocks that will be allocated
//!   at once from the upstream memory resource to replenish a pool. If the value of
//!   `max_blocks_per_chunk` is zero or is greater than an implementation-defined limit,
//!   that limit is used instead. The implementation may choose to use a smaller value
//!   than is specified in this field and may use different values for different pools.
//!
//! - `std::size_t largest_required_pool_block`: The largest allocation size that is required
//!   to be fulfilled using the pooling mechanism. Attempts to allocate a single block
//!   larger than this threshold will be allocated directly from the upstream memory
//!   resource. If largest_required_pool_block is zero or is greater than an
//!   implementation-defined limit, that limit is used instead. The implementation may
//!   choose a pass-through threshold larger than specified in this field.

/// @brief The pool_options is a set of constructor options for pool resources including.
/// @archtrace 633881
struct pool_options
{
  /// @brief The parameter constructor of pool_options object.
  pool_options() : max_blocks_per_chunk(0u), largest_required_pool_block(0u)
  {
  }
  /// @brief The parameter constructor of pool_options object.
  /// @param[in] blocks The maximum number of blocks that will be allocated at once from the upstream.
  /// @param[in] largest_block The largest allocation size that is required to be fulfilled using the pooling mechanism.
  explicit pool_options(std::size_t blocks, std::size_t largest_block) :
      max_blocks_per_chunk(blocks), largest_required_pool_block(largest_block)
  {
  }
  std::size_t max_blocks_per_chunk;
  std::size_t largest_required_pool_block;
};

} // namespace pmr
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_POOL_OPTIONS_H_
