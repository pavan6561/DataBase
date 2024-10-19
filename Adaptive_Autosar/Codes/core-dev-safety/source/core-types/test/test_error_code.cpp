// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_error_code.cpp
//
// Purpose     : This file provides the unit test cases of class ErrorCode.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/error_code.h"
#include "ara/core/future_error_domain.h"

#include "test_domain.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <type_traits>

using ::testing::Eq;
using ::testing::Not;
using ::testing::NotNull;
using ::testing::Ref;
/*******************************************************************************
@TestCaseId:          UT_CORE_0067
@Test_Description:    To test the functionality of ara::core::ErrorCode.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611440
*******************************************************************************/
TEST(ErrorCodeTest, StaticTypeProperties)
{
  /* @Start: UT_CORE_0067 */
  /* @TS1: Get the value of std::is_trivially_copy_constructible<ara::core::ErrorCode>::value. */
  /* @TE1: std::is_trivially_copy_constructible<ara::core::ErrorCode> should return true. */
  /* @End: UT_CORE_0067 */
  EXPECT_TRUE(std::is_trivially_copy_constructible<ara::core::ErrorCode>::value);

  /* @Start: UT_CORE_0067 */
  /* @TS2: Get the value of std::is_trivially_move_constructible<ara::core::ErrorCode>::value. */
  /* @TE2: std::is_trivially_move_constructible<ara::core::ErrorCode> should return true. */
  /* @End: UT_CORE_0067 */
  EXPECT_TRUE(std::is_trivially_move_constructible<ara::core::ErrorCode>::value);

  /* @Start: UT_CORE_0067 */
  /* @TS3: Get the value of std::is_trivially_copy_assignable<ara::core::ErrorCode>::value. */
  /* @TE3: std::is_trivially_copy_assignable<ara::core::ErrorCode> should return true. */
  /* @End: UT_CORE_0067 */
  EXPECT_TRUE(std::is_trivially_copy_assignable<ara::core::ErrorCode>::value);

  /* @Start: UT_CORE_0067 */
  /* @TS4: Get the value of std::is_trivially_move_assignable<ara::core::ErrorCode>::value. */
  /* @TE4: std::is_trivially_move_assignable<ara::core::ErrorCode> should return true. */
  /* @End: UT_CORE_0067 */
  EXPECT_TRUE(std::is_trivially_move_assignable<ara::core::ErrorCode>::value);

  /* @Start: UT_CORE_0067 */
  /* @TS5: Get the value of std::is_trivially_destructible<ara::core::ErrorCode>::value. */
  /* @TE5: std::is_trivially_destructible<ara::core::ErrorCode> should return true. */
  /* @End: UT_CORE_0067 */
  EXPECT_TRUE(std::is_trivially_destructible<ara::core::ErrorCode>::value);

  /* @Start: UT_CORE_0067 */
  /* @TS6: Get the value of std::is_nothrow_destructible<ara::core::ErrorCode>::value. */
  /* @TE6: std::is_nothrow_destructible<ara::core::ErrorCode> should return true. */
  /* @End: UT_CORE_0067 */
  EXPECT_TRUE(std::is_nothrow_destructible<ara::core::ErrorCode>::value);

  /* @Start: UT_CORE_0067 */
  /* @TS7: Get the value of std::is_trivially_copyable<ara::core::ErrorCode>::value. */
  /* @TE7: std::is_trivially_copyable<ara::core::ErrorCode> should return true. */
  /* @End: UT_CORE_0067 */
  EXPECT_TRUE(std::is_trivially_copyable<ara::core::ErrorCode>::value);

  /* @Start: UT_CORE_0067 */
  /* @TS8: Get the value of std::is_standard_layout<ara::core::ErrorCode>::value. */
  /* @TE8: std::is_trivially_standard_layout<ara::core::ErrorCode> should return true. */
  /* @End: UT_CORE_0067 */
  EXPECT_TRUE(std::is_standard_layout<ara::core::ErrorCode>::value);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0068
@Test_Description:    To test the functionality of Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611441, 611444, 611445
*******************************************************************************/
TEST(ErrorCodeTest, Constructor1_WorksConstexpr)
{
  using namespace ara::core;

  constexpr ErrorDomain const& domain = GetTestErrorDomain();

  // 2-arguments ctor
  constexpr ErrorCode const error1b(TestErrc::some_error, 11);
  constexpr ErrorDomain const& errorDomain1b = error1b.Domain();
  constexpr ErrorDomain::CodeType const code1b = error1b.Value();
  constexpr ErrorDomain::SupportDataType const errorExtra1b = error1b.SupportData();
  /* @Start: UT_CORE_0068 */
  /* @TS1: Create instance ErrorCode as Domain, Value and SupportData and invoke error1b.Domain(). */
  /* @TE1: errorDomain1b should be same as reference domain. */
  /* @End: UT_CORE_0068 */
  EXPECT_THAT(errorDomain1b, Ref(domain));
  /* @Start: UT_CORE_0068 */
  /* @TS2: Invoke error1b.Domain. */
  /* @TE2: code1b should be equal to some_error. */
  /* @End: UT_CORE_0068 */
  EXPECT_THAT(code1b, Eq(static_cast<ErrorDomain::CodeType>(TestErrc::some_error)));
  /* @Start: UT_CORE_0068 */
  /* @TS3: Invoke error1b.SupportData(). */
  /* @TE3: errorExtra1b should be equal to 11. */
  /* @End: UT_CORE_0068 */
  EXPECT_THAT(errorExtra1b, Eq(11));

  // 1-argument ctor
  constexpr ErrorCode const error1c(TestErrc::some_error);
  constexpr ErrorDomain const& errorDomain1c = error1c.Domain();
  constexpr ErrorDomain::CodeType const code1c = error1c.Value();
  constexpr ErrorDomain::SupportDataType const errorExtra1c = error1c.SupportData();
  /* @Start: UT_CORE_0068 */
  /* @TS4: Create instance ErrorCode as Domain, Value and SupportData and invoke error1c.Domain() */
  /* @TE4: errorDomain1c should be same as reference domain. */
  /* @End: UT_CORE_0068 */
  EXPECT_THAT(errorDomain1c, Ref(domain));
  /* @Start: UT_CORE_0068 */
  /* @TS5: Invoke error1c.Value(). */
  /* @TE5: code1 should be equal to some_error. */
  /* @End: UT_CORE_0068 */
  EXPECT_THAT(code1c, Eq(static_cast<ErrorDomain::CodeType>(TestErrc::some_error)));
  /* @Start: UT_CORE_0068 */
  /* @TS6: Invoke error1c.SupportData(). */
  /* @TE6: errorExtra1c should be equal to 0. */
  /* @End: UT_CORE_0068 */
  EXPECT_THAT(errorExtra1c, Eq(0));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0069
@Test_Description:    To test the functionality of Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611444, 611445, 611443
*******************************************************************************/
TEST(ErrorCodeTest, Constructor2_WorksConstexpr)
{
  using namespace ara::core;

  constexpr ErrorDomain const& domain = GetTestErrorDomain();
  constexpr ErrorDomain::CodeType const code = static_cast<ErrorDomain::CodeType>(TestErrc::some_error);

  // 3-arguments ctor
  constexpr ErrorCode const error3b(code, GetTestErrorDomain(), 14);
  constexpr ErrorDomain const& errorDomain3b = error3b.Domain();
  constexpr ErrorDomain::CodeType const code3b = error3b.Value();

  constexpr ErrorDomain::SupportDataType const errorExtra3b = error3b.SupportData();
  /* @Start: UT_CORE_0069 */
  /* @TS1: Create instance ErrorCode as Domain, Value, SupportData and invoke error3b.Domain(). */
  /* @TE1: errorDomain3b should be same as reference domain. */
  /* @End: UT_CORE_0069 */
  EXPECT_THAT(errorDomain3b, Ref(domain));
  /* @Start: UT_CORE_0069 */
  /* @TS2: Invoke error3b.Domain() . */
  /* @TE2: code3b should be equal to some_error. */
  /* @End: UT_CORE_0069 */
  EXPECT_THAT(code3b, Eq(static_cast<ErrorDomain::CodeType>(TestErrc::some_error)));
  /* @Start: UT_CORE_0069 */
  /* @TS3: Invoke errorExtra3b.Value(). */
  /* @TE3: errorExtra3b should be equal to 14. */
  /* @End: UT_CORE_0069 */
  EXPECT_THAT(errorExtra3b, Eq(14));

  // 2-arguments ctor
  constexpr ErrorCode const error3c(code, GetTestErrorDomain());
  constexpr ErrorDomain const& errorDomain3c = error3c.Domain();
  constexpr ErrorDomain::CodeType const code3c = error3c.Value();
  constexpr ErrorDomain::SupportDataType const errorExtra3c = error3c.SupportData();
  /* @Start: UT_CORE_0069 */
  /* @TS4: Create instance ErrorCode as Domain, Value and SupportData and Invoke error3b.Domain(). */
  /* @TE4: errorDomain3b should be same as reference domain. */
  /* @End: UT_CORE_0069 */
  EXPECT_THAT(errorDomain3c, Ref(domain));
  /* @Start: UT_CORE_0069 */
  /* @TS5: Invoke error3c.Value(). */
  /* @TE5: code3c should be equal to some_error. */
  /* @End: UT_CORE_0069 */
  EXPECT_THAT(code3c, Eq(static_cast<ErrorDomain::CodeType>(TestErrc::some_error)));
  /* @Start: UT_CORE_0069 */
  /* @TS6: Invoke errorExtra3c.Value(). */
  /* @TE6: errorExtra3c should be equal to 0. */
  /* @End: UT_CORE_0069 */
  EXPECT_THAT(errorExtra3c, Eq(0));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0070
@Test_Description:    To test the functionality of Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611443, 611444, 611445
*******************************************************************************/
TEST(ErrorCodeTest, Constructor_Implicit_WorksConstexpr)
{
  using namespace ara::core;

  constexpr ErrorCode const error = TestErrc::some_error;

  constexpr ErrorDomain::CodeType const errorValue = error.Value();
  constexpr ErrorDomain::SupportDataType const errorExtra = error.SupportData();
  constexpr ErrorDomain const& errorDomain = error.Domain();
  /* @Start: UT_CORE_0070 */
  /* @TS1: Create instance ErrorDomain as errorValue, errorExtra, errorDomain and invoke error.Value().  */
  /* @TE1: errorValue should be equal to some_error. */
  /* @End: UT_CORE_0070 */
  EXPECT_THAT(errorValue, Eq(static_cast<ErrorDomain::CodeType>(TestErrc::some_error)));
  /* @Start: UT_CORE_0070 */
  /* @TS2: Invoke error.SupportData(). */
  /* @TE2: errorExtra should be equal to 0. */
  /* @End: UT_CORE_0070 */
  EXPECT_THAT(errorExtra, Eq(0));
  constexpr ErrorDomain const& domain = GetTestErrorDomain();
  /* @Start: UT_CORE_0070 */
  /* @TS3: Invoke error.Domain().  */
  /* @TE3: errorDomain should be same as reference doamin. */
  /* @End: UT_CORE_0070 */
  EXPECT_THAT(errorDomain, Ref(domain));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0071
@Test_Description:    To test the functionality of Comparison.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611441, 611442
*******************************************************************************/
TEST(ErrorCodeTest, Comparison_IsConstexpr)
{
  constexpr ara::core::ErrorCode const error1 = TestErrc::some_error;
  constexpr ara::core::ErrorCode const error2 = TestErrc::some_error;

  constexpr bool const errorsAreSame = (error1 == error2);
  constexpr bool const errorsAreNotSame = (error1 != error2);
  /* @Start: UT_CORE_0071 */
  /* @TS1: Create instance of ara::core::ErrorCode as error1 and error2 and Invoke errorsAreSame = (error1 == error2). */
  /* @TE1: errorsAreSame = (error1 == error2) should return true. */
  /* @End: UT_CORE_0071 */
  EXPECT_THAT(errorsAreSame, true);
  /* @Start: UT_CORE_0071 */
  /* @TS2: Invoke errorsAreNotSame = (error1 != error2). */
  /* @TE2: errorsAreNotSame = (error1 != error2) should return false. */
  /* @End: UT_CORE_0071 */
  EXPECT_THAT(errorsAreNotSame, false);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0072
@Test_Description:    To test the functionality of Equality.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611441, 611442
*******************************************************************************/
TEST(ErrorCodeTest, Equals_ConsidersDomain)
{
  ara::core::ErrorDomain::CodeType const code = 42;
  ara::core::ErrorCode const error1(code, GetTestErrorDomain());
  ara::core::ErrorCode const error2(code, ara::core::GetFutureErrorDomain());
  /* @Start: UT_CORE_0072 */
  /* @TS1: Create instance of ara::core::ErrorCode as error1, error2 and call error1() and Compare error1 and error2. */
  /* @TE1: error1 should not be equal to error2. */
  /* @End: UT_CORE_0072 */
  EXPECT_THAT(error1, Not(Eq(error2)));

  ara::core::ErrorCode const error3{static_cast<TestErrc>(code)};
  /* @Start: UT_CORE_0072 */
  /* @TS2: Create instance of ara::core::ErrorCode as error3 and call error3() and Compare error3 and error1. */
  /* @TE2: error3 should be equal to error1. */
  /* @End: UT_CORE_0072 */
  EXPECT_THAT(error3, Eq(error1));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0073
@Test_Description:    To test the functionality of Equality.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611441, 611442
*******************************************************************************/
TEST(ErrorCodeTest, Equals_ConsidersValue)
{
  ara::core::ErrorDomain::CodeType const code1 = 42;
  ara::core::ErrorDomain::CodeType const code2 = 43;
  ara::core::ErrorCode const error1(code1, GetTestErrorDomain());
  ara::core::ErrorCode const error2(code2, GetTestErrorDomain());
  /* @Start: UT_CORE_0073 */
  /* @TS1: Create instance of ara::core::ErrorCode as error1, error2, error3 and invoke error1() and Compare error1, error2. */
  /* @TE1: error1 should not be equal to error2. */
  /* @End: UT_CORE_0073 */
  EXPECT_THAT(error1, Not(Eq(error2)));

  ara::core::ErrorCode const error3(code1, GetTestErrorDomain());
  /* @Start: UT_CORE_0073 */
  /* @TS2: Invoke ara::core::ErrorCode and call error3() and Compare error3 and error1. */
  /* @TE2: error3 should be equal to error1. */
  /* @End: UT_CORE_0073 */
  EXPECT_THAT(error3, Eq(error1));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0074
@Test_Description:    To test the functionality of Equality.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611441, 611442
*******************************************************************************/
TEST(ErrorCodeTest, Equals_IgnoresSupportData)
{
  ara::core::ErrorDomain::CodeType const code = 42;
  ara::core::ErrorCode const error1(code, GetTestErrorDomain());
  ara::core::ErrorCode const error2(code, GetTestErrorDomain(), 1234);
  ara::core::ErrorCode const error3(code, GetTestErrorDomain(), 56789);
  /* @Start: UT_CORE_0074 */
  /* @TS1: Create instance of ara::core::ErrorCode as error1, error2, error3 and Compare error1 and error2. */
  /* @TE1: error1 should be equal to error2. */
  /* @End: UT_CORE_0074 */
  EXPECT_THAT(error1, Eq(error2));
  /* @Start: UT_CORE_0074 */
  /* @TS2: Compare error1 and error3. */
  /* @TE2: error1 should be equal to error3. */
  /* @End: UT_COUT_CORE_0074RE_0128 */
  EXPECT_THAT(error1, Eq(error3));
  /* @Start: UT_CORE_0074 */
  /* @TS3: Compare error2 and error3. */
  /* @TE3: error2 should be equal to error3. */
  /* @End: UT_CORE_0074 */
  EXPECT_THAT(error2, Eq(error3));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0075
@Test_Description:    To test the functionality of Copy Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611443, 611443
*******************************************************************************/
TEST(ErrorCodeTest, CopyCtor_Works)
{
  using namespace ara::core;

  ErrorCode const error1 = TestErrc::some_error;
  ErrorCode const error2(error1);
  /* @Start: UT_CORE_0075 */
  /* @TS1: Create instance of ara::core as error1, error2 and invoke Value() and Compare error1 and error2. */
  /* @TE1: error2.Value() should be equal to some_error. */
  /* @End: UT_CORE_0075 */
  EXPECT_THAT(error2.Value(), Eq(static_cast<ErrorDomain::CodeType>(TestErrc::some_error)));
}

static void Sink(ara::core::ErrorCode const&)
{
  // Nothing to be done
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0076
@Test_Description:    To test the functionality of Copy Assignment.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      537887, 611443
*******************************************************************************/
TEST(ErrorCodeTest, CopyAssignment_Works)
{
  using namespace ara::core;

  ErrorCode error1 = TestErrc::some_error;
  Sink(error1); // suppress a cppcheck redundantAssignment warning
  ErrorCode const error2 = FutureErrc::kBrokenPromise;

  error1 = error2;
  /* @Start: UT_CORE_0076 */
  /* @TS1: Create instance of ara::core::ErrorCode as error1, error2 and invoke Value() and Compare error1 and error2. */
  /* @TE1: error1.Value() should be equal to error2. */
  /* @End: UT_CORE_0076 */
  EXPECT_THAT(error1.Value(), Eq(static_cast<ErrorDomain::CodeType>(FutureErrc::kBrokenPromise)));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0077
@Test_Description:    To test the functionality of Move Constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611443
*******************************************************************************/
TEST(ErrorCodeTest, MoveCtor_Works)
{
  using namespace ara::core;

  ErrorCode const error(ErrorCode(TestErrc::some_error));
  /* @Start: UT_CORE_0077 */
  /* @TS1: Create instance ara::core and invoke Value(). */
  /* @TE1: error.Value() should be equal to some_error. */
  /* @End: UT_CORE_0077 */
  EXPECT_THAT(error.Value(), Eq(static_cast<ErrorDomain::CodeType>(TestErrc::some_error)));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0078
@Test_Description:    To test the functionality of Move Assignment.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611443
*******************************************************************************/
TEST(ErrorCodeTest, MoveAssignment_Works)
{
  using namespace ara::core;

  ErrorCode error1 = TestErrc::some_error;
  Sink(error1); // suppress a cppcheck redundantAssignment warning
  ErrorCode error2 = FutureErrc::kBrokenPromise;

  error1 = std::move(error2);
  /* @Start: UT_CORE_0078 */
  /* @TS1: Create instance of ara::core as error1, error2 invoke Value() and Compare error1 and error2. */
  /* @TE1: error1.Value() should be equal to error2. */
  /* @End: UT_CORE_0078 */
  EXPECT_THAT(error1.Value(), Eq(static_cast<ErrorDomain::CodeType>(FutureErrc::kBrokenPromise)));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0079
@Test_Description:    To test the functionality of ThrowAsException.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611448, 611440
*******************************************************************************/
TEST(ErrorCodeTest, ThrowAsException_Works)
{
  ara::core::ErrorCode const error = TestErrc::some_error;

  bool wasThrown = false;
  try
  {
    error.ThrowAsException();
  }
  catch (TestException const& ex)
  {
    wasThrown = true;
    /* @Start: UT_CORE_0079 */
    /* @TS1: Create instance of ara::core::ErrorCode as some_error and invoke Error() and Compare ex.Error and error. */
    /* @TE1: ex.Error() should be equal to error. */
    /* @End: UT_CORE_0079 */
    EXPECT_THAT(ex.Error(), Eq(error));
  }
  /* @Start: UT_CORE_0079 */
  /* @TS2: Create instance of ara::core::ErrorCode and Invoke wasThrown. */
  /* @TE2: wasThrown should return true. */
  /* @End: UT_CORE_0079 */
  EXPECT_TRUE(wasThrown);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0533
@Test_Description:    To test the functionality of Message().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611446
*******************************************************************************/
TEST(ErrorCodeTest, Message)
{
  constexpr ara::core::ErrorCode const error(TestErrc::some_error, 11);
  /* @Start: UT_CORE_0533 */
  /* @TS1: Create instance of ara::core::ErrorCode as some_error and invoke Message(). */
  /* @TE1: error.Message() should not throw any exception. */
  /* @End: UT_CORE_0533 */
  EXPECT_NO_THROW(error.Message());
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0534
@Test_Description:    To test the functionality of GetDomainId.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611447
*******************************************************************************/
TEST(ErrorCodeTest, GetDomainId)
{
  constexpr ara::core::ErrorCode const error(TestErrc::some_error, 11);
  /* @Start: UT_CORE_0534 */
  /* @TS1: Create instance of ara::core::ErrorCode as error and invoke GetDomainId(). */
  /* @TE1: error.GetDomainId() should not throw any exception. */
  /* @End: UT_CORE_0534 */
  EXPECT_NO_THROW(error.GetDomainId());
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0535
@Test_Description:    To test the functionality of ThrowOrTerminate.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ErrorCodeTest
@Requirement Tracing: -
@Design Tracing:      611826
*******************************************************************************/
TEST(ErrorCodeTest, ThrowOrTerminate)
{
  ara::core::ErrorCode error = TestErrc::some_error;

  bool wasThrown = false;
  try
  {
    ara::core::ifc::ThrowOrTerminate<TestException>(error);
  }
  catch (TestException const& ex)
  {
    wasThrown = true;

    /* @Start: UT_CORE_0535 */
    /* @TS1: Create instance of ara::core::ErrorCode as error and invoke Error() and Compare ex.Error and error. */
    /* @TE1: ex.Error() should be equal to error. */
    /* @End: UT_CORE_0535 */
    EXPECT_THAT(ex.Error(), Eq(error));
    EXPECT_TRUE(wasThrown);
  }
}