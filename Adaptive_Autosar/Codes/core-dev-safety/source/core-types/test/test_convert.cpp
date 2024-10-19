// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_convert.cpp
//
// Purpose     : This file provides the unit test cases of class convert.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/convert.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "iceoryx_hoofs/cxx/convert.hpp"
using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::SetArgReferee;
using NumberType = ara::core::convert::NumberType;

/*******************************************************************************
@TestCaseId:          UT_CORE_3058
@Test_Description:    To test the functionality of type toString(toString_uint8_t).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642401, 642376
*******************************************************************************/
TEST(ConvertTest, toString_uint8_t)
{
  constexpr uint8_t DATA = 131U;
  EXPECT_THAT(ara::core::convert::toString(DATA), "131");

  /* @Start: UT_CORE_3058 */
  /* @TS1: Invoke the toString(). */
  /* @TE1: toString() should return "131". */
  /* @End: UT_CORE_3058 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3059
@Test_Description:    To test the functionality of type toString(toString_int8_t).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642401, 642376
*******************************************************************************/
TEST(ConvertTest, toString_int8_t)
{
  constexpr int8_t DATA = 31U;
  EXPECT_THAT(ara::core::convert::toString(DATA), "31");

  /* @Start: UT_CORE_3059 */
  /* @TS1: Invoke the toString(). */
  /* @TE1: toString() should return "31". */
  /* @End: UT_CORE_3059 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3060
@Test_Description:    To test the functionality of type toString(toString_Integer).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642401, 642376
*******************************************************************************/
TEST(ConvertTest, toString_Integer)
{
  constexpr int DATA = 33331;
  EXPECT_THAT(ara::core::convert::toString(DATA), "33331");

  /* @Start: UT_CORE_3060 */
  /* @TS1: Invoke the toString(). */
  /* @TE1: toString() should return "33331". */
  /* @End: UT_CORE_3060 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3061
@Test_Description:    To test the functionality of type toString(toString_Float).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642401, 642376
*******************************************************************************/
TEST(ConvertTest, toString_Float)
{
  constexpr float DATA = 333.1F;
  EXPECT_THAT(ara::core::convert::toString(DATA), "333.1");

  /* @Start: UT_CORE_3061 */
  /* @TS1: Invoke the toString(). */
  /* @TE1: toString() should return "33331". */
  /* @End: UT_CORE_3061 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3062
@Test_Description:    To test the functionality of type toString(toString_LongLongUnsignedInt).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642401, 642376
*******************************************************************************/
TEST(ConvertTest, toString_LongLongUnsignedInt)
{
  constexpr long long unsigned DATA = 123LLU;
  EXPECT_THAT(ara::core::convert::toString(DATA), "123");

  /* @Start: UT_CORE_3062 */
  /* @TS1: Invoke the toString(). */
  /* @TE1: toString() should return "123". */
  /* @End: UT_CORE_3062 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3063
@Test_Description:    To test the functionality of type toString(toString_Char).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642401, 642376
*******************************************************************************/
TEST(ConvertTest, toString_Char)
{
  constexpr char DATA = 'x';
  EXPECT_THAT(ara::core::convert::toString(DATA), "x");

  /* @Start: UT_CORE_3063 */
  /* @TS1: Invoke the toString(). */
  /* @TE1: toString() should return "x". */
  /* @End: UT_CORE_3063 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3064
@Test_Description:    To test the functionality of type toString(toString_String).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642401, 642376
*******************************************************************************/
TEST(ConvertTest, toString_String)
{
  const std::string DATA = "hello";
  EXPECT_THAT(ara::core::convert::toString(DATA), "hello");

  /* @Start: UT_CORE_3064 */
  /* @TS1: Invoke the toString(). */
  /* @TE1: toString() should return "hello". */
  /* @End: UT_CORE_3064 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3065
@Test_Description:    To test the functionality of type toString(toString_StringConvertableClass).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642401, 642376
*******************************************************************************/
TEST(ConvertTest, toString_StringConvertableClass)
{
  struct A
  {
    operator std::string() const
    {
      return "fuu";
    }
  };

  EXPECT_THAT(ara::core::convert::toString(A()), "fuu");

  /* @Start: UT_CORE_3065 */
  /* @TS1: Invoke the toString(). */
  /* @TE1: toString() should return "fuu". */
  /* @End: UT_CORE_3065 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3066
@Test_Description:    To test the functionality of type fromString(FromString_String).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, FromString_String)
{
  std::string source = "hello";
  std::string destination;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(source, destination);

  /* @Start: UT_CORE_3066 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return true. */
  /* @End: UT_CORE_3066 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3067
@Test_Description:    To test the functionality of type fromString(fromString_Char_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_Char_Success)
{
  std::string source = "h";
  char destination = '\0';
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(source[0], destination);

  /* @Start: UT_CORE_3067 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return true. */
  /* @End: UT_CORE_3067 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3068
@Test_Description:    To test the functionality of type fromString(fromString_Char_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_Char_Fail)
{
  std::string source = "hasd";
  char destination = '\0';
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3068 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3068 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3069
@Test_Description:    To test the functionality of type stringIsNumber(stringIsNumber_IsINTEGER).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642403, 642376
*******************************************************************************/
TEST(ConvertTest, stringIsNumber_IsINTEGER)
{
  EXPECT_THAT(ara::core::convert::stringIsNumber("123921301", NumberType::INTEGER), true);

  /* @Start: UT_CORE_3069 */
  /* @TS1: Invoke the stringIsNumber(). */
  /* @TE1: stringIsNumber() should return true. */
  /* @End: UT_CORE_3069 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3070
@Test_Description:    To test the functionality of type stringIsNumber(stringIsNumber_IsEmpty).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642403, 642376
*******************************************************************************/
TEST(ConvertTest, stringIsNumber_IsEmpty)
{
  EXPECT_THAT(ara::core::convert::stringIsNumber("", NumberType::INTEGER), false);

  /* @Start: UT_CORE_3070 */
  /* @TS1: Invoke the stringIsNumber(). */
  /* @TE1: stringIsNumber() should return false. */
  /* @End: UT_CORE_3070 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3071
@Test_Description:    To test the functionality of type stringIsNumber(stringIsNumber_IsZero).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642403, 642376
*******************************************************************************/
TEST(ConvertTest, stringIsNumber_IsZero)
{
  EXPECT_THAT(ara::core::convert::stringIsNumber("0", NumberType::INTEGER), true);

  /* @Start: UT_CORE_3071 */
  /* @TS1: Invoke the stringIsNumber(). */
  /* @TE1: stringIsNumber() should return true. */
  /* @End: UT_CORE_3071 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3072
@Test_Description:    To test the functionality of type stringIsNumber(stringIsNumber_INTEGERWithSign).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642403, 642376
*******************************************************************************/
TEST(ConvertTest, stringIsNumber_INTEGERWithSign)
{
  EXPECT_THAT(ara::core::convert::stringIsNumber("-521", NumberType::INTEGER), true);

  /* @Start: UT_CORE_3072 */
  /* @TS1: Invoke the stringIsNumber(). */
  /* @TE1: stringIsNumber() should return true. */
  /* @End: UT_CORE_3072 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3073
@Test_Description:    To test the functionality of type stringIsNumber
                        (stringIsNumber_INTEGERWithSignPlacedWrongly).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642403, 642376
*******************************************************************************/
TEST(ConvertTest, stringIsNumber_INTEGERWithSignPlacedWrongly)
{
  EXPECT_THAT(ara::core::convert::stringIsNumber("2-3", NumberType::UNSIGNED_INTEGER), false);

  /* @Start: UT_CORE_3073 */
  /* @TS1: Invoke the stringIsNumber(). */
  /* @TE1: stringIsNumber() should return false. */
  /* @End: UT_CORE_3073 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3074
@Test_Description:    To test the functionality of type stringIsNumber
                        (stringIsNumber_SimpleFLOAT).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642403, 642376
*******************************************************************************/
TEST(ConvertTest, stringIsNumber_SimpleFLOAT)
{
  EXPECT_THAT(ara::core::convert::stringIsNumber("123.456", NumberType::FLOAT), true);

  /* @Start: UT_CORE_3074 */
  /* @TS1: Invoke the stringIsNumber(). */
  /* @TE1: stringIsNumber() should return true. */
  /* @End: UT_CORE_3074 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3075
@Test_Description:    To test the functionality of type stringIsNumber
                        (stringIsNumber_MultiDotFLOAT).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642403, 642376
*******************************************************************************/
TEST(ConvertTest, stringIsNumber_MultiDotFLOAT)
{
  EXPECT_THAT(ara::core::convert::stringIsNumber("11.1.123", NumberType::FLOAT), false);

  /* @Start: UT_CORE_3075 */
  /* @TS1: Invoke the stringIsNumber(). */
  /* @TE1: stringIsNumber() should return false. */
  /* @End: UT_CORE_3075 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3076
@Test_Description:    To test the functionality of type stringIsNumber
                        (stringIsNumber_FLOATWithSign).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642403, 642376
*******************************************************************************/
TEST(ConvertTest, stringIsNumber_FLOATWithSign)
{
  EXPECT_THAT(ara::core::convert::stringIsNumber("+123.321", NumberType::FLOAT), true);

  /* @Start: UT_CORE_3076 */
  /* @TS1: Invoke the stringIsNumber(). */
  /* @TE1: stringIsNumber() should return true. */
  /* @End: UT_CORE_3076 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3077
@Test_Description:    To test the functionality of type stringIsNumber
                        (stringIsNumber_NumberWithLetters).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642403, 642376
*******************************************************************************/
TEST(ConvertTest, stringIsNumber_NumberWithLetters)
{
  EXPECT_THAT(ara::core::convert::stringIsNumber("+123a.123", NumberType::FLOAT), false);

  /* @Start: UT_CORE_3077 */
  /* @TS1: Invoke the stringIsNumber(). */
  /* @TE1: stringIsNumber() should return false. */
  /* @End: UT_CORE_3077 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3078
@Test_Description:    To test the functionality of type fromString
                        (fromString_FLOAT_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_FLOAT_Success)
{
  std::string source = "123.01";
  float destination = 0.0F;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_FLOAT_EQ(destination, 123.01F);

  /* @Start: UT_CORE_3078 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return true. */
  /* @End: UT_CORE_3078 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3079
@Test_Description:    To test the functionality of type fromString
                        (fromString_FLOAT_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_FLOAT_Fail)
{
  std::string source = "hasd";
  float destination = 0.0F;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3079 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3079 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3080
@Test_Description:    To test the functionality of type fromString
                        (fromString_Double_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_Double_Success)
{
  std::string source = "123.04";
  double destination = 0.0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(destination, 123.04);

  /* @Start: UT_CORE_3080 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return true. */
  /* @End: UT_CORE_3080 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3081
@Test_Description:    To test the functionality of type fromString
                        (fromString_Double_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_Double_Fail)
{
  std::string source = "hasd";
  double destination = 0.0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3081 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3081 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3082
@Test_Description:    To test the functionality of type fromString
                        (fromString_LongDouble_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_LongDouble_Success)
{
  std::string source = "121.01";
  long double destination = 0.0;
  constexpr long double VERIFY = 121.01;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(static_cast<double>(destination), static_cast<double>(VERIFY));

  /* @Start: UT_CORE_3082 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1.1: fromString() should return true. */
  /* @TE1.2: fromString() should return 121.01. */
  /* @End: UT_CORE_3082 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3083
@Test_Description:    To test the functionality of type fromString
                        (fromString_LongDouble_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_LongDouble_Fail)
{
  std::string source = "hasd";
  double destination = 0.0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3083 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3083 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3084
@Test_Description:    To test the functionality of type fromString
                        (fromString_UNSIGNED_Int_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_UNSIGNED_Int_Success)
{
  std::string source = "100";
  unsigned int destination = 0.0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(destination, 100U);

  /* @Start: UT_CORE_3084 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return true. */
  /* @End: UT_CORE_3084 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3085
@Test_Description:    To test the functionality of type fromString
                        (fromString_UNSIGNED_Int_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_UNSIGNED_Int_Fail)
{
  std::string source = "-331";
  unsigned int destination = 0U;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3085 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3085 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3086
@Test_Description:    To test the functionality of type fromString
                        (fromString_UNSIGNED_LongInt_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_UNSIGNED_LongInt_Success)
{
  std::string source = "999";
  uint64_t destination = 0U;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(destination, 999LU);

  /* @Start: UT_CORE_3086 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return true. */
  /* @End: UT_CORE_3086 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3087
@Test_Description:    To test the functionality of type fromString
                        (fromString_UNSIGNED_LongInt_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_UNSIGNED_LongInt_Fail)
{
  std::string source = "-a123";
  uint64_t destination = 0U;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3087 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3087 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3088
@Test_Description:    To test the functionality of type fromString
                        (fromString_Int_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_Int_Fail)
{
  std::string source = "-+321";
  int destination = 0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3088 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3088 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3089
@Test_Description:    To test the functionality of type fromString
                        (fromString_ShortInt_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_ShortInt_Success)
{
  std::string source = "12345";
  short destination = 0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(destination, 12345);

  /* @Start: UT_CORE_3089 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return true. */
  /* @End: UT_CORE_3089 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3090
@Test_Description:    To test the functionality of type fromString
                        (fromString_ShortInt_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_ShortInt_Fail)
{
  std::string source = "-+123321";
  short destination = 0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3090 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3090 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3091
@Test_Description:    To test the functionality of type fromString
                        (fromString_Bool_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_Bool_Success)
{
  std::string source = "1";
  bool destination = false;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(destination, true);

  /* @Start: UT_CORE_3091 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1.1: fromString() should return true. */
  /* @TE1.2: fromString() should return true. */
  /* @End: UT_CORE_3091 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3092
@Test_Description:    To test the functionality of type fromString
                        (fromString_Bool_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_Bool_Fail)
{
  std::string source = "-+222";
  bool destination = false;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3092 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3092 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3093
@Test_Description:    To test the functionality of type fromString
                        (fromString_UShortInt_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_UShortInt_Success)
{
  std::string source = "333";
  unsigned short destination = 0U;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(destination, 333);

  /* @Start: UT_CORE_3093 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1.1: fromString() should return true. */
  /* @TE1.2: fromString() should return 333. */
  /* @End: UT_CORE_3093 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3094
@Test_Description:    To test the functionality of type fromString
                        (fromString_UShortInt_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_UShortInt_Fail)
{
  std::string source = "-+111";
  unsigned short destination = 0U;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3094 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3094 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3095
@Test_Description:    To test the functionality of type fromString
                        (fromString_LongInt_Success).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_LongInt_Success)
{
  std::string source = "-1123";
  int64_t destination = 0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  EXPECT_THAT(destination, -1123L);

  /* @Start: UT_CORE_3095 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1.1: fromString() should return true. */
  /* @TE1.2: fromString() should return 1123L. */
  /* @End: }UT_CORE_3095
 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3096
@Test_Description:    To test the functionality of type fromString
                        (fromString_LongInt_Fail).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_LongInt_Fail)
{
  std::string source = "-a121";
  int64_t destination = 0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3096 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1: fromString() should return false. */
  /* @End: UT_CORE_3096 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3097
@Test_Description:    To test the functionality of type fromString
                        (fromString_MinMaxShort).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_MinMaxShort)
{
  std::string source = "32767";
  int16_t destination = 0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  source = "32768";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);
  source = "-32768";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  source = "-32769";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3097 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1.1: fromString() should return true. */
  /* @TE1.2: fromString() should return false. */
  /* @TE1.3: fromString() should return true. */
  /* @TE1.4: fromString() should return false. */
  /* @End: UT_CORE_3097 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3098
@Test_Description:    To test the functionality of type fromString
                        (fromString_MinMaxUNSIGNED_Short).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_MinMaxUNSIGNED_Short)
{
  std::string source = "65535";
  uint16_t destination = 0U;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  source = "65536";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);
  source = "0";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  source = "-1";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3098 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1.1: fromString() should return true. */
  /* @TE1.2: fromString() should return false. */
  /* @TE1.3: fromString() should return true. */
  /* @TE1.4: fromString() should return false. */
  /* @End: UT_CORE_3098 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3099
@Test_Description:    To test the functionality of type fromString
                        (fromString_MinMaxInt).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_MinMaxInt)
{
  std::string source = "2147483647";
  int32_t destination = 0;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  source = "2147483648";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);
  source = "-2147483648";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  source = "-2147483649";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3099 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1.1: fromString() should return true. */
  /* @TE1.2: fromString() should return false. */
  /* @TE1.3: fromString() should return true. */
  /* @TE1.4: fromString() should return false. */
  /* @End: UT_CORE_3099 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_3100
@Test_Description:    To test the functionality of type fromString
                        (fromString_MinMaxUNSIGNED_Int).
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ConvertTest
@Design Tracing:      642402, 642376
*******************************************************************************/
TEST(ConvertTest, fromString_MinMaxUNSIGNED_Int)
{
  std::string source = "4294967295";
  uint32_t destination = 0U;
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  source = "4294967296";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);
  source = "0";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), true);
  source = "-1";
  EXPECT_THAT(ara::core::convert::fromString(source.c_str(), destination), false);

  /* @Start: UT_CORE_3100 */
  /* @TS1: Invoke the fromString(). */
  /* @TE1.1: fromString() should return true. */
  /* @TE1.2: fromString() should return false. */
  /* @TE1.3: fromString() should return true. */
  /* @TE1.4: fromString() should return false. */
  /* @End: UT_CORE_3100 */
}
