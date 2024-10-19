// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_future_error_domain.cpp
//
// Purpose     : This file provides the unit test cases of class FutureErrorDomain.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/future_error_domain.h"
#include "ara/core/core_error_domain.h"
#include "ara/core/error_domain.h"
#include "ara/core/error_code.h"
#include "ara/core/exception.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <cstdint>

/*******************************************************************************
@TestCaseId:          UT_CORE_0538
@Test_Description:    To test the functionality of Static Type Properties of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611806
*******************************************************************************/
TEST(FutureErrorDomainTest, StaticTypeProperties)
{
  /* @Start: UT_CORE_0538 */
  /* @TS1: Get the value of std::is_enum<ara::core::FutureErrorDomain::Errc> of FutureErrorDomain. */
  /* @TE1: std::is_enum<ara::core::FutureErrorDomain::Errc> should return true. */
  /* @End: UT_CORE_0538 */
  static_assert(std::is_enum<ara::core::FutureErrorDomain::Errc>::value, "Nested type Errc is not an enum");

  /* @Start: UT_CORE_0538 */
  /* @TS2: Get the value of std::is_same<ara::core::FutureErrorDomain::Errc, ara::core::FutureErrc> of FutureErrorDomain. */
  /* @TE2: std::is_same<ara::core::FutureErrorDomain::Errc, ara::core::FutureErrc> should return true. */
  /* @End: UT_CORE_0538 */
  static_assert(std::is_same<ara::core::FutureErrorDomain::Errc, ara::core::FutureErrc>::value,
      "Nested type Errc is not the expected enum");

  /* @Start: UT_CORE_0538 */
  /* @TS3: Get the value of std::is_class<ara::core::FutureErrorDomain::Exception> of FutureErrorDomain. */
  /* @TE3: std::is_class<ara::core::FutureErrorDomain::Exception> should return true. */
  /* @End: UT_CORE_0538 */
  static_assert(std::is_class<ara::core::FutureErrorDomain::Exception>::value,
      "Nested type Exception is not a class type");

  /* @Start: UT_CORE_0538 */
  /* @TS4: Get the value of std::is_base_of<ara::core::Exception, ara::core::FutureErrorDomain::Exception> of FutureErrorDomain. */
  /* @TE4: std::is_base_of<ara::core::Exception, ara::core::FutureErrorDomain::Exception> should return true. */
  /* @End: UT_CORE_0538 */
  static_assert(std::is_base_of<ara::core::Exception, ara::core::FutureErrorDomain::Exception>::value,
      "Nested type Exception is not derived from ara::core::Exception");

  /* @Start: UT_CORE_0538 */
  /* @TS5: Get the value of std::is_final<ara::core::FutureErrorDomain> of FutureErrorDomain. */
  /* @TE5: std::is_final<ara::core::FutureErrorDomain> should return true. */
  /* @End: UT_CORE_0538 */
  static_assert(std::is_final<ara::core::FutureErrorDomain>::value, "ErrorDomain subclass is not final");

  SUCCEED();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0539
@Test_Description:    To test the functionality of Name of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611808
*******************************************************************************/
TEST(FutureErrorDomainTest, Name)
{
  ara::core::FutureErrorDomain obj;
  obj.Name();
  /* @Start: UT_CORE_0539 */
  /* @TS1: Create insatnce of ara::core::FutureErrorDomain as obj and invoke the Name(). */
  /* @TE1: Name() should be equal to "Future". */
  /* @End: UT_CORE_0539 */
  EXPECT_EQ(obj.Name(), ("Future"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0540
@Test_Description:    To test the functionality of Message of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611809
*******************************************************************************/
TEST(FutureErrorDomainTest, Message)
{
  constexpr ara::core::FutureErrorDomain obj;
  ara::core::FutureErrc err = ara::core::FutureErrc::kBrokenPromise;
  ara::core::ErrorDomain::CodeType num = static_cast<ara::core::ErrorDomain::CodeType>(err);
  /* @Start: UT_CORE_0540 */
  /* @TS1: Invoke Message() of ara::core::FutureErrc. */
  /* @TE1: Message() should be equal to "broken promise". */
  /* @End: UT_CORE_0540 */
  EXPECT_EQ(obj.Message(num), ("broken promise"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0541
@Test_Description:    To test the functionality of Message of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611809
*******************************************************************************/
TEST(FutureErrorDomainTest, Message1)
{
  constexpr ara::core::FutureErrorDomain obj;
  ara::core::FutureErrc err = ara::core::FutureErrc::kFutureAlreadyRetrieved;
  ara::core::ErrorDomain::CodeType num = static_cast<ara::core::ErrorDomain::CodeType>(err);
  /* @Start: UT_CORE_0541 */
  /* @TS1: Invoke Message() of ara::core::FutureErrc. */
  /* @TE1: Message() should be equal to "future already retrieved". */
  /* @End: UT_CORE_0541 */
  EXPECT_EQ(obj.Message(num), ("future already retrieved"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0542
@Test_Description:    To test the functionality of Message of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611809
*******************************************************************************/
TEST(FutureErrorDomainTest, Message2)
{
  constexpr ara::core::FutureErrorDomain obj;
  ara::core::FutureErrc err = ara::core::FutureErrc::kBrokenPromise;
  ara::core::ErrorDomain::CodeType num = static_cast<ara::core::ErrorDomain::CodeType>(err);
  /* @Start: UT_CORE_0542 */
  /* @TS1: Invoke Message() of ara::core::FutureErrc. */
  /* @TE1: Message() should be equal to "broken promise". */
  /* @End: UT_CORE_0542 */
  EXPECT_EQ(obj.Message(num), ("broken promise"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0543
@Test_Description:    To test the functionality of Message of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611809
*******************************************************************************/
TEST(FutureErrorDomainTest, Message3)
{
  constexpr ara::core::FutureErrorDomain obj;
  ara::core::FutureErrc err = ara::core::FutureErrc::kPromiseAlreadySatisfied;
  ara::core::ErrorDomain::CodeType num = static_cast<ara::core::ErrorDomain::CodeType>(err);
  /* @Start: UT_CORE_0543 */
  /* @TS1: Invoke Message() of ara::core::FutureErrc. */
  /* @TE1: Message() should be equal to "promise already satisfied". */
  /* @End: UT_CORE_0543 */
  EXPECT_EQ(obj.Message(num), ("promise already satisfied"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0544
@Test_Description:    To test the functionality of Message of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611809
*******************************************************************************/
TEST(FutureErrorDomainTest, Message4)
{
  constexpr ara::core::FutureErrorDomain obj;
  ara::core::FutureErrc err = ara::core::FutureErrc::kNoState;
  ara::core::ErrorDomain::CodeType num = static_cast<ara::core::ErrorDomain::CodeType>(err);
  /* @Start: UT_CORE_0544 */
  /* @TS1: Invoke Message() of ara::core::FutureErrc. */
  /* @TE1: Message() should be equal to "no state associated with this future". */
  /* @End: UT_CORE_0544 */
  EXPECT_EQ(obj.Message(num), ("no state associated with this future"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0545
@Test_Description:    To test the functionality of Message of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611809
*******************************************************************************/
TEST(FutureErrorDomainTest, Message5)
{
  constexpr ara::core::FutureErrorDomain obj;
  ara::core::CoreErrc err = ara::core::CoreErrc::kInvalidArgument;
  ara::core::ErrorDomain::CodeType num = static_cast<ara::core::ErrorDomain::CodeType>(err);
  /* @Start: UT_CORE_0545 */
  /* @TS1: Invoke Message() of ara::core::CoreErrc. */
  /* @TE1: Message() should be equal to "unknown future error". */
  /* @End: UT_CORE_0545 */
  EXPECT_EQ(obj.Message(num), ("unknown future error"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0546
@Test_Description:    To test the functionality of Throw of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611810
*******************************************************************************/
TEST(FutureErrorDomainTest, Throw)
{
  ara::core::ErrorCode const ec(ara::core::FutureErrc::kBrokenPromise);

  constexpr ara::core::FutureErrorDomain obj;
  /* @Start: UT_CORE_0546 */
  /* @TS1: Create instance of ara::core::ErrorCode by passing the  parameter as kBrokenPromise and inovoke the ThrowAsException(). */
  /* @TE1: ThrowAsException() should be equal to "broken promise". */
  /* @End: UT_CORE_0546 */
  EXPECT_THROW(obj.ThrowAsException(ec), ara::core::FutureException);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0547
@Test_Description:    To test the functionality of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611843
*******************************************************************************/
TEST(FutureErrorDomainTest, FutureErrorDomainAccessor)
{
  /* @Start: UT_CORE_0547 */
  /* @TS1: Invoke GetFutureErrorDomain(). */
  /* @TE1: The instancee should be created. */
  /* @End: UT_CORE_0547 */
  EXPECT_NO_THROW(ara::core::GetFutureErrorDomain());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0548
@Test_Description:    To test the functionality of ErrorCode of FutureErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          FutureErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611844 
*******************************************************************************/
TEST(FutureErrorDomainTest, MakeErrorCode)
{
  ara::core::ErrorDomain const& obj = ara::core::GetFutureErrorDomain();

  ara::core::ErrorCode ec = MakeErrorCode(ara::core::FutureErrc::kBrokenPromise, 0);

  /* @Start: UT_CORE_0548 */
  /* @TS1: Invoke SupportData() of ara::core::ErrorCode. */
  /* @TE1: SupportData() should be equal to 0. */
  /* @End: UT_CORE_0548 */
  EXPECT_EQ(ec.SupportData(), 0);
  /* @Start: UT_CORE_0548 */
  /* @TS2: Invoke ara::core::ErrorCode by passing the parameter as kBrokenPromise and inovoke the Value(). */
  /* @TE2: Value() should be equal to kBrokenPromise. */
  /* @End: UT_CORE_0548 */
  EXPECT_EQ(
      ec.Value(), (static_cast<ara::core::ErrorDomain::CodeType>(ara::core::FutureErrc::kBrokenPromise)));
}
