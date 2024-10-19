// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : block_list.h
//
// Purpose     : Templated class for a block list, a data structure that manages a list of blocks of memory.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_BLOCK_LIST_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_BLOCK_LIST_H_

#include "ara/core/polymorphic_allocator.h"
#include "ara/core/internal/pmr/circular_list_algorithms.h"

#include <cstddef>

namespace ara
{
namespace core
{
namespace pmr
{
namespace internal
{
/// @brief create a structure of list_node.
struct list_node
{
  list_node* next;
  list_node* previous;
};
/// @brief create a structure of list_node_traits.
struct list_node_traits
{
  typedef list_node node;
  typedef list_node* node_ptr;
  typedef const list_node* const_node_ptr;
  /// @brief create a defination of get_next method.
  static node_ptr get_next(const_node_ptr n)
  {
    return n->next;
  }
  /// @brief create a defination of get_previous method.
  static node_ptr get_previous(const_node_ptr n)
  {
    return n->previous;
  }
  /// @brief create a defination of set_next method.
  static void set_next(const node_ptr& n, const node_ptr& next)
  {
    n->next = next;
  }
  /// @brief create a defination of set_previous method.
  static void set_previous(const node_ptr& n, const node_ptr& previous)
  {
    n->previous = previous;
  }
};
/// @brief create a structure of block_list_header.
struct block_list_header : public list_node
{
  std::size_t size;
};

typedef circular_list_algorithms<list_node_traits> list_algo;

template <class DerivedFromBlockListHeader = block_list_header>
/// @brief The block_list_base is a class.
/// @archtrace 633881
/// @design 644972
class block_list_base
{
  list_node m_list;

  static const std::size_t MaxAlignMinus1 = memory_resource::max_align - 1u;

 public:
  static const std::size_t header_size
      = std::size_t(sizeof(DerivedFromBlockListHeader) + MaxAlignMinus1) & std::size_t(~MaxAlignMinus1);
  /// @brief The default constructor of block_list_base object.
  /// @archtrace 633881
  /// @design 644973
  explicit block_list_base()
  {
    list_algo::init_header(&m_list);
  }

  block_list_base(const block_list_base&) = delete;
  block_list_base operator=(const block_list_base&) = delete;
  /// @brief The destructor of block_list_base object.
  /// @archtrace 633881
  /// @design 644974
  ~block_list_base()
  {
  }
  /// @brief The allocate of size and memory.
  /// @param[in] size The size of the allocater.
  /// @param[in] mr The memory resource.
  /// @returns Returns the void pointer.
  /// @archtrace 633881
  /// @design 644975
  void* allocate(std::size_t size, memory_resource& mr)
  {
    if ((size_t(-1) - header_size) < size)
      throw std::bad_alloc();
    void* p = mr.allocate(size + header_size);
    block_list_header& mb = *::new ((void*)p) DerivedFromBlockListHeader;
    mb.size = size + header_size;
    list_algo::link_after(&m_list, &mb);
    return (char*)p + header_size;
  }
  /// @brief The deallocate method.
  /// @param[in] p The void pointer.
  /// @param[in] mr The memory resource.
  /// @archtrace 633881
  /// @design 644976
  void deallocate(void* p, memory_resource& mr) noexcept
  {
    DerivedFromBlockListHeader* pheader
        = static_cast<DerivedFromBlockListHeader*>(static_cast<void*>((char*)p - header_size));
    list_algo::unlink(pheader);
    const std::size_t size = pheader->size;
    static_cast<DerivedFromBlockListHeader*>(pheader)->~DerivedFromBlockListHeader();
    mr.deallocate(pheader, size, memory_resource::max_align);
  }
  /// @brief The release method.
  /// @param[in] mr The memory resource.
  /// @archtrace 633881
  /// @design 644977
  void release(memory_resource& mr) noexcept
  {
    list_node* n = list_algo::node_traits::get_next(&m_list);
    while (n != &m_list)
    {
      DerivedFromBlockListHeader& d = static_cast<DerivedFromBlockListHeader&>(*n);
      n = list_algo::node_traits::get_next(n);
      std::size_t size = d.size;
      d.~DerivedFromBlockListHeader();
      mr.deallocate(reinterpret_cast<char*>(&d), size, memory_resource::max_align);
    }
    list_algo::init_header(&m_list);
  }
};

} // namespace internal
} // namespace pmr
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_BLOCK_LIST_H_
