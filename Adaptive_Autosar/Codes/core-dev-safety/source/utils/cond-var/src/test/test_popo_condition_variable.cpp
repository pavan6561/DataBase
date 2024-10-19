// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#include "barrier.hpp"
#include "timing_test.hpp"
#include "watch_dog.hpp"
#include "condition_listener.h"
#include "condition_notifier.h"
#include "condition_variable_data.h"
#include "test.hpp"

#include <atomic>
#include <memory>
#include <thread>
#include <type_traits>

namespace ara
{
namespace core
{
using namespace ::testing;
//using namespace iox::popo;
using namespace iox::cxx;
using namespace ara::core;
using namespace iox::units::duration_literals;

class ConditionVariable_test : public Test
{
 public:
  using NotificationVector_t = ConditionListener::NotificationVectorT;
  using Type_t = iox::cxx::BestFittingType_t<ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER>;
  const ara::RuntimeNameT m_runtimeName{"Ferdinand"};
  const iox::units::Duration m_timeToWait = 2_s;
  const iox::units::Duration m_timingTestTime = 100_ms;

  ConditionVariableData m_condVarData{m_runtimeName};
  ConditionListener m_waiter{m_condVarData};
  ConditionNotifier m_signaler{m_condVarData, 0U};
  vector<ConditionNotifier, ara::MAX_NUMBER_OF_NOTIFIERS> m_notifiers;

  void SetUp() override
  {
    for (uint64_t i = 0U; i < ara::MAX_NUMBER_OF_NOTIFIERS; ++i)
    {
      m_notifiers.emplace_back(m_condVarData, i);
    }
    m_watchdog.watchAndActOnFailure([&] { std::terminate(); });
  }

  Watchdog m_watchdog{m_timeToWait};
};

/*******************************************************************************
@TestCaseId:          UT_CORE_0801
@Test_Description:    Test for Conditional listener.
@Test_Type:           Interface Test.
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611236, 611238, 611239, 611240
*******************************************************************************/
TEST_F(ConditionVariable_test, ConditionListenerIsNeitherCopyNorMovable)
{
  ::testing::Test::RecordProperty("TEST_ID", "2105fbcf-ed66-4042-aae3-46c2bb82a63c");
  /* @Start: UT_CORE_0801 */
  /* @TS1: Test ConditionalListener by taking a dummy value.*/
  /* @TE1.1: Copy constructor when invoked should return false boolean value. */
  /* @End: UT_CORE_0801 */
  EXPECT_FALSE(std::is_copy_constructible<ConditionListener>::value);
  /* @Start: UT_CORE_0801 */
  /* @TS2: Test ConditionalListener by taking a dummy value.*/
  /* @TE2.1: Move constructor when invoked should return false boolean value. */
  /* @End: UT_CORE_0801 */
  EXPECT_FALSE(std::is_move_constructible<ConditionListener>::value);
  /* @Start: UT_CORE_0801 */
  /* @TS3: Test ConditionalListener by taking a dummy value.*/
  /* @TE3.1: Copy assignable operator when invoked should return false boolean value. */
  /* @End: UT_CORE_0801 */
  EXPECT_FALSE(std::is_copy_assignable<ConditionListener>::value);
  /* @Start: UT_CORE_0801 */
  /* @TS4: Test ConditionalListener by taking a dummy value.*/
  /* @TE4.1: Move assignable operator when invoked should return false boolean value. */
  /* @End: UT_CORE_0801 */
  EXPECT_FALSE(std::is_move_assignable<ConditionListener>::value);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0802
@Test_Description:    Test for Condition Notifier.
@Test_Type:           Interface Test
@Test_Configuration:  For this test dummy condition Notifier should be configured.
@Pre-Condition:       - 
@Post-Condition:      No changes.
@Comments:            - 
@Test_Group:          ConditionVariable test.
@Design Tracing:      611231, 611232
*******************************************************************************/

TEST_F(ConditionVariable_test, ConditionNotifierIsNeitherCopyNorMovable)
{
  ::testing::Test::RecordProperty("TEST_ID", "51b971ea-2fb1-4280-8663-6f86c70ee06a");
  /* @Start: UT_CORE_0802 */
  /* @TS1: Test ConditionNotifier by taking a dummy value.*/
  /* @TE1.1: Copy constructor when invoked should return false boolean value. */
  /* @End: UT_CORE_0802 */
  EXPECT_FALSE(std::is_copy_constructible<ConditionNotifier>::value);
  /* @Start: UT_CORE_0802 */
  /* @TS2: Test ConditionNotifier by taking a dummy value.*/
  /* @TE2.1: Move constructor when invoked should return false boolean value. */
  /* @End: UT_CORE_0802 */
  EXPECT_FALSE(std::is_move_constructible<ConditionNotifier>::value);
  /* @Start: UT_CORE_0802 */
  /* @TS3: Test ConditionNotifier by taking a dummy value.*/
  /* @TE3.1: Copy assignable operator when invoked should return false boolean value. */
  /* @End: UT_CORE_0802 */
  EXPECT_FALSE(std::is_copy_assignable<ConditionNotifier>::value);
  /* @Start: UT_CORE_0802 */
  /* @TS4: Test ConditionNotifier by taking a dummy value.*/
  /* @TE4.1: Copy assignable operator when invoked should return false boolean value. */
  /* @End: UT_CORE_0802 */
  EXPECT_FALSE(std::is_move_assignable<ConditionNotifier>::value);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0803
@Test_Description:    Test for WasNotified() method of the condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       - 
@Post-Condition:      No changes.
@Comments:            - 
@Test_Group:          ConditionVariable test.
@Design Tracing:      611238, 611233
*******************************************************************************/
TEST_F(ConditionVariable_test, NotifyOnceResultsInBeingTriggered)
{
  ::testing::Test::RecordProperty("TEST_ID", "372125d2-82b4-4729-bc93-661578af4739");
  m_signaler.notify();
  /* @Start: UT_CORE_0803 */
  /* @TS1: Invoke WasNotified() of condition variable class.*/
  /* @TE1.1: WasNotified should return a true boolean value when invoked. */
  /* @End: UT_CORE_0803 */
  EXPECT_TRUE(m_waiter.WasNotified());
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0804
@Test_Description:    Test for WasNotified method of the condition variable class.
@Test_Type:             Interface Test
@Test_Configuration:  -
@Pre-Condition:       - 
@Post-Condition:      - No changes.
@Comments:            - 
@Test_Group:         ConditionVariable test.
@Design Tracing:     611238, 611233
*******************************************************************************/
TEST_F(ConditionVariable_test, NoNotifyResultsInNotBeingTriggered)
{
  ::testing::Test::RecordProperty("TEST_ID", "abe8a485-63d3-486a-b62a-94648b7f7954");
  /* @Start: UT_CORE_0804 */
  /* @TS1: Invoke WasNotified() of condition variable class.*/
  /* @TE1.1: WasNotified when invoked should return false boolean value. */
  /* @End: UT_CORE_0804 */
  EXPECT_FALSE(m_waiter.WasNotified());
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0805
@Test_Description:    Test for Wait() method of the condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       - 
@Post-Condition:      No changes.
@Comments:            - 
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233 
*******************************************************************************/
TEST_F(ConditionVariable_test, WaitResetsAllNotificationsInWait)
{
  ::testing::Test::RecordProperty("TEST_ID", "ebc9c42a-14e7-471c-a9df-9c5641b5767d");
  m_signaler.notify();
  m_signaler.notify();
  m_signaler.notify();
  m_waiter.wait();

  std::atomic_bool isThreadFinished{false};
  std::thread t([&] {
    m_waiter.wait();
    isThreadFinished = true;
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  /* @Start: UT_CORE_0805 */
  /* @TS1: Invoke load().*/
  /* @TE1.1: load() should return isThreadFinished() with false boolean value. */
  /* @End: UT_CORE_0805 */
  EXPECT_FALSE(isThreadFinished.load());
  m_signaler.notify();
  t.join();
  /* @Start: UT_CORE_0805 */
  /* @TS2: Invoke load().*/
  /* @TE2.1: load() should return a isThreadFinished() with true boolean value. */
  /* @End: UT_CORE_0805 */
  EXPECT_TRUE(isThreadFinished.load());
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0806
@Test_Description:    Test for wait and notify results is immediately triggered when multiThreaded.
@Test_Type:           Interface Test
@Test_Configuration:  For this test dummy conditional listener should be configured.
@Pre-Condition:       - 
@Post-Condition:      No changes.
@Comments:            - 
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TEST_F(ConditionVariable_test, WaitAndNotifyResultsInImmediateTriggerMultiThreaded)
{
  ::testing::Test::RecordProperty("TEST_ID", "39b40c73-3dcc-4af6-9682-b62816c69854");
  std::atomic<int> counter{0};
  Barrier isThreadStarted(1U);
  std::thread waiter([&] {
    /* @Start: UT_CORE_0806 */
    /* @TS1: Invoke Eq().*/
    /* @TE1.1: Eq() should return the counter value as 0. */
    /* @End: UT_CORE_0806 */
    EXPECT_THAT(counter, Eq(0));
    isThreadStarted.notify();
    m_waiter.wait();
    /* @Start: UT_CORE_0806 */
    /* @TS2: Invoke Eq().*/
    /* @TE2.1: Eq() should return the counter value as 1. */
    /* @End: UT_CORE_0806 */
    EXPECT_THAT(counter, Eq(1));
  });
  isThreadStarted.wait();
  counter++;
  m_signaler.notify();
  waiter.join();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0807
@Test_Description:    To check if all notifications after construction are false.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       - 
@Post-Condition:      No changes.
@Comments:            - 
@Test_Group:          ConditionVariable test.
@Design Tracing:      611237
*******************************************************************************/
TEST_F(ConditionVariable_test, AllNotificationsAreFalseAfterConstruction)
{
  ::testing::Test::RecordProperty("TEST_ID", "4e5f6dbc-84cc-468a-9d64-f5ed88012ebc");
  ConditionVariableData sut;
  for (auto& notification : sut.mActiveNotifications)
  {
    /* @Start: UT_CORE_0807 */
    /* @TS1: Invoke Eq().*/
    /* @TE1.1: When invoked Eq() should return notification value as false. */
    /* @End: UT_CORE_0807 */
    EXPECT_THAT(notification, Eq(false));
  }
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0809
@Test_Description:    Test to check if Runtime after construction is correct.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611237
*******************************************************************************/
TEST_F(ConditionVariable_test, CorrectRuntimeNameAfterConstructionWithRuntimeName)
{
  ::testing::Test::RecordProperty("TEST_ID", "acc65071-09ec-40ce-82b4-74964525fabf");
  /* @Start: UT_CORE_0809 */
  /* @TS1: Invoke StrEq().*/
  /* @TE1.1: When invoked method should return the expected runtime value. */
  /* @End: UT_CORE_0809 */
  EXPECT_THAT(m_condVarData.mRuntimeName.c_str(), StrEq(m_runtimeName));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0810
@Test_Description:    Test to check if all notification are false after construction with runtime.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611237
*******************************************************************************/
TEST_F(ConditionVariable_test, AllNotificationsAreFalseAfterConstructionWithRuntimeName)
{
  ::testing::Test::RecordProperty("TEST_ID", "4825e152-08e3-414e-a34f-d93d048f84b8");
  for (auto& notification : m_condVarData.mActiveNotifications)
  {
    /* @Start: UT_CORE_0810 */
    /* @TS1: Invoke Eq().*/
    /* @TE1.1: When invoked method should return the expected(false) boolean value. */
    /* @End: UT_CORE_0810 */
    EXPECT_THAT(notification, Eq(false));
  }
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0811
@Test_Description:    Test to check if notifiy() activates correct index.
@Test_Type:           Interface Test
@Test_Configuration:  For this test dummy conditional listener and EVENT_INDEX
                      values should be configured.
@Pre-Condition:       - 
@Post-Condition:      No changes.
@Comments:            - 
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TEST_F(ConditionVariable_test, NotifyActivatesCorrectIndex)
{
  ::testing::Test::RecordProperty("TEST_ID", "2c372bcc-7e91-47c1-8ab9-ccd5be048562");
  constexpr Type_t EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 1U;
  ConditionNotifier sut(m_condVarData, EVENT_INDEX);
  sut.notify();
  for (Type_t i = 0U; i < ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER; i++)
  {
    if (i == EVENT_INDEX)
    {
      /* @Start: UT_CORE_0811 */
      /* @TS1: Invoke Eq().*/
      /* @TE1.1: When invoked method should return the true boolean value. */
      /* @End: UT_CORE_0811 */
      EXPECT_THAT(m_condVarData.mActiveNotifications[i], Eq(true));
    }
    else
    {
      /* @Start: UT_CORE_0811 */
      /* @TS2: Invoke .*/
      /* @TE2.1: When invoked method should return the false boolean value. */
      /* @End: UT_CORE_0811 */
      EXPECT_THAT(m_condVarData.mActiveNotifications[i], Eq(false));
    }
  }
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0812
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611237
*******************************************************************************/
TEST_F(ConditionVariable_test, TimedWaitWithZeroTimeoutWorks)
{
  ::testing::Test::RecordProperty("TEST_ID", "582f0b1c-c717-410e-8143-61459db672ad");
  ConditionListener sut(m_condVarData);
  /* @Start: UT_CORE_0812 */
  /* @TS1: Invoke TimedWait() with zero timeout value.*/
  /* @TE1.1: TimedWait should return true boolean value. */
  /* @End: UT_CORE_0812 */
  EXPECT_TRUE(sut.TimedWait(iox::units::Duration::fromSeconds(0)).empty());
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0813
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          -ConditionVariable test.
@Design Tracing:      611237 
*******************************************************************************/
TEST_F(ConditionVariable_test, TimedWaitWithoutNotificationReturnsEmptyVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "15aaf499-9731-4c53-88f3-88af4983eae0");
  ConditionListener sut(m_condVarData);
  /* @Start: UT_CORE_0813 */
  /* @TS1: Invoke TimedWait() with time value of 100 milliseconds.*/
  /* @TE1.1: TimedWait should return true boolean vaule  */
  /* @End: UT_CORE_0813 */
  EXPECT_TRUE(sut.TimedWait(iox::units::Duration::fromMilliseconds(100)).empty());
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0814
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233 
*******************************************************************************/
TEST_F(ConditionVariable_test, TimedWaitReturnsOneNotifiedIndex)
{
  ::testing::Test::RecordProperty("TEST_ID", "bf9ed236-bba9-43cd-84b2-6769d7f47d50");
  ConditionListener sut(m_condVarData);
  ConditionNotifier(m_condVarData, 6U).notify();

  auto indices = sut.TimedWait(iox::units::Duration::fromMilliseconds(100));
  /* @Start: UT_CORE_0814 */
  /* @TS1: Invoke TimedWait().*/
  /* @TE1.1: When TimedWait() is invoked, it should return indices size as 1U. */
  /* @End: UT_CORE_0814 */
  ASSERT_THAT(indices.size(), Eq(1U));
  EXPECT_THAT(indices[0U], Eq(6U));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0815
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TEST_F(ConditionVariable_test, TimedWaitReturnsMultipleNotifiedIndices)
{
  ::testing::Test::RecordProperty("TEST_ID", "771c2c11-effb-435a-9c67-a7d9471fdb6e");
  ConditionListener sut(m_condVarData);
  ConditionNotifier(m_condVarData, 5U).notify();
  ConditionNotifier(m_condVarData, 15U).notify();

  auto indices = sut.TimedWait(iox::units::Duration::fromMilliseconds(100));

  /* @Start: UT_CORE_0815 */
  /* @TS1: Invoke TimedWait().*/
  /* @TE1.1: TimedWait() when invoked should return the indices size as 2U. */
  /* @End: UT_CORE_0815 */
  ASSERT_THAT(indices.size(), Eq(2U));
  EXPECT_THAT(indices[0U], Eq(5U));
  EXPECT_THAT(indices[1U], Eq(15U));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0816
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TEST_F(ConditionVariable_test, TimedWaitReturnsAllNotifiedIndices)
{
  ::testing::Test::RecordProperty("TEST_ID", "38ee654b-228a-4462-9614-2901cb5272aa");
  ConditionListener sut(m_condVarData);
  for (uint64_t i = 0U; i < ara::MAX_NUMBER_OF_NOTIFIERS; ++i)
  {
    ConditionNotifier(m_condVarData, i).notify();
  }

  auto indices = sut.TimedWait(iox::units::Duration::fromMilliseconds(100));
  /* @Start: UT_CORE_0816 */
  /* @TS1: Invoke TimedWait().*/
  /* @TE1.1: MAX_NUMBER_OF_NOTIFIERS should return as the indeces size when method is invoked. */
  /* @End: UT_CORE_0816 */
  ASSERT_THAT(indices.size(), Eq(ara::MAX_NUMBER_OF_NOTIFIERS));
  for (uint64_t i = 0U; i < ara::MAX_NUMBER_OF_NOTIFIERS; ++i)
  {
    EXPECT_THAT(indices[i], Eq(i));
  }
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0817
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TIMING_TEST_F(ConditionVariable_test, TimedWaitBlocksUntilTimeout, Repeat(5), [&] {
  ::testing::Test::RecordProperty("TEST_ID", "c755aec9-43c3-4bf4-bec4-5672c76561ef");
  ConditionListener listener(m_condVarData);
  NotificationVector_t activeNotifications;
  std::atomic_bool hasWaited{false};

  std::thread waiter([&] {
    activeNotifications = listener.TimedWait(m_timingTestTime);
    hasWaited.store(true, std::memory_order_relaxed);
    /* @Start: UT_CORE_0817 */
    /* @TS1: Invoke TimedWait() */
    /* @TE1.1: TimedWait() should return activeNotifications size as 0. */
    /* @End: UT_CORE_0817 */
    ASSERT_THAT(activeNotifications.size(), Eq(0U));
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(2 * m_timingTestTime.toMilliseconds() / 3));
  EXPECT_THAT(hasWaited, Eq(false));
  std::this_thread::sleep_for(std::chrono::milliseconds(2 * m_timingTestTime.toMilliseconds() / 3));
  EXPECT_THAT(hasWaited, Eq(true));
  waiter.join();
})

/*******************************************************************************
@TestCaseId:          UT_CORE_0818
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233 
*******************************************************************************/
TIMING_TEST_F(ConditionVariable_test, TimedWaitBlocksUntilNotification, Repeat(5), [&] {
  ::testing::Test::RecordProperty("TEST_ID", "b2999ddd-d072-4c9f-975e-fc8acc31397d");
  ConditionListener listener(m_condVarData);
  NotificationVector_t activeNotifications;
  std::atomic_bool hasWaited{false};

  std::thread waiter([&] {
    activeNotifications = listener.TimedWait(m_timingTestTime);
    hasWaited.store(true, std::memory_order_relaxed);
    /* @Start: UT_CORE_0818 */
    /* @TS1: Invoke TimedWait() */
    /* @TE1.1: TimedWait() when invoked should return value 13 as size of activeNotifications. */
    /* @End: UT_CORE_0818 */
    ASSERT_THAT(activeNotifications.size(), Eq(1U));
    EXPECT_THAT(activeNotifications[0], 13U);
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(m_timingTestTime.toMilliseconds() / 4));
  EXPECT_THAT(hasWaited, Eq(false));
  ConditionNotifier(m_condVarData, 13U).notify();
  std::this_thread::sleep_for(std::chrono::milliseconds(m_timingTestTime.toMilliseconds() / 4));
  EXPECT_THAT(hasWaited, Eq(true));
  waiter.join();
})

/*******************************************************************************
@TestCaseId:          UT_CORE_0819
@Test_Description:    Test to check if wait() is non blocking.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611240, 611241, 611242, 611243
*******************************************************************************/
TEST_F(ConditionVariable_test, WaitIsNonBlockingAfterDestroyAndReturnsEmptyVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "39bd43c0-c310-4f42-8baa-6873fbbbe705");
  ConditionListener sut(m_condVarData);
  sut.Destroy();
  const auto& activeNotifications = sut.wait();
  /* @Start: UT_CORE_0819 */
  /* @TS1: Invoke wait() */
  /* @TE1.1: Method should return the expected value 0  as activeNotifications size. */
  /* @End: UT_CORE_0819 */
  EXPECT_THAT(activeNotifications.size(), Eq(0U));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0820
@Test_Description:    Test to check if wait() is non blocking and notify().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233 
*******************************************************************************/
TEST_F(ConditionVariable_test, WaitIsNonBlockingAfterDestroyAndNotifyAndReturnsEmptyVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "b803fc3e-f3a6-405c-86a0-ecedc06d0c05");
  ConditionListener sut(m_condVarData);
  sut.Destroy();
  ConditionNotifier notifier(m_condVarData, 0U);
  notifier.notify();
  const auto& activeNotifications = sut.wait();
  /* @Start: UT_CORE_0820 */
  /* @TS1: Invoke wait() */
  /* @TE1.1: wait should return 0 as the activeNotifications size*/
  /* @End: UT_CORE_0820 */
  EXPECT_THAT(activeNotifications.size(), Eq(0U));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0821
@Test_Description:    Test to check if empty vector returns after invoking Destroy().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611240, 611241, 611242, 611243 
*******************************************************************************/
TEST_F(ConditionVariable_test, DestroyWakesUpWaitWhichReturnsEmptyVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "ed0e434c-6efd-4218-88a8-9332e33f92fd");
  ConditionListener sut(m_condVarData);

  NotificationVector_t activeNotifications;

  std::thread waiter([&] {
    activeNotifications = sut.wait();
    /* @Start: UT_CORE_0821 */
    /* @TS1: Invoke wait() */
    /* @TE1.1: wait should return 0 as the size of active notifications. */
    /* @End: UT_CORE_0821 */
    EXPECT_THAT(activeNotifications.size(), Eq(0U));
  });
  sut.Destroy();
  waiter.join();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0822
@Test_Description:    Test to GetCorrectNotificationVector after Notify() And Wait().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TEST_F(ConditionVariable_test, GetCorrectNotificationVectorAfterNotifyAndWait)
{
  ::testing::Test::RecordProperty("TEST_ID", "41a25c52-a358-4e94-b4a5-f315fb5124cd");
  constexpr Type_t EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 1U;
  ConditionNotifier notifier(m_condVarData, EVENT_INDEX);
  ConditionListener listener(m_condVarData);
  notifier.notify();
  const auto& activeNotifications = listener.wait();
  /* @Start: UT_CORE_0822*/
  /* @TS1: Invoke wait()*/
  /* @TE1.1: wait() should return the value of 1U for the default activeNotifications size.*/
  /* @End: UT_CORE_0822 */
  ASSERT_THAT(activeNotifications.size(), Eq(1U));
  EXPECT_THAT(activeNotifications[0], Eq(EVENT_INDEX));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0823
@Test_Description:    Test to GetCorrectNotificationVector after Multiple Notify() and Wait().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TEST_F(ConditionVariable_test, GetCorrectNotificationVectorAfterMultipleNotifyAndWait)
{
  ::testing::Test::RecordProperty("TEST_ID", "5b09bb18-e6c7-42cb-bb34-2da0dd26ca06");
  constexpr Type_t FIRST_EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 1U;
  constexpr Type_t SECOND_EVENT_INDEX = 0U;
  ConditionNotifier notifier1(m_condVarData, FIRST_EVENT_INDEX);
  ConditionNotifier notifier2(m_condVarData, SECOND_EVENT_INDEX);
  ConditionListener listener(m_condVarData);
  notifier1.notify();
  notifier2.notify();
  const auto& activeNotifications = listener.wait();
  /* @Start: UT_CORE_0823*/
  /* @TS1: Invoke wait()*/
  /* @TE1.1: wait() should return the value of 2U for the default activeNotifications size.*/
  /* @End: UT_CORE_0823 */
  ASSERT_THAT(activeNotifications.size(), Eq(2U));
  EXPECT_THAT(activeNotifications[0], Eq(SECOND_EVENT_INDEX));
  EXPECT_THAT(activeNotifications[1], Eq(FIRST_EVENT_INDEX));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0824
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611244, 611245
*******************************************************************************/
TEST_F(ConditionVariable_test, WaitAndNotifyResultsInCorrectNotificationVector)
{
  ::testing::Test::RecordProperty("TEST_ID", "4cac0ad0-083b-43dd-867e-dd6abb0291e8");
  constexpr Type_t EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 5U;
  ConditionNotifier notifier(m_condVarData, EVENT_INDEX);
  ConditionListener listener(m_condVarData);
  NotificationVector_t activeNotifications;
  std::thread waiter([&] {
    activeNotifications = listener.wait();
    /* @Start: UT_CORE_0824 */
    /* @TS1: Invoke wait() with zero activeNotification*/
    /* @TE1.1: activeNotification should return the expected value of 1U*/
    /* @End: UT_CORE_0824 */
    ASSERT_THAT(activeNotifications.size(), Eq(1U));
    EXPECT_THAT(activeNotifications[0], Eq(EVENT_INDEX));
  });
  notifier.notify();
  waiter.join();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0825
@Test_Description:    Test to check if wait blocks repeats.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TIMING_TEST_F(ConditionVariable_test, WaitBlocks, Repeat(5), [&] {
  ::testing::Test::RecordProperty("TEST_ID", "09d9ad43-ba97-4331-9a6b-ca22d2dbddb8");
  constexpr Type_t EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 5U;
  ConditionNotifier notifier(m_condVarData, EVENT_INDEX);
  ConditionListener listener(m_condVarData);
  NotificationVector_t activeNotifications;
  Barrier isThreadStarted(1U);
  std::atomic_bool hasWaited{false};
  std::thread waiter([&] {
    isThreadStarted.notify();
    activeNotifications = listener.wait();
    hasWaited.store(true, std::memory_order_relaxed);
    /* @Start: UT_CORE_0825 */
    /* @TS1: Invoke wait() */
    /* @TE1.1: Method should return the expected EVENT_INDEX value of 1U*/
    /* @End: UT_CORE_0825 */
    ASSERT_THAT(activeNotifications.size(), Eq(1U));
    EXPECT_THAT(activeNotifications[0], Eq(EVENT_INDEX));
  });
  isThreadStarted.wait();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(false));
  notifier.notify();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(true));
  waiter.join();
})

/*******************************************************************************
@TestCaseId:          UT_CORE_0826
@Test_Description:    Test to check if second wait blocks until new notification.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TIMING_TEST_F(ConditionVariable_test, SecondWaitBlocksUntilNewNotification, Repeat(5), [&] {
  ::testing::Test::RecordProperty("TEST_ID", "dcbd55ee-e401-42cb-bbf2-a266058c2e76");
  constexpr Type_t FIRST_EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 2U;
  constexpr Type_t SECOND_EVENT_INDEX = 0U;
  ConditionNotifier notifier1(m_condVarData, FIRST_EVENT_INDEX);
  ConditionNotifier notifier2(m_condVarData, SECOND_EVENT_INDEX);
  ConditionListener listener(m_condVarData);
  std::atomic_bool hasWaited{false};
  Watchdog watchdogFirstWait(m_timeToWait);
  watchdogFirstWait.watchAndActOnFailure([&] { listener.Destroy(); });
  notifier1.notify();
  notifier2.notify();
  NotificationVector_t activeNotifications = listener.wait();
  /* @Start: UT_CORE_0826 */
  /* @TS1: Invoke wait() with activeNotification */
  /* @TE1.1: Method should return the 2U as the activeNotifications size. */
  /* @End: UT_CORE_0826 */
  ASSERT_THAT(activeNotifications.size(), Eq(2U));
  EXPECT_THAT(activeNotifications[0], Eq(SECOND_EVENT_INDEX));
  EXPECT_THAT(activeNotifications[1], Eq(FIRST_EVENT_INDEX));
  Watchdog watchdogSecondWait(m_timeToWait);
  watchdogSecondWait.watchAndActOnFailure([&] { listener.Destroy(); });
  Barrier isThreadStarted(1U);
  std::thread waiter([&] {
    isThreadStarted.notify();
    activeNotifications = listener.wait();
    hasWaited.store(true, std::memory_order_relaxed);
    /* @Start: UT_CORE_0826 */
    /* @TS2: Invoke wait() with activeNotification */
    /* @TE2.1: Method should return activeNotifications size as 1U */
    /* @End: UT_CORE_0826 */
    ASSERT_THAT(activeNotifications.size(), Eq(1U));
    EXPECT_THAT(activeNotifications[0], Eq(FIRST_EVENT_INDEX));
    for (const auto& notification : m_condVarData.mActiveNotifications)
    {
      EXPECT_THAT(notification, Eq(false));
    }
  });
  isThreadStarted.wait();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(false));
  notifier1.notify();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(true));
  waiter.join();
})
void waitReturnsSortedListWhenTriggeredInOrder(
    ConditionVariable_test& test, const function_ref<ConditionListener::NotificationVectorT()>& wait)
{
  for (uint64_t i = 0U; i < test.m_notifiers.size(); ++i)
  {
    test.m_notifiers[i].notify();
  }

  auto notifications = wait();
  ASSERT_THAT(notifications.size(), Eq(test.m_notifiers.size()));
  for (uint64_t i = 0U; i < test.m_notifiers.size(); ++i)
  {
    EXPECT_THAT(notifications[i], Eq(i));
  }
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0827
@Test_Description:    Test to verify if Wait() returns sorted list when triggered in order.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      61244, 611245, 611241, 611242, 611243 
*******************************************************************************/
TEST_F(ConditionVariable_test, WaitReturnsSortedListWhenTriggeredInOrder)
{
  ::testing::Test::RecordProperty("TEST_ID", "d9cfc71a--41f8-b66f-486bdf5d27bc");
  /* @Start: UT_CORE_0827*/
  /* @TS1: Invoke wait()*/
  /* @TE1.1: Method should return the sorted list of time values.*/
  /* @End: UT_CORE_0827 */
  waitReturnsSortedListWhenTriggeredInOrder(*this, [this] { return m_waiter.wait(); });
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0828
@Test_Description:    Test to verify if Wait() returns sorted list when triggered in order.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611244, 611245, 611233
*******************************************************************************/
TEST_F(ConditionVariable_test, TimedWaitReturnsSortedListWhenTriggeredInOrder)
{
  ::testing::Test::RecordProperty("TEST_ID", "e9f875f6-c8ff-4c9c-aafa-78f7c0942bba");
  waitReturnsSortedListWhenTriggeredInOrder(
      *this, [this] { return m_waiter.TimedWait(iox::units::Duration::fromSeconds(1)); });
}
void waitReturnsSortedListWhenTriggeredInReverseOrder(
    ConditionVariable_test& test, const function_ref<ConditionListener::NotificationVectorT()>& wait)
{
  for (uint64_t i = 0U; i < test.m_notifiers.size(); ++i)
  {
    test.m_notifiers[test.m_notifiers.size() - i - 1U].notify();
  }
  auto notifications = wait();
  /* @Start: UT_CORE_0828*/
  /* @TS1: Invoke TimedWait()*/
  /* @TE1.1: Method should return the sorted list of time values.*/
  /* @End: UT_CORE_0828 */
  ASSERT_THAT(notifications.size(), Eq(test.m_notifiers.size()));
  for (uint64_t i = 0U; i < test.m_notifiers.size(); ++i)
  {
    EXPECT_THAT(notifications[i], Eq(i));
  }
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0829
@Test_Description:    Test to verify that Wait() returns sorted list when triggered in reverse order.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611244, 61245 
*******************************************************************************/
TEST_F(ConditionVariable_test, WaitReturnsSortedListWhenTriggeredInReverseOrder)
{
  ::testing::Test::RecordProperty("TEST_ID", "a28eb73d-c279-46ed-b6f8-369b10045ea5");
  /* @Start: UT_CORE_0829*/
  /* @TS1: Invoke wait()*/
  /* @TE1.1: Method should return the sorted list of time values.*/
  /* @End: UT_CORE_0829 */
  waitReturnsSortedListWhenTriggeredInReverseOrder(*this, [this] { return m_waiter.wait(); });
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0830
@Test_Description:    Test to verify that Wait() returns sorted list when triggered in reverse order.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      61244,61245
*******************************************************************************/
TEST_F(ConditionVariable_test, TimedWaitReturnsSortedListWhenTriggeredInReverseOrder)
{
  /* @Start: UT_CORE_0830*/
  /* @TS1: Invoke TimedWait()*/
  /* @TE1.1: Method should return the sorted list of time values.*/
  /* @End: UT_CORE_0830 */
  ::testing::Test::RecordProperty("TEST_ID", "53050a1c-fb1c-42aa-a376-bfb095bf5f94");
  waitReturnsSortedListWhenTriggeredInReverseOrder(
      *this, [this] { return m_waiter.TimedWait(iox::units::Duration::fromSeconds(1)); });
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0831
@Test_Description:    Test for SecondWaitBlocksUntilNewNotification2.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233
*******************************************************************************/
TIMING_TEST_F(ConditionVariable_test, SecondWaitBlocksUntilNewNotification2, Repeat(5), [&] {
  ::testing::Test::RecordProperty("TEST_ID", "dcbd55ee-e401-42cb-bbf2-a266058c2e76");
  constexpr Type_t FIRST_EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 2U;
  constexpr Type_t SECOND_EVENT_INDEX = 0U;
  ConditionNotifier notifier1(m_condVarData, FIRST_EVENT_INDEX);
  ConditionNotifier notifier2(m_condVarData, SECOND_EVENT_INDEX);
  ConditionListener listener(m_condVarData);
  std::atomic_bool hasWaited{false};
  Watchdog watchdogFirstWait(m_timeToWait);
  watchdogFirstWait.watchAndActOnFailure([&] { listener.Destroy(); });
  notifier1.notify();
  notifier2.notify();
  NotificationVector_t activeNotifications = listener.wait();
  /* @Start: UT_CORE_0831 */
  /* @TS1: Invoke wait() */
  /* @TE1.1: wait should return EVENT_INDEX value of 2U */
  /* @End: UT_CORE_0831 */
  ASSERT_THAT(activeNotifications.size(), Eq(2U));
  EXPECT_THAT(activeNotifications[0], Eq(SECOND_EVENT_INDEX));
  EXPECT_THAT(activeNotifications[1], Eq(FIRST_EVENT_INDEX));
  Watchdog watchdogSecondWait(m_timeToWait);
  watchdogSecondWait.watchAndActOnFailure([&] { listener.Destroy(); });
  std::mutex m;
  std::unique_lock<std::mutex> lock(m);
  Barrier isThreadStarted(1U);
  std::thread waiter([&] {
    isThreadStarted.notify();
    activeNotifications = listener.wait(lock);
    hasWaited.store(true, std::memory_order_relaxed);
    /* @Start: UT_CORE_0831 */
    /* @TS2: Invoke wait() */
    /* @TE2.1: wait should return EVENT_INDEX value as 1U. */
    /* @End: UT_CORE_0831 */
    ASSERT_THAT(activeNotifications.size(), Eq(1U));
    EXPECT_THAT(activeNotifications[0], Eq(FIRST_EVENT_INDEX));
    for (const auto& notification : m_condVarData.mActiveNotifications)
    {
      EXPECT_THAT(notification, Eq(false));
    }
  });
  isThreadStarted.wait();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(false));
  notifier1.notify();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(true));
  waiter.join();
})

/*******************************************************************************
@TestCaseId:          UT_CORE_0832
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      61244, 61245, 611233 
*******************************************************************************/
TIMING_TEST_F(ConditionVariable_test, SecondWaitBlocksUntilNewNotification3, Repeat(5), [&] {
  ::testing::Test::RecordProperty("TEST_ID", "dcbd55ee-e401-42cb-bbf2-a266058c2e76");
  constexpr Type_t FIRST_EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 2U;
  constexpr Type_t SECOND_EVENT_INDEX = 0U;
  ConditionNotifier notifier1(m_condVarData, FIRST_EVENT_INDEX);
  ConditionNotifier notifier2(m_condVarData, SECOND_EVENT_INDEX);
  ConditionListener listener(m_condVarData);
  std::atomic_bool hasWaited{false};
  Watchdog watchdogFirstWait(m_timeToWait);
  watchdogFirstWait.watchAndActOnFailure([&] { listener.Destroy(); });
  notifier1.notify();
  notifier2.notify();
  NotificationVector_t activeNotifications = listener.wait();
  /* @Start: UT_CORE_0832 */
  /* @TS1: Invoke wait() */
  /* @TE1.1: wait should return 2U as the activeNotifications size. */
  /* @End: UT_CORE_0832 */
  ASSERT_THAT(activeNotifications.size(), Eq(2U));
  EXPECT_THAT(activeNotifications[0], Eq(SECOND_EVENT_INDEX));
  EXPECT_THAT(activeNotifications[1], Eq(FIRST_EVENT_INDEX));
  Watchdog watchdogSecondWait(m_timeToWait);
  watchdogSecondWait.watchAndActOnFailure([&] { listener.Destroy(); });
  Barrier isThreadStarted(1U);
  std::thread waiter([&] {
    isThreadStarted.notify();
    activeNotifications = listener.wait();
    hasWaited.store(true, std::memory_order_relaxed);
    //ASSERT_THAT(activeNotifications.size(), Eq(1U));
    //EXPECT_THAT(activeNotifications[0], Eq(FIRST_EVENT_INDEX));
    for (const auto& notification : m_condVarData.mActiveNotifications)
    {
      EXPECT_THAT(notification, Eq(false));
    }
  });
  isThreadStarted.wait();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(false));
  notifier1.notify_all();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(true));
  waiter.join();
})

/*******************************************************************************
@TestCaseId:          UT_CORE_0833
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      611233, 611234 
*******************************************************************************/
TIMING_TEST_F(ConditionVariable_test, SecondWaitBlocksUntilNewNotification4, Repeat(5), [&] {
  ::testing::Test::RecordProperty("TEST_ID", "dcbd55ee-e401-42cb-bbf2-a266058c2e76");
  constexpr Type_t FIRST_EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 2U;
  constexpr Type_t SECOND_EVENT_INDEX = 0U;
  ConditionNotifier notifier1(m_condVarData, FIRST_EVENT_INDEX);
  ConditionNotifier notifier2(m_condVarData, SECOND_EVENT_INDEX);
  ConditionListener listener(m_condVarData);
  std::atomic_bool hasWaited{false};

  Watchdog watchdogFirstWait(m_timeToWait);
  watchdogFirstWait.watchAndActOnFailure([&] { listener.Destroy(); });

  notifier1.notify();
  notifier2.notify();
  NotificationVector_t activeNotifications = listener.wait();

  /* @Start: UT_CORE_0833 */
  /* @TS1: Invoke wait() */
  /* @TE1.1: wait when invoked should return 2U as the activeNotifications size. */
  /* @End: UT_CORE_0833 */
  ASSERT_THAT(activeNotifications.size(), Eq(2U));
  EXPECT_THAT(activeNotifications[0], Eq(SECOND_EVENT_INDEX));
  EXPECT_THAT(activeNotifications[1], Eq(FIRST_EVENT_INDEX));

  Watchdog watchdogSecondWait(m_timeToWait);
  watchdogSecondWait.watchAndActOnFailure([&] { listener.Destroy(); });
  std::mutex m;
  std::unique_lock<std::mutex> lock(m);
  Barrier isThreadStarted(1U);
  std::thread waiter([&] {
    isThreadStarted.notify();
    listener.TimedWait(lock, m_timeToWait);
    hasWaited.store(true, std::memory_order_relaxed);
    //ASSERT_THAT(activeNotifications.size(), Eq(1U));
    //EXPECT_THAT(activeNotifications[0], Eq(FIRST_EVENT_INDEX));
    for (const auto& notification : m_condVarData.mActiveNotifications)
    {
      EXPECT_THAT(notification, Eq(false));
    }
  });
  isThreadStarted.wait();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(false));
  notifier1.notify_all();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_THAT(hasWaited, Eq(true));
  waiter.join();
})

/*******************************************************************************
@TestCaseId:          UT_CORE_0834
@Test_Description:    Test for ErrorGeneration of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      61244, 61245, 611231, 611232, 611234, 611235 
*******************************************************************************/
TEST(ConditionVariableData, ErrorGeneration)
{
  const ara::RuntimeNameT m_runtimeName{"Ferdinand"};
  const iox::units::Duration m_timeToWait = 2_s;
  const iox::units::Duration m_timingTestTime = 100_ms;
  /* @Start: UT_CORE_0834 */
  /* @TS1: create an object of ConditionNotifier class*/
  /* @TE1.1: ConditionNotifier class object should be created successfully*/
  /* @End: UT_CORE_0834 */
  ConditionVariableData m_condVarData{m_runtimeName};
  ConditionNotifier notifier1(m_condVarData, 300);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0835
@Test_Description:    Test for ErrorGeneration of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      61244, 61245, 611231, 611232, 611234, 611235 
*******************************************************************************/
TEST(ConditionVariableData, asStringLiteral)
{
  const ara::RuntimeNameT m_runtimeName{"Ferdinand"};
  const iox::units::Duration m_timeToWait = 2_s;
  const iox::units::Duration m_timingTestTime = 100_ms;
  ConditionVariableData m_condVarData{m_runtimeName};
  /* @Start: UT_CORE_0835 */
  /* @TS1: create an object of ConditionNotifier class*/
  /* @TE1.1: ConditionNotifier class object should be created successfully*/
  /* @End: UT_CORE_0835 */
  ConditionNotifier notifier1(m_condVarData, 300);
  ara::asStringLiteral(ara::PoshError::kPopoConditionVaribaleDataFailedToCreateSemaphore);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0836
@Test_Description:    Test for TimedWait method of condition variable class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      No changes.
@Comments:            -
@Test_Group:          ConditionVariable test.
@Design Tracing:      61244, 61245, 611231, 611232, 611234, 611235
*******************************************************************************/
TEST(ConditionVariableData, dummy)
{
  const ara::RuntimeNameT m_runtimeName{"Ferdinand"};
  using Type_t = iox::cxx::BestFittingType_t<ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER>;
  constexpr Type_t FIRST_EVENT_INDEX = ara::MAX_NUMBER_OF_EVENTS_PER_LISTENER - 2U;
  const iox::units::Duration m_timeToWait = 2_s;
  const iox::units::Duration m_timingTestTime = 100_ms;
  ConditionVariableData m_condVarData{""};
  ConditionVariableData m_condVarData2{"ABC"};
  /* @Start: UT_CORE_0836 */
  /* @TS1: create an object of ConditionNotifier and ConditionListener class*/
  /* @TE1.1: ConditionNotifier and ConditionListener class object should be created successfully*/
  /* @End: UT_CORE_0836 */
  ConditionNotifier notifier1(m_condVarData, FIRST_EVENT_INDEX);
  ConditionListener listener(m_condVarData);
  std::thread waiter([&] {
    std::mutex m;
    std::unique_lock<std::mutex> lock(m);
    listener.wait(lock);
  });
  sleep(1);
  notifier1.notify_all();
  waiter.join();
  //ConditionNotifier notifier1(m_condVarData, 300);
  // iox::asStringLiteral(iox::PoshError::kPopoConditionVaribaleDataFailedToCreateSemaphore);
}
} // namespace core
} // namespace ara
