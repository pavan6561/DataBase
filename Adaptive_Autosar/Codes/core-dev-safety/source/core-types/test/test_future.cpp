// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_future.cpp
//
// Purpose     : This file provides the unit test cases of class Result.
// -----------------------------------------------------------------------------------------------------------

#include "gmock_matchers.h"
#include "test_domain.h"

#include "ara/core/future.h"
#include "ara/core/promise.h"
#include "ara/core/error_code.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <type_traits>

using ara::core::ErrorCode;
using ara::core::Result;

using ::testing::Eq;
namespace ara
{
namespace core
{
/*******************************************************************************
@TestCaseId:          UT_CORE_1900
@Test_Description:    To test the functionality of move Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      609889, 611310
*******************************************************************************/
TEST(Future, MoveConstructor)
{
  Promise<int> obj;
  int var = 10;
  obj.set_value(var);
  Future<int> f1 = obj.get_future();
  Future<int> f2(std::move(f1));
  auto r1 = f2.GetResult();
  /* @Start: UT_CORE_1900 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: GetResult() should return 10. */
  /* @End: UT_CORE_1900 */
  EXPECT_EQ(r1.Value(), 10);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1902
@Test_Description:    To test the functionality of valid.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611312, 611310, 611502
*******************************************************************************/
TEST(Future, valid)
{
  Promise<int> obj;
  obj.set_value(10);
  Future<int> f1;
  /* @Start: UT_CORE_1902 */
  /* @TS1: Invoke the valid(). */
  /* @TE1: Result should equal to false. */
  /* @End: UT_CORE_1902 */
  EXPECT_EQ(f1.valid(), false);
  f1 = obj.get_future();
  /* @Start: UT_CORE_1902 */
  /* @TS2: Invoke the valid(). */
  /* @TE2: Result should equal to true. */
  /* @End: UT_CORE_1902 */
  EXPECT_EQ(f1.valid(), true);
}

int TestForFuture()
{
}

void DoSomething1(Promise<int>& obj)
{
  //obj.set_value(10);
}
void DoSomething2(Promise<int>& obj)
{
  obj.set_value(10);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1903
@Test_Description:    To test the functionality of wait for ready timeout.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611310, 611314
*******************************************************************************/
TEST(Future, wait_for_ready_timeout)
{
  std::chrono::seconds const kConnEstablishTimeout{3};
  Promise<int> myPromise;
  std::thread t(DoSomething1, std::ref(myPromise));
  t.detach();
  ara::core::Future<int> connection_established(myPromise.get_future());
  ara::core::future_status const status{connection_established.wait_for(kConnEstablishTimeout)};
  /* @Start: UT_CORE_1903 */
  /* @TS1: Invoke the wait_for(). */
  /* @TE1: wait_for() should return ara::core::future_status::timeout. */
  /* @End: UT_CORE_1903 */
  EXPECT_TRUE(status == ara::core::future_status::timeout);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1904
@Test_Description:    To test the functionality of wait for ready ready.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611310, 611314, 611500
*******************************************************************************/
TEST(Future, wait_for_ready_ready)
{
  std::chrono::seconds const kConnEstablishTimeout{3};
  Promise<int> myPromise;
  std::thread t(DoSomething2, std::ref(myPromise));
  t.detach();
  ara::core::Future<int> connection_established(myPromise.get_future());
  ara::core::future_status const status{connection_established.wait_for(kConnEstablishTimeout)};
  /* @Start: UT_CORE_1904 */
  /* @TS1: Invoke the wait_for(). */
  /* @TE1: wait_for() should return ara::core::future_status::ready. */
  /* @End: UT_CORE_1904 */
  EXPECT_TRUE(status == ara::core::future_status::ready);
  /* @Start: UT_CORE_1904 */
  /* @TS2: Invoke the get(). */
  /* @TE2: Result should be return true. */
  /* @End: UT_CORE_1904 */
  EXPECT_TRUE(connection_established.get() == 10);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1905
@Test_Description:    To test the functionality of wait until ready.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611310, 611505
*******************************************************************************/
TEST(Future, wait_until_ready)
{
  std::chrono::system_clock::time_point kConnEstablishTimeout
      = std::chrono::system_clock::now() + std::chrono::seconds(2);

  Promise<int> myPromise;
  std::thread t(DoSomething2, std::ref(myPromise));
  t.detach();
  ara::core::Future<int> connection_established(myPromise.get_future());
  ara::core::future_status const status{connection_established.wait_until(kConnEstablishTimeout)};
  /* @Start: UT_CORE_1905 */
  /* @TS1: Invoke the wait_until(). */
  /* @TE1: wait_until() should be return ara::core::future_status::ready. */
  /* @End: UT_CORE_1905 */
  EXPECT_TRUE(status == ara::core::future_status::ready);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1906
@Test_Description:    To test the functionality of wait until timeout.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611310, 611505
*******************************************************************************/
TEST(Future, wait_until_timeout)
{
  std::chrono::system_clock::time_point kConnEstablishTimeout
      = std::chrono::system_clock::now() + std::chrono::seconds(2);

  Promise<int> myPromise;
  std::thread t(DoSomething1, std::ref(myPromise));
  t.detach();
  ara::core::Future<int> connection_established(myPromise.get_future());
  ara::core::future_status const status{connection_established.wait_until(kConnEstablishTimeout)};
  /* @Start: UT_CORE_1906 */
  /* @TS1: Invoke the wait_until(). */
  /* @TE1: wait_until() should return ara::core::future_status::timeout. */
  /* @End: UT_CORE_1906 */
  EXPECT_TRUE(status == ara::core::future_status::timeout);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1907
@Test_Description:    To test the functionality of move Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611310
*******************************************************************************/
TEST(Future, MoveConstructor_void)
{
  Promise<void> obj;
  obj.SetError(std::move(ara::core::CoreErrc::kInvalidArgument));
  Future<void> f1 = obj.get_future();
  Future<void> f2(std::move(f1));
  auto r1 = f2.GetResult();
  /* @Start: UT_CORE_1907 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1907 */
  EXPECT_EQ(r1.Error().Value(), 22);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1908
@Test_Description:    To test the functionality of valid void.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611312
*******************************************************************************/
TEST(Future, valid_void)
{
  Promise<void> obj;
  obj.set_value();
  Future<void> f1;
  /* @Start: UT_CORE_1908 */
  /* @TS1: Invoke the valid(). */
  /* @TE1: Result should equal to false. */
  /* @End: UT_CORE_1908 */
  EXPECT_EQ(f1.valid(), false);
  f1 = obj.get_future();
  /* @Start: UT_CORE_1908 */
  /* @TS2: Invoke the valid(). */
  /* @TE2: Result should equal to true. */
  /* @End: UT_CORE_1908 */
  EXPECT_EQ(f1.valid(), true);
}

void DoSomething3(Promise<void>& obj)
{
}
void DoSomething4(Promise<void>& obj)
{
  obj.SetError(ara::core::CoreErrc::kInvalidArgument);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1909
@Test_Description:    To test the functionality of wait for ready timeout void.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611314
*******************************************************************************/
TEST(Future, wait_for_ready_timeout_void)
{
  std::chrono::seconds const kConnEstablishTimeout{3};
  Promise<void> myPromise;
  std::thread t(DoSomething3, std::ref(myPromise));
  t.detach();
  ara::core::Future<void> connection_established(myPromise.get_future());
  ara::core::future_status const status{connection_established.wait_for(kConnEstablishTimeout)};
  /* @Start: UT_CORE_1909 */
  /* @TS1: Invoke the wait_for(). */
  /* @TE1: wait_for() should return ara::core::future_status::timeout. */
  /* @End: UT_CORE_1909 */
  EXPECT_TRUE(status == ara::core::future_status::timeout);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1910
@Test_Description:    To test the functionality of wait for ready ready void.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611314
*******************************************************************************/
TEST(Future, wait_for_ready_ready_void)
{
  std::chrono::seconds const kConnEstablishTimeout{3};
  Promise<void> myPromise;
  std::thread t(DoSomething4, std::ref(myPromise));
  t.detach();
  ara::core::Future<void> connection_established(myPromise.get_future());
  ara::core::future_status const status{connection_established.wait_for(kConnEstablishTimeout)};
  /* @Start: UT_CORE_1910 */
  /* @TS1: Invoke the wait_for(). */
  /* @TE1: wait_for() should return ara::core::future_status::ready. */
  /* @End: UT_CORE_1910 */
  EXPECT_TRUE(status == ara::core::future_status::ready);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1911
@Test_Description:    To test the functionality of wait until ready void.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611314
*******************************************************************************/
TEST(Future, wait_until_ready_void)
{
  std::chrono::system_clock::time_point kConnEstablishTimeout
      = std::chrono::system_clock::now() + std::chrono::seconds(2);

  Promise<void> myPromise;
  std::thread t(DoSomething4, std::ref(myPromise));
  t.detach();
  ara::core::Future<void> connection_established(myPromise.get_future());
  ara::core::future_status const status{connection_established.wait_until(kConnEstablishTimeout)};
  /* @Start: UT_CORE_1911 */
  /* @TS1: Invoke the wait_until(). */
  /* @TE1: wait_until() should return ara::core::future_status::ready. */
  /* @End: UT_CORE_1911 */
  EXPECT_TRUE(status == ara::core::future_status::ready);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1912
@Test_Description:    To test the functionality of wait until timeout void.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Future
@Requirement Tracing: -
@Design Tracing:      611505, 611315
*******************************************************************************/
TEST(Future, wait_until_timeout_void)
{
  std::chrono::system_clock::time_point kConnEstablishTimeout
      = std::chrono::system_clock::now() + std::chrono::seconds(2);

  Promise<void> myPromise;
  std::thread t(DoSomething3, std::ref(myPromise));
  t.detach();
  ara::core::Future<void> connection_established(myPromise.get_future());
  ara::core::future_status const status{connection_established.wait_until(kConnEstablishTimeout)};
  /* @Start: UT_CORE_1912 */
  /* @TS1: Invoke the wait_until(). */
  /* @TE1: wait_until() should return ara::core::future_status::timeout. */
  /* @End: UT_CORE_1912 */
  EXPECT_TRUE(status == ara::core::future_status::timeout);
}

} // namespace core
} // namespace ara
