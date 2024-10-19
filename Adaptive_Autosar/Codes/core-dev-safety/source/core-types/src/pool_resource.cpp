// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : pool_resource.cpp
//
// Purpose     : This file provides the implementation of class pool resource.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/internal/pmr/pool_resource.h"
#include "ara/core/internal/pmr/block_slist.h"
#include "ara/core/internal/pmr/utilities.h"
#include <iostream>

#include <cstddef>

namespace ara
{
namespace core
{
namespace pmr
{
namespace internal
{
// pool_data_t..

class pool_data_t : public block_slist_base<>
{
  typedef block_slist_base<> block_slist_base_t;

 public:
  explicit pool_data_t(std::size_t initial_blocks_per_chunk) :
      block_slist_base_t(), next_blocks_per_chunk(initial_blocks_per_chunk)
  {
    slist_algo::init_header(&free_slist);
  }

  void* allocate_block() noexcept
  {
    if (slist_algo::unique(&free_slist))
    {
      return 0;
    }
    slist_node* pv = slist_algo::node_traits::get_next(&free_slist);
    slist_algo::unlink_after(&free_slist);
    pv->~slist_node();
    return pv;
  }

  void deallocate_block(void* p) noexcept
  {
    slist_node* pv = ::new (p) slist_node();
    slist_algo::link_after(&free_slist, pv);
  }

  void release(memory_resource& upstream)
  {
    slist_algo::init_header(&free_slist);
    this->block_slist_base_t::release(upstream);
    next_blocks_per_chunk = pool_options_minimum_max_blocks_per_chunk;
  }

  void replenish(memory_resource& mr, std::size_t pool_block, std::size_t max_blocks_per_chunk)
  {
    // Limit max value
    std::size_t blocks_per_chunk = min_value(max_blocks_per_chunk, next_blocks_per_chunk);
    // Avoid overflow
    blocks_per_chunk = min_value(blocks_per_chunk, std::size_t(-1) / pool_block);

    // Minimum block size is at least max_align, so all pools allocate sizes that are multiple of max_align,
    // meaning that all blocks are max_align-aligned.
    char* p = static_cast<char*>(block_slist_base_t::allocate(blocks_per_chunk * pool_block, mr));

    // Create header types. This is no-throw
    for (std::size_t i = 0, max = blocks_per_chunk; i != max; ++i)
    {
      slist_node* const pv = ::new (p) slist_node();
      slist_algo::link_after(&free_slist, pv);
      p += pool_block;
    }

    // Update next block per chunk
    next_blocks_per_chunk
        = max_blocks_per_chunk / 2u < blocks_per_chunk ? max_blocks_per_chunk : blocks_per_chunk * 2u;
  }

  std::size_t cache_count() const
  {
    return slist_algo::count(&free_slist) - 1u;
  }

  slist_node free_slist;
  std::size_t next_blocks_per_chunk;
};

// pool_resource

// Detect overflow in ceil_pow2
static_assert(pool_options_default_max_blocks_per_chunk <= (std::size_t(-1) / 2u + 1u));
// Sanity checks
static_assert(static_is_pow2<pool_options_default_max_blocks_per_chunk>::value);
static_assert(static_is_pow2<pool_options_minimum_largest_required_pool_block>::value);

// unsynchronized_pool_resource

void pool_resource::priv_limit_option(std::size_t& val, std::size_t min, std::size_t max) // static
{
  if (!val)
  {
    val = max;
  }
  else
  {
    val = val < min ? min : min_value(val, max);
  }
}

std::size_t pool_resource::priv_pool_index(std::size_t block_size) // static
{
  // For allocations equal or less than pool_options_minimum_largest_required_pool_block
  // the smallest pool is used
  block_size = max_value(block_size, pool_options_minimum_largest_required_pool_block);
  return ceil_log2(block_size) - ceil_log2(pool_options_minimum_largest_required_pool_block);
}

std::size_t pool_resource::priv_pool_block(std::size_t index) // static
{
  // For allocations equal or less than pool_options_minimum_largest_required_pool_block
  // the smallest pool is used
  return pool_options_minimum_largest_required_pool_block << index;
}

void pool_resource::priv_fix_options()
{
  priv_limit_option(m_options.max_blocks_per_chunk, pool_options_minimum_max_blocks_per_chunk,
      pool_options_default_max_blocks_per_chunk);
  priv_limit_option(m_options.largest_required_pool_block, pool_options_minimum_largest_required_pool_block,
      pool_options_default_largest_required_pool_block);
  m_options.largest_required_pool_block = ceil_pow2(m_options.largest_required_pool_block);
}

void pool_resource::priv_init_pools()
{
  const std::size_t num_pools = priv_pool_index(m_options.largest_required_pool_block) + 1u;
  // Otherwise, just use the default alloc (zero pools)
  void* p = 0;
  // This can throw
  p = m_upstream.allocate(sizeof(pool_data_t) * num_pools);
  // This is nothrow
  m_pool_data = static_cast<pool_data_t*>(p);
  for (std::size_t i = 0, max = num_pools; i != max; ++i)
  {
    ::new (&m_pool_data[i]) pool_data_t(pool_options_minimum_max_blocks_per_chunk);
  }
  m_pool_count = num_pools;
}

void pool_resource::priv_constructor_body()
{
  this->priv_fix_options();
}

pool_resource::pool_resource(const pool_options& opts, memory_resource* upstream) noexcept :
    m_options(opts), m_upstream(*upstream), m_oversized_list(), m_pool_data(), m_pool_count()
{
  this->priv_constructor_body();
}

pool_resource::pool_resource() noexcept :
    m_options(), m_upstream(*get_default_resource()), m_oversized_list(), m_pool_data(), m_pool_count()
{
  this->priv_constructor_body();
}

pool_resource::pool_resource(memory_resource* upstream) noexcept :
    m_options(), m_upstream(*upstream), m_oversized_list(), m_pool_data(), m_pool_count()
{
  this->priv_constructor_body();
}

pool_resource::pool_resource(const pool_options& opts) noexcept :
    m_options(opts), m_upstream(*get_default_resource()), m_oversized_list(), m_pool_data(), m_pool_count()
{
  this->priv_constructor_body();
}

pool_resource::~pool_resource()
{
  this->release();

  for (std::size_t i = 0, max = m_pool_count; i != max; ++i)
  {
    m_pool_data[i].~pool_data_t();
  }
  if (m_pool_data)
  {
    m_upstream.deallocate((void*)m_pool_data, sizeof(pool_data_t) * m_pool_count);
  }
}

void pool_resource::release()
{
  m_oversized_list.release(m_upstream);
  for (std::size_t i = 0, max = m_pool_count; i != max; ++i)
  {
    m_pool_data[i].release(m_upstream);
  }
}

memory_resource* pool_resource::upstream_resource() const
{
  return &m_upstream;
}

pool_options pool_resource::options() const
{
  return m_options;
}

void* pool_resource::do_allocate(std::size_t bytes, std::size_t alignment)
{
  if (!m_pool_data)
  {
    this->priv_init_pools();
  }
  (void)alignment; // alignment ignored here, max_align is used by pools
  if (bytes > m_options.largest_required_pool_block)
  {
    return m_oversized_list.allocate(bytes, m_upstream);
  }
  else
  {
    const std::size_t pool_idx = priv_pool_index(bytes);
    pool_data_t& pool = m_pool_data[pool_idx];
    void* p = pool.allocate_block();
    if (!p)
    {
      pool.replenish(m_upstream, priv_pool_block(pool_idx), m_options.max_blocks_per_chunk);
      p = pool.allocate_block();
    }
    return p;
  }
}

void pool_resource::do_deallocate(void* p, std::size_t bytes, std::size_t alignment)
{
  (void)alignment; // alignment ignored here, max_align is used by pools
  if (bytes > m_options.largest_required_pool_block)
  {
    // Just cached
    return m_oversized_list.deallocate(p, m_upstream);
  }
  else
  {
    const std::size_t pool_idx = priv_pool_index(bytes);
    return m_pool_data[pool_idx].deallocate_block(p);
  }
}

std::size_t pool_resource::pool_count() const
{
  if ((0 != m_pool_data))
  {
    return m_pool_count;
  }
  else
  {
    return priv_pool_index(m_options.largest_required_pool_block) + 1u;
  }
}

std::size_t pool_resource::pool_index(std::size_t bytes) const
{
  if (bytes > m_options.largest_required_pool_block)
  {
    return pool_count();
  }
  else
  {
    return priv_pool_index(bytes);
  }
}

std::size_t pool_resource::pool_next_blocks_per_chunk(std::size_t pool_idx) const
{
  if ((m_pool_data && pool_idx < m_pool_count))
  {
    return m_pool_data[pool_idx].next_blocks_per_chunk;
  }
  else
  {
    return 1u;
  }
}

std::size_t pool_resource::pool_block(std::size_t pool_idx) const
{
  return priv_pool_block(pool_idx);
}

std::size_t pool_resource::pool_cached_blocks(std::size_t pool_idx) const
{
  if ((m_pool_data && pool_idx < m_pool_count))
  {
    return m_pool_data[pool_idx].cache_count();
  }
  else
  {
    return 0u;
  }
}

} //  namespace internal
} //  namespace pmr
} //  namespace core
} //  namespace ara
