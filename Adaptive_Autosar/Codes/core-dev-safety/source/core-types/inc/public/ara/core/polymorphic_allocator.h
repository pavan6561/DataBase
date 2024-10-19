// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : polymorphic_allocator.h
//
// Purpose     : This file contains the polymorphic allocatorclass declaration.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_POLYMORPHIC_ALLOCATOR_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_POLYMORPHIC_ALLOCATOR_H_

#include <atomic>
#include <memory>
#include <new>
#include <scoped_allocator>
#include <cstddef> // For max_align_t
#include <iostream>

// The `byte` type is defined exactly this way in C++17's `<cstddef>` (section
// [cstddef.syn]).  It is defined here to allow use of
// `polymorphic_allocator<byte>` as a vocabulary type.
enum class byte : unsigned char
{
};
namespace ara
{
namespace core
{
namespace pmr
{
/// @brief The memory_resource is an abstract interface to an unbounded set of classes encapsulating
/// memory resources.
/// @archtrace 633881
/// @design 634075
class memory_resource
{
  static std::atomic<memory_resource*> s_default_resource;

  friend memory_resource* set_default_resource(memory_resource*);
  friend memory_resource* get_default_resource();

 public:
  static constexpr size_t max_align = alignof(max_align_t);
  /// @brief The destructor for memory_resource objects.
  /// @archtrace 633881
  /// @design 645118
  virtual ~memory_resource();
  /// @brief Allocate size of bytes.
  /// @param[in] bytes The bytes of allocate.
  /// @param[in] alignment The size of allocate.
  /// @returns Returns the void ponter.
  /// @archtrace 633881
  /// @design 645119
  void* allocate(size_t bytes, size_t alignment = max_align)
  {
    return do_allocate(bytes, alignment);
  }
  /// @brief The Deallocate size of bytes.
  /// @param[in] p The allocate return type.
  /// @param[in] bytes The bytes of deallocate.
  /// @param[in] alignment The size of deallocate.
  /// @archtrace 633881
  /// @design 645120
  void deallocate(void* p, size_t bytes, size_t alignment = max_align)
  {
    return do_deallocate(p, bytes, alignment);
  }

  /// @brief Is equal to the memory resource object.
  /// @param[in] other The memory resource object.
  /// @returns Returns the true or false.
  /// @archtrace 633881
  /// @design 645121
  bool is_equal(const memory_resource& other) const noexcept
  {
    return do_is_equal(other);
  }

 protected:
  /// @brief The Allocates storage.
  /// @param[in] bytes The memory bytes.
  /// @param[in] alignment The memory alignment.
  /// @returns A pointer to allocated storage of at least bytes bytes in size.
  /// @design 634076
  virtual void* do_allocate(size_t bytes, size_t alignment) = 0;
  /// @brief The Deallocates storage.
  /// @param[in] p Deallocates the storage pointed to by p.
  /// @param[in] bytes The memory bytes.
  /// @param[in] alignment The memory alignment.
  /// @design 634077
  virtual void do_deallocate(void* p, size_t bytes, size_t alignment) = 0;
  /// @brief memory allocated using a memory_resource can only be deallocated using that same resource.
  /// @param[in] other The memory_resource of the object.
  /// @returns Returns the true or flase.
  /// @design 634078
  virtual bool do_is_equal(const memory_resource& other) const noexcept = 0;
};
/// @brief The operator equal.
/// @param[in] a The memory_resource object.
/// @param[in] b The memory_resource object.
/// @returns Returns the true or false.
/// @archtrace 633881
/// @design 645122
inline bool operator==(const memory_resource& a, const memory_resource& b)
{
  // Call `is_equal` rather than using address comparisons because some
  // polymorphic allocators are produced as a result of type erasure.  In
  // that case, `a` and `b` may contain `memory_resource`s with different
  // addresses which, nevertheless, should compare equal.
  return &a == &b || a.is_equal(b);
}
/// @brief The operator not equal.
/// @param[in] a The memory_resource object.
/// @param[in] b The memory_resource object.
/// @returns Returns the true or false.
/// @archtrace 63388
/// @design 645123
inline bool operator!=(const memory_resource& a, const memory_resource& b)
{
  return !(a == b);
}

namespace __details
{
// STL allocator that holds a pointer to a polymorphic allocator resource.
// Used to implement `polymorphic_allocator`, which is a scoped allocator.

/// @brief The polymorphic_allocator is an Allocator which exhibits different allocation behavior
/// depending upon the memory_resource from which it is constructed.
/// @tparam TP The type of class.
/// @archtrace 633881
/// @design 634079
template <class Tp>
class polymorphic_allocator_imp
{
  memory_resource* m_resource;

 public:
  using value_type = Tp;

  // These types are old-fashioned, pre-C++11 requirements, still needed by
  // g++'s `basic_string` implementation.
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = Tp&;
  using const_reference = Tp const&;
  using pointer = Tp*;
  using const_pointer = Tp const*;
  /// @brief The default constructor of polymorphic_allocator_imp object.
  /// @archtrace 633881
  /// @design 634099
  polymorphic_allocator_imp();
  /// @brief The parameter constructor of polymorphic_allocator_imp object.
  /// @param[in] r A memory_resource to an object
  /// @archtrace 633881
  /// @design 634100
  polymorphic_allocator_imp(memory_resource* r);
  /// @brief The parameter constructor of polymorphic_allocator object.
  /// @tparam U The type of class.
  /// @param[in] other A polymorphic allocator imp to an object
  /// @archtrace 633881
  /// @design 645128
  template <class U>
  polymorphic_allocator_imp(const polymorphic_allocator_imp<U>& other);
  /// @brief Allocate the size of bytes.
  /// @param[in] n The size of allocate.
  /// @returns Returns the object.
  /// @archtrace 633881
  /// @design 645129
  Tp* allocate(size_t n);
  /// @brief Deallocate the size of bytes.
  /// @param[in] p The type of class.
  /// @param[in] n The size of deallocate.
  /// @archtrace 633881
  /// @design 645130
  void deallocate(Tp* p, size_t n);

  /// @brief Create a new polymorphic_allocator for use by a container's copy constructor.
  /// @returns Returns the polymorphic allocator imp object.
  /// @archtrace 633881
  /// @design 645131
  polymorphic_allocator_imp select_on_container_copy_construction() const;
  /// @brief The resource method.
  /// @returns Returns the memory resource object.
  /// @archtrace 633881
  /// @design 645132
  memory_resource* resource() const;
};
/// @brief The operator equal.
/// @tparam T1 The type of class.
/// @tparam T2 The type of class.
/// @param[in] a The polymorphic_allocator_imp object.
/// @param[in] b The polymorphic_allocator_imp object.
/// @returns Returns the true or false.
/// @archtrace 633881
/// @design 645133
template <class T1, class T2>
bool operator==(const polymorphic_allocator_imp<T1>& a, const polymorphic_allocator_imp<T2>& b);
/// @brief The operator not equal.
/// @tparam T1 The type of class.
/// @tparam T2 The type of class.
/// @param[in] a The polymorphic_allocator_imp object.
/// @param[in] b The polymorphic_allocator_imp object.
/// @returns Returns the true or false.
/// @archtrace 633881
/// @design 645134
template <class T1, class T2>
bool operator!=(const polymorphic_allocator_imp<T1>& a, const polymorphic_allocator_imp<T2>& b);

template <size_t Align>
struct aligned_chunk;

template <>
struct aligned_chunk<1>
{
  char x;
};
template <>
struct aligned_chunk<2>
{
  short x;
};
template <>
struct aligned_chunk<4>
{
  int x;
};
template <>
struct aligned_chunk<8>
{
  long long x;
};
template <>
struct aligned_chunk<16>
{
  __attribute__((aligned(16))) char x;
};
template <>
struct aligned_chunk<32>
{
  __attribute__((aligned(32))) char x;
};
template <>
struct aligned_chunk<64>
{
  __attribute__((aligned(64))) char x;
};

/// @brief The Adaptor to make a polymorphic allocator resource type from an STL allocator type.
/// @tparam Allocator The type of class.
/// @archtrace 633881
/// @design 634071
template <class Allocator>
class resource_adaptor_imp : public memory_resource
{
  typename std::allocator_traits<Allocator>::template rebind_alloc<max_align_t> m_alloc;
  /// @brief Allocate the size of bytes.
  /// @tparam Allocator The type of size_t.
  /// @param[in] bytes The resource_adaptor_imp bytes.
  /// @return Return the void pointer.
  /// @archtrace 633881
  /// @design 645124
  template <size_t Align>
  void* allocate_imp(size_t bytes);
  /// @brief Deallocate the size of bytes.
  /// @tparam Align The type of size_t.
  /// @param[in] p The void pointer.
  /// @param[in] bytes The resource_adaptor_imp bytes.
  /// @archtrace 633881
  /// @design 645125
  template <size_t Align>
  void deallocate_imp(void* p, size_t bytes);

 public:
  typedef Allocator allocator_type;
  /// @brief The default constructor of resource_adaptor_imp object.
  /// @archtrace 633881
  resource_adaptor_imp() = default;
  /// @brief The default Copy constructor
  /// @archtrace 633881
  resource_adaptor_imp(const resource_adaptor_imp&) = default;
  /// @brief The prameter of resource_adaptor_imp constructor.
  /// @tparam Allocator2 The type of class.
  /// @param[in] a2 The Allocator2 object.
  /// @param[in] type The resource_adaptor_imp typename.
  /// @archtrace 633881
  /// @design 645126
  template <class Allocator2>
  resource_adaptor_imp(Allocator2&& a2,
      typename std::enable_if<std::is_convertible<Allocator2, Allocator>::value, int>::type = 0);

 protected:
  /// @brief The Allocates storage.
  /// @param[in] bytes The resource_adaptor bytes.
  /// @param[in] alignment The resource_adaptor alignment.
  /// @returns A pointer to allocated storage of at least bytes bytes in size.
  /// @design 634072
  void* do_allocate(size_t bytes, size_t alignment = 0) override;
  /// @brief The Deallocates storage.
  /// @param[in] p Deallocates the storage pointed to by p.
  /// @param[in] bytes The resource_adaptor bytes.
  /// @param[in] alignment The resource_adaptor alignment.
  /// @design 634073
  void do_deallocate(void* p, size_t bytes, size_t alignment = 0) override;
  /// @brief memory allocated using a resource_adaptor_imp can only be deallocated using that same resource.
  /// @param[in] other The memory_resource of the object.
  /// @returns Returns the true or flase.
  /// @design 634074
  bool do_is_equal(const memory_resource& other) const noexcept override;
  /// @brief memory get allocated using a resource_adaptor_imp .
  /// @returns Returns the allocator type object.
  /// @design 645127
  allocator_type get_allocator() const
  {
    return m_alloc;
  }
};

} // end namespace __details

/// @brief The A resource_adaptor converts a traditional STL allocator to a polymorphic memory resource.
/// @tparam Allocator The type of class.This alias ensures that `resource_adaptor<T>` and
/// resource_adaptor<U>` are always the same type, whether or not `T` and `U` are the same type.
/// @archtrace 633881
template <class Allocator>
using resource_adaptor
    = __details::resource_adaptor_imp<typename std::allocator_traits<Allocator>::template rebind_alloc<byte>>;

/// @brief The Memory resource that uses new and delete
/// @archtrace 633881
class new_delete_resource : public resource_adaptor<std::allocator<byte>>
{
};

/// @brief The singleton of new delete resource.
/// @returns Return a pointer to a global instance of new_delete_resource.
/// @archtrace 633881
/// @design 645154
new_delete_resource* new_delete_resource_singleton();

/// @brief The null memory resource.
/// @returns Returns A pointer to a static-duration object of a type derived from
/// memory_resource for which allocate() always throws bad_alloc and for which
/// deallocate() has no effect. The same value is returned every time this function
/// is called. For return value p and memory resource r, p->is_equal(r) returns &r == p.
/// @archtrace 633881
/// @design 645155
memory_resource* null_memory_resource() noexcept;

/// @brief The get default resource method.
/// @returns Returns the previous value of the default memory resource pointer.
/// @archtrace 633881
/// @design 645156
memory_resource* get_default_resource();

/// @brief The default memory resource pointer is used by certain facilities when an
/// explicit memory resource is not supplied. The initial default memory resource pointer is the
/// return value of std::pmr::new_delete_resource.
/// @param[in] r A memory_resource to an object.
/// @returns Returns the previous value of the default memory resource pointer.
/// @archtrace 633881
/// @design 645157
memory_resource* set_default_resource(memory_resource* r);

/// @brief The Null Memory resource implimentation
/// @archtrace 633881
struct null_memory_resource_imp : public memory_resource
{
 public:
  /// @brief The destructor for null_memory_resource_imp objects.
  /// @archtrace 633881
  ~null_memory_resource_imp() override
  {
  }
  /// @brief The Allocates storage.
  /// @param[in] bytes The null_memory_resource_imp bytes.
  /// @param[in] alignment The null_memory_resource_imp alignment.
  /// @returns A pointer to allocated storage of at least bytes bytes in size.
  void* do_allocate(std::size_t bytes, std::size_t alignment) override
  {
    (void)bytes;
    (void)alignment;
    throw std::bad_alloc();
  }
  /// @brief The Deallocates storage.
  /// @param[in] p Deallocates the storage pointed to by p.
  /// @param[in] bytes The null_memory_resource_imp bytes.
  /// @param[in] alignment The null_memory_resource_imp alignment.
  void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override
  {
    (void)p;
    (void)bytes;
    (void)alignment;
  }
  /// @brief memory allocated using a null_memory_resource_imp can only be deallocated using that same resource.
  /// @param[in] other The memory_resource of the object.
  /// @returns Returns the true or flase.
  bool do_is_equal(const memory_resource& other) const noexcept override
  {
    return &other == this;
  }
};

/// @brief The Adaptor to make a polymorphic allocator resource type from an STL allocator type.
/// @tparam Tp The type of class.
/// @archtrace 633881
/// @design 634054
template <class Tp>
class polymorphic_allocator : public std::scoped_allocator_adaptor<__details::polymorphic_allocator_imp<Tp>>
{
  typedef __details::polymorphic_allocator_imp<Tp> Imp;
  typedef std::scoped_allocator_adaptor<Imp> Base;

 public:
  // g++-4.6.3 does not use allocator_traits in shared_ptr, so we have to
  // provide an explicit rebind.
  template <typename U>
  struct rebind
  {
    typedef polymorphic_allocator<U> other;
  };
  /// @brief The default constructor of polymorphic_allocator object.
  /// @archtrace 633881
  /// @design 634101
  polymorphic_allocator() = default;
  /// @brief The parameter constructor of polymorphic_allocator object.
  /// @param[in] r A memory_resource to an object
  /// @archtrace 633881
  /// @design 634102
  polymorphic_allocator(memory_resource* r) : Base(Imp(r))
  {
  }
  /// @brief The parameter constructor of polymorphic_allocator object.
  /// @tparam U The type of class.
  /// @param[in] other A polymorphic allocator to an object
  /// @archtrace 633881
  /// @design 645148
  template <class U>
  polymorphic_allocator(const polymorphic_allocator<U>& other) : Base(Imp((other.resource())))
  {
  }
  /// @brief The parameter constructor of polymorphic_allocator object.
  /// @tparam U The type of class.
  /// @param[in] other A polymorphic allocator imp to an object.
  /// @archtrace 633881
  /// @design 645149
  template <class U>
  polymorphic_allocator(const __details::polymorphic_allocator_imp<U>& other) : Base(other)
  {
  }

  /// @brief Create a new polymorphic_allocator for use by a container's copy constructor.
  /// @returns Returns the polymorphic allocator object.
  /// @archtrace 633881
  /// @design 645150
  polymorphic_allocator select_on_container_copy_construction() const
  {
    return polymorphic_allocator();
  }
  /// @brief The resource method.
  /// @returns Returns the memory resource object.
  /// @archtrace 633881
  /// @design 645151
  memory_resource* resource() const
  {
    return this->outer_allocator().resource();
  }
};
/// @brief The operator equal.
/// @tparam T1 The type of class.
/// @tparam T2 The type of class.
/// @param[in] a The polymorphic_allocator_imp object.
/// @param[in] b The polymorphic_allocator_imp object.
/// @returns Returns the true or false.
/// @archtrace 633881
/// @design 645152
template <class T1, class T2>
inline bool operator==(const polymorphic_allocator<T1>& a, const polymorphic_allocator<T2>& b)
{
  return a.outer_allocator() == b.outer_allocator();
}
/// @brief The operator not equal.
/// @tparam T1 The type of class.
/// @tparam T2 The type of class.
/// @param[in] a The polymorphic_allocator_imp object.
/// @param[in] b The polymorphic_allocator_imp object.
/// @returns Returns the true or false.
/// @archtrace 633881
/// @design 645153
template <class T1, class T2>
inline bool operator!=(const polymorphic_allocator<T1>& a, const polymorphic_allocator<T2>& b)
{
  return !(a == b);
}

// INLINE AND TEMPLATE FUNCTION IMPLEMENTATIONS

inline memory_resource::~memory_resource()
{
}

inline memory_resource* get_default_resource()
{
  memory_resource* ret = memory_resource::s_default_resource.load();
  if (nullptr == ret)
    ret = new_delete_resource_singleton();
  return ret;
}

inline memory_resource* set_default_resource(memory_resource* r)
{
  if (nullptr == r)
    r = new_delete_resource_singleton();

  // TBD, should use an atomic swap
  memory_resource* prev = get_default_resource();
  memory_resource::s_default_resource.store(r);
  return prev;
}

template <class Allocator>
template <class Allocator2>
inline __details::resource_adaptor_imp<Allocator>::resource_adaptor_imp(
    Allocator2&& a2, typename std::enable_if<std::is_convertible<Allocator2, Allocator>::value, int>::type) :
    m_alloc(std::forward<Allocator2>(a2))
{
}

template <class Allocator>
template <size_t Align>
void* __details::resource_adaptor_imp<Allocator>::allocate_imp(size_t bytes)
{
  typedef __details::aligned_chunk<Align> chunk;
  size_t chunks = (bytes + Align - 1) / Align;

  typedef typename std::allocator_traits<Allocator>::template rebind_traits<chunk> chunk_traits;
  typename chunk_traits::allocator_type rebound(m_alloc);
  return chunk_traits::allocate(rebound, chunks);
}

template <class Allocator>
template <size_t Align>
void __details::resource_adaptor_imp<Allocator>::deallocate_imp(void* p, size_t bytes)
{
  typedef __details::aligned_chunk<Align> chunk;
  size_t chunks = (bytes + Align - 1) / Align;

  typedef typename std::allocator_traits<Allocator>::template rebind_traits<chunk> chunk_traits;
  typename chunk_traits::allocator_type rebound(m_alloc);
  return chunk_traits::deallocate(rebound, static_cast<chunk*>(p), chunks);
}

template <class Allocator>
void* __details::resource_adaptor_imp<Allocator>::do_allocate(size_t bytes, size_t alignment)
{
  static const size_t max_natural_alignment = sizeof(max_align_t);

  if (0 == alignment)
  {
    // Choose natural alignment for `bytes`
    alignment = ((bytes ^ (bytes - 1)) >> 1) + 1;
    if (alignment > max_natural_alignment)
      alignment = max_natural_alignment;
  }

  switch (alignment)
  {
  case 1:
    return allocate_imp<1>(bytes);
  case 2:
    return allocate_imp<2>(bytes);
  case 4:
    return allocate_imp<4>(bytes);
  case 8:
    return allocate_imp<8>(bytes);
  case 16:
    return allocate_imp<16>(bytes);
  case 32:
    return allocate_imp<32>(bytes);
  case 64:
    return allocate_imp<64>(bytes);
  default:
  {
    size_t chunks = (bytes + sizeof(void*) + alignment - 1) / 64;
    size_t chunkbytes = chunks * 64;
    void* original = allocate_imp<64>(chunkbytes);

    // Make room for original pointer storage
    char* p = static_cast<char*>(original) + sizeof(void*);

    // Round up to nearest alignment boundary
    p += alignment - 1;
    p -= (size_t(p)) & (alignment - 1);

    // Store original pointer in word before allocated pointer
    reinterpret_cast<void**>(p)[-1] = original;

    return p;
  }
  }
}

template <class Allocator>
void __details::resource_adaptor_imp<Allocator>::do_deallocate(void* p, size_t bytes, size_t alignment)
{
  static const size_t max_natural_alignment = sizeof(max_align_t);

  if (0 == alignment)
  {
    // Choose natural alignment for `bytes`
    alignment = ((bytes ^ (bytes - 1)) >> 1) + 1;
    if (alignment > max_natural_alignment)
      alignment = max_natural_alignment;
  }

  switch (alignment)
  {
  case 1:
    deallocate_imp<1>(p, bytes);
    break;
  case 2:
    deallocate_imp<2>(p, bytes);
    break;
  case 4:
    deallocate_imp<4>(p, bytes);
    break;
  case 8:
    deallocate_imp<8>(p, bytes);
    break;
  case 16:
    deallocate_imp<16>(p, bytes);
    break;
  case 32:
    deallocate_imp<32>(p, bytes);
    break;
  case 64:
    deallocate_imp<64>(p, bytes);
    break;
  default:
  {
    size_t chunks = (bytes + sizeof(void*) + alignment - 1) / 64;
    size_t chunkbytes = chunks * 64;
    void* original = reinterpret_cast<void**>(p)[-1];

    deallocate_imp<64>(original, chunkbytes);
  }
  }
}

template <class Allocator>
bool __details::resource_adaptor_imp<Allocator>::do_is_equal(const memory_resource& other) const noexcept
{
  const resource_adaptor_imp* other_p = dynamic_cast<const resource_adaptor_imp*>(&other);

  if (other_p)
    return this->m_alloc == other_p->m_alloc;
  else
    return false;
}

namespace __pmrd = __details;

template <class Tp>
inline __pmrd::polymorphic_allocator_imp<Tp>::polymorphic_allocator_imp() : m_resource(get_default_resource())
{
}

template <class Tp>
inline __pmrd::polymorphic_allocator_imp<Tp>::polymorphic_allocator_imp(memory_resource* r) :
    m_resource(r ? r : get_default_resource())
{
}

template <class Tp>
template <class U>
inline __pmrd::polymorphic_allocator_imp<Tp>::polymorphic_allocator_imp(
    const __pmrd::polymorphic_allocator_imp<U>& other) :
    m_resource(other.resource())
{
}

template <class Tp>
inline Tp* __pmrd::polymorphic_allocator_imp<Tp>::allocate(size_t n)
{
  return static_cast<Tp*>(m_resource->allocate(n * sizeof(Tp), alignof(Tp)));
}

template <class Tp>
inline void __pmrd::polymorphic_allocator_imp<Tp>::deallocate(Tp* p, size_t n)
{
  m_resource->deallocate(p, n * sizeof(Tp), alignof(Tp));
}

template <class Tp>
inline __pmrd::polymorphic_allocator_imp<Tp>
    __pmrd::polymorphic_allocator_imp<Tp>::select_on_container_copy_construction() const
{
  return __pmrd::polymorphic_allocator_imp<Tp>();
}

template <class Tp>
inline memory_resource* __pmrd::polymorphic_allocator_imp<Tp>::resource() const
{
  return m_resource;
}

template <class T1, class T2>
inline bool __pmrd::operator==(
    const __pmrd::polymorphic_allocator_imp<T1>& a, const __pmrd::polymorphic_allocator_imp<T2>& b)
{
  // `operator==` for `memory_resource` first checks for equality of
  // addresses and calls `is_equal` only if the addresses differ.  The call
  // `is_equal` because some polymorphic allocators are produced as a result
  // of type erasure.  In that case, `a` and `b` may contain
  // `memory_resource`s with different addresses which, nevertheless,
  // should compare equal.
  return *a.resource() == *b.resource();
}

template <class T1, class T2>
inline bool __pmrd::operator!=(
    const __pmrd::polymorphic_allocator_imp<T1>& a, const __pmrd::polymorphic_allocator_imp<T2>& b)
{
  return *a.resource() != *b.resource();
}

} // namespace pmr
} // namespace core
} // namespace ara
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_POLYMORPHIC_ALLOCATOR_H_
