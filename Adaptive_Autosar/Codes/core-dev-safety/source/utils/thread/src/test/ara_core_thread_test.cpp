// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ara_core_thread_test.cpp
//
// Purpose     : This file provides Unit Test cases for Thread class.
// -----------------------------------------------------------------------------------------------------------

#include "iceoryx_hoofs/internal/units/duration.hpp"
#include "iceoryx_hoofs/posix_wrapper/thread.hpp"
#include <condition_variable>
#include <cstdint>
#include <mutex>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../inc/public/ara/core/thread/thread.h"
#include <thread>

namespace ara
{
namespace core
{
namespace thread
{
using namespace ::testing;
using namespace iox::posix;
using namespace iox::cxx;
using namespace iox::units;
using namespace iox::units::duration_literals;

class ThreadTest : public ::testing::Test
{
 public:
  ThreadTest()
  {
  }
  optional<Thread> sut;
};

/*******************************************************************************
@TestCaseId:          UT_CORE_0906
@Test_Description:    create thread with non empty callable succeeds.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ThreadTest
@Requirement Tracing: -
@Design Tracing:      611249, 611253, 614041, 
*******************************************************************************/
TEST_F(ThreadTest, CreateThreadWithNonEmptyCallableSucceeds)
{
  ::testing::Test::RecordProperty("TEST_ID", "0d1e439d-c84e-4a46-ac45-dc8be7530c32");
  bool callableWasCalled = false;
  Thread::Callable callable = [&] { callableWasCalled = true; };
  ASSERT_FALSE(ThreadBuilder().create(sut, callable).has_error());
  sut.reset();
  EXPECT_TRUE(callableWasCalled);
}

class Barrier
{
 public:
  Barrier(uint32_t requiredCount = 0) : m_requiredCount(requiredCount)
  {
  }

  void notify()
  {
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      ++m_count;
    }
    if (m_count >= m_requiredCount)
    {
      m_condVar.notify_all();
    }
  }

  void wait()
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    auto cond = [&]() { return m_count >= m_requiredCount; };
    m_condVar.wait(lock, cond);
  }

  void reset(uint32_t requiredCount)
  {
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_requiredCount = requiredCount;
      m_count = 0;
    }

    // notify regardless of count, the threads woken up need to check the condition
    m_condVar.notify_all();
  }

 private:
  uint32_t m_count{0};
  std::mutex m_mutex;
  std::condition_variable m_condVar;
  uint32_t m_requiredCount;
};
/*******************************************************************************
@TestCaseId:          UT_CORE_0907
@Test_Description:    Dtor Of Thread BlocksUntil Callback Has Finished.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ThreadTest
@Requirement Tracing: -
@Design Tracing:      611250
*******************************************************************************/
TEST_F(ThreadTest, DtorOfThreadBlocksUntilCallbackHasFinished)
{
  ::testing::Test::RecordProperty("TEST_ID", "1062a036-e825-4f30-bfb8-00d5de47fdfd");

  std::chrono::steady_clock::time_point start;
  std::chrono::steady_clock::time_point end;
  constexpr Duration TEST_WAIT_TIME = 100_ms;
  Barrier threadSync;

  ASSERT_FALSE(ThreadBuilder()
                   .create(sut,
                       [&] {
                         threadSync.wait();
                         std::this_thread::sleep_for(
                             std::chrono::nanoseconds(TEST_WAIT_TIME.toNanoseconds()));
                       })
                   .has_error());

  start = std::chrono::steady_clock::now();
  threadSync.notify();
  sut.reset();
  end = std::chrono::steady_clock::now();

  EXPECT_THAT((end - start).count(), Ge(TEST_WAIT_TIME.toNanoseconds()));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0908
@Test_Description:    Set and get with empty thread name is working.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ThreadTest
@Requirement Tracing: -
@Design Tracing:      611251, 611252
*******************************************************************************/
TEST_F(ThreadTest, SetAndGetWithEmptyThreadNameIsWorking)
{
  ::testing::Test::RecordProperty("TEST_ID", "ba2ed4d9-f051-4ad1-a2df-6741134c494f");
  ThreadName emptyString = "";
  ASSERT_FALSE(ThreadBuilder()
                   .name(emptyString)
                   .create(sut, [] { std::this_thread::sleep_for(std::chrono::milliseconds(10)); })
                   .has_error());

  auto getResult = sut->GetName();

  EXPECT_THAT(getResult, StrEq(emptyString));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0909
@Test_Description:    Set and Get with thread name capacity is working.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ThreadTest
@Requirement Tracing: -
@Design Tracing:      611251,611253
*******************************************************************************/
TEST_F(ThreadTest, SetAndGetWithThreadNameCapacityIsWorking)
{
  ::testing::Test::RecordProperty("TEST_ID", "a67128fe-a779-4bdb-a849-3bcbfed4b20f");
  ThreadName stringEqualToThreadNameCapacitiy = "123456789ABCDEF";
  EXPECT_THAT(stringEqualToThreadNameCapacitiy.capacity(), Eq(stringEqualToThreadNameCapacitiy.size()));
  ASSERT_FALSE(ThreadBuilder()
                   .name(stringEqualToThreadNameCapacitiy)
                   .create(sut, [] { std::this_thread::sleep_for(std::chrono::milliseconds(10)); })
                   .has_error());

  auto getResult = sut->GetName();

  EXPECT_THAT(getResult, StrEq(stringEqualToThreadNameCapacitiy));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_09010
@Test_Description:    Set and Get small string is working.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ThreadTest
@Requirement Tracing: -
@Design Tracing:      611251, 611253
*******************************************************************************/
TEST_F(ThreadTest, SetAndGetSmallStringIsWorking)
{
  ::testing::Test::RecordProperty("TEST_ID", "b5141d3c-2721-478c-b3d1-f35fb3321117");
  ThreadName stringShorterThanThreadNameCapacitiy = "I'm short";
  ASSERT_FALSE(ThreadBuilder()
                   .name(stringShorterThanThreadNameCapacitiy)
                   .create(sut, [] { std::this_thread::sleep_for(std::chrono::milliseconds(10)); })
                   .has_error());

  auto getResult = sut->GetName();

  EXPECT_THAT(getResult, StrEq(stringShorterThanThreadNameCapacitiy));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_09011
@Test_Description:    Set a thread name.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ThreadTest
@Requirement Tracing: -
@Design Tracing:      611249
*******************************************************************************/
TEST_F(ThreadTest, SettheThreadName)
{
  ThreadName name = "AVIN";
  std::thread mthread = std::thread([] { std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
  SetThreadName(mthread.native_handle(), name);
  mthread.join();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_09012
@Test_Description:    get a thread name.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ThreadTest
@Requirement Tracing: -
@Design Tracing:      611252
*******************************************************************************/
TEST_F(ThreadTest, GetTheThreadName)
{
  ThreadName name = "AVIN1";
  std::thread mthread = std::thread([] { std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
  SetThreadName(mthread.native_handle(), name);

  ThreadName output = GetThreadName(mthread.native_handle());
  EXPECT_EQ(output, "AVIN1");
  mthread.join();
}

} // namespace thread
} // namespace core
} // namespace ara
