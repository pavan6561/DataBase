// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_pmr.cpp
//
// Purpose     : This file provides the implementation of pmr test cases.
// -----------------------------------------------------------------------------------------------------------

#include "gtest/gtest.h"

#include "ara/core/internal/pmr/utilities.h"
#include "ara/core/monotonic_buffer_resource.h"
#include "ara/core/unsynchronized_pool_resource.h"
#include "ara/core/polymorphic_allocator.h"
#include "ara/core/internal/pmr/pool_resource.h"

#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "ara/core/array.h"
#include <unistd.h>
#include <cstring>

class debug_resource : public ara::core::pmr::memory_resource
{
 public:
  explicit debug_resource(ara::core::pmr::String name,
      ara::core::pmr::memory_resource* up = ara::core::pmr::get_default_resource()) :
      name_(name),
      upstream_(up)
  {
  }
  inline void* do_allocate(size_t bytes, size_t alignment) override
  {
    std::cout << name_ << " do_allocate(): " << bytes << '\n';
    void* ret = upstream_->allocate(bytes, alignment);
    return ret;
  }
  inline void do_deallocate(void* ptr, size_t bytes, size_t alignment) override
  {
    std::cout << name_ << " do_deallocate(): " << bytes << '\n';
    upstream_->deallocate(ptr, bytes, alignment);
  }
  inline bool do_is_equal(const ara::core::pmr::memory_resource& other) const noexcept override
  {
    return this == &other;
  }

 private:
  ara::core::pmr::memory_resource* upstream_ = ara::core::pmr::get_default_resource();
  ara::core::pmr::String name_;
};

template <typename Container, typename Memory, typename... Args>
auto pushElements(Memory& m, Args... args)
{
  Container c{&m};
  c.reserve(sizeof...(args));
  (c.push_back(args), ...);
  return c;
}

class test_resource : public ara::core::pmr::memory_resource
{
 public:
  explicit test_resource(ara::core::pmr::memory_resource* parent = ara::core::pmr::get_default_resource());
  ~test_resource();

  ara::core::pmr::memory_resource* parent() const;

  size_t bytes_allocated() const;
  size_t bytes_deallocated() const;
  size_t bytes_outstanding() const;
  size_t bytes_highwater() const;
  size_t blocks_outstanding() const;

  static size_t leaked_bytes();
  static size_t leaked_blocks();
  static void clear_leaked();

 protected:
  void* do_allocate(size_t bytes, size_t alignment) override;
  void do_deallocate(void* p, size_t bytes, size_t alignment) override;
  bool do_is_equal(const ara::core::pmr::memory_resource& other) const noexcept override;

 private:
  // Record holding the results of an allocation
  struct allocation_rec
  {
    void* m_ptr;
    size_t m_bytes;
    size_t m_alignment;
  };

  ara::core::pmr::memory_resource* m_parent;
  size_t m_bytes_allocated;
  size_t m_bytes_outstanding;
  size_t m_bytes_highwater;
  ara::core::pmr::Vector<allocation_rec> m_blocks;

  static size_t s_leaked_bytes;
  static size_t s_leaked_blocks;
};

// Keep track of number of bytes that would be leaked by
// allocator destructor.
size_t test_resource::s_leaked_bytes = 0;

// Keep track of number of blocks that would be leaked by
// allocator destructor.
size_t test_resource::s_leaked_blocks = 0;

test_resource::test_resource(ara::core::pmr::memory_resource* parent) :
    m_parent(parent), m_bytes_allocated(0), m_bytes_outstanding(0), m_bytes_highwater(0), m_blocks(parent)
// Vector memory comes from parent, too
{
}

test_resource::~test_resource()
{
  // If any blocks have not been released, report them as leaked.
  s_leaked_blocks += blocks_outstanding();

  // Reclaim blocks that would have been leaked.
  for (auto& alloc_rec : m_blocks)
  {
    s_leaked_bytes += alloc_rec.m_bytes;
    m_parent->deallocate(alloc_rec.m_ptr, alloc_rec.m_bytes, alloc_rec.m_alignment);
  }
}

ara::core::pmr::memory_resource* test_resource::parent() const
{
  return m_parent;
}

size_t test_resource::bytes_allocated() const
{
  return m_bytes_allocated;
}

size_t test_resource::bytes_deallocated() const
{
  return m_bytes_allocated - m_bytes_outstanding;
}

size_t test_resource::bytes_outstanding() const
{
  return m_bytes_outstanding;
}

size_t test_resource::bytes_highwater() const
{
  return m_bytes_highwater;
}

size_t test_resource::blocks_outstanding() const
{
  return m_blocks.size();
}

size_t test_resource::leaked_bytes()
{
  return s_leaked_bytes;
}

size_t test_resource::leaked_blocks()
{
  return s_leaked_blocks;
}

void test_resource::clear_leaked()
{
  s_leaked_bytes = 0;
  s_leaked_blocks = 0;
}

void* test_resource::do_allocate(size_t bytes, size_t alignment)
{
  void* ret = m_parent->allocate(bytes, alignment);
  m_blocks.push_back(allocation_rec{ret, bytes, alignment});
  m_bytes_allocated += bytes;
  m_bytes_outstanding += bytes;
  if (m_bytes_outstanding > m_bytes_highwater)
    m_bytes_highwater = m_bytes_outstanding;
  return ret;
}

void test_resource::do_deallocate(void* p, size_t bytes, size_t alignment)
{
  // Check that deallocation args exactly match allocation args.
  auto i = std::find_if(m_blocks.begin(), m_blocks.end(), [p](allocation_rec& r) { return r.m_ptr == p; });
  if (i == m_blocks.end())
    throw std::invalid_argument("deallocate: Invalid pointer");
  else if (i->m_bytes != bytes)
    throw std::invalid_argument("deallocate: Size mismatch");
  else if (i->m_alignment != alignment)
    throw std::invalid_argument("deallocate: Alignment mismatch");

  m_parent->deallocate(p, i->m_bytes, i->m_alignment);
  m_blocks.erase(i);
  m_bytes_outstanding -= bytes;
}

bool test_resource::do_is_equal(const ara::core::pmr::memory_resource& other) const noexcept
{
  return this == &other;
}

int testStatus = 0;

void aSsErT(int c, const char* s, int i)
{
  if (c)
  {
    std::cout << __FILE__ << ":" << i << ": error: " << s << "    (failed)" << std::endl;
    if (testStatus >= 0 && testStatus <= 100)
      ++testStatus;
  }
}

#define ASSERT(X) \
  { \
    aSsErT(!(X), #X, __LINE__); \
  }
// --------------------------------------------------------------------------
#define LOOP_ASSERT(I, X) \
  { \
    if (!(X)) \
    { \
      std::cout << #I << ": " << I << "\n"; \
      aSsErT(1, #X, __LINE__); \
    } \
  }

#define LOOP2_ASSERT(I, J, X) \
  { \
    if (!(X)) \
    { \
      std::cout << #I << ": " << I << "\t" << #J << ": " << J << "\n"; \
      aSsErT(1, #X, __LINE__); \
    } \
  }

#define LOOP3_ASSERT(I, J, K, X) \
  { \
    if (!(X)) \
    { \
      std::cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" << #K << ": " << K << "\n"; \
      aSsErT(1, #X, __LINE__); \
    } \
  }

#define LOOP4_ASSERT(I, J, K, L, X) \
  { \
    if (!(X)) \
    { \
      std::cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" << #K << ": " << K << "\t" << #L \
                << ": " << L << "\n"; \
      aSsErT(1, #X, __LINE__); \
    } \
  }

#define LOOP5_ASSERT(I, J, K, L, M, X) \
  { \
    if (!(X)) \
    { \
      std::cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" << #K << ": " << K << "\t" << #L \
                << ": " << L << "\t" << #M << ": " << M << "\n"; \
      aSsErT(1, #X, __LINE__); \
    } \
  }

#define LOOP6_ASSERT(I, J, K, L, M, N, X) \
  { \
    if (!(X)) \
    { \
      std::cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" << #K << ": " << K << "\t" << #L \
                << ": " << L << "\t" << #M << ": " << M << "\t" << #N << ": " << N << "\n"; \
      aSsErT(1, #X, __LINE__); \
    } \
  }

// Polymorphic Allocator Test
/*******************************************************************************
@TestCaseId:          UT_CORE_3030
@Test_Description:    To test the functionality of polymorphic allocator test resources.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:     634080, 634081, 634094, 634095, 634096
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, TestResource)
{
  std::cout << "Testing constructors\n";
  test_resource tr1;
  ASSERT(0 == tr1.bytes_allocated());
  ASSERT(0 == tr1.bytes_deallocated());
  ASSERT(0 == tr1.bytes_outstanding());
  ASSERT(0 == tr1.bytes_highwater());
  ASSERT(0 == tr1.blocks_outstanding());
  ASSERT(ara::core::pmr::new_delete_resource_singleton() == tr1.parent());
  test_resource tr2(&tr1);
  ASSERT(&tr1 == tr2.parent());
  /* @Start: UT_CORE_3030 */
  /* @TS1: Invoke new_delete_resource_singleton() of Polymorphic Allocator class. */
  /* @TE1: parent() object should be equal to the new_delete_resource_singleton. */
  /* @End: UT_CORE_3030 */

  std::cout << "Testing operator ==\n";
  ASSERT(tr1.is_equal(tr1));
  ASSERT(tr1 == tr1);
  ASSERT(!tr1.is_equal(tr2));
  ASSERT(tr1 != tr2);

  std::cout << "Testing statistics gathering\n";
  void* p1 = tr1.allocate(40);
  ASSERT(40 == tr1.bytes_allocated());
  ASSERT(0 == tr1.bytes_deallocated());
  ASSERT(40 == tr1.bytes_outstanding());
  ASSERT(40 == tr1.bytes_highwater());
  ASSERT(1 == tr1.blocks_outstanding());
  tr1.deallocate(p1, 40);
  /* @Start: UT_CORE_3030 */
  /* @TS2: Invoke deallocate() of memory resource class. */
  /* @TE2: value of p1 should be equal to 40. */
  /* @End: UT_CORE_3030 */
  ASSERT(40 == tr1.bytes_allocated());
  ASSERT(40 == tr1.bytes_deallocated());
  ASSERT(0 == tr1.bytes_outstanding());
  ASSERT(40 == tr1.bytes_highwater());
  ASSERT(0 == tr1.blocks_outstanding());
  p1 = tr1.allocate(32);
  /* @Start: UT_CORE_3030 */
  /* @TS3: Invoke allocate() of memory resource class. */
  /* @TE3: allocate the 32 bytes into p1. */
  /* @End: UT_CORE_3030 */
  ASSERT(72 == tr1.bytes_allocated());
  ASSERT(40 == tr1.bytes_deallocated());
  ASSERT(32 == tr1.bytes_outstanding());
  ASSERT(40 == tr1.bytes_highwater());
  ASSERT(1 == tr1.blocks_outstanding());
  void* p2 = tr1.allocate(64, 2);
  ASSERT(136 == tr1.bytes_allocated());
  ASSERT(40 == tr1.bytes_deallocated());
  ASSERT(96 == tr1.bytes_outstanding());
  ASSERT(96 == tr1.bytes_highwater());
  ASSERT(2 == tr1.blocks_outstanding());
  tr1.deallocate(p1, 32);
  /* @Start: UT_CORE_3030 */
  /* @TS4: Invoke deallocate() of memory resource class. */
  /* @TE4: value of p1 should be equal to 32. */
  /* @End: UT_CORE_3030 */
  ASSERT(136 == tr1.bytes_allocated());
  ASSERT(72 == tr1.bytes_deallocated());
  ASSERT(64 == tr1.bytes_outstanding());
  ASSERT(96 == tr1.bytes_highwater());
  ASSERT(1 == tr1.blocks_outstanding());

  std::cout << "Testing detection of mismatched pointer on deallocation\n";
  try
  {
    int dummy = 0;
    tr1.deallocate(&dummy, sizeof(int), sizeof(int));
    ASSERT(false && "unreachable");
    /* @Start: UT_CORE_3030 */
    /* @TS5: Invoke deallocate() of memory resource class. */
    /* @TE5: deallocate() should return unreachable. */
    /* @End: UT_CORE_3030 */
  }
  catch (std::invalid_argument& ex)
  {
    const char* experr = "deallocate: Invalid pointer";
    LOOP_ASSERT(ex.what(), 0 == strcmp(experr, ex.what()));
    /* @Start: UT_CORE_3030 */
    /* @TS6: Invoke deallocate() of memory resource class. */
    /* @TE6: deallocate() should return Invalid pointer. */
    /* @End: UT_CORE_3030 */
  }

  std::cout << "Testing detection of mismatched size on deallocation\n";
  try
  {
    tr1.deallocate(p2, 32, 2);
    ASSERT(false && "unreachable");
    /* @Start: UT_CORE_3030 */
    /* @TS7: Invoke deallocate() of memory resource class. */
    /* @TE7: deallocate() should return unreachable. */
    /* @End: UT_CORE_3030 */
  }
  catch (std::invalid_argument& ex)
  {
    const char* experr = "deallocate: Size mismatch";
    LOOP_ASSERT(ex.what(), 0 == strcmp(experr, ex.what()));
    /* @Start: UT_CORE_3030 */
    /* @TS8: Invoke deallocate() of memory resource class. */
    /* @TE8: deallocate() should return Size mismatch. */
    /* @End: UT_CORE_3030 */
  }

  std::cout << "Testing detection of mismatched alignment on deallocation\n";
  try
  {
    tr1.deallocate(p2, 64, 4);
    ASSERT(false && "unreachable");
    /* @Start: UT_CORE_3030 */
    /* @TS9: Invoke deallocate() of memory resource class. */
    /* @TE9: deallocate() should return unreachable. */
    /* @End: UT_CORE_3030 */
  }
  catch (std::invalid_argument& ex)
  {
    const char* experr = "deallocate: Alignment mismatch";
    LOOP_ASSERT(ex.what(), 0 == strcmp(experr, ex.what()));
    /* @Start: UT_CORE_3030 */
    /* @TS10: Invoke deallocate() of memory resource class. */
    /* @TE10: deallocate() should return Alignment mismatch. */
    /* @End: UT_CORE_3030 */
  }

  tr1.deallocate(p2, 64, 2);
  ASSERT(136 == tr1.bytes_allocated());
  ASSERT(136 == tr1.bytes_deallocated());
  ASSERT(0 == tr1.bytes_outstanding());
  ASSERT(96 == tr1.bytes_highwater());
  ASSERT(0 == tr1.blocks_outstanding());

  std::cout << "Testing delegation to parent resource\n";
  test_resource parentr;
  {
    test_resource tr3(&parentr);
    ASSERT(&parentr == tr3.parent());
    ASSERT(0 == parentr.blocks_outstanding());

    // First allocation in tr3 allocates an extra block in parent for
    // internal vector.
    void* p3 = tr3.allocate(128);
    (void)p3;
    ASSERT(1 == tr3.blocks_outstanding());
    ASSERT(2 == parentr.blocks_outstanding());

    void* p4 = tr3.allocate(18, 2);
    void* p5 = tr3.allocate(8, 8);
    (void)p5;
    ASSERT(154 == tr3.bytes_outstanding());
    ASSERT(3 == tr3.blocks_outstanding());
    ASSERT(4 == parentr.blocks_outstanding());

    // No leak recorded until resource is destroyed.
    ASSERT(0 == test_resource::leaked_bytes());
    ASSERT(0 == test_resource::leaked_blocks());

    tr3.deallocate(p4, 18, 2);
    tr3.deallocate(p5, 8, 8);
    tr3.deallocate(p3, 128);
    ASSERT(0 == tr3.bytes_outstanding());
    ASSERT(0 == tr3.blocks_outstanding());
    ASSERT(1 >= parentr.blocks_outstanding());

    // tr3 destroyed -- no leak
  }
  ASSERT(0 == test_resource::leaked_bytes());
  ASSERT(0 == test_resource::leaked_blocks());
  ASSERT(0 == parentr.blocks_outstanding());

  std::cout << "Testing detection of leaks on resource destruction\n";
  {
    test_resource tr4(&parentr);
    ASSERT(&parentr == tr4.parent());
    ASSERT(0 == parentr.blocks_outstanding());

    void* p3 = tr4.allocate(128);
    (void)p3;
    void* p4 = tr4.allocate(18, 2);
    void* p5 = tr4.allocate(8, 8);
    (void)p5;

    ASSERT(154 == tr4.bytes_outstanding());
    ASSERT(3 == tr4.blocks_outstanding());
    ASSERT(4 == parentr.blocks_outstanding());

    tr4.deallocate(p4, 18, 2);
    ASSERT(136 == tr4.bytes_outstanding());
    ASSERT(2 == tr4.blocks_outstanding());
    ASSERT(3 == parentr.blocks_outstanding());

    // No leak recorded until resource is destroyed.
    ASSERT(0 == test_resource::leaked_bytes());
    ASSERT(0 == test_resource::leaked_blocks());

    // tr4 destructor called here, leaking 2 blocks.
  }
  LOOP_ASSERT(test_resource::leaked_bytes(), 136 == test_resource::leaked_bytes());
  LOOP_ASSERT(test_resource::leaked_blocks(), 2 == test_resource::leaked_blocks());
  // Test that all "leaked" blocks were actually returned to parent resource
  LOOP_ASSERT(parentr.blocks_outstanding(), 0 == parentr.blocks_outstanding());

  test_resource::clear_leaked();
  ASSERT(0 == test_resource::leaked_bytes());
  ASSERT(0 == test_resource::leaked_blocks());

  EXPECT_EQ(testStatus, 0);
}

class AllocCounters
{
  int m_num_allocs;
  int m_num_deallocs;
  int m_bytes_allocated;
  int m_bytes_deallocated;

 public:
  AllocCounters() : m_num_allocs(0), m_num_deallocs(0), m_bytes_allocated(0), m_bytes_deallocated(0)
  {
  }

  void allocate(std::size_t nbytes)
  {
    ++m_num_allocs;
    m_bytes_allocated += nbytes;
  }

  void deallocate(std::size_t nbytes)
  {
    ++m_num_deallocs;
    m_bytes_deallocated += nbytes;
  }

  int blocks_outstanding() const
  {
    return m_num_allocs - m_num_deallocs;
  }
  int bytes_outstanding() const
  {
    return m_bytes_allocated - m_bytes_deallocated;
  }

  void dump(std::ostream& os, const char* msg)
  {
    os << msg << ":\n";
    os << "  num allocs         = " << m_num_allocs << '\n';
    os << "  num deallocs       = " << m_num_deallocs << '\n';
    os << "  outstanding allocs = " << blocks_outstanding() << '\n';
    os << "  bytes allocated    = " << m_bytes_allocated << '\n';
    os << "  bytes deallocated  = " << m_bytes_deallocated << '\n';
    os << "  outstanding bytes  = " << bytes_outstanding() << '\n';
    os << std::endl;
  }

  void clear()
  {
    m_num_allocs = 0;
    m_num_deallocs = 0;
    m_bytes_allocated = 0;
    m_bytes_deallocated = 0;
  }
};

union CountedAllocHeader {
  void* m_align;
  std::size_t m_size;
};

void* countedAllocate(std::size_t nbytes, AllocCounters* counters)
{
  static const std::size_t headerSize = sizeof(CountedAllocHeader);
  std::size_t blocksize = (nbytes + 2 * headerSize - 1) & ~(headerSize - 1);
  CountedAllocHeader* ret = static_cast<CountedAllocHeader*>(std::malloc(blocksize));
  ret->m_size = nbytes;
  ++ret;
  if (counters)
    counters->allocate(nbytes);
  return ret;
}

void countedDeallocate(void* p, std::size_t nbytes, AllocCounters* counters)
{
  CountedAllocHeader* h = static_cast<CountedAllocHeader*>(p) - 1;
  ASSERT(nbytes == h->m_size);
  h->m_size = 0xdeadbeaf;
  std::free(h);
  if (counters)
    counters->deallocate(nbytes);
}

void countedDeallocate(void* p, AllocCounters* counters)
{
  CountedAllocHeader* h = static_cast<CountedAllocHeader*>(p) - 1;
  std::size_t nbytes = h->m_size;
  h->m_size = 0xdeadbeaf;
  std::free(h);
  if (counters)
    counters->deallocate(nbytes);
}

AllocCounters newDeleteCounters;

// Replace global new and delete
void* operator new(std::size_t nbytes)
{
  return countedAllocate(nbytes, &newDeleteCounters);
}

void operator delete(void* p) noexcept
{
  countedDeallocate(p, &newDeleteCounters);
}

class TestResource : public ara::core::pmr::memory_resource
{
  AllocCounters m_counters;

  // Not copyable
  TestResource(const TestResource&);
  TestResource& operator=(const TestResource&);

 public:
  TestResource()
  {
  }
  ~TestResource() override;

  AllocCounters& counters()
  {
    return m_counters;
  }
  AllocCounters const& counters() const
  {
    return m_counters;
  }

  void clear()
  {
    m_counters.clear();
  }

 protected:
  void* do_allocate(size_t bytes, size_t alignment) override;
  void do_deallocate(void* p, size_t bytes, size_t alignment) override;
  bool do_is_equal(const memory_resource& other) const noexcept override;
};

TestResource::~TestResource()
{
  ASSERT(0 == m_counters.blocks_outstanding());
}

void* TestResource::do_allocate(size_t bytes, size_t alignment)
{
  return countedAllocate(bytes, &m_counters);
}

void TestResource::do_deallocate(void* p, size_t bytes, size_t alignment)
{
  countedDeallocate(p, bytes, &m_counters);
}

bool TestResource::do_is_equal(const memory_resource& other) const noexcept
{
  // Two TestResource objects are equal only if they are the same object
  return this == &other;
}

TestResource dfltTestRsrc;
AllocCounters& dfltTestCounters = dfltTestRsrc.counters();

AllocCounters dfltSimpleCounters;

template <typename Tp>
class SimpleAllocator
{
  AllocCounters* m_counters;

 public:
  typedef Tp value_type;

  SimpleAllocator(AllocCounters* c = &dfltSimpleCounters) : m_counters(c)
  {
  }

  // Required constructor
  template <typename T>
  SimpleAllocator(const SimpleAllocator<T>& other) : m_counters(other.counters())
  {
  }

  Tp* allocate(std::size_t n)
  {
    return static_cast<Tp*>(countedAllocate(n * sizeof(Tp), m_counters));
  }

  void deallocate(Tp* p, std::size_t n)
  {
    countedDeallocate(p, n * sizeof(Tp), m_counters);
  }

  AllocCounters* counters() const
  {
    return m_counters;
  }
};

template <typename Tp1, typename Tp2>
bool operator==(const SimpleAllocator<Tp1>& a, const SimpleAllocator<Tp2>& b)
{
  return a.counters() == b.counters();
}

template <typename Tp1, typename Tp2>
bool operator!=(const SimpleAllocator<Tp1>& a, const SimpleAllocator<Tp2>& b)
{
  return !(a == b);
}

int func(const char* s)
{
  return std::atoi(s);
}

struct UniqDummyType
{
  void zzzzz(UniqDummyType, bool)
  {
  }
};
typedef void (UniqDummyType::*UniqPointerType)(UniqDummyType);

template <typename _Tp>
struct unvoid
{
  typedef _Tp type;
};
template <>
struct unvoid<void>
{
  struct type
  {
  };
};
template <>
struct unvoid<const void>
{
  struct type
  {
  };
};

template <typename Alloc>
class SimpleString
{
  typedef std::allocator_traits<Alloc> AllocTraits;

  Alloc alloc_;
  typename AllocTraits::pointer data_;

 public:
  typedef Alloc allocator_type;

  SimpleString(const Alloc& a = Alloc()) : alloc_(a), data_(nullptr)
  {
  }
  SimpleString(const char* s, const Alloc& a = Alloc()) : alloc_(a), data_(nullptr)
  {
    assign(s);
  }

  SimpleString(const SimpleString& other) :
      alloc_(AllocTraits::select_on_container_copy_construction(other.alloc_)), data_(nullptr)
  {
    assign(other.data_);
  }

  SimpleString(SimpleString&& other) : alloc_(other.alloc_), data_(other.data_)
  {
    other.data_ = nullptr;
  }

  SimpleString(const SimpleString& other, const Alloc& a) : alloc_(a), data_(nullptr)
  {
    assign(other.data_);
  }

  SimpleString(SimpleString&& other, const Alloc& a) : alloc_(a), data_(nullptr)
  {
    *this = std::move(other);
  }

  SimpleString& operator=(const SimpleString& other)
  {
    if (this == &other)
      return *this;
    clear();
    if (AllocTraits::propagate_on_container_copy_assignment::value)
      alloc_ = other.alloc_;
    assign(other.data_);
    return *this;
  }

  SimpleString& operator=(SimpleString&& other)
  {
    if (this == &other)
      return *this;
    else if (alloc_ == other.alloc_)
      std::swap(data_, other.data_);
    else if (AllocTraits::propagate_on_container_move_assignment::value)
    {
      clear();
      alloc_ = std::move(other.alloc_);
      data_ = other.data_;
      other.data_ = nullptr;
    }
    else
      assign(other.c_str());
    return *this;
  }

  ~SimpleString()
  {
    clear();
  }

  void clear()
  {
    if (data_)
      AllocTraits::deallocate(alloc_, data_, std::strlen(&*data_) + 1);
  }

  void assign(const char* s)
  {
    clear();
    data_ = AllocTraits::allocate(alloc_, std::strlen(s) + 1);
    std::strcpy(&*data_, s);
  }

  const char* c_str() const
  {
    return &(*data_);
  }

  Alloc get_allocator() const
  {
    return alloc_;
  }
};

template <typename Alloc>
inline bool operator==(const SimpleString<Alloc>& a, const SimpleString<Alloc>& b)
{
  return 0 == std::strcmp(a.c_str(), b.c_str());
}

template <typename Alloc>
inline bool operator!=(const SimpleString<Alloc>& a, const SimpleString<Alloc>& b)
{
  return !(a == b);
}

template <typename Alloc>
inline bool operator==(const SimpleString<Alloc>& a, const char* b)
{
  return 0 == std::strcmp(a.c_str(), b);
}

template <typename Alloc>
inline bool operator!=(const SimpleString<Alloc>& a, const char* b)
{
  return !(a == b);
}

template <typename Alloc>
inline bool operator==(const char* a, const SimpleString<Alloc>& b)
{
  return 0 == std::strcmp(a, b.c_str());
}

template <typename Alloc>
inline bool operator!=(const char* a, const SimpleString<Alloc>& b)
{
  return !(a == b);
}

template <typename Tp, typename Alloc = std::allocator<Tp>>
class SimpleVector
{
  // A simple vector-like class with a fixed capacity determined at
  // constructor.

  static_assert(std::is_same<typename Alloc::value_type, Tp>::value,
      "Allocator's value_type does not match container's");

  typedef std::allocator_traits<Alloc> AllocTraits;

  std::size_t m_capacity;
  std::size_t m_size;
  Alloc m_alloc;
  Tp* m_data;

  enum
  {
    default_capacity = 5
  };

  // No need to make this assignable, so skip implementation
  SimpleVector& operator=(const SimpleVector& other)
  {
    return *this;
  }

 public:
  typedef Tp value_type;
  typedef Alloc allocator_type;
  typedef Tp* iterator;
  typedef const Tp* const_iterator;

  SimpleVector(const allocator_type& a = allocator_type());
  SimpleVector(std::size_t capacity, const allocator_type& a = allocator_type());
  SimpleVector(std::size_t size, const value_type& v, const allocator_type& a = allocator_type());
  SimpleVector(const SimpleVector& other);
  SimpleVector(const SimpleVector& other, const allocator_type& a);
  ~SimpleVector();

  void push_back(const value_type& x);
  template <typename... Args>
  void emplace_back(Args&&... args);
  void pop_back();

  Tp& front()
  {
    return m_data[0];
  }
  Tp const& front() const
  {
    return m_data[0];
  }
  Tp& back()
  {
    return m_data[m_size - 1];
  }
  Tp const& back() const
  {
    return m_data[m_size - 1];
  }
  Tp& operator[](std::size_t i)
  {
    return m_data[i];
  }
  Tp const& operator[](std::size_t i) const
  {
    return m_data[i];
  }
  iterator begin()
  {
    return m_data;
  }
  const_iterator begin() const
  {
    return m_data;
  }
  iterator end()
  {
    return m_data + m_size;
  }
  const_iterator end() const
  {
    return m_data + m_size;
  }

  std::size_t capacity() const
  {
    return m_capacity;
  }
  std::size_t size() const
  {
    return m_size;
  }
  allocator_type get_allocator() const
  {
    return m_alloc;
  }
};

template <typename Tp, typename Alloc>
SimpleVector<Tp, Alloc>::SimpleVector(const allocator_type& a) :
    m_capacity(default_capacity), m_size(0), m_alloc(a), m_data(nullptr)
{
  m_data = AllocTraits::allocate(m_alloc, m_capacity);
}

template <typename Tp, typename Alloc>
SimpleVector<Tp, Alloc>::SimpleVector(std::size_t capacity, const allocator_type& a) :
    m_capacity(capacity), m_size(0), m_alloc(a), m_data(nullptr)
{
  m_data = AllocTraits::allocate(m_alloc, m_capacity);
}

template <typename Tp, typename Alloc>
SimpleVector<Tp, Alloc>::SimpleVector(std::size_t size, const value_type& v, const allocator_type& a) :
    m_capacity(size + default_capacity), m_size(0), m_alloc(a), m_data(nullptr)
{
  m_data = AllocTraits::allocate(m_alloc, m_capacity);
  for (std::size_t i = 0; i < size; ++i)
    push_back(v);
}

template <typename Tp, typename Alloc>
SimpleVector<Tp, Alloc>::SimpleVector(const SimpleVector& other) :
    m_capacity(other.m_capacity), m_size(0),
    m_alloc(AllocTraits::select_on_container_copy_construction(other.m_alloc)), m_data(nullptr)
{
  m_data = AllocTraits::allocate(m_alloc, m_capacity);
  for (std::size_t i = 0; i < other.m_size; ++i)
    push_back(other[i]);
}

template <typename Tp, typename Alloc>
SimpleVector<Tp, Alloc>::SimpleVector(const SimpleVector& other, const allocator_type& a) :
    m_capacity(other.m_capacity), m_size(0), m_alloc(a), m_data(nullptr)
{
  m_data = AllocTraits::allocate(m_alloc, m_capacity);
  for (std::size_t i = 0; i < other.m_size; ++i)
    push_back(other[i]);
}

template <typename Tp, typename Alloc>
SimpleVector<Tp, Alloc>::~SimpleVector()
{
  for (std::size_t i = 0; i < m_size; ++i)
    AllocTraits::destroy(m_alloc, std::addressof(m_data[i]));
  AllocTraits::deallocate(m_alloc, m_data, m_capacity);
}

template <typename Tp, typename Alloc>
void SimpleVector<Tp, Alloc>::push_back(const value_type& x)
{
  ASSERT(m_size < m_capacity);
  AllocTraits::construct(m_alloc, std::addressof(m_data[m_size++]), x);
}

template <typename Tp, typename Alloc>
template <typename... Args>
void SimpleVector<Tp, Alloc>::emplace_back(Args&&... args)
{
  ASSERT(m_size < m_capacity);
  AllocTraits::construct(m_alloc, std::addressof(m_data[m_size++]), std::forward<Args>(args)...);
}

template <typename Tp, typename Alloc>
void SimpleVector<Tp, Alloc>::pop_back()
{
  ASSERT(m_size > 0);
  AllocTraits::destroy(m_alloc, std::addressof(m_data[--m_size]));
}

// Force instantiation of whole classes
template class ara::core::pmr::polymorphic_allocator<double>;
template class SimpleVector<double, ara::core::pmr::polymorphic_allocator<double>>;
template class SimpleAllocator<double>;
template class SimpleVector<double, SimpleAllocator<double>>;

// Polymorphic Allocator Test
/*******************************************************************************
@TestCaseId:          UT_CORE_3031
@Test_Description:    To test the functionality of polymorphic allocator test resources.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634054, 634101, 634102, 634179, 634099, 634100
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, PolymorphicAllocator)
{
  testStatus = 0;
#define PMA ara::core::pmr::polymorphic_allocator

  std::cout << "Testing new_delete_resource\n";
  {
    // Test new_delete_resource
    int expBlocks = newDeleteCounters.blocks_outstanding();
    int expBytes = newDeleteCounters.bytes_outstanding();

    ara::core::pmr::memory_resource* r = ara::core::pmr::new_delete_resource_singleton();
    ASSERT(ara::core::pmr::get_default_resource() == r);

    void* p = r->allocate(5, 1);
    ++expBlocks;
    expBytes += 5;

    ASSERT(p);
    ASSERT(newDeleteCounters.blocks_outstanding() == expBlocks);
    ASSERT(newDeleteCounters.bytes_outstanding() == expBytes);

    r->deallocate(p, 5, 1);
    ASSERT(newDeleteCounters.blocks_outstanding() == 0);
    ASSERT(newDeleteCounters.bytes_outstanding() == 0);
    /* @Start: UT_CORE_3031 */
    /* @TS1: Invoke new_delete_resource() of Polymorphic Allocator class. */
    /* @TE1: Instance of new_delete_resource should be created. */
    /* @End: UT_CORE_3031 */
  }

  ara::core::pmr::set_default_resource(&dfltTestRsrc);
  ASSERT(ara::core::pmr::get_default_resource() == &dfltTestRsrc);

  std::cout << "Testing polymorphic allocator constructors\n";
  {
    // Test construction with resource
    TestResource ar;
    const PMA<double> a1(&ar);
    ASSERT(a1.resource() == &ar);

    // Test conversion constructor
    PMA<char> a2(a1);
    ASSERT(a2.resource() == &ar);

    // Test default construction
    PMA<char> a3;
    ASSERT(a3.resource() == &dfltTestRsrc);

    // Test construction with null pointer
    PMA<char> a4(nullptr);
    ASSERT(a4.resource() == &dfltTestRsrc);

    // Test copy constructor
    PMA<char> a5(a2);
    ASSERT(a5.resource() == &ar);
    /* @Start: UT_CORE_3031 */
    /* @TS2: Invoke polymorphic_Allocator() constructors . */
    /* @TE2: Instance of polymorphic_Allocator should be created. */
    /* @End: UT_CORE_3031 */
  }

  TestResource x, y, z;
  AllocCounters &xc = x.counters(), &yc = y.counters();

  newDeleteCounters.clear();

  std::cout << "Testing simple use of vector with polymorphic allocator\n";
  {
    typedef PMA<int> Alloc;

    SimpleVector<int, Alloc> vx(&x);
    ASSERT(1 == xc.blocks_outstanding());

    vx.push_back(3);
    ASSERT(3 == vx.front());
    ASSERT(1 == vx.size());
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(0 == dfltTestCounters.blocks_outstanding());
    ASSERT(0 == newDeleteCounters.blocks_outstanding());
    /* @Start: UT_CORE_3031 */
    /* @TS3: Invoke polymorphic_Allocator() with vector . */
    /* @TE3: Instance of polymorphic_Allocator should be created. */
    /* @End: UT_CORE_3031 */
  }
  ASSERT(0 == xc.blocks_outstanding());
  ASSERT(0 == dfltTestCounters.blocks_outstanding());
  ASSERT(0 == newDeleteCounters.blocks_outstanding());

  std::cout << "Testing outer allocator is polymorphic, inner is not.\n";
  {
    typedef SimpleString<SimpleAllocator<char>> String;
    typedef PMA<String> Alloc;

    SimpleVector<String, Alloc> vx(&x);
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(0 == dfltSimpleCounters.blocks_outstanding());

    vx.push_back("hello");
    ASSERT(1 == vx.size());
    ASSERT("hello" == vx.back());
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(1 == dfltSimpleCounters.blocks_outstanding());
    vx.push_back("goodbye");
    ASSERT(2 == vx.size());
    ASSERT("hello" == vx.front());
    ASSERT("goodbye" == vx.back());
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(2 == dfltSimpleCounters.blocks_outstanding());
    ASSERT(SimpleAllocator<char>() == vx.front().get_allocator());

    ASSERT(0 == newDeleteCounters.blocks_outstanding());
    /* @Start: UT_CORE_3031 */
    /* @TS4: Invoke outer polymorphic_Allocator(). */
    /* @TE4: Instance of polymorphic_Allocator should be created. */
    /* @End: UT_CORE_3031 */
  }
  ASSERT(0 == xc.blocks_outstanding());
  ASSERT(0 == dfltSimpleCounters.blocks_outstanding());
  ASSERT(0 == newDeleteCounters.blocks_outstanding());

  std::cout << "Testing inner allocator is polymorphic, outer is not.\n";
  {
    typedef SimpleString<PMA<char>> String;
    typedef SimpleAllocator<String> Alloc;

    SimpleVector<String, Alloc> vx(&xc);
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(0 == dfltTestCounters.blocks_outstanding());
    ASSERT(&xc == vx.get_allocator().counters())

    vx.push_back("hello");
    ASSERT(1 == vx.size());
    ASSERT("hello" == vx.back());
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(1 == dfltTestCounters.blocks_outstanding());
    vx.push_back("goodbye");
    ASSERT(2 == vx.size());
    ASSERT("hello" == vx.front());
    ASSERT("goodbye" == vx.back());
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(2 == dfltTestCounters.blocks_outstanding());
    ASSERT(&dfltTestRsrc == vx.front().get_allocator().resource());

    ASSERT(0 == newDeleteCounters.blocks_outstanding());
    /* @Start: UT_CORE_3031 */
    /* @TS5: Invoke inner polymorphic_Allocator(). */
    /* @TE5: Instance of polymorphic_Allocator should be created. */
    /* @End: UT_CORE_3031 */
  }
  ASSERT(0 == xc.blocks_outstanding());
  ASSERT(0 == dfltTestCounters.blocks_outstanding());
  ASSERT(0 == newDeleteCounters.blocks_outstanding());

  std::cout << "Testing both outer and inner allocators are polymorphic.\n";
  {
    typedef SimpleString<PMA<char>> String;
    typedef PMA<String> Alloc;

    SimpleVector<String, Alloc> vx(&x);
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(0 == dfltTestCounters.blocks_outstanding());

    vx.push_back("hello");
    ASSERT(1 == vx.size());
    ASSERT("hello" == vx.back());
    ASSERT(2 == xc.blocks_outstanding());
    ASSERT(0 == dfltTestCounters.blocks_outstanding());
    vx.push_back("goodbye");
    ASSERT(2 == vx.size());
    ASSERT("hello" == vx.front());
    ASSERT("goodbye" == vx.back());
    ASSERT(3 == xc.blocks_outstanding());
    ASSERT(0 == dfltTestCounters.blocks_outstanding());
    ASSERT(&x == vx.front().get_allocator().resource());

    ASSERT(0 == newDeleteCounters.blocks_outstanding());
    /* @Start: UT_CORE_3031 */
    /* @TS6: Invoke both inner and outer polymorphic_Allocator(). */
    /* @TE6: Instance of polymorphic_Allocator should be created. */
    /* @End: UT_CORE_3031 */
  }
  ASSERT(0 == xc.blocks_outstanding());
  ASSERT(0 == dfltTestCounters.blocks_outstanding());
  ASSERT(0 == newDeleteCounters.blocks_outstanding());

  std::cout << "Testing container copy construction\n";
  {
    typedef SimpleString<PMA<char>> String;
    typedef PMA<String> Alloc;

    SimpleVector<String, Alloc> vx(&x);
    ASSERT(1 == xc.blocks_outstanding());

    vx.push_back("hello");
    vx.push_back("goodbye");
    ASSERT(2 == vx.size());
    ASSERT("hello" == vx.front());
    ASSERT("goodbye" == vx.back());
    ASSERT(3 == xc.blocks_outstanding());
    ASSERT(0 == dfltTestCounters.blocks_outstanding());
    ASSERT(&x == vx.front().get_allocator().resource());

    SimpleVector<String, Alloc> vg(vx);
    ASSERT(2 == vg.size());
    ASSERT("hello" == vg.front());
    ASSERT("goodbye" == vg.back());
    ASSERT(3 == xc.blocks_outstanding());
    ASSERT(3 == dfltTestCounters.blocks_outstanding());
    ASSERT(&dfltTestRsrc == vg.front().get_allocator().resource());

    SimpleVector<String, Alloc> vy(vx, &y);
    ASSERT(2 == vy.size());
    ASSERT("hello" == vy.front());
    ASSERT("goodbye" == vy.back());
    ASSERT(3 == xc.blocks_outstanding());
    ASSERT(3 == yc.blocks_outstanding());
    ASSERT(3 == dfltTestCounters.blocks_outstanding());
    ASSERT(&y == vy.front().get_allocator().resource());

    ASSERT(0 == newDeleteCounters.blocks_outstanding());
  }
  ASSERT(0 == xc.blocks_outstanding());
  ASSERT(0 == yc.blocks_outstanding());
  ASSERT(0 == dfltTestCounters.blocks_outstanding());
  ASSERT(0 == newDeleteCounters.blocks_outstanding());

  std::cout << "Testing resource_adaptor\n";
  {
    typedef SimpleString<PMA<char>> String;
    typedef SimpleVector<String, PMA<String>> strvec;
    typedef SimpleVector<strvec, PMA<strvec>> strvecvec;

    SimpleAllocator<char> sax(&xc);
    SimpleAllocator<char> say(&yc);
    ara::core::pmr::resource_adaptor<SimpleAllocator<char>> crx(sax);
    ara::core::pmr::resource_adaptor<SimpleAllocator<char>> cry(say);

    strvec a(&crx);
    strvecvec b(&cry);
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(1 == yc.blocks_outstanding());

    ASSERT(0 == a.size());
    a.push_back("hello");
    ASSERT(1 == a.size());
    ASSERT("hello" == a.front());
    a.push_back("goodbye");
    ASSERT(2 == a.size());
    ASSERT("hello" == a.front());
    ASSERT("goodbye" == a.back());
    ASSERT(3 == xc.blocks_outstanding());
    b.push_back(a);
    ASSERT(1 == b.size());
    ASSERT(2 == b.front().size());
    ASSERT("hello" == b.front().front());
    ASSERT("goodbye" == b.front().back());
    ASSERT(3 == xc.blocks_outstanding());
    LOOP_ASSERT(yc.blocks_outstanding(), 4 == yc.blocks_outstanding());

    b.emplace_back(3, "repeat");
    ASSERT(2 == b.size());
    ASSERT(3 == b.back().size());
    ASSERT(3 == xc.blocks_outstanding());
    LOOP_ASSERT(yc.blocks_outstanding(), 8 == yc.blocks_outstanding());

    static const char* const exp[] = {"hello", "goodbye", "repeat", "repeat", "repeat"};

    int e = 0;
    for (strvecvec::iterator i = b.begin(); i != b.end(); ++i)
    {
      ASSERT(i->get_allocator().resource() == &cry);
      for (strvec::iterator j = i->begin(); j != i->end(); ++j)
      {
        ASSERT(j->get_allocator().resource() == &cry);
        ASSERT(*j == exp[e++]);
      }
    }
    /* @Start: UT_CORE_3031 */
    /* @TS7: Invoke resource_adaptor() class . */
    /* @TE7: Instance of resource_adaptor should be created. */
    /* @End: UT_CORE_3031 */
  }
  LOOP_ASSERT(xc.blocks_outstanding(), 0 == xc.blocks_outstanding());
  LOOP_ASSERT(yc.blocks_outstanding(), 0 == yc.blocks_outstanding());
  ASSERT(0 == dfltSimpleCounters.blocks_outstanding());
  ASSERT(0 == newDeleteCounters.blocks_outstanding());

// Older versions of gcc library don't support scoped_allocator constructor
// of pair.
#if defined(__GNUC__) && (__GNUC__ > 4)
  std::cout << "Testing construct() using pairs\n";
  {
    typedef SimpleString<PMA<char>> String;
    typedef std::pair<String, int> StrInt;
    typedef PMA<StrInt> Alloc;

    SimpleVector<StrInt, SimpleAllocator<StrInt>> vx(&xc);
    SimpleVector<StrInt, Alloc> vy(&y);

    vx.push_back(StrInt("hello", 5));
    ASSERT(1 == vx.size());
    ASSERT(1 == xc.blocks_outstanding());
    ASSERT(1 == dfltTestCounters.blocks_outstanding());
    ASSERT(&dfltTestRsrc == vx.front().first.get_allocator().resource());
    ASSERT("hello" == vx.front().first);
    ASSERT(5 == vx.front().second);
    vy.push_back(StrInt("goodbye", 6));
    ASSERT(1 == vy.size());
    ASSERT(2 == yc.blocks_outstanding());
    ASSERT(1 == dfltTestCounters.blocks_outstanding());
    ASSERT(&y == vy.front().first.get_allocator().resource());
    ASSERT("goodbye" == vy.front().first);
    ASSERT(6 == vy.front().second);
    vy.emplace_back("howdy", 9);
    ASSERT(2 == vy.size());
    ASSERT(3 == yc.blocks_outstanding());
    ASSERT(&y == vy.back().first.get_allocator().resource());
    ASSERT(1 == dfltTestCounters.blocks_outstanding());
    ASSERT("goodbye" == vy[0].first);
    ASSERT(6 == vy[0].second);
    ASSERT("howdy" == vy[1].first);
    ASSERT(9 == vy[1].second);
  }
#endif
  ASSERT(0 == xc.blocks_outstanding());
  ASSERT(0 == yc.blocks_outstanding());
  ASSERT(0 == dfltTestCounters.blocks_outstanding());
  ASSERT(0 == dfltSimpleCounters.blocks_outstanding());
  ASSERT(0 == newDeleteCounters.blocks_outstanding());

  ara::core::pmr::set_default_resource(nullptr);
  ASSERT(ara::core::pmr::new_delete_resource_singleton() == ara::core::pmr::get_default_resource());

  if (testStatus > 0)
  {
    std::cerr << "Error, non-zero test status = " << testStatus << "." << std::endl;
  }
}

std::size_t allocation_count = 0;

void* operator new[](std::size_t count)
{
  ++allocation_count;
  return std::malloc(count);
}

void operator delete[](void* p)
{
  --allocation_count;
  return std::free(p);
}

// New Delete Resource Test
/*******************************************************************************
@TestCaseId:          UT_CORE_3032
@Test_Description:    To test the functionality of new delete resource.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634080, 634081, 634094, 634095
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, new_delete_resource_test)
{
  // Make sure new_delete_resource_singleton calls new[]/delete[]
  std::size_t memcount = allocation_count;
  ara::core::pmr::memory_resource* mr = ara::core::pmr::new_delete_resource_singleton();
  // Each time should return the same pointer
  EXPECT_TRUE(mr == ara::core::pmr::new_delete_resource_singleton());
  EXPECT_TRUE(memcount == allocation_count);
  void* addr = mr->allocate(16, 1);
  mr->deallocate(addr, 16, 1);
  /* @Start: UT_CORE_3032 */
  /* @TS1: Invoke new_delete_resource_singleton() class . */
  /* @TE1: Instance of new_delete_resource_singleton should be created. */
  /* @End: UT_CORE_3032 */
}

// Null Memory Resource Test
/*******************************************************************************
@TestCaseId:          UT_CORE_3033
@Test_Description:    To test the functionality of null memory resource.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634080, 634081, 634094
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, null_memory_resource_test)
{
  // Make sure it throw
  ara::core::pmr::memory_resource* mr = ara::core::pmr::null_memory_resource();
  EXPECT_TRUE(mr != 0);

  bool bad_allocexception_thrown = false;

  try
  {
    mr->allocate(1, 1);
  }
  catch (std::bad_alloc&)
  {
    bad_allocexception_thrown = true;
  }
  catch (...)
  {
  }

  EXPECT_TRUE(bad_allocexception_thrown == true);
  /* @Start: UT_CORE_3033 */
  /* @TS1: Invoke null_memory_resource() class . */
  /* @TE1: Instance of null_memory_resource should be created. */
  /* @End: UT_CORE_3033 */
}

class memory_resource_logger : public ara::core::pmr::memory_resource
{
 public:
  struct allocation_info
  {
    char* address;
    std::size_t bytes;
    std::size_t alignment;
  };

  ara::core::pmr::Vector<allocation_info> m_info;
  unsigned m_mismatches;

  explicit memory_resource_logger() : m_info(), m_mismatches()
  {
  }

  virtual ~memory_resource_logger() override
  {
    this->reset();
  }

  virtual void* do_allocate(std::size_t bytes, std::size_t alignment) override
  {
    char* addr = (char*)std::malloc(bytes);
    if (!addr)
    {
      std::__throw_bad_alloc();
    }
    allocation_info info;
    info.address = addr;
    info.bytes = bytes;
    info.alignment = alignment;
    m_info.push_back(info);
    return addr;
  }

  virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override
  {
    std::size_t i = 0, max = m_info.size();
    while (i != max && m_info[i].address != p)
    {
      ++i;
    }
    if (i == max)
    {
      ++m_mismatches;
    }
    else
    {
      const allocation_info& info = m_info[i];
      m_mismatches += info.bytes != bytes || info.alignment != alignment;
      std::free(p);
      m_info.erase(m_info.begin() + i);
    }
  }

  virtual bool do_is_equal(const ara::core::pmr::memory_resource& other) const noexcept override
  {
    return static_cast<const memory_resource*>(this) == &other;
  }

  void reset()
  {
    while (!m_info.empty())
    {
      std::free(m_info.back().address);
      m_info.pop_back();
    }
    m_mismatches = 0u;
  }
};

class derived_from_memory_resource : public ara::core::pmr::memory_resource
{
 public:
  explicit derived_from_memory_resource(unsigned i = 0u) : id(i)
  {
  }

  virtual ~derived_from_memory_resource() override
  {
    destructor_called = true;
  }

  virtual void* do_allocate(std::size_t bytes, std::size_t alignment) override
  {
    do_allocate_called = true;
    do_allocate_bytes = bytes;
    do_allocate_alignment = alignment;
    return do_allocate_return;
  }

  virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override
  {
    do_deallocate_called = true;
    do_deallocate_p = p;
    do_deallocate_bytes = bytes;
    do_deallocate_alignment = alignment;
  }

  virtual bool do_is_equal(const ara::core::pmr::memory_resource& other) const noexcept override
  {
    do_is_equal_called = true;
    do_is_equal_other = &other;
    return static_cast<const derived_from_memory_resource&>(other).id == this->id;
  }

  void reset()
  {
    destructor_called = false;

    do_allocate_return = 0;
    do_allocate_called = false;
    do_allocate_bytes = 0u;
    do_allocate_alignment = 0u;

    do_deallocate_called = false;
    do_deallocate_p = 0;
    do_deallocate_bytes = 0u;
    do_deallocate_alignment = 0u;

    do_is_equal_called = false;
    do_is_equal_other = 0;
  }
  // checkers
  static bool destructor_called;

  unsigned id;
  void* do_allocate_return;
  mutable bool do_allocate_called;
  mutable std::size_t do_allocate_bytes;
  mutable std::size_t do_allocate_alignment;

  mutable bool do_deallocate_called;
  mutable void* do_deallocate_p;
  mutable std::size_t do_deallocate_bytes;
  mutable std::size_t do_deallocate_alignment;

  mutable bool do_is_equal_called;
  mutable const ara::core::pmr::memory_resource* do_is_equal_other;
};

bool derived_from_memory_resource::destructor_called = false;

// Default Resource Test
/*******************************************************************************
@TestCaseId:          UT_CORE_3034
@Test_Description:    To test the functionality of default resources.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634080, 634081, 634094
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, default_resource_test)
{
  // Default resource must be new/delete before set_default_resource
  EXPECT_TRUE(ara::core::pmr::get_default_resource() == ara::core::pmr::new_delete_resource_singleton());
  // Set default resource and obtain previous
  derived_from_memory_resource d;
  ara::core::pmr::memory_resource* prev_default = ara::core::pmr::set_default_resource(&d);
  EXPECT_TRUE(ara::core::pmr::get_default_resource() == &d);
  // Set default resource with null, which should be new/delete
  prev_default = ara::core::pmr::set_default_resource(0);
  /* @Start: UT_CORE_3034 */
  /* @TS1: Invoke set_default_resource(). */
  /* @TE1: set_default_resource should return the previous value of the default memory resource pointer. */
  /* @End: UT_CORE_3034 */
  EXPECT_TRUE(prev_default == &d);
  EXPECT_TRUE(ara::core::pmr::get_default_resource() == ara::core::pmr::new_delete_resource_singleton());
  /* @Start: UT_CORE_3034 */
  /* @TS2: Invoke get_default_resource(). */
  /* @TE2: get_default_resource should return the current default resource. */
  /* @End: UT_CORE_3034 */
}

static const std::size_t AllocCount = 32u;

namespace test_block_chain
{
void test_constructor()
{
  memory_resource_logger mrl;
  ara::core::pmr::internal::block_slist bc(mrl);
  // Resource stored
  EXPECT_TRUE(&bc.upstream_resource() == &mrl);
  // No allocation performed
  EXPECT_TRUE(mrl.m_info.size() == 0u);
}

void test_allocate()
{
  memory_resource_logger mrl;
  ara::core::pmr::internal::block_slist bc(mrl);

  for (unsigned i = 0; i != unsigned(AllocCount); ++i)
  {
    // Allocate and trace data
    const std::size_t alloc = i + 1;
    char* const addr = (char*)bc.allocate(alloc);
    // Should have allocated a new entry
    EXPECT_TRUE(mrl.m_info.size() == (i + 1));
    // Requested size must be bigger to include metadata
    EXPECT_TRUE(mrl.m_info[i].bytes > alloc);
    EXPECT_TRUE(mrl.m_info[i].alignment == ara::core::pmr::memory_resource::max_align);
    // Returned address should be between the allocated buffer
    EXPECT_TRUE(mrl.m_info[i].address < addr);
    EXPECT_TRUE(addr < (mrl.m_info[i].address + mrl.m_info[i].bytes));
    // Allocate size should include all requested size
    EXPECT_TRUE((addr + alloc) <= (mrl.m_info[i].address + mrl.m_info[i].bytes));
    // Allocation must be max-aligned
    EXPECT_TRUE((std::size_t(addr) % ara::core::pmr::memory_resource::max_align) == 0);
  }
}

void test_release()
{
  memory_resource_logger mrl;
  ara::core::pmr::internal::block_slist bc(mrl);

  // Allocate and trace data
  char* bufs[AllocCount];
  for (unsigned i = 0; i != unsigned(AllocCount); ++i)
  {
    bufs[i] = (char*)bc.allocate(i + 1);
  }
  (void)bufs;
  // Should have allocated a new entry
  EXPECT_TRUE(mrl.m_info.size() == AllocCount);

  // Now release and check all allocations match deallocations
  bc.release();
  EXPECT_TRUE(mrl.m_mismatches == 0);
  EXPECT_TRUE(mrl.m_info.size() == 0u);
}

void test_memory_resource()
{
  derived_from_memory_resource d;
  ara::core::pmr::internal::block_slist bc(d);
  // Resource stored
  EXPECT_TRUE(&bc.upstream_resource() == &d);
}

void test_destructor()
{
  memory_resource_logger mrl;
  {
    ara::core::pmr::internal ::block_slist bc(mrl);

    // Allocate and trace data
    char* bufs[AllocCount];
    for (unsigned i = 0; i != unsigned(AllocCount); ++i)
    {
      bufs[i] = (char*)bc.allocate(i + 1);
    }
    (void)bufs;
    // Should have allocated a new entry
    EXPECT_TRUE(mrl.m_info.size() == AllocCount);

    // Destructor should release all memory
  }
  EXPECT_TRUE(mrl.m_mismatches == 0);
  EXPECT_TRUE(mrl.m_info.size() == 0u);
}

} // namespace test_block_chain
// Namespace test_block_chain

void test_resource_constructor()
{
  // First constructor, null resource
  {
    memory_resource_logger mrl;
    EXPECT_TRUE(mrl.m_info.size() == 0u);
    ara::core::pmr::set_default_resource(&mrl);
    ara::core::pmr::monotonic_buffer_resource m;
    // Test postconditions
    EXPECT_TRUE(m.upstream_resource() == ara::core::pmr::get_default_resource());
    // Test it does not allocate any memory
    EXPECT_TRUE(mrl.m_info.size() == 0u);
    ara::core::pmr::set_default_resource(0);
  }
  // First constructor, non-null resource
  {
    derived_from_memory_resource dmr;
    dmr.reset();
    ara::core::pmr::monotonic_buffer_resource m(&dmr);
    // Test postconditions
    EXPECT_TRUE(m.upstream_resource() == &dmr);
    EXPECT_TRUE(m.next_buffer_size() == ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size);
    EXPECT_TRUE(m.current_buffer() == 0);
    // Test it does not allocate any memory
    EXPECT_TRUE(dmr.do_allocate_called == false);
  }
}

void test_initial_size_constructor()
{
  // Second constructor, null resource
  const std::size_t initial_size = ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size * 2;
  {
    memory_resource_logger mrl;
    EXPECT_TRUE(mrl.m_info.size() == 0u);
    ara::core::pmr::set_default_resource(&mrl);
    ara::core::pmr::monotonic_buffer_resource m(initial_size);
    // Test postconditions
    EXPECT_TRUE(m.upstream_resource() == ara::core::pmr::get_default_resource());
    EXPECT_TRUE(m.next_buffer_size() >= initial_size);
    EXPECT_TRUE(m.current_buffer() == 0);
    // Test it does not allocate any memory
    EXPECT_TRUE(mrl.m_info.size() == 0u);
    ara::core::pmr::set_default_resource(0);
  }
  // Second constructor, non-null resource
  {
    derived_from_memory_resource dmr;
    dmr.reset();
    ara::core::pmr::monotonic_buffer_resource m(initial_size, &dmr);
    // Test postconditions
    EXPECT_TRUE(m.upstream_resource() == &dmr);
    EXPECT_TRUE(m.next_buffer_size() >= initial_size);
    EXPECT_TRUE(m.current_buffer() == 0);
    // Test it does not allocate any memory
    EXPECT_TRUE(dmr.do_allocate_called == false);
  }
}

void test_buffer_constructor()
{
  const std::size_t BufSz = ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size * 2;
  unsigned char buf[BufSz];
  // Third constructor, null resource
  {
    memory_resource_logger mrl;
    EXPECT_TRUE(mrl.m_info.size() == 0u);
    ara::core::pmr::set_default_resource(&mrl);
    ara::core::pmr::monotonic_buffer_resource m(buf, BufSz);
    // Test postconditions
    EXPECT_TRUE(m.upstream_resource() == ara::core::pmr::get_default_resource());
    EXPECT_TRUE(m.next_buffer_size() >= BufSz * 2);
    EXPECT_TRUE(m.current_buffer() == buf);
    // Test it does not allocate any memory
    EXPECT_TRUE(mrl.m_info.size() == 0u);
    ara::core::pmr::set_default_resource(0);
  }
  // Third constructor, non-null resource
  {
    derived_from_memory_resource dmr;
    dmr.reset();
    ara::core::pmr::monotonic_buffer_resource m(buf, sizeof(buf), &dmr);
    // Test postconditions
    EXPECT_TRUE(m.upstream_resource() == &dmr);
    EXPECT_TRUE(m.next_buffer_size() >= sizeof(buf) * 2);
    EXPECT_TRUE(m.current_buffer() == buf);
    // Test it does not allocate any memory
    EXPECT_TRUE(dmr.do_allocate_called == false);
  }
  // Check for empty buffers
  {
    ara::core::pmr::monotonic_buffer_resource m(buf, 0);
    EXPECT_TRUE(m.upstream_resource() == ara::core::pmr::get_default_resource());
    EXPECT_TRUE(m.next_buffer_size() > 1);
    EXPECT_TRUE(m.current_buffer() == buf);
  }
}

struct derived_from_monotonic_buffer_resource : public ara::core::pmr::monotonic_buffer_resource
{
  explicit derived_from_monotonic_buffer_resource(memory_resource* p) : monotonic_buffer_resource(p)
  {
  }

  explicit derived_from_monotonic_buffer_resource(std::size_t initial_size, memory_resource* upstream) :
      monotonic_buffer_resource(initial_size, upstream)
  {
  }

  explicit derived_from_monotonic_buffer_resource(
      void* buffer, std::size_t buffer_size, memory_resource* upstream) :
      monotonic_buffer_resource(buffer, buffer_size, upstream)
  {
  }

  using ara::core::pmr::monotonic_buffer_resource::do_allocate;
  using ara::core::pmr::monotonic_buffer_resource::do_deallocate;
  using ara::core::pmr::monotonic_buffer_resource::do_is_equal;
};

void test_upstream_resource()
{
  // Allocate buffer first to avoid stack-use-after-scope in monotonic_buffer_resource's destructor
  const std::size_t BufSz = ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size;
  std::aligned_storage<BufSz + ara::core::pmr::internal::block_slist::header_size> buf;
  // Test stores the resource and uses it to allocate memory
  derived_from_memory_resource dmr;
  dmr.reset();
  derived_from_monotonic_buffer_resource dmbr(&dmr);
  // Resource must be stored and initial values given (no current buffer)
  EXPECT_TRUE(dmbr.upstream_resource() == &dmr);
  EXPECT_TRUE(dmbr.next_buffer_size() == ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size);
  EXPECT_TRUE(dmbr.current_buffer() == 0);
  // Test it does not allocate any memory
  EXPECT_TRUE(dmr.do_allocate_called == false);
  // Now stub buffer storage it as the return buffer
  // for "derived_from_memory_resource":
  dmr.do_allocate_return = &buf;
  // Test that allocation uses the upstream_resource()
  void* addr = dmbr.do_allocate(1u, 1u);
  // Test returns stubbed memory with the internal initial size plus metadata size
  EXPECT_TRUE(addr > (char*)&buf);
  EXPECT_TRUE(dmr.do_allocate_called == true);
  EXPECT_TRUE(dmr.do_allocate_bytes > BufSz);
  // Alignment for the resource must be max_align
  EXPECT_TRUE(dmr.do_allocate_alignment == ara::core::pmr::memory_resource::max_align);
}

void test_do_allocate()
{
  memory_resource_logger mrl;
  {
    std::size_t remaining_storage = 0u;
    derived_from_monotonic_buffer_resource dmbr(&mrl);
    // First test, no buffer
    {
      dmbr.do_allocate(1, 1);
      // It should allocate initial size
      EXPECT_TRUE(mrl.m_info.size() == 1u);
      // Which requests the initial size plus the header size to the allcoator
      EXPECT_TRUE(mrl.m_info[0].bytes
          == ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size
              + ara::core::pmr::internal::block_slist::header_size);
      std::size_t remaining = dmbr.remaining_storage(1u);
      // Remaining storage should be one less than initial, as we requested 1 byte with minimal alignment
      EXPECT_TRUE(remaining == ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size - 1u);
      remaining_storage = remaining;
    }
    // Now ask for more internal storage with misaligned current buffer
    {
      // Test wasted space
      std::size_t wasted_due_to_alignment;
      dmbr.remaining_storage(4u, wasted_due_to_alignment);
      EXPECT_TRUE(wasted_due_to_alignment == 3u);
      dmbr.do_allocate(4, 4);
      // It should not have allocated
      EXPECT_TRUE(mrl.m_info.size() == 1u);
      std::size_t remaining = dmbr.remaining_storage(1u);
      // We wasted some bytes due to alignment plus 4 bytes of real storage
      EXPECT_TRUE(remaining == remaining_storage - 4 - wasted_due_to_alignment);
      remaining_storage = remaining;
    }
    // Now request the same alignment to test no storage is wasted
    {
      std::size_t wasted_due_to_alignment;
      std::size_t remaining = dmbr.remaining_storage(1u, wasted_due_to_alignment);
      EXPECT_TRUE(mrl.m_info.size() == 1u);
      dmbr.do_allocate(4, 4);
      // It should not have allocated
      EXPECT_TRUE(mrl.m_info.size() == 1u);
      remaining = dmbr.remaining_storage(1u);
      // We wasted no bytes due to alignment plus 4 bytes of real storage
      EXPECT_TRUE(remaining == remaining_storage - 4u);
      remaining_storage = remaining;
    }
    // Now exhaust the remaining storage with 2 byte alignment (the last allocation
    // was 4 bytes with 4 byte alignment) so it should be already 2-byte aligned.
    {
      dmbr.do_allocate(remaining_storage, 2);
      std::size_t wasted_due_to_alignment;
      std::size_t remaining = dmbr.remaining_storage(1u, wasted_due_to_alignment);
      EXPECT_TRUE(wasted_due_to_alignment == 0u);
      EXPECT_TRUE(remaining == 0u);
      // It should not have allocated
      EXPECT_TRUE(mrl.m_info.size() == 1u);
      remaining_storage = 0u;
    }
    // The next allocation should trigger the upstream resource, even with a 1 byte
    // allocation.
    {
      dmbr.do_allocate(1u, 1u);
      EXPECT_TRUE(mrl.m_info.size() == 2u);
      // The next allocation should be geometrically bigger.
      EXPECT_TRUE(mrl.m_info[1].bytes
          == 2 * ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size
              + ara::core::pmr::internal::block_slist::header_size);
      std::size_t wasted_due_to_alignment;
      // For a 2 byte alignment one byte will be wasted from the previous 1 byte allocation
      std::size_t remaining = dmbr.remaining_storage(2u, wasted_due_to_alignment);
      EXPECT_TRUE(wasted_due_to_alignment == 1u);
      EXPECT_TRUE(remaining
          == (mrl.m_info[1].bytes - 1u - wasted_due_to_alignment
                 - ara::core::pmr::internal::block_slist::header_size));
      // It should not have allocated
      remaining_storage = dmbr.remaining_storage(1u);
    }
    // Now try a bigger than next allocation and see if next_buffer_size is doubled.
    {
      std::size_t next_alloc = 5 * ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size;
      dmbr.do_allocate(next_alloc, 1u);
      EXPECT_TRUE(mrl.m_info.size() == 3u);
      // The next allocation should be geometrically bigger.
      EXPECT_TRUE(mrl.m_info[2].bytes
          == 8 * ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size
              + ara::core::pmr::internal::block_slist::header_size);
      remaining_storage = dmbr.remaining_storage(1u);
    }
  }
  // Derived_from_monotonic_buffer_resource dmbr(&mrl) is destroyed
  EXPECT_TRUE(mrl.m_mismatches == 0u);
  EXPECT_TRUE(mrl.m_info.size() == 0u);

  // Now use a local buffer
  {
    char buf[ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size];
    // Supply an external buffer
    derived_from_monotonic_buffer_resource dmbr(&buf, sizeof(buf), &mrl);
    EXPECT_TRUE(dmbr.remaining_storage(1u) == sizeof(buf));
    // Allocate all remaining storage
    dmbr.do_allocate(dmbr.remaining_storage(1u), 1u);
    // No new allocation should have occurred
    EXPECT_TRUE(mrl.m_info.size() == 0u);
    EXPECT_TRUE(dmbr.remaining_storage(1u) == 0u);
  }
  EXPECT_TRUE(mrl.m_mismatches == 0u);
  EXPECT_TRUE(mrl.m_info.size() == 0u);
}

void test_do_deallocate()
{
  memory_resource_logger mrl;
  const std::size_t initial_size = 1u;
  {
    derived_from_monotonic_buffer_resource dmbr(initial_size, &mrl);
    // First test, no buffer
    const unsigned iterations = 8;
    char* bufs[iterations];
    std::size_t sizes[iterations];
    // Test each iteration allocates memory
    for (unsigned i = 0; i != iterations; ++i)
    {
      sizes[i] = dmbr.remaining_storage() + 1;
      bufs[i] = (char*)dmbr.do_allocate(sizes[i], 1);
      EXPECT_TRUE(mrl.m_info.size() == (i + 1));
    }
    std::size_t remaining = dmbr.remaining_storage();
    // Test do_deallocate does not release any storage
    for (unsigned i = 0; i != iterations; ++i)
    {
      dmbr.do_deallocate(bufs[i], sizes[i], 1u);
      EXPECT_TRUE(mrl.m_info.size() == iterations);
      EXPECT_TRUE(remaining == dmbr.remaining_storage());
      EXPECT_TRUE(mrl.m_mismatches == 0u);
    }
  }
}

void test_do_is_equal()
{
  memory_resource_logger mrl;
  derived_from_monotonic_buffer_resource dmbr(&mrl);
  derived_from_monotonic_buffer_resource dmbr2(&mrl);
  EXPECT_TRUE(true == dmbr.do_is_equal(dmbr));
  EXPECT_TRUE(false == dmbr.do_is_equal(dmbr2));
  // A different type should be always different
  derived_from_memory_resource dmr;
  EXPECT_TRUE(false == dmbr.do_is_equal(dmr));
}

void test_release()
{
  {
    memory_resource_logger mrl;
    const std::size_t initial_size = 1u;
    derived_from_monotonic_buffer_resource dmbr(initial_size, &mrl);
    // First test, no buffer
    const unsigned iterations = 8;
    // Test each iteration allocates memory
    for (unsigned i = 0; i != iterations; ++i)
    {
      dmbr.do_allocate(dmbr.remaining_storage() + 1, 1);
      EXPECT_TRUE(mrl.m_info.size() == (i + 1));
    }
    // Release and check memory was released
    dmbr.release();
    EXPECT_TRUE(mrl.m_mismatches == 0u);
    EXPECT_TRUE(mrl.m_info.size() == 0u);
  }
  // Now use a local buffer
  {
    std::aligned_storage<ara::core::pmr::monotonic_buffer_resource::initial_next_buffer_size> buf;
    // Supply an external buffer
    ara::core::pmr::monotonic_buffer_resource monr(&buf, sizeof(buf));
    ara::core::pmr::memory_resource& mr = monr;
    EXPECT_TRUE(monr.remaining_storage(1u) == sizeof(buf));
    // Allocate all remaining storage
    mr.allocate(monr.remaining_storage(1u), 1u);
    EXPECT_TRUE(monr.current_buffer() == ((char*)&buf + sizeof(buf)));
    // No new allocation should have occurred
    EXPECT_TRUE(monr.remaining_storage(1u) == 0u);
    // Release and check memory was released and the original buffer is back
    monr.release();
    EXPECT_TRUE(monr.remaining_storage(1u) == sizeof(buf));
    EXPECT_TRUE(monr.current_buffer() == &buf);
  }
}

void test_destructor()
{
  memory_resource_logger mrl;
  const std::size_t initial_size = 1u;
  {
    derived_from_monotonic_buffer_resource dmbr(initial_size, &mrl);
    // First test, no buffer
    const unsigned iterations = 8;
    // Test each iteration allocates memory
    for (unsigned i = 0; i != iterations; ++i)
    {
      dmbr.do_allocate(dmbr.remaining_storage() + 1, 1);
      EXPECT_TRUE(mrl.m_info.size() == (i + 1));
    }
  }
  // Dmbr is destroyed, memory should be released
  EXPECT_TRUE(mrl.m_mismatches == 0u);
  EXPECT_TRUE(mrl.m_info.size() == 0u);
}

// Monotonic Buffer Resource Test
/*******************************************************************************
@TestCaseId:          UT_CORE_3035
@Test_Description:    To test the functionality of monotonic buffer resource.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634080, 634081, 634082, 634083, 634084, 634085, 634086, 634087, 634088, 634089, 634090
                      634091, 634092, 634093, 634094, 634095, 634096
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, monotonic_buffer_resource_test)
{
  test_block_chain::test_constructor();
  test_block_chain::test_allocate();
  test_block_chain::test_release();
  test_block_chain::test_memory_resource();
  test_block_chain::test_destructor();

  test_resource_constructor();
  test_initial_size_constructor();
  test_buffer_constructor();

  test_upstream_resource();
  /* @Start: UT_CORE_3035 */
  /* @TS1: Invoke upstream_resource() of monotonic_buffer_resource class. */
  /* @TE1: upstream_resource should return a pointer to the upstream memory resource. */
  /* @End: UT_CORE_3035 */
  test_do_allocate();
  /* @Start: UT_CORE_3035 */
  /* @TS2: Invoke do_allocate() of monotonic_buffer_resource class. */
  /* @TE2: do_allocate should return a pointer to allocated storage of at least bytes bytes in size. */
  /* @End: UT_CORE_3035 */
  test_do_deallocate();
  /* @Start: UT_CORE_3035 */
  /* @TS3: Invoke do_deallocate() of monotonic_buffer_resource class. */
  /* @TE3: do_deallocate deleted the allocated bytes. */
  /* @End: UT_CORE_3035 */
  test_do_is_equal();
  /* @Start: UT_CORE_3035 */
  /* @TS4: Invoke do_is_equal() of monotonic_buffer_resource class. */
  /* @TE4: do_is_equal returns the true. */
  /* @End: UT_CORE_3035 */
  test_release();
  /* @Start: UT_CORE_3035 */
  /* @TS5: Invoke release() of monotonic_buffer_resource class. */
  /* @TE5: release check memory was released. */
  /* @End: UT_CORE_3035 */
  test_destructor();
}

// Monotonic Buffer Resource Test

template <class InputIt, class UnaryFunction>
constexpr UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
{
  for (; first != last; ++first)
  {
    f(*first);
  }
  return f;
  // Implicit move since C++11
}

template <class InputIt, class Size, class UnaryFunction>
InputIt for_each_n(InputIt first, Size n, UnaryFunction f)
{
  for (Size i = 0; i < n; ++first, (void)++i)
  {
    f(*first);
  }
  return first;
}
#if 1
/*******************************************************************************
@TestCaseId:          UT_CORE_3036
@Test_Description:    To test the functionality of polymorphic allocator test resources.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634080, 634081, 634082
*******************************************************************************/
TEST(UPSTREAM_TEST, testa)
{
  auto buffer_size = 5;
  auto backup_size = 1000;

  auto buffer = std::make_unique<char[]>(buffer_size);
  auto backup = std::make_unique<char[]>(backup_size);

  std::fill_n(buffer.get(), buffer_size, '_');
  std::fill_n(backup.get(), backup_size, '_');

  ara::core::pmr::monotonic_buffer_resource upst(backup.get(), backup_size);

  ara::core::pmr::monotonic_buffer_resource pool(buffer.get(), buffer_size, &upst);

  ara::core::pmr::Vector<char> v(&pool);
  for (auto c = 'a'; c <= 'i'; ++c)
  {
    v.push_back(c);
  }

  std::cout << "main buffer contents:\n";
  for_each_n(buffer.get(), buffer_size, [idx = 0](auto x) mutable { std::cout << x << " "; });
  std::cout << "\nbackup buffer contents:\n";
  for_each_n(backup.get(), backup_size, [idx = 0](auto x) mutable { std::cout << x << " "; });
  std::cout << "\nvector contents:\n";
  for (auto vc : v)
  {
    std::cout << vc;
  }
  std::cout << std::endl;
  /* @Start: UT_CORE_3036 */
  /* @TS1: Invoke monotonic_buffer_resource class. */
  /* @TE1: Instance of monotonic_buffer_resource should be created. */
  /* @End: UT_CORE_3036 */
}
#endif

ara::core::pmr::memory_resource* GetMemoryResource()
{
  // Buffer on DS
  static ara::core::Array<std::uint32_t, 10000> buffer{};

  // Monotonic Buffer
  static ara::core::pmr::monotonic_buffer_resource monotonic_buffer{buffer.data(), buffer.size()};

  // Unsync pool(monotonic as a upstream buffer)
  static ara::core::pmr::unsynchronized_pool_resource unsync_pool{
      ara::core::pmr::pool_options(4, 32), &monotonic_buffer};

  return &unsync_pool;
}

struct derived_from_unsynchronized_pool_resource : public ara::core::pmr::unsynchronized_pool_resource
{
  explicit derived_from_unsynchronized_pool_resource(memory_resource* p) : unsynchronized_pool_resource(p)
  {
  }
  using ara::core::pmr::unsynchronized_pool_resource::do_allocate;
  using ara::core::pmr::unsynchronized_pool_resource::do_deallocate;
  using ara::core::pmr::unsynchronized_pool_resource::do_is_equal;
};

// Unsynchronized Pool Resource
/*******************************************************************************
@TestCaseId:          UT_CORE_3037
@Test_Description:    To test the functionality of init .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634085, 634080, 634081, 634097, 634038, 634081, 634045, 634056, 634055, 634063
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, Init)
{
  int buffer_test[1024];
  debug_resource default_dbg("default");
  {
    ara::core::pmr::monotonic_buffer_resource pool{&buffer_test, 1024, &default_dbg};
    debug_resource dbg("buffer", &pool);
    ara::core::pmr::unsynchronized_pool_resource resource(ara::core::pmr::pool_options(4, 32), &dbg);
    auto c = pushElements<ara::core::pmr::Vector<int>>(resource, 11, 22, 33, 44, 55, 66, 77, 88, 99);

    ara::core::pmr::pool_options opt(4, 32);
    ASSERT_EQ(4, resource.options().max_blocks_per_chunk);
    ASSERT_EQ(32, resource.options().largest_required_pool_block);
    /* @Start: UT_CORE_3037 */
    /* @TS1: Invoke options() of unsynchronized_pool_resource class. */
    /* @TE1.1: Value of first object should be equal to 4. */
    /* @TE1.2: Value of second object should be should be equal to 32. */
    /* @End: UT_CORE_3037 */
  }
  std::cout << buffer_test << std::endl;
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3038
@Test_Description:    To test the functionality of parameter constrctor with pool option.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634038, 634040, 634046, 634055,634056,634059, 634066
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, paracons1)
{
  ara::core::pmr::unsynchronized_pool_resource resource;
  ara::core::pmr::unsynchronized_pool_resource resource1(ara::core::pmr::pool_options(4, 32));
  std::size_t size = resource.pool_count();
  EXPECT_EQ(size, 9);
  /* @Start: UT_CORE_3038 */
  /* @TS1: Invoke pool_count() of unsynchronized_pool_resource class. */
  /* @TE1: Value of size should be equal to 9. */
  /* @End: UT_CORE_3038 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3039
@Test_Description:    To test the functionality of parameter constrctor with memory resource.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634085, 634080, 634081, 634097, 634039, 634058
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, paracons2)
{
  int buffer_test[1024];
  debug_resource default_dbg("default");
  ara::core::pmr::monotonic_buffer_resource pool{&buffer_test, 1024, &default_dbg};
  debug_resource dbg("buffer", &pool);
  ara::core::pmr::unsynchronized_pool_resource resource(&dbg);
  /* @Start: UT_CORE_3039 */
  /* @TS1: Create an ara::core::pmr::unsynchronized_pool_resource data type with resource variable. */
  /* @TE1: Instance of unsynchronized_pool_resource should be created. */
  /* @End: UT_CORE_3039 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3040
@Test_Description:    To test the functionality of release.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634038, 634043, 634057, 634061
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, release)
{
  ara::core::pmr::unsynchronized_pool_resource resource;
  EXPECT_NO_THROW(resource.release());
  /* @Start: UT_CORE_3040 */
  /* @TS1: Invoke release() of unsynchronized_pool_resource class. */
  /* @TE1: release() should return void. */
  /* @End: UT_CORE_3040 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3041
@Test_Description:    To test the functionality of upstream resource.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634038, 634044, 634057, 634062
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, upstream_resource)
{
  ara::core::pmr::unsynchronized_pool_resource resource;
  EXPECT_TRUE(resource.upstream_resource() != nullptr);
  /* @Start: UT_CORE_3041 */
  /* @TS1: Invoke upstream_resource() of unsynchronized_pool_resource class. */
  /* @TE1: upstream_resource() should return memory resouce. */
  /* @End: UT_CORE_3041 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3042
@Test_Description:    To test the functionality of do is equal.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634039, 634053, 634058
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, do_is_equal)
{
  memory_resource_logger mrl;
  derived_from_unsynchronized_pool_resource dmbr(&mrl);
  EXPECT_TRUE(true == dmbr.do_is_equal(dmbr));
  /* @Start: UT_CORE_3042 */
  /* @TS1: Invoke do_is_equal() of unsynchronized_pool_resource class. */
  /* @TE1: do_is_equal() should return true. */
  /* @End: UT_CORE_3042 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3043
@Test_Description:    To test the functionality of do allocate.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634039, 634051, 634058, 634064
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, do_allocate)
{
  memory_resource_logger mrl;
  derived_from_unsynchronized_pool_resource dmbr(&mrl);
  EXPECT_TRUE(0 == *(int*)dmbr.do_allocate(0, 1));
  /* @Start: UT_CORE_3043 */
  /* @TS1: Invoke do_allocate() of unsynchronized_pool_resource class. */
  /* @TE1: Value of do_allocate should be equal to 0. */
  /* @End: UT_CORE_3043 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3044
@Test_Description:    To test the functionality of do deallocate.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634039, 634052, 634051, 634058, 634065, 634064
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, do_deallocate)
{
  memory_resource_logger mrl;
  derived_from_unsynchronized_pool_resource dmbr(&mrl);
  EXPECT_NO_THROW(dmbr.do_deallocate(dmbr.do_allocate(0, 1), 0, 1));
  /* @Start: UT_CORE_3044 */
  /* @TS1: Invoke do_deallocate() of unsynchronized_pool_resource class. */
  /* @TE1: do_deallocate() should return void. */
  /* @End: UT_CORE_3044 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3045
@Test_Description:    To test the functionality of pool index with lower bytes.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634038, 634047, 634057, 634067
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, pool_index1)
{
  std::size_t bytes = 2;
  ara::core::pmr::unsynchronized_pool_resource resource;
  std::size_t size = resource.pool_index(bytes);
  EXPECT_EQ(size, 0);
  /* @Start: UT_CORE_3045 */
  /* @TS1: Invoke pool_index() of unsynchronized_pool_resource class. */
  /* @TE1: Value of pool_index should be equal to 0. */
  /* @End: UT_CORE_3045 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3046
@Test_Description:    To test the functionality of pool index with higher bytes.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634040, 634044, 634059, 634067
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, pool_index2)
{
  std::size_t bytes = 34;
  ara::core::pmr::unsynchronized_pool_resource resource(ara::core::pmr::pool_options(4, 32));
  std::size_t size = resource.pool_index(bytes);
  EXPECT_EQ(size, 2);
  /* @Start: UT_CORE_3046 */
  /* @TS1: Invoke pool_index() of unsynchronized_pool_resource class. */
  /* @TE1: Value of pool_index should be equal to 2. */
  /* @End: UT_CORE_3046 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3047
@Test_Description:    To test the functionality of pool block.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634038, 634049, 634057, 634067, 634069
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, pool_block)
{
  std::size_t pool_idx = 1;
  ara::core::pmr::unsynchronized_pool_resource resource;
  std::size_t size = resource.pool_block(pool_idx);
  EXPECT_EQ(size, 32);
  /* @Start: UT_CORE_3047 */
  /* @TS1: Invoke pool_block() of unsynchronized_pool_resource class. */
  /* @TE1: Value of pool_block should be equal to 32. */
  /* @End: UT_CORE_3047 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3048
@Test_Description:    To test the functionality of pool next blocks per chunk with null index.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634038, 634048, 
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, pool_next_blocks_per_chunk1)
{
  std::size_t pool_idx;
  ara::core::pmr::unsynchronized_pool_resource resource;
  std::size_t size = resource.pool_next_blocks_per_chunk(pool_idx);
  EXPECT_EQ(size, 1);
  /* @Start: UT_CORE_3048 */
  /* @TS1: Invoke pool_next_blocks_per_chunk() of unsynchronized_pool_resource class. */
  /* @TE1: Value of pool_next_blocks_per_chunk should be equal to 1. */
  /* @End: UT_CORE_3048 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3049
@Test_Description:    To test the functionality of pool next blocks per chunk with value index.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634039, 634051, 634048, 634058, 634068
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, pool_next_blocks_per_chunk2)
{
  std::size_t pool_idx = 1;
  memory_resource_logger mrl;
  derived_from_unsynchronized_pool_resource dmbr(&mrl);
  EXPECT_TRUE(0 == *(int*)dmbr.do_allocate(1, 1));
  /* @Start: UT_CORE_3049 */
  /* @TS1: Invoke do_allocate() of unsynchronized_pool_resource class. */
  /* @TE1: Value of do_allocate should be equal to 0. */
  /* @End: UT_CORE_3049 */
  std::size_t size = dmbr.pool_next_blocks_per_chunk(pool_idx);
  EXPECT_EQ(size, 1);
  /* @Start: UT_CORE_3049 */
  /* @TS2: Invoke pool_next_blocks_per_chunk() of unsynchronized_pool_resource class. */
  /* @TE2: Value of pool_next_blocks_per_chunk should be equal to 1. */
  /* @End: UT_CORE_3049 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3050
@Test_Description:    To test the functionality of pool cached blocks with pool index 1.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634038, 634050, 634057, 634070
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, pool_cached_blocks1)
{
  std::size_t pool_idx = 1;
  ara::core::pmr::unsynchronized_pool_resource resource;
  std::size_t size = resource.pool_cached_blocks(pool_idx);
  EXPECT_EQ(size, 0);
  /* @Start: UT_CORE_3050 */
  /* @TS1: Invoke pool_cached_blocks() of unsynchronized_pool_resource class. */
  /* @TE1: Value of pool_cached_blocks should be equal to 0. */
  /* @End: UT_CORE_3050 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3051
@Test_Description:    To test the functionality of pool cached blocks with memory resource.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634039, 634050, 634051, 634058, 634070
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, pool_cached_blocks2)
{
  std::size_t pool_idx = 1;
  memory_resource_logger mrl;
  derived_from_unsynchronized_pool_resource dmbr(&mrl);
  EXPECT_TRUE(0 == *(int*)dmbr.do_allocate(1, 1));
  /* @Start: UT_CORE_3051 */
  /* @TS1: Invoke do_allocate() of unsynchronized_pool_resource class. */
  /* @TE1: Value of do_allocate should be equal to 0. */
  /* @End: UT_CORE_3051 */
  std::size_t size = dmbr.pool_cached_blocks(pool_idx);
  EXPECT_EQ(size, 0);
  /* @Start: UT_CORE_3051 */
  /* @TS2: Invoke pool_cached_blocks() of unsynchronized_pool_resource class. */
  /* @TE2: Value of pool_cached_blocks should be equal to 0. */
  /* @End: UT_CORE_3051 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3052
@Test_Description:    To test the functionality of pool count.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634039, 634051, 634046, 634058, 634066
*******************************************************************************/
TEST(UNSYNCHRONIZED_POOL_RESOURCE_TEST, pool_count)
{
  memory_resource_logger mrl;
  derived_from_unsynchronized_pool_resource dmbr(&mrl);
  EXPECT_TRUE(0 == *(int*)dmbr.do_allocate(1, 1));
  /* @Start: UT_CORE_3052 */
  /* @TS1: Invoke do_allocate() of unsynchronized_pool_resource class. */
  /* @TE1: Value of do_allocate should be equal to 0. */
  /* @End: UT_CORE_3052 */
  std::size_t size = dmbr.pool_count();
  std::cout << "pool count" << size;
  EXPECT_EQ(size, 9);
  /* @Start: UT_CORE_3052 */
  /* @TS2: Invoke pool_count() of unsynchronized_pool_resource class. */
  /* @TE2: Value of pool_count should be equal to p. */
  /* @End: UT_CORE_3052 */
}

template <class Allocator>
class derived_from_polymorphic_allocator : public ara::core::pmr::__details::resource_adaptor_imp<Allocator>
{
 public:
  derived_from_polymorphic_allocator() = default;
  derived_from_polymorphic_allocator(const derived_from_polymorphic_allocator&) = default;
  template <class Allocator2>
  derived_from_polymorphic_allocator(Allocator2&& a2,
      typename std::enable_if<std::is_convertible<Allocator2, Allocator>::value, int>::type = 0);
  using ara::core::pmr::__details::resource_adaptor_imp<Allocator>::do_is_equal;
  using ara::core::pmr::__details::resource_adaptor_imp<Allocator>::do_deallocate;
  using ara::core::pmr::__details::resource_adaptor_imp<Allocator>::do_allocate;
};

/*******************************************************************************
@TestCaseId:          UT_CORE_3053
@Test_Description:    To test the functionality of do allocate.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      6340544, 634101, 634071, 634072
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, do_allocate)
{
  derived_from_polymorphic_allocator<std::allocator<int>> polyObj;
  EXPECT_TRUE(0 == *(int*)polyObj.do_allocate(64, 0));
  EXPECT_TRUE(0 == *(int*)polyObj.do_allocate(4, 4));
  EXPECT_TRUE(0 == *(int*)polyObj.do_allocate(32, 32));
  EXPECT_TRUE(0 == *(int*)polyObj.do_allocate(64, 64));
  EXPECT_TRUE(0 == *(int*)polyObj.do_allocate(128, 128));
  /* @Start: UT_CORE_3053 */
  /* @TS1: Invoke do_allocate() of polymorphic_allocator class. */
  /* @TE1: Value of do_allocate should be equal to 0. */
  /* @End: UT_CORE_3053 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3054
@Test_Description:    To test the functionality of do deallocate.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      6340544, 634101, 634071, 634072, 634073
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, do_deallocate)
{
  derived_from_polymorphic_allocator<std::allocator<int>> polyObj;
  EXPECT_NO_THROW(polyObj.do_deallocate(polyObj.do_allocate(64, 0), 64, 0));
  EXPECT_NO_THROW(polyObj.do_deallocate(polyObj.do_allocate(4, 4), 4, 4));
  EXPECT_NO_THROW(polyObj.do_deallocate(polyObj.do_allocate(32, 32), 32, 32));
  EXPECT_NO_THROW(polyObj.do_deallocate(polyObj.do_allocate(64, 64), 64, 64));
  EXPECT_NO_THROW(polyObj.do_deallocate(polyObj.do_allocate(128, 128), 128, 128));
  /* @Start: UT_CORE_3054 */
  /* @TS1: Invoke do_deallocate() of polymorphic_allocator class. */
  /* @TE1: do_deallocate should return void */
  /* @End: UT_CORE_3054 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3055
@Test_Description:    To test the functionality of do is equal.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634071, 634074
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, do_is_equal)
{
  test_resource mrl;
  derived_from_polymorphic_allocator<std::allocator<int>> polyObj;
  EXPECT_TRUE(false == polyObj.do_is_equal(mrl));
  /* @Start: UT_CORE_3055 */
  /* @TS1: Invoke do_is_equal() of polymorphic_allocator class. */
  /* @TE1: do_is_equal should return true */
  /* @End: UT_CORE_3055 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_3056
@Test_Description:    To test the functionality of null memory resource do_is_equal.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634075, 634078
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, null_memory_resource_do_is_equal)
{
  test_resource mrl;
  ara::core::pmr::null_memory_resource_imp obj;
  EXPECT_TRUE(false == obj.do_is_equal(mrl));
  /* @Start: UT_CORE_3056 */
  /* @TS1: Invoke do_is_equal() of polymorphic_allocator with null_memory_resource class. */
  /* @TE1: do_is_equal should return false. */
  /* @End: UT_CORE_3056 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3057
@Test_Description:    To test the functionality of null memory resource do deallocate.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          PmrTest
@Requirement Tracing: -
@Design Tracing:      634075, 634077
*******************************************************************************/
TEST(POLYMORPHIC_ALLOCATOR_TEST, null_memory_resource_do_deallocate)
{
  std::size_t bytes;
  std::size_t alignment;
  int pp = 1;
  void* p = &pp;
  ara::core::pmr::null_memory_resource_imp obj;
  /* @Start: UT_CORE_3057 */
  /* @TS1: Invoke do_deallocate() of polymorphic_allocator with null_memory_resource class. */
  /* @TE1: do_deallocate should return void. */
  /* @End: UT_CORE_3057 */
  EXPECT_NO_THROW(obj.do_deallocate(p, bytes, alignment));
}
