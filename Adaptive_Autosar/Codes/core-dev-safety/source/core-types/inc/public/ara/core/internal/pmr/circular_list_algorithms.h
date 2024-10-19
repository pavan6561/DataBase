// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : circular_list_algorithms.h
//
// Purpose     : Provides the basic algorithms to manipulate nodes forming a circular doubly linked list.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_CIRCULAR_LIST_ALGORITHMS_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_CIRCULAR_LIST_ALGORITHMS_H_

#include <cstddef>

//! circular_list_algorithms provides basic algorithms to manipulate nodes
//! forming a circular doubly linked list. An empty circular list is formed by a node
//! whose pointers point to itself.
//!
//! circular_list_algorithms is configured with a NodeTraits class, which encapsulates the
//! information about the node to be manipulated. NodeTraits must support the
//! following interface:
//!
//! <b>Typedefs</b>:
//!
//! <tt>node</tt>: The type of the node that forms the circular list
//!
//! <tt>node_ptr</tt>: A pointer to a node
//!
//! <tt>const_node_ptr</tt>: A pointer to a const node
//!
//! <b>Static functions</b>:
//!
//! <tt>static node_ptr get_previous(const_node_ptr n);</tt>
//!
//! <tt>static void set_previous(node_ptr n, node_ptr prev);</tt>
//!
//! <tt>static node_ptr get_next(const_node_ptr n);</tt>
//!
//! <tt>static void set_next(node_ptr n, node_ptr next);</tt>

namespace ara
{
namespace core
{
namespace pmr
{
namespace internal
{
/// @brief The circular_list_algorithms is a class.
/// @tparam NodeTraits The type of class.
/// @archtrace 633881
/// @design 644995
template <class NodeTraits>
class circular_list_algorithms
{
 public:
  typedef typename NodeTraits::node node;
  typedef typename NodeTraits::node_ptr node_ptr;
  typedef typename NodeTraits::const_node_ptr const_node_ptr;
  typedef NodeTraits node_traits;

  /// @brief The init method.
  /// @param[in] this_node The node pointer.
  /// @archtrace 633881
  /// @design 644996
  __always_inline static void init(const node_ptr& this_node)
  {
    const node_ptr null_node((node_ptr()));
    NodeTraits::set_next(this_node, null_node);
    NodeTraits::set_previous(this_node, null_node);
  }

  /// @brief The inited method.
  /// @param[in] this_node The const node pointer.
  /// @returns Returns the true or false.
  /// @archtrace 633881
  /// @design 644997
  __always_inline static bool inited(const const_node_ptr& this_node)
  {
    return !NodeTraits::get_next(this_node);
  }

  /// @brief The init header method.
  /// @param[in] this_node The node pointer.
  /// @archtrace 633881
  /// @design 644998
  __always_inline static void init_header(const node_ptr& this_node)
  {
    NodeTraits::set_next(this_node, this_node);
    NodeTraits::set_previous(this_node, this_node);
  }

  /// @brief The init unique method.
  /// @param[in] this_node The const node pointer.
  /// @returns Returns the true or false.
  /// @archtrace 633881
  /// @design 644999
  __always_inline static bool unique(const const_node_ptr& this_node)
  {
    node_ptr next = NodeTraits::get_next(this_node);
    return !next || next == this_node;
  }

  /// @brief The count the this nodes.
  /// @param[in] this_node The const node pointer.
  /// @returns Returns the size of count.
  /// @archtrace 633881
  /// @design 645000
  static std::size_t count(const const_node_ptr& this_node)
  {
    std::size_t result = 0;
    const_node_ptr p = this_node;
    do
    {
      p = NodeTraits::get_next(p);
      ++result;
    } while (p != this_node);
    return result;
  }

  /// @brief The unlink the this nodes.
  /// @param[in] this_node The node pointer.
  /// @returns Returns the node pointer.
  /// @archtrace 633881
  /// @design 645001
  __always_inline static node_ptr unlink(const node_ptr& this_node)
  {
    node_ptr next(NodeTraits::get_next(this_node));
    node_ptr prev(NodeTraits::get_previous(this_node));
    NodeTraits::set_next(prev, next);
    NodeTraits::set_previous(next, prev);
    return next;
  }

  /// @brief The unlink the node pointers.
  /// @param[in] b The node pointer.
  /// @param[in] e The node pointer.
  /// @archtrace 633881
  /// @design 645002
  __always_inline static void unlink(const node_ptr& b, const node_ptr& e)
  {
    if (b != e)
    {
      node_ptr prevb(NodeTraits::get_previous(b));
      NodeTraits::set_previous(e, prevb);
      NodeTraits::set_next(prevb, e);
    }
  }

  /// @brief The link before the node pointers.
  /// @param[in] nxt_node The node pointer.
  /// @param[in] this_node The node pointer.
  /// @archtrace 633881
  /// @design 645003
  __always_inline static void link_before(const node_ptr& nxt_node, const node_ptr& this_node)
  {
    node_ptr prev(NodeTraits::get_previous(nxt_node));
    NodeTraits::set_previous(this_node, prev);
    NodeTraits::set_next(this_node, nxt_node);
    // nxt_node might be an alias for prev->next_
    // so use it before NodeTraits::set_next(prev, ...)
    // is called and the reference changes its value
    NodeTraits::set_previous(nxt_node, this_node);
    NodeTraits::set_next(prev, this_node);
  }

  /// @brief The link after the node pointers.
  /// @param[in] prev_node The node pointer.
  /// @param[in] this_node The node pointer.
  /// @archtrace 633881
  /// @design 645004
  __always_inline static void link_after(const node_ptr& prev_node, const node_ptr& this_node)
  {
    node_ptr next(NodeTraits::get_next(prev_node));
    NodeTraits::set_previous(this_node, prev_node);
    NodeTraits::set_next(this_node, next);
    // prev_node might be an alias for next->next_
    // so use it before update it before NodeTraits::set_previous(next, ...)
    // is called and the reference changes it's value
    NodeTraits::set_next(prev_node, this_node);
    NodeTraits::set_previous(next, this_node);
  }

  /// @brief The swap the node pointers.
  /// @param[in] this_node The node pointer.
  /// @param[in] other_node The node pointer.
  /// @archtrace 633881
  /// @design 645005
  static void swap_nodes(const node_ptr& this_node, const node_ptr& other_node)
  {
    if (other_node == this_node)
      return;
    bool this_inited = inited(this_node);
    bool other_inited = inited(other_node);
    if (this_inited)
    {
      init_header(this_node);
    }
    if (other_inited)
    {
      init_header(other_node);
    }

    node_ptr next_this(NodeTraits::get_next(this_node));
    node_ptr prev_this(NodeTraits::get_previous(this_node));
    node_ptr next_other(NodeTraits::get_next(other_node));
    node_ptr prev_other(NodeTraits::get_previous(other_node));
    // these first two swaps must happen before the other two
    swap_prev(next_this, next_other);
    swap_next(prev_this, prev_other);
    swap_next(this_node, other_node);
    swap_prev(this_node, other_node);

    if (this_inited)
    {
      init(other_node);
    }
    if (other_inited)
    {
      init(this_node);
    }
  }

  /// @brief The transfer the node pointers.
  /// @param[in] p The node pointer.
  /// @param[in] b The node pointer.
  /// @param[in] e The node pointer.
  /// @archtrace 633881
  /// @design 645006
  static void transfer(const node_ptr& p, const node_ptr& b, const node_ptr& e)
  {
    if (b != e)
    {
      node_ptr prev_p(NodeTraits::get_previous(p));
      node_ptr prev_b(NodeTraits::get_previous(b));
      node_ptr prev_e(NodeTraits::get_previous(e));
      NodeTraits::set_next(prev_e, p);
      NodeTraits::set_previous(p, prev_e);
      NodeTraits::set_next(prev_b, e);
      NodeTraits::set_previous(e, prev_b);
      NodeTraits::set_next(prev_p, b);
      NodeTraits::set_previous(b, prev_p);
    }
  }

  /// @brief The transfer the node pointers.
  /// @param[in] p The node pointer.
  /// @param[in] i The node pointer.
  /// @archtrace 633881
  /// @design 645007
  static void transfer(const node_ptr& p, const node_ptr& i)
  {
    node_ptr n(NodeTraits::get_next(i));
    if (n != p && i != p)
    {
      node_ptr prev_p(NodeTraits::get_previous(p));
      node_ptr prev_i(NodeTraits::get_previous(i));
      NodeTraits::set_next(prev_p, i);
      NodeTraits::set_previous(i, prev_p);
      NodeTraits::set_next(i, p);
      NodeTraits::set_previous(p, i);
      NodeTraits::set_previous(n, prev_i);
      NodeTraits::set_next(prev_i, n);
    }
  }

  /// @brief The reverse the node pointer.
  /// @param[in] p The node pointer.
  /// @archtrace 633881
  /// @design 645008
  static void reverse(const node_ptr& p)
  {
    node_ptr f(NodeTraits::get_next(p));
    node_ptr i(NodeTraits::get_next(f)), e(p);

    while (i != e)
    {
      node_ptr n = i;
      i = NodeTraits::get_next(i);
      transfer(f, n, i);
      f = n;
    }
  }

  /// @brief The move backwards the node pointers.
  /// @param[in] p The node pointer.
  /// @param[in] n The size node pointer.
  /// @archtrace 633881
  /// @design 645009
  static void move_backwards(const node_ptr& p, std::size_t n)
  {
    // Null shift, nothing to do
    if (!n)
      return;
    node_ptr first = NodeTraits::get_next(p);
    // size() == 0 or 1, nothing to do
    if (first == NodeTraits::get_previous(p))
      return;
    unlink(p);
    // Now get the new first node
    while (n--)
    {
      first = NodeTraits::get_next(first);
    }
    link_before(first, p);
  }

  /// @brief The move forward the node pointers.
  /// @param[in] p The node pointer.
  /// @param[in] n The size node pointer.
  /// @archtrace 633881
  /// @design 645010
  static void move_forward(const node_ptr& p, std::size_t n)
  {
    // Null shift, nothing to do
    if (!n)
      return;
    node_ptr last = NodeTraits::get_previous(p);
    // size() == 0 or 1, nothing to do
    if (last == NodeTraits::get_next(p))
      return;

    unlink(p);
    // Now get the new last node
    while (n--)
    {
      last = NodeTraits::get_previous(last);
    }
    link_after(last, p);
  }

  /// @brief The distance of the node pointers.
  /// @param[in] f The const node pointer.
  /// @param[in] l The const node pointer.
  /// @archtrace 633881
  /// @design 645011
  static std::size_t distance(const const_node_ptr& f, const const_node_ptr& l)
  {
    const_node_ptr i(f);
    std::size_t result = 0;
    while (i != l)
    {
      i = NodeTraits::get_next(i);
      ++result;
    }
    return result;
  }

  struct stable_partition_info
  {
    std::size_t num_1st_partition;
    std::size_t num_2nd_partition;
    node_ptr beg_2st_partition;
  };

  /// @brief The stable partition.
  /// @tparam Pred The type of class.
  /// @param[in] beg The node pointer.
  /// @param[in] end The node pointer.
  /// @param[in] pred The pred data type.
  /// @param[in] info The stable partition info.
  /// @archtrace 633881
  /// @design 645012
  template <class Pred>
  static void stable_partition(node_ptr beg, const node_ptr& end, Pred pred, stable_partition_info& info)
  {
    node_ptr bcur = node_traits::get_previous(beg);
    node_ptr cur = beg;
    node_ptr new_f = end;

    std::size_t num1 = 0, num2 = 0;
    while (cur != end)
    {
      if (pred(cur))
      {
        ++num1;
        bcur = cur;
        cur = node_traits::get_next(cur);
      }
      else
      {
        ++num2;
        node_ptr last_to_remove = bcur;
        new_f = cur;
        bcur = cur;
        cur = node_traits::get_next(cur);
        try
        {
          // Main loop
          while (cur != end)
          {
            if (pred(cur))
            {
              // Might throw
              ++num1;
              // Process current node
              node_traits::set_next(last_to_remove, cur);
              node_traits::set_previous(cur, last_to_remove);
              last_to_remove = cur;
              node_ptr nxt = node_traits::get_next(cur);
              node_traits::set_next(bcur, nxt);
              node_traits::set_previous(nxt, bcur);
              cur = nxt;
            }
            else
            {
              ++num2;
              bcur = cur;
              cur = node_traits::get_next(cur);
            }
          }
        }
        catch (...)
        {
          node_traits::set_next(last_to_remove, new_f);
          node_traits::set_previous(new_f, last_to_remove);
          throw;
        }
        node_traits::set_next(last_to_remove, new_f);
        node_traits::set_previous(new_f, last_to_remove);
        break;
      }
    }
    info.num_1st_partition = num1;
    info.num_2nd_partition = num2;
    info.beg_2st_partition = new_f;
  }

 private:
  /// @brief The swap previous nodes.
  /// @param[in] this_node The const node pointer.
  /// @param[in] other_node The const node pointer.
  /// @archtrace 633881
  /// @design 645013
  __always_inline static void swap_prev(const node_ptr& this_node, const node_ptr& other_node)
  {
    node_ptr temp(NodeTraits::get_previous(this_node));
    NodeTraits::set_previous(this_node, NodeTraits::get_previous(other_node));
    NodeTraits::set_previous(other_node, temp);
  }
  /// @brief The swap next nodes.
  /// @param[in] this_node The const node pointer.
  /// @param[in] other_node The const node pointer.
  /// @archtrace 633881
  /// @design 645014
  __always_inline static void swap_next(const node_ptr& this_node, const node_ptr& other_node)
  {
    node_ptr temp(NodeTraits::get_next(this_node));
    NodeTraits::set_next(this_node, NodeTraits::get_next(other_node));
    NodeTraits::set_next(other_node, temp);
  }
};
} // namespace internal
} // namespace pmr
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_CIRCULAR_LIST_ALGORITHMS_H_
