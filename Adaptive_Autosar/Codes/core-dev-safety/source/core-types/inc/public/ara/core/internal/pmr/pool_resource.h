// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : pool_resource.h
//
// Purpose     : This file contains the pool resource class declaration.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_POOL_RESOURCE_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_POOL_RESOURCE_H_

#include "ara/core/polymorphic_allocator.h"
#include "ara/core/internal/pmr/block_list.h"
#include "ara/core/internal/pmr/pool_options.h"

#include <cstddef>

namespace ara
{
namespace core
{
namespace pmr
{
namespace internal
{
/// @brief The pool_data_t is a forward class.
/// @archtrace 633881
class pool_data_t;

static const std::size_t pool_options_minimum_max_blocks_per_chunk = 1u;
static const std::size_t pool_options_default_max_blocks_per_chunk = 32u;
static const std::size_t pool_options_minimum_largest_required_pool_block
    = memory_resource::max_align > 2 * sizeof(void*) ? memory_resource::max_align : 2 * sizeof(void*);
static const std::size_t pool_options_default_largest_required_pool_block
    = pool_options_minimum_largest_required_pool_block > 4096u
    ? pool_options_minimum_largest_required_pool_block
    : 4096u;

/// @brief The pool_resource is a special-purpose pool resource class that releases
/// @archtrace 633881
/// @design 634055
class pool_resource
{
  typedef block_list_base<> block_list_base_t;

  pool_options m_options;
  memory_resource& m_upstream;
  block_list_base_t m_oversized_list;
  pool_data_t* m_pool_data;
  std::size_t m_pool_count;
  /// @brief The priv_limit_option method.
  /// @param[in] val The size of value.
  /// @param[in] min The size of minimum value.
  /// @param[in] max The size of maximum value.
  /// @archtrace 633881
  /// @design 645103
  static void priv_limit_option(std::size_t& val, std::size_t min, std::size_t max);
  /// @brief The priv_pool_index method.
  /// @param[in] block_size The size of value.
  /// @returns Returns the size of the pool index.
  /// @archtrace 633881
  /// @design 645104
  static std::size_t priv_pool_index(std::size_t block_size);
  /// @brief The priv_pool_block method.
  /// @param[in] index The size of value.
  /// @returns Returns the size of the pool block.
  /// @archtrace 633881
  /// @design 645105
  static std::size_t priv_pool_block(std::size_t index);
  /// @brief The privious fixed options method.
  /// @archtrace 633881
  /// @design 645106
  void priv_fix_options();
  /// @brief The privious init pools method.
  /// @archtrace 633881
  /// @design 645107
  void priv_init_pools();
  /// @brief The privious constructor body method.
  /// @archtrace 633881
  /// @design 645108
  void priv_constructor_body();

 public:
  /// @brief The parameter constructor of pool_resource object.
  /// @param[in] upstream A memory_resource to an object
  /// @param[in] opts A pool_options to an object
  /// @archtrace 633881
  /// @design 634056
  pool_resource(const pool_options& opts, memory_resource* upstream) noexcept;

  /// @brief The default constructor of pool_resource object.
  /// @archtrace 633881
  /// @design 634057
  pool_resource() noexcept;

  /// @brief The parameter constructor of pool_resource object.
  /// @param[in] upstream A memory_resource to an object
  /// @archtrace 633881
  /// @design 634058
  explicit pool_resource(memory_resource* upstream) noexcept;

  /// @brief The parameter constructor of pool_resource object.
  /// @param[in] opts A pool_options to an object
  /// @archtrace 633881
  /// @design 634059
  explicit pool_resource(const pool_options& opts) noexcept;
  /// @brief Copy constructor shall be disabled.
  /// @archtrace 633881
  pool_resource(const pool_resource&) = delete;
  /// @brief Copy assignment operator shall be disabled.
  /// @archtrace 633881
  pool_resource operator=(const pool_resource&) = delete;

  /// @brief The destructor for unsynchronized_pool_resource objects.
  /// @archtrace 633881
  /// @design 634060
  ~pool_resource();

  /// @brief The releases all memory owned by this resource.
  /// @archtrace 633881
  /// @design 634061
  void release();

  /// @brief This is the same value as the upstream argument passed to the constructor of this object.
  /// @returns Returns a pointer to the upstream memory resource.
  /// @archtrace 633881
  /// @design 634062
  memory_resource* upstream_resource() const;

  /// @brief The options that controls the pooling behavior of this resource.
  /// @returns Returns the options that controls the pooling behavior of this resource.
  /// @archtrace 633881
  /// @design 634063
  pool_options options() const;

 public: //public so that [un]synchronized_pool_resource can use them
  /// @brief The Allocates storage.
  /// @param[in] bytes The pool bytes.
  /// @param[in] alignment The pool alignment.
  /// @returns A pointer to allocated storage of at least bytes bytes in size.
  /// @archtrace 633881
  /// @design 634064
  void* do_allocate(std::size_t bytes, std::size_t alignment);

  /// @brief The Deallocates storage.
  /// @param[in] p Deallocates the storage pointed to by p.
  /// @param[in] bytes The pool bytes.
  /// @param[in] alignment The pool alignment.
  /// @archtrace 633881
  /// @design 634065
  void do_deallocate(void* p, std::size_t bytes, std::size_t alignment);

  //Non-standard observers
 public:
  /// @brief The count of the pool.
  /// @returns size of the count pool.
  /// @archtrace 633881
  /// @design 634066
  std::size_t pool_count() const;

  /// @brief The index of the pool.
  /// @param[in] bytes The pool bytes.
  /// @returns size of the index pool.
  /// @archtrace 633881
  /// @design 634067
  std::size_t pool_index(std::size_t bytes) const;

  /// @brief The block chunk of the pool.
  /// @param[in] pool_idx The size of thr pool index.
  /// @returns The number blocks that will be allocated in the next chunk
  ///  from the pool specified by `pool_idx`.
  /// @archtrace 633881
  /// @design 634068
  std::size_t pool_next_blocks_per_chunk(std::size_t pool_idx) const;

  /// @brief The block of the pool.
  /// @param[in] pool_idx The size of thr pool index.
  /// @returns size of block pool.
  /// @archtrace 633881
  /// @design 634069
  std::size_t pool_block(std::size_t pool_idx) const;

  /// @brief The cached block of the pool.
  /// @param[in] pool_idx The size of thr pool index.
  /// @returns size of cached block pool.
  /// @archtrace 633881
  /// @design 634070
  std::size_t pool_cached_blocks(std::size_t pool_idx) const;
};

} // namespace internal
} // namespace pmr
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_POOL_RESOURCE_H_
