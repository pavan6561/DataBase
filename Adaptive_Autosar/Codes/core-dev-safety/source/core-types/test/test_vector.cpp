// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_vector.cpp
//
// Purpose     : This file provides the unit test cases of class Vector.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/vector.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Ne;
/*******************************************************************************
@TestCaseId:          UT_CORE_0180
@Test_Description:    To test the functionality of type is available and compilable.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          VectorTest
@Requirement Tracing: -
@Design Tracing:      611877
*******************************************************************************/
TEST(VectorTest, TypeIsAvailableAndCompilable)
{
  ara::core::Vector<int> v1;
  ara::core::Vector<std::string> v2;
  /* @Start: UT_CORE_0180 */
  /* @TS1: Create an instance of Vector for int data type. */
  /* @TE1: Instance should be created. */
  /* @TS2: Create an instance of Vector for std::string type. */
  /* @TE2: Instance should be created. */
  /* @End: UT_CORE_0180 */
  SUCCEED();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0181
@Test_Description:    To test the functionality of global compar is on operators work.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          VectorTest
@Requirement Tracing: -
@Design Tracing:      611877, 611878
*******************************************************************************/
TEST(VectorTest, GlobalComparisonOperatorsWork)
{
  ara::core::Vector<int> v1;
  ara::core::Vector<int> v2;

  ASSERT_THAT(v1, Eq(v2));

  ara::core::Vector<int> v3{1, 2, 3};

  ASSERT_THAT(v1, Ne(v3));
  /* @Start: UT_CORE_0181 */
  /* @TS1: Get the value of v1. */
  /* @TE1.1: Result should be equal to v2. */
  /* @TE1.2: Result should not be equal to v3. */
  /* @End: UT_CORE_0181 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0182
@Test_Description:    To test the functionality of swap overload works.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          VectorTest
@Requirement Tracing: -
@Design Tracing:      611883
*******************************************************************************/
TEST(VectorTest, SwapOverloadWorks)
{
  ara::core::Vector<int> v1{1, 2, 3};
  ara::core::Vector<int> v2{4, 5, 6};

  using std::swap;
  swap(v1, v2);

  ASSERT_THAT(v1, ElementsAre(4, 5, 6));
  ASSERT_THAT(v2, ElementsAre(1, 2, 3));
  /* @Start: UT_CORE_0182 */
  /* @TS1: Get the value of v1. */
  /* @TE1: Result should be equal to ElementsAre(4, 5, 6). */
  /* @TS2: Get the value of v2. */
  /* @TE2: Result should be equal to ElementsAre(1, 2, 3). */
  /* @End: UT_CORE_0182 */
}