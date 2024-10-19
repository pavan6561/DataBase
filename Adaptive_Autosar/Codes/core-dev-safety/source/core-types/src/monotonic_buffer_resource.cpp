// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : monotonic_buffer_resource.cpp
//
// Purpose     : Contains the implementation of MonotonicBufferResource.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/monotonic_buffer_resource.h"
#include "ara/core/internal/pmr/utilities.h"

#include <cstddef>
#include <type_traits>
#include <iostream>

namespace
{
typedef std::size_t uintptr_type;
static const std::size_t minimum_buffer_size = 2 * sizeof(void*);

} // namespace

namespace ara
{
namespace core
{
namespace pmr
{
void monotonic_buffer_resource::increase_next_buffer()
{
  m_next_buffer_size
      = (std::size_t(-1) / 2 < m_next_buffer_size) ? std::size_t(-1) : m_next_buffer_size * growth_factor;
}

void monotonic_buffer_resource::increase_next_buffer_at_least_to(std::size_t minimum_size)
{
  if (m_next_buffer_size < minimum_size)
  {
    if (internal::is_pow2(minimum_size))
    {
      m_next_buffer_size = minimum_size;
    }
    else if (std::size_t(-1) / 2 < minimum_size)
    {
      m_next_buffer_size = minimum_size;
    }
    else
    {
      m_next_buffer_size = internal::ceil_pow2(minimum_size);
    }
  }
}

monotonic_buffer_resource::monotonic_buffer_resource(memory_resource* upstream) noexcept :
    m_memory_blocks(upstream ? *upstream : *get_default_resource()), m_current_buffer(0),
    m_current_buffer_size(0u), m_next_buffer_size(initial_next_buffer_size), m_initial_buffer(0),
    m_initial_buffer_size(0u)
{
}

monotonic_buffer_resource::monotonic_buffer_resource(std::size_t initial_size) noexcept :
    m_memory_blocks(*get_default_resource()), m_current_buffer(0), m_current_buffer_size(0u),
    m_next_buffer_size(minimum_buffer_size), m_initial_buffer(0), m_initial_buffer_size(0u)
{
  //In case initial_size is zero
  this->increase_next_buffer_at_least_to(initial_size + !initial_size);
}

monotonic_buffer_resource::monotonic_buffer_resource(
    std::size_t initial_size, memory_resource* upstream) noexcept :
    m_memory_blocks(upstream ? *upstream : *get_default_resource()),
    m_current_buffer(0), m_current_buffer_size(0u), m_next_buffer_size(minimum_buffer_size),
    m_initial_buffer(0), m_initial_buffer_size(0u)
{
  // In case initial_size is zero
  this->increase_next_buffer_at_least_to(initial_size + !initial_size);
}

monotonic_buffer_resource::monotonic_buffer_resource(void* buffer, std::size_t buffer_size) noexcept :
    m_memory_blocks(*get_default_resource()), m_current_buffer(buffer), m_current_buffer_size(buffer_size),
    m_next_buffer_size(internal::previous_or_equal_pow2(
        internal::max_value(buffer_size, std::size_t(initial_next_buffer_size)))),
    m_initial_buffer(buffer), m_initial_buffer_size(buffer_size)
{
  this->increase_next_buffer();
}

monotonic_buffer_resource::monotonic_buffer_resource(
    void* buffer, std::size_t buffer_size, memory_resource* upstream) noexcept :
    m_memory_blocks(upstream ? *upstream : *get_default_resource()),
    m_current_buffer(buffer), m_current_buffer_size(buffer_size),
    m_next_buffer_size(internal::previous_or_equal_pow2(
        internal::max_value(buffer_size, std::size_t(initial_next_buffer_size)))),
    m_initial_buffer(buffer), m_initial_buffer_size(buffer_size)
{
  this->increase_next_buffer();
}

monotonic_buffer_resource::~monotonic_buffer_resource()
{
  this->release();
}

void monotonic_buffer_resource::release() noexcept
{
  m_memory_blocks.release();
  m_current_buffer = m_initial_buffer;
  m_current_buffer_size = m_initial_buffer_size;
  m_next_buffer_size = initial_next_buffer_size;
}

memory_resource* monotonic_buffer_resource::upstream_resource() const noexcept
{
  return &m_memory_blocks.upstream_resource();
}

std::size_t monotonic_buffer_resource::remaining_storage(
    std::size_t alignment, std::size_t& wasted_due_to_alignment) const noexcept
{
  const uintptr_type up_alignment_minus1 = alignment - 1u;
  const uintptr_type up_alignment_mask = ~up_alignment_minus1;
  const uintptr_type up_addr = uintptr_type(m_current_buffer);
  const uintptr_type up_aligned_addr = (up_addr + up_alignment_minus1) & up_alignment_mask;
  wasted_due_to_alignment = std::size_t(up_aligned_addr - up_addr);
  return m_current_buffer_size <= wasted_due_to_alignment ? 0u
                                                          : m_current_buffer_size - wasted_due_to_alignment;
}

std::size_t monotonic_buffer_resource::remaining_storage(std::size_t alignment) const noexcept
{
  std::size_t ignore_this;
  return this->remaining_storage(alignment, ignore_this);
}

const void* monotonic_buffer_resource::current_buffer() const noexcept
{
  return m_current_buffer;
}

std::size_t monotonic_buffer_resource::next_buffer_size() const noexcept
{
  return m_next_buffer_size;
}

void* monotonic_buffer_resource::allocate_from_current(std::size_t aligner, std::size_t bytes)
{
  char* p = (char*)m_current_buffer + aligner;
  m_current_buffer = p + bytes;
  m_current_buffer_size -= aligner + bytes;
  return p;
}

void* monotonic_buffer_resource::do_allocate(std::size_t bytes, std::size_t alignment)
{
  if (alignment > memory_resource::max_align)
  {
    (void)bytes;
    (void)alignment;
    throw std::bad_alloc();
  }

  // See if there is room in current buffer
  std::size_t aligner = 0u;
  if (this->remaining_storage(alignment, aligner) < bytes)
  {
    // The new buffer will be aligned to the strictest alignment so reset
    // the aligner, which was needed for the old buffer.
    aligner = 0u;
    // Update next_buffer_size to at least bytes
    this->increase_next_buffer_at_least_to(bytes);
    // Now allocate and update internal data
    m_current_buffer = (char*)m_memory_blocks.allocate(m_next_buffer_size);
    m_current_buffer_size = m_next_buffer_size;
    this->increase_next_buffer();
  }
  // Enough internal storage, extract from it.
  return this->allocate_from_current(aligner, bytes);
}

void monotonic_buffer_resource::do_deallocate(void* p, std::size_t bytes, std::size_t alignment) noexcept
{
  (void)p;
  (void)bytes;
  (void)alignment;
}

bool monotonic_buffer_resource::do_is_equal(const memory_resource& other) const noexcept
{
  return this == &other;
}

} // namespace pmr
} // namespace core
} // namespace ara
