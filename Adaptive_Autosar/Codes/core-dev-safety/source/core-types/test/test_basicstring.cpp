// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_basicstring.cpp
//
// Purpose     : This file provides the unit test cases of class basic_string.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/string.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <memory>

using ::testing::Eq;
using ::testing::IsEmpty;
using ::testing::Not;

// An allocator for strings that mimics the behavior of std::allocator
class StatelessStringAllocator
{
 public:
  using size_type = size_t;
  using difference_type = ssize_t;
  using pointer = char*;
  using const_pointer = char const*;
  using reference = char&;
  using const_reference = char const&;
  using value_type = char;

  template <typename U>
  struct rebind
  {
    typedef StatelessStringAllocator other;
  };

  constexpr StatelessStringAllocator() noexcept = default;

  constexpr StatelessStringAllocator(StatelessStringAllocator const& other) noexcept = default;

  ~StatelessStringAllocator() = default;

  StatelessStringAllocator& operator=(StatelessStringAllocator const&) = default;

  pointer allocate(size_type n)
  {
    if (n == 0)
      return nullptr;
    pointer p = static_cast<char*>(::operator new(n));
    return p;
  }

  void deallocate(pointer p, size_t n)
  {
    static_cast<void>(n);
    // Note: p cannot be nullptr
    ::operator delete(p);
  }

  constexpr size_type max_size() const
  {
    return size_type(-1);
  }

  void construct(pointer p, const_reference val)
  {
    ::new (static_cast<void*>(p)) char(val);
  }

  void destroy(pointer p)
  {
    static_cast<void>(p);
  }
};

// Fine for the purpose of testing
using StatefulStringAllocator = StatelessStringAllocator;
/*******************************************************************************
@TestCaseId:          UT_CORE_2000
@Test_Description:    To test the functionality of type Availability.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          BasicStringTest
@Requirement Tracing: -
@Design Tracing:      611645
*******************************************************************************/
TEST(BasicStringTest, TypeIsAvailableAndCompilable)
{
  ara::core::basic_string<std::allocator<char>> s1;
  ara::core::basic_string<> s2;

  /* @Start: UT_CORE_2000 */
  /* @TS1: Create an instance of basic_string for char data type. */
  /* @TE1: Instance should be created. */
  /* @TS2: Create an instance of basic_string type. */
  /* @TE2: Instance should be created. */
  /* @End: UT_CORE_2000 */

  SUCCEED();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2001
@Test_Description:    To test the functionality of swap overload works.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          BasicStringTest
@Requirement Tracing: -
@Design Tracing:      611792
*******************************************************************************/
TEST(BasicStringTest, SwapOverloadWorks)
{
  ara::core::basic_string<> s1 = "The Quick Brown Fox";
  ara::core::basic_string<> s2 = "Jumps Over the Lazy Dog";

  ara::core::swap(s1, s2);

  ASSERT_THAT(s1, Eq("Jumps Over the Lazy Dog"));
  ASSERT_THAT(s2, Eq("The Quick Brown Fox"));
  /* @Start: UT_CORE_2001 */
  /* @TS1: Invoke the swap(). */
  /* @TE1: swap() should replace the data between the two basic_string objects. */
  /* @End: UT_CORE_2001 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2002
@Test_Description:    To test the functionality of construct with custom stateless allocator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          BasicStringTest
@Requirement Tracing: -
@Design Tracing:      611773
*******************************************************************************/
TEST(BasicStringTest, ConstructWithCustomStatelessAllocator)
{
  ara::core::basic_string<StatelessStringAllocator> s;

  ASSERT_THAT(s, IsEmpty());
  ASSERT_THAT(s, Eq(""));
  /* @Start: UT_CORE_2002 */
  /* @TS1: Invoke the IsEmpty(). */
  /* @TE1: IsEmpty() should return true. */
  /* @End: UT_CORE_2002 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2003
@Test_Description:    To test the functionality of construct with custom stateful allocator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          BasicStringTest
@Requirement Tracing: -
@Design Tracing:      611646
*******************************************************************************/
TEST(BasicStringTest, ConstructWithCustomStatefulAllocator)
{
  StatefulStringAllocator mem;

  ara::core::basic_string<StatefulStringAllocator> s(mem);
  ASSERT_THAT(s, IsEmpty());
  ASSERT_THAT(s, Eq(""));
  /* @Start: UT_CORE_2003 */
  /* @TS1: Invoke the IsEmpty(). */
  /* @TE1: IsEmpty() should return true. */
  /* @End: UT_CORE_2003 */
  ara::core::basic_string<StatefulStringAllocator> s2("The Quick Brown Fox", mem);
  ASSERT_THAT(s2, Not(IsEmpty()));
  ASSERT_THAT(s2, Eq("The Quick Brown Fox"));
  /* @Start: UT_CORE_2003 */
  /* @TS2: Invoke the IsEmpty(). */
  /* @TE2: IsEmpty() should not return true. */
  /* @End: UT_CORE_2003 */
}
