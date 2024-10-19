// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_span.cpp
//
// Purpose     : This file provides the unit test cases of class Span.
// -----------------------------------------------------------------------------------------------------------

#include "gmock_matchers.h"

#include "ara/core/span.h"
#include "ara/core/array.h"
#include "ara/core/vector.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <arpa/inet.h> // for htonl()

using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::Eq;
using ::testing::Ge;
using ::testing::Gt;
using ::testing::IsEmpty;
using ::testing::IsNull;
using ::testing::Le;
using ::testing::Lt;
using ::testing::Not;
using ::testing::NotNull;
using ::testing::SizeIs;
using ::testing::StartsWith;

using ara::core::dynamic_extent;
using ara::core::Span;

// ------------------------------------------------------------------------
// Constructors and assignment
/*******************************************************************************
@TestCaseId:          UT_CORE_0106
@Test_Description:    To test the functionality of Static Type Properties of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613227, 613191, 613207
*******************************************************************************/
TEST(SpanTest, StaticTypeProperties)
{
  std::size_t ex1 = Span<int>::stkExtent;
  EXPECT_THAT(ex1, Eq(dynamic_extent));

  std::size_t ex2 = Span<int, dynamic_extent>::stkExtent;
  EXPECT_THAT(ex2, Eq(dynamic_extent));

  std::size_t ex3 = Span<int, 0>::stkExtent;
  EXPECT_THAT(ex3, Eq(0));

  std::size_t ex4 = Span<int, 42>::stkExtent;
  EXPECT_THAT(ex4, Eq(42));
  /* @Start: UT_CORE_0106 */
  /* @TS1: Create an instance of Span by passing an empty capacity. */
  /* @TE1: value of Span::stkExtent should be equal to dynamic_extent. */
  /* @TS2: Create an instance of Span by passing capacity as dynamic_extent. */
  /* @TE2: value of Span::stkExtent should be equal to dynamic_extent. */
  /* @TS3: Create an instance of Span by passing capacity as 0. */
  /* @TE3: value of Span::stkExtent should be equal to 0. */
  /* @TS4: Create an instance of Span by passing capacity as 42. */
  /* @TE4: value of Span::stkExtent should be equal to 42. */
  /* @End: UT_CORE_0106 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0107
@Test_Description:    To test the functionality of IsSpan of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613227, 613191, 613207, 613190
*******************************************************************************/
TEST(SpanTest, IsSpanTrait)
{
  using T1 = Span<int>;

  constexpr bool b1 = ara::core::internal::IsAraCoreSpan<T1>::value;
  EXPECT_TRUE(b1);

  constexpr bool b2 = ara::core::internal::IsAraCoreSpan<T1&>::value;
  EXPECT_TRUE(b2);

  constexpr bool b3 = ara::core::internal::IsAraCoreSpan<T1&&>::value;
  EXPECT_TRUE(b3);

  constexpr bool b4 = ara::core::internal::IsAraCoreSpan<T1 const&&>::value;
  EXPECT_TRUE(b4);

  constexpr bool b5 = ara::core::internal::IsAraCoreSpan<int>::value;
  EXPECT_FALSE(b5);

  using T2 = Span<int, 42>;

  constexpr bool b11 = ara::core::internal::IsAraCoreSpan<T2>::value;
  EXPECT_TRUE(b11);
  /* @Start: UT_CORE_0107 */
  /* @TS1: Get the value of IsAraCoreSpan and pass the span object to it. */
  /* @TE1: value of IsAraCoreSpan should return true. */
  /* @TS2: Get the value of IsAraCoreSpan and copy the span object to it. */
  /* @TE2: value of IsAraCoreSpan should return true. */
  /* @TS3: Get the value of IsAraCoreSpan and move the span object to it. */
  /* @TE3: value of IsAraCoreSpan should return true. */
  /* @TS4: Get the value of IsAraCoreSpan and move the contant span object to it. */
  /* @TE4: value of IsAraCoreSpan should return true. */
  /* @TS5: Get the value of IsAraCoreSpan and pass the int to it. */
  /* @TE5: IsAraCoreSpan should return false. */
  /* @TS6: Get the value of IsAraCoreSpan and pass the span object to it. */
  /* @TE6: valuee of IsAraCoreSpan should return true. */
  /* @End: UT_CORE_0107 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0108
@Test_Description:    To test the functionality of IsArray of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613227, 613191, 613207, 613190
*******************************************************************************/
TEST(SpanTest, IsArrayTrait)
{
  using T1 = ara::core::Array<int, 4>;

  constexpr bool b1 = ara::core::internal::IsAraCoreArray<T1>::value;
  EXPECT_TRUE(b1);

  constexpr bool b2 = ara::core::internal::IsAraCoreArray<T1&>::value;
  EXPECT_TRUE(b2);

  constexpr bool b3 = ara::core::internal::IsAraCoreArray<T1&&>::value;
  EXPECT_TRUE(b3);

  constexpr bool b4 = ara::core::internal::IsAraCoreArray<T1 const&&>::value;
  EXPECT_TRUE(b4);

  constexpr bool b5 = ara::core::internal::IsAraCoreArray<int>::value;
  EXPECT_FALSE(b5);
  /* @Start: UT_CORE_0108 */
  /* @TS1: Get the value of IsAraCoreArray by passing the Array object. */
  /* @TE1: IsAraCoreArray should return value as true. */
  /* @TS2: Get the value of IsAraCoreArray by copying the Array object. */
  /* @TE2: IsAraCoreArray should return true. */
  /* @TS3: Get the value ofis_ara_core_array and move the Array object to it. */
  /* @TE3: IsAraCoreArray should return true. */
  /* @TS4: Get the value ofis_ara_core_array and move the constant Array object to it. */
  /* @TE4: IsAraCoreArray should return true. */
  /* @TS5: Get the value ofis_ara_core_array and pass the int to it.  */
  /* @TE5: IsAraCoreArray should return false. */
  /* @End: UT_CORE_0108 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0109
@Test_Description:    To test the functionality of IsSuitableContainer of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613206
*******************************************************************************/
TEST(SpanTest, IsSuitableContainerTrait)
{
  using C1 = ara::core::Vector<int>;

  constexpr bool b1 = ara::core::internal::IsSuitableContainer<C1, C1&, int>::value;
  EXPECT_TRUE(b1);

  constexpr bool b2 = ara::core::internal::IsSuitableContainer<C1, C1 const&, int const>::value;
  EXPECT_TRUE(b2);

  constexpr bool b3 = ara::core::internal::IsSuitableContainer<C1, C1&&, int const>::value;
  EXPECT_TRUE(b3);

  // "is not a specialization of Array"
  using CX = ara::core::Array<int, 4>;
  constexpr bool bX1 = ara::core::internal::IsSuitableContainer<CX, CX, int>::value;
  EXPECT_FALSE(bX1);
  constexpr bool bX2 = ara::core::internal::IsSuitableContainer<CX, CX&, int>::value;
  EXPECT_FALSE(bX2);
  constexpr bool bX3 = ara::core::internal::IsSuitableContainer<CX, CX const&, int const>::value;
  EXPECT_FALSE(bX3);

  /* @Start: UT_CORE_0109 */
  /* @TS1: Get the value of IsSuitableContainer and pass the Vector object to it. */
  /* @TE1: IsSuitableContainer should return true. */
  /* @TS2: Get the value of IsSuitableContainer and pass the Vector object to it. */
  /* @TE2: IsSuitableContainer should return true. */
  /* @TS3: Get the value of IsSuitableContainer and pass the Vector object to it. */
  /* @TE3: IsSuitableContainer should return true. */
  /* @TS4: Get the value ofis_suitable_container and pass the Arrayobject to it. */
  /* @TE4: IsSuitableContainer should return false. */
  /* @TS5: Get the value ofis_suitable_container and pass the Arrayobject to it. */
  /* @TE5: IsSuitableContainer should return false. */
  /* @TS6: Get the value ofis_suitable_container and pass the Arrayobject to it. */
  /* @TE6: IsSuitableContainer should return false. */
  /* @End: UT_CORE_0109 */
}

TEST(SpanTest, DefaultConstructor)
{
  // Span<int, 1> sp4;  // shall not compile! (disabled via SFINAE)
  // Span<int, -2> sp5;  // shall not compile! (disabled via SFINAE)
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0111
@Test_Description:    To test the functionality of Pointer of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613216
*******************************************************************************/
TEST(SpanTest, ConstructFromPointerWithSize)
{
  int buffer[4] = {1, 2, 3, 4};
  int const cbuffer[4] = {10, 11, 12, 13};

  Span<int> sp1(&buffer[0], 4);
  EXPECT_THAT(sp1, Not(IsEmpty()));
  EXPECT_THAT(sp1.data(), NotNull());
  EXPECT_THAT(sp1.size(), Eq(4));
  EXPECT_THAT(sp1, ElementsAre(1, 2, 3, 4));
  /* @Start: UT_CORE_0111 */
  /* @TS1: Create the instance of span and invoke the empty(). */
  /* @TE1: empty() should return true. */
  /* @TS2: Get the value ofdata(). */
  /* @TE2: data() should return NotNull. */
  /* @TS3: Get the value of size(). */
  /* @TE3: size() should be equal to 0. */
  /* @TS4: Get the value of array to the span. */
  /* @TE4: span elments should be equal to the array elements. */
  /* @End: UT_CORE_0111 */

  Span<int> sp2(&buffer[0], 3);
  EXPECT_THAT(sp2, Not(IsEmpty()));
  EXPECT_THAT(sp2.data(), NotNull());
  EXPECT_THAT(sp2.size(), Eq(3));
  EXPECT_THAT(sp2, ElementsAre(1, 2, 3));
  /* @Start: UT_CORE_0111 */
  /* @TS5: Create the instance of span and invoke the empty(). */
  /* @TE5: empty() should return true. */
  /* @TS6: Get the value of data(). */
  /* @TE6: data() should return NotNull. */
  /* @TS7: Get the value ofsize(). */
  /* @TE7: size() should be equal to 3. */
  /* @TS8: Get the value of array to the span. */
  /* @TE8: span elments should be equal to the array elements. */
  /* @End: UT_CORE_0111 */

  Span<int, 3> sp3(&buffer[1], 3);
  EXPECT_THAT(sp3, Not(IsEmpty()));
  EXPECT_THAT(sp3.data(), NotNull());
  EXPECT_THAT(sp3.size(), Eq(3));
  EXPECT_THAT(sp3, ElementsAre(2, 3, 4));
  /* @Start: UT_CORE_0111 */
  /* @TS9: Create the instance of span and Get the value ofempty(). */
  /* @TE9: empty() should return true. */
  /* @TS10: Get the value ofdata(). */
  /* @TE10: data() should return NotNull. */
  /* @TS11: Get the value ofsize(). */
  /* @TE11: size() should be equal to 3. */
  /* @TS12: Get the value of array to the span. */
  /* @TE12: span elments should be equal to the array elements. */
  /* @End: UT_CORE_0111 */
#ifndef NDEBUG
  // Helper function for calling a multi-argument constructor that assert()s.
  // The EXPECT_DEATH() macro cannot cope with the ctor call we need here
  // because of the commas contained therein.
  auto f = [&buffer] { Span<int, 4> sp(&buffer[0], 3); };
  EXPECT_DEATH(f(), "inconsistent stkExtent");
#endif

  Span<int const, 3> sp4(&cbuffer[1], 3);
  EXPECT_THAT(sp4, ElementsAre(11, 12, 13));

  // Span<int, 3> sp5(&cbuffer[1], 3);  // shall not compile!
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0112
@Test_Description:    To test the functionality of EndPointer of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613227, 613191, 613207, 613190
*******************************************************************************/
TEST(SpanTest, ConstructFromStartEndPointer)
{
  int buffer[4] = {1, 2, 3, 4};
  int const cbuffer[4] = {10, 11, 12, 13};

  Span<int> sp1(&buffer[0], &buffer[4]);
  EXPECT_THAT(sp1, ElementsAre(1, 2, 3, 4));

  Span<int> sp2(&buffer[0], &buffer[3]);
  EXPECT_THAT(sp2, ElementsAre(1, 2, 3));

  Span<int, 3> sp3(&buffer[1], &buffer[4]);
  EXPECT_THAT(sp3, ElementsAre(2, 3, 4));

#ifndef NDEBUG
  auto f = [&buffer] { Span<int, 4> sp(&buffer[0], &buffer[3]); };
  EXPECT_DEATH(f(), "inconsistent stkExtent");
#endif

  Span<int const, 3> sp4(&cbuffer[1], &cbuffer[4]);
  EXPECT_THAT(sp4, ElementsAre(11, 12, 13));
  /* @Start: UT_CORE_0112 */
  /* @TS1: Create the instance of span and pass an array to it. */
  /* @TE1: Span should be copy the array elements into the span from the given range. */
  /* @TS2: Create the instance of span of constant elments and pass constant array to it. */
  /* @TE2: Span should be copy the array elements into the span from the given range. */
  /* @End: UT_CORE_0112 */

  // Span<int, 3> sp5(&cbuffer[1], &cbuffer[4]);  // shall not compile!
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0113
@Test_Description:    To test the functionality of raw Array of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613202, 609894
*******************************************************************************/
TEST(SpanTest, ConstructFromRawArray)
{
  int buffer[4] = {1, 2, 3, 4};
  int const cbuffer[4] = {10, 11, 12, 13};

  Span<int> sp1(buffer);
  /* @Start: UT_CORE_0113 */
  /* @TS1: Create the instance of span and pass an array to the span. */
  /* @TE1: sp1 elments should be equal to the array elements. */
  /* @End: UT_CORE_0113 */
  EXPECT_THAT(sp1, ElementsAre(1, 2, 3, 4));

  Span<int const> sp2(buffer);
  /* @Start: UT_CORE_0113 */
  /* @TS2: Create the instance of span and pass an array to the span. */
  /* @TE2: sp2 elments should be equal to the array elements. */
  /* @End: UT_CORE_0113 */
  EXPECT_THAT(sp2, ElementsAre(1, 2, 3, 4));

  Span<int const> sp3(cbuffer);
  /* @Start: UT_CORE_0113 */
  /* @TS3: Create the instance of span and pass an array to the span. */
  /* @TE3: sp3 elments should be equal to the array elements. */
  /* @End: UT_CORE_0113 */
  EXPECT_THAT(sp3, ElementsAre(10, 11, 12, 13));

  // Span<int> sp4(cbuffer);  // shall not compile!

  Span<int, 4> sp5(buffer);
  /* @Start: UT_CORE_0113 */
  /* @TS4: Create the instance of span and pass an array to the span. */
  /* @TE4: sp5 elments should be equal to the array elements. */
  /* @End: UT_CORE_0113 */
  EXPECT_THAT(sp5, ElementsAre(1, 2, 3, 4));

  Span<int const, 4> sp6(cbuffer);
  /* @Start: UT_CORE_0113 */
  /* @TS5: Create the instance of span and pass an array to the span. */
  /* @TE5: sp6 elments should be equal to the array elements. */
  /* @End: UT_CORE_0113 */
  EXPECT_THAT(sp6, ElementsAre(10, 11, 12, 13));

  // Span<int, 5> sp7(buffer);  // shall not compile (mismatching size)

  // Span<int const, 5> sp8(cbuffer);  // shall not compile (mismatching size)
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0114
@Test_Description:    To test the functionality of Array class of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613202
*******************************************************************************/
TEST(SpanTest, ConstructFromArrayClass)
{
  ara::core::Array<int, 4> array{1, 2, 3, 4};
  ara::core::Array<int, 4> const carray{10, 11, 12, 13};

  Span<int> sp1(array);
  EXPECT_THAT(sp1, ElementsAre(1, 2, 3, 4));

  Span<int const> sp2(array);
  EXPECT_THAT(sp2, ElementsAre(1, 2, 3, 4));

  Span<int const> sp3(carray);
  EXPECT_THAT(sp3, ElementsAre(10, 11, 12, 13));

  // Span<int> sp4(carray);  // shall not compile!

  Span<int, 4> sp5(array);
  EXPECT_THAT(sp5, ElementsAre(1, 2, 3, 4));

  Span<int const, 4> sp6(carray);
  EXPECT_THAT(sp6, ElementsAre(10, 11, 12, 13));

  /* @Start: UT_CORE_0114 */
  /* @TS1: Create the instance of span and pass an array to the span. */
  /* @TE1: span elments should be equal to the array elements. */
  /* @TS2: Create the instance of const span and pass an const array to the span. */
  /* @TE2: span elments should be equal to the array elements. */
  /* @End: UT_CORE_0114 */

  // Span<int, 5> sp7(array); // shall not compile!

  // Span<int const, 5> sp8(carray); // shall not compile!
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0115
@Test_Description:    To To test the functionality of Container of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613205, 613206, 609894
*******************************************************************************/
TEST(SpanTest, ConstructFromContainer)
{
  ara::core::Vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);

  ara::core::Vector<int> const cvec{11};

  Span<int> sp1(vec);
  /* @Start: UT_CORE_0115 */
  /* @TS1: Create the instance of span and pass Vector object to it. */
  /* @TE1: sp1 elments should be equal to the Vector object. */
  /* @End: UT_CORE_0115 */
  EXPECT_THAT(sp1, ElementsAre(1, 2, 3, 4));

  Span<int const> sp2(vec);
  /* @Start: UT_CORE_0115 */
  /* @TS2: Create the instance of span and pass Vector object to it. */
  /* @TE2: sp2 elments should be equal to the Vector object. */
  /* @End: UT_CORE_0115 */
  EXPECT_THAT(sp2, ElementsAre(1, 2, 3, 4));

  Span<int const> sp3(cvec);

  /* @Start: UT_CORE_0115 */
  /* @TS3: Create the instance of span and pass Vector object to it. */
  /* @TE3: sp3 elments should be equal to the Vector object. */
  /* @End: UT_CORE_0115 */
  // EXPECT_THAT(sp3, ElementsAre(10, 11, 12, 13));

  // Span<int> sp4(cvec);  // shall not compile!

  Span<int, 4> sp5(vec);
  /* @Start: UT_CORE_0115 */
  /* @TS4: Create the instance of span and pass Vector object to it. */
  /* @TE4: sp5 elments should be equal to the Vector object. */
  /* @End: UT_CORE_0115 */
  EXPECT_THAT(sp5, ElementsAre(1, 2, 3, 4));

#ifndef NDEBUG
  auto f = [&vec] { Span<int, 5> sp(vec); };
  EXPECT_DEATH(f(), "inconsistent stkExtent");
#endif
}
template <typename T>
struct MyMinimalContainer
{
  using pointer = T*;

  T buf[4] = {1, 2, 3, 4};

  T const* data() const
  {
    return buf;
  }
  T* data()
  {
    return buf;
  }

  size_t size() const
  {
    return 4;
  }
};

/*******************************************************************************
@TestCaseId:          UT_CORE_0116
@Test_Description:    To test the functionality of Container of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613205, 613206, 609894
*******************************************************************************/
TEST(SpanTest, ConstructFromCustomContainer)
{
  MyMinimalContainer<int> cont;

  Span<int> sp1(cont);
  /* @Start: UT_CORE_0116 */
  /* @TS1: Create an instance of span and pass the structure object to it. */
  /* @TE1: sp1 elments should be equal to the structure object elements. */
  /* @End: UT_CORE_0116 */
  EXPECT_THAT(sp1, ElementsAre(1, 2, 3, 4));

  Span<const int> sp2(cont);
  /* @Start: UT_CORE_0116 */
  /* @TS2: Create an instance of const span and pass the structure object to it. */
  /* @TE2: sp2 elments should be equal to the structure object elements. */
  /* @End: UT_CORE_0116 */
  EXPECT_THAT(sp2, ElementsAre(1, 2, 3, 4));

  ara::core::StringView sv = "Quick";
  /* @Start: UT_CORE_0116 */
  /* @TS3: Create an instance of span and pass the StringView to it. */
  /* @TE3: sp3 elments should be equal to the StringView elements. */
  /* @End: UT_CORE_0116 */
  Span<char const> sp3(sv);

  ara::core::String str = "Quick";
  /* @Start: UT_CORE_0116 */
  /* @TS4: Create an instance of span and pass the StringView to it. */
  /* @TE4: sp4 elments should be equal to the StringView elements. */
  /* @End: UT_CORE_0116 */
  Span<char const> sp4(str);

  // only compilable with C++17 extensions for String (ie. with non-const data())
  // Span<char> sp5(str);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0117
@Test_Description:    To test the functionality of Container of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613205, 613206, 609894
*******************************************************************************/
TEST(SpanTest, ConstructFromCustomContainerOfConstValues)
{
  MyMinimalContainer<int const> cont;

  Span<int const> sp1(cont);
  EXPECT_THAT(sp1, ElementsAre(1, 2, 3, 4));

  /* @Start: UT_CORE_0117 */
  /* @TS1: Create an instance of const span and pass the structure const object to it. */
  /* @TE1: span elments should be equal to the structure object elements. */
  /* @End: UT_CORE_0117 */

  // Span<int> sp2(cont);  // OK, does not compile!
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0118
@Test_Description:    To test the functionality of Container of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613205, 613206, 609894
*******************************************************************************/
TEST(SpanTest, ConstructFromConstCustomContainer)
{
  MyMinimalContainer<int> const cont;

  Span<int const> sp1(cont);
  EXPECT_THAT(sp1, ElementsAre(1, 2, 3, 4));

  /* @Start: UT_CORE_0118 */
  /* @TS1: Create an instance of const span and pass the structure const object to it. */
  /* @TE1: span elments should be equal to the structure object elements. */
  /* @End: UT_CORE_0118 */
  // Span<int> sp2(cont);  // shall not compile!
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0119
@Test_Description:    To test the functionality of of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613227, 613191, 613207, 613190
*******************************************************************************/
TEST(SpanTest, CopyConstructAndAssign)
{
  // non-const items, dynamic stkExtent

  int buffer[4] = {1, 2, 3, 4};
  int tmp[4] = {5, 6, 7, 8};

  Span<int> sp1;
  Span<int> sp2(buffer);

  Span<int> sp4(sp2);
  EXPECT_THAT(sp4, ElementsAre(1, 2, 3, 4));

  sp4 = sp1;
  EXPECT_THAT(sp4, IsEmpty());

  /* @Start: UT_CORE_0119 */
  /* @TS1: Create an instance of span sp2 and pass an array to it. */
  /* @TE1: sp2 should be have the same elments as an array. */
  /* @TS2: Create an instance of span sp4 and pass sp2 to it. */
  /* @TE2: sp4 should be have the same elments as sp2. */
  /* @End: UT_CORE_0119 */
  // non-const items, static stkExtent

  Span<int, 4> sp6(buffer);
  Span<int, 4> sp7(tmp);

  Span<int, 4> sp8(sp6);
  EXPECT_THAT(sp8, ElementsAre(1, 2, 3, 4));

  sp8 = sp7;
  EXPECT_THAT(sp8, ElementsAre(5, 6, 7, 8));

  // const items, dynamic stkExtent

  int const cbuffer[4] = {10, 11, 12, 13};
  int const ctmp[4] = {5, 6, 7, 8};

  Span<int const> sp10;
  Span<int const> sp11(cbuffer);

  Span<int const> sp12(sp11);
  EXPECT_THAT(sp12, ElementsAre(10, 11, 12, 13));

  sp12 = sp10;
  EXPECT_THAT(sp12, IsEmpty());

  // const items, static stkExtent

  Span<int const, 4> sp13(cbuffer);
  Span<int const, 4> sp14(ctmp);

  Span<int const, 4> sp15(sp13);
  EXPECT_THAT(sp15, ElementsAre(10, 11, 12, 13));

  sp15 = sp14;
  EXPECT_THAT(sp15, ElementsAre(5, 6, 7, 8));

  /* @Start: UT_CORE_0119 */
  /* @TS3: Create an instance of span s6 and pass an array to it. */
  /* @TE3: s6 should be have the same elments as an array. */
  /* @TS4: Create an instance of span s7 and pass an array to it. */
  /* @TE4: s7 should be have the same elments as an array. */
  /* @TS5: Create an instance of span s8 and pass s6 to it. */
  /* @TE5: s8 should be have the same elments as s6. */
  /* @End: UT_CORE_0119 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0120
@Test_Description:    To test the functionality of Converting Constructor of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613227, 613191, 613207, 613190
*******************************************************************************/
TEST(SpanTest, ConvertingConstruct)
{
  int buffer[4] = {1, 2, 3, 4};
  Span<int, 4> sp1(buffer);
  Span<int> sp2(buffer);
  Span<int> sp5;
  Span<int, 0> sp6;

  Span<int> sp10(sp1);
  EXPECT_THAT(sp10, ElementsAre(1, 2, 3, 4));

  Span<int const> sp11(sp2);
  EXPECT_THAT(sp11, ElementsAre(1, 2, 3, 4));

  Span<int const> sp12(sp1);
  EXPECT_THAT(sp12, ElementsAre(1, 2, 3, 4));

  Span<int const, 4> sp13(sp1);
  EXPECT_THAT(sp13, ElementsAre(1, 2, 3, 4));

  Span<int> sp14(sp6);
  EXPECT_THAT(sp14, IsEmpty());

  Span<int const> sp15(sp5);
  EXPECT_THAT(sp15, IsEmpty());

  /* @Start: UT_CORE_0120 */
  /* @TS1: Create an instance of span and pass an array to it. */
  /* @TE1: span elments should be equal to array elements. */
  /* @End: UT_CORE_0120 */

  // Span<int, 4> sp14(sp2);  // shall not compile!
  // Span<int const, 4> sp15(sp2);  // shall not compile!
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0121
@Test_Description:    To test the functionality of first element of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613227, 613191, 613207, 613190
*******************************************************************************/
TEST(SpanTest, First)
{
  int buffer[4] = {1, 2, 3, 4};
  Span<int, 4> sp1(buffer);
  Span<int> sp2(buffer);

  Span<int, 2> sp10 = sp1.first<2>();
  EXPECT_THAT(sp10, ElementsAre(1, 2));

  Span<int, 0> sp11 = sp1.first<0>();
  EXPECT_THAT(sp11, IsEmpty());

#ifndef NDEBUG
  EXPECT_DEATH(sp1.first<5>(), "Count <= size()");
#endif

  Span<int> sp12 = sp1.first(3);
  EXPECT_THAT(sp12, ElementsAre(1, 2, 3));

  Span<int> sp13 = sp1.first(0);
  EXPECT_THAT(sp13, IsEmpty());

  Span<int> sp14 = sp2.first(3);
  EXPECT_THAT(sp14, ElementsAre(1, 2, 3));

  Span<int, 3> sp15 = sp2.first<3>();
  EXPECT_THAT(sp15, ElementsAre(1, 2, 3));

  Span<int, 0> sp16 = sp2.first<0>();
  EXPECT_THAT(sp16, IsEmpty());

#ifndef NDEBUG
  EXPECT_DEATH(sp2.first<5>(), "Count <= size()");
#endif

  /* @Start: UT_CORE_0121 */
  /* @TS1: place the first two elements of span into the another 2 capacity span. */
  /* @TE1: only two elments should be copy from one span to another. */
  /* @End: UT_CORE_0121 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0122
@Test_Description:    To test the functionality of last element of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613227, 613191, 613207, 613190
*******************************************************************************/
TEST(SpanTest, Last)
{
  int buffer[4] = {1, 2, 3, 4};
  Span<int, 4> sp1(buffer);
  Span<int> sp2(buffer);

  Span<int, 2> sp10 = sp1.last<2>();
  EXPECT_THAT(sp10, ElementsAre(3, 4));

  /* @Start: UT_CORE_0122 */
  /* @TS1: place the last two elements of span sp1 into the sp10. */
  /* @TE1: Only last 2 elments from sp1 should be copy into the sp10. */
  /* @End: UT_CORE_0122 */
  Span<int, 0> sp11 = sp1.last<0>();
  EXPECT_THAT(sp11, IsEmpty());
  /* @Start: UT_CORE_0122 */
  /* @TS2: place the last empty element of span sp1 into the sp11. */
  /* @TE2: sp11 should be empty. */
  /* @End: UT_CORE_0122 */
#ifndef NDEBUG
  EXPECT_DEATH(sp1.last<5>(), "Count <= size()");
#endif

  Span<int> sp12 = sp1.last(3);
  EXPECT_THAT(sp12, ElementsAre(2, 3, 4));
  /* @Start: UT_CORE_0122 */
  /* @TS3: place the last three elements of span sp1 into the sp12. */
  /* @TE3: Only last 3 elments from sp1 should be copy into the sp12. */
  /* @End: UT_CORE_0122 */
  Span<int> sp13 = sp1.last(0);
  EXPECT_THAT(sp13, IsEmpty());
  /* @Start: UT_CORE_0122 */
  /* @TS4: place the last empty element of span sp1 into the sp13. */
  /* @TE4: sp13 should be empty. */
  /* @End: UT_CORE_0122 */
  Span<int> sp14 = sp2.last(3);
  EXPECT_THAT(sp14, ElementsAre(2, 3, 4));
  /* @Start: UT_CORE_0122 */
  /* @TS5: place the last three elements of span sp2 into the sp14. */
  /* @TE5: Only last 3 elments from sp2 should be copy into the sp14. */
  /* @End: UT_CORE_0122 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0123
@Test_Description:    To test the functionality of subspan().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613209, 613213, 613214  
*******************************************************************************/
TEST(SpanTest, Subspan)
{
  int buffer[5] = {1, 2, 3, 4, 5};
  Span<int, 5> sp1(buffer);
  Span<int> sp2(buffer);

  Span<int> sp3 = sp1.subspan(1, 2);
  EXPECT_THAT(sp3, ElementsAre(2, 3));

  Span<int> sp4 = sp1.subspan(1);
  EXPECT_THAT(sp4, ElementsAre(2, 3, 4, 5));
  /* @Start: UT_CORE_0123 */
  /* @TS1: Get the value of subspan() of span and pass the range of index. */
  /* @TE1: subspan() should be copy the elements from the given range. */
  /* @End: UT_CORE_0123 */
#ifndef NDEBUG
  EXPECT_DEATH(sp1.subspan(6, 2), "offset <= size()");
  EXPECT_DEATH(sp1.subspan(4, 2), "count == dynamic_extent || (offset + count <= size())");
#endif

  Span<int, 2> sp10 = sp1.subspan<1, 2>();
  EXPECT_THAT(sp10, ElementsAre(2, 3));

  Span<int, 4> sp11 = sp1.subspan<1>();
  EXPECT_THAT(sp11, ElementsAre(2, 3, 4, 5));
  /* @Start: UT_CORE_0123 */
  /* @TS2: Get the value of subspan() of span and pass the range of index. */
  /* @TE2: subspan methos should be copy the elements from the given range .*/
  /* @End: UT_CORE_0123 */

#ifndef NDEBUG
  auto f = [&sp1] { sp1.subspan<1, 5>(); };
  EXPECT_DEATH(f(), "Count == dynamic_extent || (Offset + Count <= size())");
  EXPECT_DEATH(sp1.subspan<1000>(), "Offset <= size()");
#endif

  Span<int, 2> sp20 = sp2.subspan<1, 2>();
  EXPECT_THAT(sp20, ElementsAre(2, 3));

  Span<int, 4> sp21 = sp2.subspan<1, 4>();
  EXPECT_THAT(sp21, ElementsAre(2, 3, 4, 5));

  Span<int> sp22 = sp2.subspan<1>();
  EXPECT_THAT(sp22, ElementsAre(2, 3, 4, 5));
  /* @Start: UT_CORE_0123 */
  /* @TS3: Get the value of subspan() of span and pass the range of index. */
  /* @TE3: subspan() should be copy the elements from the given range. */
  /* @End: UT_CORE_0123 */
}

// Simple POD struct for testing as_bytes()
struct SomeData
{
  std::uint32_t x;
  std::uint32_t y;
  std::uint32_t z;
};

/*******************************************************************************
@TestCaseId:          UT_CORE_0124
@Test_Description:    To test the functionality of AsBytes of Span.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613215, 613227, 613191, 613207, 613190
*******************************************************************************/
TEST(SpanTest, AsBytes)
{
  // Testing:
  // template <typename ElementType, std::size_t Extent>
  // Span<const Byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>
  // as_bytes(Span<ElementType, Extent> s) noexcept;
  // with Extent == dynamic_extent

  SomeData const data1{htonl(0x0155aa10), htonl(0x0266bb20), htonl(0x0377cc30)};
  ara::core::Span<SomeData const> input1(&data1, 1);
  ara::core::Span<ara::core::Byte const> bytes1 = ara::core::as_bytes(input1);
  EXPECT_THAT(bytes1.size(), sizeof(data1));

  using BytesType1 = ara::core::Span<ara::core::Byte const, ara::core::dynamic_extent>;
  bool const bytesHasCorrectExtent1 = std::is_same<decltype(bytes1), BytesType1>::value;
  EXPECT_THAT(bytesHasCorrectExtent1, true);
  /* @Start: UT_CORE_0124 */
  /* @TS1: Create instance of std::is_same<decltype(bytes1), BytesType1>::value as bytesHasCorrectExtent1. */
  /* @TE1: bytesHasCorrectExtent1 should return true. */
  /* @End: UT_CORE_0124 */

  constexpr std::size_t extent1 = decltype(bytes1)::stkExtent;
  EXPECT_THAT(extent1, ara::core::dynamic_extent);

  std::uint8_t const ex1[] = {1, 0x55, 0xaa, 0x10, 2, 0x66, 0xbb, 0x20, 3, 0x77, 0xcc, 0x30};
  ara::core::Byte expected1[sizeof(ex1)];
  std::transform(&ex1[0], &ex1[sizeof(ex1)], &expected1[0],
      [](std::uint8_t v) { return static_cast<ara::core::Byte>(v); });
  EXPECT_THAT(bytes1, ElementsAreArray(expected1));

  // The same for size 0:
  ara::core::Span<SomeData const> input1a(nullptr, std::size_t(0));
  auto bytes1a = ara::core::as_bytes(input1a);
  EXPECT_THAT(bytes1a.size(), 0);
  using BytesType1a = ara::core::Span<ara::core::Byte const, ara::core::dynamic_extent>;
  bool const bytesHasCorrectExtent1a = std::is_same<decltype(bytes1a), BytesType1a>::value;
  EXPECT_THAT(bytesHasCorrectExtent1a, true);
  constexpr std::size_t extent1a = decltype(bytes1a)::stkExtent;
  EXPECT_THAT(extent1a, ara::core::dynamic_extent);

  // Testing:
  // template <typename ElementType, std::size_t Extent>
  // Span<const Byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>
  // as_bytes(Span<ElementType, Extent> s) noexcept;
  // with Extent != dynamic_extent

  SomeData const data2[] = {{htonl(1), htonl(2), htonl(3)}, {htonl(4), htonl(5), htonl(6)}};
  ara::core::Span<SomeData const, 2> input2(data2);
  auto bytes2 = ara::core::as_bytes(input2);
  EXPECT_THAT(bytes2.size(), sizeof(data2));

  using BytesType2 = ara::core::Span<ara::core::Byte const, sizeof(data2)>;
  bool const bytesHasCorrectExtent2 = std::is_same<decltype(bytes2), BytesType2>::value;
  EXPECT_THAT(bytesHasCorrectExtent2, true);

  std::size_t const extent2 = decltype(bytes2)::stkExtent;
  EXPECT_THAT(extent2, sizeof(data2));

  std::uint8_t const ex2[] = {0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 0, 0, 0, 6};
  ara::core::Byte expected2[sizeof(ex2)];
  std::transform(&ex2[0], &ex2[sizeof(ex2)], &expected2[0],
      [](std::uint8_t v) { return static_cast<ara::core::Byte>(v); });
  EXPECT_THAT(bytes2, ElementsAreArray(expected2));

  // The same for stkExtent 0:
  ara::core::Array<SomeData, 0> const data2a;
  ara::core::Span<SomeData const, 0> input2a(data2a);
  auto bytes2a = ara::core::as_bytes(input2a);
  EXPECT_THAT(bytes2a.size(), 0);
  using BytesType2a = ara::core::Span<ara::core::Byte const, 0>;
  bool const bytesHasCorrectExtent2a = std::is_same<decltype(bytes2a), BytesType2a>::value;
  EXPECT_THAT(bytesHasCorrectExtent2a, true);
  constexpr std::size_t extent2a = decltype(bytes2a)::stkExtent;
  EXPECT_THAT(extent2a, 0);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0125
@Test_Description:    To test the functionality of Bytes.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      613215
*******************************************************************************/
TEST(SpanTest, AsWritableBytes)
{
  // Testing:
  // template <typename ElementType, std::size_t Extent>
  // Span<Byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>
  // as_writable_bytes(Span<ElementType, Extent> s) noexcept;
  // with Extent == dynamic_extent

  SomeData data1{htonl(0x0155aa10), htonl(0x0266bb20), htonl(0x0377cc30)};
  ara::core::Span<SomeData> input1(&data1, 1);
  ara::core::Span<ara::core::Byte> bytes1 = ara::core::as_writable_bytes(input1);
  EXPECT_THAT(bytes1.size(), sizeof(data1));

  using BytesType1 = ara::core::Span<ara::core::Byte, ara::core::dynamic_extent>;
  bool const bytesHasCorrectExtent1 = std::is_same<decltype(bytes1), BytesType1>::value;
  EXPECT_THAT(bytesHasCorrectExtent1, true);
  /* @Start: UT_CORE_0125 */
  /* @TS1: Get the value of std::is_same<decltype(bytes1), BytesType1> as bytesHasCorrectExtent1. */
  /* @TE1: bytesHasCorrectExtent1 should return true. */
  /* @End: UT_CORE_0125 */
  constexpr std::size_t extent1 = decltype(bytes1)::stkExtent;
  EXPECT_THAT(extent1, ara::core::dynamic_extent);

  std::uint8_t const ex1[] = {1, 0x55, 0xaa, 0x10, 2, 0x66, 0xbb, 0x20, 3, 0x77, 0xcc, 0x30};
  ara::core::Byte expected1[sizeof(ex1)];
  std::transform(&ex1[0], &ex1[sizeof(ex1)], &expected1[0],
      [](std::uint8_t v) { return static_cast<ara::core::Byte>(v); });
  EXPECT_THAT(bytes1, ElementsAreArray(expected1));

  // Write-through
  // This is only safe to do when compiling with something like -fno-strict-aliasing, which we are.
  bytes1[0] = ara::core::Byte{0x22};
  bytes1[11] = ara::core::Byte{0x44};

  EXPECT_THAT(data1.x, htonl(0x2255aa10));
  EXPECT_THAT(data1.y, htonl(0x0266bb20));
  EXPECT_THAT(data1.z, htonl(0x0377cc44));

  std::uint8_t const ex1b[] = {0x22, 0x55, 0xaa, 0x10, 2, 0x66, 0xbb, 0x20, 3, 0x77, 0xcc, 0x44};
  ara::core::Byte expected1b[sizeof(ex1b)];
  std::transform(&ex1b[0], &ex1b[sizeof(ex1b)], &expected1b[0],
      [](std::uint8_t v) { return static_cast<ara::core::Byte>(v); });
  EXPECT_THAT(bytes1, ElementsAreArray(expected1b));

  // Testing:
  // template <typename ElementType, std::size_t Extent>
  // Span<Byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>
  // as_bytes(Span<ElementType, Extent> s) noexcept;
  // with Extent != dynamic_extent

  SomeData data2[] = {{htonl(1), htonl(2), htonl(3)}, {htonl(4), htonl(5), htonl(6)}};
  ara::core::Span<SomeData, 2> input2(data2);
  auto bytes2 = ara::core::as_writable_bytes(input2);
  EXPECT_THAT(bytes2.size(), sizeof(data2));

  using BytesType2 = ara::core::Span<ara::core::Byte, sizeof(data2)>;
  bool const bytesHasCorrectExtent2 = std::is_same<decltype(bytes2), BytesType2>::value;
  EXPECT_THAT(bytesHasCorrectExtent2, true);

  std::size_t const extent2 = decltype(bytes2)::stkExtent;
  EXPECT_THAT(extent2, sizeof(data2));

  std::uint8_t const ex2[] = {0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 0, 0, 0, 6};
  ara::core::Byte expected2[sizeof(ex2)];
  std::transform(&ex2[0], &ex2[sizeof(ex2)], &expected2[0],
      [](std::uint8_t v) { return static_cast<ara::core::Byte>(v); });
  EXPECT_THAT(bytes2, ElementsAreArray(expected2));

  // Write-through
  // This is only safe to do when compiling with something like -fno-strict-aliasing, which we are.
  bytes2[0] = ara::core::Byte{0x22};
  bytes2[11] = ara::core::Byte{0x44};

  EXPECT_THAT(data2[0].x, htonl(0x22000001));
  EXPECT_THAT(data2[0].y, htonl(0x00000002));
  EXPECT_THAT(data2[0].z, htonl(0x00000044));

  std::uint8_t const ex2b[]
      = {0x22, 0, 0, 0x01, 0, 0, 0, 2, 0, 0, 0, 0x44, 0, 0, 0, 4, 0, 0, 0, 5, 0, 0, 0, 6};
  ara::core::Byte expected2b[sizeof(ex2b)];
  std::transform(&ex2b[0], &ex2b[sizeof(ex2b)], &expected2b[0],
      [](std::uint8_t v) { return static_cast<ara::core::Byte>(v); });
  EXPECT_THAT(bytes2, ElementsAreArray(expected2b));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0126
@Test_Description:    To test the functionality of MakeSpan.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          SpanTest
@Requirement Tracing: -
@Design Tracing:      609894
*******************************************************************************/
TEST(SpanTest, MakeSpan)
{
  int buffer[5] = {1, 2, 3, 4, 5};
  ara::core::Array<int, 5> array{10, 11, 12, 13, 14};
  ara::core::Array<int, 5> const carray{10, 11, 12, 13, 14};

  Span<int> sp1 = ara::core::MakeSpan(&buffer[1], 3);
  EXPECT_THAT(sp1, ElementsAre(2, 3, 4));
  /* @Start: UT_CORE_0126 */
  /* @TS1: Get the value of MakeSpan() of span and pass the parameter as array and number of elements as 3. */
  /* @TE1: MakeSpan() should copy 3 elements of an array into the span. */
  /* @End: UT_CORE_0126 */
  Span<int> sp2 = ara::core::MakeSpan(&buffer[1], &buffer[4]);
  EXPECT_THAT(sp2, ElementsAre(2, 3, 4));
  /* @Start: UT_CORE_0126 */
  /* @TS2: Get the value of MakeSpan() of span and pass the parameter as index 1 address and index 4 address. */
  /* @TE2: MakeSpan() should copy the elments between the 1st and 4th index. */
  /* @End: UT_CORE_0126 */
  Span<int, 5> sp3 = ara::core::MakeSpan(buffer);
  EXPECT_THAT(sp3, ElementsAre(1, 2, 3, 4, 5));
  /* @Start: UT_CORE_0126 */
  /* @TS3: Get the value of MakeSpan() of span and pass the array to the span. */
  /* @TE3: MakeSpan() should be able to copy the array elments into the span. */
  /* @End: UT_CORE_0126 */
  Span<int> sp4 = ara::core::MakeSpan(array);
  EXPECT_THAT(sp4, ElementsAre(10, 11, 12, 13, 14));
  /* @Start: UT_CORE_0126 */
  /* @TS4: Get the value of MakeSpan() of span and pass the array to the span. */
  /* @TE4: MakeSpan() should be able to copy the array elments into the span. */
  /* @End: UT_CORE_0126 */
  Span<int const> sp5 = ara::core::MakeSpan(carray);
  EXPECT_THAT(sp5, ElementsAre(10, 11, 12, 13, 14));
  /* @Start: UT_CORE_0126 */
  /* @TS5: Get the value of MakeSpan() of span and pass the const array to the span. */
  /* @TE5: MakeSpan() should be able to copy the const array elments into the span. */
  /* @End: UT_CORE_0126 */
}
