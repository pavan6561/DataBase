// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_core_error_domain.cpp
//
// Purpose     : This file provides the unit test cases of class CoreErrorDomain.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/core_error_domain.h"
#include "ara/core/error_code.h"
#include "ara/core/exception.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <type_traits>

using ::testing::Eq;
using ::testing::NotNull;
using ::testing::Ref;
/*******************************************************************************
@TestCaseId:          UT_CORE_0061
@Test_Description:    To test the functionality of Static Type Properties of CoreErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CoreErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611299
*******************************************************************************/
TEST(CoreErrorDomainTest, StaticTypeProperties)
{
  /* @Start: UT_CORE_0061 */
  /* @TS1: Get the value of std::is_enum<ara::core::CoreErrorDomain::Errc> of CoreErrorDomain. */
  /* @TE1: std::is_enum<ara::core::CoreErrorDomain::Errc> should return true. */
  /* @End: UT_CORE_0061 */
  static_assert(std::is_enum<ara::core::CoreErrorDomain::Errc>::value, "Nested type Errc is not an enum");

  /* @Start: UT_CORE_0061 */
  /* @TS2: Get the value of std::is_same<ara::core::CoreErrorDomain::Errc, ara::core::CoreErrc> of CoreErrorDomain. */
  /* @TE2: std::is_same<ara::core::CoreErrorDomain::Errc, ara::core::CoreErrc> should return true. */
  /* @End: UT_CORE_0061 */
  static_assert(std::is_same<ara::core::CoreErrorDomain::Errc, ara::core::CoreErrc>::value,
      "Nested type Errc is not the expected enum");

  /* @Start: UT_CORE_0061 */
  /* @TS3: Get the value of std::is_class<ara::core::CoreErrorDomain::Exception> of CoreErrorDomain. */
  /* @TE3: std::is_class<ara::core::CoreErrorDomain::Exception> should return true. */
  /* @End: UT_CORE_0061 */
  static_assert(std::is_class<ara::core::CoreErrorDomain::Exception>::value,
      "Nested type Exception is not a class type");

  /* @Start: UT_CORE_0061 */
  /* @TS4: Get the value of std::is_base_of<ara::core::Exception, ara::core::CoreErrorDomain::Exception> of CoreErrorDomain. */
  /* @TE4: std::is_base_of<ara::core::Exception, ara::core::CoreErrorDomain::Exception> should return true. */
  /* @End: UT_CORE_0061 */
  static_assert(std::is_base_of<ara::core::Exception, ara::core::CoreErrorDomain::Exception>::value,
      "Nested type Exception is not derived from ara::core::Exception");

  /* @Start: UT_CORE_0061 */
  /* @TS5: Get the value of std::is_final<ara::core::CoreErrorDomain> of CoreErrorDomain. */
  /* @TE5: std::is_final<ara::core::CoreErrorDomain> should return true. */
  /* @End: UT_CORE_0061 */
  static_assert(std::is_final<ara::core::CoreErrorDomain>::value, "ErrorDomain subclass is not final");

  SUCCEED();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0062
@Test_Description:    To test the functionality of Name of CoreErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CoreErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611300
*******************************************************************************/
TEST(CoreErrorDomainTest, Name)
{
  constexpr ara::core::CoreErrorDomain core;
  /* @Start: UT_CORE_0062 */
  /* @TS1: Create insatnce of ara::core::CoreErrorDomain as core and invoke the Name(). */
  /* @TE1: Name() should be equal to "Core". */
  /* @End: UT_CORE_0062 */
  EXPECT_THAT(core.Name(), Eq("Core"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0063
@Test_Description:    To test the functionality of Meessage of CoreErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CoreErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611301
*******************************************************************************/
TEST(CoreErrorDomainTest, Message)
{
  constexpr ara::core::CoreErrorDomain core;

  ara::core::CoreErrc err = ara::core::CoreErrc::kInvalidArgument;
  ara::core::ErrorDomain::CodeType num = static_cast<ara::core::ErrorDomain::CodeType>(err);
  /* @Start: UT_CORE_0063 */
  /* @TS1: Invoke Message() of ara::core::CoreErrc. */
  /* @TE1: Message() should be equal to "Invalid argument". */
  /* @End: UT_CORE_0063 */
  EXPECT_THAT(core.Message(num), Eq("Invalid argument"));

  ara::core::ErrorDomain::CodeType invalidNum = 0x12345678;
  /* @Start: UT_CORE_0063 */
  /* @TS2: Invoke Message() of ara::core::ErrorDomain::CodeType invalidNum = 0x12345678. */
  /* @TE2: Message() should return NotNull(). */
  /* @End: UT_CORE_0063 */
  EXPECT_THAT(core.Message(invalidNum), NotNull());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0064
@Test_Description:    To test the functionality of Message of CoreErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CoreErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611302
*******************************************************************************/
TEST(CoreErrorDomainTest, Throw)
{
  ara::core::ErrorCode const ec(ara::core::CoreErrc::kInvalidArgument);

  constexpr ara::core::CoreErrorDomain core;

  /* @Start: UT_CORE_0064 */
  /* @TS1: Create instance of ara::core::ErrorCode by passing the  parameter as kInvalidArgument and inovoke the ThrowAsException(). */
  /* @TE1: Message() should be equal to "Invalid argument". */
  /* @End: UT_CORE_0064 */
  EXPECT_THROW(core.ThrowAsException(ec), ara::core::CoreException);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0065
@Test_Description:    To test the functionality of CoreErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CoreErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611822, 611300
*******************************************************************************/
TEST(CoreErrorDomainTest, CoreErrorDomainAccessor)
{
  constexpr ara::core::ErrorDomain const& core = ara::core::GetCoreErrorDomain();
  /* @Start: UT_CORE_0065 */
  /* @TS1: Invoke Name() of ara::core::ErrorDomain. */
  /* @TE1: Name() should be equal to "Core". */
  /* @End: UT_CORE_0065 */
  EXPECT_THAT(core.Name(), Eq("Core"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0066
@Test_Description:    To test the functionality of ErrorCode of CoreErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CoreErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611822, 611823, 611823
*******************************************************************************/
TEST(CoreErrorDomainTest, MakeErrorCodeOverload)
{
  ara::core::ErrorDomain const& core = ara::core::GetCoreErrorDomain();

  constexpr ara::core::ErrorCode ec = MakeErrorCode(ara::core::CoreErrc::kInvalidArgument, 0);
  /* @Start: UT_CORE_0066 */
  /* @TS1: Invoke GetCoreErrorDomain() of ara::core::ErrorDomain. */
  /* @TE1: Domain() should be same as reference core. */
  /* @End: UT_CORE_0066 */
  EXPECT_THAT(ec.Domain(), Ref(core));
  /* @Start: UT_CORE_0066 */
  /* @TS2: Invoke SupportData() of ara::core::ErrorCode. */
  /* @TE2: SupportData() should be equal to 0. */
  /* @End: UT_CORE_0066 */
  EXPECT_THAT(ec.SupportData(), Eq(0));
  /* @Start: UT_CORE_0066 */
  /* @TS3: Invoke ara::core::ErrorCode by passing the parameter as kInvalidArgument and inovoke the Value(). */
  /* @TE3: Value() should be equal to kInvalidArgument. */
  /* @End: UT_CORE_0066 */
  EXPECT_THAT(
      ec.Value(), Eq(static_cast<ara::core::ErrorDomain::CodeType>(ara::core::CoreErrc::kInvalidArgument)));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0371
@Test_Description:    To test the functionality of Invalid Meta Model of CoreErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CoreErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611301
*******************************************************************************/
TEST(CoreErrorDomainTest, kInvalidMetaModelShortname)
{
  constexpr ara::core::CoreErrorDomain core;

  ara::core::CoreErrc err = ara::core::CoreErrc::kInvalidMetaModelShortname;
  ara::core::ErrorDomain::CodeType num = static_cast<ara::core::ErrorDomain::CodeType>(err);
  /* @Start: UT_CORE_0371 */
  /* @TS1: Invoke Message() of ara::core::CoreErrc. */
  /* @TE1: Message() should be equal to "Invalid meta model shortname". */
  /* @End: UT_CORE_0371 */
  EXPECT_THAT(core.Message(num), Eq("Invalid meta model shortname"));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0372
@Test_Description:    To test the functionality of Invalid Meta Model of CoreErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CoreErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611301, 611822
*******************************************************************************/
TEST(CoreErrorDomainTest, kInvalidMetaModelPath)
{
  constexpr ara::core::CoreErrorDomain core;

  ara::core::CoreErrc err = ara::core::CoreErrc::kInvalidMetaModelPath;
  ara::core::ErrorDomain::CodeType num = static_cast<ara::core::ErrorDomain::CodeType>(err);
  /* @Start: UT_CORE_0372 */
  /* @TS1: Invoke Message() of ara::core::CoreErrc. */
  /* @TE1: Message() should be equal to "Invalid meta model path". */
  /* @End: UT_CORE_0372 */
  EXPECT_THAT(core.Message(num), Eq("Invalid meta model path"));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0373
@Test_Description:    To test the functionality of Exception of CoreErrorDomain.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          CoreErrorDomainTest
@Requirement Tracing: -
@Design Tracing:      611553
*******************************************************************************/
TEST(CoreErrorDomainTest, Exception)
{
  ara::core::ErrorCode const ec(ara::core::CoreErrc::kInvalidArgument);
  ara::core::Exception obj(ec);
  obj.what();
  /* @Start: UT_CORE_0373 */
  /* @TS1: Invoke what() of ara::core::CoreErrc. */
  /* @TE1: what() should return std::exception. */
  /* @End: UT_CORE_0373 */
}