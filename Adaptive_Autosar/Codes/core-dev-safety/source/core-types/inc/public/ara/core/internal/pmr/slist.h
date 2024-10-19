// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : slist.h
//
// Purpose     : Contains the declarations of slits class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_SLIST_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_SLIST_H_

#include "ara/core/polymorphic_allocator.h"

#include <algorithm>
#include <cassert>

namespace ara
{
namespace core
{
namespace pmr
{
namespace internal
{
template <typename Tp>
class slist;

namespace slist_details
{
template <typename Tp>
struct node;

template <typename Tp>
struct node_base
{
  node<Tp>* m_next;

  node_base() : m_next(nullptr)
  {
  }
  node_base(const node_base&) = delete;
  node_base operator=(const node_base&) = delete;
};

template <typename Tp>
struct node : node_base<Tp>
{
  union {
    // By putting value into a union, constructor invocation is
    // suppressed, leaving raw bytes that are correctly aligned.
    Tp m_value;
  };
};

/// @brief The const_iterator is a class.
/// @tparam Tp The type of class.
/// @archtrace 633881
/// @design 644769
template <typename Tp>
class const_iterator
{
 public:
  using value_type = Tp;
  using pointer = Tp const*;
  using reference = Tp const&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  /// @brief The reference operator.
  /// @returns Returns a reference.
  /// @archtrace 633881
  /// @design 644770
  reference operator*() const
  {
    return m_prev->m_next->m_value;
  }
  /// @brief THe pointer operator.
  /// @returns Returns a pointer.
  /// @archtrace 633881
  /// @design 644771
  pointer operator->() const
  {
    return std::addressof(m_prev->m_next->m_value);
  }
  /// @brief The increment operator.
  /// @returns Returns a const_iterator object.
  /// @archtrace 633881
  /// @design 644772
  const_iterator& operator++()
  {
    m_prev = m_prev->m_next;
    return *this;
  }
  /// @brief The increment operator.
  /// @param[in] int The integer data type.
  /// @returns Returns a const_iterator.
  /// @archtrace 633881
  /// @design 644773
  const_iterator operator++(int)
  {
    const_iterator tmp(*this);
    ++*this;
    return tmp;
  }
  /// @brief The equal operator.
  /// @param[in] other The const_iterator to an object.
  /// @returns Returns a true or false.
  /// @archtrace 633881
  /// @design 644774
  bool operator==(const_iterator other) const
  {
    return m_prev == other.m_prev;
  }
  /// @brief The not equal operator.
  /// @param[in] other The const_iterator to an object.
  /// @returns Returns a true or false.
  /// @archtrace 633881
  /// @design 644775
  bool operator!=(const_iterator other) const
  {
    return !operator==(other);
  }

 protected:
  friend class slist<Tp>;
  // pointer to node before current element
  node_base<Tp>* m_prev;
  /// @brief The parameter constructor of const_iterator object.
  /// @tparam Tp The type of class.
  /// @param[in] prev A memory_resource<Tp> to an object
  /// @archtrace 633881
  /// @design 644776
  explicit const_iterator(const node_base<Tp>* prev) : m_prev(const_cast<node_base<Tp>*>(prev))
  {
  }
};

/// @brief The iterator is a class.
/// @tparam Tp The type of class.
/// @archtrace 633881
/// @design 644778
template <typename Tp>
class iterator : public const_iterator<Tp>
{
  using Base = const_iterator<Tp>;

 public:
  using pointer = Tp*;
  using reference = Tp&;
  /// @brief The reference operator.
  /// @returns Returns a reference.
  /// @archtrace 633881
  /// @design 644813
  reference operator*() const
  {
    return this->m_prev->m_next->m_value;
  }
  /// @brief THe pointer operator.
  /// @returns Returns a pointer.
  /// @archtrace 633881
  /// @design 644814
  pointer operator->() const
  {
    return std::addressof(this->m_prev->m_next->m_value);
  }
  /// @brief The increment operator.
  /// @returns Returns a const_iterator object.
  /// @archtrace 633881
  /// @design 644815
  iterator& operator++()
  {
    Base::operator++();
    return *this;
  }
  /// @brief The increment operator.
  /// @param[in] int The integer data type.
  /// @returns Returns a const_iterator.
  /// @archtrace 633881
  /// @design 644816
  iterator operator++(int)
  {
    iterator tmp(*this);
    ++*this;
    return tmp;
  }

 private:
  /// @brief The parameter constructor of iterator object.
  /// @tparam Tp The type of class.
  /// @param[in] prev A memory_resource<Tp> to an object
  /// @archtrace 633881
  /// @design 644912
  friend class slist<Tp>;
  explicit iterator(node_base<Tp>* prev) : const_iterator<Tp>(prev)
  {
  }
};

} // namespace slist_details

/// @brief The Singly-linked list that supports the use of a polymorphic allocator.
/// @tparam Tp The type of class.
/// @archtrace 633881
/// @design 644928
template <typename Tp>
class slist
{
 public:
  using value_type = Tp;
  using reference = value_type&;
  using const_reference = value_type const&;
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using allocator_type = ara::core::pmr::polymorphic_allocator<byte>;
  using iterator = slist_details::iterator<Tp>;
  using const_iterator = slist_details::const_iterator<Tp>;
  /// @brief The parameter constructor of slist object.
  /// @param[in] a The polymorphic_allocator class object.
  /// @archtrace 633881
  /// @design 644944
  slist(allocator_type a = {}) : m_head(), m_tail_p(&m_head), m_size(0), m_allocator(a)
  {
  }
  /// @brief The parameter constructor of slist object.
  /// @param[in] other A slist to an object
  /// @param[in] a The polymorphic_allocator class object.
  /// @archtrace 633881
  /// @design 644946
  slist(const slist& other, allocator_type a = {});
  /// @brief The move constructor of slist.
  /// @param[in] other A slist to an object
  /// @archtrace 633881
  /// @design 644947
  slist(slist&& other);
  /// @brief The move constructor of slist.
  /// @param[in] other A slist to an object
  /// @param[in] a The polymorphic_allocator class object.
  /// @archtrace 633881
  /// @design 644945
  slist(slist&& other, allocator_type a);
  /// @brief The destructor for slist object.
  /// @archtrace 633881
  /// @design 644948
  ~slist();
  /// @brief The Copy assignment operator.
  /// @param[in] other The const slist to an object.
  /// @returns Returns a object of the slist.
  /// @archtrace 633881
  /// @design 644949
  slist& operator=(const slist& other);
  /// @brief The Copy assignment operator.
  /// @param[in] other The slist to an object.
  /// @returns Returns a object of the slist.
  /// @archtrace 633881
  /// @design 644950
  slist& operator=(slist&& other);
  /// @brief The Swap is collections class to interchange the values present at different indexes in the list.
  /// @param[in] other The slist to an object.
  /// @archtrace 633881
  /// @design 644951
  void swap(slist& other) noexcept;
  /// @brief The size is used to calculate the number of objects in an slist.
  /// @returns Returns a size of the slist.
  /// @archtrace 633881
  /// @design 644952
  size_t size() const noexcept
  {
    return m_size;
  }
  /// @brief The empty is used to checks whether a slist is empty or not.
  /// @returns Returns a true or false.
  /// @archtrace 633881
  /// @design 644953
  bool empty() const noexcept
  {
    return 0 == m_size;
  }
  /// @brief The begin is used to sets the starting point for the code fragment you want to mark as a test case.
  /// @returns Returns a iterator object.
  /// @archtrace 633881
  /// @design 644954
  iterator begin()
  {
    return iterator(&m_head);
  }
  /// @brief The end is used to reverts the most recent destructive operation, changing the set of matched elements to its previous state right before the destructive operation.
  /// @returns Returns a iterator object.
  /// @archtrace 633881
  /// @design 644955
  iterator end()
  {
    return iterator(m_tail_p);
  }
  /// @brief The begin is used to sets the starting point for the code fragment you want to mark as a test case.
  /// @returns Returns a const iterator object.
  /// @archtrace 633881
  /// @design 644956
  const_iterator begin() const
  {
    return const_iterator(&m_head);
  }
  /// @brief The end is used to reverts the most recent destructive operation, changing the set of matched elements to its previous state right before the destructive operation.
  /// @returns Returns a const iterator object.
  /// @archtrace 633881
  /// @design 644957
  const_iterator end() const
  {
    return const_iterator(m_tail_p);
  }

  /// @brief The cbegin is used to returns a constant random access iterator which points to the beginning of the vector.
  /// @returns Returns a const iterator object.
  /// @archtrace 633881
  /// @design 644958
  const_iterator cbegin() const
  {
    return const_iterator(&m_head);
  }
  /// @brief The cend is used to returns a constant random access iterator which points to past-the-end element of the vector.
  /// @returns Returns a const iterator object.
  /// @archtrace 633881
  /// @design 644959
  const_iterator cend() const
  {
    return const_iterator(m_tail_p);
  }

  /// @brief The front is used to  returns the item at the front of the queue.
  /// @returns Returns a Tp object.
  /// @archtrace 633881
  /// @design 644960
  Tp& front()
  {
    return m_head.m_next->m_value;
  }
  /// @brief The front is used to  returns the item at the front of the queue.
  /// @returns Returns a const Tp object.
  /// @archtrace 633881
  /// @design 644961
  Tp const& front() const
  {
    return m_head.m_next->m_value;
  }
  /// @brief The emplace is used to extend the set container by inserting new elements into the container.
  /// @tparam Args The type of class.
  /// @param[in] i The iterator to an object.
  /// @param[in] args The Args to an object.
  /// @returns Returns a iterator object.
  /// @archtrace 633881
  /// @design 644962
  template <typename... Args>
  iterator emplace(iterator i, Args&&... args);
  /// @brief The emplace_front is used to adds a new element in the beginning of the list and the size of the container is increased by one.
  /// @tparam Args The type of class.
  /// @param[in] args The Args to an object.
  /// @archtrace 633881
  /// @design 644963
  template <typename... Args>
  void emplace_front(Args&&... args)
  {
    emplace(begin(), std::forward<Args>(args)...);
  }
  /// @brief The emplace_back is used to inserts a new element at the end of the container, right after its current last element.
  /// @tparam Args The type of class.
  /// @param[in] args The Args to an object.
  /// @archtrace 633881
  /// @design 644964
  template <typename... Args>
  void emplace_back(Args&&... args)
  {
    emplace(end(), std::forward<Args>(args)...);
  }
  /// @brief The insert is used to inserts the specified value at the specified position.
  /// @param[in] i The iterator to an object.
  /// @param[in] v The Tp to an object.
  /// @returns Returns a iterator object.
  /// @archtrace 633881
  /// @design 644965
  iterator insert(iterator i, const Tp& v)
  {
    return emplace(i, v);
  }
  /// @brief The push_front is used to push/insert the element in the list container at the front i.e. at the beginning.
  /// @param[in] v The Tp to an object.
  /// @archtrace 633881
  /// @design 644966
  void push_front(const Tp& v)
  {
    emplace(begin(), v);
  }
  /// @brief The push_back is used to  a pre-defined function that is used to insert data or elements at the end of the vector or it pushes the element in the vector from the back.
  /// @param[in] v The Tp to an object.
  /// @archtrace 633881
  /// @design 644967
  void push_back(const Tp& v)
  {
    emplace(end(), v);
  }

  /// @brief The erase is used to deletes all of the graphics and annotations within a graphic window, but does not destroy the window itself.
  /// @param[in] b The Tp to an object.
  /// @param[in] e The Tp to an object.
  /// @returns Returns a iterator object.
  /// @archtrace 633881
  /// @design 644968
  iterator erase(iterator b, iterator e);
  /// @brief The erase is used to deletes all of the graphics and annotations within a graphic window, but does not destroy the window itself.
  /// @param[in] i The Tp to an object.
  /// @returns Returns a iterator object.
  /// @archtrace 633881
  /// @design 644969
  iterator erase(iterator i)
  {
    iterator e = i;
    return erase(i, ++e);
  }
  /// @brief The pop_front is used to pop (delete) the element from the beginning of the list container.
  /// @archtrace 633881
  /// @design 644970
  void pop_front()
  {
    erase(begin());
  }
  /// @brief The get_allocator is used to  used to return the copy of allocator object which helps to construct the set container.
  /// @returns Returns a polymorphic_allocator object.
  /// @archtrace 633881
  /// @design 644971
  allocator_type get_allocator() const
  {
    return m_allocator;
  }

 private:
  using node_base = slist_details::node_base<Tp>;
  using node = slist_details::node<Tp>;

  node_base m_head;
  node_base* m_tail_p;
  size_t m_size;
  allocator_type m_allocator;
};

/// @brief Swaps the values a and b.
/// @tparam Tp The type of class.
/// @param[in] a The Tp to an object.
/// @param[in] b The Tp to an object.
/// @archtrace 633881
template <class Tp>
inline void swap(slist<Tp>& a, slist<Tp>& b) noexcept
{
  a.swap(b);
}

/// @brief Compare the values a and b for equality.
/// @tparam Tp The type of class.
/// @param[in] a The Tp to an object.
/// @param[in] b The Tp to an object.
/// @returns True if the two instances compare equal, false otherwise.
/// @archtrace 633881
template <class Tp>
inline bool operator==(const slist<Tp>& a, const slist<Tp>& b)
{
  if (a.size() != b.size())
    return false;
  else
    return std::equal(a.begin(), a.end(), b.begin());
}

/// @brief Compare the values a and b for inequality.
/// @tparam Tp The type of class.
/// @param[in] a The Tp to an object.
/// @param[in] b The Tp to an object.
/// @returns True if the two instances compare unequal, false otherwise.
/// @archtrace 633881
template <class Tp>
inline bool operator!=(const slist<Tp>& a, const slist<Tp>& b)
{
  return !(a == b);
}

template <typename Tp>
slist<Tp>::slist(const slist& other, allocator_type a) : slist(a)
{
  operator=(other);
}

template <typename Tp>
slist<Tp>::slist(slist&& other) : slist(other.get_allocator())
{
  operator=(std::move(other));
}

template <typename Tp>
slist<Tp>::slist(slist&& other, allocator_type a) : slist(a)
{
  operator=(std::move(other));
}

template <typename Tp>
slist<Tp>::~slist()
{
  erase(begin(), end());
}

template <typename Tp>
slist<Tp>& slist<Tp>::operator=(const slist& other)
{
  if (&other == this)
    return *this;
  erase(begin(), end());
  for (const Tp& v : other)
    push_back(v);
  return *this;
}

template <typename Tp>
slist<Tp>& slist<Tp>::operator=(slist&& other)
{
  if (&other == this)
    return *this;
  if (m_allocator == other.m_allocator)
  {
    erase(begin(), end());
    swap(other);
  }
  else
    // Copy assign
    operator=(other);

  return *this;
}

template <typename Tp>
void slist<Tp>::swap(slist& other) noexcept
{
  assert(m_allocator == other.m_allocator);
  node_base* new_tail = other.empty() ? &m_head : other.m_tail_p;
  node_base* new_other_tail = empty() ? &other.m_head : m_tail_p;
  std::swap(m_head.m_next, other.m_head.m_next);
  std::swap(m_size, other.m_size);
  m_tail_p = new_tail;
  other.m_tail_p = new_other_tail;
}

template <typename Tp>
template <typename... Args>
typename slist<Tp>::iterator slist<Tp>::emplace(iterator i, Args&&... args)
{
  node* new_node = static_cast<node*>(m_allocator.resource()->allocate(sizeof(node), alignof(node)));
  try
  {
    m_allocator.construct(std::addressof(new_node->m_value), std::forward<Args>(args)...);
  }
  catch (...)
  {
    // Recover resources if exception on constructor call.
    m_allocator.resource()->deallocate(new_node, sizeof(node), alignof(node));
    throw;
  }

  new_node->m_next = i.m_prev->m_next;
  i.m_prev->m_next = new_node;
  if (i.m_prev == m_tail_p)
    // Added at end
    m_tail_p = new_node;
  ++m_size;
  return i;
}

template <typename Tp>
typename slist<Tp>::iterator slist<Tp>::erase(iterator b, iterator e)
{
  node* erase_next = b.m_prev->m_next;
  // one past last erasure
  node* erase_past = e.m_prev->m_next;
  if (nullptr == erase_past)
    // Erasing at tail
    m_tail_p = b.m_prev;
  // splice out sublist
  b.m_prev->m_next = erase_past;
  while (erase_next != erase_past)
  {
    node* old_node = erase_next;
    erase_next = erase_next->m_next;
    --m_size;
    m_allocator.destroy(std::addressof(old_node->m_value));
    m_allocator.resource()->deallocate(old_node, sizeof(node), alignof(node));
  }

  return b;
}
} // namespace internal
} // namespace pmr
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_PMR_SLIST_H_
