// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_fixed_vector.cpp
//
// Purpose     : This file provides the unit test cases of class FixedVector.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/fixed_vector.h"
#include "ara/core/instance_specifier.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Ne;
namespace
{
using namespace ::testing;
//using namespace iox::cxx;

class test
{
 public:
  test(ara::core::InstanceSpecifier const& data) :
      obj(iox::cxx::TruncateToCapacity, data.ToString().data(), data.ToString().size())
  {
  }

  iox::cxx::string<10> obj;
};
class vector_test : public Test
{
 public:
  static int cTor;
  static int customCTor;
  static int copyCTor;
  static int moveCTor;
  static int moveAssignment;
  static int copyAssignment;
  static int dTor;
  static int classValue;

  static std::vector<int> dtorOrder;

  class CTorTest
  {
   public:
    CTorTest()
    {
      cTor++;
      classValue = value;
    }

    CTorTest(const int value) : value(value)
    {
      customCTor++;
      classValue = value;
    }

    CTorTest(const CTorTest& rhs)
    {
      copyCTor++;
      value = rhs.value;
      classValue = value;
    }

    CTorTest(CTorTest&& rhs)
    {
      moveCTor++;
      value = rhs.value;
      classValue = value;
    }

    CTorTest& operator=(const CTorTest& rhs)
    {
      copyAssignment++;
      value = rhs.value;
      classValue = value;
      return *this;
    }

    CTorTest& operator=(CTorTest&& rhs)
    {
      moveAssignment++;
      value = rhs.value;
      classValue = value;
      return *this;
    }

    ~CTorTest()
    {
      dTor++;
      classValue = value;
      dtorOrder.emplace_back(value);
    }

    int value = 0;
  };

  void SetUp()
  {
    cTor = 0;
    customCTor = 0;
    copyCTor = 0;
    moveCTor = 0;
    moveAssignment = 0;
    copyAssignment = 0;
    dTor = 0;
    classValue = 0;
    dtorOrder.clear();
  }

  ara::core::FixedVector<int, 10> sut;
};

int vector_test::cTor;
int vector_test::customCTor;
int vector_test::copyCTor;
int vector_test::moveCTor;
int vector_test::moveAssignment;
int vector_test::copyAssignment;
int vector_test::dTor;
int vector_test::classValue;
std::vector<int> vector_test::dtorOrder;

/*******************************************************************************
@TestCaseId:          UT_CORE_0028
@Test_Description:    To test the functionality of empty() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611279
*******************************************************************************/
TEST_F(vector_test, NewlyCreatedVectorIsEmpty)
{
  ::testing::Test::RecordProperty("TEST_ID", "8ebb8b11-d044-459e-b9a1-4a3076c8d49c");
  EXPECT_THAT(sut.empty(), Eq(true));
  /* @Start: UT_CORE_0028 */
  /* @TS1: Call the empty() of FixedVector. */
  /* @TE1: empty() should return true. */
  /* @End: UT_CORE_0028 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0029
@Test_Description:    To test the functionality of size() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611278
*******************************************************************************/
TEST_F(vector_test, NewlyCreatedVectorHasSizeZero)
{
  ::testing::Test::RecordProperty("TEST_ID", "f850b288-df04-43b8-b317-bec76c6c4924");
  EXPECT_THAT(sut.size(), Eq(0));
  /* @Start: UT_CORE_0029 */
  /* @TS1: Call the size() of FixedVector. */
  /* @TE1: size() should return true. */
  /* @End: UT_CORE_0029 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0030
@Test_Description:    To test the functionality of capacity() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611277
*******************************************************************************/
TEST_F(vector_test, Capacity)
{
  ::testing::Test::RecordProperty("TEST_ID", "e0a244d5-6e01-4cbb-9f9a-ac07cad81a5c");
  EXPECT_THAT(sut.capacity(), Eq(10));
  /* @Start: UT_CORE_0030 */
  /* @TS1: Call the capacity() of FixedVector. */
  /* @TE1: capacity() should return true. */
  /* @End: UT_CORE_0030 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0031
@Test_Description:    To test the functionality of empty() of random capacity FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611279
*******************************************************************************/
TEST_F(vector_test, NewVectorWithElementsCTorWithZeroElements)
{
  ::testing::Test::RecordProperty("TEST_ID", "618cd7f7-42d7-49e0-a504-3894e34a28f8");
  constexpr uint64_t CAPACITY{42};
  constexpr int DEFAULT_VALUE{13};
  ara::core::FixedVector<int, CAPACITY> sut(0, DEFAULT_VALUE);
  EXPECT_THAT(sut.empty(), Eq(true));
  /* @Start: UT_CORE_0031 */
  /* @TS1: Call the empty() of FixedVector. */
  /* @TE1: empty() should return true. */
  /* @End: UT_CORE_0031 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0032
@Test_Description:    To test the functionality of size() of random capacity FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611278
*******************************************************************************/
TEST_F(vector_test, NewVectorWithElementsCTorWithSomeElements)
{
  ::testing::Test::RecordProperty("TEST_ID", "65fd89f4-167e-4844-8264-484d2a57c035");
  constexpr uint64_t CAPACITY{42};
  constexpr uint64_t ELEMENT_COUNT{37};
  constexpr int DEFAULT_VALUE{13};
  ara::core::FixedVector<int, CAPACITY> sut(ELEMENT_COUNT, DEFAULT_VALUE);
  EXPECT_THAT(sut.size(), Eq(ELEMENT_COUNT));
  for (const auto& item : sut)
  {
    EXPECT_THAT(item, Eq(DEFAULT_VALUE));
  }
  /* @Start: UT_CORE_0032 */
  /* @TS1: call the size() of FixedVector. */
  /* @TE1: size() should return 37. */
  /* @End: UT_CORE_0032 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0033
@Test_Description:    To test the functionality of size() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611278
*******************************************************************************/
TEST_F(vector_test, NewVectorWithElementsCTorWithCapacityElements)
{
  ::testing::Test::RecordProperty("TEST_ID", "9987c1cb-2266-4bad-b91d-0b171dd87f40");
  constexpr uint64_t CAPACITY{42};
  constexpr int DEFAULT_VALUE{13};
  ara::core::FixedVector<int, CAPACITY> sut(CAPACITY, DEFAULT_VALUE);
  EXPECT_THAT(sut.size(), Eq(CAPACITY));
  for (const auto& item : sut)
  {
    EXPECT_THAT(item, Eq(DEFAULT_VALUE));
  }
  /* @Start: UT_CORE_0033 */
  /* @TS1: call the size(). */
  /* @TE1: size() should return 42. */
  /* @End: UT_CORE_0033 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0034
@Test_Description:    To test the functionality of size() of random capacity FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611278
*******************************************************************************/
TEST_F(vector_test, NewVectorWithElementsCTorWithMoreThanCapacityElements)
{
  ::testing::Test::RecordProperty("TEST_ID", "a22a3329-d4c1-4ecf-a94f-69a990a35658");
  constexpr uint64_t CAPACITY{42};
  constexpr uint64_t ELEMENT_COUNT{73};
  constexpr int DEFAULT_VALUE{13};
  ara::core::FixedVector<int, CAPACITY> sut(ELEMENT_COUNT, DEFAULT_VALUE);
  EXPECT_THAT(sut.size(), Eq(CAPACITY));
  for (const auto& item : sut)
  {
    EXPECT_THAT(item, Eq(DEFAULT_VALUE));
  }
  /* @Start: UT_CORE_0034 */
  /* @TS1: Create the FixedVector instance with capacity of 42 and element count of 13. */
  /* @TE1: size() should return 42. */
  /* @TS2: call the size() . */
  /* @TE2: All the vector elements should be have default data as 13. */
  /* @End: UT_CORE_0034 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0035
@Test_Description:    To test the functionality of emplace_back() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283
*******************************************************************************/
TEST_F(vector_test, EmplaceBackSuccessfullWhenSpaceAvailable)
{
  ::testing::Test::RecordProperty("TEST_ID", "98d17e04-0d2b-4575-a1f0-7b3cd918c54d");
  EXPECT_THAT(sut.emplace_back(5), Eq(true));
  /* @Start: UT_CORE_0035 */
  /* @TS1: call the emplace_back() with input argument as 5. */
  /* @TE1: emplace_back() should return true. */
  /* @End: UT_CORE_0035 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0036
@Test_Description:    To test the functionality of emplace_back() of random capacity FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283
*******************************************************************************/
TEST_F(vector_test, EmplaceBackFailsWhenSpaceNotAvailable)
{
  ::testing::Test::RecordProperty("TEST_ID", "199e6fc7-5bc4-4896-b211-e04fc668ccd0");
  for (uint64_t i = 0; i < 10; ++i)
  {
    EXPECT_THAT(sut.emplace_back(5), Eq(true));
  }
  EXPECT_THAT(sut.emplace_back(5), Eq(false));
  /* @Start: UT_CORE_0036 */
  /* @TS1: call the emplace_back() 10 times with input argument as 5. */
  /* @TE1: emplace_back() should return true for all 10 calls. */
  /* @TS2: call the emplace_back() as 5. */
  /* @TE2: emplace_back() should return false for 11th call. */
  /* @End: UT_CORE_0036 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0037
@Test_Description:    To test the functionality of push_back() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611284, 611278, 611269
*******************************************************************************/
TEST_F(vector_test, PushBackSuccessfullWhenSpaceAvailableLValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "42102325-91fa-45aa-a5cb-2bce785d11c1");
  const int a{5};
  EXPECT_THAT(sut.push_back(a), Eq(true));
  ASSERT_THAT(sut.size(), Eq(1u));
  EXPECT_THAT(sut.at(0), Eq(a));
  /* @Start: UT_CORE_0037 */
  /* @TS1: call the push_back() of FixedVector. */
  /* @TE1: push_back() should return true. */
  /* @TS2: call the size() of FixedVector. */
  /* @TE2: size() should return 1. */
  /* @End: UT_CORE_0037 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0038
@Test_Description:    To test the functionality of push_back() of random capacity FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611284
*******************************************************************************/
TEST_F(vector_test, PushBackFailsWhenSpaceNotAvailableLValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "e47a9f1b-a039-4740-a4b5-ba81886c1330");
  const int a{5};
  for (uint64_t i = 0; i < 10; ++i)
  {
    EXPECT_THAT(sut.push_back(a), Eq(true));
  }
  EXPECT_THAT(sut.push_back(a), Eq(false));
  /* @Start: UT_CORE_0038 */
  /* @TS1: call the push_back() 10 times with an input argument as 5. */
  /* @TE1: push_back() should return true for all 10 calls. */
  /* @TS2: call the push_back(). */
  /* @TE2: push_back() should return false for 11th call. */
  /* @End: UT_CORE_0038 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0039
@Test_Description:    To test the functionality of push_back(),size(),at() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611284, 611278, 611269, 611270
*******************************************************************************/
TEST_F(vector_test, PushBackSuccessfullWhenSpaceAvailableRValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "47988e05-9c67-4b34-bdee-994552df3fa7");
  EXPECT_THAT(sut.push_back(5), Eq(true));
  ASSERT_THAT(sut.size(), Eq(1u));
  EXPECT_THAT(sut.at(0), Eq(5));
  /* @Start: UT_CORE_0039 */
  /* @TS1: call the push_back() of FixedVector. */
  /* @TE1: push_back() should return true. */
  /* @TS2: call the size() of FixedVector . */
  /* @TE2: size() should return true. */
  /* @TS3: call the at() of FixedVector. */
  /* @TE3: at() should return true. */
  /* @End: UT_CORE_0039 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0040
@Test_Description:    To test the functionality of push_back() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611284, 611285
*******************************************************************************/
TEST_F(vector_test, PushBackFailsWhenSpaceNotAvailableRValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "07790d09-110d-4cc7-84d9-3ff28e494c8f");
  for (uint64_t i = 0; i < 10; ++i)
  {
    EXPECT_THAT(sut.push_back(5), Eq(true));
  }
  EXPECT_THAT(sut.push_back(5), Eq(false));
  /* @Start: UT_CORE_0040 */
  /* @TS1: call the push_back 10 times with an input argument as 5. */
  /* @TE1: push_back() should return true for all the 10 calls. */
  /* @TS2: call the push_back() . */
  /* @TE2: push_back() should return false. */
  /* @End: UT_CORE_0040 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0041
@Test_Description:    To test the functionality of pop_back(),size() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611286, 611278
*******************************************************************************/
TEST_F(vector_test, PopBackOnEmptyVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "03ac7f78-680d-49b1-b7e1-7551006b1545");
  sut.pop_back();
  ASSERT_THAT(sut.size(), Eq(0u));
  /* @Start: UT_CORE_0041 */
  /* @TS1: call the pop_back() and check the size(). */
  /* @TE1: size() should return 0. */
  /* @End: UT_CORE_0041 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0042
@Test_Description:    To test the functionality of emplace_back() and size() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611278, 611286
*******************************************************************************/
TEST_F(vector_test, PopBackNonEmptyVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "ef4ecaa7-d467-4c8a-9ab4-4444b87727d0");
  ara::core::FixedVector<CTorTest, 10> sut;
  sut.emplace_back(101);
  ASSERT_THAT(sut.size(), Eq(1u));
  dTor = 0;
  sut.pop_back();
  ASSERT_THAT(sut.size(), Eq(0u));
  /* @Start: UT_CORE_0042 */
  /* @TS1: call the emplace_back() with input value as 101 and invoke the size() and pop_back() . */
  /* @TE1: size() should be equal to 1 and after pop_back() size should be 0. */
  /* @End: UT_CORE_0042 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0043
@Test_Description:    To test the functionality of emplace_back() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611278
*******************************************************************************/
TEST_F(vector_test, SizeIncreasesWhenElementIsAdded)
{
  ::testing::Test::RecordProperty("TEST_ID", "2f1814ce-dfc8-4dbe-a7c7-ab004e28a7a2");
  sut.emplace_back(5);
  EXPECT_THAT(sut.size(), Eq(1));
  /* @Start: UT_CORE_0043 */
  /* @TS1: call the emplace_back() with input value as 5 and invoke the size(). */
  /* @TE1: size() should return 1. */
  /* @End: UT_CORE_0043 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0044
@Test_Description:    To test the functionality of emplace_back(),size() and capacity() FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611278, 611277
*******************************************************************************/
TEST_F(vector_test, SizeEqualsCapacityWheFull)
{
  ::testing::Test::RecordProperty("TEST_ID", "733985c2-ef1d-4772-9c01-4e26e841581d");
  for (uint64_t i = 0; i < 10; ++i)
  {
    sut.emplace_back(5);
  }
  EXPECT_THAT(sut.size(), Eq(sut.capacity()));
  /* @Start: UT_CORE_0044 */
  /* @TS1: call the emplace_back() 10 times with input value as 5 and check the size and capacity of FixedVector. */
  /* @TE1: size() and capacity() should be same. */
  /* @End: UT_CORE_0044 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0045
@Test_Description:    To test the functionality of of emplace_back(),size() and capacity() FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611278, 611277
*******************************************************************************/
TEST_F(vector_test, SizeUnchangedWhenEmplaceFails)
{
  ::testing::Test::RecordProperty("TEST_ID", "6ae42f49-ef4d-4b9c-9360-a6e63a8b9357");
  for (uint64_t i = 0; i < 10; ++i)
  {
    sut.emplace_back(5);
  }
  EXPECT_THAT(sut.emplace_back(5), Eq(false));
  EXPECT_THAT(sut.size(), Eq(sut.capacity()));
  /* @Start: UT_CORE_0045 */
  /* @TS1: call the emplace_back() 10 times with input value as 5. */
  /* @TE1: emplace_back should return true for first 10 elements. */
  /* @TS2: call the emplace_back() of FixedVector. */
  /* @TE2: emplace_back() should return false. */
  /* @TS3: call the size() and capacity() of FixedVector. */
  /* @TE3: size() and capacity() should be same. */
  /* @End: UT_CORE_0045 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0046
@Test_Description:    To test the functionality of emplace_back() and empty() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611278
*******************************************************************************/
TEST_F(vector_test, NotEmptyWhenElementWasAdded)
{
  ::testing::Test::RecordProperty("TEST_ID", "651703b8-0828-471e-9333-547dc1b00295");
  sut.emplace_back(5);
  EXPECT_THAT(sut.empty(), Eq(false));
  /* @Start: UT_CORE_0046 */
  /* @TS1: call the emplace_back() with input value as 5 and invoke the empty(). */
  /* @TE1: empty() should return false. */
  /* @End: UT_CORE_0046 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0047
@Test_Description:    To test the functionality of emplace_back(),clear() and empty() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611280, 611279
*******************************************************************************/
TEST_F(vector_test, EmptyAfterClear)
{
  ::testing::Test::RecordProperty("TEST_ID", "f08cd8f7-2eca-4ef2-b2c3-d642529ecd60");
  sut.emplace_back(5);
  sut.clear();
  EXPECT_THAT(sut.empty(), Eq(true));
  /* @Start: UT_CORE_0047 */
  /* @TS1: call the emplace_back() with input value as 5 and clear(). */
  /* @TE1: empty() should return true. */
  /* @End: UT_CORE_0047 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0048
@Test_Description:    To test the functionality of emplace_back(),clear() and size() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611280, 611278
*******************************************************************************/
TEST_F(vector_test, SizeZeroAfterClear)
{
  ::testing::Test::RecordProperty("TEST_ID", "7fb46858-5e85-488c-8c10-40c44d412c61");
  sut.emplace_back(5);
  sut.clear();
  EXPECT_THAT(sut.size(), Eq(0));
  /* @Start: UT_CORE_0048 */
  /* @TS1: call the emplace_back() with input value as 5 and clear(). */
  /* @TE1: size() should return false. */
  /* @End: UT_CORE_0048 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0049
@Test_Description:    To test the functionality of CopyConstructor of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611280, 611279, 611279
*******************************************************************************/
TEST_F(vector_test, CopyConstructor)
{
  ::testing::Test::RecordProperty("TEST_ID", "afc46f10-e2fe-4c62-beb3-75c28d18d0f9");
  ara::core::FixedVector<CTorTest, 10> sut1;
  sut1.emplace_back(101);
  sut1.emplace_back(102);

  ara::core::FixedVector<CTorTest, 10> sut2(sut1);
  EXPECT_THAT(copyCTor, Eq(2));
  EXPECT_THAT(sut2.at(0).value, Eq(101));
  EXPECT_THAT(sut2.at(1).value, Eq(102));
  EXPECT_THAT(sut2.empty(), Eq(false));
  EXPECT_THAT(sut2.size(), Eq(2));
  /* @Start: UT_CORE_0049 */
  /* @TS1: call the emplace_back() with input value as 5 and call clear() of FixedVector. */
  /* @TE1: size() should return false. */
  /* @End: UT_CORE_0049 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0050
@Test_Description:    To test the functionality of size() and empty() of empty FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611279, 611278
*******************************************************************************/
TEST_F(vector_test, CopyConstructorWithEmptyVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "438c8835-8545-40e4-b544-d66107507e2f");
  ara::core::FixedVector<CTorTest, 10> sut1;
  ara::core::FixedVector<CTorTest, 10> sut2(sut1);
  EXPECT_THAT(copyCTor, Eq(0));
  EXPECT_THAT(sut2.size(), Eq(0));
  EXPECT_THAT(sut2.empty(), Eq(true));
  /* @Start: UT_CORE_0050 */
  /* @TS1: call the size() of FixedVector. */
  /* @TE1: size() should return 0. */
  /* @TS2: call the empty() of FixedVector. */
  /* @TE2: empty() should return true. */
  /* @End: UT_CORE_0050 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0051
@Test_Description:    To test the functionality of emplace_back(),size() and empty() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611279, 611278
*******************************************************************************/
TEST_F(vector_test, CopyConstructorWithFullVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "b7420f78-f3ca-4a85-8382-fffe49d94bc2");
  ara::core::FixedVector<CTorTest, 10> sut1;
  for (uint64_t i = 0; i < 10; ++i)
  {
    sut1.emplace_back(i);
  }

  ara::core::FixedVector<CTorTest, 10> sut2(sut1);
  for (uint64_t i = 0; i < 10; ++i)
  {
    // sut2.at(i) = i;
  }
  EXPECT_THAT(copyCTor, Eq(10));
  EXPECT_THAT(sut2.size(), Eq(10));
  EXPECT_THAT(sut2.empty(), Eq(false));
  /* @Start: UT_CORE_0051 */
  /* @TS1: call the emplace_back() of FixedVector. */
  /* @TE1: emplace_back() should return true. */
  /* @TS2: call the size() of FixedVector */
  /* @TE2: size() should return 10. */
  /* @TS3: call the empty() of FixedVector */
  /* @TE3: empty() should return false. */
  /* @End: UT_CORE_0051 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0052
@Test_Description:    To test the functionality of MoveConstructor of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611269, 611270, 611279, 611278
*******************************************************************************/
TEST_F(vector_test, MoveConstructor)
{
  ::testing::Test::RecordProperty("TEST_ID", "c96fdf10-822b-4872-b3d2-d3745a2ccb52");
  ara::core::FixedVector<CTorTest, 10> sut1;
  sut1.emplace_back(8101);
  sut1.emplace_back(8102);

  ara::core::FixedVector<CTorTest, 10> sut2(std::move(sut1));
  EXPECT_THAT(moveCTor, Eq(2));
  EXPECT_THAT(sut2.at(0).value, Eq(8101));
  EXPECT_THAT(sut2.at(1).value, Eq(8102));
  EXPECT_THAT(sut2.empty(), Eq(false));
  EXPECT_THAT(sut2.size(), Eq(2));

  /* @Start: UT_CORE_0052 */
  /* @TS1: call at(0) of FixedVector.*/
  /* @TE1: at(0) should return 8101. */
  /* @TS2: call at(1) of FixedVector.*/
  /* @TE2: at(1) should return 8102. */
  /* @TS3: call empty() of FixedVector.*/
  /* @TE3: empty() should return false. */
  /* @TS4: call size() of FixedVector.*/
  /* @TE4: size() should return 2. */
  /* @End: UT_CORE_0052 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0053
@Test_Description:    To test the functionality of size() and empty() of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611279, 611278
*******************************************************************************/
TEST_F(vector_test, MoveConstructorWithEmptyVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "1d89b1e1-f62f-49c4-bf04-9aba3dbbecab");
  ara::core::FixedVector<CTorTest, 10> sut1;
  ara::core::FixedVector<CTorTest, 10> sut2(sut1);
  EXPECT_THAT(moveCTor, Eq(0));
  EXPECT_THAT(sut2.size(), Eq(0));
  EXPECT_THAT(sut2.empty(), Eq(true));
  /* @Start: UT_CORE_0053 */
  /* @TS1: Call size() of FixedVector. */
  /* @TE1: size should return 0. */
  /* @TS2: Call empty() of FixedVector. */
  /* @TE2: size should return 0. */
  /* @End: UT_CORE_0053 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0054
@Test_Description:    To test the functionality of operator< of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611838 
*******************************************************************************/
TEST_F(vector_test, operatorlessthan)
{
  /* AVIN_IMPLIMENTED */

  /* @Start: UT_CORE_0054 */
  /* @TS1: Invoke the operator< of FixedVector.*/
  /* @TE1: operator< should return true if the second operand is greater than first operand.*/
  /* @End: UT_CORE_0054 */
  ara::core::FixedVector<int, 10> sut1;
  ara::core::FixedVector<int, 10> sut2;

  sut1.emplace_back(800);
  sut1.emplace_back(900);
  sut1.emplace_back(1000);

  sut2.emplace_back(800);
  sut2.emplace_back(1000);
  sut2.emplace_back(1000);

  EXPECT_THAT(sut1 < sut2, Eq(true));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(800);
  sut1.emplace_back(800);
  sut1.emplace_back(1000);

  sut2.emplace_back(700);
  sut2.emplace_back(800);
  sut2.emplace_back(1000);

  EXPECT_THAT(sut1 < sut2, Eq(false));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(1000);
  sut1.emplace_back(1000);
  sut1.emplace_back(1000);

  sut2.emplace_back(1000);
  sut2.emplace_back(1000);
  EXPECT_THAT(sut1 < sut2, Eq(false));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(1000);
  sut1.emplace_back(1000);

  sut2.emplace_back(1000);
  sut2.emplace_back(1000);
  sut2.emplace_back(1000);

  EXPECT_THAT(sut1 < sut2, Eq(true));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0055
@Test_Description:    To test the functionality of operator> of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611839, 611280
*******************************************************************************/
TEST_F(vector_test, operatorgreaterthan)
{
  ara::core::FixedVector<int, 10> sut1;
  ara::core::FixedVector<int, 10> sut2;

  sut1.emplace_back(800);
  sut1.emplace_back(900);
  sut1.emplace_back(1000);

  sut2.emplace_back(800);
  sut2.emplace_back(1000);
  sut2.emplace_back(1000);

  EXPECT_THAT(sut1 > sut2, Eq(false));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(800);
  sut1.emplace_back(800);
  sut1.emplace_back(1000);

  sut2.emplace_back(700);
  sut2.emplace_back(800);
  sut2.emplace_back(1000);

  EXPECT_THAT(sut1 > sut2, Eq(true));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(1000);
  sut1.emplace_back(1000);
  sut1.emplace_back(1000);

  sut2.emplace_back(1000);
  sut2.emplace_back(1000);
  EXPECT_THAT(sut1 > sut2, Eq(true));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(1000);
  sut1.emplace_back(1000);

  sut2.emplace_back(1000);
  sut2.emplace_back(1000);
  sut2.emplace_back(1000);

  EXPECT_THAT(sut1 > sut2, Eq(false));

  /* @Start: UT_CORE_0055 */
  /* @TS1: Invoke the operator> of FixedVector.*/
  /* @TE1: operator> should return true if the first operand is greater than second operand.*/
  /* @End: UT_CORE_0055 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0056
@Test_Description:    To test the functionality of operator<= of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611280, 611841
*******************************************************************************/
TEST_F(vector_test, operatorlessthanorequel)
{
  ara::core::FixedVector<int, 10> sut1;
  ara::core::FixedVector<int, 10> sut2;

  sut1.emplace_back(800);
  sut1.emplace_back(800);
  sut1.emplace_back(800);

  sut2.emplace_back(800);
  sut2.emplace_back(800);
  sut2.emplace_back(800);

  EXPECT_THAT(sut1 <= sut2, Eq(true));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(800);
  sut1.emplace_back(900);
  sut1.emplace_back(1000);

  sut2.emplace_back(800);
  sut2.emplace_back(1000);
  sut2.emplace_back(1000);

  EXPECT_THAT(sut1 <= sut2, Eq(true));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(800);
  sut1.emplace_back(800);
  sut1.emplace_back(1000);

  sut2.emplace_back(700);
  sut2.emplace_back(800);
  sut2.emplace_back(1000);

  EXPECT_THAT(sut1 <= sut2, Eq(false));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(800);
  sut1.emplace_back(800);
  sut1.emplace_back(1000);

  sut2.emplace_back(800);
  sut2.emplace_back(800);
  sut2.emplace_back(1000);

  EXPECT_THAT(sut1 <= sut2, Eq(true));

  /* @Start: UT_CORE_0056 */
  /* @TS1: Invoke the operator<= of FixedVector.*/
  /* @TE1: operator<= should return true if the second operand is greater or equal to first*/
  /* @End: UT_CORE_0056 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0057
@Test_Description:    To test the functionality of operator>= of FixedVector.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize vector_test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          vector_test
@Requirement Tracing: -
@Design Tracing:      611283, 611280, 611840
*******************************************************************************/
TEST_F(vector_test, operatorgreaterthanorequel)
{
  ara::core::FixedVector<int, 10> sut1;
  ara::core::FixedVector<int, 10> sut2;

  sut1.emplace_back(800);
  sut1.emplace_back(800);
  sut1.emplace_back(800);

  sut2.emplace_back(800);
  sut2.emplace_back(800);
  sut2.emplace_back(800);
  /* @Start: UT_CORE_0057 */
  /* @TS1: Invoke the operator>= of FixedVector. */
  /* @TE1: operator>= should return true if the first operand is greater or equal to second. */
  /* @End: UT_CORE_0057 */
  EXPECT_THAT(sut1 >= sut2, Eq(true));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(800);
  sut1.emplace_back(900);
  sut1.emplace_back(1000);

  sut2.emplace_back(800);
  sut2.emplace_back(1000);
  sut2.emplace_back(1000);
  /* @Start: UT_CORE_0057 */
  /* @TS2: Invoke the operator>= of FixedVector. */
  /* @TE2: operator>= should return true if the first operand is greater or equal to second. */
  /* @End: UT_CORE_0057 */
  EXPECT_THAT(sut1 >= sut2, Eq(false));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(800);
  sut1.emplace_back(800);
  sut1.emplace_back(1000);

  sut2.emplace_back(700);
  sut2.emplace_back(800);
  sut2.emplace_back(1000);
  /* @Start: UT_CORE_0057 */
  /* @TS3: Invoke the operator>= of FixedVector. */
  /* @TE3: operator>= should return true if the first operand is greater or equal to second. */
  /* @End: UT_CORE_0057 */
  EXPECT_THAT(sut1 >= sut2, Eq(true));

  sut1.clear();
  sut2.clear();

  sut1.emplace_back(800);
  sut1.emplace_back(800);
  sut1.emplace_back(1000);

  sut2.emplace_back(800);
  sut2.emplace_back(800);
  sut2.emplace_back(1000);
  /* @Start: UT_CORE_0057 */
  /* @TS4: Invoke the operator>= of FixedVector. */
  /* @TE4: operator>= should return true if the first operand is greater or equal to second. */
  /* @End: UT_CORE_0057 */
  EXPECT_THAT(sut1 >= sut2, Eq(true));
}

} // namespace
