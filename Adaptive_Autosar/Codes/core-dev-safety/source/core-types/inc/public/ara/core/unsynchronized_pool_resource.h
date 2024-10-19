// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : unsynchronized_pool_resource.h
//
// Purpose     : This file contains the unsynchronized pool resource class declaration.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_UNSYNCHRONIZED_POOL_RESOURCE_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_UNSYNCHRONIZED_POOL_RESOURCE_H_

#include "ara/core/polymorphic_allocator.h"
#include "ara/core/internal/pmr/pool_resource.h"

#include <cstddef>

namespace ara
{
namespace core
{
namespace pmr
{
//! A unsynchronized_pool_resource is a general-purpose memory resources having
//! the following qualities:
//!
//! - Each resource owns the allocated memory, and frees it on destruction,
//!   even if deallocate has not been called for some of the allocated blocks.
//!
//! - A pool resource consists of a collection of pools, serving
//!   requests for different block sizes. Each individual pool manages a
//!   collection of chunks that are in turn divided into blocks of uniform size,
//!   returned via calls to do_allocate. Each call to do_allocate(size, alignment)
//!   is dispatched to the pool serving the smallest blocks accommodating at
//!   least size bytes.
//!
//! - When a particular pool is exhausted, allocating a block from that pool
//!   results in the allocation of an additional chunk of memory from the upstream
//!   allocator (supplied at construction), thus replenishing the pool. With
//!   each successive replenishment, the chunk size obtained increases
//!   geometrically. [ Note: By allocating memory in chunks, the pooling strategy
//!   increases the chance that consecutive allocations will be close together
//!   in memory. - end note ]
//!
//! - Allocation requests that exceed the largest block size of any pool are
//!   fulfilled directly from the upstream allocator.
//!
//! - A pool_options struct may be passed to the pool resource constructors to
//!   tune the largest block size and the maximum chunk size.
//!
//! An unsynchronized_pool_resource class may not be accessed from multiple threads
//! simultaneously and thus avoids the cost of synchronization entirely in
//! single-threaded applications.

/// @brief The unsynchronized_pool_resource class using the specified upstream memory resource and tuned
/// according to the specified options. The resulting object holds a copy of upstream but does not
/// own the resource to which upstream points.
/// @archtrace 633881
/// @design 634097
class unsynchronized_pool_resource : public memory_resource
{
  internal::pool_resource m_resource;

 public:
  /// @brief The default constructor of unsynchronized_pool_resource object.
  /// @archtrace 633881
  /// @design 634038
  unsynchronized_pool_resource() noexcept;
  /// @brief The parameter constructor of unsynchronized_pool_resource object.
  /// @param[in] upstream A memory_resource to an object
  /// @archtrace 633881
  /// @design 634039
  explicit unsynchronized_pool_resource(memory_resource* upstream) noexcept;
  /// @brief The parameter constructor of unsynchronized_pool_resource object.
  /// @param[in] opts A pool_options to an object
  /// @archtrace 633881
  /// @design 634040
  explicit unsynchronized_pool_resource(const pool_options& opts) noexcept;
  /// @brief The parameter constructor of unsynchronized_pool_resource object.
  /// @param[in] upstream A memory_resource to an object
  /// @param[in] opts A pool_options to an object
  /// @archtrace 633881
  /// @design 634041
  unsynchronized_pool_resource(const pool_options& opts, memory_resource* upstream) noexcept;
  /// @brief Copy constructor shall be disabled.
  /// @archtrace 633881
  unsynchronized_pool_resource(const unsynchronized_pool_resource&) = delete;
  /// @brief Copy assignment operator shall be disabled.
  /// @archtrace 633881
  unsynchronized_pool_resource operator=(const unsynchronized_pool_resource&) = delete;
  /// @brief The destructor for unsynchronized_pool_resource objects.
  /// @archtrace 633881
  /// @design 634042
  ~unsynchronized_pool_resource() override;
  /// @brief The releases all memory owned by this resource.
  /// @archtrace 633881
  /// @design 634043
  void release();
  /// @brief This is the same value as the upstream argument passed to the constructor of this object.
  /// @returns Returns a pointer to the upstream memory resource.
  /// @archtrace 633881
  /// @design 634044
  memory_resource* upstream_resource() const;
  /// @brief The options that controls the pooling behavior of this resource.
  /// @returns Returns the options that controls the pooling behavior of this resource.
  /// @archtrace 633881
  /// @design 634045
  pool_options options() const;

 protected:
  /// @brief The Allocates storage.
  /// @param[in] bytes The pool bytes.
  /// @param[in] alignment The pool alignment.
  /// @returns A pointer to allocated storage of at least bytes bytes in size.
  /// @archtrace 633881
  /// @design 634051
  virtual void* do_allocate(std::size_t bytes, std::size_t alignment) noexcept;
  /// @brief The Deallocates storage.
  /// @param[in] p Deallocates the storage pointed to by p.
  /// @param[in] bytes The pool bytes.
  /// @param[in] alignment The pool alignment.
  /// @archtrace 633881
  /// @design 634052
  virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) noexcept;
  /// @brief Memory allocated using a unsynchronized_pool_resource can only be deallocated using that same resource.
  /// @param[in] other The memory_resource of the object.
  /// @returns Returns the true or flase.
  /// @archtrace 633881
  /// @design 634053
  virtual bool do_is_equal(const memory_resource& other) const noexcept;

 public:
  /// @brief The count of the pool.
  /// @returns size of the count pool.
  /// @archtrace 633881
  /// @design 634046
  std::size_t pool_count() const;
  /// @brief The index of the pool.
  /// @param[in] bytes The pool bytes.
  /// @returns size of the index pool.
  /// @archtrace 633881
  /// @design 634047
  std::size_t pool_index(std::size_t bytes) const;
  /// @brief The block chunk of the pool.
  /// @param[in] pool_idx The size of thr pool index.
  /// @returns size of block chunk pool.
  /// @archtrace 633881
  /// @design 634048
  std::size_t pool_next_blocks_per_chunk(std::size_t pool_idx) const;
  /// @brief The block of the pool.
  /// @param[in] pool_idx The size of thr pool index.
  /// @returns size of block pool.
  /// @archtrace 633881
  /// @design 634049
  std::size_t pool_block(std::size_t pool_idx) const;
  /// @brief The cached block of the pool.
  /// @param[in] pool_idx The size of thr pool index.
  /// @returns size of cached block pool.
  /// @archtrace 633881
  /// @design 634050
  std::size_t pool_cached_blocks(std::size_t pool_idx) const;
};

} // namespace pmr
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_UNSYNCHRONIZED_POOL_RESOURCE_H_
