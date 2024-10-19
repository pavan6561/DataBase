// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : timer_thread_test.cpp
//
// Purpose     : This file provides Unit Test cases for TimerThread class.
// -----------------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ara/core/timer/timer_thread.h"
using ara::core::timer::TimerThread;

namespace ara
{
namespace core
{
namespace timer
{
// @brief Supporting class for testing of Supervision class.
class TimerThreadTest : public ::testing::Test
{
 public:
  TimerThreadTest()
  {
  }
  bool handler_fun(bool flag)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    return (!flag);
  }
};

/*******************************************************************************
@TestCaseId:          UT_CORE_0901
@Test_Description:    Validate timer thread instance and constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611207, 611209, 611219, 611220, 611299, 611808
*******************************************************************************/
TEST_F(TimerThreadTest, Init)
{
  /* @Start: UT_CORE_0901 */
  /* @TS1: Create an instance of TimerThread class and check remaining time. */
  /* @TE1.1: Instance of TimerThread class should get created properly. */
  /* @End: UT_CORE_0901 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  static_cast<void>(timer);
  EXPECT_EQ(0, timer.GetRemainingTime(2));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0902
@Test_Description:    Validate single shot timer function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611223
*******************************************************************************/
TEST_F(TimerThreadTest, StartSingleShotTimer)
{
  /* @Start: UT_CORE_0902 */
  /* @TS1: Execute StartSingleShotTimer function twice with different timeout values. */
  /* @TE1.1: StartSingleShotTimer function should get executed properly with different timeout values. */
  /* @End: UT_CORE_0902 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  static_cast<void>(timer);
  std::size_t id1 = timer.StartSingleShotTimer(0, std::bind(&TimerThreadTest::handler_fun, this, true));
  std::size_t id = timer.StartSingleShotTimer(5000, std::bind(&TimerThreadTest::handler_fun, this, false));
  EXPECT_EQ(4999, timer.GetRemainingTime(id));
  EXPECT_EQ(0, timer.GetRemainingTime(id1));
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0903
@Test_Description:    Validate periodic timer function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611208, 611210, 611211, 611225
*******************************************************************************/
TEST_F(TimerThreadTest, StartPeriodicTimer)
{
  /* @Start: UT_CORE_0903 */
  /* @TS1: Execute StartPeriodicTimer function twice with different timeout values. */
  /* @TE1.1: StartPeriodicTimer function should get executed periodically with different timeout values. */
  /* @End: UT_CORE_0903 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  static_cast<void>(timer);
  std::size_t id1 = timer.StartPeriodicTimer(0, std::bind(&TimerThreadTest::handler_fun, this, true));
  std::size_t id = timer.StartPeriodicTimer(5000, std::bind(&TimerThreadTest::handler_fun, this, false));
  EXPECT_EQ(4999, timer.GetRemainingTime(id));
  EXPECT_EQ(0, timer.GetRemainingTime(id1));
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0904
@Test_Description:    Validate periodic StopTimer function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611213, 611217, 611227
*******************************************************************************/
TEST_F(TimerThreadTest, StopTimer)
{
  /* @Start: UT_CORE_0904 */
  /* @TS1: Create an instance of TimerThread class and invoke StopTimer. */
  /* @TE1.1: StopTimer() should stop the mentioned timer. */
  /* @End: UT_CORE_0904 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  std::size_t id = timer.StartPeriodicTimer(5000, std::bind(&TimerThreadTest::handler_fun, this, false));
  EXPECT_EQ(4999, timer.GetRemainingTime(id));
  timer.StopTimer(id);
  EXPECT_EQ(0, timer.GetRemainingTime(id));
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0905
@Test_Description:    Validate GetHeadQueueTimerId function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611227
*******************************************************************************/
TEST_F(TimerThreadTest, GetHeadQueueTimerId)
{
  /* @Start: UT_CORE_0905 */
  /* @TS1: Create an instance of TimerThread class and invoke GetHeadQueueTimerId(). */
  /* @TE1.1: GetHeadQueueTimerId() should return the first timer id from the queue. */
  /* @End: UT_CORE_0905 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  std::size_t id = timer.StartPeriodicTimer(5000, std::bind(&TimerThreadTest::handler_fun, this, false));
  EXPECT_EQ(4999, timer.GetRemainingTime(id));
  EXPECT_EQ(id, timer.GetHeadQueueTimerId());
  timer.StopTimer(id);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0906
@Test_Description:    Validate single shot timer function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611249, 611253, 614041
*******************************************************************************/
TEST_F(TimerThreadTest, StartSingleShotTimerWithInvalidTimer)
{
  /* @Start: UT_CORE_0902 */
  /* @TS1: Execute StartSingleShotTimer function twice with different timeout values. */
  /* @TE1.1: StartSingleShotTimer function should get executed properly with different timeout values. */
  /* @End: UT_CORE_0902 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  static_cast<void>(timer);
  ara::core::Result<TimerId> id1
      = timer.StartSingleShotTimerWithResult(5000, std::bind(&TimerThreadTest::handler_fun, this, true));
  ara::core::Result<TimerId> id2
      = timer.StartSingleShotTimerWithResult(0, std::bind(&TimerThreadTest::handler_fun, this, true));
  EXPECT_EQ(4999, timer.GetRemainingTimeWithResult(id1.Value()).Value());
  EXPECT_EQ(id2.Error(), ara::core::TimerErrc::kInvalidReqRequestTimer);
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0907
@Test_Description:    Validate single shot timer function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611250
*******************************************************************************/
TEST_F(TimerThreadTest, StartSingleShotTimerWithInvalidHandler)
{
  /* @Start: UT_CORE_0902 */
  /* @TS1: Execute StartSingleShotTimer function twice with different timeout values. */
  /* @TE1.1: StartSingleShotTimer function should get executed properly with different timeout values. */
  /* @End: UT_CORE_0902 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  static_cast<void>(timer);
  ara::core::Result<TimerId> id = timer.StartSingleShotTimerWithResult(5000, nullptr);
  EXPECT_EQ(id.Error(), ara::core::TimerErrc::kInvalidHandler);
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0908
@Test_Description:    Validate periodic Stop Timer function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611251, 611252
*******************************************************************************/
TEST_F(TimerThreadTest, GetRemainingTimeWithInvalidId)
{
  /* @Start: UT_CORE_0904 */
  /* @TS1: Create an instance of TimerThread class and invoke StopTimer. */
  /* @TE1.1: StopTimer() should stop the mentioned timer. */
  /* @End: UT_CORE_0904 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  std::size_t id = timer.StartPeriodicTimer(5000, std::bind(&TimerThreadTest::handler_fun, this, false));
  EXPECT_EQ(4999, timer.GetRemainingTimeWithResult(id).Value());
  timer.StopTimerWithResult(id);
  EXPECT_EQ(ara::core::TimerErrc::kInvalidId, (timer.GetRemainingTimeWithResult(id)).Error());
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0909
@Test_Description:    Validate periodic StopTimer function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611251, 611253
*******************************************************************************/
TEST_F(TimerThreadTest, StopTimereWithInvalidId)
{
  /* @Start: UT_CORE_0904 */
  /* @TS1: Create an instance of TimerThread class and invoke StopTimer. */
  /* @TE1.1: StopTimer() should stop the mentioned timer. */
  /* @End: UT_CORE_0904 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  std::size_t id = timer.StartPeriodicTimer(5000, std::bind(&TimerThreadTest::handler_fun, this, false));
  EXPECT_EQ(4999, timer.GetRemainingTimeWithResult(id).Value());
  timer.StopTimerWithResult(id);
  EXPECT_EQ(ara::core::TimerErrc::kInvalidId, (timer.StopTimerWithResult(id)).Error());
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0910
@Test_Description:    Validate single start periodic function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611251, 611253
*******************************************************************************/
TEST_F(TimerThreadTest, StartPeriodicTimerWithInvalidTimer)
{
  /* @Start: UT_CORE_0902 */
  /* @TS1: Execute StartSingleShotTimer function twice with different timeout values. */
  /* @TE1.1: StartSingleShotTimer function should get executed properly with different timeout values. */
  /* @End: UT_CORE_0902 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  static_cast<void>(timer);
  ara::core::Result<TimerId> id1
      = timer.StartPeriodicTimerWithResult(5000, std::bind(&TimerThreadTest::handler_fun, this, true));
  ara::core::Result<TimerId> id2
      = timer.StartPeriodicTimerWithResult(0, std::bind(&TimerThreadTest::handler_fun, this, true));
  EXPECT_EQ(4999, timer.GetRemainingTimeWithResult(id1.Value()).Value());
  EXPECT_EQ(id2.Error(), ara::core::TimerErrc::kInvalidReqRequestTimer);
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0911
@Test_Description:    Validate start periodic timer function of timer thread class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          TimerThreadTest
@Requirement Tracing: -
@Design Tracing:      611249
*******************************************************************************/
TEST_F(TimerThreadTest, StartPeriodicTimerWithInvalidHandler)
{
  /* @Start: UT_CORE_0902 */
  /* @TS1: Execute StartSingleShotTimer function twice with different timeout values. */
  /* @TE1.1: StartSingleShotTimer function should get executed properly with different timeout values. */
  /* @End: UT_CORE_0902 */
  TimerThread& timer{TimerThread::GetTimerThreadInstance()};
  static_cast<void>(timer);
  ara::core::Result<TimerId> id = timer.StartPeriodicTimerWithResult(5000, nullptr);
  EXPECT_EQ(id.Error(), ara::core::TimerErrc::kInvalidHandler);
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
}
} // namespace timer
} // namespace core
} // namespace ara