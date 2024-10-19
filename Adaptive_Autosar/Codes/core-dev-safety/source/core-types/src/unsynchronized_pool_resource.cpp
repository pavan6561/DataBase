// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : unsynchronized_pool_resource.cpp
//
// Purpose     : This file provides the implementation of class unsynchronized pool resource.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/unsynchronized_pool_resource.h"

namespace ara
{
namespace core
{
namespace pmr
{
/// @brief creates a unsynchronized_pool_resource parameter constructor.
unsynchronized_pool_resource::unsynchronized_pool_resource(
    const pool_options& opts, memory_resource* upstream) noexcept :
    m_resource(opts, upstream)
{
}
/// @brief creates a unsynchronized_pool_resource default constructor.
unsynchronized_pool_resource::unsynchronized_pool_resource() noexcept : m_resource()
{
}
/// @brief creates a unsynchronized_pool_resource parameter constructor.
unsynchronized_pool_resource::unsynchronized_pool_resource(memory_resource* upstream) noexcept :
    m_resource(upstream)
{
}
/// @brief creates a unsynchronized_pool_resource parameter constructor.
unsynchronized_pool_resource::unsynchronized_pool_resource(const pool_options& opts) noexcept :
    m_resource(opts)
{
}
/// @brief creates a unsynchronized_pool_resource destructor.
unsynchronized_pool_resource::~unsynchronized_pool_resource() //virtual
{
}
/// @brief creates a defination of release method.
void unsynchronized_pool_resource::release()
{
  m_resource.release();
}
/// @brief creates a defination of upstream_resource method.
memory_resource* unsynchronized_pool_resource::upstream_resource() const
{
  return m_resource.upstream_resource();
}
/// @brief creates a defination of options method.
pool_options unsynchronized_pool_resource::options() const
{
  return m_resource.options();
}
/// @brief creates a defination of do_allocate method.
void* unsynchronized_pool_resource::do_allocate(std::size_t bytes, std::size_t alignment) noexcept
{
  return m_resource.do_allocate(bytes, alignment);
}
/// @brief creates a defination of do_deallocate method.
void unsynchronized_pool_resource::do_deallocate(void* p, std::size_t bytes, std::size_t alignment) noexcept
{
  return m_resource.do_deallocate(p, bytes, alignment);
}
/// @brief creates a defination of do_is_equal method.
bool unsynchronized_pool_resource::do_is_equal(const memory_resource& other) const noexcept
{
  return this == &other;
}
/// @brief creates a defination of pool_count method.
std::size_t unsynchronized_pool_resource::pool_count() const
{
  return m_resource.pool_count();
}
/// @brief creates a defination of pool_index method.
std::size_t unsynchronized_pool_resource::pool_index(std::size_t bytes) const
{
  return m_resource.pool_index(bytes);
}
/// @brief creates a defination of pool_next_blocks_per_chunk method.
std::size_t unsynchronized_pool_resource::pool_next_blocks_per_chunk(std::size_t pool_idx) const
{
  return m_resource.pool_next_blocks_per_chunk(pool_idx);
}
/// @brief creates a defination of pool_block method.
std::size_t unsynchronized_pool_resource::pool_block(std::size_t pool_idx) const
{
  return m_resource.pool_block(pool_idx);
}
/// @brief creates a defination of pool_cached_blocks method.
std::size_t unsynchronized_pool_resource::pool_cached_blocks(std::size_t pool_idx) const
{
  return m_resource.pool_cached_blocks(pool_idx);
}

} // namespace pmr
} // namespace core
} // namespace ara
