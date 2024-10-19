// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_result.cpp
//
// Purpose     : This file provides the unit test cases of class Result.
// -----------------------------------------------------------------------------------------------------------

#include "gmock_matchers.h"
#include "test_domain.h"

#include "ara/core/result.h"
#include "ara/core/error_code.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <type_traits>

using ara::core::ErrorCode;
using ara::core::Result;

using ::testing::Eq;

/*******************************************************************************
@TestCaseId:          UT_CORE_0080
@Test_Description:    To test the functionality of Static Type Properties of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611369
*******************************************************************************/

TEST(ResultTest, StaticTypeProperties)
{
  /* @Start: UT_CORE_0080 */
  /* @TS1: Get the value of std::is_same by passing the object of result of type int. */
  /* @TE1: std::is_same should return true. */
  /* @End: UT_CORE_0080 */
  static_assert(std::is_same<Result<int>::ValueType, int>::value, "wrong ValueType definition");
  static_assert(std::is_same<Result<int>::ErrorType, ErrorCode>::value, "wrong ErrorType definition");

  /* @Start: UT_CORE_0080 */
  /* @TS2: Get the value of std::is_same by passing the object of result of type int and long. */
  /* @TE2: std::is_same should return true. */
  /* @End: UT_CORE_0080 */
  static_assert(std::is_same<Result<int, long>::ValueType, int>::value, "wrong ValueType definition");
  static_assert(std::is_same<Result<int, long>::ErrorType, long>::value, "wrong ErrorType definition");

  SUCCEED();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0081
@Test_Description:    To test the functionality of Constructor of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611368
*******************************************************************************/
TEST(ResultTest, ConvertingConstructors)
{
  static_assert(std::is_constructible<Result<int>, int>::value, "Result<T> constructible from T");
  static_assert(std::is_constructible<Result<int>, ErrorCode>::value, "Result<T> constructible from E");
  static_assert(std::is_constructible<Result<int, long>, long>::value, "Result<T,E> constructible from T");
  static_assert(!std::is_constructible<Result<int, long>, ErrorCode>::value,
      "Result<T,E> not constructible from ErrorCode");

  static_assert(std::is_convertible<int, Result<int>>::value, "Result<T> implicitly convertible from T");
  static_assert(
      !std::is_convertible<ErrorCode, Result<int>>::value, "Result<T> not implicitly convertible from E");

  // Result(T const& t)
  int const value = 42;
  Result<int> r1(value);
  EXPECT_THAT(r1, ArHasValueWhichIs(42));
  Result<int> r1a = value;
  EXPECT_THAT(r1a, ArHasValueWhichIs(42));

  // Result(T&& t)
  Result<int> r2(42);
  EXPECT_THAT(r2, ArHasValueWhichIs(42));
  Result<int> r2a = 42;
  EXPECT_THAT(r2a, ArHasValueWhichIs(42));

  // explicit Result(E const& e)
  ErrorCode const ec(TestErrc::some_error);
  Result<int> r3(ec);
  EXPECT_THAT(r3, ArNoValue());
  // Result<int> r3a = ec;  // OK: does not compile!

  // explicit Result(E&& e)
  Result<int> r4(ErrorCode{TestErrc::some_error});
  EXPECT_THAT(r4, ArNoValue());
  // Result<int> r4a = ErrorCode(TestErrc::some_error);  // OK: does not compile!

  /* @End: UT_CORE_0081 */
  /* @TS1: Create an instance of the class Result as r1 and pass the value as 42. */
  /* @TE1: r1 should be conatain value as 42. */
  /* @TS2: Create an instance of the class Result as r3 and pass an error as some_error. */
  /* @TE2: r3 should not contain any value. */
  /* @End: UT_CORE_0081 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0082
@Test_Description:    To test the functionality of Copy and Move constructor of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611369
*******************************************************************************/
TEST(ResultTest, CopyAndMove)
{
  Result<int> s1(42);
  Result<int> const s3(44);

  // Result(Result const& other)
  Result<int> r1(s1);
  EXPECT_THAT(r1, ArHasValueWhichIs(42));

  /* @Start: UT_CORE_0082 */
  /* @TS1: Create the instance of Result as s1 and copy it into the another instance of Result r1. */
  /* @TE1: r1 should be have same data as the s1. */
  /* @End: UT_CORE_0082 */

  // Result(Result&& other)
  // Must make sure to prevent copy elision from taking place,
  // otherwise this will not actually call the move ctor.
  Result<int> r1tmp(s1);
  Result<int> r2(std::move(r1tmp));
  EXPECT_THAT(r2, ArHasValueWhichIs(42));
  /* @Start: UT_CORE_0082 */
  /* @TS2: Create the instance of Result as s1 and move it into the another instance of Result r1. */
  /* @TE2: r1 should be have same data as the s1. */
  /* @End: UT_CORE_0082 */

  // Result& operator=(Result const& other) = default;
  r1 = s3;
  EXPECT_THAT(r1, ArHasValueWhichIs(44));

  // Result& operator=(Result&& other) = default;
  r1 = Result<int>(45);
  EXPECT_THAT(r1, ArHasValueWhichIs(45));
  /* @Start: UT_CORE_0082 */
  /* @TS3: call the assigment operator of Result class. */
  /* @TE3: assignment operator should be able to copy the data from one resul object to another result object. */
  /* @End: UT_CORE_0082 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0083
@Test_Description:    To test the functionality of HasValue() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611519
*******************************************************************************/
TEST(ResultTest, HasValue)
{
  static_assert(std::is_constructible<bool, Result<int>>::value, "Result<T> convertible to bool");
  static_assert(
      !std::is_convertible<Result<int>, bool>::value, "Result<T> not implicitly convertible to bool");

  Result<int> const r1(42);
  Result<int> const r2(ErrorCode{TestErrc::some_error});

  // bool HasValue() const noexcept
  EXPECT_TRUE(r1.HasValue());
  EXPECT_FALSE(r2.HasValue());

  // explicit operator bool() const noexcept
  EXPECT_TRUE(bool(r1));
  EXPECT_FALSE(bool(r2));
  /* @Start: UT_CORE_0083 */
  /* @TS1: Create the instance of Result as r1 and pass the int value to it. */
  /* @TE1: r1.HasValue() should return true. */
  /* @TS2: Create the instance of Result as r2 and pass the error to it. */
  /* @TE2: r1.HasValue() should return false. */
  /* @End: UT_CORE_0083 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0084
@Test_Description:    To test the functionality of Dereference Operator of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611521
*******************************************************************************/
TEST(ResultTest, DereferenceOperator)
{
  Result<int> const r1(42);

  // T const& operator*() const&
  EXPECT_THAT(*r1, Eq(42));

  // T&& operator*() &&
  auto f = []() -> Result<std::string> { return std::string("foobar"); };
  EXPECT_THAT(*(f()), Eq("foobar"));
  /* @Start: UT_CORE_0084 */
  /* @TS1: Create the instance of Result as r1 and pass the 42 value to it. */
  /* @TE1: Dereference of r1 should return the 42. */
  /* @TS2: Invoke the lambda function and return the Result type. */
  /* @TE2: Dereference of function pointer should return string. */
  /* @End: UT_CORE_0084 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0085
@Test_Description:    To test the functionality of Dereference Void of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611522
*******************************************************************************/
TEST(ResultTest, Dereference_Void)
{
  Result<void> const r1;
  //*r1;

  SUCCEED();
  /* @Start: UT_CORE_0085 */
  /* @TS1: Create the instance of Result of void type. */
  /* @TE1: It should not throw any exception. */
  /* @End: UT_CORE_0085 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0086
@Test_Description:    To test the functionality of Ok() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611382, 611383
*******************************************************************************/
TEST(ResultTest, Ok)
{
  // Optional<T> Ok() const&

  Result<int> const r1(42);
  ara::core::Optional<int> const v1 = r1.Ok();
  EXPECT_THAT(v1.has_value(), true);
  EXPECT_THAT(*v1, 42);

  Result<int> const r2(ErrorCode{TestErrc::some_error});
  ara::core::Optional<int> const v2 = r2.Ok();
  EXPECT_THAT(v2.has_value(), false);

  // Optional<T> Ok() &&

  ara::core::Optional<int> const v3 = Result<int>(44).Ok();
  EXPECT_THAT(v3.has_value(), true);
  EXPECT_THAT(*v3, 44);

  ara::core::Optional<int> const v4 = Result<int>(ErrorCode{TestErrc::some_error}).Ok();
  EXPECT_THAT(v4.has_value(), false);
  /* @Start: UT_CORE_0086 */
  /* @TS1: assign the value of result into the ara::core::Optional. */
  /* @TE1: Optional.HasValue() should return true and value should be same as result. */
  /* @TS2: Create the instance of the result r2 with an error type and assign the r2 to the ara::core::Optional. */
  /* @TE2: Optional.HasValue() should return false. */
  /* @End: UT_CORE_0086 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0087
@Test_Description:    To test the functionality of Err() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611384, 611385
*******************************************************************************/
TEST(ResultTest, Err)
{
  // Optional<E> Err() const&

  Result<int> const r1(42);
  ara::core::Optional<ErrorCode> const v1 = r1.Err();
  EXPECT_THAT(v1.has_value(), false);

  Result<int> const r2(ErrorCode{TestErrc::some_error});
  ara::core::Optional<ErrorCode> const v2 = r2.Err();
  EXPECT_THAT(v2.has_value(), true);
  EXPECT_THAT(*v2, TestErrc::some_error);

  // Optional<E> Err() &&

  ara::core::Optional<ErrorCode> const v3 = Result<int>(44).Err();
  EXPECT_THAT(v3.has_value(), false);

  ara::core::Optional<ErrorCode> const v4 = Result<int>(ErrorCode{TestErrc::some_error}).Err();
  EXPECT_THAT(v4.has_value(), true);
  EXPECT_THAT(*v4, TestErrc::some_error);
  /* @Start: UT_CORE_0087 */
  /* @TS1: Create the instance of the result r1 with the 42 value and assign the r1 to the ara::core::optional. */
  /* @TE1: Optional.HasValue() should return false. */
  /* @TS2: Create the instance of the result r2 with an error type and assignt the r2 to the ara::core::Optional. */
  /* @TE2: Optional.HasValue() should return false. */
  /* @End: UT_CORE_0087 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0088
@Test_Description:    To test the functionality of Err Void of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611384, 611385
*******************************************************************************/
TEST(ResultTest, Err_Void)
{
  // Optional<E> Err() const&

  Result<void> const r1;
  ara::core::Optional<ErrorCode> const e1 = r1.Err();
  EXPECT_THAT(e1.has_value(), false);

  Result<void> const r2(ErrorCode{TestErrc::some_error});
  ara::core::Optional<ErrorCode> const v2 = r2.Err();
  EXPECT_THAT(v2.has_value(), true);
  EXPECT_THAT(*v2, TestErrc::some_error);

  // Optional<E> Err() &&

  ara::core::Optional<ErrorCode> const v3 = Result<void>().Err();
  EXPECT_THAT(v3.has_value(), false);

  ara::core::Optional<ErrorCode> const v4 = Result<void>(ErrorCode{TestErrc::some_error}).Err();
  EXPECT_THAT(v4.has_value(), true);
  EXPECT_THAT(*v4, TestErrc::some_error);
  /* @Start: UT_CORE_0088 */
  /* @TS1: Create the instance of the result r1 with the void type and assign the r1 to the ara::core::optional. */
  /* @TE1: Optional.HasValue() should return false. */
  /* @TS2: Create the instance of the result r2 with an error type and assignt the r2 to the ara::core::Optional. */
  /* @TE2: Optional.HasValue() should return false. */
  /* @End: UT_CORE_0088 */
}

// An enum type used as T for Result tests below
enum class ValueEnum
{
  first_value = 10,
  second_value = 11,
};
/*******************************************************************************
@TestCaseId:          UT_CORE_0089
@Test_Description:    To test the functionality of ValueOr of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611386, 611387
*******************************************************************************/
TEST(ResultTest, ValueOr)
{
  // template <typename U>
  // T ValueOr(U&& defaultValue) const&

  Result<int> const r1(42);
  int const v1 = r1.ValueOr(17);
  EXPECT_THAT(v1, Eq(42));

  Result<int> const r2(ErrorCode{TestErrc::some_error});
  int const v2 = r2.ValueOr(17);
  EXPECT_THAT(v2, Eq(17));

  // The default value is static_cast'd to the Result's value type, and this is a test
  // where that makes a difference: Using a plain int as default value for a Result
  // that holds an enum.
  Result<ValueEnum> const r3(ErrorCode{TestErrc::some_error});
  ValueEnum v3 = r3.ValueOr(11);
  EXPECT_THAT(v3, Eq(ValueEnum::second_value));

  // template <typename U>
  // T ValueOr(U&& defaultValue) &&

  int const v11 = Result<int>(42).ValueOr(17);
  EXPECT_THAT(v11, Eq(42));

  int const v12 = Result<int>(ErrorCode{TestErrc::some_error}).ValueOr(17);
  EXPECT_THAT(v12, Eq(17));

  ValueEnum const v13 = Result<ValueEnum>(ErrorCode{TestErrc::some_error}).ValueOr(11);
  EXPECT_THAT(v13, Eq(ValueEnum::second_value));

  /* @Start: UT_CORE_0089 */
  /* @TS1: Create the instance of the result r1 with the int type and assign the r1 to the ara::core::optional. */
  /* @TE1: v11 should be equal to 42. */
  /* @TS2: Create the instance of the result r2 with an error type and assignt the r2 to the ara::core::Optional. */
  /* @TE2: v12 should be equal to 17. */
  /* @End: UT_CORE_0089 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0090
@Test_Description:    To test the functionality of ValueOr() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611386, 611387
*******************************************************************************/
TEST(ResultTest, ValueOr_Void)
{
  Result<void> const r1;
  r1.ValueOr(42);
  SUCCEED();
  /* @Start: UT_CORE_0090 */
  /* @TS1: Create the instance of Result as r1 with the void type and invoke the ValueOr(). */
  /* @TE1: ValueOr() should not throw any exception. */
  /* @End: UT_CORE_0090 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0091
@Test_Description:    To test the functionality of Resolve() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611393, 611532
*******************************************************************************/
TEST(ResultTest, Resolve_Void)
{
  Result<void> const r1;
  r1.Resolve([](ErrorCode const&) { return; });
  SUCCEED();
  /* @Start: UT_CORE_0091 */
  /* @TS1: Create the instance of Result as r1 with the void type and invoke the Resolve(). */
  /* @TE1: Resolve() should not throw any exception. */
  /* @End: UT_CORE_0091 */
}
#ifndef ARA_NO_EXCEPTIONS
/*******************************************************************************
@TestCaseId:          UT_CORE_00101
@Test_Description:    To test the functionality of Resolve() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611393, 611532
*******************************************************************************/
TEST(ResultTest, ValueOrThrow_Void)
{
  Result<int> r1{10};
  r1.ValueOrThrow();
  SUCCEED();
  /* @Start: UT_CORE_00101 */
  /* @TS1: Create the instance of Result as r1 with the int type and invoke the ValueOrThrow(). */
  /* @TE1: ValueOrThrow() should  throw any exception. */
  /* @End: UT_CORE_00101 */
}
#endif // ARA_NO_EXCEPTIONS
/*******************************************************************************
@TestCaseId:          UT_CORE_0092
@Test_Description:    To test the functionality of ErrorOr() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611388, 611389
*******************************************************************************/
TEST(ResultTest, ErrorOr)
{
  // template <typename G>
  // E ErrorOr(G&& defaultError) const&

  Result<int> r1 = 42;
  EXPECT_THAT(r1.ErrorOr(ErrorCode(TestErrc::some_error)), Eq(ErrorCode(TestErrc::some_error)));

  Result<int> r2(ErrorCode(TestErrc::some_error));
  EXPECT_THAT(r2.ErrorOr(ErrorCode(TestErrc::another_error)), Eq(ErrorCode(TestErrc::some_error)));

  // template <typename G>
  // E ErrorOr(G&& defaultError) &&

  EXPECT_THAT(Result<int>(42).ErrorOr(ErrorCode(TestErrc::some_error)), Eq(ErrorCode(TestErrc::some_error)));

  EXPECT_THAT(Result<int>(ErrorCode(TestErrc::some_error)).ErrorOr(ErrorCode(TestErrc::another_error)),
      Eq(ErrorCode(TestErrc::some_error)));

  /* @Start: UT_CORE_0092 */
  /* @TS1: Create the instance of Result as r1 with the int type and invoke the ErrorOr() and pass the error. */
  /* @TE1: ErrorOr() should return the some_error. */
  /* @End: UT_CORE_0092 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0093
@Test_Description:    To test the functionality of ara::core::Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611359
*******************************************************************************/
TEST(ResultTest, ErrorOr_Void)
{
  // template <typename G>
  // E ErrorOr(G&& defaultError) const&

  Result<void> r1;
  EXPECT_THAT(r1.ErrorOr(ErrorCode(TestErrc::some_error)), Eq(ErrorCode(TestErrc::some_error)));

  Result<void> r2(ErrorCode(TestErrc::some_error));
  EXPECT_THAT(r2.ErrorOr(ErrorCode(TestErrc::another_error)), Eq(ErrorCode(TestErrc::some_error)));

  // template <typename G>
  // E ErrorOr(G&& defaultError) &&

  EXPECT_THAT(Result<void>{}.ErrorOr(ErrorCode(TestErrc::some_error)), Eq(ErrorCode(TestErrc::some_error)));

  EXPECT_THAT(Result<void>(ErrorCode(TestErrc::some_error)).ErrorOr(ErrorCode(TestErrc::another_error)),
      Eq(ErrorCode(TestErrc::some_error)));

  /* @Start: UT_CORE_0093 */
  /* @TS1: Create the instance of Result as r1 with the void type and invoke the ErrorOr() and pass the some_error. */
  /* @TE1: ErrorOr() should return the some_error. */
  /* @TS2: Create the instance of Result as r2 with the void type and invoke the ErrorOr() and pass the some_error. */
  /* @TE2: ErrorOr() should return the some_error. */
  /* @End: UT_CORE_0093 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0094
@Test_Description:    To test the functionality of CheckError() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611390
*******************************************************************************/
TEST(ResultTest, CheckError)
{
  // template <typename G>
  // bool CheckError(G&& e) const

  Result<int> const r1(1);
  Result<int> const r2(ErrorCode{TestErrc::some_error});
  Result<int> const r3(ErrorCode{TestErrc::another_error});

  bool v1a = r1.CheckError(TestErrc::some_error);
  bool v1b = r1.CheckError(TestErrc::another_error);
  EXPECT_FALSE(v1a);
  EXPECT_FALSE(v1b);

  /* @Start: UT_CORE_0094 */
  /* @TS1: Create the instance of Result as r1,r2 and r3. */
  /* @TE1: CheckError() should return false. */
  /* @TS2: Create the instance of Result as r2 with the void type and invoke the ErrorOr() and pass the some_error. */
  /* @TE2: CheckError() should return false. */
  /* @End: UT_CORE_0094 */

  bool v2a = r2.CheckError(TestErrc::some_error);
  bool v2b = r2.CheckError(TestErrc::another_error);
  EXPECT_TRUE(v2a);
  EXPECT_FALSE(v2b);
  /* @Start: UT_CORE_0094 */
  /* @TS3: Invoke the CheckError() for r2 object and check for some_error. */
  /* @TE3: CheckError() should return true. */
  /* @TS4: Invoke the CheckError() for r2 object and check for another_error. */
  /* @TE4: CheckError() should be false. */
  /* @End: UT_CORE_0094 */

  bool v3a = r3.CheckError(TestErrc::some_error);
  bool v3b = r3.CheckError(TestErrc::another_error);
  EXPECT_FALSE(v3a);
  EXPECT_TRUE(v3b);

  /* @Start: UT_CORE_0094 */
  /* @TS5: Invoke the CheckError() for r3 object and check for some_error. */
  /* @TE5: CheckError() should return false. */
  /* @TS6: Invoke the CheckError() for r3 object and check for another_error. */
  /* @TE6: CheckError() should be true. */
  /* @End: UT_CORE_0094 */
}

struct DataType
{
  int mValue;

  explicit DataType(int v) : mValue(v)
  {
  }

  int Value() const
  {
    return mValue;
  }
};
/*******************************************************************************
@TestCaseId:          UT_CORE_0095
@Test_Description:    To test the functionality of ara::core::Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611359
*******************************************************************************/
TEST(ResultTest, Bind)
{
  auto f1 = [](DataType const& data) -> Result<int> { return Result<int>::FromValue(data.Value()); };
  auto f2 = [](DataType const& data) -> int { return data.Value(); };

  // template <typename F, typename = CallableReturnsResult<F>>
  // auto Bind(F&& f) const -> decltype(f(Value()))

  Result<DataType> const r1(DataType(1));
  Result<int> const r1a = r1.Bind(f1);
  EXPECT_THAT(r1a, ArHasValueWhichIs(1));

  /* @Start: UT_CORE_0095 */
  /* @TS1: Create the instance of Result as r1 and invoke the Bind(). */
  /* @TE1: Bind should return the r1 data.*/
  /* @End: UT_CORE_0095 */
  Result<DataType> const r3(ErrorCode{TestErrc::some_error});
  Result<int> const r3a = r3.Bind(f1);
  EXPECT_THAT(r3a, ArNoValue());
  EXPECT_TRUE(r3a.CheckError(TestErrc::some_error));

  /* @Start: UT_CORE_0095 */
  /* @TS2: Create the instance of Result as r3 for ErrorCode and invoke the Bind(). */
  /* @TE2: ArNoValue should return the true and CheckError should return the true. */
  /* @End: UT_CORE_0095 */

  // template <typename F, typename = CallableReturnsNoResult<F>>
  // auto Bind(F&& f) const -> Result<decltype(f(Value())), E>

  Result<DataType> const r2(DataType(2));
  Result<int> const r2a = r2.Bind(f2);
  EXPECT_THAT(r2a, ArHasValueWhichIs(2));

  /* @Start: UT_CORE_0095 */
  /* @TS3: Create the instance of Result as r2 for value and invoke the Bind(). */
  /* @TE3: It should return the value as 2. */
  /* @End: UT_CORE_0095 */

  Result<DataType> const r4(ErrorCode{TestErrc::some_error});
  Result<int> const r4a = r4.Bind(f2);
  EXPECT_THAT(r4a, ArNoValue());
  EXPECT_TRUE(r4a.CheckError(TestErrc::some_error));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0096
@Test_Description:    To test the functionality of Bind() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611394, 611395, 611533, 611534
*******************************************************************************/
TEST(ResultTest, Bind_Void)
{
  auto f1 = []() -> Result<int> { return Result<int>::FromValue(1); };
  auto f2 = []() -> int { return 2; };

  // template <typename F, typename = CallableReturnsResult<F>>
  // auto Bind(F&& f) const -> decltype(f())

  Result<void> const r1;
  Result<int> const r1a = r1.Bind(f1);
  EXPECT_THAT(r1a, ArHasValueWhichIs(1));
  /* @Start: UT_CORE_0096 */
  /* @TS1: Create the instance of Result as r1 and invoke the Bind(). */
  /* @TE1: Bind() should return the 1. */
  /* @End: UT_CORE_0096 */
  DataType const data1(11);
  Result<int> const r1b = r1.Bind([&data1]() { return data1.Value(); });
  EXPECT_THAT(r1b, ArHasValueWhichIs(11));
  /* @Start: UT_CORE_0096 */
  /* @TS2: Invoke the Bind() of r1 and pass the function pointer to it. */
  /* @TE2: r1b should return the 11. */
  /* @End: UT_CORE_0096 */
  Result<void> const r3(ErrorCode{TestErrc::some_error});
  Result<int> const r3a = r3.Bind(f1);
  EXPECT_THAT(r3a, ArNoValue());
  EXPECT_TRUE(r3a.CheckError(TestErrc::some_error));
  /* @Start: UT_CORE_0096 */
  /* @TS3: Invoke the Bind() of r1 and pass the function pointer to it. */
  /* @TE3: r1b should return the 11. */
  /* @End: UT_CORE_0096 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0097
@Test_Description:    To test the functionality of Swap.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611518, 611372
*******************************************************************************/
TEST(ResultTest, Swap)
{
  // Result<void> r1(ErrorCode{TestErrc::some_error});
  // Result<void> r2(ErrorCode{TestErrc::another_error});
  //std::cout<<"Error->"<<r1.Error()<<std::endl;
  //std::cout<<"Error->"<<r2.Error()<<std::endl;
  //r1.Swap(r2);
  //std::cout<<"Error->"<<r1.Error()<<std::endl;
  //std::cout<<"Error->"<<r2.Error()<<std::endl;
  // EXPECT_TRUE(r1.CheckError(TestErrc::some_error));
  // EXPECT_TRUE(r2.CheckError(TestErrc::another_error));

  Result<int> r1(ErrorCode{TestErrc::some_error});
  Result<int> r2(ErrorCode{TestErrc::another_error});
  EXPECT_TRUE(r1.CheckError(TestErrc::some_error));
  EXPECT_TRUE(r2.CheckError(TestErrc::another_error));
  r1.Swap(r2);
  EXPECT_TRUE(r1.CheckError(TestErrc::another_error));
  EXPECT_TRUE(r2.CheckError(TestErrc::some_error));
  /* @Start: UT_CORE_0097 */
  /* @TS1: Get the value of r1.CheckError(TestErrc::some_error). */
  /* @TE1: Result should be return true. */
  /* @TS2: Get the value of r1.CheckError(TestErrc::another_error). */
  /* @TE2: Result should be return true. */
  /* @TS3: Get the value of r1.CheckError(TestErrc::another_error). */
  /* @TE3: Result should be return true. */
  /* @TS4: Get the value of r1.CheckError(TestErrc::some_error). */
  /* @TE4: Result should be return true. */
  /* @End: UT_CORE_0097 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0098
@Test_Description:    To test the functionality of Swap.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611372, 611518
*******************************************************************************/
TEST(ResultTest, FromError_copy)
{
  // static Result FromError(E&& e)
  Result<void> r1(ErrorCode{TestErrc::some_error});
  Result<void> r2 = r1.FromError(ErrorCode{TestErrc::another_error});
  EXPECT_TRUE(r2.CheckError(TestErrc::another_error));
  /* @Start: UT_CORE_0098 */
  /* @TS1: Get the value of r2.CheckError(TestErrc::another_error). */
  /* @TE1: Result should be return true. */
  /* @End: UT_CORE_0098 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_00099
@Test_Description:    To test the functionality of Swap.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611372, 611518
*******************************************************************************/
TEST(ResultTest, FromError_move)
{
  // static Result FromError(E&& e)
  Result<void> r1(ErrorCode{TestErrc::some_error});
  Result<void> r2 = r1.FromError(std::move(ErrorCode{TestErrc::another_error}));
  EXPECT_TRUE(r2.CheckError(TestErrc::another_error));
  /* @Start: UT_CORE_00099 */
  /* @TS1: Get the value of r2.CheckError(TestErrc::another_error). */
  /* @TE1: Result should be return true. */
  /* @End: UT_CORE_00099 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0551
@Test_Description:    To test the functionality of operator==().
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611851, 611853, 611857
*******************************************************************************/
TEST(ResultTest, EqualityOperator1)
{
  // bool operator==(Result<T, E> const& lhs, Result<T, E> const& rhs)
  Result<int> r1{100};
  Result<int> r2{100};
  /* @Start: UT_CORE_0551 */
  /* @TS1: Invoke operator==() of an Result. */
  /* @TE1: operator==() should return true. */
  /* @End: UT_CORE_0551 */
  EXPECT_THAT(r1 == r2, Eq(true));

  Result<int> const r3(ErrorCode{TestErrc::some_error});
  Result<int> const r4(ErrorCode{TestErrc::another_error});
  /* @Start: UT_CORE_0551 */
  /* @TS2: Invoke operator==() of an Result. */
  /* @TE2: operator==() should return false. */
  /* @End: UT_CORE_0551 */
  EXPECT_THAT(r3 == r4, Eq(false));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0552
@Test_Description:    To test the functionality of operator!=().
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611852, 611855, 611856, 611860
*******************************************************************************/
TEST(ResultTest, NotEqualityOperator1)
{
  // bool operator!=(Result<T, E> const& lhs, Result<T, E> const& rhs)
  Result<int> r1{10};
  Result<int> r2{20};
  /* @Start: UT_CORE_0552 */
  /* @TS1: Invoke operator!=() of an Result. */
  /* @TE1: operator!=() should return true. */
  /* @End: UT_CORE_0552 */
  EXPECT_THAT(r1 != r2, Eq(true));

  Result<int> r3{10};
  Result<int> r4{0};
  /* @Start: UT_CORE_0552 */
  /* @TS2: Invoke operator!=() of an Result. */
  /* @TE2: operator!=() should return true. */
  /* @End: UT_CORE_0552 */
  EXPECT_THAT(r3 != r4, Eq(true));

  Result<int> const r5(ErrorCode{TestErrc::some_error});
  Result<int> const r6(ErrorCode{TestErrc::another_error});
  /* @Start: UT_CORE_0552 */
  /* @TS3: Invoke operator!=() of an Result. */
  /* @TE3: operator!=() should return true. */
  /* @End: UT_CORE_0552 */
  EXPECT_THAT(r5 != r6, Eq(true));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0553
@Test_Description:    To test the functionality of operator==().
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611851, 611853, 611857
*******************************************************************************/
TEST(ResultTest, EqualityOperator2)
{
  // bool operator==(Result<T, E> const& lhs, T const& value)
  Result<int> r1{10};
  Result<int> r2{20};
  /* @Start: UT_CORE_0553 */
  /* @TS1: Invoke operator==() for r1 and r2. */
  /* @TE1: r1 should be equal to 20 and return false. */
  /* @End: UT_CORE_0553 */
  EXPECT_FALSE(r1 == 20);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0554
@Test_Description:    To test the functionality of operator==().
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611851, 611853, 611857
*******************************************************************************/
TEST(ResultTest, EqualityOperator3)
{
  // bool operator==(T const& value, Result<T, E> const& rhs)
  Result<int> r1{10};
  Result<int> r2{20};
  /* @End: UT_CORE_0554 */
  /* @TS1: Invoke operator==() for r1 and r2. */
  /* @TE1: r2 should be equal to 10 and return false. */
  /* @End: UT_CORE_0554 */
  EXPECT_FALSE(10 == r2);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0555
@Test_Description:    To test the functionality of operator!=().
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611852, 611855, 611856, 611860
*******************************************************************************/
TEST(ResultTest, NotEqualityOperator2)
{
  // bool operator!=(Result<T, E> const& lhs, T const& value)
  Result<int> r1{10};
  Result<int> r2{20};
  /* @End: UT_CORE_0555 */
  /* @TS1: Invoke operator!=() for r1 and r2. */
  /* @TE1: operator!=() should return true. */
  /* @End: UT_CORE_0555 */
  EXPECT_THAT(r1 != 20, Eq(true));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0556
@Test_Description:    To test the functionality of operator!=().
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:       611852, 611855, 611856, 611860
*******************************************************************************/
TEST(ResultTest, NotEqualityOperator3)
{
  // bool operator!=(T const& value, Result<T, E> const& rhs)
  Result<int> r1{10};
  Result<int> r2{20};
  /* @End: UT_CORE_0556 */
  /* @TS1: Invoke operator!=() for r1 and r2. */
  /* @TE1: operator!=() should return true. */
  /* @End: UT_CORE_0556 */
  EXPECT_THAT(10 != r2, Eq(true));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0557
@Test_Description:    To test the functionality of Converting Constructor of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611361, 611362, 611363, 611364
*******************************************************************************/
TEST(ResultTest, ConvertingConstructor_TEST)
{
  //Result(E&& e) : mData(std::move(e))
  Result<int> r1(ErrorCode{TestErrc::some_error});
  /* @End: UT_CORE_0557 */
  /* @TS1: Create an instance of the class Result as r1 and pass an error as some_error. */
  /* @TE1: r1 should not contain any value. */
  /* @End: UT_CORE_0557 */
  EXPECT_THAT(r1, ArNoValue());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0559
@Test_Description:    To test the functionality of Swap of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611372, 611518
*******************************************************************************/
TEST(ResultTest, Swap2)
{
  Result<void> r1(ErrorCode{TestErrc::some_error});
  Result<void> r2(ErrorCode{TestErrc::another_error});
  r1.Swap(r2);
  /* @Start: UT_CORE_0559 */
  /* @TS1: Get the value of r1.CheckError(TestErrc::some_error). */
  /* @TE1: Result should be return false. */
  /* @TS2: Get the value of r2.CheckError(TestErrc::another_error). */
  /* @TE2: Result should be return false. */
  /* @End: UT_CORE_0559 */
  EXPECT_FALSE(r1.CheckError(TestErrc::some_error));
  EXPECT_FALSE(r2.CheckError(TestErrc::another_error));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0560
@Test_Description:    To test the functionality of FromError of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611363, 611364, 611365, 611509, 611510, 611511
*******************************************************************************/
TEST(ResultTest, FromError)
{
  // static Result FromError(E&& e)
  Result<int> r3(ErrorCode{TestErrc::some_error});
  Result<int> r4 = r3.FromError((ErrorCode{TestErrc::another_error}));
  /* @Start: UT_CORE_0560 */
  /* @TS1: Get the value of r4.CheckError(TestErrc::another_error). */
  /* @TE1: Result should be return true. */
  /* @End: UT_CORE_0560 */
  EXPECT_TRUE(r4.CheckError(TestErrc::another_error));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0561
@Test_Description:    To test the functionality of operator==() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611851, 611853, 611857
*******************************************************************************/
TEST(ResultTest, EqualityOperator4)
{
  // bool operator==(Result<T, E> const& lhs, E const& error)
  Result<int> const r1(ErrorCode{TestErrc::some_error});
  Result<int> const r2(ErrorCode{TestErrc::another_error});
  /* @End: UT_CORE_0561 */
  /* @TS1: Invoke operator==() for r1 and r2. */
  /* @TE1: operator==() should return true. */
  /* @End: UT_CORE_0561 */
  EXPECT_THAT(r1 == (r2, ErrorCode{TestErrc::some_error}), true);

  Result<int> const r3(ErrorCode{TestErrc::some_error});
  Result<int> const r4(ErrorCode{TestErrc::some_error});
  /* @End: UT_CORE_0561 */
  /* @TS2: Invoke operator==() for r3 and r4. */
  /* @TE2: operator==() should return false. */
  /* @End: UT_CORE_0561 */
  EXPECT_THAT(r3 == (r4, ErrorCode{TestErrc::another_error}), false);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0562
@Test_Description:    To test the functionality of operator!=() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611852, 611855, 611856, 611860
*******************************************************************************/
TEST(ResultTest, NotEqualityOperator4)
{
  // bool operator!=(Result<T, E> const& lhs, E const& error)
  Result<int> const r3(ErrorCode{TestErrc::some_error});
  Result<int> const r4(ErrorCode{TestErrc::another_error});
  /* @End: UT_CORE_0562 */
  /* @TS1: Invoke operator!=() for r3 and r4. */
  /* @TE1: operator!=() should return false. */
  /* @End: UT_CORE_0562 */
  EXPECT_THAT(r3 != (r4, ErrorCode{TestErrc::some_error}), false);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0563
@Test_Description:    To test the functionality of operator==() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611851, 611853, 611857
*******************************************************************************/
TEST(ResultTest, EqualityOperator5)
{
  // bool operator==(E const& error, Result<T, E> const& rhs)
  Result<int> const r1(ErrorCode{TestErrc::some_error});
  Result<int> const r2(ErrorCode{TestErrc::another_error});
  /* @End: UT_CORE_0563 */
  /* @TS1: Invoke operator==() for r1 and r2. */
  /* @TE1: operator==() should return false. */
  /* @End: UT_CORE_0563 */
  EXPECT_FALSE((ErrorCode{TestErrc::another_error} == r2, r1));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0564
@Test_Description:    To test the functionality of operator!=() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611852, 611855, 611856, 611859, 611860
*******************************************************************************/
TEST(ResultTest, NonEqualityOperator5)
{
  // bool operator!=(E const& error, Result<T, E> const& rhs)
  Result<int> const r1(ErrorCode{TestErrc::some_error});
  Result<int> const r2(ErrorCode{TestErrc::another_error});
  /* @End: UT_CORE_0564 */
  /* @TS1: Invoke operator!=() for r1 and r2. */
  /* @TE1: operator!=() should return false. */
  /* @End: UT_CORE_0564 */
  EXPECT_FALSE((ErrorCode{TestErrc::another_error} != r2, r1));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0566
@Test_Description:    To test the functionality of FromValue() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611360, 611361, 611362, 611363
*******************************************************************************/
TEST(ResultTest, FromValue1)
{
  // static Result FromValue(T const& t)
  Result<int> r1{40};
  Result<int> r2{50};
  EXPECT_TRUE(r2.FromValue(r1.Value()));
  /* @Start: UT_CORE_0566 */
  /* @TS1: Create an instance of the class Result as r1 and r2. */
  /* @TE1: Instance of Result should return true. */
  /* @End: UT_CORE_0566 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0567
@Test_Description:    To test the functionality of EmplaceError() of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611517
*******************************************************************************/
TEST(ResultTest, EmplaceError)
{
  // void EmplaceError(Args&&... args)
  Result<int> r1{ErrorCode{TestErrc::another_error}};
  Result<int> r2{50};
  /* @Start: UT_CORE_0567 */
  /* @TS1: Get the value of r2.EmplaceError(ErrorCode(TestErrc::another_error)). */
  /* @TE1: Result should be return successfully. */
  /* @End: UT_CORE_0567 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0570
@Test_Description:    To test the functionality of FromValue of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611360, 611361, 611362, 611508
*******************************************************************************/
TEST(ResultTest, FromValue2)
{
  // static Result FromValue(Args&&... args)
  Result<int> r1{40};
  r1.FromValue();
  /* @Start: UT_CORE_0570 */
  /* @TS1: Create an Result<int> r1 and invoke FromValue(). */
  /* @TE1: Instance of Result should be created successfully. */
  /* @End: UT_CORE_0570 */
  SUCCEED();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0571
@Test_Description:    To test the functionality of Resolve of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611393, 611532
*******************************************************************************/
TEST(ResultTest, Resolve)
{
  Result<int> const r1{20};
  r1.Resolve([](ErrorCode const&) { return 0; });
  /* @Start: UT_CORE_0571 */
  /* @TS1: Create an Result<int> const r1{20}. */
  /* @TE1: Instance of Result should be created. */
  /* @End: UT_CORE_0571 */
  SUCCEED();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0572
@Test_Description:    To test the functionality of Error_move of Result.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ResultTest
@Requirement Tracing: -
@Design Tracing:      611359 
*******************************************************************************/
TEST(ResultTest, Error_copy)
{
  // E const& Error() const&
  Result<void> const r1(ErrorCode(TestErrc::some_error));
  ErrorCode e = r1.Error();
  EXPECT_TRUE(e == (TestErrc::some_error));
  /* @Start: UT_CORE_0572 */
  /* @TS1: Create an  Result<void> const r1(ErrorCode(TestErrc::some_error)). */
  /* @TE1: Instance of Result should be created. */
  /* @End: UT_CORE_0572 */
  SUCCEED();
}