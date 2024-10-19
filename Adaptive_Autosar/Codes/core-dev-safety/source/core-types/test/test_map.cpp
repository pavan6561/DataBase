// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_map.cpp
//
// Purpose     : This file provides the unit test cases of class Map.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/map.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>

using ::testing::Eq;
using ::testing::IsEmpty;
using ::testing::Ne;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;
/*******************************************************************************
@TestCaseId:          UT_CORE_00106
@Test_Description:    To test the functionality of TypeIsAvailableAndCompilable.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          MapTest
@Requirement Tracing: -
@Design Tracing:      611847 
*******************************************************************************/
TEST(MapTest, TypeIsAvailableAndCompilable)
{
  ara::core::Map<int, int> v1;
  /* @Start: UT_CORE_00106 */
  /* @TS1: Create an ara::core::Map<int, int> v1. */
  /* @TE1: Instance of map should be created. */
  /* @End: UT_CORE_00106 */
  SUCCEED();
}