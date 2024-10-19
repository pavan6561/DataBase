// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : block_slist.h
//
// Purpose     : Header file contains template class for a singly-linked list of blocks of memory.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_BLOCK_SLIST_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_BLOCK_SLIST_H_

#include "ara/core/polymorphic_allocator.h"

#include <cstddef>

namespace ara
{
namespace core
{
namespace pmr
{
namespace internal
{
/// @brief The linear_slist_algorithms is a class.
/// @tparam NodeTraits The type of class.
/// @archtrace 633881
/// @design 644984
template <class NodeTraits>
class linear_slist_algorithms
{
 public:
  typedef typename NodeTraits::node node;
  typedef typename NodeTraits::node_ptr node_ptr;
  typedef typename NodeTraits::const_node_ptr const_node_ptr;
  typedef NodeTraits node_traits;
  /// @brief The parameter constructor of unique object.
  /// @param[in] this_node The const node pointer.
  /// @returns Returns the true or false.
  /// @archtrace 633881
  /// @design 644985
  static bool unique(const_node_ptr this_node)
  {
    node_ptr next = NodeTraits::get_next(this_node);
    return !next || next == this_node;
  }
  /// @brief The unlink after node.
  /// @param[in] prev_node The node pointer.
  /// @archtrace 633881
  /// @design 644986
  static void unlink_after(const node_ptr& prev_node)
  {
    const_node_ptr this_node(NodeTraits::get_next(prev_node));
    NodeTraits::set_next(prev_node, NodeTraits::get_next(this_node));
  }
  /// @brief The link after node.
  /// @param[in] prev_node The node pointer.
  /// @param[in] this_node The node pointer.
  /// @archtrace 633881
  /// @design 644987
  inline static void link_after(const node_ptr& prev_node, const node_ptr& this_node)
  {
    NodeTraits::set_next(this_node, NodeTraits::get_next(prev_node));
    NodeTraits::set_next(prev_node, this_node);
  }
  /// @brief The init header node.
  /// @param[in] this_node The node pointer.
  /// @archtrace 633881
  /// @design 644988
  inline static void init_header(const node_ptr& this_node)
  {
    NodeTraits::set_next(this_node, node_ptr());
  }
  /// @brief The count of the node.
  /// @param[in] this_node The node pointer.
  /// @returns Returns the size of the count.
  /// @archtrace 633881
  /// @design 644989
  static std::size_t count(const const_node_ptr& this_node)
  {
    std::size_t result = 0;
    const_node_ptr p = this_node;
    do
    {
      p = NodeTraits::get_next(p);
      ++result;
    } while (p);
    return result;
  }
};
/// @brief create a structure of slist_node.
struct slist_node
{
  slist_node* next;
};
/// @brief create a structure of slist_node_traits.
struct slist_node_traits
{
  typedef slist_node node;
  typedef slist_node* node_ptr;
  typedef const slist_node* const_node_ptr;
  /// @brief create a defination of get_next method.
  static node_ptr get_next(const_node_ptr n)
  {
    return n->next;
  }
  /// @brief create a defination of set_next method.
  static void set_next(const node_ptr& n, const node_ptr& next)
  {
    n->next = next;
  }
};
/// @brief create a structure of block_slist_header.
struct block_slist_header : public slist_node
{
  std::size_t size;
};

typedef linear_slist_algorithms<slist_node_traits> slist_algo;
template <class DerivedFromBlockSlistHeader = block_slist_header>
/// @brief The block_slist_base is a class.
/// @archtrace 633881
/// @design 644990
class block_slist_base
{
  slist_node m_slist;

  static const std::size_t MaxAlignMinus1 = memory_resource::max_align - 1u;

 public:
  static const std::size_t header_size
      = std::size_t(sizeof(DerivedFromBlockSlistHeader) + MaxAlignMinus1) & std::size_t(~MaxAlignMinus1);
  /// @brief The default constructor of block_slist object.
  /// @archtrace 633881
  /// @design 644991
  explicit block_slist_base()
  {
    slist_algo::init_header(&m_slist);
  }

  block_slist_base(const block_slist_base&) = delete;
  block_slist_base operator=(const block_slist_base&) = delete;
  /// @brief The destructor of block_slist object.
  /// @archtrace 633881
  /// @design 644992
  ~block_slist_base()
  {
  }
  /// @brief The allocate of size and memory.
  /// @param[in] size The size of the allocater.
  /// @param[in] mr The memory resource.
  /// @returns Returns the void pointer.
  /// @archtrace 633881
  /// @design 644993
  void* allocate(std::size_t size, memory_resource& mr)
  {
    if ((size_t(-1) - header_size) < size)
    {
      std::bad_alloc();
    }
    void* p = mr.allocate(size + header_size);
    block_slist_header& mb = *::new ((void*)p) DerivedFromBlockSlistHeader;
    mb.size = size + header_size;
    slist_algo::link_after(&m_slist, &mb);
    return (char*)p + header_size;
  }
  /// @brief The release method.
  /// @param[in] mr The memory resource.
  /// @archtrace 633881
  /// @design 644994
  void release(memory_resource& mr) noexcept
  {
    slist_node* n = slist_algo::node_traits::get_next(&m_slist);
    while (n)
    {
      DerivedFromBlockSlistHeader& d = static_cast<DerivedFromBlockSlistHeader&>(*n);
      n = slist_algo::node_traits::get_next(n);
      std::size_t size = d.block_slist_header::size;
      d.~DerivedFromBlockSlistHeader();
      mr.deallocate(reinterpret_cast<char*>(&d), size, memory_resource::max_align);
    }
    slist_algo::init_header(&m_slist);
  }
};
/// @brief The block slist is a class.
/// @archtrace 633881
/// @design 644978
class block_slist : public block_slist_base<>
{
  memory_resource& m_upstream_rsrc;

 public:
  /// @brief The parameter constructor of block_slist object.
  /// @param[in] upstream_rsrc The memory_resource of the object.
  /// @archtrace 633881
  /// @design 644979
  explicit block_slist(memory_resource& upstream_rsrc) : block_slist_base<>(), m_upstream_rsrc(upstream_rsrc)
  {
  }

  block_slist(const block_slist&) = delete;
  block_slist operator=(const block_slist&) = delete;

  /// @brief The destrutor of block_slist object.
  /// @archtrace 633881
  /// @design 644980
  ~block_slist()
  {
    this->release();
  }

  /// @brief The allocate of size.
  /// @param[in] size The size of the allocater.
  /// @returns Returns the void pointer.
  /// @archtrace 633881
  /// @design 644981
  void* allocate(std::size_t size)
  {
    return this->block_slist_base<>::allocate(size, m_upstream_rsrc);
  }
  /// @brief The release method.
  /// @archtrace 633881
  /// @design 644982
  void release() noexcept
  {
    return this->block_slist_base<>::release(m_upstream_rsrc);
  }
  /// @brief The upstream_resource method.
  /// @returns Returns the memory_resource.
  /// @archtrace 633881
  /// @design 644983
  memory_resource& upstream_resource() const noexcept
  {
    return m_upstream_rsrc;
  }
};

} // namespace internal
} // namespace pmr
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_BLOCK_SLIST_H_
