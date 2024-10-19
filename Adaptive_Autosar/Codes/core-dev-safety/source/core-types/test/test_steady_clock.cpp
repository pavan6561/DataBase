// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_steady_clock.cpp
//
// Purpose     : This file provides the unit test cases of class SteadyClock.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/steady_clock.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <type_traits>
#include <cstdint>

using ::testing::Eq;
using ::testing::Not;
using ::testing::NotNull;
using ::testing::Ref;
/*******************************************************************************
@TestCaseId:          UT_CORE_00102
@Test_Description:    To test the functionality of Static Type Properties of SteadyClock.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SteadyClockTest
@Requirement Tracing: -
@Design Tracing:      611288
*******************************************************************************/
TEST(SteadyClockTest, StaticTypeProperties)
{
  /* @Start: UT_CORE_00102 */
  /* @TS1: Get the value of std::is_same<ara::core::SteadyClock::Rep, std::int64_t> of SteadyClock. */
  /* @TE1: std::is_same<ara::core::SteadyClock::Rep, std::int64_t> should be return true.  */
  /* @End: UT_CORE_00102 */
  constexpr bool is64BitInt = std::is_same<ara::core::SteadyClock::Rep, std::int64_t>::value;
  EXPECT_TRUE(is64BitInt);

  /* @Start: UT_CORE_00102 */
  /* @TS2: Get the value of std::is_same<ara::core::SteadyClock::Period, std::nano> of SteadyClock. */
  /* @TE2: std::is_same<ara::core::SteadyClock::Period, std::nano> should be return true.  */
  /* @End: UT_CORE_00102 */
  constexpr bool isNano = std::is_same<ara::core::SteadyClock::Period, std::nano>::value;
  EXPECT_TRUE(isNano);

  /* @Start: UT_CORE_00102 */
  /* @TS3: Get the value of ara::core::SteadyClock::stkisSteady of SteadyClock. */
  /* @TE3: ara::core::SteadyClock::stkisSteady should be return true.  */
  /* @End: UT_CORE_00102 */
  constexpr bool isSteady = ara::core::SteadyClock::stkisSteady;
  EXPECT_TRUE(isSteady);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_00103
@Test_Description:    To test the functionality of GetTimestamp of SteadyClock.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SteadyClockTest
@Requirement Tracing: -
@Design Tracing:      611289
*******************************************************************************/
TEST(SteadyClockTest, GetTimestamp)
{
  ara::core::SteadyClock::TimePoint tp = ara::core::SteadyClock::Now();
  static_cast<void>(tp);
  /* @Start: UT_CORE_00103 */
  /* @TS1: Invoke Now() of SteadyClock. */
  /* @TE1: ara::core::SteadyClock::Now() should not throw any exception. */
  /* @End: UT_CORE_00103 */
  // Nothing to check here, except that it compiles.
  SUCCEED();
}