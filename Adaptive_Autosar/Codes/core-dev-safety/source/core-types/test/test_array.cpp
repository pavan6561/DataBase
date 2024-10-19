// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_array.cpp
//
// Purpose     : This file provides the unit test cases of class Array.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/array.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>
#include <utility>
#include <algorithm>
#include <tuple>
#include <type_traits>
#include <cstdint>

using ::testing::ContainerEq;
using ::testing::Each;
using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::Ne;
using ::testing::NotNull;

/*******************************************************************************
@TestCaseId:          UT_CORE_0001
@Test_Description:    To test the functionality of is_aggregate().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: - 
@Design Tracing:      611459
*******************************************************************************/
TEST(ArrayTest, StaticTypeProperties)
{
#ifdef __cpp_lib_is_aggregate
  bool isAggregate1 = std::is_aggregate < ara::core::Array<int, 2>::value;
  /* @Start: UT_CORE_0001 */
  /* @TS1: Get the value of is_aggregate. */
  /* @TE1: is_aggregate should return true. */
  /* @End: UT_CORE_0001 */
  EXPECT_TRUE(isAggregate1);

  bool isAggregate2 = std::is_aggregate < ara::core::Array<int, 0>::value;
  /* @Start: UT_CORE_0001 */
  /* @TS2: Get the value of is_aggregate. */
  /* @TE2: is_aggregate should return true. */
  /* @End: UT_CORE_0001 */
  EXPECT_TRUE(isAggregate2);
#endif

  SUCCEED();
}

// Helper type that is implicitly convertible to int
struct StringSizer final
{
  std::string const mText;

  explicit StringSizer(std::string const& text) : mText(text)
  {
  }

  operator int() const noexcept
  {
    return static_cast<int>(mText.size());
  }
};
/*******************************************************************************
@TestCaseId:          UT_CORE_0002
@Test_Description:    To test the functionality of size(),max_size() and empty() of four capacity Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611473, 611474, 611475
*******************************************************************************/
TEST(ArrayTest, Create)
{
  ara::core::Array<int, 4> a1;
  /* @Start: UT_CORE_0002 */
  /* @TS1: Call the size() of an Array. */
  /* @TE1: size() should be equal to 4. */
  /* @End: UT_CORE_0002 */
  EXPECT_THAT(a1.size(), Eq(4));
  /* @Start: UT_CORE_0002 */
  /* @TS2: Call the max_size() of an Array. */
  /* @TE2: max_size() should be equal to 4. */
  /* @End: UT_CORE_0002 */
  EXPECT_THAT(a1.max_size(), Eq(4));
  /* @Start: UT_CORE_0002 */
  /* @TS3: Call the empty() of an Array. */
  /* @TE3: empty() should return false. */
  /* @End: UT_CORE_0002 */
  EXPECT_THAT(a1.empty(), false);
  /* @Start: UT_CORE_0002 */
  /* @TS4: Call the size() of an Array. */
  /* @TE4: size() should be equal to 4. */
  /* @End: UT_CORE_0002 */
  constexpr ara::core::Array<int, 4> a2{1, 2, 3, 4};
  EXPECT_THAT(a2.size(), Eq(4));
  /* @Start: UT_CORE_0002 */
  /* @TS5: Call the size() of an Array. */
  /* @TE5: size() should be equal to 4. */
  /* @End: UT_CORE_0002 */
  constexpr ara::core::Array<int, 4> a3{1, 2, 3};
  EXPECT_THAT(a3.size(), Eq(4));
  /* @Start: UT_CORE_0002 */
  /* @TS6: Call the size() of an Array. */
  /* @TE6: size() should be equal to 4. */
  /* @End: UT_CORE_0002 */
  constexpr ara::core::Array<int, 4> a4 = {1, 2, 3, 4};
  EXPECT_THAT(a4.size(), Eq(4));
  // Can create from an initializer list "whose types are convertible to T".
  /* @Start: UT_CORE_0002 */
  /* @TS7: Call the size() of an Array. */
  /* @TE7: size() should be equal to 5. */
  /* @End: UT_CORE_0002 */
  ara::core::Array<int, 5> a5 = {1u, 2L, 3ul, 4uLL, StringSizer("hello, world")};
  EXPECT_THAT(a5.size(), Eq(5));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0003
@Test_Description:    To test the functionality of size(),max_size() and empty() of zero capacity Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611473, 611474, 611475
*******************************************************************************/
TEST(ArrayTest, CreateZeroSize)
{
  constexpr ara::core::Array<int, 0> a1;
  /* @Start: UT_CORE_0003 */
  /* @TS1: Call the size() of an Array. */
  /* @TE1: size() should be equal to 0. */
  /* @End: UT_CORE_0003 */
  EXPECT_THAT(a1.size(), Eq(0));
  /* @Start: UT_CORE_0003 */
  /* @TS2: Call the max_size() of an Array.  */
  /* @TE2: max_size() should be equal to 0. */
  /* @End: UT_CORE_0003 */
  EXPECT_THAT(a1.max_size(), Eq(0));
  /* @Start: UT_CORE_0003 */
  /* @TS3: Call the empty() of an Array. */
  /* @TE3: empty() should return true. */
  /* @End: UT_CORE_0003 */
  EXPECT_THAT(a1.empty(), true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0004
@Test_Description:    To test the functionality of Randoom Element of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611476
*******************************************************************************/
TEST(ArrayTest, RandomElementAccess)
{
  ara::core::Array<int, 4> a1{5, 6, 7, 8};
  int& e1 = a1[0];
  int& e2 = a1[1];
  int& e3 = a1[2];
  int& e4 = a1[3];
  /* @Start: UT_CORE_0004 */
  /* @TS1: Get the value of 0th index of an array. */
  /* @TE1: Reference variable of 0th index should be equal to 5. */
  /* @End: UT_CORE_0004 */
  EXPECT_THAT(e1, Eq(5));
  /* @Start: UT_CORE_0004 */
  /* @TS2: Get the value of 1st index of an array. */
  /* @TE2: Reference variable of 1st index should be equal to 6. */
  /* @End: UT_CORE_0004 */
  EXPECT_THAT(e2, Eq(6));
  /* @Start: UT_CORE_0004 */
  /* @TS3: Get the value of 2nd index of an array. */
  /* @TE3: Reference variable of 2nd index should be equal to 7. */
  /* @End: UT_CORE_0004 */
  EXPECT_THAT(e3, Eq(7));
  /* @Start: UT_CORE_0004 */
  /* @TS4: Get the value of 3rd index of an array. */
  /* @TE4: Reference variable of 3rd index should be equal to 8. */
  /* @End: UT_CORE_0004 */
  EXPECT_THAT(e4, Eq(8));

  constexpr ara::core::Array<int, 4> const a2{11, 12, 13, 14};
  constexpr int ce1 = a2[0];
  constexpr int ce2 = a2[1];
  constexpr int ce3 = a2[2];
  constexpr int ce4 = a2[3];
  /* @Start: UT_CORE_0004 */
  /* @TS5: Get the value of 0th index of an array. */
  /* @TE5: Reference variable of 0th index should be equal to 11. */
  /* @End: UT_CORE_0004 */
  EXPECT_THAT(ce1, Eq(11));
  /* @Start: UT_CORE_0004 */
  /* @TS6: Get the value of 0th index of an array. */
  /* @TE6: Reference variable of 1st index should be equal to 12. */
  /* @End: UT_CORE_0004 */
  EXPECT_THAT(ce2, Eq(12));
  /* @Start: UT_CORE_0004 */
  /* @TS7: Get the value of 0th index of an array. */
  /* @TE7: Reference variable of 2nd index should be equal to 13. */
  /* @End: UT_CORE_0004 */
  EXPECT_THAT(ce3, Eq(13));
  /* @Start: UT_CORE_0004 */
  /* @TS8: Get the value of 0th index of an array. */
  /* @TE8: Reference variable of 3rd index should be equal to 14. */
  /* @End: UT_CORE_0004 */
  EXPECT_THAT(ce4, Eq(14));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0005
@Test_Description:    To test the functionality of front(), back() elements of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611478, 611480
*******************************************************************************/
TEST(ArrayTest, FrontBackAccess)
{
  ara::core::Array<int, 4> a1{5, 6, 7, 8};
  /* @Start: UT_CORE_0005 */
  /* @TS1: Invoke front() of an Array. */
  /* @TE1: front() should be equal to 5. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a1.front(), Eq(5));
  /* @Start: UT_CORE_0005 */
  /* @TS2: Invoke back() of an Array. */
  /* @TE2: back() should be equal to 8. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a1.back(), Eq(8));
  ara::core::Array<int, 4> const a1b{5, 6, 7, 8};
  /* @Start: UT_CORE_0005 */
  /* @TS3: Invoke front() of an Array. */
  /* @TE3: front() should be equal to 5. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a1b.front(), Eq(5));
  /* @Start: UT_CORE_0005 */
  /* @TS4: Invoke back() of an Array.*/
  /* @TE4: back() should be equal to 8. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a1b.back(), Eq(8));

  ara::core::Array<int, 2> a2{11, 12};
  /* @Start: UT_CORE_0005 */
  /* @TS5: Invoke front() of an Array. */
  /* @TE5: front() should be equal to 11. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a2.front(), Eq(11));
  /* @Start: UT_CORE_0005 */
  /* @TS6: Invoke back() of an Array. */
  /* @TE6: back() should be equal to 12. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a2.back(), Eq(12));
  ara::core::Array<int, 2> const a2b{11, 12};
  /* @Start: UT_CORE_0005 */
  /* @TS7: Invoke front() of an Array. */
  /* @TE7: front() should be equal to 11. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a2b.front(), Eq(11));
  /* @Start: UT_CORE_0005 */
  /* @TS8: Invoke back() of an Array. */
  /* @TE8: back() should be equal to 12. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a2b.back(), Eq(12));

  ara::core::Array<int, 1> a3{21};
  /* @Start: UT_CORE_0005 */
  /* @TS9: Invoke front() of an Array. */
  /* @TE9: front() should be equal to 21. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a3.front(), Eq(21));
  /* @Start: UT_CORE_0005 */
  /* @TS10: Invoke back() of an Array. */
  /* @TE10: back() should be equal to 21. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a3.back(), Eq(21));
  ara::core::Array<int, 1> const a3b{21};
  /* @Start: UT_CORE_0005 */
  /* @TS11: Invoke front() of an Array. */
  /* @TE11: front() should be equal to 21. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a3b.front(), Eq(21));
  /* @Start: UT_CORE_0005 */
  /* @TS12: Invoke back() of an Array. */
  /* @TE12: back() should be equal to 21. */
  /* @End: UT_CORE_0005 */
  EXPECT_THAT(a3b.back(), Eq(21));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0006
@Test_Description:    To test the functionality of Data of random capacity Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611483                                                                                                                                                                                
*******************************************************************************/
TEST(ArrayTest, DataAccess)
{
  ara::core::Array<int, 4> a1{5, 6, 7, 8};
  /* @Start: UT_CORE_0006 */
  /* @TS1: Invoke data() of an Array. */
  /* @TE1: data() should not be return empty. */
  /* @End: UT_CORE_0006 */
  EXPECT_THAT(a1.data(), NotNull());

  ara::core::Array<int, 4> const a1b{5, 6, 7, 8};
  /* @Start: UT_CORE_0006 */
  /* @TS2: Invoke data() of an Array. */
  /* @TE2: data() should not be return empty. */
  /* @End: UT_CORE_0006 */
  EXPECT_THAT(a1b.data(), NotNull());

  // This is not mandated by the standard, but the Demonstrator shall do this.
  ara::core::Array<int, 0> a0;
  /* @Start: UT_CORE_0006 */
  /* @TS3: Invoke data() of an Array. */
  /* @TE3: data() should return empty. */
  /* @End: UT_CORE_0006 */
  EXPECT_THAT(a0.data(), IsNull());

  ara::core::Array<int, 0> const a0b(a0);
  /* @Start: UT_CORE_0006 */
  /* @TS4: Invoke data() of an Array. */
  /* @TE4: data() should return empty. */
  /* @End: UT_CORE_0006 */
  EXPECT_THAT(a0b.data(), IsNull());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0007
@Test_Description:    To test the functionality of Iterator begin(), end() of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611462, 611463, 611464, 611469, 611470
*******************************************************************************/
TEST(ArrayTest, IteratorBeginEnd)
{
  ara::core::Array<int, 4> a1{5, 6, 7, 8};
  /* @Start: UT_CORE_0007 */
  /* @TS1: Invoke begin() of an Array. */
  /* @TE1: begin() should be equal to data(). */
  /* @End: UT_CORE_0007 */
  EXPECT_THAT(a1.begin(), Eq(a1.data()));
  /* @Start: UT_CORE_0007 */
  /* @TS2: Invoke end() of an Array. */
  /* @TE2: end() should be equal to last element of data(). */
  /* @End: UT_CORE_0007 */
  EXPECT_THAT(a1.end(), Eq(a1.data() + 4));

  /* @Start: UT_CORE_0007 */
  /* @TS3: Invoke cbegin() of an Array. */
  /* @TE3: cbegin() should be equal to data(). */
  /* @End: UT_CORE_0007 */
  EXPECT_THAT(a1.cbegin(), Eq(a1.data()));
  /* @Start: UT_CORE_0007 */
  /* @TS4: Invoke cend() of an Array. */
  /* @TE4: cend() should be equal to lastt element of data().*/
  /* @End: UT_CORE_0007 */
  EXPECT_THAT(a1.cend(), Eq(a1.data() + 4));

  ara::core::Array<int, 4> const a1b{5, 6, 7, 8};
  /* @Start: UT_CORE_0007 */
  /* @TS5: Invoke begin() of an Array. */
  /* @TE5: begin() should be equal to data(). */
  /* @End: UT_CORE_0007 */
  EXPECT_THAT(a1b.begin(), Eq(a1b.data()));
  /* @Start: UT_CORE_0007 */
  /* @TS6: Invoke end() of an Array. */
  /* @TE6: end() should be equal to last element of data(). */
  /* @End: UT_CORE_0007 */
  EXPECT_THAT(a1b.end(), Eq(a1b.data() + 4));

  /* @Start: UT_CORE_0007 */
  /* @TS7: Invoke cbegin() of an Array. */
  /* @TE7: cbegin() should be equal to data(). */
  /* @End: UT_CORE_0007 */
  EXPECT_THAT(a1b.cbegin(), Eq(a1b.data()));
  /* @Start: UT_CORE_0007 */
  /* @TS8: Invoke cend() of an Array. */
  /* @TE8: cend() should be equal to last element of data(). */
  /* @End: UT_CORE_0007 */
  EXPECT_THAT(a1b.cend(), Eq(a1b.data() + 4));

  ara::core::Array<int, 5> a5 = {1u, 2L, 3ul, 4uLL, StringSizer("hello, world")};
  /* @Start: UT_CORE_0007 */
  /* @TS9: Invoke Data() of an array. */
  /* @TE9: Data() should return the array elements. */
  /* @End: UT_CORE_0007 */
  EXPECT_THAT(a5, ElementsAre(1, 2, 3, 4, 12));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0008
@Test_Description:    To test the functionality of Iterator begin(), end() of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611471, 611471, 611472
*******************************************************************************/
TEST(ArrayTest, IteratorRbeginRend)
{
  ara::core::Array<int, 4> a1{5, 6, 7, 8};
  /* @Start: UT_CORE_0008 */
  /* @TS1: Invoke rbegin() of an Array. */
  /* @TE1: rbegin() should be equal to 8. */
  /* @End: UT_CORE_0008 */
  EXPECT_THAT(*a1.rbegin(), Eq(8));
  auto it1 = a1.rend();
  --it1;
  /* @Start: UT_CORE_0008 */
  /* @TS2: Invoke Iterator() of an Array */
  /* @TE2: Iterator() should be equal to 5. */
  /* @End: UT_CORE_0008 */
  EXPECT_THAT(*it1, Eq(5));

  /* @Start: UT_CORE_0008 */
  /* @TS3: Invoke crbegin() of an Array. */
  /* @TE3: crbegin() should be equal to 8. */
  /* @End: UT_CORE_0008 */
  EXPECT_THAT(*a1.crbegin(), Eq(8));
  auto it1a = a1.crend();
  --it1a;
  /* @Start: UT_CORE_0008 */
  /* @TS4: Invoke Iterator() of an Array */
  /* @TE4: Iterator should be equal to 5. */
  /* @End: UT_CORE_0008 */
  EXPECT_THAT(*it1a, Eq(5));

  ara::core::Array<int, 4> const a2{5, 6, 7, 8};
  /* @Start: UT_CORE_0008 */
  /* @TS5: Invoke rbegin() of an Array. */
  /* @TE5: rbegin() should be equal to 8. */
  /* @End: UT_CORE_0008 */
  EXPECT_THAT(*a2.rbegin(), Eq(8));
  auto it2 = a2.rend();
  --it2;
  /* @Start: UT_CORE_0008 */
  /* @TS6: Invoke Iterator() of an Array */
  /* @TE6: Iterator should be equal to 5. */
  /* @End: UT_CORE_0008 */
  EXPECT_THAT(*it2, Eq(5));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0009
@Test_Description:    To test the functionality of Operators for sigle items of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      538244, 538245, 538246, 538247, 538248, 538249
*******************************************************************************/
TEST(ArrayTest, ComparisonOperatorsWorkForSingleItem)
{
  ara::core::Array<std::uint8_t, 1> const a1{13u};
  ara::core::Array<std::uint8_t, 1> const a2{13u};
  ara::core::Array<std::uint8_t, 1> const a3{37u};
  /* @Start: UT_CORE_0009 */
  /* @TS1: Invoke operator==() of an Array */
  /* @TE1: operator==() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator==(a1, a2), true);
  /* @Start: UT_CORE_0009 */
  /* @TS2: Invoke operator==() of an Array. */
  /* @TE2: operator==() should return false. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator==(a1, a3), false);

  /* @Start: UT_CORE_0009 */
  /* @TS3: Invoke operator!=() of an Array. */
  /* @TE3: operator!=() should return false. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator!=(a1, a2), false);
  /* @Start: UT_CORE_0009 */
  /* @TS4: Invoke operator!=() of an Array. */
  /* @TE4: operator!=() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator!=(a1, a3), true);

  /* @Start: UT_CORE_0009 */
  /* @TS5: Invoke operator<() of an Array. */
  /* @TE5: operator<() should return false. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator<(a1, a2), false);
  /* @Start: UT_CORE_0009 */
  /* @TS6: Invoke operator<() of an Array. */
  /* @TE6: operator<() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator<(a1, a3), true);
  /* @Start: UT_CORE_0009 */
  /* @TS7: Invoke operator<() of an Array. */
  /* @TE7: operator<() should return false. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator<(a3, a1), false);

  /* @Start: UT_CORE_0009 */
  /* @TS8: Invoke operator>() of an Array. */
  /* @TE8: operator>() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator>(a1, a2), false);
  /* @Start: UT_CORE_0009 */
  /* @TS9: Invoke operator>() of an Array. */
  /* @TE9: operator>() should return false. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator>(a1, a3), false);
  /* @Start: UT_CORE_0009 */
  /* @TS10: Invoke operator>() of an Array. */
  /* @TE10: operator>() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator>(a3, a1), true);

  /* @Start: UT_CORE_0009 */
  /* @TS11: Invoke operator<=() of an Array. */
  /* @TE11: operator<=() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator<=(a1, a2), true);
  /* @Start: UT_CORE_0009 */
  /* @TS12: Invoke operator<=() of an Array. */
  /* @TE12: operator<=() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator<=(a1, a3), true);
  /* @Start: UT_CORE_0009 */
  /* @TS13: Invoke operator<=() of an Array. */
  /* @TE13: operator<=() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator<=(a2, a1), true);
  /* @Start: UT_CORE_0009 */
  /* @TS14: Invoke operator<=() of an Array. */
  /* @TE14: operator<=() should return false. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator<=(a3, a1), false);

  /* @Start: UT_CORE_0009 */
  /* @TS15: Invoke operator>=() of an Array. */
  /* @TE15: operator>=() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator>=(a1, a2), true);
  /* @Start: UT_CORE_0009 */
  /* @TS16: Invoke operator>=() of an Array. */
  /* @TE16: operator>=() should return false. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator>=(a1, a3), false);
  /* @Start: UT_CORE_0009 */
  /* @TS17: Invoke operator>=() of an Array. */
  /* @TE17: operator>=() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator>=(a2, a1), true);
  /* @Start: UT_CORE_0009 */
  /* @TS18: Invoke operator>=() of an Array. */
  /* @TE18: operator>=() should return true. */
  /* @End: UT_CORE_0009 */
  EXPECT_THAT(ara::core::operator>=(a3, a1), true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0010
@Test_Description:    To test the functionality of Operators for multiple items of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611477, 611478
*******************************************************************************/
TEST(ArrayTest, ComparisonOperatorsWorkForMultipleItems)
{
  ara::core::Array<int, 3> const a1{10, 15, 20};
  ara::core::Array<int, 3> const a2{10, 15, 20};
  ara::core::Array<int, 3> const a3{10, 15, 21};
  ara::core::Array<int, 3> const a4{11, 15, 20};
  /* @Start: UT_CORE_0010 */
  /* @TS1: Invoke operator==() of an Array.  */
  /* @TE1: operator==() should return true. */
  /* @End: UT_CORE_0010 */
  EXPECT_THAT(ara::core::operator==(a1, a2), true);
  /* @Start: UT_CORE_0010 */
  /* @TS2: Invoke operator==() of an Array. */
  /* @TE2: operator==() should return false. */
  /* @End: UT_CORE_0010 */
  EXPECT_THAT(ara::core::operator==(a1, a3), false);
  /* @Start: UT_CORE_0010 */
  /* @TS3: Invoke operator==() of an Array.  */
  /* @TE3: operator==() should return false. */
  /* @End: UT_CORE_0010 */
  EXPECT_THAT(ara::core::operator==(a1, a4), false);
  /* @Start: UT_CORE_0010 */
  /* @TS4: Invoke operator!=() of an Array. */
  /* @TE4: operator!=() should return false. */
  /* @End: UT_CORE_0010 */
  EXPECT_THAT(ara::core::operator!=(a1, a2), false);
  /* @Start: UT_CORE_0010 */
  /* @TS5: Invoke operator!=() of an Array. */
  /* @TE5: operator!=() should return true. */
  /* @End: UT_CORE_0010 */
  EXPECT_THAT(ara::core::operator!=(a1, a3), true);
  /* @Start: UT_CORE_0010 */
  /* @TS6: Invoke operator!=() of an Array. */
  /* @TE6: operator!=() should return true. */
  /* @End: UT_CORE_0010 */
  EXPECT_THAT(ara::core::operator!=(a1, a4), true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0011
@Test_Description:    To test the functionality of Operators of zero capacity Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611812, 611813, 611814, 611815, 611816, 611817
*******************************************************************************/
TEST(ArrayTest, ComparisonOperatorsWorkForZeroSize)
{
  ara::core::Array<int, 0> const a1{};
  ara::core::Array<int, 0> const a2{};
  /* @Start: UT_CORE_0011 */
  /* @TS1: Invoke operator==() of an Array.  */
  /* @TE1: operator==() should return true. */
  /* @End: UT_CORE_0011 */
  EXPECT_THAT(ara::core::operator==(a1, a2), true);
  /* @Start: UT_CORE_0011 */
  /* @TS2: Invoke operator==() of an Array. */
  /* @TE2: operator!=() should return false. */
  /* @End: UT_CORE_0011 */
  EXPECT_THAT(ara::core::operator!=(a1, a2), false);
  /* @Start: UT_CORE_0011 */
  /* @TS3: Invoke operator<() of an Array. */
  /* @TE3: operator<() should return false. */
  /* @End: UT_CORE_0011 */
  EXPECT_THAT(ara::core::operator<(a1, a2), false);
  /* @Start: UT_CORE_0011 */
  /* @TS4: Invoke operator>() of an Array. */
  /* @TE4: operator>() should return false. */
  /* @End: UT_CORE_0011 */
  EXPECT_THAT(ara::core::operator>(a1, a2), false);
  /* @Start: UT_CORE_0011 */
  /* @TS5: Invoke operator<=() of an Array. */
  /* @TE5: operator<=() should return true. */
  /* @End: UT_CORE_0011 */
  EXPECT_THAT(ara::core::operator<=(a1, a2), true);
  /* @Start: UT_CORE_0011 */
  /* @TS6: Invoke operator>=() of an Array. */
  /* @TE6: operator>=() should return true. */
  /* @End: UT_CORE_0011 */
  EXPECT_THAT(ara::core::operator>=(a1, a2), true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0012
@Test_Description:    To test the functionality of ara::core::Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611482, 611483
*******************************************************************************/
TEST(ArrayTest, Fill)
{
  ara::core::Array<std::uint8_t, 4> actual{1u, 2u, 3u, 4u};
  ara::core::Array<std::uint8_t, 4> expected{42u, 42u, 42u, 42u};
  /* @Start: UT_CORE_0012 */
  /* @TS1: Invoke actual() of an Array. */
  /* @TE1: actual() should not be equal to expected. */
  /* @End: UT_CORE_0012 */
  EXPECT_THAT(actual, Ne(expected));

  actual.fill(42u);
  /* @Start: UT_CORE_0012 */
  /* @TS2: Invoke actual() of an Array. */
  /* @TE2: actual() should be equal to expected(). */
  /* @End: UT_CORE_0012 */
  EXPECT_THAT(actual, Each(42u));
}

// A custom type with its own (throwing) swap() overload.
// We use this to verify that our noexcept specifier for Array correclty propagates
// the noexcept property of T to Array<T>.
// If the noexcept specifier for Array is equivalent to 'true', the throw statement
// in the swap() overload here will result in std::terminate being called.
namespace test
{
struct ThrowingType
{
};

void swap(ThrowingType&, ThrowingType&)
{
  throw 42;
}

} // namespace test
/*******************************************************************************
@TestCaseId:          UT_CORE_0013
@Test_Description:    To test the functionality of members of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611460
*******************************************************************************/
TEST(ArrayTest, SwapMember)
{
  ara::core::Array<std::uint8_t, 3> a1{13u, 20u, 103u};
  ara::core::Array<std::uint8_t, 3> a2{13u, 20u, 103u};
  ara::core::Array<std::uint8_t, 3> a3{37u, 55u, 255u};

  /* @Start: UT_CORE_0013 */
  /* @TS1: Create instance of Array class. */
  /* @TS2: Compare the a1 and a3. */
  /* @TS3: swap the data of a3 into a1 and Compare the a1 and a2. */
  /* @TS4: Compare the a1 and a3. */
  /* @TE1: Instance of Array class should be initialized successfully. */
  /* @TE2: a1 should not be equal to a3. */
  /* @TE3: a1 should not be equal to a2. */
  /* @TE4: a1 should be equal to a3. */
  /* @End: UT_CORE_0013 */

  EXPECT_THAT(a1, ContainerEq(a2));
  EXPECT_THAT(a1, Ne(a3));

  a2.swap(a3);

  EXPECT_THAT(a1, Ne(a2));
  EXPECT_THAT(a1, ContainerEq(a3));

  // Test a case where ADL within the noexcept specifier for Array::swap() makes a difference.

  ara::core::Array<test::ThrowingType, 2> a4{test::ThrowingType{}, test::ThrowingType{}};
  ara::core::Array<test::ThrowingType, 2> a5{test::ThrowingType{}, test::ThrowingType{}};
  try
  {
    a4.swap(a5);
  }
  catch (int x)
  {
    SUCCEED();
    // if std::terminate() is called instead, the noexcept() specifier for Array got it wrong.
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0014
@Test_Description:    To test the functionality of ara::core::Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611818, 611483
*******************************************************************************/
TEST(ArrayTest, GlobalSwap)
{
  ara::core::Array<std::uint8_t, 3> a1{13u, 20u, 103u};
  ara::core::Array<std::uint8_t, 3> a2{13u, 20u, 103u};
  ara::core::Array<std::uint8_t, 3> a3{37u, 55u, 255u};
  /* @Start: UT_CORE_0014 */
  /* @TS1: Create instance of ara::core::Array as a1, a2, a3 and Compare a1 and a2.  */
  /* @TE1: a1 should be equal to a2. */
  /* @End: UT_CORE_0014 */
  EXPECT_THAT(a1, ContainerEq(a2));
  /* @Start: UT_CORE_0014 */
  /* @TS2: Compare a1 and a3. */
  /* @TE2: a1 should not be equal to a3. */
  /* @End: UT_CORE_0014 */
  EXPECT_THAT(a1, Ne(a3));

  ara::core::swap(a2, a3);
  /* @Start: UT_CORE_0014 */
  /* @TS3: swap the data between a2, a3 and Compare a1 and a2.  */
  /* @TE3: a1 should not be equal to a2. */
  /* @End: UT_CORE_0014 */
  EXPECT_THAT(a1, Ne(a2));
  /* @Start: UT_CORE_0014 */
  /* @TS4: Compare a1 and a3. */
  /* @TE4: a1 should be equal to a3. */
  /* @End: UT_CORE_0014 */
  EXPECT_THAT(a1, ContainerEq(a3));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0015
@Test_Description:    To test the functionality of actual(), expected() of an Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611461, 611463
*******************************************************************************/
TEST(ArrayTest, Sort)
{
  ara::core::Array<std::uint8_t, 4> actual{42u, 11u, 37u, 13u};
  ara::core::Array<std::uint8_t, 4> expected{11u, 13u, 37u, 42u};
  /* @Start: UT_CORE_0015 */
  /* @TS1: Invoke actual() of an Array. */
  /* @TE1: actual() should not be equal to expected. */
  /* @End: UT_CORE_0015 */
  EXPECT_THAT(actual, Ne(expected));

  std::sort(actual.begin(), actual.end());
  /* @Start: UT_CORE_0015 */
  /* @TS2: Invoke actual() an Array.  */
  /* @TE2: actual should be equal to expected */
  /* @End: UT_CORE_0015 */
  EXPECT_THAT(actual, ContainerEq(expected));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0016
@Test_Description:    To test the functionality of size() of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611473
*******************************************************************************/
TEST(ArrayTest, TupleSize)
{
  ara::core::Array<int, 0> a0;
  constexpr std::size_t s0 = std::tuple_size<decltype(a0)>::value;
  /* @Start: UT_CORE_0016 */
  /* @TS1: Invoke size() of an Array. */
  /* @TE1: Size() should be equal to 0. */
  /* @End: UT_CORE_0016 */
  EXPECT_THAT(s0, Eq(0));

  ara::core::Array<int, 4> a1{5, 6, 7, 8};
  constexpr std::size_t s1 = std::tuple_size<decltype(a1)>::value;
  /* @Start: UT_CORE_0016 */
  /* @TS2: Invoke size() of an Array. */
  /* @TE2: Size() should be equal to 4. */
  /* @End: UT_CORE_0016 */
  EXPECT_THAT(s1, Eq(4));

  constexpr ara::core::Array<int, 0> const a0b;
  constexpr std::size_t s0b = std::tuple_size<decltype(a0b)>::value;
  /* @Start: UT_CORE_0016 */
  /* @TS3: Invoke size() of an Array. */
  /* @TE3: Size() should be equal to 0. */
  /* @End: UT_CORE_0016 */
  EXPECT_THAT(s0b, Eq(0));

  constexpr ara::core::Array<int, 4> const a1b{5, 6, 7, 8};
  constexpr std::size_t s1b = std::tuple_size<decltype(a1b)>::value;
  /* @Start: UT_CORE_0016 */
  /* @TS4: Invoke size() of an Array. */
  /* @TE4: Size() should be equal to 4. */
  /* @End: UT_CORE_0016 */
  EXPECT_THAT(s1b, Eq(4));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0017
@Test_Description:    To test the functionality of Tuple element of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611458
*******************************************************************************/
TEST(ArrayTest, TupleElement)
{
  ara::core::Array<int, 4> a1{5, 6, 7, 8};
  using T0 = std::tuple_element<0, decltype(a1)>::type;
  using T1 = std::tuple_element<1, decltype(a1)>::type;
  /* @Start: UT_CORE_0017 */
  /* @TS1: Invoke tuple() of 0th element of an Array. */
  /* @TE1: tuple() should return true. */
  /* @End: UT_CORE_0017 */
  bool t0IsInt = std::is_same<T0, int>::value;
  EXPECT_TRUE(t0IsInt);
  /* @Start: UT_CORE_0017 */
  /* @TS2: Invoke tuple() of 1st element of an Array. */
  /* @TE2: tuple() should return true. */
  /* @End: UT_CORE_0017 */
  bool t1IsInt = std::is_same<T1, int>::value;
  EXPECT_TRUE(t1IsInt);

  ara::core::Array<int, 4> const a2{5, 6, 7, 8};
  using T2 = std::tuple_element<0, decltype(a2)>::type;
  /* @Start: UT_CORE_0017 */
  /* @TS3: Invoke tuple() of 2nd element of an Array. */
  /* @TE3: tuple() should return true. */
  /* @End: UT_CORE_0017 */
  bool t2IsInt = std::is_same<T2, int const>::value;
  EXPECT_TRUE(t2IsInt);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0018
@Test_Description:    To test the functionality of get() of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611819, 611820, 611821
*******************************************************************************/
TEST(ArrayTest, QualifiedGet)
{
  // template <size_t I, typename T, size_t N>
  // constexpr T& get(Array<T, N>& a) noexcept;

  ara::core::Array<int, 4> a1{5, 6, 7, 8};
  int& a1e1 = ara::core::get<0>(a1);
  int& a1e2 = ara::core::get<1>(a1);
  int& a1e3 = ara::core::get<2>(a1);
  int& a1e4 = ara::core::get<3>(a1);
  /* @Start: UT_CORE_0018 */
  /* @TS1: Get the value of 0th index of an array. */
  /* @TE1: Reference variable of 0th index should be equal to 5. */
  /* @End: UT_CORE_0018  */
  EXPECT_THAT(a1e1, Eq(5));
  /* @Start: UT_CORE_0018 */
  /* @TS2: Get the value of 1st index of an array. */
  /* @TE2: Reference variable of 1st index should be equal to 6. */
  /* @End: UT_CORE_0018  */
  EXPECT_THAT(a1e2, Eq(6));
  /* @Start: UT_CORE_0018 */
  /* @TS3: Get the value of 2nd index of an array. */
  /* @TE3: Reference variable of 2nd index should be equal to 7. */
  /* @End: UT_CORE_0018  */
  EXPECT_THAT(a1e3, Eq(7));
  /* @Start: UT_CORE_0018 */
  /* @TS4: Get the value of 3rd index of an array. */
  /* @TE4: Reference variable of 3rd index should be equal to 8. */
  /* @End: UT_CORE_0018  */
  EXPECT_THAT(a1e4, Eq(8));

  // template <size_t I, typename T, size_t N>
  // constexpr T const& get(Array<T, N> const& a) noexcept;
  static constexpr ara::core::Array<int, 4> const a2{5, 6, 7, 8};
  constexpr int const& a2e1 = ara::core::get<0>(a2);
  /* @Start: UT_CORE_0018 */
  /* @TS5: Get the value of 0th index of an array. */
  /* @TE5: Reference variable of 0th index should be equal to 5. */
  /* @End: UT_CORE_0018  */
  EXPECT_THAT(a2e1, Eq(5));

  // template <size_t I, typename T, size_t N>
  // constexpr T&& get(Array<T, N>&& a) noexcept;
  int a3e1 = ara::core::get<0>(ara::core::Array<int, 2>{1, 2});
  /* @Start: UT_CORE_0018 */
  /* @TS6: Get the value of 0th index of an array. */
  /* @TE6: Reference variable of 0th index should be equal to 1. */
  /* @End: UT_CORE_0018  */
  EXPECT_THAT(a3e1, Eq(1));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0019
@Test_Description:    To test the functionality of get() of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611819, 611820, 611821
*******************************************************************************/
TEST(ArrayTest, UnqualifiedGet)
{
  // Need to make a template declaration of get() visible, so that I can call
  // unqualified get() below and have ADL working, even with C++ below C++20.
  using std::get;

  // template <size_t I, typename T, size_t N>
  // constexpr T& get(Array<T, N>& a) noexcept;

  ara::core::Array<int, 4> a1{5, 6, 7, 8};
  int& a1e1 = get<0>(a1);
  int& a1e2 = get<1>(a1);
  int& a1e3 = get<2>(a1);
  int& a1e4 = get<3>(a1);
  /* @Start: UT_CORE_0019 */
  /* @TS1: Get the value of 0th index of an array. */
  /* @TE1: Reference variable of 0th index should be equal to 5. */
  /* @End: UT_CORE_0019 */
  EXPECT_THAT(a1e1, Eq(5));
  /* @Start: UT_CORE_0119 */
  /* @TS2: Get the value of 1st index of an array. */
  /* @TE2: Reference variable of 1st index should be equal to 6. */
  /* @End: UT_CORE_0019 */
  EXPECT_THAT(a1e2, Eq(6));
  /* @Start: UT_CORE_0019 */
  /* @TS3: Get the value of 2nd index of an array. */
  /* @TE3: Reference variable of 2nd index should be equal to 7. */
  /* @End: UT_CORE_0019 */
  EXPECT_THAT(a1e3, Eq(7));
  /* @Start: UT_CORE_0019 */
  /* @TS4: Get the value of 3rd index of an array. */
  /* @TE4: Reference variable of 0th index should be equal to 8. */
  /* @End: UT_CORE_0019 */
  EXPECT_THAT(a1e4, Eq(8));

  // template <size_t I, typename T, size_t N>
  // constexpr T const& get(Array<T, N> const& a) noexcept;
  static constexpr ara::core::Array<int, 4> const a2{5, 6, 7, 8};
  constexpr int const& a2e1 = get<0>(a2);
  /* @Start: UT_CORE_0019 */
  /* @TS5: Get the value of 0th index of an array. */
  /* @TE5: Reference variable of 0th index should be equal to 5. */
  /* @End: UT_CORE_0019 */
  EXPECT_THAT(a2e1, Eq(5));

  // template <size_t I, typename T, size_t N>
  // constexpr T&& get(Array<T, N>&& a) noexcept;
  int a3e1 = get<0>(ara::core::Array<int, 2>{1, 2});
  /* @Start: UT_CORE_0019 */
  /* @TS6: Get the value of 0th index of an array. */
  /* @TE6: a3e1 should be equal to 1. */
  /* @End: UT_CORE_0019 */
  EXPECT_THAT(a3e1, Eq(1));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0531
@Test_Description:    To test the functionality of Randoom Elements of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611477
*******************************************************************************/
TEST(ArrayTest, RandomElementAccessToOperator)
{
  //constexpr ConstReference operator[](SizeType n) const
  ara::core::Array<int, 4> const a1{11, 12, 13, 14};
  int e1 = a1[0];
  int e2 = a1[1];
  int e3 = a1[2];
  int e4 = a1[3];
  /* @Start: UT_CORE_0531 */
  /* @TS1: Get the value of 0th index of an array. */
  /* @TE1: Reference variable of 0th index should be equal to 11. */
  /* @End: UT_CORE_0531 */
  EXPECT_THAT(e1, Eq(11));
  /* @Start: UT_CORE_0531 */
  /* @TS2: Get the value of 1st index of an array. */
  /* @TE2: Reference variable of 1st index should be equal to 12. */
  /* @End: UT_CORE_0531 */
  EXPECT_THAT(e2, Eq(12));
  /* @Start: UT_CORE_0531 */
  /* @TS3: Get the value of 2nd index of an array. */
  /* @TE3: Reference variable of 2nd index should be equal to 13. */
  /* @End: UT_CORE_0531 */
  EXPECT_THAT(e3, Eq(13));
  /* @Start: UT_CORE_0531 */
  /* @TS4: Get the value of 3rd index of an array. */
  /* @TE4: Reference variable of 3rd index should be equal to 14. */
  /* @End: UT_CORE_0531 */
  EXPECT_THAT(e4, Eq(14));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0532
@Test_Description:    To test the functionality of Get() of Array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ArrayTest
@Requirement Tracing: -
@Design Tracing:      611819, 611820, 611821
*******************************************************************************/
TEST(ArrayTest, Get)
{
  using std::get;
  // template <size_t I, typename T, size_t N>
  // constexpr T const& get(Array<T, N> const& a) noexcept;
  ara::core::Array<int, 4> const a1{5, 6};
  int const& a1e1 = get<0>(a1);
  int const& a1e2 = get<1>(a1);
  /* @Start: UT_CORE_0532 */
  /* @TS1: Get the value of 0th index of an array. */
  /* @TE1: Reference variable of 0th index should be equal to 5. */
  /* @End: UT_CORE_0532 */
  EXPECT_THAT(a1e1, Eq(5));
  /* @Start: UT_CORE_0532 */
  /* @TS2: Get the value of 1st index of an array. */
  /* @TE2: Reference variable of 0th index should be equal to 6. */
  /* @End: UT_CORE_0532 */
  EXPECT_THAT(a1e2, Eq(6));
}