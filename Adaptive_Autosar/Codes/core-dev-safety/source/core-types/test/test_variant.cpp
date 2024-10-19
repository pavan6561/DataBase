// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_variant.cpp
//
// Purpose     : This file provides the unit test cases of class Variant.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/variant.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Ne;
using namespace testing;

class variant_Test : public Test
{
 public:
  void SetUp() override
  {
    internal::CaptureStderr();
    DoubleDelete::dtorCalls = 0;
    DoubleDelete::ctorCalls = 0;
    DTorTest::dtorWasCalled = false;
  }

  void TearDown() override
  {
    std::string output = internal::GetCapturedStderr();
    if (Test::HasFailure())
    {
      std::cout << output << std::endl;
    }
  }

  class ComplexClass
  {
   public:
    ComplexClass(int a, float b) : a(a), b(b)
    {
    }

    int a;
    float b;
  };

  struct DTorTest
  {
    ~DTorTest()
    {
      dtorWasCalled = true;
    }
    static bool dtorWasCalled;
  };

  class DoubleDelete
  {
   public:
    DoubleDelete() : doDtorCall{true}
    {
      ctorCalls++;
    }
    ~DoubleDelete()
    {
      Delete();
    }
    DoubleDelete(const DoubleDelete& rhs) : doDtorCall{false}
    {
      *this = rhs;
    }
    DoubleDelete(DoubleDelete&& rhs) : doDtorCall{false}
    {
      *this = std::move(rhs);
    }

    DoubleDelete& operator=(const DoubleDelete& rhs)
    {
      if (this != &rhs)
      {
        Delete();
        doDtorCall = rhs.doDtorCall;
      }
      return *this;
    }

    DoubleDelete& operator=(DoubleDelete&& rhs)
    {
      if (this != &rhs)
      {
        Delete();
        doDtorCall = std::move(rhs.doDtorCall);
        rhs.doDtorCall = false;
      }
      return *this;
    }

    void Delete()
    {
      if (doDtorCall)
      {
        dtorCalls++;
      }
    }
    static int dtorCalls;
    static int ctorCalls;

   private:
    bool doDtorCall;
  };

  ara::core::Variant<int, float, ComplexClass> sut;
};
bool variant_Test::DTorTest::dtorWasCalled = false;
int variant_Test::DoubleDelete::dtorCalls = 0;
int variant_Test::DoubleDelete::ctorCalls = 0;
static constexpr uint64_t INVALID_VARIANT_INDEX = std::numeric_limits<uint64_t>::max();

/*******************************************************************************
@TestCaseId:          UT_CORE_0127
@Test_Description:    To test the functionality of Invalid_Variant of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611336
*******************************************************************************/
TEST_F(variant_Test, DefaultCTorCreatesInvalidVariant)
{
  //::testing::Test::RecordProperty("TEST_ID", "368fdd21-fd98-4f7a-abb6-");
  EXPECT_THAT(sut.index(), Eq(INVALID_VARIANT_INDEX));
  /* @Start: UT_CORE_0127 */
  /* @TS1: Call the index() of variant_Test fixture. */
  /* @TE1: Index() of variant_Test fixture should be equal to INVALID_VARIANT_INDEX. */
  /* @End: UT_CORE_0127 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0128
@Test_Description:    To test the functionality of Index() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611336
*******************************************************************************/
TEST_F(variant_Test, InitializedVariantReturnsCorrectIndex)
{
  ::testing::Test::RecordProperty("TEST_ID", "7b1cf4b5-e38d-4aea-804a-f8bd75e0a605");
  sut.emplace<float>(1231.22F);
  EXPECT_THAT(sut.index(), Eq(1U));
  /* @Start: UT_CORE_0128 */
  /* @TS1: emplace the float data type into variant_Test and Call the index() of variant_Test fixture. */
  /* @TE1: Index() of variant_Test fixture should be equal to 1. */
  /* @End: UT_CORE_0128 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0129
@Test_Description:    To test the functionality of Variant value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611330
*******************************************************************************/
TEST_F(variant_Test, CreatingVariantFromPODTypeReturnsProvidedValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "d087b440-a669-4467-a016-3a4ae5b5882a");
  ara::core::Variant<ComplexClass, float> sut2{42.42F};

  ASSERT_THAT(sut2.index(), Eq(1U));
  ASSERT_THAT(sut2.get<float>(), Ne(nullptr));
  EXPECT_THAT(*sut2.get<float>(), Eq(42.42F));
  /* @Start: UT_CORE_0129 */
  /* @TS1: Create the instance of Variant as sut2 with value as 42.42F and Call the index() of sut2. */
  /* @TS2: Call the get() of sut2 for float data type. */
  /* @TE1: Index() of sut2 should return1. */
  /* @TE2: get<float>() of sut2 should return42.42F. */
  /* @End: UT_CORE_0129 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0130
@Test_Description:    To test the functionality of Variant value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611332
*******************************************************************************/
TEST_F(variant_Test, CreatingVariantFromLValueReturnsProvidedValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "ff991aeb-15de-45fe-b6fb-a0a3d3c36c68");
  std::string string("Buhh");
  ara::core::Variant<std::string, float> sut2{string};
  ASSERT_THAT(sut2.index(), Eq(0U));
  ASSERT_THAT(sut2.get<std::string>(), Ne(nullptr));
  EXPECT_THAT(sut2.get<std::string>()->c_str(), StrEq("Buhh"));
  /* @Start: UT_CORE_0130 */
  /* @TS1: Create the instance of Variant as sut2 for string and float data type. */
  /* @TE1: index() of sut2 should return0. */
  /* @TS2: Call the index() ofsut2. */
  /* @TE2: get() of sut2 should not return nullptr. */
  /* @TS3: Call the get() of sut2 for std::string data type. */
  /* @TE3: get() of sut2 should returnstring as "Buhh". */
  /* @End: UT_CORE_0130 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0131
@Test_Description:    To test the functionality of Index() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611333
*******************************************************************************/
TEST_F(variant_Test, CreatingVariantWithSameTypeChoosesFirstFittingType)
{
  ::testing::Test::RecordProperty("TEST_ID", "819b5c7d-106c-476a-a49e-aaa78e092e3f");
  ara::core::Variant<float, float> sut2{73.73F};

  ASSERT_THAT(sut2.index(), Eq(0U));
  ASSERT_THAT(sut2.get<float>(), Ne(nullptr));
  EXPECT_THAT(*sut2.get<float>(), Eq(73.73F));
  /* @Start: UT_CORE_0131 */
  /* @TS1: Create the instance of Variant as sut2 for float and float data type. */
  /* @TE1: index() of sut2 should return0. */
  /* @TS2: Call the get() of sut2 for float data type. */
  /* @TE2: get() of sut2 should not return nullptr. */
  /* @TS3: Call the get() of sut2 for float data type. */
  /* @TE3: get() of sut2 should returnstring as 73.73F. */
  /* @End: UT_CORE_0131 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0132
@Test_Description:    To test the functionality of Emplace Valid elements of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611328
*******************************************************************************/
TEST_F(variant_Test, EmplaceValidElementWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "24021bd7-3749-4ca3-9b35-db3cb4837bff");
  ASSERT_THAT(sut.emplace<ComplexClass>(123, 456.789F), Eq(true));
  ASSERT_THAT(sut.get<ComplexClass>(), Ne(nullptr));
  EXPECT_THAT(sut.get<ComplexClass>()->a, Eq(123));
  EXPECT_THAT(sut.get<ComplexClass>()->b, Eq(456.789F));
  /* @Start: UT_CORE_0132 */
  /* @TS1: Create the instance of Variant as sut2 forComplexClass data type. */
  /* @TE1: emplace() of sut should returntrue. */
  /* @TS2: Emplace the ComplexClass into sut2 with values 123 and 456.789F. */
  /* @TE2: get() of sut should not return nullptr. */
  /* @End: UT_CORE_0132 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0133
@Test_Description:    To test the functionality of Valid elements of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611329
*******************************************************************************/
TEST_F(variant_Test, EmplaceSecondValidElementWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "fb4ba160-dd0c-4255-9916-cc56d950e970");
  sut.emplace<ComplexClass>(123, 456.789f);
  ASSERT_THAT(sut.emplace<ComplexClass>(912, 65.03F), Eq(true));
  ASSERT_THAT(sut.get<ComplexClass>(), Ne(nullptr));
  EXPECT_THAT(sut.get<ComplexClass>()->a, Eq(912));
  EXPECT_THAT(sut.get<ComplexClass>()->b, Eq(65.03F));
  /* @Start: UT_CORE_0133 */
  /* @TS1: Create the instance of Variant as sut for ComplexClass data type. */
  /* @TE1: emplace() of sut should returntrue. */
  /* @TS2: emplace the ComplexClass into sut2 with values 123 and 456.789F. */
  /* @TE2: get() of sut should returnfirst value as 912. */
  /* @TS3: emplace the ComplexClass into sut2 with values 912 and 65.03F. */
  /* @TE3: get() of sut should returnfirst value as 65.03F. */
  /* @End: UT_CORE_0133 */
}

TEST_F(variant_Test, DISABLED_emplaceInvalidElement)
{
  ::testing::Test::RecordProperty("TEST_ID", "6fa3b290-8249-4825-8eac-72235a06710e");
  // this is a compile time check, if you uncomment this the compiler will
  // fail
  //    EXPECT_THAT(sut.emplace< unsigned int >(0), Eq(false));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0134
@Test_Description:    To test the functionality of emplace() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611328
*******************************************************************************/
TEST_F(variant_Test, EmplaceWhenAlreadyDifferentTypeAssignedDoesNotWork)
{
  ::testing::Test::RecordProperty("TEST_ID", "f2c0825a-349c-4acb-9643-cb046f7e0310");
  sut.emplace<int>(123);
  EXPECT_THAT(sut.emplace<float>(123.F), Eq(false));
  /* @Start: UT_CORE_0134 */
  /* @TS1: emplace the int into the test sut with the value 123. */
  /* @TE1: emplace() () for float should returnfalse. */
  /* @End: UT_CORE_0134 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0135
@Test_Description:    To test the functionality of Uninitialized of Variant
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611332, 611333
*******************************************************************************/
TEST_F(variant_Test, GetOnUninitializedVariantFails)
{
  ::testing::Test::RecordProperty("TEST_ID", "d1da6416-f198-4756-9bdd-2c61b9750005");
  EXPECT_THAT(sut.get<float>(), Eq(nullptr));
  /* @Start: UT_CORE_0135 */
  /* @TS1: call the get( ) of variant_Test sut for float data type. */
  /* @TE1: get() of float should returnnullptr. */
  /* @End: UT_CORE_0135 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0136
@Test_Description:    To test the functionality of Emplace value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611328, 611332, 611333
*******************************************************************************/
TEST_F(variant_Test, GetVariantWithCorrectValueWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "ab08976b-0986-4fad-bfdb-f10810d19ae6");
  sut.emplace<float>(123.12F);
  EXPECT_THAT(sut.get<float>(), Ne(nullptr));
  /* @Start: UT_CORE_0136 */
  /* @TS1: Call the emplace() of variant_Test fixture for float data type with value of 123.12F. */
  /* @TE1: get() of variant_Test fixture should not equal to nullptr. */
  /* @End: UT_CORE_0136 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0137
@Test_Description:    To test the functionality of emplace() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611328, 611332, 611333
*******************************************************************************/
TEST_F(variant_Test, GetVariantWithIncorrectValueFails)
{
  ::testing::Test::RecordProperty("TEST_ID", "9c83f8ed-f4a1-4e45-b63f-26924544cc77");
  sut.emplace<float>(123.12F);
  EXPECT_THAT(sut.get<int>(), Eq(nullptr));
  /* @Start: UT_CORE_0137 */
  /* @TS1: Call the emplace() of variant_Test fixture for float data type with value of 123.12F. */
  /* @TE1: get() of variant_Test fixture for int data type should returnnullptr. */
  /* @End: UT_CORE_0137 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0138
@Test_Description:    To test the functionality of get() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611332, 611333
*******************************************************************************/
TEST_F(variant_Test, ConstGetOnUninitializedVariantFails)
{
  ::testing::Test::RecordProperty("TEST_ID", "16b511c8-e56a-48c9-bbff-5a7d07e7a500");
  EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->get<float>(), Eq(nullptr));
  /* @Start: UT_CORE_0138 */
  /* @TS1: Call the emplace() of variant_Test fixture for float. */
  /* @TE1: get() of variant_Test fixture for float data type should returnnullptr. */
  /* @End: UT_CORE_0138 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0139
@Test_Description:    To test the functionality of emplace() Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611332, 611333
*******************************************************************************/
TEST_F(variant_Test, constGetVariantWithCorrectValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "4419f569-0541-43ab-8448-9ba10556b459");
  sut.emplace<float>(123.12F);
  EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->get<float>(), Ne(nullptr));
  /* @Start: UT_CORE_0139 */
  /* @TS1: Call the emplace() of variant_Test fixture for float data type with value of 123.12F. */
  /* @TE1: get() of variant_Test fixture for float data type should not return nullptr. */
  /* @End: UT_CORE_0139 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0140
@Test_Description:    To test the functionality of get() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611332, 611333
*******************************************************************************/
TEST_F(variant_Test, ConstGetVariantWithIncorrectValueFails)
{
  ::testing::Test::RecordProperty("TEST_ID", "979e1131-e981-4f67-bef2-a1fe4770c5a6");
  sut.emplace<float>(123.12F);
  EXPECT_THAT(const_cast<const decltype(sut)*>(&sut)->get<int>(), Eq(nullptr));
  /* @Start: UT_CORE_0140 */
  /* @TS1: Call the emplace() of variant_Test fixture for float data type with value of 123.12F. */
  /* @TE1: get() of variant_Test fixture for int data type should returnnullptr. */
  /* @End: UT_CORE_0140 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0141
@Test_Description:    To test the functionality of Get_if() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611334
*******************************************************************************/
TEST_F(variant_Test, Get_ifWhenUninitializedReturnsProvidedValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "64585111-3495-4c01-8900-af2e19223e62");
  float bla;
  EXPECT_THAT(sut.get_if<float>(&bla), Eq(&bla));
  /* @Start: UT_CORE_0141 */
  /* @TS1: Call the get_if() of variant_Test fixture for float data type. */
  /* @TE1: get_if() of variant_Test fixture for float data type should returnsame refrence. */
  /* @End: UT_CORE_0141 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0142
@Test_Description:    To test the functionality of emplace() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611334, 611332, 611333
*******************************************************************************/
TEST_F(variant_Test, Get_ifInitializedWithCorrectValueWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "27b80822-0f32-46a6-83d9-595b35e23139");
  sut.emplace<float>(12.1F);
  float bla;
  EXPECT_THAT(sut.get_if<float>(&bla), Ne(&bla));
  /* @Start: UT_CORE_0142 */
  /* @TS1: Call the emplace() of variant_Test fixture for float data type with value of 12.1F. */
  /* @TE1: get_if() of variant_Test fixture for float data type should not return same refrence. */
  /* @End: UT_CORE_0142 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0143
@Test_Description:    To test the functionality of get_if() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611334, 611332, 611333
*******************************************************************************/
TEST_F(variant_Test, Get_ifInitializedWithIncorrectValueReturnsProvidedValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "4126af94-d4ce-405a-bcc7-1b6d1fce6d0b");
  sut.emplace<float>(12.1F);
  int bla;
  EXPECT_THAT(sut.get_if<int>(&bla), Eq(&bla));
  /* @Start: UT_CORE_0143 */
  /* @TS1: Call the emplace() of variant_Test fixture for float data type with value of 12.1F. */
  /* @TE1: get_if() of variant_Test fixture for int data type should returnsame refrence. */
  /* @End: UT_CORE_0143 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0144
@Test_Description:    To test the functionality of DTorTest of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611326
*******************************************************************************/
TEST_F(variant_Test, DTorIsCalled)
{
  ::testing::Test::RecordProperty("TEST_ID", "46f16073-af10-4d4b-9944-a316bc5847ef");
  DTorTest::dtorWasCalled = false;
  {
    ara::core::Variant<int, DTorTest> schlomo;
    schlomo.emplace<DTorTest>();
  }
  EXPECT_THAT(DTorTest::dtorWasCalled, Eq(true));
  /* @Start: UT_CORE_0144 */
  /* @TS1: Call the DTorTest::dtorWasCalled of variant_Test fixture. */
  /* @TE1: DTorTest::dtorWasCalled should returntrue. */
  /* @End: UT_CORE_0144 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0145
@Test_Description:    To test the functionality of dtorWasCalled() Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611326
*******************************************************************************/
TEST_F(variant_Test, DTorIsCalledAfterEmplace)
{
  ::testing::Test::RecordProperty("TEST_ID", "0de8d215-76be-4727-9fb4-42c54cb1b742");
  {
    ara::core::Variant<int, float, DTorTest, double> ignatz;
    ignatz.emplace<DTorTest>();
    DTorTest::dtorWasCalled = false;
  }
  EXPECT_THAT(DTorTest::dtorWasCalled, Eq(true));
  /* @Start: UT_CORE_0145 */
  /* @TS1: Call the DTorTest::dtorWasCalled() of variant_Test fixture. */
  /* @TE1: DTorTest::dtorWasCalled should returntrue. */
  /* @End: UT_CORE_0145 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0146
@Test_Description:    To test the functionality of get() Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611332
*******************************************************************************/
TEST_F(variant_Test, CopyCTorWithValueLeadsToSameValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "710f8add-4a3c-4b9c-b429-5f42689b49da");
  ara::core::Variant<int, char> schlomo;
  schlomo.emplace<int>(123);
  ara::core::Variant<int, char> ignatz(schlomo);
  ASSERT_THAT(ignatz.get<int>(), Ne(nullptr));
  EXPECT_THAT(*ignatz.get<int>(), Eq(123));
  /* @Start: UT_CORE_0146 */
  /* @TS1: create the instance of Variant as schlomo and emplace the int data type of value 123. */
  /* @TE1: get() of ignatz for int data type should not return nullptr. */
  /* @TS2: create the vairant for char and int types and assign the schlomo in it. */
  /* @TE2: derefrence get () of ignatz for int data type should return123. */
  /* @End: UT_CORE_0146 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0147
@Test_Description:    To test the functionality of Invalid_Variant value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611336
*******************************************************************************/
TEST_F(variant_Test, CopyCTorWithoutValueResultsInInvalidVariant)
{
  ::testing::Test::RecordProperty("TEST_ID", "31b12efc-4f2d-4b3c-ad72-c12ca8a0cfc3");
  ara::core::Variant<int, char> schlomo;
  ara::core::Variant<int, char> ignatz(schlomo);
  ASSERT_THAT(ignatz.index(), Eq(ara::core::INVALID_VARIANT_INDEX));
  /* @Start: UT_CORE_0147 */
  /* @TS1: create the two diffrent objects for Variant with diffrent data types */
  /* @TE1: index() of ignatz for int and char data type should be equal to INVALID_VARIANT_INDEX. */
  /* @End: UT_CORE_0147 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0148
@Test_Description:    To test the functionality of assignment operator of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611323
*******************************************************************************/
TEST_F(variant_Test, CopyAssignmentWithValueLeadsToSameValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "ebf341fa-c218-4ab8-ae13-a35d707bb5b2");
  ara::core::Variant<int, char> ignatz;
  ignatz.emplace<char>('c');
  {
    ara::core::Variant<int, char> schlomo;
    schlomo.emplace<int>(447);
    ignatz = schlomo;
  }
  ASSERT_THAT(ignatz.get<int>(), Ne(nullptr));
  ASSERT_THAT(*ignatz.get<int>(), Eq(447));

  /* @Start: UT_CORE_0148 */
  /* @TS1: Create the two diffrent objects for Variant as ignatz and schlomo and call the assignment operator. */
  /* @TE1: Assignment operator should be copy the data from one Variant to another. */
  /* @End: UT_CORE_0148 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0149
@Test_Description:    To test the functionality of Copy Assignment of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611323
*******************************************************************************/
TEST_F(variant_Test, CopyAssignmentWithoutValueResultsInInvalidVariant)
{
  ::testing::Test::RecordProperty("TEST_ID", "75466e8c-78b3-4a2f-84cb-06849cf80baa");
  ara::core::Variant<int, char> ignatz;
  ignatz.emplace<char>('c');
  {
    ara::core::Variant<int, char> schlomo;
    ignatz = schlomo;
  }
  ASSERT_THAT(ignatz.index(), Eq(ara::core::INVALID_VARIANT_INDEX));
  ASSERT_THAT(ignatz.get<char>(), Eq(nullptr));
  /* @Start: UT_CORE_0149 */
  /* @TS1: Create the two diffrent objects for Variant as ignatz and schlomo and call the assignment operator. */
  /* @TE1: Assignment operator should copy the data from one Variant to another. */
  /* @End: UT_CORE_0149 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0150
@Test_Description:    To test the functionality of Move constructor of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611324
*******************************************************************************/
TEST_F(variant_Test, MoveCTorWithValueLeadsToSameValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "34962242-8319-48e5-a064-5118c6ffa080");
  ara::core::Variant<int, char> schlomo;
  schlomo.emplace<int>(123);
  ara::core::Variant<int, char> ignatz(std::move(schlomo));
  ASSERT_THAT(ignatz.get<int>(), Ne(nullptr));
  EXPECT_THAT(*ignatz.get<int>(), Eq(123));
  EXPECT_THAT(schlomo.index(), Eq(0U));
  /* @Start: UT_CORE_0150 */
  /* @TS1: Create the two diffrent objects for Variant as ignatz and schlomo and call move constructor. */
  /* @TE1: Move constructor should be move the data from one object to into another object. */
  /* @End: UT_CORE_0150 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0151
@Test_Description:    To test the functionality of Invalid_Variant_Index of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611336
*******************************************************************************/
TEST_F(variant_Test, MoveCTorWithoutValueResultsInInvalidVariant)
{
  ::testing::Test::RecordProperty("TEST_ID", "83db1777-3e66-4755-9072-0c90973ae303");
  ara::core::Variant<int, char> schlomo;
  ara::core::Variant<int, char> ignatz(std::move(schlomo));
  ASSERT_THAT(ignatz.index(), Eq(ara::core::INVALID_VARIANT_INDEX));
  /* @Start: UT_CORE_0151 */
  /* @TS1: create the two diffrent objects for Variant as ignatz and schlomo and call move constructor. */
  /* @TE1: move constructor should be move the data from one object to into another object. */
  /* @End: UT_CORE_0151 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0152
@Test_Description:    To test the functionality of Move Assignment with value of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611325
*******************************************************************************/
TEST_F(variant_Test, MoveAssignmentWithValueLeadsToSameValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "ee36df28-545f-42bc-9ef6-3699284f1a42");
  ara::core::Variant<int, char> ignatz;
  ignatz.emplace<char>('c');
  {
    ara::core::Variant<int, char> schlomo;
    schlomo.emplace<int>(447);
    ignatz = std::move(schlomo);
  }
  ASSERT_THAT(ignatz.get<int>(), Ne(nullptr));
  ASSERT_THAT(*ignatz.get<int>(), Eq(447));
  /* @Start: UT_CORE_0152 */
  /* @TS1: create the two diffrent objects for Variant as ignatz and schlomo and call move constructor. */
  /* @TE1: move constructor should be move the data from one object to into another object. */
  /* @End: UT_CORE_0152 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0153
@Test_Description:    To test the functionality of index() Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611336
*******************************************************************************/
TEST_F(variant_Test, MoveAssignmentWithoutValueResultsInInvalidVariant)
{
  ::testing::Test::RecordProperty("TEST_ID", "5b6c8183-3ea1-44ee-ac99-7f427127b82b");
  ara::core::Variant<int, char> ignatz;
  ignatz.emplace<char>('c');
  {
    ara::core::Variant<int, char> schlomo;
    ignatz = std::move(schlomo);
  }
  ASSERT_THAT(ignatz.get<int>(), Eq(nullptr));
  ASSERT_THAT(ignatz.index(), Eq(ara::core::INVALID_VARIANT_INDEX));
  /* @Start: UT_CORE_0153 */
  /* @TS1: create the two diffrent objects for Variant as ignatz and schlomo and call move constructor. */
  /* @TE1: move constructor should be move the data from one object to into another object. */
  /* @End: UT_CORE_0153 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0154
@Test_Description:    To test the functionality of DTorTest of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611326
*******************************************************************************/
TEST_F(variant_Test, CreatingSecondObjectViaCopyCTorResultsInTwoDTorCalls)
{
  ::testing::Test::RecordProperty("TEST_ID", "57a5836f-d981-445a-9040-6c3358cee6c4");
  {
    ara::core::Variant<int, DTorTest> ignatz;
    ignatz.emplace<DTorTest>();
    DTorTest::dtorWasCalled = false;
    {
      ara::core::Variant<int, DTorTest> schlomo(ignatz);
      EXPECT_THAT(DTorTest::dtorWasCalled, Eq(false));
    }
    EXPECT_THAT(DTorTest::dtorWasCalled, Eq(true));
    DTorTest::dtorWasCalled = false;
  }
  EXPECT_THAT(DTorTest::dtorWasCalled, Eq(true));
  /* @Start: UT_CORE_0154 */
  /* @TS1: Call the DTorTest::dtorWasCalled method of variant_Test fixture. */
  /* @TE1: DTorTest::dtorWasCalled should returntrue. */
  /* @End: UT_CORE_0154 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0155
@Test_Description:    To test the functionality of DTorCalls of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611326
*******************************************************************************/
TEST_F(variant_Test, CreatingSecondObjectViaCopyAssignmentResultsInTwoDTorCalls)
{
  ::testing::Test::RecordProperty("TEST_ID", "6932e30b-f24f-46be-88e0-d745d9f0db92");
  {
    ara::core::Variant<int, DTorTest> ignatz;
    ignatz.emplace<DTorTest>();
    DTorTest::dtorWasCalled = false;
    {
      ara::core::Variant<int, DTorTest> schlomo;
      schlomo.emplace<int>(123);
      schlomo = ignatz;
      EXPECT_THAT(DTorTest::dtorWasCalled, Eq(false));
    }
    EXPECT_THAT(DTorTest::dtorWasCalled, Eq(true));
    DTorTest::dtorWasCalled = false;
  }
  EXPECT_THAT(DTorTest::dtorWasCalled, Eq(true));
  /* @Start: UT_CORE_0155 */
  /* @TS1: Call the DTorTest::dtorWasCalled method of variant_Test fixture. */
  /* @TE1: DTorTest::dtorWasCalled should returntrue. */
  /* @End: UT_CORE_0154 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0158
@Test_Description:    To test the functionality of index() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611336
*******************************************************************************/
TEST_F(variant_Test, DirectValueAssignmentResultsInCorrectIndex)
{
  ::testing::Test::RecordProperty("TEST_ID", "55377419-08dc-4d6b-b163-b2da7ad70417");
  ara::core::Variant<int, float> schlomo;
  schlomo = 123;
  EXPECT_THAT(schlomo.index(), Eq(0U));
  /* @Start: UT_CORE_0158 */
  /* @TS1: create the object for Variant and place the int value in it and call the index () . */
  /* @TE1: index() should returnthe index of Variant */
  /* @End: UT_CORE_0158 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0159
@Test_Description:    To test the functionality of index of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611336
*******************************************************************************/
TEST_F(variant_Test, DirectValueAssignmentWhenAlreadyAssignedWithDifferentType)
{
  ::testing::Test::RecordProperty("TEST_ID", "a058c173-497b-43ec-ba03-2702f3ba8190");
  ara::core::Variant<int, float> schlomo;
  schlomo = 123;
  schlomo = 123.01F;
  EXPECT_THAT(schlomo.index(), Eq(0U));
  /* @Start: UT_CORE_0159 */
  /* @TS1: create the object for Variant and place the int value in it and call the index (). */
  /* @TE1: index () should return the index of Variant. */
  /* @End: UT_CORE_0159 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0160
@Test_Description:    To test the functionality of holds_alternative for int dat type of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611875, 611875
*******************************************************************************/
TEST_F(variant_Test, HoldsAlternativeForCorrectType)
{
  ::testing::Test::RecordProperty("TEST_ID", "9da264db-a84e-41cd-94ff-92af529e2d6b");
  ara::core::Variant<int, float> schlomo;
  schlomo = 123;
  EXPECT_THAT(ara::core::holds_alternative<int>(schlomo), Eq(true));
  /* @Start: UT_CORE_0160 */
  /* @TS1: create the object of Variant and call the holds_alternative() for int data type. */
  /* @TE1: holds_alternative should be returns the true. */
  /* @End: UT_CORE_0160 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0161
@Test_Description:    To test the functionality of holds_alternative() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611875
*******************************************************************************/
TEST_F(variant_Test, HoldsAlternativeForIncorrectType)
{
  ::testing::Test::RecordProperty("TEST_ID", "63f3690f-1f66-407c-8050-97f47f62638e");
  ara::core::Variant<int, float> schlomo;
  schlomo = 123;
  EXPECT_THAT(ara::core::holds_alternative<float>(schlomo), Eq(false));
  /* @Start: UT_CORE_0161 */
  /* @TS1: create the object of Variant and call the holds_alternative() for float data type. */
  /* @TE1: holds_alternative should be returns the false. */
  /* @End: UT_CORE_0161 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0162
@Test_Description:    To test the functionality of index() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611330, 611328
*******************************************************************************/
TEST_F(variant_Test, SameTypeVariantAndEmplaceWithIndexResultsInCorrectValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "fb55e6d8-d42d-4073-b5db-5300c56df540");
  ara::core::Variant<int, float, int> schlomo;

  ASSERT_THAT(schlomo.emplace_at_index<2>(123), Eq(true));
  EXPECT_THAT(*schlomo.get_at_index<2>(), Eq(123));
  /* @Start: UT_CORE_0162 */
  /* @TS1: create the object of Variant and call the emplace_at_index (). */
  /* @TE1: get_at_index should returnemplaced value. */
  /* @End: UT_CORE_0162 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0163
@Test_Description:    To test the functionality of emplace_at_index() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611328
*******************************************************************************/
TEST_F(variant_Test, SameTypeVariantResultsInCorrectIndex)
{
  ::testing::Test::RecordProperty("TEST_ID", "10994259-9fb5-411f-9e12-365f2d8e09fd");
  ara::core::Variant<int, float, int> schlomo;

  EXPECT_THAT(schlomo.emplace_at_index<1>(1.23F), Eq(true));
  EXPECT_THAT(schlomo.index(), Eq(1U));
  /* @Start: UT_CORE_0163 */
  /* @TS1: create the object of Variant and call the emplace_at_index () for float data type. */
  /* @TE1: get_at_index should returnemplaced value. */
  /* @End: UT_CORE_0163 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0164
@Test_Description:    To test the functionality of get_at_index () of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611328
*******************************************************************************/
TEST_F(variant_Test, SameTypeVariantReturnsNothingForIncorrectIndex)
{
  ::testing::Test::RecordProperty("TEST_ID", "04c16cb1-f67f-47fa-bde8-f15ff0d044c3");
  ara::core::Variant<int, float, int> schlomo;

  ASSERT_THAT(schlomo.emplace_at_index<2>(123), Eq(true));
  EXPECT_THAT(schlomo.get_at_index<1>(), Eq(nullptr));
  /* @Start: UT_CORE_0164 */
  /* @TS1: create the object of Variant and call the emplace_at_index () for index number 2. */
  /* @TE1: get_at_index at first index should returnnullptr. */
  /* @End: UT_CORE_0164 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0165
@Test_Description:    To test the functionality of variant_emplace with index value of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611328
*******************************************************************************/
TEST_F(variant_Test, ConstSameTypeVariantAndEmplaceWithIndexResultsInCorrectValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "89511ef2-46e6-49f5-8272-713a860ff070");
  ara::core::Variant<int, float, int> schlomo;
  const ara::core::Variant<int, float, int>* ignatz = &schlomo;

  ASSERT_THAT(schlomo.emplace_at_index<2>(4123), Eq(true));
  EXPECT_THAT(*ignatz->get_at_index<2>(), Eq(4123));
  /* @Start: UT_CORE_0165 */
  /* @TS1: create the object of Variant and call the emplace_at_index () for index number 2 with value 4123. */
  /* @TE1: get_at_index at 2 index should return4123. */
  /* @End: UT_CORE_0165 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0166
@Test_Description:    To test the functionality of index() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611319
*******************************************************************************/
TEST_F(variant_Test, InPlaceAtIndexCTorResultsInCorrectIndexAndValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "6a1fe90d-da43-4d82-b5f5-e940b4724e86");
  ara::core::Variant<int, float, int> schlomo(ara::core::InPlaceIndex<0>(), 445);

  ASSERT_THAT(schlomo.index(), Eq(0U));
  EXPECT_THAT(*schlomo.get_at_index<0>(), Eq(445));
  /* @Start: UT_CORE_0166 */
  /* @TS1: create the object of Variant and call the InPlaceIndex() for index number 0 with value 445. */
  /* @TE1: get_at_index at 0 index should return 445. */
  /* @End: UT_CORE_0166 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0167
@Test_Description:    To test the functionality of index() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611319
*******************************************************************************/
TEST_F(variant_Test, InPlaceAtTypeCTorResultsInCorrectIndexAndValue)
{
  ::testing::Test::RecordProperty("TEST_ID", "71ddd0cd-125f-4f69-b12d-f0f024c541a4");
  ara::core::Variant<int, float, double> schlomo(ara::core::InPlaceType<double>(), 90.12);

  ASSERT_THAT(schlomo.index(), Eq(2U));
  EXPECT_THAT(*schlomo.get_at_index<2>(), Eq(90.12));
  /* @Start: UT_CORE_0167 */
  /* @TS1: create the object of Variant and call the InPlaceIndex() for index number 0 with value 445. */
  /* @TE1: get_at_index at 0 index should return 445. */
  /* @End: UT_CORE_0167 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0177
@Test_Description:    To test the functionality of emplace() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       Initialize variant_Test Test Fixture.
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611328
*******************************************************************************/
TEST_F(variant_Test, CopyVariantIntoVariantOfDifferentType)
{
  ::testing::Test::RecordProperty("TEST_ID", "d0ea4fed-7b18-4d0d-aa05-d76911fb29f7");
  DoubleDelete::ctorCalls = 0;
  DoubleDelete::dtorCalls = 0;
  ara::core::Variant<DoubleDelete, ComplexClass> sut1, sut2;
  sut1.emplace<DoubleDelete>();
  sut2.emplace<ComplexClass>(12, 12.12F);

  sut1 = sut2;

  EXPECT_THAT(DoubleDelete::dtorCalls, Eq(1));
  /* @Start: UT_CORE_0177 */
  /* @TS1: create the object of Variant and call the emplace method for sut1. */
  /* @TE1: emplace() should return true. */
  /* @End: UT_CORE_0177 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0179
@Test_Description:    To test the functionality of Swap() of Variant.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          variant_Test
@Requirement Tracing: -
@Design Tracing:      611875
*******************************************************************************/
TEST(VariantTest, Swap)
{
  ara::core::Variant<int, std::string> s1;
  ara::core::Variant<int, std::string> s2(s1);
  ara::core::Variant<int> s3;
  s1.emplace<std::string>("PHM");
  s2.emplace<std::string>("DM");
  std::string ret1 = s1.get<std::string>()->c_str();
  std::string ret2 = s2.get<std::string>()->c_str();
  ara::core::Swap(s1, s2);
  std::string ret3 = s1.get<std::string>()->c_str();
  std::string ret4 = s2.get<std::string>()->c_str();

  SUCCEED();
  /* @Start: UT_CORE_0179 */
  /* @TS1: Create the instances of Variant class and invoke the swap (). */
  /* @TE1: Swap () should be able to swap the content between the two instances. */
  /* @End: UT_CORE_0179 */
}
