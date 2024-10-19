// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_instance_specifier.cpp
//
// Purpose     : This file provides the unit test cases of class InstanceSpecifier.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/instance_specifier.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::Eq;
using ::testing::Lt;
using ::testing::Ne;
using ::testing::Not;
/*******************************************************************************
@TestCaseId:          UT_CORE_0099
@Test_Description:    To test the functionality of Constructor of InstanceSpecifier.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InstanceSpecifierTest
@Requirement Tracing: -
@Design Tracing:      611449
*******************************************************************************/
TEST(InstanceSpecifierTest, Constructor_StringView_Success)
{
  /* @Start: UT_CORE_0099 */
  /* @TS1: Create an instance of ara::core::InstanceSpecifier with parameter as "executable/swc/port". */
  /* @TE1: InstanceSpecifier object creation should not be throw any exception. */
  /* @End: UT_CORE_0099 */
  ara::core::InstanceSpecifier is("executable/swc/port");
  SUCCEED();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0100
@Test_Description:    To test the functionality of Factory Validation of InstanceSpecifier.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InstanceSpecifierTest
@Requirement Tracing: -
@Design Tracing:      611450, 611451
*******************************************************************************/
TEST(InstanceSpecifierTest, Factory_Validation)
{
  // OK
  /* @Start: UT_CORE_0100 */
  /* @TS1: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter the as "executable/swc/port". */
  /* @TE1: Create() should return HasValue as true. */
  /* @End: UT_CORE_0100 */
  ara::core::Result<ara::core::InstanceSpecifier> res1
      = ara::core::InstanceSpecifier::Create("executable/swc/port");
  EXPECT_THAT(res1.HasValue(), true);
  /* @Start: UT_CORE_0100 */
  /* @TS2: Check the return value of Create(). */
  /* @TE2: Create() should return the value equal to "executable/swc/port". */
  /* @End: UT_CORE_0100 */
  ara::core::InstanceSpecifier const& is1 = res1.Value();
  EXPECT_THAT(is1.ToString(), Eq("executable/swc/port"));

  // OK
  /* @Start: UT_CORE_0100 */
  /* @TS3: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as "executable/cluster1/cluster2/swc/port". */
  /* @TE3: Create() should return HasValue as true. */
  /* @End: UT_CORE_0100 */
  auto res2 = ara::core::InstanceSpecifier::Create("executable/cluster1/cluster2/swc/port");
  EXPECT_THAT(res2.HasValue(), true);

  /* @Start: UT_CORE_0100 */
  /* @TS4: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as "executable/s%%c/port". */
  /* @TE4: Create() should return the error as kInvalidMetaModelShortname. */
  /* @End: UT_CORE_0100 */
  // Invalid character
  auto res3 = ara::core::InstanceSpecifier::Create("executable/s%%c/port");
  EXPECT_TRUE(res3.CheckError(ara::core::CoreErrc::kInvalidMetaModelShortname));

  /* @Start: UT_CORE_0100 */
  /* @TS5: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as "executable/swc/port". */
  /* @TE5: Create() should return the error as kInvalidMetaModelShortname. */
  /* @End: UT_CORE_0100 */
  // Invalid start character for an identifier fragment
  auto res4 = ara::core::InstanceSpecifier::Create("_executable/swc/port");
  EXPECT_TRUE(res4.CheckError(ara::core::CoreErrc::kInvalidMetaModelShortname));

  /* @Start: UT_CORE_0100 */
  /* @TS6: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as "executable/swc/port". */
  /* @TE6: Create() should return the error as kInvalidMetaModelPath. */
  /* @End: UT_CORE_0100 */
  // Invalid leading '/' (i.e. empty fragment at the start)
  auto res5 = ara::core::InstanceSpecifier::Create("/executable/swc/port");
  EXPECT_TRUE(res5.CheckError(ara::core::CoreErrc::kInvalidMetaModelPath));

  /* @Start: UT_CORE_0100 */
  /* @TS7: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as "executable/swc/port". */
  /* @TE7: Create() should return the error as kInvalidMetaModelPath. */
  /* @End: UT_CORE_0100 */
  // Superfluous trailing '/' (i.e. empty fragment at the end)
  auto res6 = ara::core::InstanceSpecifier::Create("executable/swc/port/");
  EXPECT_TRUE(res6.CheckError(ara::core::CoreErrc::kInvalidMetaModelPath));

  /* @Start: UT_CORE_0100 */
  /* @TS8: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as "executable/swc/port". */
  /* @TE8: Create() should return the error as kInvalidMetaModelPath. */
  /* @End: UT_CORE_0100 */
  // Empty fragment in the middle
  auto res7 = ara::core::InstanceSpecifier::Create("executable/swc//port");
  EXPECT_TRUE(res7.CheckError(ara::core::CoreErrc::kInvalidMetaModelPath));

  /* @Start: UT_CORE_0100 */
  /* @TS9: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as "executable/s c/port". */
  /* @TE9: Create() should return the error as kInvalidMetaModelShortname.  */
  /* @End: UT_CORE_0100 */
  // element with whitespace
  auto res8 = ara::core::InstanceSpecifier::Create("executable/s c/port");
  EXPECT_TRUE(res8.CheckError(ara::core::CoreErrc::kInvalidMetaModelShortname));

  /* @Start: UT_CORE_0100 */
  /* @TS10: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as kInvalidMetaModelShortname. */
  /* @TE10: Create() should return the error as kInvalidMetaModelShortname. */
  /* @End: UT_CORE_0100 */
  auto res9 = ara::core::InstanceSpecifier::Create("executable/  /port");
  EXPECT_TRUE(res9.CheckError(ara::core::CoreErrc::kInvalidMetaModelShortname));

  /* @Start: UT_CORE_0100 */
  /* @TS11: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as empty string. */
  /* @TE11: Create() should return the error as kInvalidMetaModelShortname. */
  /* @End: UT_CORE_0100 */
  // Path with only whitespace
  auto res10 = ara::core::InstanceSpecifier::Create("  ");
  EXPECT_TRUE(res10.CheckError(ara::core::CoreErrc::kInvalidMetaModelShortname));

  /* @Start: UT_CORE_0100 */
  /* @TS12: Create an instance of ara::core::InstanceSpecifier::Create() and pass parameter as empty string. */
  /* @TE12 : Create() should return the error as kInvalidMetaModelPath. */
  /* @End: UT_CORE_0100 */
  // Empty path
  auto res11 = ara::core::InstanceSpecifier::Create("");
  EXPECT_TRUE(res11.CheckError(ara::core::CoreErrc::kInvalidMetaModelPath));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0101
@Test_Description:    To test the functionality of ToString InstanceSpecifier.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InstanceSpecifierTest
@Requirement Tracing: -
@Design Tracing:      611452
*******************************************************************************/
TEST(InstanceSpecifierTest, ToString)
{
  ara::core::InstanceSpecifier const is("executable/swc/port");
  /* @Start: UT_CORE_0101 */
  /* @TS1: Create an instance of ara::core::InstanceSpecifier as is and pass the parameter "executable/swc/port". */
  /* @TE1: is.ToString() should be equal to "executable/swc/port". */
  /* @End: UT_CORE_0101 */
  ASSERT_THAT(is.ToString(), Eq("executable/swc/port"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0102
@Test_Description:    To test the functionality of Equality operator of InstanceSpecifier.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InstanceSpecifierTest
@Requirement Tracing: -
@Design Tracing:      611449
*******************************************************************************/
TEST(InstanceSpecifierTest, EqualityComparisonOperator_OtherInstanceSpecifier)
{
  ara::core::InstanceSpecifier const is1("executable/swc/port");
  ara::core::InstanceSpecifier const is2("executable/swc/port");
  ara::core::InstanceSpecifier const is3("another_executable/swc/path");
  /* @Start: UT_CORE_0102 */
  /* @TS1: Create an instance of ara::core::InstanceSpecifier as is1, is2, is3. */
  /* @TE1: is1 should be equal to is2. */
  /* @End: UT_CORE_0102 */
  ASSERT_THAT(is1, Eq(is2));
  /* @Start: UT_CORE_0102 */
  /* @TS2: Compare the is1 and is3. */
  /* @TE2: is1 should not be equal to is3.*/
  /* @End: UT_CORE_0102 */
  ASSERT_THAT(is1, Ne(is3));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0103
@Test_Description:    To test the functionality of Equality operator of InstanceSpecifier.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InstanceSpecifierTest
@Requirement Tracing: -
@Design Tracing:      611449
*******************************************************************************/
TEST(InstanceSpecifierTest, EqualityComparisonOperator_StringView)
{
  ara::core::InstanceSpecifier const is("executable/swc/port");
  /* @Start: UT_CORE_0103 */
  /* @TS1: Create an instance of ara::core::InstanceSpecifier as is and pass the parameter as "executable/swc/port". */
  /* @TE1: instance should be equal to "executable/swc/port". */
  /* @End: UT_CORE_0103 */
  ASSERT_THAT(is, Eq("executable/swc/port"));
  /* @Start: UT_CORE_0103 */
  /* @TS2: Create an instance of ara::core::InstanceSpecifier as is and pass the parameter as "executable/swc/port". */
  /* @TE2: is should not be equal to "another_executable/swc/port" . */
  /* @End: UT_CORE_0103 */
  ASSERT_THAT(is, Ne("another_executable/swc/port"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0104
@Test_Description:    To test the functionality of Equality Comparison operator of InstanceSpecifier.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InstanceSpecifierTest
@Requirement Tracing: -
@Design Tracing:      611449
*******************************************************************************/
TEST(InstanceSpecifierTest, GlobalEqualityComparisonOperator_StringView)
{
  ara::core::InstanceSpecifier const is("executable/swc/port");
  /* @Start: UT_CORE_0104 */
  /* @TS1: Create an instance of ara::core::InstanceSpecifier as is("executable/swc/port"). */
  /* @TE1: "executable/swc/port" should be equal to is. */
  /* @End: UT_CORE_0104 */
  ASSERT_THAT("executable/swc/port", Eq(is));
  /* @Start: UT_CORE_0104 */
  /* @TS2: Create an instance of ara::core::InstanceSpecifier as is.*/
  /* @TE2: "another_executable/swc/port" should not be equal to instance is. */
  /* @End: UT_CORE_0104 */
  ASSERT_THAT("another_executable/swc/port", Ne(is));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0105
@Test_Description:    To test the functionality of Comparison operator of InstanceSpecifier.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InstanceSpecifierTest
@Requirement Tracing: -
@Design Tracing:      611449
*******************************************************************************/
TEST(InstanceSpecifierTest, OrderingComparisonOperator_OtherInstanceSpecifier)
{
  ara::core::InstanceSpecifier const is1("abc/swc/port");
  ara::core::InstanceSpecifier const is2("aca/swc/port");
  ara::core::InstanceSpecifier const is3("adc/swc/port");

  /* @Start: UT_CORE_0105 */
  /* @TS1: Create an instance of ara::core::InstanceSpecifier as is1, is2, is3 and compare the instances. */
  /* @TE1: is1 should be less than is2. */
  /* @End: UT_CORE_0105 */
  ASSERT_THAT(is1, Lt(is2));
  /* @Start: UT_CORE_0105 */
  /* @TS2: Compare is1 and is3. */
  /* @TE2: is1 should be less than is3. */
  /* @End: UT_CORE_0105 */
  ASSERT_THAT(is1, Lt(is3));
  /* @Start: UT_CORE_0105 */
  /* @TS3: Compare is2 and is3. */
  /* @TE3: is2 should be less than is3. */
  /* @End: UT_CORE_0105 */
  ASSERT_THAT(is2, Lt(is3));
  /* @Start: UT_CORE_0105 */
  /* @TS4: Compare is2 and is1. */
  /* @TE4: is1 should not be less than is1. */
  /* @End: UT_CORE_0105 */
  ASSERT_THAT(is2, Not(Lt(is1)));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0575
@Test_Description:    To test the functionality of Equality Operator of InstanceSpecifier.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InstanceSpecifierTest
@Requirement Tracing: -
@Design Tracing:      611453
*******************************************************************************/
TEST(InstanceSpecifierTest, EqualityOperator)
{
  // bool operator==(InstanceSpecifier const& other) const noexcept
  ara::core::InstanceSpecifier is1("executable/swc/port");
  ara::core::InstanceSpecifier is2("executable/swc/port");
  /* @Start: UT_CORE_0575 */
  /* @TS1: Create an instance of ara::core::InstanceSpecifier as is1, is2. */
  /* @TE1: is1 should be equal to is2. */
  /* @End: UT_CORE_0575 */
  ASSERT_THAT(is1, Eq(is2));
}