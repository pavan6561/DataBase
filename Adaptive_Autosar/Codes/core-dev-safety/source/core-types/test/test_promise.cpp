// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_promise.cpp
//
// Purpose     : This file provides the unit test cases of class Result .
// -----------------------------------------------------------------------------------------------------------

#include "gmock_matchers.h"
#include "test_domain.h"

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
@TestCaseId:          UT_CORE_1800
@Test_Description:    To test the functionality of Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611338, 611340
*******************************************************************************/
TEST(Promise, Constructor)
{
  Promise<int> obj;
  int var = 10;
  obj.set_value(var);
  Future<int> f = obj.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1800 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 10. */
  /* @End: UT_CORE_1800 */
  EXPECT_EQ(r.Value(), 10);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1801
@Test_Description:    To test the functionality of Move Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611338, 611340
*******************************************************************************/
TEST(Promise, MoveConstructor)
{
  Promise<int> obj1;
  obj1.set_value(10);
  Promise<int> obj2(std::move(obj1));
  Future<int> f = obj2.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1801 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should be return 10. */
  /* @End: UT_CORE_1801 */
  EXPECT_EQ(r.Value(), 10);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1802
@Test_Description:    To test the functionality of Assignment Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611341
*******************************************************************************/
TEST(Promise, AssignmentOperator)
{
  Promise<int> obj1;
  Promise<int> obj2;
  obj1.set_value(10);
  obj2 = std::move(obj1);
  Future<int> f = obj2.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1802 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should be return 10. */
  /* @End: UT_CORE_1802 */
  EXPECT_EQ(r.Value(), 10);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1803
@Test_Description:    To test the functionality of Assignment Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611339, 611340
*******************************************************************************/
TEST(Promise, SetErrorMove)
{
  Promise<int> obj1;
  obj1.SetError(std::move(ara::core::CoreErrc::kInvalidArgument));
  Future<int> f = obj1.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1803 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1803 */
  EXPECT_EQ(r.Error().Value(), 22);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1804
@Test_Description:    To test the functionality of set error copy .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611538, 609890, 611541
*******************************************************************************/
TEST(Promise, SetErrorCopy)
{
  Promise<int> obj1;
  obj1.SetError(ara::core::CoreErrc::kInvalidArgument);
  Future<int> f = obj1.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1804 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1804 */
  EXPECT_EQ(r.Error().Value(), 22);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1807
@Test_Description:    To test the functionality of set value copy .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611538, 609890, 611541
*******************************************************************************/
TEST(Promise, SetValueCopy)
{
  Promise<int> obj1;
  obj1.set_value(10);
  Future<int> f = obj1.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1807 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 10. */
  /* @End: UT_CORE_1807 */
  EXPECT_TRUE(r.Value() == 10);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1808
@Test_Description:    To test the functionality of set value move .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611346, 611538, 609890, 611541, 611340
*******************************************************************************/
TEST(Promise, SetValueMove)
{
  Promise<int> obj1;
  obj1.set_value(std::move(10));
  Future<int> f = obj1.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1808 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 10. */
  /* @End: UT_CORE_1808 */
  EXPECT_TRUE(r.Value() == 10);
}
ara::core::Future<int> PrepareRollback()
{
  Promise<int> obj1;
  obj1.set_value(10);
  return obj1.get_future();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1809
@Test_Description:    To test the functionality of swap.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611346, 611538, 611342
*******************************************************************************/
TEST(Promise, swap)
{
  Promise<int> obj1;
  obj1.set_value(std::move(10));
  Promise<int> obj2;
  obj2.set_value(std::move(20));
  obj1.swap(obj2);

  Future<int> f1 = obj1.get_future();
  auto r1 = f1.GetResult();
  /* @Start: UT_CORE_1809 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 20. */
  /* @End: UT_CORE_1809 */
  EXPECT_TRUE(r1.Value() == 20);

  Future<int> f2 = obj2.get_future();
  auto r2 = f2.GetResult();
  /* @Start: UT_CORE_1809 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 10. */
  /* @End: UT_CORE_1809 */
  EXPECT_TRUE(r2.Value() == 10);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1810
@Test_Description:    To test the functionality of set result .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611346, 611347
*******************************************************************************/
TEST(Promise, SetResult)
{
  Promise<int> obj1;
  ara::core::Result<int> result(10);
  obj1.SetResult(result);
  Future<int> f1 = obj1.get_future();
  auto r1 = f1.GetResult();
  /* @Start: UT_CORE_1810 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 10. */
  /* @End: UT_CORE_1810 */
  EXPECT_TRUE(r1.Value() == 10);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1811
@Test_Description:    To test the functionality of set result move .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611346, 611538, 611342
*******************************************************************************/
TEST(Promise, SetResultMove)
{
  Promise<int> obj1;
  ara::core::Result<int> result(10);
  obj1.SetResult(std::move(result));
  Future<int> f1 = obj1.get_future();
  auto r1 = f1.GetResult();
  /* @Start: UT_CORE_1811 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 10. */
  /* @End: UT_CORE_1811 */
  EXPECT_TRUE(r1.Value() == 10);
}
//////////////////////////////////////////////////////////////////
/*******************************************************************************
@TestCaseId:          UT_CORE_1812
@Test_Description:    To test the functionality of v   oid constructor .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611343, 611546
*******************************************************************************/
TEST(Promise, Void_Constructor)
{
  Promise<void> obj1;
  obj1.SetError(ara::core::CoreErrc::kInvalidArgument);
  Future<void> f = obj1.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1812 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1812 */
  EXPECT_EQ(r.Error().Value(), 22);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1813
@Test_Description:    To test the functionality of void move constructor .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611338, 611340
*******************************************************************************/
TEST(Promise, void_MoveConstructor)
{
  Promise<void> obj1;
  obj1.SetError(std::move(ara::core::CoreErrc::kInvalidArgument));
  Promise<void> obj2(std::move(obj1));
  Future<void> f = obj2.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1813 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1813 */
  EXPECT_EQ(r.Error().Value(), 22);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1814
@Test_Description:    To test the functionality of void assignment operator .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611541
*******************************************************************************/
TEST(Promise, void_AssignmentOperator)
{
  Promise<void> obj1;
  Promise<void> obj2;
  obj1.SetError(ara::core::CoreErrc::kInvalidArgument);
  obj2 = std::move(obj1);
  Future<void> f = obj2.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1814 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1814 */
  EXPECT_EQ(r.Error().Value(), 22);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1815
@Test_Description:    To test the functionality of void set error move .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611538, 609890, 611541
*******************************************************************************/
TEST(Promise, void_SetErrorMove)
{
  Promise<void> obj1;
  obj1.SetError(std::move(ara::core::CoreErrc::kInvalidArgument));
  Future<void> f = obj1.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1815 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1815 */
  EXPECT_EQ(r.Error().Value(), 22);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1816
@Test_Description:    To test the functionality of void set error copy .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611538, 609890, 611541
*******************************************************************************/
TEST(Promise, void_SetErrorCopy)
{
  Promise<void> obj1;
  obj1.SetError(ara::core::CoreErrc::kInvalidArgument);
  Future<void> f = obj1.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1816 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1816 */
  EXPECT_EQ(r.Error().Value(), 22);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1818
@Test_Description:    To test the functionality of void set value copy .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611538, 609890, 611541
*******************************************************************************/
TEST(Promise, void_SetValueCopy)
{
  Promise<void> obj1;
  obj1.set_value();
  Future<void> f = obj1.get_future();
  auto r = f.GetResult();
  /* @Start: UT_CORE_1818 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return true. */
  /* @End: UT_CORE_1818 */
  EXPECT_TRUE(r.HasValue() == true);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1819
@Test_Description:    To test the functionality of void set swap .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611346, 611538, 611342
*******************************************************************************/
TEST(Promise, void_swap)
{
  Promise<void> obj1;
  obj1.SetError(ara::core::CoreErrc::kInvalidArgument);
  Promise<void> obj2;
  obj2.swap(obj1);
  Future<void> f2 = obj2.get_future();
  auto r2 = f2.GetResult();
  /* @Start: UT_CORE_1819 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1819 */
  EXPECT_EQ(r2.Error().Value(), 22);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1820
@Test_Description:    To test the functionality of void set result.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611346, 611347
*******************************************************************************/
TEST(Promise, void_SetResult)
{
  Promise<void> obj1;
  ara::core::Result<void> result(ara::core::CoreErrc::kInvalidArgument);
  obj1.SetResult(result);
  Future<void> f1 = obj1.get_future();
  auto r1 = f1.GetResult();
  /* @Start: UT_CORE_1820 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1820 */
  EXPECT_EQ(r1.Error().Value(), 22);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1821
@Test_Description:    To test the functionality of void set result move .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          Promise
@Requirement Tracing: -
@Design Tracing:      611346, 611538, 611342
*******************************************************************************/
TEST(Promise, void_SetResultMove)
{
  Promise<void> obj1;
  ara::core::Result<void> result(ara::core::CoreErrc::kInvalidArgument);
  obj1.SetResult(std::move(result));
  Future<void> f1 = obj1.get_future();
  auto r1 = f1.GetResult();
  /* @Start: UT_CORE_1821 */
  /* @TS1: Invoke the GetResult(). */
  /* @TE1: Result should return 22. */
  /* @End: UT_CORE_1821 */
  EXPECT_EQ(r1.Error().Value(), 22);
}

} // namespace core
} // namespace ara