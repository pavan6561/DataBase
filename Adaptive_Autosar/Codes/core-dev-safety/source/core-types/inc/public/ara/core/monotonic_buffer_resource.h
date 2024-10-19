// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : monotonic_buffer_resource.h
//
// Purpose     : This file contains the monotonic buffer resource declaration.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_MONOTONIC_BUFFER_RESOURCE_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_MONOTONIC_BUFFER_RESOURCE_H_

#include "ara/core/polymorphic_allocator.h"
#include "ara/core/internal/pmr/block_slist.h"
#include <iostream>
#include <cstddef>

namespace ara
{
namespace core
{
namespace pmr
{
//! A monotonic_buffer_resource is a special-purpose memory resource intended for
//! very fast memory allocations in situations where memory is used to build up a
//! few objects and then is released all at once when the memory resource object
//! is destroyed. It has the following qualities:
//!
//! - A call to deallocate has no effect, thus the amount of memory consumed
//!   increases monotonically until the resource is destroyed.
//!
//! - The program can supply an initial buffer, which the allocator uses to satisfy
//!   memory requests.
//!
//! - When the initial buffer (if any) is exhausted, it obtains additional buffers
//!   from an upstream memory resource supplied at construction. Each additional
//!   buffer is larger than the previous one, following a geometric progression.
//!
//! - It is intended for access from one thread of control at a time. Specifically,
//!   calls to allocate and deallocate do not synchronize with one another.
//!
//! - It owns the allocated memory and frees it on destruction, even if deallocate has
//!   not been called for some of the allocated blocks.

/// @brief The monotonic_buffer_resource is a special-purpose memory resource class that releases
/// the allocated memory only when the resource is destroyed.
/// @archtrace 633881
/// @design 634080
class monotonic_buffer_resource : public memory_resource
{
  internal::block_slist m_memory_blocks;
  void* m_current_buffer;
  std::size_t m_current_buffer_size;
  std::size_t m_next_buffer_size;
  void* const m_initial_buffer;
  std::size_t const m_initial_buffer_size;

  /// @brief Increase the buffer resource of monotonic.
  /// @archtrace 633881
  /// @design 645158
  void increase_next_buffer();
  /// @brief Increase the buffer at least one resource.
  /// @param[in] minimum_size The size of minimum.
  /// @archtrace 633881
  /// @design 645159
  void increase_next_buffer_at_least_to(std::size_t minimum_size);
  /// @brief Allocate bytes from the current resource.
  /// @param[in] aligner The size of aligner.
  /// @param[in] bytes The size of bytes.
  /// @returns Return the void pointer.
  /// @archtrace 633881
  /// @design 645160
  void* allocate_from_current(std::size_t aligner, std::size_t bytes);

 public:
  static const std::size_t initial_next_buffer_size = 32u * sizeof(void*);
  static constexpr float growth_factor = 2;
  /// @brief The default constructor of monotonic_buffer_resource object.
  /// @archtrace 633881
  /// @design 634081
  monotonic_buffer_resource() :
      m_memory_blocks(*get_default_resource()), m_current_buffer(0), m_current_buffer_size(0u),
      m_next_buffer_size(initial_next_buffer_size), m_initial_buffer(0), m_initial_buffer_size(0u)
  {
  }
  /// @brief The destructor for unsynchronized_pool_resource objects.
  /// @archtrace 633881
  /// @design 634082
  ~monotonic_buffer_resource() override;

  /// @brief The parameter constructor of monotonic_buffer_resource object.
  /// @param[in] upstream A memory_resource to an object.
  /// @archtrace 633881
  /// @design 634083
  explicit monotonic_buffer_resource(memory_resource* upstream) noexcept;
  /// @brief The parameter constructor of monotonic_buffer_resource object.
  /// @param[in] initial_size Initialization size of resource.
  /// @archtrace 633881
  /// @design 634084
  explicit monotonic_buffer_resource(std::size_t initial_size) noexcept;
  /// @brief The parameter constructor of monotonic_buffer_resource object.
  /// @param[in] upstream A memory_resource to an object.
  /// @param[in] initial_size Initialization size of resource.
  /// @archtrace 633881
  /// @design 634085
  monotonic_buffer_resource(std::size_t initial_size, memory_resource* upstream) noexcept;
  /// @brief The parameter constructor of monotonic_buffer_resource object.
  /// @param[in] buffer A monotonic buffer resouces.
  /// @param[in] buffer_size A monotonic buffer resouces size.
  /// @archtrace 633881
  /// @design 634086
  monotonic_buffer_resource(void* buffer, std::size_t buffer_size) noexcept;
  /// @brief The parameter constructor of monotonic_buffer_resource object.
  /// @param[in] buffer A monotonic buffer resouces.
  /// @param[in] buffer_size A monotonic buffer resouces size.
  /// @param[in] upstream A memory_resource to an object.
  /// @archtrace 633881
  /// @design 634087
  monotonic_buffer_resource(void* buffer, std::size_t buffer_size, memory_resource* upstream) noexcept;
  /// @brief Copy constructor shall be disabled.
  /// @archtrace 633881

  monotonic_buffer_resource(monotonic_buffer_resource const&) = delete;
  /// @brief Copy assignment operator shall be disabled.
  /// @archtrace 633881

  monotonic_buffer_resource operator=(monotonic_buffer_resource const&) = delete;

  /// @brief The monotonic remaining storage.
  /// @param[in] alignment The monotonic alignment.
  /// @param[in] wasted_due_to_alignment The monotonic wasted alignment.
  /// @returns Return the remaing storage of monotonic.
  /// @archtrace 633881
  /// @design 634088
  std::size_t remaining_storage(std::size_t alignment, std::size_t& wasted_due_to_alignment) const noexcept;
  /// @brief The monotonic remaining storage.
  /// @param[in] alignment The monotonic alignment.
  /// @returns Return the remaing storage of monotonic.
  /// @archtrace 633881
  /// @design 634089
  std::size_t remaining_storage(std::size_t alignment = 1u) const noexcept;
  /// @brief The monotonic current buffer.
  /// @returns Return the void pointer.
  /// @archtrace 633881
  /// @design 634090
  const void* current_buffer() const noexcept;
  /// @brief The monotonic next buffer size.
  /// @returns Return the next buffer size.
  /// @archtrace 633881
  /// @design 634091
  std::size_t next_buffer_size() const noexcept;
  /// @brief The releases all memory owned by this resource.
  /// @archtrace 633881
  /// @design 634092
  void release() noexcept;
  /// @brief This is the same value as the upstream argument passed to the constructor of this object.
  /// @returns Returns a pointer to the upstream memory resource.
  /// @archtrace 633881
  /// @design 634093
  memory_resource* upstream_resource() const noexcept;

 protected:
  /// @brief The Allocates storage.
  /// @param[in] bytes The pool bytes.
  /// @param[in] alignment The pool alignment.
  /// @returns A pointer to allocated storage of at least bytes bytes in size.
  /// @archtrace 633881
  /// @design 634094
  virtual void* do_allocate(std::size_t bytes, std::size_t alignment) override;
  /// @brief The Deallocates storage.
  /// @param[in] p Deallocates the storage pointed to by p.
  /// @param[in] bytes The pool bytes.
  /// @param[in] alignment The pool alignment.
  /// @archtrace 633881
  /// @design 634095
  virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) noexcept override;
  /// @brief Memory allocated using a unsynchronized_pool_resource can only be deallocated using that same resource.
  /// @param[in] other The memory_resource of the object.
  /// @returns Returns the true or flase.
  /// @archtrace 633881
  /// @design 634096
  virtual bool do_is_equal(const memory_resource& other) const noexcept override;
};

} // namespace pmr
} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_MONOTONIC_BUFFER_RESOURCE_H_
