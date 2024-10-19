// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_fixed_string.cpp
//
// Purpose     : This file provides the unit test cases of class string.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/fixed_string.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>
#include <cstring>

using ::testing::Eq;
using ::testing::IsEmpty;
using ::testing::Lt;
using ::testing::Ne;
using ::testing::StrEq;

namespace
{
using namespace ::testing;
using namespace ara::core;

template <typename T>
class stringTyped_test : public testing::Test
{
 protected:
  T testSubject;
  using stringType = T;
};

using Implementations = testing::Types<FixedString<1>, FixedString<15>, FixedString<100>, FixedString<1000>>;

TYPED_TEST_CASE(stringTyped_test, Implementations);

/*******************************************************************************
@TestCaseId:          UT_CORE_0251
@Test_Description:    To test the functionality of capacity() that returns specified capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          string_test
@Requirement Tracing: -
@Design Tracing:      611439
*******************************************************************************/
TEST(string_test, CapacityReturnsSpecifiedCapacity)
{
  ::testing::Test::RecordProperty("TEST_ID", "6eed7f1a-c8d9-4902-ac22-405a3bd62d28");
  constexpr uint16_t CAPACITY_ONE{1};
  constexpr uint16_t CAPACITY_FOURTYTWO{42};

  EXPECT_THAT(ara::core::FixedString<CAPACITY_ONE>::capacity(), Eq(CAPACITY_ONE));
  EXPECT_THAT(ara::core::FixedString<CAPACITY_FOURTYTWO>::capacity(), Eq(CAPACITY_FOURTYTWO));

  /* @Start: UT_CORE_0251 */
  /* @TS1: Invoke the capacity() of FixedString. */
  /* @TE1: capacity() should be equal to 1. */
  /* @TS2: Invoke the capacity() of FixedString. */
  /* @TE2: capacity() should be equal to 42. */
  /* @End: UT_CORE_0251 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0252
@Test_Description:    To test the functionality of Empty string that results in size 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611424
*******************************************************************************/
TYPED_TEST(stringTyped_test, EmptyInitializationResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "4a1a3850-b151-4f11-8208-b286e403847d");
  EXPECT_THAT(this->testSubject.size(), Eq(0U));

  /* @Start: UT_CORE_0252 */
  /* @TS1: Invoke the size() of empty string. */
  /* @TE1: Size() should be equal to 0. */
  /* @End: UT_CORE_0002 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0253
@Test_Description:    To test the functionality of String that results to empty String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611424
*******************************************************************************/
TYPED_TEST(stringTyped_test, EmptyInitializationResultsInEmptyString)
{
  ::testing::Test::RecordProperty("TEST_ID", "b4833ccc-814c-408e-a469-9950550703b9");
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0253 */
  /* @TS1: Invoke the c_str() of empty string. */
  /* @TE1: c_str() should be equal to null. */
  /* @End: UT_CORE_0253 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0254
@Test_Description:    To test the functionality of Empty string that results in size() of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, CopyConstructEmptyStringResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "8a88b069-4c7c-425a-b515-35deeaacaae5");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu(this->testSubject);
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0254 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and insert testSubject in it. */
  /* @TE2: Instance fuu should be created with same capacity of MyString. */
  /* @TS3: Invoke the capacity() of fuu. */
  /* @TE3: capacity() should return equal to STRINGCAP. */
  /* @TS4: Invoke the size() of fuu. */
  /* @TE4: size() should be equal to 0. */
  /* @TS5: Invoke the c_str() of fuu. */
  /* @TE5: c_str() should be null. */
  /* @End: UT_CORE_0254 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0255
@Test_Description:    To test the functionality of String capacity that results in size capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, CopyConstructStringOfSizeCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "0d6bc778-8a1e-4eb5-8f87-0ebf45c5116e");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP> fuu(this->testSubject);
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.c_str(), Eq(testString));

  /* @Start: UT_CORE_0255 */
  /* @TS1: Create an instance of FixedString as testString and insert capacity of MyString, pass parameter 'M'. */
  /* @TE1: Instance testString should be created with the same capacity of MyString and passed parameter 'M'. */
  /* @TS2: Create an instance of FixedString as fuu and invoke the capacity(). */
  /* @TE2: capacity() should return the capacity of MyString. */
  /* @TS3: Invoke the unsafe_assign() of testSubject. */
  /* @TE3: unsafe_assign() of testSubject should return true. */
  /* @TS4: Invoke the capacity() of FixedString instance fuu. */
  /* @TE4: capacity() of fuu should be equal to STRINGCAP. */
  /* @TS5: Invoke the size() of fuu. */
  /* @TE5: size() of fuu should be equal to STRINGCAP. */
  /* @TS6: Invoke the c_str() of fuu. */
  /* @TE6: c_str() of fuu should be equal to testString. */
  /* @End: UT_CORE_0255 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0256
@Test_Description:    To test the functionality Move Constructor with string size 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, MoveConstructionWithStringOfSize0Works)
{
  ::testing::Test::RecordProperty("TEST_ID", "1857ab80-bb27-4e4d-8f11-6ddf94b0cfaf");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testString(std::move(this->testSubject));
  EXPECT_THAT(testString.size(), Eq(0));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(testString.c_str(), StrEq(""));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0256 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString with the capacity of MyString and move testSubject to it. */
  /* @TE2: Instance testString should be created with same capacity of MyString. */
  /* @TS3: Invoke the size() of testString. */
  /* @TE3: Size() of testString should be equal to 0. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: Size() of testSubject should be equal to 0 */
  /* @TS5: Invoke the c_str() of testString. */
  /* @TE5: c_str() of testString. should be equal to null. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() of testSubject should be equal to null. */
  /* @End: UT_CORE_0256 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0257
@Test_Description:    Test the operations on string type.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, MoveConstructionWithStringOfSizeSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "cb79ff56-d10f-4bb3-85a5-08c1ccc6947a");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP - 1U, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP> fuu(std::move(this->testSubject));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP - 1U));
  EXPECT_THAT(fuu.c_str(), Eq(testString));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0257 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString insert the capacity by subsistuting by 1 of FixedString and pass parameter 'M'. */
  /* @TE2: Instance testString should be created with decreasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with the capacity of MyString and move testSubject in it. */
  /* @TE3: Instance fuu should be created with same capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testsubject. */
  /* @TE4: unsafe_assign() of testsubject should return true. */
  /* @TS5: Invoke the size() of testsubject. */
  /* @TE5: Size() of testsubject should be equal to 0. */
  /* @TS6: Invoke the size() of fuu. */
  /* @TE6: Size() of fuu should be equal to STRINGCAP by decreasing 1. */
  /* @TS7: Invoke the c_str() of fuu. */
  /* @TE7: c_str() of fuu should be equal to testString. */
  /* @TS8: Invoke the c_str() of testSubject. */
  /* @TE8: c_str() of testsubject should be equal to null. */
  /* @End: UT_CORE_0257 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0258
@Test_Description:    To test the functionality of String size capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, MoveConstructionWithStringOfSizeCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "f313dd8a-7dc4-40e3-8203-e099e2a05305");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP> fuu(std::move(this->testSubject));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.c_str(), Eq(testString));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0258 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString should be created by decreasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with the capacity of MyString and move testSubject in it. */
  /* @TE3: Instance fuu should be created with same capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() of testSubject should return true. */
  /* @TS5: Invoke the size() of testsubject. */
  /* @TE5: Size() of testsubject should be equal to 0. */
  /* @TS6: Invoke the size() of fuu. */
  /* @TE6: Size() should be equal to STRINGCAP */
  /* @TS7: Invoke the c_str() of fuu. */
  /* @TE7: c_str() should be equal to testString. */
  /* @TS8: Invoke the c_str() of testSubject. */
  /* @TE8: c_str() of testsubject should be equal to null. */
  /* @End: UT_CORE_0258 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0259
@Test_Description:    To test the functionality of Copy assignment of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, SelfCopyAssignmentExcluded)
{
  ::testing::Test::RecordProperty("TEST_ID", "51b70520-fc11-4948-8fdf-643c638a81b9");
  this->testSubject = "M";
  this->testSubject = this->testSubject;
  EXPECT_THAT(this->testSubject.size(), Eq(1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq("M"));

  /* @Start: UT_CORE_0259 */
  /* @TS1: Create an instance as testSubject and pass parameter "M". */
  /* @TE1: Instance testSubject should be created by decreasing 1 to the capacity of MyString. */
  /* @TS2: Assign testSubject to testSubject. */
  /* @TE2: testSubject should be assigned to the testSubject. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: Size() of testSubject should be equal to 1. */
  /* @TS4: Invoke the c_str() of testSubject. */
  /* @TE4: c_str() of testSubject should be equal to M */
  /* @End: UT_CORE_0259 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0260
@Test_Description:    To test the functionality of Copy assignment with string size 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, CopyAssignmentWithStringOfSize0Works)
{
  ::testing::Test::RecordProperty("TEST_ID", "fdfca1c5-8293-4e14-beef-519729e71420");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu;
  fuu = this->testSubject;
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0260 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and assign testSubject to it. */
  /* @TE2: Instance fuu should be created with same capacity of MyString. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: Size() of testSubject should be equal to 0. */
  /* @TS4: Invoke the c_str() of testSubject. */
  /* @TE4: c_str() of testSubject should be equal to null. */
  /* @TS5: Invoke the size() of fuu. */
  /* @TE5: Size() of fuu should be equal to 0. */
  /* @TS6: Invoke the c_str() of fuu. */
  /* @TE6: c_str() of fuu should be equal to null. */
  /* @End: UT_CORE_0260 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0261
@Test_Description:    To test the functionality of Copy Assignment of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, CopyAssignmentWithStringOfSizeSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "b9b030db-e760-42ab-adbd-311cb5293a2e");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP - 1U, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP> fuu;
  fuu = this->testSubject;
  EXPECT_THAT(this->testSubject.size(), Eq(STRINGCAP - 1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(testString.substr(0U, STRINGCAP - 1U)));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP - 1U));
  EXPECT_THAT(fuu.c_str(), StrEq(testString.substr(0U, STRINGCAP - 1U)));

  /* @Start: UT_CORE_0261 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString by substistuting 1 to the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString should be created by decreasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with the capacity of MyString and assign testSubject to it. */
  /* @TE3: Instance fuu should be created with same capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() should return true. */
  /* @TS5: Invoke the size() of testSubject. */
  /* @TE5: Size() of testSubject should be equal to testString.substr(0, STRINGCAP - 1). */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() of testSubject should be equal to null. */
  /* @TS7: Invoke the size() of fuu. */
  /* @TE7: Size() of fuu should be equal to STRINGCAP by decreasing 1. */
  /* @TS8: Invoke the c_str() of fuu. */
  /* @TE8: c_str() of fuu should be equal to testSubject. */
  /* @End: UT_CORE_0261 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0262
@Test_Description:    To test the functionality of Copy Assignment of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, CopyAssignmentWithStringOfSizeCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "10fa4f3d-089c-4d10-83e9-5a37f7d5f0ec");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP> fuu;
  fuu = this->testSubject;
  EXPECT_THAT(this->testSubject.size(), Eq(STRINGCAP));
  EXPECT_THAT(this->testSubject.c_str(), Eq(testString));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.c_str(), Eq(testString));

  /* @Start: UT_CORE_0262 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString and insert capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with the capacity of MyString and assign testSubject to it. */
  /* @TE3: Instance fuu should be created with same capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: Unsafe_assign() of testSubject should return true. */
  /* @TS5: Invoke the size() of testsubject. */
  /* @TE5: Size() of testsubject should be equal to STRINGCAP. */
  /* @TS6: Invoke the c_str() of testsubject. */
  /* @TE6: c_str() of testsubject should be equal to testString. */
  /* @TS7: Invoke the size() of fuu. */
  /* @TE7: Size() of fuu should be equal to STRINGCAP. */
  /* @TS8: Invoke the c_str() of fuu. */
  /* @TE8: c_str() of fuu should be equal to testString. */
  /* @End: UT_CORE_0262 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0263
@Test_Description:    To test the functionality of Move Assignment of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, SelfMoveAssignmentExcluded)
{
  ::testing::Test::RecordProperty("TEST_ID", "0ad45975-b68b-465a-b8c5-83dd8d8290d5");
  this->testSubject = "M";
  this->testSubject = std::move(this->testSubject);
  EXPECT_THAT(this->testSubject.size(), Eq(1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq("M"));

  /* @Start: UT_CORE_0263 */
  /* @TS1: Create an instance as testSubject and assign parameter "M". */
  /* @TE1: Instance testSubject should be created with parameter "M". */
  /* @TS2: Move testSubject to testSubject. */
  /* @TE2: testSubject should be moved to testSubject. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: Size() of testSubject should be equal to 1. */
  /* @TS4: Invoke the c_str() of testSubject. */
  /* @TE4: c_str() of testSubject should be equal to M. */
  /* @End: UT_CORE_0263 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0264
@Test_Description:    To test the functionality of Move Assignment of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, MoveAssignmentOfStringWithSize0ResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "7170b9d6-ff73-4e3f-a6d1-b66f6de21dcd");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu;
  fuu = std::move(this->testSubject);
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0264 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and move testSubject to it. */
  /* @TE2: Instance fuu should be created with same capacity of MyString. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: Size() of testSubject should be equal to 0. */
  /* @TS4: Invoke the size() of fuu. */
  /* @TE4: Size() of fuu should be equal to 0. */
  /* @TS5: Invoke the c_str() of testSubject. */
  /* @TE5: c_str() of testSubject should be equal to null. */
  /* @TS6: Invoke the c_str() of fuu. */
  /* @TE6: c_str() of fuu should be equal to null. */
  /* @End: UT_CORE_0264 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0265
@Test_Description:    To test the functionality of Move assignment of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, MoveAssignmentOfStringWithSmallerSizeResultsInSmallerSize)
{
  ::testing::Test::RecordProperty("TEST_ID", "ca455516-5ca6-4882-933b-082769eb975a");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP - 1U, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP> fuu;
  fuu = std::move(this->testSubject);
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP - 1U));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), Eq(testString));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0265 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString by substistuting 1 to the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString should be created by decreasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with the capacity of MyString and move testSubject to it. */
  /* @TE3: Instance fuu should be created with same capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() should return true */
  /* @TS5: Invoke the size() of fuu. */
  /* @TE5: Size() of fuu should be equal to STRINGCAP by decreasing 1. */
  /* @TS6: Invoke the size() of testSubject. */
  /* @TE6: Size() of testSubject be should equal to 0. */
  /* @TS7: Invoke the c_str() of fuu. */
  /* @TE7: c_str() of fuu should be equal to testString. */
  /* @TS8: Invoke the c_str() of testSubject. */
  /* @TE8: c_str() of testSubject should be equal to null. */
  /* @End: UT_CORE_0265 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0266
@Test_Description:    To test the functionality of String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, MoveAssignmentOfStringWithSizeCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "375944b8-6817-44d4-8a05-de9e0235c503");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP> fuu;
  fuu = std::move(this->testSubject);
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), Eq(testString));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0266 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString by substistuting 1 to the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString should be created by decreasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with the capacity of MyString and move testSubject to it. */
  /* @TE3: Instance fuu should be created with same capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() should return true. */
  /* @TS5: Invoke the size() of fuu. */
  /* @TE5: Size() of fuu should be equal to STRINGCAP. */
  /* @TS6: Invoke the size() of testSubject. */
  /* @TE6: Size() of testSubject should be equal to 0. */
  /* @TS7: Invoke the c_str() of fuu. */
  /* @TE7: c_str() of fuu should be equal to testString. */
  /* @TS8: Invoke the c_str() of testSubject. */
  /* @TE8: c_str() of testSubject should be equal to null. */
  /* @End: UT_CORE_0266 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0267
@Test_Description:    To test the functionality Char to String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, CharToStringConvConstrWithSize0ResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "29e9f7b8-a1c7-4462-81d0-52ef34a8b883");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("");
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0267 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass empty parameter. */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Invoke the capacity() of fuu. */
  /* @TE3: capacity() of fuu should be equal to STRINGCAP. */
  /* @TS4: Invoke the size() of fuu. */
  /* @TE4: Size() of fuu should be equal to 0. */
  /* @TS5: Invoke the c_str() of fuu. */
  /* @TE5: c_str() of fuu should be equal to null. */
  /* @End: UT_CORE_0267 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0268
@Test_Description:    To test the functionality of Char to String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, CharToStringConvConstrWithSizeCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "de81475a-527e-43e0-97b8-faf7a9300204");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  char testChar[STRINGCAP];
  for (uint64_t i = 0U; i < STRINGCAP - 1U; i++)
  {
    testChar[i] = 'M';
  }
  testChar[STRINGCAP - 1U] = '\0';
  FixedString<STRINGCAP> testSubject(testChar);
  EXPECT_THAT(testSubject.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(testSubject.size(), Eq(STRINGCAP - 1U));
  EXPECT_THAT(testSubject.c_str(), StrEq(testChar));

  /* @Start: UT_CORE_0268 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testSubject and insert testChar in it. */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Invoke the capacity() of testSubject. */
  /* @TE3: capacity() of testSubject should be equal to STRINGCAP. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testSubject should be equal to STRINGCAP by decreasing 1. */
  /* @TS5: Invoke the c_str() of testSubject. */
  /* @TE5: c_str() of testSubject should be equal to testChar. */
  /* @End: UT_CORE_0268 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0269
@Test_Description:    To test the functionality of Unsafe Char to string of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeCharToStringConvConstrWithSize0ResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "3ed6360e-c4a0-474b-8ed7-e6b4e129a6c0");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu(ara::core::TruncateToCapacity, "");
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0269 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with capacity of MyString and insert TruncateToCapacity, pass empty parameter.. */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Invoke the capacity() of fuu. */
  /* @TE3: capacity() should be equal to STRINGCAP. */
  /* @TS4: Invoke the size() of fuu. */
  /* @TE4: size() should be equal to 0. */
  /* @TS5: Invoke the c_str() of fuu. */
  /* @TE5: c_str() should be equal to null. */
  /* @End: UT_CORE_0269 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0270
@Test_Description:    To test the functionality of Unsafe Char to string of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeCharToStringConvConstrWithSizeCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "5c417b37-ee9d-42f9-bb25-c59c6929d4ca");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  char testChar[STRINGCAP + 1];
  for (uint64_t i = 0U; i < STRINGCAP - 1U; i++)
  {
    testChar[i] = 'M';
  }
  testChar[STRINGCAP - 1U] = '\0';
  FixedString<STRINGCAP> testSubject(ara::core::TruncateToCapacity, testChar);
  EXPECT_THAT(testSubject.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(testSubject.size(), Eq(STRINGCAP - 1U));
  EXPECT_THAT(testSubject.c_str(), StrEq(testChar));

  /* @Start: UT_CORE_0270 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testSubject with capacity of MyString and insert TruncateToCapacity, testChar in it. */
  /* @TE2: Instance testSubject should be created with the same capacity of MyString. */
  /* @TS3: Invoke the capacity() of testSubject. */
  /* @TE3: capacity() of testSubject should be equal to STRINGCAP. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: Size() of testSubject should be equal to STRINGCAP by decreasing 1. */
  /* @TS5: Invoke the c_str() of testSubject. */
  /* @TE5: c_str() of testSubject should be equal to testChar. */
  /* @End: UT_CORE_0270 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0271
@Test_Description:    To test the functionality of Unsafe Char to String with greater string capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeCharToStringConvConstrWithSizeGreaterCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "5e0a2022-ea15-43d5-aae8-980a75be6122");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  char testChar[STRINGCAP + 1U];
  for (uint64_t i = 0U; i < STRINGCAP; i++)
  {
    testChar[i] = 'M';
  }
  testChar[STRINGCAP] = '\0';
  FixedString<STRINGCAP> testSubject(ara::core::TruncateToCapacity, testChar);
  EXPECT_THAT(testSubject.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(testSubject.size(), Eq(STRINGCAP));
  /* @Start: UT_CORE_0271 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testSubject with capacity of MyString and insert TruncateToCapacity, testChar in it. */
  /* @TE2: Instance testSubject should be created with the same capacity of MyString. */
  /* @TS3: Invoke the capacity of testSubject. */
  /* @TE3: capacity() of testSubject should return equal to STRINGCAP. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testSubject should return equal to STRINGCAP. */
  /* @End: UT_CORE_0271 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0272
@Test_Description:    To test the functionality of UnsafeChar to String results to Empty string of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeCharToStringConvConstrWithNullPtrResultsEmptyString)
{
  ::testing::Test::RecordProperty("TEST_ID", "c6bbcbc6-e049-4c2c-bf84-8d89dcf42ce8");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu(ara::core::TruncateToCapacity, nullptr);
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0272 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with capacity of MyString and insert TruncateToCapacity, nullptr in it. */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Invoke the capacity() of fuu. */
  /* @TE3: capacity() of fuu should be equal to STRINGCAP. */
  /* @TS4: Invoke the size() of fuu. */
  /* @TE4: size() of fuu should be equal to 0. */
  /* @TS5: Invoke the c_str() of fuu. */
  /* @TE5: c_str() of fuu should be equal to null. */
  /* @End: UT_CORE_0272 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0273
@Test_Description:    To test the functionality of Unsafe STD String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeSTDStringToStringConvConstrWithSize0ResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "83e1b7b2-8487-4c71-ac86-f4d5d98c1918");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString;
  FixedString<STRINGCAP> fuu(ara::core::TruncateToCapacity, testString);
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0273 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString. */
  /* @TE2: Instance testString should be created. */
  /* @TS3: Create an instance of FixedString as fuu with capacity of MyString and insert TruncateToCapacity, testString in it. */
  /* @TE3: Instance fuu should be created with the same capacity of MyString. */
  /* @TS4: Invoke the capacity() of fuu . */
  /* @TE4: capacity() of fuu should be equal to STRINGCAP. */
  /* @TS5: Invoke the size() of fuu. */
  /* @TE5: size() of fuu should be equal to 0. */
  /* @TS6: Invoke the c_str() of fuu. */
  /* @TE6: c_str() of fuu should be equal to null. */
  /* @End: UT_CORE_0273 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0274
@Test_Description:    To test the functionality of Unsafe STD string that result in smaller size capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeSTDStringToStringConvConstrWithSizeSmallerCapaResultsInSizeSmallerCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "1bd6cd60-0487-4ba2-9e51-3a9297078454");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP - 1U, 'M');
  FixedString<STRINGCAP> fuu(ara::core::TruncateToCapacity, testString);
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP - 1U));
  EXPECT_THAT(fuu.c_str(), Eq(testString));

  /* @Start: UT_CORE_0274 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString and by substituting 1 to the capacity of MyString, pass parameter 'M'. */
  /* @TE2: Instance testString should be created by decreasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with capacity of MyString and insert TruncateToCapacity, testString in it. */
  /* @TE3: Instance fuu should be created with same capacity of MyString. */
  /* @TS4: Invoke the capacity() of fuu. */
  /* @TE4: capacity() of fuu should be equal to STRINGCAP. */
  /* @TS5: Invoke the size() of fuu. */
  /* @TE5: size() of fuu should be equal to STRINGCAP by decreasing 1. */
  /* @TS6: Invoke the c_str() of fuu. */
  /* @TE6: c_str() of fuu should be equal to testString. */
  /* @End: UT_CORE_0274 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0275
@Test_Description:    To test the functionality of Unsafe STD String to String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeSTDStringToStringConvConstrWithSizeCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "afa37f19-fde0-40ab-b1bd-10862f623ae7");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  FixedString<STRINGCAP> fuu(ara::core::TruncateToCapacity, testString);
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.c_str(), Eq(testString));

  /* @Start: UT_CORE_0275 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString and by insert the capacity of MyString, pass parameter 'M'. */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with capacity of MyString and insert TruncateToCapacity, testString in it. */
  /* @TE3: Instance fuu should be created with same capacity of MyString. */
  /* @TS4: Invoke the capacity() of fuu. */
  /* @TE4: capacity() of fuu should be equal to STRINGCAP. */
  /* @TS5: Invoke the size() of fuu. */
  /* @TE5: size() of fuu should be equal to STRINGCAP. */
  /* @TS6: Invoke the c_str() of fuu. */
  /* @TE6: c_str() of fuu should be equal to testString. */
  /* @End: UT_CORE_0275 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0276
@Test_Description:    To test the functionality of String capacity() of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeSTDStringToStringConvConstrWithSizeGreaterCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "67cba3f0-30ed-415d-8232-8e8b5898fe04");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP + 1U, 'M');
  FixedString<STRINGCAP> fuu(ara::core::TruncateToCapacity, testString);
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.c_str(), Eq(testString.substr(0U, STRINGCAP)));
  /* @Start: UT_CORE_0276 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString and by adding 1 to the capacity of MyString, pass parameter 'M'. */
  /* @TE2: Instance testString should created by increasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with capacity of MyString and insert TruncateToCapacity, testString in it. */
  /* @TE3: Instance fuu should be created with same capacity of MyString. */
  /* @TS4: Invoke the capacity() of fuu. */
  /* @TE4: capacity() of fuu should be equal to STRINGCAP. */
  /* @TS5: Invoke the size() of fuu. */
  /* @TE5: size() of fuu should be equal to STRINGCAP. */
  /* @TS6: Invoke the c_str() of fuu. */
  /* @TE6: c_str() of fuu should be equal to testString. */
  /* @End: UT_CORE_0276 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0277
@Test_Description:    To test the functionality of UnsafeChar to string that result in Size 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeCharToStringConstrWithCount0ResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "6cbd9ee8-f015-41ce-8a2c-b8c023b0722a");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu(ara::core::TruncateToCapacity, "Yoda", 0U);
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0277 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with capacity of MyString and insert TruncateToCapacity, parameter "Yoda", 0 capacity in it. */
  /* @TE2: Instance testString should be created by decreasing 1 to the capacity of MyString. */
  /* @TS3: Invoke the capacity() of fuu. */
  /* @TE3: capacity() of fuu should be equal to STRINGCAP. */
  /* @TS4: Invoke the size() of fuu. */
  /* @TE4: size() of fuu should be equal to 0. */
  /* @TS5: Invoke the c_str() of fuu. */
  /* @TE5: c_str() of fuu should be equal to null. */
  /* @End: UT_CORE_0277 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0278
@Test_Description:    To test the functionality of UnsafeChar to String with equl capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeCharToStringConstrWithCountEqCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "0e62c9fe-78e7-4b6d-8c31-27270dca7581");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  FixedString<STRINGCAP> testSubject(ara::core::TruncateToCapacity, testString.c_str(), STRINGCAP);
  EXPECT_THAT(testSubject.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(testSubject.size(), Eq(STRINGCAP));
  EXPECT_THAT(testSubject.c_str(), StrEq(testString));

  /* @Start: UT_CORE_0278 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString and insert the capacity of MyString, pass parameter 'M'. */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as testSubject with capacity of MyString and insert TruncateToCapacity, testString.c_str(), capacity of MyString in it. */
  /* @TE3: Instance testSubject should be created with same capacity of MyString. */
  /* @TS4: Invoke the capacity of testSubject. */
  /* @TE4: capacity() of testSubject should be equal to STRINGCAP. */
  /* @TS5: Invoke the size() of testSubject. */
  /* @TE5: size() of testSubject should be equal to STRINGCAP. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str of testSubject should be equal to testString. */
  /* @End: UT_CORE_0278 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0279
@Test_Description:    To test the functionality of UnsafeChar to string with greater capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeCharToStringConstrWithCountGreaterCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "d2d321ed-8ee3-4337-a7c9-4785b5ae9a67");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP + 1U, 'M');
  FixedString<STRINGCAP> testSubject(ara::core::TruncateToCapacity, testString.c_str(), STRINGCAP + 1U);
  EXPECT_THAT(testSubject.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(testSubject.size(), Eq(STRINGCAP));
  EXPECT_THAT(testSubject.c_str(), StrEq(testString.substr(0U, STRINGCAP)));

  /* @Start: UT_CORE_0279 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString by adding 1 to the capacity of MyString, pass parameter 'M'. */
  /* @TE2: Instance testString should be created by decreasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as testSubject with capacity of MyString and insert TruncateToCapacity, testString.c_str, adding 1 to the capacity of MyString in it. */
  /* @TE3: Instance testSubject should be created with same capacity of MyString. */
  /* @TS4: Invoke the capacity of testSubject. */
  /* @TE4: capacity() of testSubject should be equal to STRINGCAP. */
  /* @TS5: Invoke the size() of testSubject. */
  /* @TE5: size() of testSubject should be equal to STRINGCAP. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() of testSubject should be equal to testString.substr(0U, STRINGCAP). */
  /* @End: UT_CORE_0279 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0280
@Test_Description:    To test the functionality of NullChar of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          string_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TEST(string_test, UnsafeCharToStringConstrIncludingNullCharWithCountResultsInSizeCount)
{
  ::testing::Test::RecordProperty("TEST_ID", "548eb26e-39b0-4c35-ad80-7665cde80361");
  std::string testString{"ice\0ryx", 7U};
  FixedString<100U> testSubject(ara::core::TruncateToCapacity, testString.c_str(), 7U);
  EXPECT_THAT(testSubject.capacity(), Eq(100U));
  EXPECT_THAT(testSubject.size(), Eq(7U));
  EXPECT_THAT(testSubject.c_str(), StrEq("ice\0ryx"));

  /* @Start: UT_CORE_0280 */
  /* @TS1: Create an instance of string as testString and pass parameter "ice\0ryx", capacity 7. */
  /* @TE1: Instance testString should be created. */
  /* @TS2: Create an instance of FixedString as testSubject with capacity 100 and insert TruncateToCapacity, testString.c_str, capacity 7 in it. */
  /* @TE2: Instance testSubject should be created with capacity 100. */
  /* @TS3: Invoke the capacity() of testSubject. */
  /* @TE3: capacity() of testSubject should be equal to 100. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testSubject should be equal to 7. */
  /* @TS5: Invoke the c_str() of testSubject. */
  /* @TE5: c_str() of testSubject should be equal to "ice\0ryx". */
  /* @End: UT_CORE_0280 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0281
@Test_Description:    To test the functionality of Assign char array with string size of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CharArrayAssignment
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TEST(CharArrayAssignment, AssignCharArrayWithStringSizeLessThanArraySize)
{
  ::testing::Test::RecordProperty("TEST_ID", "886f580d-e57c-4685-90bf-2399737779be");
  char testString[20] = "iceoryx";
  FixedString<20U> testSubject(testString);
  EXPECT_THAT(testSubject.size(), Eq(7U));
  EXPECT_THAT(testSubject.c_str(), StrEq("iceoryx"));

  /* @Start: UT_CORE_0281 */
  /* @TS1: Assign parameter "iceoryx" to the testString with char[20]. */
  /* @TE1: Instance testString should be created with char[20]. */
  /* @TS2: Create an instance of FixedString as testSubject with capacity 20 and insert teststring in it. */
  /* @TE2: Instance testSubject should be created with capacity 20. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: size() of testSubject should be equal to 7. */
  /* @TS4: Invoke the c_str() of testSubject. */
  /* @TE4: c_str() of testSubject should be equal to "iceoryx". */
  /* @End: UT_CORE_0281 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0282
@Test_Description:    To test the functionality of Assign zero for Char Array size of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CharArrayAssignment
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TEST(CharArrayAssignment, AssignZeroTerminatedCharArrayWithSizeForFullCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "884e724a-f5d3-41d1-89fa-96f55ce99a96");
  char testString[8] = "iceoryx";
  FixedString<7U> testSubject(testString);
  EXPECT_THAT(testSubject.size(), Eq(7U));
  EXPECT_THAT(testSubject.c_str(), StrEq("iceoryx"));
  /* @Start: UT_CORE_0282 */
  /* @TS1: Assign parameter "iceoryx" to the testString with char[8]. */
  /* @TE1: Instance testString should be created with char[8]. */
  /* @TS2: Create an instance of FixedString as testSubject with capacity 7 and insert teststring in it. */
  /* @TE2: Instance testSubject should be created with capacity 7. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: size() of testSubject should be equal to 7. */
  /* @TS4: Invoke the c_str() of testSubject. */
  /* @TE4: c_str() of testSubject should be equal to "iceoryx". */
  /* @End: UT_CORE_0282 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0283
@Test_Description:    To test the functionality of Assign Non-zero for Char Array size of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CharArrayAssignment
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TEST(CharArrayAssignment, AssignNonZeroTerminatedCharArrayOfSizeForFullCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "2a43553f-4358-4c41-a885-1495de0d7f4f");
  char testString[8] = "iceoryx";
  testString[7] = 'x'; // overwrite the 0 termination
  FixedString<7U> testSubject(testString);
  EXPECT_THAT(testSubject.size(), Eq(7U));
  EXPECT_THAT(testSubject.c_str(), StrEq("iceoryx"));
  /* @Start: UT_CORE_0283 */
  /* @TS1: Assign the "iceoryx" to the testString with char[8]. */
  /* @TE1: Instance testString should be created with char[8]. */
  /* @TS2: Create an instance of FixedString as testSubject with capacity 7 and insert teststring in it. */
  /* @TE2: Instance testSubject should be created with capacity 7. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: size() of testSubject should be equal to 7. */
  /* @TS4: Invoke the c_str() of testSubject. */
  /* @TE4: c_str() of testSubject should be equal to "iceoryx". */
  /* @End: UT_CORE_0283 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0284
@Test_Description:    To test the functionality of NullPtr that result to Empty String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeCharToStringConstrWithNullPtrResultsEmptyString)
{
  ::testing::Test::RecordProperty("TEST_ID", "cd772d2a-b6db-4b9d-b90d-f2a0aca4aaf6");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu(ara::core::TruncateToCapacity, nullptr, STRINGCAP);
  EXPECT_THAT(fuu.capacity(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), StrEq(""));
  /* @Start: UT_CORE_0284 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu. */
  /* @TE2: Instance fuu should created with the same capacity of MyString. */
  /* @TS3: Invoke the capacity() of fuu. */
  /* @TE3: capacity() of fuu should be equal to STRINGCAP. */
  /* @TS4: Invoke the size() of fuu. */
  /* @TE4: size() of fuu should be equal to 0. */
  /* @TS5: Invoke the c_str() of fuu. */
  /* @TE5: c_str() of fuu should be equal to null. */
  /* @End: UT_CORE_0284 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0285
@Test_Description:    To test the functionality of AssignCString of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignCStringOfSize0WithOperatorResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "49d08010-7a31-472d-bd99-6c6e7c49aad5");
  this->testSubject = "";
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));
  /* @Start: UT_CORE_0285 */
  /* @TS1: Assign empty string to testSubject. */
  /* @TE1: Empty string should be assigned to testSubject. */
  /* @TS2: Invoke the size() of testSubject. */
  /* @TE2: size() of testSubject should be equal to 0. */
  /* @TS3: Invoke the c_str() of testSubject. */
  /* @TE3: c_str() of testSubject should be equal to null. */
  /* @End: UT_CORE_0285 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0286
@Test_Description:    To test the functionality of AssignCString of size capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignCStringOfSizeCapaWithOperatorResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "19b0a4af-acfa-4d9b-b432-145ab1e7f59d");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  char testChar[STRINGCAP];
  for (uint64_t i = 0U; i < STRINGCAP - 1U; i++)
  {
    testChar[i] = 'M';
  }
  testChar[STRINGCAP - 1U] = '\0';
  FixedString<STRINGCAP> testSubject;
  testSubject = testChar;
  EXPECT_THAT(testSubject.size(), Eq(STRINGCAP - 1U));
  EXPECT_THAT(testSubject.c_str(), StrEq(testChar));
  /* @Start: UT_CORE_0286 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create char array as testChar with same as MyString capacity . */
  /* @TE2: testChar should be created with the same capacity of MyString. */
  /* @TS3: Assign parameter "M" to the testChar[i]. */
  /* @TE3: Parameter "M" should be assigned to testChar[i]. */
  /* @TS4: Assign parameter '\0' to the testChar by substituting 1 to the capacity of MyString. */
  /* @TE4: Parameter '\0' should be assigned to testChar by decreasing 1 to the capacity of MyString. */
  /* @TS5: Create an instance of FixedString as testSubject with capacity of MyString. */
  /* @TE5: Instance testSubject should be created with the same capacity of MyString. */
  /* @TS6: Assign testChar to testSubject. */
  /* @TE6: testchar should be assgined to testSubject. */
  /* @TS7: Invoke the size() of testSubject. */
  /* @TE7: size() of testSubject should be equal to STRINGCAP by decreasing 1. */
  /* @TS8: Invoke the c_str() of testSubject. */
  /* @TE8: c_str() of testSubject should be equal to testChar. */
  /* @End: UT_CORE_0286 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0287
@Test_Description:    To test the functionality of Assignment of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, SelfAssignmentIsExcluded)
{
  ::testing::Test::RecordProperty("TEST_ID", "32c07349-3924-4b86-9ce8-21247c8148b9");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testSubject;
  testSubject.assign(testSubject);
  EXPECT_THAT(testSubject.size(), Eq(0U));

  /* @Start: UT_CORE_0287 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testSubject with capacity of MyString. */
  /* @TE2: Instance testSubject should be created with the same capacity of MyString. */
  /* @TS3: Assign testSubject to testSubject. */
  /* @TE3: testSubject should be assigned to the testSubject. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testSubject should be equal to 0. */
  /* @End: UT_CORE_0287 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0288
@Test_Description:    To test the functionality of Assign String of size 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignStringOfSize0ResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "ee981dcb-80ab-439e-a3bf-a52a591b7dbb");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu;
  this->testSubject.assign(fuu);
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0288 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with capacity of MyString. */
  /* @TE2: Instance fuu should be created with capacity of MyString. */
  /* @TS3: Assign fuu to testSubject. */
  /* @TE3: fuu should be assigned to testSubject. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testSubject should return equal to 0. */
  /* @TS5: Invoke the c_str() of testSubject. */
  /* @TE5: c_str() of testSubject should return equal to null. */
  /* @TS6: Invoke the size() of fuu. */
  /* @TE6: size() of fuu should return be equal to 0. */
  /* @TS7: Invoke the c_str() of fuu. */
  /* @TE7: c_str() of fuu should return be equal to null. */
  /* @End: UT_CORE_0288 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0289
@Test_Description:    To test the functionality of Assign String of Size capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignStringOfSizeCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "75803631-93e8-4555-84d1-cdb44d88adc4");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  FixedString<STRINGCAP> fuu(ara::core::TruncateToCapacity, testString);
  this->testSubject.assign(fuu);
  EXPECT_THAT(this->testSubject.size(), Eq(STRINGCAP));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(testString));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.c_str(), StrEq(testString));

  /* @Start: UT_CORE_0289 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString insert capcity of MyString and pass parameter 'M' in it. */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with capacity of MyString and insert TruncateToCapacity, testString in it. */
  /* @TE3: Instance testSubject should be created with same capacity of MyString. */
  /* @TS4: Assign fuu to testSubject. */
  /* @TE4: fuu should be assigned to testSubject. */
  /* @TS5: Invoke the size() of testSubject. */
  /* @TE5: size() should be equal to STRINGCAP. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() should be equal to testString. */
  /* @TS7: Invoke the size() of fuu. */
  /* @TE7: size() should be equal to STRINGCAP. */
  /* @TS8: Invoke the c_str() of fuu. */
  /* @TE8: c_str() should be equal to testString. */
  /* @End: UT_CORE_0289 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0290
@Test_Description:    To test the functionality of Assign String of size 0 & smaller capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignStringOfSize0AndSmallerCapaResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "d66ee5d5-03ac-4485-ac73-b8b6f044d38a");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> testString;
  testString.assign(this->testSubject);
  EXPECT_THAT(testString.size(), Eq(0U));
  EXPECT_THAT(testString.c_str(), StrEq(""));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0290 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString.*/
  /* @TS2: Create an instance of FixedString as testString by adding 1 to the capacity of MyString it. */
  /* @TE2: Instance testString should be created by increasing to the capacity of MyString. */
  /* @TS3: Assign testSubject to testString. */
  /* @TE3: testSubject should be assigned to testString. */
  /* @TS4: Invoke the size() of testString. */
  /* @TE4: size() of testString should be equal to 0. */
  /* @TS5: Invoke the c_str() of testString. */
  /* @TE5: c_str() of testString should be equal to null. */
  /* @TS6: Invoke the size() of testSubject. */
  /* @TE6: size() of testSubject should be equal to 0. */
  /* @TS7: Invoke the c_str() of testSubject. */
  /* @TE7: c_str() of testSubject should be equal to null. */
  /* @End: UT_CORE_0290 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0291
@Test_Description:    To test the functionality of Assign String with smaller capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignStringWithSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "d7bdad39-fd7f-4b32-9e3f-60e6cd246bdc");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testStdString(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testStdString), Eq(true));
  FixedString<STRINGCAP + 1U> testString;
  testString.assign(this->testSubject);
  EXPECT_THAT(testString.size(), Eq(STRINGCAP));
  EXPECT_THAT(testString.c_str(), StrEq(testStdString));
  EXPECT_THAT(this->testSubject.size(), Eq(STRINGCAP));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(testStdString));

  /* @Start: UT_CORE_0291 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testStdString and insert the capacity of MyString and parameter 'M' in it. */
  /* @TE2: Instance testStdString should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as testString by adding 1 to the capacity of MyString to it. */
  /* @TE3: Instance testString should be created by increasing 1 to the capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() of testSubject should return true. */
  /* @TS5: Invoke the size() of testString. */
  /* @TE5: size() of testString should be equal to STRINGCAP. */
  /* @TS6: Invoke the c_str() of testString. */
  /* @TE6: c_str() of testString should be equal to testStdString. */
  /* @TS7: Invoke the size() of testSubject. */
  /* @TE7: size() of testSubject should be equal to STRINGCAP. */
  /* @TS8: Invoke the c_str() of testSubject. */
  /* @TE8: c_str() of testString should be equal to testStdString. */
  /* @End: UT_CORE_0291 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0292
@Test_Description:    To test the functionality of Assign that results in size 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611410, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, FreshlyAssignNothingResultsInZeroSize)
{
  ::testing::Test::RecordProperty("TEST_ID", "c0df23ee-86f8-4821-b577-38bad21c2e77");
  this->testSubject.assign("");
  EXPECT_THAT(this->testSubject.size(), Eq(0U));

  /* @Start: UT_CORE_0292 */
  /* @TS1: Assign empty parameter to testSubject. */
  /* @TE1: Empty parameter should be assigned to the testSubject. */
  /* @TS2: Create an instance of FixedString as testSubject. */
  /* @TE2: Instance testSubject should be created. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: size() of testSubject should be equal to 0. */
  /* @End: UT_CORE_0292 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0293
@Test_Description:    To test the functionality of Reassign that results in size 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611410, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, ReassignNothingResultsInZeroSize)
{
  ::testing::Test::RecordProperty("TEST_ID", "854089ee-bf0f-432c-96ce-20e83e09181c");
  this->testSubject.assign("M");
  this->testSubject.assign("");
  EXPECT_THAT(this->testSubject.size(), Eq(0U));

  /* @Start: UT_CORE_0293 */
  /* @TS1: Assign parameter "M" to testSubject. */
  /* @TE1: Parameter "M" should be assigned to the testSubject. */
  /* @TS2: Assign empty parameter to testSubject. */
  /* @TE2: Empty parameter should be assigned to the testSubject. */
  /* @TS3: Create an instance of FixedString as testSubject. */
  /* @TE3: Instance testSubject should be created. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testSubject should be equal to 0. */
  /* @End: UT_CORE_0293 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0294
@Test_Description:    To test the functionality of AssignCString of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignCStringOfSizeCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "25f4f306-2303-4cc8-a42b-d0cbb600d833");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  char testChar[STRINGCAP];
  for (uint64_t i = 0U; i < STRINGCAP - 1U; i++)
  {
    testChar[i] = 'M';
  }
  testChar[STRINGCAP - 1U] = '\0';
  FixedString<STRINGCAP> testSubject;
  testSubject.assign(testChar);
  EXPECT_THAT(testSubject.size(), Eq(STRINGCAP - 1U));
  EXPECT_THAT(testSubject.c_str(), StrEq(testChar));
  /* @Start: UT_CORE_0294 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TS2: Create an instance of char array testChar and insert char with MyString capacity in it. */
  /* @TS3: Pass parameter "M" to the testChar[i]. */
  /* @TS4: Pass parameter '\0' to the testChar by substituting 1 to the capacity of MyString. */
  /* @TS5: Create an instance of FixedString as testSubject with capacity of MyString. */
  /* @TS6: Assign testChar to testSubject. */
  /* @TS7: Invoke the size() of testSubject. */
  /* @TS8: Invoke the c_str()  of testSubject. */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TE2: Instance testChar should be created with the same capacity of MyString. */
  /* @TE3: Parameter "M" should be passed to testChar[i]. */
  /* @TE4: Parameter '\0' should be passed to testChar by decreasing 1 to the capacity of MyString. */
  /* @TE5: Instance testSubject should be created with the same capacity of MyString. */
  /* @TE6: testChar should be assigned to testSubject. */
  /* @TE7: size() of testSubject should be equal to STRINGCAP by decreasing 1. */
  /* @TE8: c_str() of testSubject should be equal to testChar. */
  /* @End: UT_CORE_0294 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0295
@Test_Description:    To test the functionality of Unsafe Assign CString that results in size 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611412, 611423, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignOfCStringOfSize0ResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "861aa3a2-4fa3-401a-b6af-2ddbb3139f69");
  EXPECT_THAT(this->testSubject.unsafe_assign(""), Eq(true));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0295 */
  /* @TS1: Invoke the unsafe_assign() of testSubject. */
  /* @TE1: unsafe_assign() of testSubject should return true. */
  /* @TS2: Invoke the size() of testSubject. */
  /* @TE2: size() of testSubject should be equal to 0. */
  /* @TS3: Invoke the c_str() of testSubject. */
  /* @TE3: c_str() of testSubject should be equal to null. */
  /* @End: UT_CORE_0295 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0296
@Test_Description:    To test the functionality of Unsafe Assign CString that results in size 1 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611412, 611423, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignOfCStringOfSize1ResultsInSize1)
{
  ::testing::Test::RecordProperty("TEST_ID", "7b09a56c-1706-43b9-85d0-6c8bdf79f1b2");
  EXPECT_THAT(this->testSubject.unsafe_assign("M"), Eq(true));
  EXPECT_THAT(this->testSubject.size(), Eq(1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq("M"));

  /* @Start: UT_CORE_0296 */
  /* @TS1: Invoke the unsafe_assign() of testSubject. */
  /* @TE1: unsafe_assign() of testSubject should return true. */
  /* @TS2: Invoke the size() of testSubject. */
  /* @TE2: size() of testSubject should be equal to 1. */
  /* @TS3: Invoke the c_str() of testSubject. */
  /* @TE3: c_str() of testSubject should be equal to M. */
  /* @End: UT_CORE_0296 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0297
@Test_Description:    To test the functionality of UnsafeAssign CString that results in size capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611396, 611439, 611412, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignCStringOfSizeCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "422a0400-0862-441f-a7d0-20272856800f");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::vector<char> testCharstring(STRINGCAP, 'M');
  testCharstring.emplace_back('\0');
  EXPECT_THAT(this->testSubject.unsafe_assign(testCharstring.data()), Eq(true));
  EXPECT_THAT(this->testSubject.size(), Eq(STRINGCAP));

  /* @Start: UT_CORE_0297 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of vector as testCharstring and insert capacity of MyString, pass parameter 'M' in it.*/
  /* @TE2: Instance testCharstring should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of testCharstring as emplace_back with parameter '\0. */
  /* @TE3: Instance emplace_back should be created with parameter "0". */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() of testSubject should return true. */
  /* @TS5: Invoke the size() of testSubject . */
  /* @TE5: size() of testSubject should be equal to STRINGCAP. */
  /* @End: UT_CORE_0297 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0298
@Test_Description:    To test the functionality of Unsafe Assign CString of size of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignCStringOfSizeGreaterCapaResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "6012209b-87f8-4796-b9ec-336881cf7e24");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::vector<char> testCharstring(STRINGCAP + 1U, 'M');
  testCharstring.emplace_back('\0');
  EXPECT_THAT(this->testSubject.unsafe_assign(testCharstring.data()), Eq(false));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0298 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of vector as testCharstring and insert capacity of MyString by adding 1, pass parameter 'M' in it. */
  /* @TE2: Instance testCharstring should be created by increasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of testCharstring as emplace_back with parameter '\0. */
  /* @TE3: Instance emplace_back should be created with parameter "0". */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() of testSubject should return false. */
  /* @TS5: Invoke the size() of testSubject. */
  /* @TE5: size() should be equal to 0. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() should be equal to null. */
  /* @End: UT_CORE_0298 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0299
@Test_Description:    To test the functionality of Unsafe Assign of InvalidString of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423, 611412
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignOfInvalidCStringFails)
{
  ::testing::Test::RecordProperty("TEST_ID", "863d38e5-5c5f-4a78-a2ca-155205bb9ecc");
  this->testSubject = "L";

  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::vector<char> testCharstring(STRINGCAP + 1U, 'M');
  testCharstring.emplace_back('\0');

  EXPECT_THAT(this->testSubject.unsafe_assign(testCharstring.data()), Eq(false));
  EXPECT_THAT(this->testSubject.size(), Eq(1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq("L"));

  /* @Start: UT_CORE_0299 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of vector as testCharstring and insert capacity of MyString by adding 1, pass parameter 'M' in it.*/
  /* @TE2: Instance testCharstring should be created by increasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of testCharstring as emplace_back with parameter '\0'. */
  /* @TE3: Instance emplace_back should be created with parameter "0". */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() of testSubject should return false. */
  /* @TS5: Invoke the size() of testSubject. */
  /* @TE5: size() of testSubject should be equal to 1. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() of testSubject should be equal to L. */
  /* @End: UT_CORE_0299 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0300
@Test_Description:    To test the functionality of Unsafe assign of CharPtr pointing to same address of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611396, 611412, 611413
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignOfCharPointerPointingToSameAddress)
{
  ::testing::Test::RecordProperty("TEST_ID", "a129add2-4d8e-4953-ba28-b78ece5a2f02");
  this->testSubject = "M";
  const char* fuu = this->testSubject.c_str();
  EXPECT_THAT(this->testSubject.unsafe_assign(fuu), Eq(false));

  /* @Start: UT_CORE_0300 */
  /* @TS1: Create an instance as testSubject and pass parameter "M" to it.*/
  /* @TE1: Instance testSubject should be created by passing the parameter "M". */
  /* @TS2: Create an instance as fuu and assign testSubject.c_str to it.*/
  /* @TE2: Instance fuu should be created by assinging testSubject.c_str to it. */
  /* @TS3: Invoke the unsafe_assign() of testSubject. */
  /* @TE3: unsafe_assign() of testSubject should return false. */
  /* @End: UT_CORE_0300 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0301
@Test_Description:    To test the functionality of unsafe_assign of nullptr of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611396, 611412, 611413
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignOfNullptrFails)
{
  ::testing::Test::RecordProperty("TEST_ID", "140e5402-c6b5-4a07-a0f7-2a10f6d307fb");
  EXPECT_THAT(this->testSubject.unsafe_assign(nullptr), Eq(false));

  /* @Start: UT_CORE_0301 */
  /* @TS1: Create an instance of FixedString as testSubject. */
  /* @TE1: Instance testSubject should be created. */
  /* @TS2: Invoke the unsafe_assign() of testSubject. */
  /* @TE2: unsafe_assign() of testSubject should return false. */
  /* @End: UT_CORE_0301 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0302
@Test_Description:    To test the functionality of STD String size that results in size 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611396, 611412, 611413, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignOfSTDStringOfSize0ResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "709e4c86-963b-495e-a81c-c09a84cb2320");
  std::string testString;
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0302 */
  /* @TS1: Create an instance of string as testString. */
  /* @TE1: Instance testString should be created. */
  /* @TS2: Invoke the unsafe_assign() of testSubject. */
  /* @TE2: unsafe_assign() of testSubject should return true. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: size() of testSubject should be equal to 0. */
  /* @TS4: Invoke the c_str() of testSubject. */
  /* @TE4: c_str() of testSubject should be equal to null. */
  /* @End: UT_CORE_0302 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0303
@Test_Description:    To test the functionality of STD String size that results in size 1 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test 
@Requirement Tracing: -
@Design Tracing:      611396, 611412, 611413, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignOfSTDStringOfSize1ResultsInSize1)
{
  ::testing::Test::RecordProperty("TEST_ID", "84dc3655-a4c8-46bc-afd2-059501da1c86");
  std::string testString = "M";
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  EXPECT_THAT(this->testSubject.size(), Eq(1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq("M"));

  /* @Start: UT_CORE_0303 */
  /* @TS1: Create an instance of string as testSubject and pass parameter "M" to it. */
  /* @TE1: Instance testSubject should be created by passing parameter "M". */
  /* @TS2: Invoke the unsafe_assign() of testSubject. */
  /* @TE2: unsafe_assign()  of testSubject should return true. */
  /* @TS3: Invoke the size() of testSubject. */
  /* @TE3: size() of testSubject should be equal to 1. */
  /* @TS4: Invoke the c_str() of testSubject. */
  /* @TE4: c_str() of testSubject should be equal to M. */
  /* @End: UT_CORE_0303 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0304
@Test_Description:    To test the functionality of Unsafe assign STD String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611396, 611412, 611413, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignSTDStringOfSizeCapaResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "27011e92-813a-410a-ba14-fec7dfefe942");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  EXPECT_THAT(this->testSubject.size(), Eq(STRINGCAP));

  /* @Start: UT_CORE_0304 */
  /* @TS1: Create an instance of string as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testSubject with the capacity of MyString and pass parameter 'M' to it. */
  /* @TE2: Instance testSubject should be created with capacity() of MyString. */
  /* @TS3: Invoke the unsafe_assign() of testSubject. */
  /* @TE3: unsafe_assign() of testSubject should return true. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testSubject should be equal to STRINGCAP. */
  /* @End: UT_CORE_0304 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0305
@Test_Description:    To test the functionality of String size of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611396, 611412, 611413, 611422ss
*******************************************************************************/
TYPED_TEST(stringTyped_test, UnsafeAssignSTDStringOfSizeGreaterCapaResultsInSize0)
{
  ::testing::Test::RecordProperty("TEST_ID", "72dbeffb-5787-41cf-9998-b745d8683bb4");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP + 1U, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(false));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));

  /* @Start: UT_CORE_0305 */
  /* @TS1: Create an instance of string as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testSubject by adding 1 the capacity of MyString and pass parameter 'M' to it. */
  /* @TE2: Instance testSubject should be created by increasing 1 to the capacity of MyString. */
  /* @TS3: Invoke the unsafe_assign() of testSubject. */
  /* @TE3: unsafe_assign() of testSubject should return false. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testSubject should be equal to 0. */
  /* @End: UT_CORE_0305 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0306
@Test_Description:    To test the functionality of Assign of Invalid STD String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611396, 611412, 611413, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignOfInvalidSTDStringFails)
{
  ::testing::Test::RecordProperty("TEST_ID", "c16cec0c-40b5-43cb-adab-f95e11b96b8e");
  this->testSubject = "L";

  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP + 1U, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(false));
  EXPECT_THAT(this->testSubject.size(), Eq(1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq("L"));

  /* @Start: UT_CORE_0306 */
  /* @TS1: Create an instance of string as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of string as testSubject by adding 1 the capacity of MyString and pass parameter 'M' to it. */
  /* @TE2: Instance testSubject should be created by increasing 1 to the capacity of MyString. */
  /* @TS3: Invoke the unsafe_assign() of testSubject. */
  /* @TE3: unsafe_assign() of testSubject should return false. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testSubject should be equal to 1. */
  /* @TS5: Invoke the c_str() of testSubject. */
  /* @TE5: c_str() of testSubject should be equal to L. */
  /* @End: UT_CORE_0306 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0307
@Test_Description:    To test the functionality of String Equality that results in 0 of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareEqStringsResultsInZero)
{
  ::testing::Test::RecordProperty("TEST_ID", "8271fa31-3301-4d2e-a290-a4b61a70dfb5");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP> fuu;
  EXPECT_THAT(fuu.unsafe_assign(testString), Eq(true));
  EXPECT_THAT(this->testSubject.compare(this->testSubject), Eq(0));
  EXPECT_THAT(this->testSubject.compare(fuu), Eq(0));

  /* @Start: UT_CORE_0307 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of string as testSubject with the capacity of MyString and pass parameter "M" to it. */
  /* @TE2: Instance testSubject should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu with the capacity of MyString. */
  /* @TE3: Instance fuu should be created with the same capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() of testSubject should return true */
  /* @TS5: Invoke the compare() of testSubject. */
  /* @TE5: compare() of testSubject should be equal to 0. */
  /* @TS6: Invoke the compare() of testSubject. */
  /* @TE6: compare() of testSubject should be equal to 0. */
  /* @End: UT_CORE_0307 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0308
@Test_Description:    To test the functionality of Result comparison of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611412, 611413
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareResultNegative)
{
  ::testing::Test::RecordProperty("TEST_ID", "b17fc495-a82b-4ee8-af17-28afaabd3f0e");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString1(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString1), Eq(true));
  FixedString<STRINGCAP> fuu;
  std::string testString2(STRINGCAP, 'L');
  EXPECT_THAT(fuu.unsafe_assign(testString2), Eq(true));
  EXPECT_THAT(fuu.compare(this->testSubject), Lt(0));

  /* @Start: UT_CORE_0308 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString1 insert the capacity of MyString and pass parameter 'M' to it. */
  /* @TE2: Instance testString1 should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of string as testString2 insert the capacity of MyString and pass parameter 'L' to it. */
  /* @TE3: Instance testString2 should be created with the same capacity of MyString. */
  /* @TS4: Create an instance of FixedString as fuu with the capacity of MyString. */
  /* @TE4: Instance fuu should be created with the same capacity of MyString. */
  /* @TS5: Invoke the unsafe_assign() of testSubject. */
  /* @TE5: unsafe_assign() of testString1 of should return  true. */
  /* @TS6: Invoke the unsafe_assign() of fuu. */
  /* @TE6: unsafe_assign() of fuu should return true */
  /* @TS7: Invoke the compare of fuu. */
  /* @TE7: compare of fuu should be less than 0. */
  /* @End: UT_CORE_0308 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0309
@Test_Description:    To test the functionality of result of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611412, 611413, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareResultPositive)
{
  ::testing::Test::RecordProperty("TEST_ID", "ff95b244-937a-4519-90d8-8c82acf01b6b");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString1(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString1), Eq(true));
  FixedString<STRINGCAP> fuu;
  std::string testString2(STRINGCAP, 'L');
  EXPECT_THAT(fuu.unsafe_assign(testString2), Eq(true));
  EXPECT_THAT(this->testSubject.compare(fuu), Gt(0));

  /* @Start: UT_CORE_0309 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: Instance testString1 should be created with the same capacity of MyString. */
  /* @TS2: Create an instance of string as testString1 insert the capacity of MyString and pass parameter 'M' to it. */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of string as testString2 insert the capacity of MyString and pass parameter 'L' to it. */
  /* @TE3: Instance testString2 should be created with the same capacity of MyString. */
  /* @TS4: Create an instance of FixedString as fuu with the capacity of MyString. */
  /* @TE4: unsafe_assign() of testString1 should return true. */
  /* @TS5: Invoke the compare of testSubject. */
  /* @TE5: unsafe_assign() of testString2 should return true. */
  /* @TS6: capacity() should return the capacity of MyString. */
  /* @TE6: compare of fuu should be greater than 0. */
  /* @End: UT_CORE_0309 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0310
@Test_Description:    To test the functionality of Empty String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611412, 611413, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareWithEmptyStringResultsInPositive)
{
  ::testing::Test::RecordProperty("TEST_ID", "0495349d-2e63-442f-8214-b44d249f057f");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  EXPECT_THAT(fuu.compare(this->testSubject), Gt(0));

  /* @Start: UT_CORE_0310 */
  /* @TS1: Create an instance of FixedString as MyString and Invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with capacity of MyString and pass parameter "M" in it. */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Invoke the compare of fuu. */
  /* @TE3: compare of fuu should be greater than 0. */
  /* @End: UT_CORE_0310 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0311
@Test_Description:    To test the functionality of String that Incl null chara of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          String100
@Requirement Tracing: -
@Design Tracing:      611439, 611414, 611412, 611413
*******************************************************************************/
TEST(String100, CompareStringsInclNullCharacterWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "a6c1b983-a88c-46bb-a38f-33947e52f209");
  std::string testString1{"ice\0ryx", 7};
  std::string testString2{"ice\0rYx", 7};
  FixedString<100U> testSubject1(ara::core::TruncateToCapacity, testString1.c_str(), 7U);
  FixedString<100U> testSubject2(ara::core::TruncateToCapacity, testString2.c_str(), 7U);
  EXPECT_THAT(testSubject1.compare(testSubject2), Gt(0));

  /* @Start: UT_CORE_0311 */
  /* @TS1: Create an instance of string as testString1 and pass parameter "ice\0ryx", capacity 7. */
  /* @TE1: Instance testString1 should be created. */
  /* @TS2: Create an instance of string as testString2 and pass parameter "ice\0ryx", capacity 7. */
  /* @TE2: Instance testString2 should be created. */
  /* @TS3: Create an instance of FixedString as testSubject1 with capacity 100 and insert TruncateToCapacity, testString1.c_str, 7 in it. */
  /* @TE3: Instance testSubject1 should be created with the capacity 100. */
  /* @TS4: Create an instance of FixedString as testSubject2 with capacity 100 and insert TruncateToCapacity, testString1.c_str, 7 in it. */
  /* @TE4: Instance testSubject2 should be created with the capacity 100. */
  /* @TS5: Invoke the compare of testSubject1. */
  /* @TE5: compare should be greater than 0. */
  /* @End: UT_CORE_0311 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0312
@Test_Description:    To test the functionality of equality string comparison of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611414, 611412, 611413
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareEqStringsWithDifferentCapaResultsInZero)
{
  ::testing::Test::RecordProperty("TEST_ID", "ddf78ea1-b5cd-44fd-9320-6801893c30e7");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP + 1U> fuu;
  EXPECT_THAT(fuu.unsafe_assign(testString), Eq(true));
  EXPECT_THAT(this->testSubject.compare(this->testSubject), Eq(0));
  EXPECT_THAT(this->testSubject.compare(fuu), Eq(0));

  /* @Start: UT_CORE_0312 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString insert the capacity of MyString and pass parameter 'M' to it. */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu by adding 1 to the capacity of MyString. */
  /* @TE3: Instance fuu should be created by increasing 1 to the capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() of testSubject should return return true */
  /* @TS5: Invoke the unsafe_assign() of fuu. */
  /* @TE5: unsafe_assign() of fuu should return true. */
  /* @TS6: Invoke the compare of testSubject. */
  /* @TE6: compare of testSubject should be equal to 0. */
  /* @TS7: Invoke the compare of testSubject. */
  /* @TE7: compare of testSubject should be equal to 0. */
  /* @End: UT_CORE_0312 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0313
@Test_Description:    To test the functionality of Result of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611412, 611413
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareResultNegativeWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "a0499ebc-249c-4b31-a1b0-0e34035e77f2");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString1(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString1), Eq(true));
  FixedString<STRINGCAP + 1U> fuu;
  std::string testString2(STRINGCAP + 1U, 'M');
  EXPECT_THAT(fuu.unsafe_assign(testString2), Eq(true));
  EXPECT_THAT(this->testSubject.compare(fuu), Lt(0));

  /* @Start: UT_CORE_0313 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString1 with capacity of MyString and parameter 'M'. */
  /* @TE2: Instance testString1 should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of string as testString2 by adding 1 to the capacity of MyString and pass parameter 'M'. */
  /* @TE3: Instance testString2 should be created by increasing 1 to the capacity of MyString. */
  /* @TS4: Create an instance of FixedString as fuu by adding 1 to the capacity of MyString. */
  /* @TE4: Instance fuu should be created by increasing 1 to the capacity of MyString. */
  /* @TS5: Invoke the unsafe_assign() of testSubject. */
  /* @TE5: unsafe_assign() of testSubject should return true. */
  /* @TS6: Invoke the unsafe_assign() of fuu. */
  /* @TE6: unsafe_assign() of fuu should return true. */
  /* @TS7: Invoke the compare() of testSubject. */
  /* @TE7: compare of testSubject should be less than 0. */
  /* @End: UT_CORE_0313 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0314
@Test_Description:    To test the functionality of result that compare with different capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611412, 611413
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareResultPositiveWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "97fed2cb-4f25-4732-9bbe-4d710b9a35f7");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString1(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString1), Eq(true));
  FixedString<STRINGCAP + 1U> fuu;
  std::string testString2(STRINGCAP + 1U, 'M');
  EXPECT_THAT(fuu.unsafe_assign(testString2), Eq(true));
  EXPECT_THAT(fuu.compare(this->testSubject), Gt(0));

  /* @Start: UT_CORE_0314 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString1 with capacity of MyString and parameter 'M'. */
  /* @TE2: Instance testString1 should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of string as testString2 by adding 1 to the capacity of MyString and pass parameter 'M'. */
  /* @TE3: Instance testString2 should be created by increasing 1 to the capacity of MyString. */
  /* @TS4: Create an instance of FixedString as fuu by adding 1 to the capacity of MyString. */
  /* @TE4: Instance fuu should be created by increasing 1 to the capacity of MyString. */
  /* @TS5: Invoke the unsafe_assign() of testSubject . */
  /* @TE5: unsafe_assign() of testSubject should return true */
  /* @TS6: Invoke the unsafe_assign() of fuu . */
  /* @TE6: unsafe_assign() of fuu should return true */
  /* @TS7: Invoke the compare of fuu. */
  /* @TE7: compare of fuu should be Greater than to 0. */
  /* @End: UT_CORE_0314 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0315
@Test_Description:    To test the functionality of Empty String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611421, 611414
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareWithEmptyStringOfDifferentCapaResultsInPositive)
{
  ::testing::Test::RecordProperty("TEST_ID", "9d54c681-fc19-444b-8e1b-059ff6237a8f");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> fuu("M");
  EXPECT_THAT(fuu.compare(this->testSubject), Gt(0));

  /* @Start: UT_CORE_0315 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu by adding 1 to the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance fuu should be created by increasing 1 to the capacity of MyString. */
  /* @TS3: Invoke the compare() () of fuu. */
  /* @TE3: compare() of fuu should return Greater than 0. */
  /* @End: UT_CORE_0315 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0316
@Test_Description:    To test the functionality of String with different capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          String100
@Requirement Tracing: -
@Design Tracing:      611439, 611421, 611414
*******************************************************************************/
TEST(String100, CompareStringsWithDifferentCapaInclNullCharacterWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "fe260cb6-5d77-42b1-89b8-073c9ea9593d");
  std::string testString1{"ice\0ryx", 7};
  std::string testString2{"ice\0rYx", 7};
  FixedString<200U> testSubject1(ara::core::TruncateToCapacity, testString1.c_str(), 7U);
  FixedString<100U> testSubject2(ara::core::TruncateToCapacity, testString2.c_str(), 7U);
  EXPECT_THAT(testSubject1.compare(testSubject2), Gt(0));

  /* @Start: UT_CORE_0316 */
  /* @TS1: Create an instance of string as testString1 and insert parameter "ice\0ryx", capacity 7. */
  /* @TE1: Instance testString1 should be created with parameter "ice\0ryx", capacity 7. */
  /* @TS2: Create an instance of string as testString2 and insert parameter "ice\0ryx", capacity 7. */
  /* @TE2: Instance testString2 should be created with parameter "ice\0ryx", capacity 7. */
  /* @TS3: Create an instance of FixedString as testSubject1 with capacity 200U and insert testString1.c_str 7U. */
  /* @TE3: Instance fuu should be created with capacity 200. */
  /* @TS4: Create an instance of FixedString as testSubject2 with capacity 100U and insert testString2.c_str 7U. */
  /* @TE4: Instance fuu should be created with capacity 100. */
  /* @TS5: Invoke the compare of testSubject1. */
  /* @TE5: compare(testSubject2) of testSubject1 should return Greater than 0. */
  /* @End: UT_CORE_0316 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0317
@Test_Description:    To test the functionality of operator== for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415 
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorEqualResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "149069dd-a2f2-441a-9d16-30aa038a7c5e");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  EXPECT_THAT(fuu == fuu, Eq(true));

  /* @Start: UT_CORE_0317 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Invoke == operator for fuu and fuu. */
  /* @TE3: operator == should return true. */
  /* @End: UT_CORE_0317 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0318
@Test_Description:    To test the functionality of operator== of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415 
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorEqualResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "30c9bb50-03ea-437e-99c8-c663ed62340b");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::cout << "capacitiy at 2207 line   " << STRINGCAP << std::endl;
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(fuu == bar, Eq(false));

  /* @Start: UT_CORE_0318 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as bar with the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance bar should be created with the same capacity of MyString. */
  /* @TS4: Invoke == operator for fuu and bar. */
  /* @TE4: operator == should return false. */
  /* @End: UT_CORE_0318 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0319
@Test_Description:    To test the functionality of operator== for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415 
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorEqualResultTrueWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "10ce496d-635e-4aeb-9969-95881d1efc87");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testString1("M");
  FixedString<STRINGCAP + 1U> testString2("M");
  EXPECT_THAT(testString1 == testString2, Eq(true));

  /* @Start: UT_CORE_0319 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should be equal to capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString1 with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString1 should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as testString2 with the capacity of MyString and pass parameter "M". */
  /* @TE3: Instance testString2 should be created with the same capacity of MyString. */
  /* @TS4: Invoke == operator for testString1 and testString2. */
  /* @TE4: operator == should return true */
  /* @End: UT_CORE_0319 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0320
@Test_Description:    To test the functionality of operator== for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415 
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorEqualResultFalseWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "15444c15-7319-4224-8519-091d2b47da22");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> testString1("M");
  FixedString<STRINGCAP> testString2("L");
  FixedString<STRINGCAP + 1U> testString3;
  std::string testStdString(STRINGCAP + 1U, 'L');
  EXPECT_THAT(testString3.unsafe_assign(testStdString), Eq(true));
  EXPECT_THAT(testString1 == testString2, Eq(false));
  EXPECT_THAT(testString3 == testString2, Eq(false));

  /* @Start: UT_CORE_0320 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString1 by adding 1 to the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString1 should be created by increasing the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as testString2 by with the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance testString2 should be created with the same capacity of MyString. */
  /* @TS4: Create an instance of FixedString as testString3 by adding 1 to the capacity of MyString and pass parameter "L". */
  /* @TE4: Instance testString3 should be created by increasing the capacity of MyString. */
  /* @TS5: Create an instance of string as testStdString and pass parameter of "L". */
  /* @TE5: Instance testStdString should be created and passed parameter "L". */
  /* @TS6: Invoke unsafe_assign() for testString3 and pass argument as testStdString. */
  /* @TE6: unsafe_assign() of testString3 should return true. */
  /* @TS7: Invoke == operator for testString1 and testString2. */
  /* @TE7: operator == for testString1 and testString2 should return false. */
  /* @TS8: Invoke == operator for testString3 and testString2. */
  /* @TE8: operator == for testString3 and testString2 should return false. */
  /* @End: UT_CORE_0320 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0321
@Test_Description:    To test the functionality of operator>= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611416
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorNotEqualResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "b31770c0-2695-4fd6-b5b6-cae479852417");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  EXPECT_THAT(fuu != fuu, Eq(false));

  /* @Start: UT_CORE_0321 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Invoke != operator for fuu and fuu. */
  /* @TE3: operator != should return false. */
  /* @End: UT_CORE_0321 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0322
@Test_Description:    To test the functionality of operator!= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611416
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorNotEqualResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "57db46d4-df73-49da-aa43-0dfc8ff04f44");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(fuu != bar, Eq(true));

  /* @Start: UT_CORE_0322 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as bar with the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance bar should be created with the same capacity of MyString. */
  /* @TS4: Invoke != operator for fuu and bar. */
  /* @TE4: operator != should return true. */
  /* @End: UT_CORE_0322 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0323
@Test_Description:    To test the functionality of operator!= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611416
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorNotEqualResultFalseWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "f5b53871-4bb0-4f2a-adea-e5335c6c4611");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testString1("M");
  FixedString<STRINGCAP + 1U> testString2("M");
  EXPECT_THAT(testString1 != testString2, Eq(false));

  /* @Start: UT_CORE_0323 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString1 with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString1 should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as testString2 by adding 1 to the capacity of MyString and pass parameter "M". */
  /* @TE3: Instance testString2 should be created by increasing 1 to the capacity of MyString. */
  /* @TS4: Invoke != operator for testString1 and testString2. */
  /* @TE4: operator != should return false. */
  /* @End: UT_CORE_0323 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0324
@Test_Description:    To test the functionality of operator!= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415 
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorNotEqualResultTrueWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "394d3dd9-2304-4608-9f29-12cfaacaeef7");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> testString1("M");
  FixedString<STRINGCAP> testString2("L");
  FixedString<STRINGCAP + 1U> testString3;
  std::string testStdString(STRINGCAP + 1U, 'L');
  EXPECT_THAT(testString3.unsafe_assign(testStdString), Eq(true));
  EXPECT_THAT(testString1 != testString2, Eq(true));
  EXPECT_THAT(testString3 != testString2, Eq(true));
  /* @Start: UT_CORE_0324 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString1 with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString1 should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as testString2 with the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance testString2 should be created with the same capacity of MyString. */
  /* @TS4: Create an instance of FixedString as testString3 by adding 1 to the capacity of MyString. */
  /* @TE4: Instance testString3 should be created by increasing 1 to the capacity of MyString. */
  /* @TS5: Create an instance of string as testStdString by adding 1 to the capacity of MyString and pass parameter "L". */
  /* @TE5: Instance testStdString should be created by increasing 1 to the capacity of MyString. */
  /* @TS6: Invoke unsafe_assign() testString3. */
  /* @TE6: unsafe_assign() testString3 should return true. */
  /* @TS7: Invoke == operator for testString1 and testString2. */
  /* @TE7: operator == for testString1 and testString2 should return true. */
  /* @TS8: Invoke == operator for testString3 and testString2. */
  /* @TE8: operator == for testString3 and testString2 should return true. */
  /* @End: UT_CORE_0324 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0325
@Test_Description:    To test the functionality of operator< for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611417, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorlessResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "cb8495ba-f2f9-4e7c-a017-f8295fcff518");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(bar < fuu, Eq(true));

  /* @Start: UT_CORE_0325 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as bar with the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance bar should be created with the same capacity of MyString. */
  /* @TS4: Invoke operator< for bar and fuu. */
  /* @TE4: operator< for bar and fuu should return true. */
  /* @End: UT_CORE_0325 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0326
@Test_Description:    To test the functionality of operator< for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      613228,611417, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorlessResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "6bf80f49-f637-4207-900e-2cfec89eb556");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(fuu < bar, Eq(false));
  EXPECT_THAT(fuu < fuu, Eq(false));

  /* @Start: UT_CORE_0326 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as bar with the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance bar should be created with the same capacity of MyString. */
  /* @TS4: Invoke < operator for fuu and bar. */
  /* @TE4: operator < for fuu and bar should return false. */
  /* @TS5: Invoke < operator for fuu and fuu. */
  /* @TE5: operator < for fuu and fuu should return false. */
  /* @End: UT_CORE_0326 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0327
@Test_Description:    To test the functionality of operator< for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      613228,611417, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorlessResultTrueWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "bbe252cb-6fba-45d1-ad31-cac42fb73caa");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testString1("M");
  FixedString<STRINGCAP + 1U> testString2("L");
  EXPECT_THAT(testString2 < testString1, Eq(true));

  /* @Start: UT_CORE_0327 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString1 with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString1 should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as testString2 by adding 1 to the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance testString2 should be created with the same capacity of MyString. */
  /* @TS4: Invoke < operator for testString2 and testString1. */
  /* @TE4: operator < for testString2 and testString1 should return true. */
  /* @End: UT_CORE_0327 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0328
@Test_Description:    To test the functionality of operator< for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      613228,611417, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorlessResultFalseWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "884217ee-085b-479d-8ab1-a0476f14105c");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> testString1("M");
  FixedString<STRINGCAP> testString2("L");
  EXPECT_THAT(testString1 < testString2, Eq(false));
  EXPECT_THAT(testString1 < testString1, Eq(false));

  /* @Start: UT_CORE_0328 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString1 by adding 1 to the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString1 should be created by increasing 1 to the capacity of MyString. */
  /* @TS3: Create an instance of FixedString as testString2 with the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance testString2 should be created with the same capacity of MyString. */
  /* @TS4: Invoke < operator for testString1 and testString2. */
  /* @TE4: operator < for testString1 and testString2 should return false. */
  /* @TS5: Invoke < operator for testString1 and testString1. */
  /* @TE5: operator < for testString1 and testString1 should return false. */
  /* @End: UT_CORE_0328 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0329
@Test_Description:    To test the functionality of operator<= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611418, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorlessEqResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "3a8ba399-4e2c-483d-835a-6d4bcbfa9a29");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(this->testSubject <= fuu, Eq(true));
  EXPECT_THAT(bar <= fuu, Eq(true));

  /* @Start: UT_CORE_0329 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as bar with the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance bar should be created with the same capacity of MyString. */
  /* @TS4: Invoke <= operator for testSubject and fuu. */
  /* @TE4: operator <= for testSubject and fuu should return true. */
  /* @TS5: Invoke <= operator for bar and fuu. */
  /* @TE5: operator <= for bar and fuu should return true. */
  /* @End: UT_CORE_0329 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0330
@Test_Description:    To test the functionality of operator<= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611418, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorlessEqResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "59928731-2d4b-4122-8924-7809001fa631");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(fuu <= bar, Eq(false));

  /* @Start: UT_CORE_0330 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as bar with the capacity of MyString and pass parameter "L". */
  /* @TE3: Instance bar should be created with the same capacity of MyString. */
  /* @TS4: Invoke < operator for fuu and bar. */
  /* @TE4: operator <= for fuu and bar should return true. */
  /* @End: UT_CORE_0330 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0331
@Test_Description:    To test the functionality of operator<= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611418, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorlessEqResultTrueWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "60c20357-c67a-4d4f-aca7-1ce3ab84677c");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP + 1U> bar("L");
  EXPECT_THAT(this->testSubject <= fuu, Eq(true));
  EXPECT_THAT(bar <= fuu, Eq(true));

  /* @Start: UT_CORE_0331 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu with the capacity of MyStringand pass parameter "M". */
  /* @TE2: Instance fuu should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as bar by adding 1 to the capacity of MyStringand pass parameter "L". */
  /* @TE3: Instance bar should be created by increasing 1 to the capacity of MyString. */
  /* @TS4: Invoke <= operator for testSubject and fuu. */
  /* @TE4: operator <= for testSubject and fuu should return true. */
  /* @TS5: Invoke <= operator for bar and fuu. */
  /* @TE5: operator <= for bar and fuu should return true. */
  /* @End: UT_CORE_0331 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0332
@Test_Description:    To test the functionality of operator<= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611418, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorlessEqResultFalseWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "9126ec2b-eb0c-4567-aa9e-8009e9910571");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(fuu <= bar, Eq(false));

  /* @Start: UT_CORE_0332 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass parameter "M". */
  /* @TE2: Instance fuu should be created. */
  /* @TS3: Create an instance of FixedString as bar and pass parameter "L". */
  /* @TE3: Instance bar should be created. */
  /* @TS4: Invoke <= operator for fuu and bar. */
  /* @TE4: operator <= fuu bar and bar should return false. */
  /* @End: UT_CORE_0332 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0333
@Test_Description:    To test the functionality of operator> for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611419, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorGreaterResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "92c81295-3169-49ac-a12f-700a5dd2179b");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(fuu > bar, Eq(true));

  /* @Start: UT_CORE_0333 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass parameter "M". */
  /* @TE2: Instance fuu should be created. */
  /* @TS3: Create an instance of FixedString as bar and pass parameter "L". */
  /* @TE3: Instance bar should be created. */
  /* @TS4: Invoke > operator for bar and fuu. */
  /* @TE4: operator > for bar and fuu should return true. */
  /* @End: UT_CORE_0333 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0334
@Test_Description:    To test the functionality of operator> for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611419, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorGreaterResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "5b3d33af-c905-44de-9ece-c66a96e59fb9");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(bar > fuu, Eq(false));
  EXPECT_THAT(bar > bar, Eq(false));

  /* @Start: UT_CORE_0334 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass parameter "M". */
  /* @TE2: Instance fuu should be created. */
  /* @TS3: Create an instance of FixedString as bar and pass parameter "L". */
  /* @TE3: Instance bar should e created. */
  /* @TS4: Invoke > operator for bar and fuu. */
  /* @TE4: operator > for bar and fuu should return false. */
  /* @TS5: Invoke > operator for bar and bar. */
  /* @TE5: operator > for bar and bar should return false. */
  /* @End: UT_CORE_0334 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0335
@Test_Description:    To test the functionality of operator> for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611419, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorGreaterResultTrueWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "fb2b2abd-4670-44e9-a1ea-e1ae2d437ce2");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(fuu > bar, Eq(true));

  /* @Start: UT_CORE_0335 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass parameter "M". */
  /* @TE2: Instance fuu should be created with passed parameter "M". */
  /* @TS3: Create an instance of FixedString as bar and pass parameter "L". */
  /* @TE3: Instance bar should be created with passed parameter "L". */
  /* @TS4: Invoke > operator for fuu and bar. */
  /* @TE4: operator > should return true. */
  /* @End: UT_CORE_0335 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0336
@Test_Description:   To test the functionality of operator> for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611419, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorGreaterResultFalseWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "5bacb492-2d28-4260-96d2-516858462054");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP + 1U> bar("L");
  EXPECT_THAT(bar > fuu, Eq(false));
  EXPECT_THAT(bar > bar, Eq(false));

  /* @Start: UT_CORE_0336 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass parameter "M". */
  /* @TE2: Instance fuu should be created with passed parameter "M". */
  /* @TS3: Create an instance of FixedString as bar and pass parameter "L". */
  /* @TE3: Instance bar should be created with passed parameter "L". */
  /* @TS4: Invoke > operator for bar and fuu. */
  /* @TE4: operator > for bar and fuu should return false. */
  /* @TS5: Invoke > operator for bar and bar. */
  /* @TE5: operator > for bar and bar should return false. */
  /* @End: UT_CORE_0336 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0337
@Test_Description:    To test the functionality of operator>= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611420, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorGreaterEqResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "97184a06-2153-4b8e-93b7-0c620b90efb2");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  this->testSubject = "M";
  EXPECT_THAT(fuu >= bar, Eq(true));
  EXPECT_THAT(fuu >= this->testSubject, Eq(true));

  /* @Start: UT_CORE_0337 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass parameter "M". */
  /* @TE2: Instance fuu should be created with the passed parameter "M". */
  /* @TS3: Create an instance of FixedString as bar and pass parameter "L". */
  /* @TE3: Instance bar should be created with the passed parameter "L". */
  /* @TS4: Invoke >= operator for fuu and bar. */
  /* @TE4: operator >= for fuu and bar should return true. */
  /* @TS5: Invoke >= operator for fuu and testSubject. */
  /* @TE5: operator >= for fuu and testSubject should return true. */
  /* @End: UT_CORE_0337 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0338
@Test_Description:    To test the functionality of operator>= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611420, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorGreaterEqResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "72dca46d-86aa-4a01-b282-97b46f7f1504");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP> bar("L");
  EXPECT_THAT(bar >= fuu, Eq(false));

  /* @Start: UT_CORE_0338 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass parameter "M". */
  /* @TE2: Instance fuu should be created. */
  /* @TS3: Create an instance of FixedString as bar and pass parameter "L". */
  /* @TE3: Instance bar should be created. */
  /* @TS4: Invoke >= operator for bar and fuu. */
  /* @TE4: operator >= for bar and fuu should return false. */
  /* @End: UT_CORE_0338 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0339
@Test_Description:    To test the functionality of operator>= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611420, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorGreaterEqResultTrueWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "edb0b737-630e-4305-9fd6-0f9c8be524b4");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> fuu("M");
  FixedString<STRINGCAP> bar("L");
  this->testSubject = "M";
  EXPECT_THAT(fuu >= bar, Eq(true));
  EXPECT_THAT(fuu >= this->testSubject, Eq(true));

  /* @Start: UT_CORE_0339 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass parameter "M". */
  /* @TE2: Instance fuu should be created. */
  /* @TS3: Create an instance of FixedString as bar and pass parameter "L". */
  /* @TE3: Instance bar should be created. */
  /* @TS4: Assign parameter "M" to  testSubject. */
  /* @TE4: Parameter "M" should be assigned to testSubject. */
  /* @TS5: Invoke >= operator for fuu and bar. */
  /* @TE5: operator >= for fuu and bar should return true. */
  /* @TS6: Invoke >= operator for fuu and testSubject. */
  /* @TE6: operator >= for fuu and testSubject should return true. */
  /* @End: UT_CORE_0339 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0340
@Test_Description:    To test the functionality of operator>= for stringType.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611420, 611439
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorGreaterEqResultFalseWithDifferentCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "aa2e606e-28a2-424b-ac98-b1d194a11738");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> fuu("M");
  FixedString<STRINGCAP + 1U> bar("L");
  this->testSubject = "L";
  EXPECT_THAT(bar >= fuu, Eq(false));

  /* @Start: UT_CORE_0340 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as fuu and pass parameter "M". */
  /* @TE2: Instance fuu should be created. */
  /* @TS3: Create an instance of FixedString as bar and pass parameter "L". */
  /* @TE3: Instance bar should be created. */
  /* @TS4: Assign parameter "M" to  testSubject. */
  /* @TE4: Parameter "M" should be assigned to testSubject. */
  /* @TS5: Invoke >= operator for bar and fuu. */
  /* @TE5: operator >= for bar and fuu should return false. */
  /* @End: UT_CORE_0340 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0341
@Test_Description:    To test the functionality of Empty String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611423, 611422
*******************************************************************************/
TYPED_TEST(stringTyped_test, EmptyStringToSTDStringConvResultsInZeroSize)
{
  ::testing::Test::RecordProperty("TEST_ID", "753888b8-12e2-4534-a2fd-32b29b457803");
  std::string testString = std::string(this->testSubject);
  EXPECT_THAT(testString.size(), Eq(0U));
  EXPECT_THAT(testString.c_str(), StrEq(""));

  /* @Start: UT_CORE_0341 */
  /* @TS1: Create an instance of std::string as testString and insert string(testSubject) to it. */
  /* @TE1: Instance bar should be created with passed parameter "L". */
  /* @TS2: Invoke the size() of testString . */
  /* @TE2: size() of testString should be equal to 0. */
  /* @TS3: Invoke the c_str() of testString. */
  /* @TE3: c_str() of testString  should be equal to null. */
  /* @End: UT_CORE_0341 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0342
@Test_Description:    To test the functionality of String of Size of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423, 611412, 611413
*******************************************************************************/
TYPED_TEST(stringTyped_test, StringOfSizeCapaToSTDStringConvResultsInSizeCapa)
{
  ::testing::Test::RecordProperty("TEST_ID", "50e727f3-c855-4613-9e38-a56429fa5748");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString1(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString1), Eq(true));
  std::string testString2 = std::string(this->testSubject);
  EXPECT_THAT(testString2.size(), Eq(STRINGCAP));
  EXPECT_THAT(testString2.c_str(), StrEq(testString1.substr(0, STRINGCAP)));

  /* @Start: UT_CORE_0342 */
  /* @TS1: Create an instance of string as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of string as testString1 and insert capacity of MyString, pass parameter "M'. */
  /* @TE2: Instance testString1 should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of string as testString2 and Assign string(testSubject) to it. */
  /* @TE3: Instance testString2 should be created and assigned string(testSubject) to it. */
  /* @TS4: Invoke the unsafe_assign() of testSubject. */
  /* @TE4: unsafe_assign() of testSubject should return true. */
  /* @TS5: Invoke the size() of testString2 . */
  /* @TE5: size() of testString2 should be equal to STRINGCAP. */
  /* @TS6: Invoke the c_str() of testString2. */
  /* @TE6: c_str() of testString2 should be equal to testString1.substr(0, STRINGCAP). */
  /* @End: UT_CORE_0342 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0343
@Test_Description:    To test the functionality of operatr== of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorSTDStringEqualFixedStringResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "6c048e63-2e9f-443a-8d22-380f5fc41057");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "M";
  EXPECT_THAT(testSTDString == testFixedString, Eq(true));

  /* @Start: UT_CORE_0343 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of FixedString as testSTDString and pass parameter "M". */
  /* @TE3: Instance testSTDString should be created . */
  /* @TS4: Invoke == operator for testSTDString and testFixedString. */
  /* @TE4: operator == for testSTDString and testFixedString should return true. */
  /* @End: UT_CORE_0343 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0344
@Test_Description:    To test the functionality of operator== of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorSTDStringEqualFixedStringWithSameSizeResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "7500e443-43d0-4483-909e-0d4dc4c6b6bd");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "L";
  EXPECT_THAT(testSTDString == testFixedString, Eq(false));

  /* @Start: UT_CORE_0344 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of FixedString as testSTDString and pass parameter "L". */
  /* @TE3: Instance testSTDString should be created. */
  /* @TS4: Invoke == operator for testSTDString and testFixedString. */
  /* @TE4: operator == for testSTDString and testFixedString should return false. */
  /* @End: UT_CORE_0344 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0345
@Test_Description:    To test the functionality of String Equality of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorSTDStringEqualFixedStringResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "19e827d2-eece-47d6-a28d-efd3ad2938b4");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "ML";
  EXPECT_THAT(testSTDString == testFixedString, Eq(false));

  /* @Start: UT_CORE_0345 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of FixedString as testSTDString and pass parameter "ML". */
  /* @TE3: Instance testSTDString should be created. */
  /* @TS4: Invoke == operator for testSTDString and testFixedString. */
  /* @TE4: operator == for testSTDString and testFixedString should return false. */
  /* @End: UT_CORE_0345 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0346
@Test_Description:    To test the functionality of operator== of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorFixedStringEqualSTDStringResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "c7a12cc3-6062-4f96-b34a-be1cd8976824");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "M";
  EXPECT_THAT(testFixedString == testSTDString, Eq(true));

  /* @Start: UT_CORE_0346 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of string as testSTDString and pass parameter "M". */
  /* @TE3: Instance testSTDString should be created. */
  /* @TS4: Invoke == operator for testSTDString and testFixedString. */
  /* @TE4: operator == for testSTDString and testFixedString should return true. */
  /* @End: UT_CORE_0346 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0347
@Test_Description:    To test the functionality of operator== of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorFixedStringEqualSTDStringWithSameSizeResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "558d111c-d5ba-497c-bb90-9ffee2d155d8");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "L";
  EXPECT_THAT(testFixedString == testSTDString, Eq(false));

  /* @Start: UT_CORE_0347 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of string as testSTDString and pass parameter "L". */
  /* @TE3: Instance testSTDString should be created. */
  /* @TS4: Invoke == operator for testSTDString and testFixedString. */
  /* @TE4: operator == for testSTDString and testFixedString should return false. */
  /* @End: UT_CORE_0347 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0348
@Test_Description:    To test the functionality of String equality of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611415
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorFixedStringEqualSTDStringResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "cd651629-e6e7-4f7f-82da-aab5517aebec");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "ML";
  EXPECT_THAT(testFixedString == testSTDString, Eq(false));

  /* @Start: UT_CORE_0348 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created passed parameter "M". */
  /* @TS3: Create an instance of string as testSTDString and pass parameter "ML". */
  /* @TE3: Instance testSTDString should be created passed parameter "ML". */
  /* @TS4: Invoke == operator for testSTDString and testFixedString. */
  /* @TE4: operator == for testSTDString and testFixedString should return false. */
  /* @End: UT_CORE_0348 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0349
@Test_Description:    To test the functionality of Operator!= of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611416, 611831
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorSTDStringNotEqualFixedStringResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "0c2523db-6848-49d9-99c9-43bb25edbe5d");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "Ferdinand SpitzschnÃ¼ffler";
  EXPECT_THAT(testSTDString != testFixedString, Eq(true));

  /* @Start: UT_CORE_0349 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of FixedString as testSTDString and pass parameter "Ferdinand Spitzschnffler". */
  /* @TE3: Instance testSTDString should be created.*/
  /* @TS4: Invoke != operator for testSTDString and testFixedString. */
  /* @TE4: operator != for testSTDString and testFixedString should return true. */
  /* @End: UT_CORE_0349 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0350
@Test_Description:    To test the functionality of operator!= of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611416, 611831
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorSTDStringNotEqualFixedStringWithSameSizeResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "9e1292c3-44ac-4889-9a2f-233233cb3fe5");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "L";
  EXPECT_THAT(testSTDString != testFixedString, Eq(true));

  /* @Start: UT_CORE_0350 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of string as testSTDString and pass parameter "L". */
  /* @TE3: Instance testSTDString should be created.*/
  /* @TS4: Invoke != operator for testSTDString and testFixedString. */
  /* @TE4: operator != for testSTDString and testFixedString should return true. */
  /* @End: UT_CORE_0350 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0351
@Test_Description:    To test the functionality of operator!= of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611416, 611831
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorSTDStringNotEqualFixedStringResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "34c13d2b-9e1b-4293-9d30-c5a5969c58e8");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "M";
  EXPECT_THAT(testSTDString != testFixedString, Eq(false));

  /* @Start: UT_CORE_0351 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString with capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of std::string as testSTDString and pass parameter "M". */
  /* @TE3: Instance testSTDString should be created. */
  /* @TS4: Invoke operator!= for testSTDString and testFixedString. */
  /* @TE4: operator!= for testSTDString and testFixedString should return false. */
  /* @End: UT_CORE_0351 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0352
@Test_Description:    To test the functionality of operator!= of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611416, 611831
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorFixedStringNotEqualSTDStringResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "d3ceaf9a-6f77-4688-be70-9a56a7b2cb78");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "Mslimdchen";
  EXPECT_THAT(testFixedString != testSTDString, Eq(true));

  /* @Start: UT_CORE_0352 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString with capacity of MyString and pass the parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of std::string as testSTDString and pass parameter "Mslimdchen". */
  /* @TE3: Instance testSTDString should be created.*/
  /* @TS4: Invoke != operator for testFixedString and testSTDString. */
  /* @TE4: operator != for testFixedString and testSTDString should return true. */
  /* @End: UT_CORE_0352 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0353
@Test_Description:    To test the functionality of operator!= of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611416, 611831
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorFixedStringNotEqualSTDStringWithSameSizeResultTrue)
{
  ::testing::Test::RecordProperty("TEST_ID", "99a40704-9d0b-49f9-bd5c-4c87b9f2ba01");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "L";
  EXPECT_THAT(testFixedString != testSTDString, Eq(true));

  /* @Start: UT_CORE_0353 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString with capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created. */
  /* @TS3: Create an instance of string as testSTDString and pass parameter "L". */
  /* @TE3: Instance testSTDString should be created passed parameter "L". */
  /* @TS4: Invoke != operator for testFixedString and testSTDString. */
  /* @TE4: operator != for testFixedString and testSTDString should return true. */
  /* @End: UT_CORE_0353 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0354
@Test_Description:    To test the functionality of operator!= of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611416, 611831
*******************************************************************************/
TYPED_TEST(stringTyped_test, CompareOperatorFixedStringNotEqualSTDStringResultFalse)
{
  ::testing::Test::RecordProperty("TEST_ID", "ad5b2551-2a6c-4da4-a421-ee02e3b39f88");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP> testFixedString("M");
  std::string testSTDString = "M";
  EXPECT_THAT(testFixedString != testSTDString, Eq(false));

  /* @Start: UT_CORE_0354 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testFixedString with same capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testFixedString should be created passed parameter "M". */
  /* @TS3: Create an instance of std::string as testSTDString and pass parameter "M". */
  /* @TE3: Instance testSTDString should be created passed parameter "M". */
  /* @TS4: Invoke != operator for testFixedString and testSTDString. */
  /* @TE4: operator != for testFixedString and testSTDString should return false. */
  /* @End: UT_CORE_0354 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0355
@Test_Description:    To test the functionality of Empty String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611836
*******************************************************************************/
TYPED_TEST(stringTyped_test, EmptyStreamInputWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "88c68194-9a9c-4f2f-a0e0-90bd72f9b102");
  std::ostringstream testStream;
  testStream << "";
  EXPECT_THAT(testStream.str(), StrEq(""));

  /* @Start: UT_CORE_0355 */
  /* @TS1: Create an instance of std::ostringstream as testStream and invoke the << operator. */
  /* @TE1: Instance testStream should be created with empty parameter. */
  /* @TS2: Invoke the str of testStream.*/
  /* @TE2: str should be equal to null. */
  /* @End: UT_CORE_0355 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0356
@Test_Description:    To test the functionality of Size capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611836
*******************************************************************************/
TYPED_TEST(stringTyped_test, StreamInputOfSizeCapacityWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "c9b3dff3-018d-4189-818f-3534767e7ee4");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  FixedString<STRINGCAP> testFixedString(ara::core::TruncateToCapacity, testString);
  std::ostringstream testStream;
  testStream << testFixedString;
  EXPECT_THAT(testStream.str(), Eq(testFixedString.c_str()));

  /* @Start: UT_CORE_0356 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return capacity of MyString. */
  /* @TS2: Create an instance of string as testString and insert capaity of MyString, pass parameter "M". */
  /* @TE2: Instance testString should be created with the same capaity of MyString. */
  /* @TS3: Create an instance of FixedString as testFixedString with capacity of MyString and pass the parameter as TruncateToCapacity, testString in it. */
  /* @TE3: Instance testFixedString should be created. */
  /* @TS4: Create an instance of ostringstream as testStream. */
  /* @TE4: Instance testStream should be created. */
  /* @TS5: Invoke operator<< for testStream and testFixedString. */
  /* @TE5: operator<< should be invoked. */
  /* @TS6: Invoke str of testStream. */
  /* @TE6: str of testStream should be equal to testFixedString.c_str. */
  /* @End: UT_CORE_0356 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0357
@Test_Description:    To test the functionality of String that IsEmpty of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611424
*******************************************************************************/
TYPED_TEST(stringTyped_test, NewlyCreatedStringIsEmpty)
{
  ::testing::Test::RecordProperty("TEST_ID", "b76d10c1-46e2-4bc3-ab79-af187b85d584");
  using MyString = typename TestFixture::stringType;
  MyString sut;
  EXPECT_THAT(sut.empty(), Eq(true));

  /* @Start: UT_CORE_0357 */
  /* @TS1: Create an instance of Mystring as sut. */
  /* @TE1: Instance of Mystring should be created. */
  /* @TS2: Invoke empty() of sut. */
  /* @TE2: Empty() of sut should return true. */
  /* @End: UT_CORE_0357 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0358
@Test_Description:    To test the functionality of String which content is not empty of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611424
*******************************************************************************/
TYPED_TEST(stringTyped_test, StringWithContentIsNotEmtpy)
{
  ::testing::Test::RecordProperty("TEST_ID", "bb7fd1ff-82dc-4ae2-af70-9b080eb2265d");
  using MyString = typename TestFixture::stringType;
  MyString sut(ara::core::TruncateToCapacity, "Dr.SchluepferStrikesAgain!");
  EXPECT_THAT(sut.empty(), Eq(false));

  /* @Start: UT_CORE_0358 */
  /* @TS1: Create an instance of FixedString as sut and pass the parameters as TruncateToCapacity and "Dr.SchluepferStrikesAgain!". */
  /* @TE1: Instance suu should be created. */
  /* @TS2: Invoke the empty () of sut.*/
  /* @TE2: Empty () of sut should return false. */
  /* @End: UT_CORE_0358 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0359
@Test_Description:    To test the functionality of Empty String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, ConstrWithEmptyStringWithSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "94bb789d-a98e-4eaa-8255-029a6d837d7c");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> testString(this->testSubject);
  EXPECT_THAT(testString.c_str(), StrEq(""));
  EXPECT_THAT(testString.size(), Eq(0U));
  EXPECT_THAT(testString.capacity(), Eq(STRINGCAP + 1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.capacity(), Eq(STRINGCAP));

  /* @Start: UT_CORE_0359 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString by adding 1 to the capacity of MyString and insert testSubject in it. */
  /* @TE2: Instance testString should be created by increasing 1 to the capacity of MyString. */
  /* @TS3: Invoke the c_str() of testString. */
  /* @TE3: c_str() of testString should be equal to 0. */
  /* @TS4: Invoke the size() of testString. */
  /* @TE4: size() of testString should be equal to 1. */
  /* @TS5: Invoke the capacity of testString. */
  /* @TE5: capacity() of testString should be equal to STRINGCAP by increasing 1. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() of testSubject should be equal to null. */
  /* @TS7: Invoke the size() of testSubject. */
  /* @TE7: size() of testSubject should be equal to 0. */
  /* @TS8: Invoke the capacity of testSubject.*/
  /* @TE8: capacity() of testSubject should be equal to STRINGCAP. */
  /* @End: UT_CORE_0359 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0360
@Test_Description:    To test the functionality of String with smaller capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, ConstrWithStringWithSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "cebe491f-abaa-4473-8417-6c3264620f61");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  this->testSubject = "M";
  FixedString<STRINGCAP + 1U> testString(this->testSubject);
  EXPECT_THAT(testString.c_str(), StrEq("M"));
  EXPECT_THAT(testString.size(), Eq(1U));
  EXPECT_THAT(testString.capacity(), Eq(STRINGCAP + 1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq("M"));
  EXPECT_THAT(this->testSubject.size(), Eq(1U));
  EXPECT_THAT(this->testSubject.capacity(), Eq(STRINGCAP));

  /* @Start: UT_CORE_0360 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString by adding 1 to the capacity of MyString and insert testSubject with parameter "M" in it. */
  /* @TE2: Instance testString should be created by increasing 1 to the capacity of MyString. */
  /* @TS3: Invoke the c_str() of testString. */
  /* @TE3: c_str() of testString should be equal to M. */
  /* @TS4: Invoke the size() of testString. */
  /* @TE4: size() of testString should be equal to 1. */
  /* @TS5: Invoke the capacity() of testString. */
  /* @TE5: capacity() of testString should be equal to STRINGCAP by increasing 1. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() of testSubject should be equal to M. */
  /* @TS7: Invoke the size() of testSubject. */
  /* @TE7: size() of testSubject should be equal to 1. */
  /* @TS8: Invoke the capacity() of testSubject.*/
  /* @TE8: capacity() of testSubject should be equal to STRINGCAP. */
  /* @End: UT_CORE_0360 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0361
@Test_Description:    To test the functionality of Move constructor with Empty String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, MoveConstrWithEmptyStringWithSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "76527963-8483-4689-816e-01e6803e078a");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 30U> testString(std::move(this->testSubject));
  EXPECT_THAT(testString.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(testString.c_str(), StrEq(""));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0361 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString by adding 30 to the capacity of MyString and move testSubject to it */
  /* @TE2: Instance testString should be created by increasing 30 to the capacity of MyString. */
  /* @TS3: Invoke the size() of testString. */
  /* @TE3: size() of testString should be equal to 0. */
  /* @TS4: Invoke the size() of testSubject. */
  /* @TE4: size() of testsubject should be equal to 0. */
  /* @TS5: Invoke the c_str() of testString. */
  /* @TE5: c_str() of testString should be equal to null. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() of testsubject should be equal to null. */
  /* @End: UT_CORE_0361 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0362
@Test_Description:    To test the functionality of Move construct with string of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, MoveConstrWithStringSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "4bc0b719-080b-4345-9875-a74d0b5cf77d");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  std::string testString(STRINGCAP, 'M');
  EXPECT_THAT(this->testSubject.unsafe_assign(testString), Eq(true));
  FixedString<STRINGCAP + 11U> fuu(std::move(this->testSubject));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(fuu.size(), Eq(STRINGCAP));
  EXPECT_THAT(fuu.c_str(), Eq(testString));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));

  /* @Start: UT_CORE_0362 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of string as testString and pass a capacity of string with parameter "M". */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Create an instance of FixedString as fuu by adding 11 to the capacity of MyString and move testSubject to it */
  /* @TE3: Instance fuu should be created by increasing 11 to the capacity of MyString. */
  /* @TS4: Invoke the unsafe_assign() of testsubject. */
  /* @TE4: unsafe_assign() of testsubject should return true. */
  /* @TS5: Invoke the size() of testString. */
  /* @TE5: size() of testString should be equal to 0. */
  /* @TS6: Invoke the size() of fuu. */
  /* @TE6: size() of fuu should be equal to STRINGCAP. */
  /* @TS7: Invoke the c_str() of fuu. */
  /* @TE7: c_str() of fuu should be equal to testString. */
  /* @TS8: Invoke the c_str() of testSubject. */
  /* @TE8: c_str() of testsubject should be equal to null. */
  /* @End: UT_CORE_0362 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0363
@Test_Description:    To test the functionality of Assignment of String of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignmentOfStringWithSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "983c0fd2-6224-4712-9704-01367f1cd2c6");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> testString;
  testString = this->testSubject;
  EXPECT_THAT(testString.c_str(), StrEq(""));
  EXPECT_THAT(testString.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));

  /* @Start: UT_CORE_0363 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString by adding 1 to the capacity to the MyString . */
  /* @TE2: Instance testString should be created by increasing 1 to the capacity of MyString. */
  /* @TS3: Assign testSubject to testString. */
  /* @TE3: testSubject to testString should be assigned. */
  /* @TS4: Invoke the c_str() of testString. */
  /* @TE4: c_str() of testString should be equal to null. */
  /* @TS5: Invoke the size() of testString. */
  /* @TE5: size() of testString should be equal to 0. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() of testSubject should be equal to null. */
  /* @TS7: Invoke the size() of testSubject. */
  /* @TE7: size() of testsubject should be equal to 0. */
  /* @End: UT_CORE_0363 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0364
@Test_Description:    To test the functionality of Assignment of empty string FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignmentOfEmptyStringWithSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "c2361b47-cc22-458e-b9bc-e295324c3216");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 1U> testString("M");
  testString = this->testSubject;
  EXPECT_THAT(testString.c_str(), StrEq(""));
  EXPECT_THAT(testString.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));
  EXPECT_THAT(this->testSubject.size(), Eq(0U));

  /* @Start: UT_CORE_0364 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString by adding 1 to the capacity of MyString and pass parameter "M". */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Assign testSubject to testString. */
  /* @TE3: testSubject to testString should be assigned. */
  /* @TS4: Invoke the c_str() of testString. */
  /* @TE4: c_str() of testString should be equal to null. */
  /* @TS5: Invoke the size() of testString. */
  /* @TE5: size() of testString should be equal to 0. */
  /* @TS6: Invoke the c_str() of testSubject. */
  /* @TE6: c_str() of testSubject. should be equal to null. */
  /* @TS7: Invoke the size() of testSubject. */
  /* @TE7: size() of testsubject should be equal to 0. */
  /* @End: UT_CORE_0364 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0365
@Test_Description:    To test the functionality of Assignment of Non-empty string FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, AssignmentOfNotEmptyStringWithSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "66eaa759-fe92-4b94-9c1c-371f2c4fc2bf");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  this->testSubject = "M";
  FixedString<STRINGCAP + 30U> testString("Ferdinand Spitzschnueffler");
  testString = this->testSubject;
  EXPECT_THAT(testString.c_str(), StrEq("M"));
  EXPECT_THAT(testString.size(), Eq(1U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq("M"));
  EXPECT_THAT(this->testSubject.size(), Eq(1U));

  /* @Start: UT_CORE_0365 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString by adding 30 to the capacity of MyString and move testSubject to it */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Assign testSubject into the testString. */
  /* @TE3: testSubject to testString should be assigned. */
  /* @TS4: Invoke the c_str() of testString. */
  /* @TE4: c_str() of testString should be equal to M. */
  /* @TS5: Invoke the size() of testString. */
  /* @TE5: size() of testString should be equal to 1. */
  /* @TS6: Invoke the c_str() of testsubject. */
  /* @TE6: c_str() of testsubject should be equal to M. */
  /* @TS7: Invoke the size() of testSubject. */
  /* @TE7: size() of testsubject should be equal to 1. */
  /* @End: UT_CORE_0365 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0366
@Test_Description:    To test the functionality of Empty String with smaller capacity of FixedString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          stringTyped_test
@Requirement Tracing: -
@Design Tracing:      611439, 611422, 611423
*******************************************************************************/
TYPED_TEST(stringTyped_test, MoveAssignmentOfEmptyStringWithSmallerCapaWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "0703614e-88a6-4f5f-80c7-f0d7e51736f6");
  using MyString = typename TestFixture::stringType;
  constexpr auto STRINGCAP = MyString::capacity();
  FixedString<STRINGCAP + 63U> fuu;
  fuu = std::move(this->testSubject);
  EXPECT_THAT(this->testSubject.size(), Eq(0U));
  EXPECT_THAT(fuu.size(), Eq(0U));
  EXPECT_THAT(this->testSubject.c_str(), StrEq(""));
  EXPECT_THAT(fuu.c_str(), StrEq(""));

  /* @Start: UT_CORE_0366 */
  /* @TS1: Create an instance of FixedString as MyString and invoke the capacity(). */
  /* @TE1: capacity() should return the capacity of MyString. */
  /* @TS2: Create an instance of FixedString as testString by adding 63 to the capacity of MyString. */
  /* @TE2: Instance testString should be created with the same capacity of MyString. */
  /* @TS3: Move the testSubject to fuu. */
  /* @TE3: testSubject to testString should be assigned. */
  /* @TS4: Invoke the size() of testString. */
  /* @TE4: size() of testString should be equal to 0. */
  /* @TS5: Invoke the size() of fuu. */
  /* @TE5: size() of fuu should be equal to 0. */
  /* @TS6: Invoke the c_str() of testsubject. */
  /* @TE6: c_str() of testsubject should be equal to null. */
  /* @TS7: Invoke the c_str() of fuu. */
  /* @TE7: c_str() of fuu should be equal to null. */
  /* @End: UT_CORE_0366 */
}

} // namespace
