// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_string.cpp
//
// Purpose     : This file provides the unit test cases for String class.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/string.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <vector>
#include <string>
#include <cstring>

using ::testing::Eq;
using ::testing::IsEmpty;
using ::testing::Lt;
using ::testing::Ne;
using ::testing::StrEq;

// A simple string class that implicitly converts to ara::core::StringView
namespace
{
class MyText
{
  char* mText;

 public:
  explicit MyText(char const* text) : mText(strdup(text))
  {
  }

  ~MyText()
  {
    free(mText);
  }

  MyText(MyText const&) = delete;
  MyText(MyText&&) = delete;
  MyText& operator=(MyText const&) = delete;
  MyText& operator=(MyText&&) = delete;

  operator ara::core::StringView() const noexcept
  {
    return mText;
  }
};

} // namespace
/*******************************************************************************
@TestCaseId:          UT_CORE_2010
@Test_Description:    To test the functionality of type is available and compilable.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611645, 611646, 611647, 611648, 611649, 611650
*******************************************************************************/
TEST(StringTest, TypeIsAvailableAndCompilable)
{
  ara::core::String s;
  /* @Start: UT_CORE_2010 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Instance of string should be created. */
  /* @End: UT_CORE_2010 */

  SUCCEED();
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2011
@Test_Description:    To test the functionality of default constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611651, 611652, 611653, 611654, 611655
*******************************************************************************/
TEST(StringTest, DefaultConstructor)
{
  ara::core::String s;

  ASSERT_THAT(s, IsEmpty());
  ASSERT_THAT(s, Eq(""));
  /* @Start: UT_CORE_2011 */
  /* @TS1: Create an ara::core::String empty data type with s variable. */
  /* @TE1.1: IsEmpty() should return true. */
  /* @TE1.2: Result should be equal to "". */
  /* @End: UT_CORE_2011 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2012
@Test_Description:    To test the functionality of constructor from stringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611651, 611652, 611653, 611654, 611655
*******************************************************************************/
TEST(StringTest, ConstructFromStringView)
{
  ara::core::StringView const sv = "my stringview";

  ara::core::String const s(sv);
  /* @Start: UT_CORE_2012 */
  /* @TS1: Create an ara::core::String const data type with s variable. */
  /* @TE1: Result should be equal to "my stringview". */
  /* @End: UT_CORE_2012 */
  ASSERT_THAT(s, Eq("my stringview"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2013
@Test_Description:    To test the functionality of construct from type ConvertibleToStringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611651, 611652, 611653, 611654, 611655
*******************************************************************************/
TEST(StringTest, ConstructFromTypeConvertibleToStringView)
{
  char const text[] = "my array";

  ara::core::String const s(text);
  /* @Start: UT_CORE_2013 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "my array". */
  /* @End: UT_CORE_2013 */
  ASSERT_THAT(s, Eq("my array"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2014
@Test_Description:    To test the functionality of construct from type ConvertibleToStringViewPart.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611651, 611652, 611653, 611654, 611655
*******************************************************************************/
TEST(StringTest, ConstructFromTypeConvertibleToStringViewPart)
{
  char const text[] = "my array";

  ara::core::String const s(text, 3, 5);
  /* @Start: UT_CORE_2014 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "array". */
  /* @End: UT_CORE_2014 */
  ASSERT_THAT(s, Eq("array"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2015
@Test_Description:    To test the functionality of construct from StdString.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611651, 611652, 611653, 611654, 611655
*******************************************************************************/
TEST(StringTest, ConstructFromStdString)
{
  std::string const std = "my string";

  ara::core::String const s(std);
  /* @Start: UT_CORE_2015 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "my string". */
  /* @End: UT_CORE_2015 */
  ASSERT_THAT(s, Eq("my string"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2016
@Test_Description:    To test the functionality of AssignStandard.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611651, 611652, 611653, 611654, 611655
*******************************************************************************/
TEST(StringTest, AssignStandard)
{
  ara::core::String s;
  s.assign("my text", 7);
  /* @Start: UT_CORE_2016 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "my text". */
  /* @End: UT_CORE_2016 */
  ASSERT_THAT(s, Eq("my text"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2017
@Test_Description:    To test the functionality of assign from StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611651, 611652, 611653, 611654, 611655
*******************************************************************************/
TEST(StringTest, AssignFromStringView)
{
  ara::core::StringView const sv = "my stringview";

  ara::core::String s;
  s.assign(sv);
  /* @Start: UT_CORE_2017 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "my stringview". */
  /* @End: UT_CORE_2017 */
  ASSERT_THAT(s, Eq("my stringview"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2018
@Test_Description:    To test the functionality of assign from StringViewPart.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611651, 611652, 611653, 611654, 611655
*******************************************************************************/
TEST(StringTest, AssignFromStringViewPart)
{
  ara::core::StringView const sv = "my stringview";

  ara::core::String s;
  s.assign(sv, 3, 6);
  /* @Start: UT_CORE_2018 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "string". */
  /* @End: UT_CORE_2018 */
  ASSERT_THAT(s, Eq("string"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2019
@Test_Description:    To test the functionality of append standard.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611789, 611676
*******************************************************************************/
TEST(StringTest, AppendStandard)
{
  ara::core::String s = "test ";
  s.append("my text");
  /* @Start: UT_CORE_2019 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test my text". */
  /* @End: UT_CORE_2019 */
  ASSERT_THAT(s, Eq("test my text"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2020
@Test_Description:    To test the functionality of append StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611677
*******************************************************************************/
TEST(StringTest, AppendStringView)
{
  ara::core::StringView const sv = "my stringview";

  ara::core::String s = "test ";
  s.append(sv);
  /* @Start: UT_CORE_2020 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test my stringview". */
  /* @End: UT_CORE_2020 */
  ASSERT_THAT(s, Eq("test my stringview"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2021
@Test_Description:    To test the functionality of append StringViewPart.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611678
*******************************************************************************/
TEST(StringTest, AppendStringViewPart)
{
  char const text[] = "my char array";

  ara::core::String s = "test ";
  s.append(text, 3, 4);
  /* @Start: UT_CORE_2021 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test char". */
  /* @End: UT_CORE_2021 */
  ASSERT_THAT(s, Eq("test char"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2022
@Test_Description:    To test the functionality of copy assign std string.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611647
*******************************************************************************/
TEST(StringTest, CopyAssignStdString)
{
  std::string const std = "my string";

  ara::core::String s;
  s = std;
  /* @Start: UT_CORE_2022 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "my string". */
  /* @End: UT_CORE_2022 */
  ASSERT_THAT(s, Eq("my string"));
}

// TEST(StringTest, MoveAssignStdString)
/*******************************************************************************
@TestCaseId:          UT_CORE_2022
@Test_Description:    To test the functionality of copy assign StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611649
*******************************************************************************/
TEST(StringTest, CopyAssignStringView)
{
  ara::core::StringView const sv = "my stringview";

  ara::core::String s;
  s = sv;
  /* @Start: UT_CORE_2022 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "my stringview". */
  /* @End: UT_CORE_2022 */
  ASSERT_THAT(s, Eq("my stringview"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2024
@Test_Description:    To test the functionality of append operator char.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611659
*******************************************************************************/
TEST(StringTest, AppendOperatorChar)
{
  ara::core::String s = "test ";
  s += 'x';
  /* @Start: UT_CORE_2024 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test x". */
  /* @End: UT_CORE_2024 */
  ASSERT_THAT(s, Eq("test x"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2025
@Test_Description:    To test the functionality of append operator stringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611660
*******************************************************************************/
TEST(StringTest, AppendOperatorStringView)
{
  ara::core::StringView const sv = "my stringview";

  ara::core::String s = "test ";
  s += sv;
  /* @Start: UT_CORE_2025 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test my stringview". */
  /* @End: UT_CORE_2025 */
  ASSERT_THAT(s, Eq("test my stringview"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2026
@Test_Description:    To test the functionality of implicit ConversionToStringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611661
*******************************************************************************/
TEST(StringTest, ImplicitConversionToStringView)
{
  ara::core::String s = "my string";

  ara::core::StringView sv = s;
  /* @Start: UT_CORE_2026 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "my string". */
  /* @End: UT_CORE_2026 */
  ASSERT_THAT(sv, Eq("my string"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2027
@Test_Description:    To test the functionality of InsertStandard.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611647
*******************************************************************************/
TEST(StringTest, InsertStandard)
{
  ara::core::String s = "test !";
  s.insert(5, "text");
  /* @Start: UT_CORE_2027 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "my string". */
  /* @End: UT_CORE_2027 */
  ASSERT_THAT(s, Eq("test text!"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2028
@Test_Description:    To test the functionality of InsertStringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611648
*******************************************************************************/
TEST(StringTest, InsertStringView)
{
  ara::core::StringView const sv = "stringview";

  ara::core::String s = "test !";
  s.insert(5, sv);
  /* @Start: UT_CORE_2028 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test stringview!". */
  /* @End: UT_CORE_2028 */
  ASSERT_THAT(s, Eq("test stringview!"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2029
@Test_Description:    To test the functionality of InsertStringViewPart.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611648
*******************************************************************************/
TEST(StringTest, InsertStringViewPart)
{
  char const text[] = "a char array";

  ara::core::String s = "test !";
  s.insert(5, text, 2, 4);
  /* @Start: UT_CORE_2029 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test char!". */
  /* @End: UT_CORE_2029 */
  ASSERT_THAT(s, Eq("test char!"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2030
@Test_Description:    To test the functionality of replacing Standard.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611649, 611650, 611651, 611652, 611653
*******************************************************************************/
TEST(StringTest, ReplaceStandard)
{
  ara::core::String s = "test stuff!";
  s.replace(5, 5, "code");
  /* @Start: UT_CORE_2030 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test code!". */
  /* @End: UT_CORE_2030 */
  ASSERT_THAT(s, Eq("test code!"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2031
@Test_Description:    To test the functionality of replaceing StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611649, 611650, 611651, 611652, 611653
*******************************************************************************/
TEST(StringTest, ReplaceStringView)
{
  ara::core::StringView const sv = "stringview";

  ara::core::String s = "test stuff!";
  s.replace(5, 5, sv);
  /* @Start: UT_CORE_2031 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test stringview!". */
  /* @End: UT_CORE_2031 */
  ASSERT_THAT(s, Eq("test stringview!"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2032
@Test_Description:    To test the functionality of replaceing StringViewPart.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611649, 611650, 611651, 611652, 611653
*******************************************************************************/
TEST(StringTest, ReplaceStringViewPart)
{
  char const text[] = "a char array";

  ara::core::String s = "test stuff!";
  s.replace(5, 5, text, 2, 4);
  /* @Start: UT_CORE_2032 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test char!". */
  /* @End: UT_CORE_2032 */
  ASSERT_THAT(s, Eq("test char!"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2033
@Test_Description:    To test the functionality of replaceing via Iterators.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611649, 611650, 611651, 611652, 611653
*******************************************************************************/
TEST(StringTest, ReplaceViaIterators)
{
  ara::core::StringView const sv = "stringview";

  ara::core::String s = "test stuff!";
  ara::core::String::ConstIterator i1 = s.begin() + 5;
  ara::core::String::ConstIterator i2 = s.begin() + 10;
  s.replace(i1, i2, sv);
  /* @Start: UT_CORE_2033 */
  /* @TS1: Create an ara::core::String data type with s variable. */
  /* @TE1: Result should be equal to "test stringview!". */
  /* @End: UT_CORE_2033 */
  ASSERT_THAT(s, Eq("test stringview!"));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2034
@Test_Description:    To test the functionality of compare standard.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611889, 611895, 611901, 611907
*******************************************************************************/
TEST(StringTest, CompareStandard)
{
  ara::core::String s1 = "string";
  ASSERT_THAT(s1.compare("string"), Eq(0));
  /* @Start: UT_CORE_2034 */
  /* @TS1: Invoke compare() with s1.  */
  /* @TE1: compare() should return false. */
  /* @End: UT_CORE_2034 */
  ASSERT_THAT(s1.compare(1, 3, "trip", 0, 3), Eq(0));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2035
@Test_Description:    To test the functionality of compare with StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611889, 611895, 611901, 611907
*******************************************************************************/
TEST(StringTest, CompareWithStringView)
{
  ara::core::StringView const sv = "stringview";

  ara::core::String s1 = "string";
  /* @Start: UT_CORE_2035 */
  /* @TS1: Invoke compare() with s1 and sv.  */
  /* @TE1: compare() should return false. */
  /* @End: UT_CORE_2035 */
  ASSERT_THAT(s1.compare(sv), Lt(0));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2036
@Test_Description:    To test the functionality of compare part with StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611889, 611895, 611901, 611907
*******************************************************************************/
TEST(StringTest, ComparePartWithStringView)
{
  ara::core::StringView const sv = "string";

  ara::core::String s1 = "a string var";
  /* @Start: UT_CORE_2036 */
  /* @TS1: Invoke compare() with s1 and sv.  */
  /* @TE1: compare() should return true. */
  /* @End: UT_CORE_2036 */
  ASSERT_THAT(s1.compare(2, 6, sv), Eq(0));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2037
@Test_Description:    To test the functionality of compare part with StringViewPart.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611889, 611895, 611901, 611907
*******************************************************************************/
TEST(StringTest, ComparePartWithStringViewPart)
{
  MyText text("text with string seq");

  ara::core::String s1 = "a string var";
  /* @Start: UT_CORE_2037 */
  /* @TS1: Invoke compare() with s1.  */
  /* @TE1: compare() should return false. */
  /* @End: UT_CORE_2037 */
  ASSERT_THAT(s1.compare(2, 6, text, 10, 6), Eq(0));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2038
@Test_Description:    To test the functionality of compare.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611889, 611895, 611901, 611907, 611760, 611761, 611764, 611765
*******************************************************************************/
TEST(StringTest, compare)
{
  ara::core::String s1 = "a string var";
  ara::core::internal::basic_string<char> s2("a string var");
  std::basic_string<char> s3("a string var");
  EXPECT_NO_THROW((s1.compare(1, 1, s1)));
  EXPECT_NO_THROW(s1.compare(1, 1, s1, 0, 2));
  EXPECT_NO_THROW(s1.compare(1, 1, s3));
  EXPECT_NO_THROW(s1.compare(1, 1, s3, 0, 2));
  EXPECT_NO_THROW(s1.compare(1, 1, "ABCD"));
  EXPECT_NO_THROW(s1.compare(1, 1, "ABCD", 1));
  /* @Start: UT_CORE_2038 */
  /* @TS1: Invoke compare(1, 1, s1) with s1. */
  /* @TE1: compare() should not throw the exception. */
  /* @TS2: Invoke compare(1, 1, s1, 0, 2) with s1. */
  /* @TE2: compare() should not throw the exception. */
  /* @TS3: Invoke compare(1, 1, s3) with s1. */
  /* @TE3: compare() should not throw the exception. */
  /* @TS4: Invoke compare(1, 1, s3, 0, 2) with s1. */
  /* @TE4: compare() should not throw the exception. */
  /* @TS5: Invoke compare(1, 1, "ABCD") with s1. */
  /* @TE5: compare() should not throw the exception. */
  /* @TS6: Invoke compare(1, 1, "ABCD", 1) with s1. */
  /* @TE6: compare() should not throw the exception. */
  /* @End: UT_CORE_2038 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2039
@Test_Description:    To test the functionality of find standard.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611730, 611731, 611732, 611733, 611734, 611735, 611736
*******************************************************************************/
TEST(StringTest, FindStandard)
{
  ara::core::String const s = "some string value";
  ara::core::String const s1 = "some";
  ASSERT_THAT(s.find('t'), Eq(6));
  ASSERT_THAT(s.rfind('t'), Eq(6));
  ASSERT_THAT(s.find_first_of('t'), Eq(6));
  ASSERT_THAT(s.find_first_not_of('t'), Eq(0));
  ASSERT_THAT(s.find_last_of('t'), Eq(6));
  ASSERT_THAT(s.find_last_not_of('t'), Eq(16));
  ASSERT_THAT(s.find(s1, 0), Eq(0));
  ASSERT_THAT(s.find("Some", 0, 0), Eq(0));
  ASSERT_THAT(s.find("some", 0), Eq(0));
  /* @Start: UT_CORE_2039 */
  /* @TS1: Invoke find('t') with s. */
  /* @TE1: find() should return 6. */
  /* @TS2: Invoke rfind('t') with s. */
  /* @TE2: rfind() should return 6. */
  /* @TS3: Invoke find_first_of('t') with s. */
  /* @TE3: find_first_of() should be equal to 6. */
  /* @TS4: Invoke find_first_not_of('t') with s. */
  /* @TE4: find_first_not_of() should be equal to 0. */
  /* @TS5: Invoke find_last_of('t') with s. */
  /* @TE5: find_last_of() should be equal to 6. */
  /* @TS6: Invoke find_last_not_of('t') with s. */
  /* @TE6: find_last_not_of() should be equal to 16. */
  /* @TS7: Invoke find(s1, 0) with s. */
  /* @TE7: find() should be equal to 0. */
  /* @TS8: Invoke find("Some", 0, 0) with s. */
  /* @TE8: find()  should be equal to 0. */
  /* @TS9: Invoke find("some", 0) with s. */
  /* @TE9: find()  should be equal to 0. */
  /* @End: UT_CORE_2039 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2040
@Test_Description:    To test the functionality of find StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611737, 611738, 611739
*******************************************************************************/
TEST(StringTest, FindStringView)
{
  ara::core::StringView const sv = "string";

  ara::core::String const s = "some string value";
  /* @Start: UT_CORE_2040 */
  /* @TS1: Invoke find(sv) with s variable.  */
  /* @TE1: find() should be equal to 5. */
  /* @End: UT_CORE_2040 */
  ASSERT_THAT(s.find(sv), Eq(5));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2041
@Test_Description:    To test the functionality of R find StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611735, 611736, 611737, 611738, 611739
*******************************************************************************/
TEST(StringTest, RFindStringView)
{
  ara::core::StringView const sv = "string";
  std::basic_string<char> s1("XYZ");
  ara::core::String const s = "some string value";

  ASSERT_THAT(s.rfind(sv), Eq(5));
  ASSERT_THAT(s.rfind(sv, 6), Eq(5));
  ASSERT_THAT(s.rfind("some string value", 1, 1), Eq(0));
  ASSERT_THAT(s.rfind("some string valu", 1), Eq(0));
  /* @Start: UT_CORE_2041 */
  /* @TS1: Invoke rfind(sv) with s. */
  /* @TE1: rfind() should be equal to 5. */
  /* @TS2: Invoke rfind(sv, 6) with s. */
  /* @TE2: rfind() should be equal to 5. */
  /* @TS3: Invoke rfind("some string value", 1, 1) with s. */
  /* @TE3: rfind() should be equal to 0. */
  /* @TS4: Invoke rfind("some string valu", 1) with s. */
  /* @TE4: rfind() should be equal to 0. */
  /* @End: UT_CORE_2041 */
  std::cout << "-->" << s.rfind(s1, 3) << std::endl;
  EXPECT_NO_THROW((s.rfind(s1, 2), Eq(0)));
  /* @Start: UT_CORE_2041 */
  /* @TS6: Invoke rfind(s1, 2) with s. */
  /* @TE6: rfind() should be equal to 0 and not throw the exception. */
  /* @End: UT_CORE_2041 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2042
@Test_Description:    To test the functionality of find FirstOfStringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611740, 611741, 611742, 611743, 611744
*******************************************************************************/
TEST(StringTest, FindFirstOfStringView)
{
  ara::core::StringView const sv = "tv";

  ara::core::String const s = "some string value";
  ara::core::internal::basic_string<char> s1 = "some string value";
  ASSERT_THAT(s.find_first_of(sv), Eq(6));
  ASSERT_THAT(s.find_first_of(sv, 6), Eq(6));
  ASSERT_THAT(s.find_first_of(sv, 7), Eq(12));
  ASSERT_THAT(s.find_first_of(sv, 13), Eq(ara::core::String::npos));
  ASSERT_THAT(s.find_first_of("some string valu", 1, 5), Eq(1));
  ASSERT_THAT(s.find_first_of("some string valu", 1), Eq(1));
  ASSERT_THAT(s.find_first_of("some string valu", 1), Eq(1));
  /* @Start: UT_CORE_2042 */
  /* @TS1: Invoke find_first_of(sv) with s. */
  /* @TE1: find_first_of() should be equal to 6. */
  /* @TS2: Invoke find_first_of(sv,6) with s. */
  /* @TE2: find_first_of() should be equal to 6. */
  /* @TS3: Invoke find_first_of(sv,7) with s. */
  /* @TE3: find_first_of() should be equal to 12. */
  /* @TS4: Invoke find_first_of(sv,13) with s. */
  /* @TE4: find_first_of() should be equal to nops. */
  /* @TS5: Invoke find_first_of("some string valu", 1, 5) with s. */
  /* @TE5: find_first_of() should be equal to 1. */
  /* @TS6: Invoke find_first_of("some string valu", 1, 5) with s. */
  /* @TE6: find_first_of() should be equal to 1. */
  /* @TS7: Invoke find_first_of("some string valu", 1, 5) with s. */
  /* @TE7: find_first_of() should be equal to 1. */
  /* @End: UT_CORE_2042 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2043
@Test_Description:    To test the functionality of find LastOfStringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611745, 611746, 611747, 611748, 611749
*******************************************************************************/
TEST(StringTest, FindLastOfStringView)
{
  ara::core::StringView const sv = "tv";

  ara::core::String const s = "some string value";
  ara::core::internal::basic_string<char> s1("some string value");
  ASSERT_THAT(s.find_last_of(sv), Eq(12));
  ASSERT_THAT(s.find_last_of(sv, 11), Eq(6));
  ASSERT_THAT(s.find_last_of(sv, 5), Eq(ara::core::String::npos));
  ASSERT_THAT(s.find_last_of("some string value", 1, 2), Eq(1));
  ASSERT_THAT(s.find_last_of("some string value", 1), Eq(1));
  ASSERT_THAT(s.find_last_of(s1, 1), Eq(1));
  /* @Start: UT_CORE_2043 */
  /* @TS1: Invoke find_last_of(sv) with s. */
  /* @TE1: find_last_of() should be equal to 12. */
  /* @TS2: Invoke find_last_of(sv,11) with s. */
  /* @TE2: find_last_of should be equal to 6. */
  /* @TS3: Invoke find_last_of(sv,5) with s. */
  /* @TE3: find_last_of should be equal to nops. */
  /* @TS4: Invoke find_last_of("some string valu", 1, 2) with s. */
  /* @TE4: find_last_of should be equal to 1. */
  /* @TS5: Invoke find_last_of("some string valu", 1) with s. */
  /* @TE5: find_last_of should be equal to 1. */
  /* @TS6: Invoke find_last_of(s1,1) with s. */
  /* @TE6: find_last_of should be equal to 1. */
  /* @End: UT_CORE_2043 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2044
@Test_Description:    To test the functionality of find FirstNotOfStringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611750, 611751, 611752, 611753, 611754
*******************************************************************************/
TEST(StringTest, FindFirstNotOfStringView)
{
  ara::core::StringView const sv = "some ring alu";

  ara::core::String const s = "some string value";
  ara::core::String const s1 = "string value";
  ara::core::internal::basic_string<char> s2 = "string value";
  ASSERT_THAT(s.find_first_not_of(sv), Eq(6));
  ASSERT_THAT(s.find_first_not_of("some string value", 0, 1), Eq(1));
  ASSERT_THAT(s.find_first_not_of("some", 5), Eq(6));
  ASSERT_THAT(s.find_first_not_of('s', 0), Eq(1));
  ASSERT_THAT(s.find_first_not_of('s', 0), Eq(1));
  ASSERT_THAT(s.find_first_not_of(sv, 6), Eq(6));
  ASSERT_THAT(s.find_first_not_of(s2, 0), Eq(1));
  /* @Start: UT_CORE_2044 */
  /* @TS1: Invoke find_first_not_of(sv) with s. */
  /* @TE1: find_first_not_of() should be equal to 6. */
  /* @TS2: Invoke find_first_not_of("some string value", 0, 1) with s. */
  /* @TE2: find_first_not_of() should be equal to 1. */
  /* @TS3: Invoke find_first_not_of("some", 5) with s. */
  /* @TE3: find_first_not_of() should be equal to 6. */
  /* @TS4: Invoke find_first_not_of('s', 0) with s. */
  /* @TE4: find_first_not_of() should be equal to 1. */
  /* @TS5: Invoke find_first_not_of('s', 0) with s. */
  /* @TE5: find_first_not_of() should be equal to 1. */
  /* @TS6: Invoke find_first_not_of(sv, 6) with s. */
  /* @TE6: find_first_not_of() should be equal to 6. */
  /* @TS7: Invoke find_first_not_of(s2, 0) with s. */
  /* @TE7: find_first_not_of() should be equal to 1. */
  /* @End: UT_CORE_2044 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2045
@Test_Description:    To test the functionality of find LastNotOfStringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611755, 611756, 611757, 611758, 611759
*******************************************************************************/
TEST(StringTest, FindLastNotOfStringView)
{
  ara::core::StringView const sv = "some ring alu";

  ara::core::String const s = "some string value";
  ara::core::internal::basic_string<char> s1("some string value");
  ASSERT_THAT(s.find_last_not_of(sv), Eq(12));
  ASSERT_THAT(s.find_last_not_of("some string value", 0, 0), Eq(0));
  ASSERT_THAT(s.find_last_not_of("string", 5), Eq(4));
  /* @Start: UT_CORE_2045 */
  /* @TS1: Invoke find_last_not_of(sv) with s. */
  /* @TE1: find_last_not_of() should be equal to 12. */
  /* @TS2: Invoke find_last_not_of("some string value", 0, 0) with s. */
  /* @TE2: find_last_not_of() should be equal to 0. */
  /* @TS3: Invoke find_last_not_of("string", 5) with s. */
  /* @TE3: find_last_not_of() should be equal to 4. */
  /* @End: UT_CORE_2045 */

  EXPECT_NO_THROW(s.find_last_not_of(s1, 6));
  /* @Start: UT_CORE_2045 */
  /* @TS4: Invoke find_last_not_of(s1, 6) with s. */
  /* @TE4: find_last_not_of() should successfully perform the operation. */
  /* @End: UT_CORE_2045 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2046
@Test_Description:    To test the functionality of global comparison operators work.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611647
*******************************************************************************/
TEST(StringTest, GlobalComparisonOperatorsWork)
{
  ara::core::String s1;
  ara::core::String s2;
  ASSERT_THAT(s1, Eq(s2));
  /* @Start: UT_CORE_2046 */
  /* @TS1: Create an ara::core::String data type with s1. */
  /* @TE1: Result should be equal to ara::core::String s2. */
  /* @End: UT_CORE_2046 */
  ara::core::String s3 = "hello";
  ASSERT_THAT(s1, Ne(s3));
  /* @Start: UT_CORE_2046 */
  /* @TS2: Create an ara::core::String data type with s1. */
  /* @TE2: Result should not be equal to s3. */
  /* @End: UT_CORE_2046 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2047
@Test_Description:    To test the functionality of ToStringWorks.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611646, 611647, 611648, 611649, 611650
*******************************************************************************/
TEST(StringTest, ToStringWorks)
{
  ara::core::String s = std::to_string(42);
  ASSERT_THAT(s, Eq("42"));
  /* @Start: UT_CORE_2047 */
  /* @TS1: Invoke the to_string(42) . */
  /* @TE1: Result should be equal to "42". */
  /* @End: UT_CORE_2047 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2048
@Test_Description:    To test the functionality of ToStreamWorks.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611646, 611647, 611648, 611649, 611650
*******************************************************************************/
TEST(StringTest, ToStreamWorks)
{
  ara::core::String s = "The Quick Brown Fox";

  std::ostringstream out;
  out << s;
  std::string result = out.str();

  ASSERT_THAT(result, Eq("The Quick Brown Fox"));
  /* @Start: UT_CORE_2048 */
  /* @TS1: Create an ara::core::String data type with result. */
  /* @TE1: Result should be equal to "The Quick Brown Fox". */
  /* @End: UT_CORE_2048 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2049
@Test_Description:    To test the functionality of swap overload works.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611792, 611861
*******************************************************************************/
TEST(StringTest, SwapOverloadWorks)
{
  ara::core::String s1 = "The Quick Brown Fox";
  ara::core::String s2 = "Jumps Over the Lazy Dog";

  using std::swap;
  swap(s1, s2);

  ASSERT_THAT(s1, Eq("Jumps Over the Lazy Dog"));
  ASSERT_THAT(s2, Eq("The Quick Brown Fox"));
  /* @Start: UT_CORE_2049 */
  /* @TS1: Create an ara::core::String data type and variable is s1. */
  /* @TE1: Result should be equal to "Jumps Over the Lazy Dog". */
  /* @TS2: Create an ara::core::String data type and variable is s2. */
  /* @TE2: Result should be equal to "The Quick Brown Fox". */
  /* @End: UT_CORE_2049 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_2054
@Test_Description:    To test the functionality of constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611650, 611651, 611652, 611653
*******************************************************************************/
TEST(StringTest, constructor5)
{
  std::initializer_list<char> ilist{'A', 'B'};
  //ara::core::String s;
  ara::core::String s1(ilist);
  EXPECT_TRUE(s1 == "AB");
  /* @Start: UT_CORE_2054 */
  /* @TS1: Create an ara::core::String data type and variable is s1. */
  /* @TE1: Result should be equal to "AB" and return true. */
  /* @End: UT_CORE_2054 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2055
@Test_Description:    To test the functionality of operator assignment copy.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611658
*******************************************************************************/
TEST(StringTest, operatorAssignmentCopy)
{
  ara::core::String s1("ABC");
  ara::core::String s2;
  s2 = s1;
  EXPECT_TRUE(s2 == "ABC");
  /* @Start: UT_CORE_2055 */
  /* @TS1: Create an ara::core::String data type with s2. */
  /* @TE1: Result should be equal to "ABC" and return true. */
  /* @End: UT_CORE_2055 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2056
@Test_Description:    To test the functionality of operator assignment move.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611729, 611730, 611731, 611732
*******************************************************************************/
TEST(StringTest, operatorAssignmentMove)
{
  ara::core::String s1("ABC");
  ara::core::String s2;
  s2 = std::move(s1);
  EXPECT_TRUE(s2 == "ABC");
  /* @Start: UT_CORE_2056 */
  /* @TS1: Create an ara::core::String data type with s2 variable. */
  /* @TE1: Result should be equal to "ABC" and return true. */
  /* @End: UT_CORE_2056 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2057
@Test_Description:    To test the functionality of operator constPointer.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611729
*******************************************************************************/
TEST(StringTest, operatorconstPointer)
{
  ara::core::String s1("ABC");
  s1 = "ABC";
  EXPECT_TRUE(s1 == "ABC");
  /* @Start: UT_CORE_2057 */
  /* @TS1: Create an ara::core::String data type with s1 variable. */
  /* @TE1: Result should be equal to "ABC" and return true. */
  /* @End: UT_CORE_2057 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2058
@Test_Description:    To test the functionality of operator char.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611733, 611734
*******************************************************************************/
TEST(StringTest, operatorChar)
{
  ara::core::String s1;
  s1 = 'A';
  EXPECT_TRUE(s1 == "A");
  /* @Start: UT_CORE_2058 */
  /* @TS1: Create an ara::core::String data type with s1 variable. */
  /* @TE1: Result should be equal to "A" and return true. */
  /* @End: UT_CORE_2058 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2059
@Test_Description:    To test the functionality of operator initializer list.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611731, 611732
*******************************************************************************/
TEST(StringTest, OperatorInitializerList)
{
  std::initializer_list<char> ilist{'A', 'B'};
  ara::core::String s1;
  s1 = ilist;
  EXPECT_TRUE(s1 == "AB");
  /* @Start: UT_CORE_2059 */
  /* @TS1: Create an ara::core::String data type with s1 variable. */
  /* @TE1: Result should be equal to "AB" and return true. */
  /* @End: UT_CORE_2059 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2060
@Test_Description:    To test the functionality of operator plus and equal to 1.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611669, 611670, 611671, 611672
*******************************************************************************/
TEST(StringTest, OperatorPlusAndEquelTo1)
{
  ara::core::String s1 = "ABC";
  ara::core::String s2;
  s2 += s1;
  EXPECT_TRUE(s2 == "ABC");
  /* @Start: UT_CORE_2060 */
  /* @TS1: Create an ara::core::String data type with s2 variable. */
  /* @TE1: Result should be equal to "ABC" and return true. */
  /* @End: UT_CORE_2060 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2061
@Test_Description:    To test the functionality of operator plus and equal to 1.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611669, 611670, 611671, 611672
*******************************************************************************/
TEST(StringTest, OperatorPlusAndEquelTo2)
{
  ara::core::String s1 = "ABC";
  s1 += "D";
  EXPECT_TRUE(s1 == "ABCD");
  /* @Start: UT_CORE_2061 */
  /* @TS1: Create an ara::core::String data type with s1 variable. */
  /* @TE1: Result should be equal to "ABCD" and return true. */
  /* @End: UT_CORE_2061 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2062
@Test_Description:    To test the functionality of operator plus and equal to 3.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611669, 611670, 611671, 611672
*******************************************************************************/
TEST(StringTest, OperatorPlusAndEquelTo3)
{
  ara::core::String s1 = "ABC";
  std::initializer_list<char> ilist{'D'};
  s1 += ilist;
  EXPECT_TRUE(s1 == "ABCD");
  /* @Start: UT_CORE_2062 */
  /* @TS1: Create an ara::core::String data type with s1 variable. */
  /* @TE1: Result should be equal to "ABCD" and return true. */
  /* @End: UT_CORE_2062 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_2066
@Test_Description:    To test the functionality of append.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611683, 611684, 611685, 611686
*******************************************************************************/
TEST(StringTest, append4)
{
  ara::core::String s1 = "ABC";
  std::initializer_list<char> ilist{'D'};
  s1.append(ilist);
  EXPECT_TRUE(s1 == "ABCD");
  /* @Start: UT_CORE_2066 */
  /* @TS1: Create an ara::core::String data type with s1 variable. */
  /* @TE1: Result should be equal to "ABCD" and return true. */
  /* @End: UT_CORE_2066 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2067
@Test_Description:    To test the functionality of append.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611683, 611684, 611685, 611686
*******************************************************************************/
TEST(StringTest, append7)
{
  ara::core::String s1;
  s1.append("ABCD", 1, 2);
  EXPECT_TRUE(s1 == "BC");
  /* @Start: UT_CORE_2067 */
  /* @TS1: Create an ara::core::String data type with s1 variable. */
  /* @TE1: Result should be equal to "BC" and return true. */
  /* @End: UT_CORE_2067 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2068
@Test_Description:    To test the functionality of append.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611683, 611684, 611685, 611686
*******************************************************************************/
TEST(StringTest, append8)
{
  ara::core::String s1;
  s1.append("ABCD", 1);
  /* @Start: UT_CORE_2068 */
  /* @TS1: Create an ara::core::String data type with s1 variable. */
  /* @TE1: Result should be equal to "A" and return true. */
  /* @End: UT_CORE_2068 */
  EXPECT_TRUE(s1 == "A");

  ara::core::String s2;
  EXPECT_NO_THROW(s2.append("ABCD", "TST"));
  /* @Start: UT_CORE_2068 */
  /* @TS1: Invoke append("ABCD", "TEST") with s2. 
  /* @TE1: append() should successfully append the given string. */
  /* @End: UT_CORE_2068 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_2072
@Test_Description:    To test the functionality of assign.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611687, 611688, 611689, 611690
*******************************************************************************/
TEST(StringTest, assign4)
{
  std::basic_string<char> s1("ABC");
  ara::core::internal::basic_string<char> s2;
  EXPECT_NO_THROW(s2.assign(s1, 2, 2));
  EXPECT_NO_THROW(s2.assign(std::move(s1)));
  /* @Start: UT_CORE_2072 */
  /* @TS1: Invoke assign(s1, 2, 2) with s2. */
  /* @TE1: Result should not be throw the exception. */
  /* @TS2: Invoke assign(std::move(s1) with s2. */
  /* @TE2: Result should not be throw the exception. */
  /* @End: UT_CORE_2072 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2073
@Test_Description:    To test the functionality of replace.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611715
*******************************************************************************/
TEST(StringTest, replace1)
{
  ara::core::String s1 = "ABCD";
  ara::core::String s2 = "Z";
  s1.replace(1, 1, s2);
  EXPECT_TRUE(s1 == "AZCD");
  /* @Start: UT_CORE_2073 */
  /* @TS1: Invoke the replace(1, 1, s2) with s1. */
  /* @TE1: Result should be equal to "AZCD" and return true. */
  /* @End: UT_CORE_2073 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2074
@Test_Description:    To test the functionality of global comparison operators work.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611889, 611895
*******************************************************************************/
TEST(StringTest, replace2)
{
  ara::core::String s1 = "ABCD";
  ara::core::String s2 = "Z";
  auto it1 = s1.begin();
  auto it2 = s1.begin() + 1;
  s1.replace(it1, it2, s2);
  EXPECT_TRUE(s1 == "ZBCD");
  /* @Start: UT_CORE_2074 */
  /* @TS1: Invoke the replace(it1, it2, s2) with s1. */
  /* @TE1: Result should be equal to "ZBCD" and return true. */
  /* @End: UT_CORE_2074 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_2076
@Test_Description:    To test the functionality of replace.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611715, 611716
*******************************************************************************/
TEST(StringTest, replace4)
{
  ara::core::String s1 = "ABCD";
  ara::core::String s2 = "Z";
  auto it1 = s1.begin();
  auto it2 = s1.begin() + 1;
  s1.replace(it1, it2, "WO", 2);
  EXPECT_TRUE(s1 == "WOBCD");
  /* @Start: UT_CORE_2076 */
  /* @TS1: Invoke the replace(it1, it2, "WO", 2) with s1. */
  /* @TE1: Result should be equal to "WOBCD" and return true. */
  /* @End: UT_CORE_2076 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2077
@Test_Description:    To test the functionality of replace.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611715, 611716
*******************************************************************************/
TEST(StringTest, replace5)
{
  ara::core::String s1 = "ABCD";
  ara::core::String s2 = "Z";
  auto it1 = s1.begin();
  auto it2 = s1.begin() + 1;
  s1.replace(it1, it2, "WO");
  EXPECT_TRUE(s1 == "WOBCD");
  /* @Start: UT_CORE_2077 */
  /* @TS1: Invoke the replace(it1, it2, "WO") with s1. */
  /* @TE1: Result should be equal to "WOBCD" and return true. */
  /* @End: UT_CORE_2077 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2078
@Test_Description:    To test the functionality of replace.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611715, 611716
*******************************************************************************/
TEST(StringTest, replace6)
{
  ara::core::String s1 = "ABCD";
  s1.replace(1, 2, 1, 'Z');
  EXPECT_TRUE(s1 == "AZD");
  /* @Start: UT_CORE_2078 */
  /* @TS1: Invoke the replace(1, 2, 1, 'Z') with s1. */
  /* @TE1: Result should be equal to "AZD" and return true. */
  /* @End: UT_CORE_2078 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2079
@Test_Description:    To test the functionality of replace.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611715, 611716
*******************************************************************************/
TEST(StringTest, replace7)
{
  ara::core::String s1 = "ABCD";
  ara::core::String s2 = "Z";
  auto it1 = s1.begin();
  auto it2 = s1.begin() + 1;
  s1.replace(it1, it2, 1, 'Z');
  EXPECT_TRUE(s1 == "ZBCD");
  /* @Start: UT_CORE_2079 */
  /* @TS1: Invoke the replace(it1, it2, 1, 'Z') with s1. */
  /* @TE1: Result should be equal to "ZBCD" and return true. */
  /* @End: UT_CORE_2079 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2080
@Test_Description:    To test the functionality of replace.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611715, 611716
*******************************************************************************/
TEST(StringTest, replace8)
{
  ara::core::String s1 = "ABCD";
  ara::core::String s2 = "Z";
  auto it1 = s1.begin();
  auto it2 = s1.begin() + 1;
  std::initializer_list<char> ilist{'W'};
  s1.replace(it1, it2, ilist);
  /* @Start: UT_CORE_2080 */
  /* @TS1: Invoke the replace(it1, it2, ilist) with s1. */
  /* @TE1: Result should be equal to "WBCD" and return true. */
  /* @TS2: Invoke the replace(it1, it2, "AB", "CD") with s1. */
  /* @TE2: Result should not be trow expection. */
  /* @End: UT_CORE_2080 */
  EXPECT_TRUE(s1 == "WBCD");
  EXPECT_NO_THROW(s1.replace(it1, it2, "AB", "CD"));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_2081
@Test_Description:    To test the functionality of insert.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611701, 611702, 611703, 611704, 611705, 611706
*******************************************************************************/
TEST(StringTest, insert1)
{
  ara::core::String s1 = "Text";
  s1.insert(1, "test", 2);
  /* @Start: UT_CORE_2081 */
  /* @TS1: Invoke insert() of string class. */
  /* @TE1: insert() should successfully insert the given string.Result should be equal to "TTestext". */
  /* @End: UT_CORE_2081 */
  EXPECT_TRUE(s1 == "Tteext");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2082
@Test_Description:    To test the functionality of insert.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611707, 611708, 611709, 611710
*******************************************************************************/
TEST(StringTest, insert2)
{
  ara::core::String s1 = "Text";
  ara::core::String s2 = "Test";
  s1.insert(1, s2);
  /* @Start: UT_CORE_2082 */
  /* @TS1: Invoke insert() of string class. */
  /* @TE1: insert() should successfully insert the given string.Result should be equal to "TTestext". */
  /* @End: UT_CORE_2082 */
  EXPECT_TRUE(s1 == "TTestext");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2083
@Test_Description:    To test the functionality of insert.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611707, 611708, 611709, 611710
*******************************************************************************/
TEST(StringTest, insert3)
{
  ara::core::String s = "test stuff!";
  ara::core::String::ConstIterator i1 = s.begin() + 5;
  s.insert(i1, 'T');
  /* @Start: UT_CORE_2083 */
  /* @TS1: Invoke insert() of string class. */
  /* @TE1: Result should successfully insert the char 'T' */
  /* @End: UT_CORE_2083 */
  EXPECT_NO_THROW(s.insert(i1, 'T'));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2084
@Test_Description:    To test the functionality of insert.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611707, 611708, 611709, 611710
*******************************************************************************/
TEST(StringTest, insert4)
{
  ara::core::String s = "test stuff!";
  ara::core::String::ConstIterator i1 = s.begin() + 5;
  s.insert(i1, 2, 'T');
  /* @Start: UT_CORE_2084 */
  /* @TS1: Invoke insert() of string class. */
  /* @TE1: Result should successfully insert the char 'T'.  */
  /* @End: UT_CORE_2084 */
  EXPECT_NO_THROW(s.insert(i1, 2, 'T'));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2085
@Test_Description:    To test the functionality of insert.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611707, 611708, 611709, 611710
*******************************************************************************/
TEST(StringTest, insert5)
{
  ara::core::String s = "test stuff!";
  EXPECT_NO_THROW(s.insert(1, "UNIT"));
  //s.insert(i1, 'A', 'T');
  /* @Start: UT_CORE_2085 */
  /* @TS1: Invoke insert() of string class. */
  /* @TE1: Result should successfully insert the string "UNIT"  */
  /* @End: UT_CORE_2085 */
  //EXPECT_NO_THROW(s.insert(i1, 'A', 'T'));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2086
@Test_Description:    To test the functionality of insert.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611707, 611708, 611709, 611710
*******************************************************************************/
TEST(StringTest, insert6)
{
  ara::core::String s = "test stuff!";
  ara::core::String::ConstIterator i1 = s.begin() + 5;
  std::initializer_list<char> ilist{'A', 'B'};
  s.insert(i1, ilist);
  s.insert(0, 2, 'A');
  /* @Start: UT_CORE_2086 */
  /* @TS1: Invoke insert() of string class. */
  /* @TE1: Result should successfully insert the string char 'A'  */
  /* @End: UT_CORE_2086 */
  EXPECT_NO_THROW(s.insert(0, 2, 'A'));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2087
@Test_Description:    To test the functionality of erase.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611712, 611713, 611714
*******************************************************************************/
TEST(StringTest, erase1)
{
  ara::core::String s = "test stuff!";
  s.erase(0, 5);
  /* @Start: UT_CORE_2087 */
  /* @TS1: Invoke erase() of string class. */
  /* @TE1: erase() should successfully erase the given string.Result should be equal to "stuff!". */
  /* @End: UT_CORE_2087 */
  EXPECT_TRUE(s == "stuff!");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2088
@Test_Description:    To test the functionality of erase.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611712, 611713, 611714
*******************************************************************************/
TEST(StringTest, erase2)
{
  ara::core::String s = "test stuff!";
  ara::core::String::ConstIterator i1 = s.begin() + 5;
  s.erase(i1);
  /* @Start: UT_CORE_2088 */
  /* @TS1: Invoke erase() of string class. */
  /* @TE1: erase() should successfully erase the given string. Result should be equal to "test tuff!". */
  /* @End: UT_CORE_2088 */
  EXPECT_TRUE(s == "test tuff!");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2089
@Test_Description:    To test the functionality of erase.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611712, 611713, 611714
*******************************************************************************/
TEST(StringTest, erase3)
{
  ara::core::String s = "test stuff!";
  ara::core::String::ConstIterator i1 = s.begin();
  ara::core::String::ConstIterator i2 = s.begin() + 5;
  s.erase(i1, i2);
  /* @Start: UT_CORE_2089 */
  /* @TS1: Invoke erase() of string class. */
  /* @TE1: erase() should successfully erase the given string.Result should be equal to "stuff!". */
  /* @End: UT_CORE_2089 */
  EXPECT_TRUE(s == "stuff!");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2090
@Test_Description:    To test the functionality of erase.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611712, 611713, 611714
*******************************************************************************/
TEST(StringTest, erase4)
{
  ara::core::String s = "test stuff!";
  ara::core::String::ConstIterator i1 = s.begin();
  ara::core::String::ConstIterator i2 = s.begin() + 5;
  s.erase(i1, i2);
  /* @Start: UT_CORE_2090 */
  /* @TS1: Invoke erase() of string class. */
  /* @TE1: erase() should successfully erase the given string. Result should be equal to "stuff!". */
  /* @End: UT_CORE_2090 */
  EXPECT_TRUE(s == "stuff!");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2091
@Test_Description:    To test the functionality of length.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611775
*******************************************************************************/
TEST(StringTest, length)
{
  ara::core::String s = "test stuff!";
  /* @Start: UT_CORE_2091 */
  /* @TS1: Invoke length() of string class. */
  /* @TE1: Result should be equal to 11. */
  /* @End: UT_CORE_2091 */
  EXPECT_EQ(s.length(), 11);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2092
@Test_Description:    To test the functionality of max_size.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611776
*******************************************************************************/
TEST(StringTest, max_size)
{
  ara::core::String s = "test stuff!";
  /* @Start: UT_CORE_2092 */
  /* @TS1: Invoke max_size() of string class. */
  /* @TE1: Result should be equal to 4611686018427387903. */
  /* @End: UT_CORE_2092 */
  EXPECT_EQ(s.max_size(), 4611686018427387903);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2093
@Test_Description:    To test the functionality of capacity.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611777
*******************************************************************************/
TEST(StringTest, capacity)
{
  ara::core::String s = "test stuff!";
  /* @Start: UT_CORE_2093 */
  /* @TS1: Invoke capacity() of string class. */
  /* @TE1: Result should be equal to 15. */
  /* @End: UT_CORE_2093 */
  EXPECT_EQ(s.capacity(), 15);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2094
@Test_Description:    To test the functionality of reserve.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611778
*******************************************************************************/
TEST(StringTest, reserve)
{
  ara::core::String s = "test stuff!";
  s.reserve();
  /* @Start: UT_CORE_2094 */
  /* @TS1: Invoke reserve() of string class. */
  /* @TE1: Result should successfully reserve the string. */
  /* @End: UT_CORE_2094 */
  EXPECT_NO_THROW(s.reserve());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2095
@Test_Description:    To test the functionality of resize.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611779, 611780
*******************************************************************************/
TEST(StringTest, resize)
{
  ara::core::String s = "test stuff!";
  s.resize(5);
  /* @Start: UT_CORE_2095 */
  /* @TS1: Invoke resize() of string class. */
  /* @TE1: Size of result should be equal to 5. */
  /* @End: UT_CORE_2095 */
  EXPECT_EQ(s.size(), 5);
  s.resize(5, 'A');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2096
@Test_Description:    To test the functionality of shrink_to_fit.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611781
*******************************************************************************/
TEST(StringTest, shrink_to_fit)
{
  ara::core::String s = "test stuff!";
  /* @Start: UT_CORE_2096 */
  /* @TS1: Invoke shrink_to_fit() of string class. */
  /* @TE1: Result should adjust its size to fit the given string. */
  /* @End: UT_CORE_2096 */
  EXPECT_NO_THROW(s.shrink_to_fit());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2097
@Test_Description:    To test the functionality of clear.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611782
*******************************************************************************/
TEST(StringTest, clear)
{
  ara::core::String s = "test stuff!";
  s.clear();
  /* @Start: UT_CORE_2097 */
  /* @TS1: Invoke clear() of string class. */
  /* @TE1: Size of the result should be equal to 0. */
  /* @End: UT_CORE_2097 */
  EXPECT_EQ(s.size(), 0);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2098
@Test_Description:    To test the functionality of data.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611784
*******************************************************************************/
TEST(StringTest, data)
{
  ara::core::String s = "test stuff!";
  /* @Start: UT_CORE_2098 */
  /* @TS1: Invoke data() of string class. */
  /* @TE1: data() should return a given string. */
  /* @End: UT_CORE_2098 */
  s.data();
  EXPECT_NO_THROW(s.data());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_2099
@Test_Description:    To test the functionality of front.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611785
*******************************************************************************/
TEST(StringTest, front)
{
  ara::core::String s = "test stuff!";
  /* @Start: UT_CORE_2099 */
  /* @TS1: Invoke front() of string class. */
  /* @TE1: Result should be equal to 't'. */
  /* @End: UT_CORE_2099 */
  EXPECT_EQ(s.front(), 't');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20100
@Test_Description:    To test the functionality of front_const.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611785
*******************************************************************************/
TEST(StringTest, front_const)
{
  const ara::core::String s = "test stuff!";
  /* @Start: UT_CORE_20100 */
  /* @TS1: Invoke front() of string class. */
  /* @TE1: Result should be equal to 't'. */
  /* @End: UT_CORE_20100 */
  EXPECT_EQ(s.front(), 't');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20101
@Test_Description:    To test the functionality of back.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611788
*******************************************************************************/
TEST(StringTest, back)
{
  ara::core::String s = "test stuff!";
  /* @Start: UT_CORE_20101 */
  /* @TS1: Invoke back() of string class. */
  /* @TE1: Result should be equal to '!'. */
  /* @End: UT_CORE_20101 */
  EXPECT_EQ(s.back(), '!');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20102
@Test_Description:    To test the functionality of substr.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611788
*******************************************************************************/
TEST(StringTest, back_const)
{
  const ara::core::String s = "test stuff!";
  /* @Start: UT_CORE_20102 */
  /* @TS1: Invoke back() of string class. */
  /* @TE1: Result should be equal to '!'. */
  /* @End: UT_CORE_20102 */
  EXPECT_EQ(s.back(), '!');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20103
@Test_Description:    To test the functionality of substr.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611789
*******************************************************************************/
TEST(StringTest, push_back)
{
  ara::core::String s = "test";
  s.push_back('!');
  /* @Start: UT_CORE_20103 */
  /* @TS1: Invoke push_back() of string class. */
  /* @TE1: Result should be equal to "test!". */
  /* @End: UT_CORE_20103 */
  EXPECT_EQ(s, "test!");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20104
@Test_Description:    To test the functionality of pop_back.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611790
*******************************************************************************/
TEST(StringTest, pop_back)
{
  ara::core::String s = "test!";
  s.pop_back();
  /* @Start: UT_CORE_20104 */
  /* @TS1: Invoke pop_back() of string class. */
  /* @TE1: Result should be equal to "test". */
  /* @End: UT_CORE_20104 */
  EXPECT_EQ(s, "test");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20105
@Test_Description:    To test the functionality of substr.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611791
*******************************************************************************/
TEST(StringTest, substr)
{
  ara::core::String s = "test!";
  s.substr(0, 2);
  /* @Start: UT_CORE_20105 */
  /* @TS1: Invoke substr() of string class. */
  /* @TE1: Result should be equal to "test!". */
  /* @End: UT_CORE_20105 */
  EXPECT_EQ(s, "test!");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20106
@Test_Description:    To test the functionality of swap.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611792
*******************************************************************************/
TEST(StringTest, swap)
{
  ara::core::String s1 = "test!";
  ara::core::String s2 = "New";
  ara::core::internal::basic_string<char> s3("ABC");
  swap(s1, s2);
  /* @Start: UT_CORE_20106 */
  /* @TS1: Invoke swap() of string class. */
  /* @TE1.1: Value of first object should be equal to "New". */
  /* @TE1.2: Value of second object should be should be equal to "test!". */
  /* @End: UT_CORE_20106 */
  EXPECT_EQ(s1, "New");
  EXPECT_EQ(s2, "test!");
  s1.swap(s3);
  /* @Start: UT_CORE_20106 */
  /* @TS1: Invoke swap() of string class. */
  /* @TE1.1: Value of first object should be equal to "ABC". */
  /* @TE1.2: Value of second object should be should be equal to "New". */
  /* @End: UT_CORE_20106 */
  EXPECT_EQ(s1, "ABC");
  EXPECT_EQ(s3, "New");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20107
@Test_Description:    To test the functionality of begin.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611793, 611794
*******************************************************************************/
TEST(StringTest, begin)
{
  ara::core::String s1 = "test";
  auto it = s1.begin();
  /* @Start: UT_CORE_20107 */
  /* @TS1: Invoke begin() of string class. */
  /* @TE1: Result should be equal to 't'. */
  /* @End: UT_CORE_20107 */
  EXPECT_EQ(*it, 't');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20108
@Test_Description:    To test the functionality of  .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611795
*******************************************************************************/
TEST(StringTest, cbegin)
{
  ara::core::String s1 = "test";
  auto it = s1.cbegin();
  /* @Start: UT_CORE_20108 */
  /* @TS1: Invoke cbegin() of string class. */
  /* @TE1: Result should be equal to 't'. */
  /* @End: UT_CORE_20108 */
  EXPECT_EQ(*it, 't');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20109
@Test_Description:    To test the functionality of end.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611796
*******************************************************************************/
TEST(StringTest, end)
{
  ara::core::String s1 = "test";
  auto it = s1.end();
  /* @Start: UT_CORE_20109 */
  /* @TS1: Invoke end() of string class. */
  /* @TE1: Result should be equal to '\0'. */
  /* @End: UT_CORE_20109 */
  EXPECT_EQ(*it, '\0');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20110
@Test_Description:    To test the functionality of cend.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611798
*******************************************************************************/
TEST(StringTest, cend)
{
  ara::core::String s1 = "test";
  auto it = s1.cend();
  /* @Start: UT_CORE_20110 */
  /* @TS1: Invoke cend() of string class. */
  /* @TE1: Result should be equal to '\0'. */
  /* @End: UT_CORE_20110 */
  EXPECT_EQ(*it, '\0');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20111
@Test_Description:    To test the functionality of rbegin.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611799
*******************************************************************************/
TEST(StringTest, rbegin)
{
  ara::core::String s1 = "test";
  auto it = s1.rbegin();
  /* @Start: UT_CORE_20111 */
  /* @TS1: Invoke rbegin() of string class. */
  /* @TE1: Result should be equal to 't'. */
  /* @End: UT_CORE_20111 */
  EXPECT_EQ(*it, 't');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20112
@Test_Description:    To test the functionality of rbegin_const.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611799
*******************************************************************************/
TEST(StringTest, rbegin_const)
{
  const ara::core::String s1 = "test";
  auto it = s1.rbegin();
  /* @Start: UT_CORE_20112 */
  /* @TS1: Invoke rbegin() of string class. */
  /* @TE1: Result should be equal to 't'. */
  /* @End: UT_CORE_20112 */
  EXPECT_EQ(*it, 't');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20113
@Test_Description:    To test the functionality of crbegin.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611801
*******************************************************************************/
TEST(StringTest, crbegin)
{
  ara::core::String s1 = "test";
  auto it = s1.crbegin();
  /* @Start: UT_CORE_20113 */
  /* @TS1: Invoke crbegin() of string class. */
  /* @TE1: Result should be equal to 't'. */
  /* @End: UT_CORE_20113 */
  EXPECT_EQ(*it, 't');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20114
@Test_Description:    To test the functionality of crbegin_const.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611801
*******************************************************************************/
TEST(StringTest, crbegin_const)
{
  const ara::core::String s1 = "test";
  auto it = s1.crbegin();
  /* @Start: UT_CORE_20114 */
  /* @TS1: Invoke crbegin() of string class. */
  /* @TE1: Result should be equal to 't'. */
  /* @End: UT_CORE_20114 */
  EXPECT_EQ(*it, 't');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20115
@Test_Description:    To test the functionality of rend_const.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611804
*******************************************************************************/
TEST(StringTest, rend)
{
  ara::core::String s1 = "test";
  auto it = s1.rend();
  /* @Start: UT_CORE_20115 */
  /* @TS1: Invoke rend() of string class. */
  /* @TE1: Result should be equal to "\0". */
  /* @End: UT_CORE_20115 */
  EXPECT_EQ(*it, '\0');
  auto it2 = s1.crend();
  /* @Start: UT_CORE_20115 */
  /* @TS1: Invoke crend() of string class. */
  /* @TE1: Result should be equal to "\0". */
  /* @End: UT_CORE_20115 */
  EXPECT_EQ(*it2, '\0');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20116
@Test_Description:    To test the functionality of rend_const.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611804
*******************************************************************************/
TEST(StringTest, rend_const)
{
  ara::core::String s1 = "test";
  auto it = s1.rend();
  /* @Start: UT_CORE_20116 */
  /* @TS1: Invoke rend() of string class. */
  /* @TE1: Value of result should be equal to "\0". */
  /* @End: UT_CORE_20116 */
  EXPECT_EQ(*it, '\0');
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20117
@Test_Description:    To test the functionality of get_allocator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611805
*******************************************************************************/
TEST(StringTest, get_allocator)
{
  ara::core::String s1 = "test";
  std::allocator<char> it = s1.get_allocator();
  /* @Start: UT_CORE_20117 */
  /* @TS1: Invoke get_allocator() of string class. */
  /* @TE1: Size of the result should be equal to 4. */
  /* @End: UT_CORE_20117 */
  EXPECT_EQ(s1.size(), 4);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20118
@Test_Description:    To test the functionality of plus Operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611865
*******************************************************************************/
TEST(StringTest, plusOperator1)
{
  ara::core::internal::basic_string<char> s1("Unit");
  ara::core::internal::basic_string<char> s2("Test");
  ara::core::internal::basic_string<char> s3 = s1 + s2;
  /* @Start: UT_CORE_20118 */
  /* @TS1: Invoke the plus Operator*/
  /* @TE1: Result should be equal to "UnitTest". */
  /* @End: UT_CORE_20118 */
  EXPECT_EQ(s3, "UnitTest");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20119
@Test_Description:    To test the functionality of plus Operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611865
*******************************************************************************/
TEST(StringTest, plusOperator2)
{
  ara::core::internal::basic_string<char> s1("Unit");

  ara::core::internal::basic_string<char> s3 = s1 + "Test";
  /* @Start: UT_CORE_20119 */
  /* @TS1: Invoke the plus Operator*/
  /* @TE1: Result should be equal to "UnitTest". */
  /* @End: UT_CORE_20119 */
  EXPECT_EQ(s3, "UnitTest");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20120
@Test_Description:    To test the functionality of plus Operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611669
*******************************************************************************/
TEST(StringTest, plusOperator3)
{
  ara::core::internal::basic_string<char> s1("Unit");

  ara::core::internal::basic_string<char> s3 = "Test" + s1;
  /* @Start: UT_CORE_20120 */
  /* @TS1: Invoke the plus Operator. */
  /* @TE1: Result should be equal to "TestUnit". */
  /* @End: UT_CORE_20120 */

  EXPECT_EQ(s3, "TestUnit");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20121
@Test_Description:    To test the functionality of plus Operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611670
*******************************************************************************/
TEST(StringTest, plusOperator4)
{
  std::basic_string<char> s1("Unit");
  ara::core::internal::basic_string<char> s2("Test");
  ara::core::internal::basic_string<char> s3 = s1 + s2;
  /* @Start: UT_CORE_20121 */
  /* @TS1: Invoke the plus Operator. */
  /* @TE1: Result should be equal to "UnitTest". */
  /* @End: UT_CORE_20121 */
  EXPECT_EQ(s3, "UnitTest");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20122
@Test_Description:    To test the functionality of plus Operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611671, 611672, 611673, 611674, 611675
*******************************************************************************/
TEST(StringTest, plusOperator5)
{
  ara::core::internal::basic_string<char> s1("Test");

  std::basic_string<char> s2("Unit");
  ara::core::internal::basic_string<char> s3 = s1 + s2;
  /* @Start: UT_CORE_20122 */
  /* @TS1: Invoke the plus Operator. */
  /* @TE1: Result should be equal to "TestUnit". */
  /* @End: UT_CORE_20122 */
  EXPECT_EQ(s3, "TestUnit");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20123
@Test_Description:    To test the functionality of equal to operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611889, 611895
*******************************************************************************/
TEST(StringTest, EquelToOperator1)
{
  ara::core::internal::basic_string<char> s1("Test");
  ara::core::internal::basic_string<char> s2("Test");
  /* @Start: UT_CORE_20123 */
  /* @TS1: Invoke the Equal To Operator. */
  /* @TE1: Equal To Operator should return true. */
  /* @End: UT_CORE_20123 */
  EXPECT_TRUE(s1 == s2);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20124
@Test_Description:    To test the functionality of not equal to operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611889, 611895
*******************************************************************************/
TEST(StringTest, EquelToOperator2)
{
  ara::core::internal::basic_string<char> s1("Test");
  ara::core::internal::basic_string<char> s2("Unit");
  /* @Start: UT_CORE_20124 */
  /* @TS1: Invoke the Equal To Operator. */
  /* @TE1.1: Equal To Operator should return true. */
  /* @TE1.2: Result should not be equal to "Unit" and should return true. */
  /* @End: UT_CORE_20124 */
  EXPECT_TRUE(s1 != s2);
  EXPECT_TRUE(s1 != "Unit");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20125
@Test_Description:    To test the functionality of greater than operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611893, 611899, 611905, 611915, 611916
*******************************************************************************/
TEST(StringTest, LessthanOperator)
{
  ara::core::internal::basic_string<char> s1("ABC");
  ara::core::internal::basic_string<char> s2("Unit");
  /* @Start: UT_CORE_20125 */
  /* @TS1: Invoke the Less than Operator. */
  /* @TE1: Less than Operator should  return False. */
  /* @End: UT_CORE_20125 */
  EXPECT_FALSE(s2 < s1);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20126
@Test_Description:    To test the functionality of Greater than Operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611893, 611899, 611905, 611915, 611916
*******************************************************************************/
TEST(StringTest, GreaterthanOperator)
{
  ara::core::internal::basic_string<char> s2("ABC");
  ara::core::internal::basic_string<char> s1("Unit");
  /* @Start: UT_CORE_20126 */
  /* @TS1: Invoke the Greater than Operator. */
  /* @TE1: Greater than Operator should  return False. */
  /* @End: UT_CORE_20126 */
  EXPECT_FALSE(s2 > s1);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20127
@Test_Description:    To test the functionality of less than or equal Operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611892, 611898, 611904, 611913, 611914
*******************************************************************************/
TEST(StringTest, LessthanOrEquelOperator)
{
  ara::core::internal::basic_string<char> s1("ABC");
  ara::core::internal::basic_string<char> s2("ABC");
  /* @Start: UT_CORE_20127 */
  /* @TS1: Invoke the Less than Or Equal to Operator. */
  /* @TE1: Less than Or Equal to Operator should  return true. */
  /* @End: UT_CORE_20127 */
  EXPECT_TRUE(s2 <= s1);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20128
@Test_Description:    To test the functionality of greater than or equal Operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611900, 611906, 611918
*******************************************************************************/
TEST(StringTest, GreaterthanOrEquelOperator)
{
  ara::core::internal::basic_string<char> s2("ABC");
  ara::core::internal::basic_string<char> s1("ABC");
  /* @Start: UT_CORE_20128 */
  /* @TS1: Invoke the Greater than Or Equal to Operator. */
  /* @TE1: Greater than Or Equal to Operator should  return true. */
  /* @End: UT_CORE_20128 */
  EXPECT_TRUE(s2 >= s1);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20129
@Test_Description:    To test the functionality of greater than or equal Operator.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611900, 611906, 611918
*******************************************************************************/
TEST(StringTest, operatorEquelTo)
{
  ara::core::internal::basic_string<char> s2("ABC");
  std::basic_string<char> s1("ABC");
  /* @Start: UT_CORE_20129 */
  /* @TS1: Invoke the Operator functions of class string . */
  /* @TE1: Operator functions should successfuly verify the functionality. */
  /* @End: UT_CORE_20129 */
  EXPECT_TRUE(s2 != "Unit");
  EXPECT_TRUE(s2 == s1);
  EXPECT_FALSE(s2 != s1);
  EXPECT_FALSE(s2 < s1);
  EXPECT_TRUE(s2 <= s1);
  EXPECT_FALSE(s2 > s1);
  EXPECT_TRUE(s2 >= s1);
  EXPECT_TRUE(s1 == s2);
  EXPECT_FALSE(s1 != s2);
  EXPECT_FALSE(s1 < s2);
  EXPECT_TRUE(s1 <= s2);
  EXPECT_FALSE(s1 > s2);
  EXPECT_TRUE(s1 >= s2);
  EXPECT_FALSE("Text" == s2);
  EXPECT_TRUE("Text" != s2);

  EXPECT_FALSE("Text" < s2);
  EXPECT_TRUE(s2 < "Text");
  EXPECT_FALSE("Text" <= s2);
  EXPECT_TRUE(s2 <= "Text");

  EXPECT_TRUE("Text" > s2);
  EXPECT_FALSE(s2 > "Text");
  EXPECT_TRUE("Text" >= s2);
  EXPECT_FALSE(s2 >= "Text");
  // s1>>s2;
}
/*******************************************************************************
@TestCaseId:          UT_CORE_20130
@Test_Description:    To test the functionality of move assign std::basic_string.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611648, 611651, 611652
*******************************************************************************/
TEST(StringTest, MoveAssignStdBasicString)
{
  std::basic_string<char> s1("ABC");
  ara::core::internal::basic_string<char> s2;
  s2 = std::move(s1);
  /* @Start: UT_CORE_20130 */
  /* @TS1: Invoke the std::move() for s2. */
  /* @TE1: std::move() should successfully move the data. Result should be equal to the "ABC". */
  /* @TS2: Invoke the append() with s2. */
  /* @TE2: append() should successfully append the data .Result should be equal to the "ABC". */
  /* @End: UT_CORE_20130 */
  EXPECT_TRUE(s2 == "ABC");
  EXPECT_TRUE(s2.append(s1, 0, 2) == "ABC");
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0573
@Test_Description:    To test the functionality of IsStreamWorks.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611648, 611651, 611652
*******************************************************************************/
TEST(StringTest, IsStreamWorks)
{
  ara::core::String s = "0x5572638c8bb9";

  std::istringstream is;
  is >> s;
  std::string result = is.str();
  /* @Start: UT_CORE_0573 */
  /* @TS1: Create an ara::core::String data type with result. */
  /* @TE1: Result should be equal to empty string. */
  /* @End: UT_CORE_0573 */
  ASSERT_THAT(result, Eq(""));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0574
@Test_Description:    To test the functionality of rend().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringTest
@Requirement Tracing: -
@Design Tracing:      611802, 611803, 611804
*******************************************************************************/
TEST(StringTest, rendString)
{
  const ara::core::String s1 = "test";
  auto it = s1.rend();
  /* @Start: UT_CORE_0574 */
  /* @TS1: Invoke rend() of string class. */
  /* @TE1: Result should be equal to "\0". */
  /* @End: UT_CORE_0574 */
  EXPECT_EQ(*it, '\0');
}
