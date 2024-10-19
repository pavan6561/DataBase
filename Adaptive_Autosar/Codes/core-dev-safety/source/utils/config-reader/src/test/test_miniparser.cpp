// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_miniparser.cpp
//
// Purpose     : This file provides the unit test cases of class minijson_reader.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/configreader/config_reader.h"
#include "ara/core/optional.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ara/core/configreader/invoke.h"
#include <string>
#include <bitset>
#include <istream>
#include <ostream>
#include <iostream>
#include <cstddef>

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Ne;
template <typename Context>
void test_context_helper(Context& context)
{
  {
    Context temp(std::move(context));
    context = std::move(temp);
  }

  context.BeginLiteral();
  bool isLoop = true;
  while (isLoop)
  {
    switch (context.ReadOffset())
    {
    case 0:
      ASSERT_EQ('h', context.Read());
      context.Write('H');
      break;
    case 1:
      ASSERT_EQ('e', context.Read());
      context.Write('e');
      break;
    case 2:
      ASSERT_EQ('l', context.Read());
      context.Write('l');
      break;
    case 3:
      ASSERT_EQ('l', context.Read());
      context.Write('l');
      break;
    case 4:
      ASSERT_EQ('o', context.Read());
      context.Write('o');
      break;
    case 5:
      ASSERT_EQ(' ', context.Read());
      context.Write(0);
      ASSERT_STREQ("Hello", context.CurrentLiteral());
      context.BeginLiteral();
      break;
    case 6:
      ASSERT_EQ('w', context.Read());
      context.Write('W');
      {
        Context temp(std::move(context));
        context = std::move(temp);
      }
      break;
    case 7:
      ASSERT_EQ('o', context.Read());
      context.Write('o');
      break;
    case 8:
      ASSERT_EQ('r', context.Read());
      context.Write('r');
      break;
    case 9:
      ASSERT_EQ('l', context.Read());
      context.Write('l');
      break;
    case 10:
      ASSERT_EQ('d', context.Read());
      context.Write('d');
      break;
    case 11:
      ASSERT_EQ('.', context.Read());
      context.Write(0);
      break;
    case 12:
      ASSERT_EQ(0, context.Read());
      isLoop = false;
      break;
    }
  }

  {
    Context temp(std::move(context));
    context = std::move(temp);
  }

  ASSERT_EQ(0, context.Read());
  ASSERT_EQ(12U, context.ReadOffset());
  ASSERT_STREQ("World", context.CurrentLiteral());

  ASSERT_EQ(ara::core::configreader::detail::ContextBase::kNestedStatusNone, context.NestedStatus());

  context.BeginNested(ara::core::configreader::detail::ContextBase::kNestedStatusObject);
  ASSERT_EQ(ara::core::configreader::detail::ContextBase::kNestedStatusObject, context.NestedStatus());
  ASSERT_EQ(1U, context.NestingLevel());
  context.BeginNested(ara::core::configreader::detail::ContextBase::kNestedStatusArray);
  ASSERT_EQ(ara::core::configreader::detail::ContextBase::kNestedStatusArray, context.NestedStatus());

  Context other_context(std::move(context));
  ASSERT_EQ(2U, other_context.NestingLevel());
  other_context.EndNested();
  ASSERT_EQ(1U, other_context.NestingLevel());
  other_context.EndNested();
  ASSERT_EQ(0U, other_context.NestingLevel());

  other_context.ResetNestedStatus();
  ASSERT_EQ(ara::core::configreader::detail::ContextBase::kNestedStatusNone, other_context.NestedStatus());

  using std::swap;
  swap(context, other_context);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1500
@Test_Description:    To test the functionality of passing the buffer to context.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611075, 611093, 611106, 611107, 611117, 614016
*******************************************************************************/
TEST(minijson_reader, BufferContext)
{
  char buffer[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '.'};
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));
  test_context_helper(BufferContext);
  /* @Start: UT_CORE_1500 */
  /* @TS1: Create a char array data type with buffer variable. */
  /* @TE1: buffer should equal to "Hello". */
  /* @End: UT_CORE_1500 */
  ASSERT_STREQ("Hello", buffer);
  /* @Start: UT_CORE_1500 */
  /* @TS2: Create a char array data type with buffer variable. */
  /* @TE2: buffer should equal to "World". */
  /* @End: UT_CORE_1500 */
  ASSERT_STREQ("World", buffer + 6);
  /* @Start: UT_CORE_1500 */
  /* @TS3: Invoke the BufferContext class. */
  /* @TE3: BufferContext should matched. */
  /* @End: UT_CORE_1500 */
  ASSERT_DEATH({ BufferContext.Write('x'); }, "");
  BufferContext.BeginLiteral();
  /* @Start: UT_CORE_1500 */
  /* @TS4: Invoke the BufferContext class. */
  /* @TE4: BufferContext should equal to buffer + sizeof(buffer). */
  /* @End: UT_CORE_1500 */
  ASSERT_EQ(buffer + sizeof(buffer), BufferContext.CurrentLiteral());
  /* @Start: UT_CORE_1500 */
  /* @TS5: Invoke the BufferContext class. */
  /* @TE5: BufferContext should matched. */
  /* @End: UT_CORE_1500 */
  ASSERT_DEATH({ BufferContext.Write('x'); }, "");
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1501
@Test_Description:    To test the functionality of passing the const buffer to context.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611069, 611091, 611106, 611107, 611112, 611119, 611120
*******************************************************************************/
TEST(minijson_reader, ConstBufferContext)
{
  const char buffer[] = "hello world.";
  ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);
  const char* const originalWriteBuffer = ConstBufferContext.CurrentLiteral();
  test_context_helper(ConstBufferContext);
  /* @Start: UT_CORE_1501 */
  /* @TS1: Create a char array data type with buffer variable. */
  /* @TE1: buffer should equal to "hello world.". */
  /* @End: UT_CORE_1501 */
  ASSERT_STREQ("hello world.", buffer); // no side effects
  /* @Start: UT_CORE_1501 */
  /* @TS2: Invoke the ConstBufferContext class. */
  /* @TE2: ConstBufferContext should matched. */
  /* @End: UT_CORE_1501 */
  ASSERT_DEATH({ ConstBufferContext.Write('x'); }, "");
  ConstBufferContext.BeginLiteral();
  /* @Start: UT_CORE_1501 */
  /* @TS3: Invoke the ConstBufferContext class. */
  /* @TE3: ConstBufferContext should equal to originalWriteBuffer + strlen(buffer). */
  /* @End: UT_CORE_1501 */
  ASSERT_EQ(originalWriteBuffer + strlen(buffer), ConstBufferContext.CurrentLiteral());
  /* @Start: UT_CORE_1501 */
  /* @TS4: Invoke the ConstBufferContext class. */
  /* @TE4: ConstBufferContext should matched. */
  /* @End: UT_CORE_1501 */
  ASSERT_DEATH({ ConstBufferContext.Write('x'); }, "");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1502
@Test_Description:    To test the functionality of istream context.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      6111098
*******************************************************************************/
TEST(minijson_reader, IstreamContext)
{
  std::istringstream buffer("hello world.");
  ara::core::configreader::IstreamContext IstreamContext(buffer);
  EXPECT_NO_THROW(test_context_helper(IstreamContext));
  /* @Start: UT_CORE_1502 */
  /* @TS1: Invoke the test_context_helper(IstreamContext) with buffer data as "hello world". */
  /* @TE1: test_context_helper() shuld successfully write buffer data "hello world". */
  /* @End: UT_CORE_1502 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1503
@Test_Description:    To test the functionality of parse error.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611145, 611145, 611126, 611110, 611061, 611060
*******************************************************************************/
TEST(minijson_reader, ParseError)
{
  {
    ara::core::configreader::BufferContext BufferContext(nullptr, 0);
    ara::core::configreader::ParseError ParseError(
        BufferContext, ara::core::configreader::ParseError::kUnknown);
    /* @Start: UT_CORE_1503 */
    /* @TS1: Invoke the Offset() with ParseError class.*/
    /* @TE1: Offset() should equal to 0U. */
    /* @End: UT_CORE_1503 */
    ASSERT_EQ(0U, ParseError.Offset());
    /* @Start: UT_CORE_1503 */
    /* @TS2: Invoke the Reason() with ParseError class. */
    /* @TE2: Reason() should equal to ara::core::configreader::ParseError::kUnknown. */
    /* @End: UT_CORE_1503 */
    ASSERT_EQ(ara::core::configreader::ParseError::kUnknown, ParseError.Reason());
    /* @Start: UT_CORE_1503 */
    /* @TS3: Invoke the what() with ParseError class. */
    /* @TE3: what() and "kUnknown parse error" should equal. */
    /* @End: UT_CORE_1503 */
    ASSERT_STREQ("kUnknown parse error", ParseError.what());
  }
  {
    const char buffer[] = "hello world.";
    ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);
    ConstBufferContext.Read();
    ConstBufferContext.Read();
    /* @Start: UT_CORE_1503 */
    /* @TS4: Invoke the ReadOffset() with ConstBufferContext class. */
    /* @TE4: ReadOffset() should equal to 2U. */
    /* @End: UT_CORE_1503 */
    ASSERT_EQ(2U, ConstBufferContext.ReadOffset());

    ara::core::configreader::ParseError ParseError(
        ConstBufferContext, ara::core::configreader::ParseError::kUnknown);
    /* @Start: UT_CORE_1503 */
    /* @TS5: Invoke the Offset() with ParseError class. */
    /* @TE5: Offset() should equal to 1U. */
    /* @End: UT_CORE_1503 */
    ASSERT_EQ(1U, ParseError.Offset());
    /* @Start: UT_CORE_1503 */
    /* @TS6: Invoke the Reason() with ParseError class.*/
    /* @TE6: Reason() should equal to kUnknown. */
    /* @End: UT_CORE_1503 */
    ASSERT_EQ(ara::core::configreader::ParseError::kUnknown, ParseError.Reason());
    /* @Start: UT_CORE_1503 */
    /* @TS7: Invoke the what() with ParseError class.*/
    /* @TE7: what() should equal to "kUnknown parse error". */
    /* @End: UT_CORE_1503 */
    ASSERT_STREQ("kUnknown parse error", ParseError.what());
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1504
@Test_Description:    To test the functionality of utf16_to_utf32 valid units.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611160, 611063, 611062
*******************************************************************************/
TEST(minijson_reader_detail, utf16_to_utf32)
{
  // code points 0000 to D7FF and E000 to FFFF
  /* @Start: UT_CORE_1504 */
  /* @TS1: Invoke the all Utf16ToUtf32() range of 000000 to 00FFFF. */
  /* @TE1.1: 0x000000u should equal to Utf16ToUtf32(0x0000, 0x0000).*/
  /* @TE1.2: 0x000001u should equal to Utf16ToUtf32(0x0001, 0x0000).*/
  /* @TE1.3: 0x00D7FEu should equal to Utf16ToUtf32(0xD7FE, 0x0000).*/
  /* @TE1.4: 0x00D7FFu should equal to Utf16ToUtf32(0xD7FF, 0x0000).*/
  /* @TE1.5: 0x00E000u should equal to Utf16ToUtf32(0xE000, 0x0000).*/
  /* @TE1.6: 0x00FFFFu should equal to Utf16ToUtf32(0xFFFF, 0x0000).*/
  /* @End: UT_CORE_1504 */
  ASSERT_EQ(0x000000u, ara::core::configreader::detail::Utf16ToUtf32(0x0000, 0x0000));
  ASSERT_EQ(0x000001u, ara::core::configreader::detail::Utf16ToUtf32(0x0001, 0x0000));
  ASSERT_EQ(0x00D7FEu, ara::core::configreader::detail::Utf16ToUtf32(0xD7FE, 0x0000));
  ASSERT_EQ(0x00D7FFu, ara::core::configreader::detail::Utf16ToUtf32(0xD7FF, 0x0000));
  ASSERT_EQ(0x00E000u, ara::core::configreader::detail::Utf16ToUtf32(0xE000, 0x0000));
  ASSERT_EQ(0x00FFFFu, ara::core::configreader::detail::Utf16ToUtf32(0xFFFF, 0x0000));
  /* @Start: UT_CORE_1504 */
  /* @TS2: Invoke the all utf16_to_utf32 range of 000000 to 10FFFF. */
  /* @TE2.1: 0x000000u should equal to Utf16ToUtf32(0xD800, 0xDC00).*/
  /* @TE2.2: 0x000001u should equal to Utf16ToUtf32(0xD800, 0xDC01).*/
  /* @TE2.3: 0x10FFFEu should equal to Utf16ToUtf32(0xDBFF, 0xDFFE).*/
  /* @TE2.4: 0x10FFFFu should equal to Utf16ToUtf32(0xDBFF, 0xDFFF).*/
  /* @End: UT_CORE_1504 */
  // code points 010000 to 10FFFF
  ASSERT_EQ(0x010000u, ara::core::configreader::detail::Utf16ToUtf32(0xD800, 0xDC00));
  ASSERT_EQ(0x010001u, ara::core::configreader::detail::Utf16ToUtf32(0xD800, 0xDC01));
  ASSERT_EQ(0x10FFFEu, ara::core::configreader::detail::Utf16ToUtf32(0xDBFF, 0xDFFE));
  ASSERT_EQ(0x10FFFFu, ara::core::configreader::detail::Utf16ToUtf32(0xDBFF, 0xDFFF));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1505
@Test_Description:    To test the functionality of utf16_to_utf32 invalid units.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611160, 611063, 611062
*******************************************************************************/
TEST(minijson_reader_detail, utf16_to_utf32_invalid)
{
  /* @Start: UT_CORE_1505 */
  /* @TS1: Invoke the Utf16ToUtf32(0x0000, 0x0001).*/
  /* @TE1: Utf16ToUtf32() should return ara::core::configreader::detail::EncodingError. */
  /* @End: UT_CORE_1505 */
  ASSERT_THROW(ara::core::configreader::detail::Utf16ToUtf32(0x0000, 0x0001),
      ara::core::configreader::detail::EncodingError);
  /* @Start: UT_CORE_1505 */
  /* @TS2: Invoke the Utf16ToUtf32(0xD800, 0xDBFF).*/
  /* @TE2: Utf16ToUtf32() should return ara::core::configreader::detail::EncodingError. */
  /* @End: UT_CORE_1505 */
  ASSERT_THROW(ara::core::configreader::detail::Utf16ToUtf32(0xD800, 0xDBFF),
      ara::core::configreader::detail::EncodingError);
  /* @Start: UT_CORE_1505 */
  /* @TS3: Invoke the Utf16ToUtf32(0xD800, 0xE000).*/
  /* @TE3: Utf16ToUtf32() should return ara::core::configreader::detail::EncodingError. */
  /* @End: UT_CORE_1505 */
  ASSERT_THROW(ara::core::configreader::detail::Utf16ToUtf32(0xD800, 0xE000),
      ara::core::configreader::detail::EncodingError);
  /* @Start: UT_CORE_1505 */
  /* @TS4: Invoke the Utf16ToUtf32(0xDC00, 0xDC00).*/
  /* @TE4: Utf16ToUtf32() should return ara::core::configreader::detail::EncodingError. */
  /* @End: UT_CORE_1505 */
  ASSERT_THROW(ara::core::configreader::detail::Utf16ToUtf32(0xDC00, 0xDC00),
      ara::core::configreader::detail::EncodingError);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1506
@Test_Description:    To test the functionality of utf32_to_utf8 valid units.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611160, 611063, 611062
*******************************************************************************/
TEST(minijson_reader_detail, utf32_to_utf8)
{
  // 1 byte
  {
    const std::array<std::uint8_t, 4> expected{0x00, 0x00, 0x00, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS1: Invoke the Utf32ToUtf8(0x000000).*/
    /* @TE1: Utf32ToUtf8(0x000000) should equel to {0x00, 0x00, 0x00, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x000000));
  }
  {
    const std::array<std::uint8_t, 4> expected{0x01, 0x00, 0x00, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS2: Invoke the Utf32ToUtf8(0x000001).*/
    /* @TE2: Utf32ToUtf8(0x000001) should equal to {0x01, 0x00, 0x00, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x000001));
  }
  {
    const std::array<std::uint8_t, 4> expected{0x7E, 0x00, 0x00, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS3: Invoke the Utf32ToUtf8(0x00007E).*/
    /* @TE3: Utf32ToUtf8(0x00007E) should equal to {0x7E, 0x00, 0x00, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x00007E));
  }
  {
    const std::array<std::uint8_t, 4> expected{0x7F, 0x00, 0x00, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS4: Invoke the Utf32ToUtf8(0x00007F).*/
    /* @TE4: Utf32ToUtf8(0x00007E) should equal to {0x7E, 0x00, 0x00, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x00007F));
  }

  // 2 bytes
  {
    const std::array<std::uint8_t, 4> expected{0xC2, 0x80, 0x00, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS5: Invoke the Utf32ToUtf8(0x000080).*/
    /* @TE5: Utf32ToUtf8(0x000080) should equal to {0xC2, 0x80, 0x00, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x000080));
  }
  {
    const std::array<std::uint8_t, 4> expected{0xC2, 0x81, 0x00, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS6: Invoke the Utf32ToUtf8(0x000081).*/
    /* @TE6: Utf32ToUtf8(0x000081) should equal to {0xC2, 0x81, 0x00, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x000081));
  }
  {
    const std::array<std::uint8_t, 4> expected{0xDF, 0xBE, 0x00, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS7: Invoke the Utf32ToUtf8(0x0007FE).*/
    /* @TE7: Utf32ToUtf8(0x0007FE) should equal to {0xDF, 0xBE, 0x00, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x0007FE));
  }
  {
    const std::array<std::uint8_t, 4> expected{0xDF, 0xBF, 0x00, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS8: Invoke the Utf32ToUtf8(0x0007FF).*/
    /* @TE8: Utf32ToUtf8(0x0007FF) should equal to {0xDF, 0xBF, 0x00, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x0007FF));
  }

  // 3 bytes
  {
    const std::array<std::uint8_t, 4> expected{0xE0, 0xA0, 0x80, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS9: Invoke the Utf32ToUtf8(0x000800).*/
    /* @TE9: Utf32ToUtf8(0x000800) should equal to {0xE0, 0xA0, 0x80, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x000800));
  }
  {
    const std::array<std::uint8_t, 4> expected{0xE0, 0xA0, 0x81, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS10: Invoke the Utf32ToUtf8(0x000801).*/
    /* @TE10: Utf32ToUtf8(0x000801) should equal to {0xE0, 0xA0, 0x81, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x000801));
  }
  {
    const std::array<std::uint8_t, 4> expected{0xEF, 0xBF, 0xBE, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS11: Invoke the Utf32ToUtf8(0x00FFFE).*/
    /* @TE11: Utf32ToUtf8(0x000801) should equal to {0xE0, 0xA0, 0x81, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x00FFFE));
  }
  {
    const std::array<std::uint8_t, 4> expected{0xEF, 0xBF, 0xBF, 0x00};
    /* @Start: UT_CORE_1506 */
    /* @TS12: Invoke the Utf32ToUtf8(0x00FFFF).*/
    /* @TE12: Utf32ToUtf8(0x00FFFF) should equal to {0xE0, 0xA0, 0x81, 0x00}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x00FFFF));
  }

  // 4 bytes
  {
    const std::array<std::uint8_t, 4> expected{0xF0, 0x90, 0x80, 0x80};
    /* @Start: UT_CORE_1506 */
    /* @TS13: Invoke the Utf32ToUtf8(0x010000).*/
    /* @TE13: Utf32ToUtf8(0x010000) should equal to {0xF0, 0x90, 0x80, 0x80}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x010000));
  }
  {
    const std::array<std::uint8_t, 4> expected{0xF0, 0x90, 0x80, 0x81};
    /* @Start: UT_CORE_1506 */
    /* @TS14: Invoke the Utf32ToUtf8(0x010001).*/
    /* @TE14: Utf32ToUtf8(0x010001) should equal to {0xF0, 0x90, 0x80, 0x81}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x010001));
  }
  {
    const std::array<std::uint8_t, 4> expected{0xF7, 0xBF, 0xBF, 0xBE};
    /* @Start: UT_CORE_1506 */
    /* @TS15: Invoke the Utf32ToUtf8(0x1FFFFE).*/
    /* @TE15: Utf32ToUtf8(0x1FFFFE) should equal to {{0xF7, 0xBF, 0xBF, 0xBE}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x1FFFFE));
  }
  {
    const std::array<std::uint8_t, 4> expected{0xF7, 0xBF, 0xBF, 0xBF};
    /* @Start: UT_CORE_1506 */
    /* @TS16: Invoke the Utf32ToUtf8(0x1FFFFF).*/
    /* @TE16: Utf32ToUtf8(0x1FFFFF) should equal to {0xF7, 0xBF, 0xBF, 0xBF}. */
    /* @End: UT_CORE_1506 */
    ASSERT_EQ(expected, ara::core::configreader::detail::Utf32ToUtf8(0x1FFFFF));
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1507
@Test_Description:    To test the functionality of utf32_to_utf8 invalid units.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611154, 611156, 611155
*******************************************************************************/
TEST(minijson_reader_detail, utf32_to_utf8_invalid)
{
  /* @Start: UT_CORE_1507 */
  /* @TS1: Invoke the Utf32ToUtf8(0x200000).*/
  /* @TE1: Utf32ToUtf8(0x200000) should return ara::core::configreader::detail::EncodingError. */
  /* @End: UT_CORE_1507 */
  // invalid code unit
  ASSERT_THROW(
      ara::core::configreader::detail::Utf32ToUtf8(0x200000), ara::core::configreader::detail::EncodingError);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1508
@Test_Description:    To test the functionality of utf16_to_utf8.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611156, 611064
*******************************************************************************/
TEST(minijson_reader_detail, utf16_to_utf8)
{
  // Just one test case, since utf16_to_utf8 calls utf16_to_utf32
  // and utf32_to_utf8, and other cases have been covered by previous tests

  const std::array<std::uint8_t, 4> expected{0xF4, 0x8F, 0xBF, 0xBF};
  /* @Start: UT_CORE_1508 */
  /* @TS1: Invoke the Utf16ToUtf8(0xDBFF, 0xDFFF).*/
  /* @TE1: Utf16ToUtf8(0xDBFF, 0xDFFF) should equal to {0xF4, 0x8F, 0xBF, 0xBF}. */
  /* @End: UT_CORE_1508 */
  ASSERT_EQ(expected, ara::core::configreader::detail::Utf16ToUtf8(0xDBFF, 0xDFFF));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1509
@Test_Description:    To test the functionality of parsing of utf16_escape_sequence.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611087
*******************************************************************************/
TEST(minijson_reader_detail, parse_utf16_escape_sequence)
{
  /* @Start: UT_CORE_1509 */
  /* @TS1: Invoke the ParseUtf16EscapeSequence({'0', '0', '0', '0'}).*/
  /* @TE1: ParseUtf16EscapeSequence({'0', '0', '0', '0'}) should equal to 0x0000u. */
  /* @End: UT_CORE_1509 */
  ASSERT_EQ(0x0000u, ara::core::configreader::detail::ParseUtf16EscapeSequence({'0', '0', '0', '0'}));
  /* @Start: UT_CORE_1509 */
  /* @TS2: Invoke the ParseUtf16EscapeSequence({'1', '0', '0', '0'}).*/
  /* @TE2: ParseUtf16EscapeSequence({'1', '0', '0', '0'}) should equal to 0x1000u. */
  /* @End: UT_CORE_1509 */
  ASSERT_EQ(0x1000u, ara::core::configreader::detail::ParseUtf16EscapeSequence({'1', '0', '0', '0'}));
  /* @Start: UT_CORE_1509 */
  /* @TS3: Invoke the ParseUtf16EscapeSequence({'2', '3', '4', '5'}).*/
  /* @TE3: ParseUtf16EscapeSequence({'2', '3', '4', '5'}) should equal to 0x2345u. */
  /* @End: UT_CORE_1509 */
  ASSERT_EQ(0x2345u, ara::core::configreader::detail::ParseUtf16EscapeSequence({'2', '3', '4', '5'}));
  /* @Start: UT_CORE_1509 */
  /* @TS4: Invoke the ParseUtf16EscapeSequence({'6', '7', '8', '9'}).*/
  /* @TE4: ParseUtf16EscapeSequence({'6', '7', '8', '9'}) should equal to 0x6789u. */
  /* @End: UT_CORE_1509 */
  ASSERT_EQ(0x6789u, ara::core::configreader::detail::ParseUtf16EscapeSequence({'6', '7', '8', '9'}));
  /* @Start: UT_CORE_1509 */
  /* @TS5: Invoke the ParseUtf16EscapeSequence({'A', '6', 'B', 'C'}).*/
  /* @TE5: ParseUtf16EscapeSequence({'A', '6', 'B', 'C'}) should equal to 0xA6BCu. */
  /* @End: UT_CORE_1509 */
  ASSERT_EQ(0xA6BCu, ara::core::configreader::detail::ParseUtf16EscapeSequence({'A', '6', 'B', 'C'}));
  /* @Start: UT_CORE_1509 */
  /* @TS6: Invoke the ParseUtf16EscapeSequence({'a', 'b', 'c', 'd'}).*/
  /* @TE6: ParseUtf16EscapeSequence({'a', 'b', 'c', 'd'}) should equal to 0xabcdu. */
  /* @End: UT_CORE_1509 */
  ASSERT_EQ(0xabcdu, ara::core::configreader::detail::ParseUtf16EscapeSequence({'a', 'b', 'c', 'd'}));
  /* @Start: UT_CORE_1509 */
  /* @TS7: Invoke the ParseUtf16EscapeSequence({'A', 'B', 'C', 'D'}).*/
  /* @TE7: ParseUtf16EscapeSequence({'A', 'B', 'C', 'D'}) should equal to 0xabcdu. */
  /* @End: UT_CORE_1509 */
  ASSERT_EQ(0xabcdu, ara::core::configreader::detail::ParseUtf16EscapeSequence({'A', 'B', 'C', 'D'}));
  /* @Start: UT_CORE_1509 */
  /* @TS8: Invoke the ParseUtf16EscapeSequence({'E', 'F', 'e', 'f'}).*/
  /* @TE8: ParseUtf16EscapeSequence({'E', 'F', 'e', 'f'}) should equal to 0xEFefu. */
  /* @End: UT_CORE_1509 */
  ASSERT_EQ(0xEFefu, ara::core::configreader::detail::ParseUtf16EscapeSequence({'E', 'F', 'e', 'f'}));
  /* @Start: UT_CORE_1509 */
  /* @TS9: Invoke the ParseUtf16EscapeSequence({'F', 'F', 'F', 'F'}).*/
  /* @TE9: ParseUtf16EscapeSequence({'F', 'F', 'F', 'F'}) should equal to 0xFFFFu. */
  /* @End: UT_CORE_1509 */
  ASSERT_EQ(0xFFFFu, ara::core::configreader::detail::ParseUtf16EscapeSequence({'F', 'F', 'F', 'F'}));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1510
@Test_Description:    To test the functionality of parsing of invalid utf16_escape_sequence.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611064
*******************************************************************************/
TEST(minijson_reader_detail, parse_utf16_escape_sequence_invalid)
{
  /* @Start: UT_CORE_1510 */
  /* @TS1: Invoke the ParseUtf16EscapeSequence({'f', 'f', 'F', 'p'}).*/
  /* @TE1: ParseUtf16EscapeSequence({'f', 'f', 'F', 'p'}) should return EncodingError. */
  /* @End: UT_CORE_1510 */
  ASSERT_THROW(ara::core::configreader::detail::ParseUtf16EscapeSequence({'f', 'f', 'F', 'p'}),
      ara::core::configreader::detail::EncodingError);
  /* @Start: UT_CORE_1510 */
  /* @TS2: Invoke the ParseUtf16EscapeSequence({'-', 'b', 'c', 'd'}).*/
  /* @TE2: ParseUtf16EscapeSequence({'-', 'b', 'c', 'd'}) should return EncodingError. */
  /* @End: UT_CORE_1510 */
  ASSERT_THROW(ara::core::configreader::detail::ParseUtf16EscapeSequence({'-', 'b', 'c', 'd'}),
      ara::core::configreader::detail::EncodingError);
  /* @Start: UT_CORE_1510 */
  /* @TS3: Invoke the ParseUtf16EscapeSequence({' ', 'a', 'b', 'c'}).*/
  /* @TE3: ParseUtf16EscapeSequence({' ', 'a', 'b', 'c'}) should return EncodingError. */
  /* @End: UT_CORE_1510 */
  ASSERT_THROW(ara::core::configreader::detail::ParseUtf16EscapeSequence({' ', 'a', 'b', 'c'}),
      ara::core::configreader::detail::EncodingError);
  /* @Start: UT_CORE_1510 */
  /* @TS4: Invoke the ParseUtf16EscapeSequence({'a', 'b', 'c', 0}).*/
  /* @TE4: ParseUtf16EscapeSequence({'a', 'b', 'c', 0}) should return EncodingError. */
  /* @End: UT_CORE_1510 */
  ASSERT_THROW(ara::core::configreader::detail::ParseUtf16EscapeSequence({'a', 'b', 'c', 0}),
      ara::core::configreader::detail::EncodingError);
}

static void test_write_utf8_char(std::array<std::uint8_t, 4> c, const char* expectedStr)
{
  char buf[] = "____";

  ara::core::configreader::BufferContext BufferContext(buf, sizeof(buf));
  BufferContext.Read();
  BufferContext.Read();
  BufferContext.Read();
  BufferContext.Read();

  ara::core::configreader::detail::LiteralIo<ara::core::configreader::BufferContext> LiteralIo(BufferContext);
  ara::core::configreader::detail::WriteUtf8Char(LiteralIo, c);
  ASSERT_STREQ(expectedStr, buf);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1511
@Test_Description:    To test the functionality of write_utf8_char.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611160, 611159, 611156, 611155
*******************************************************************************/
TEST(minijson_reader_detail, write_utf8_char)
{
  test_write_utf8_char(std::array<std::uint8_t, 4>{0x00, 0x00, 0x00, 0x00}, "");
  test_write_utf8_char(std::array<std::uint8_t, 4>{0xFF, 0x00, 0x00, 0x00}, "\xFF___");
  test_write_utf8_char(std::array<std::uint8_t, 4>{0xFF, 0xFE, 0x00, 0x00}, "\xFF\xFE__");
  test_write_utf8_char(std::array<std::uint8_t, 4>{0xFF, 0xFE, 0xFD, 0x00}, "\xFF\xFE\xFD_");
  test_write_utf8_char(std::array<std::uint8_t, 4>{0xFF, 0xFE, 0xFD, 0xFC}, "\xFF\xFE\xFD\xFC");
  /* @Start: UT_CORE_1511 */
  /* @TS1: Invoke the test_write_utf8_char .*/
  /* @TE1: test_write_utf8_char return the char. */
  /* @End: UT_CORE_1511 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1512
@Test_Description:    To test the functionality of parsing string_empty.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611160, 611138, 611137, 611114, 611107, 611106, 611087
*******************************************************************************/
TEST(minijson_reader_detail, parse_string_empty)
{
  char buffer[] = "\"";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));
  ara::core::configreader::detail::ParseString(BufferContext);
  /* @Start: UT_CORE_1512 */
  /* @TS1: Invoke the CurrentLiteral() with BufferContext class .*/
  /* @TE1: CurrentLiteral() should return null string. */
  /* @End: UT_CORE_1512 */
  ASSERT_STREQ("", BufferContext.CurrentLiteral());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1513
@Test_Description:    To test the functionality of current literal.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611149, 611150, 611151, 611152, 611104
*******************************************************************************/
TEST(minijson_reader_detail, parse_string_one_char)
{
  char buffer[] = "a\"";
  std::cout << __LINE__ << " " << __FUNCTION__ << std::endl;
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));
  ara::core::configreader::detail::ParseString(BufferContext);
  /* @Start: UT_CORE_1513 */
  /* @TS1: Invoke the CurrentLiteral() with BufferContext class .*/
  /* @TE1: CurrentLiteral() should equal to "a". */
  /* @End: UT_CORE_1513 */
  ASSERT_STREQ("a", BufferContext.CurrentLiteral());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1514
@Test_Description:    To test the functionality of current literal.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611088, 611089
*******************************************************************************/
TEST(minijson_reader_detail, parse_string_ascii)
{
  char buffer[] = "foo\"";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));
  ara::core::configreader::detail::ParseString(BufferContext);
  /* @Start: UT_CORE_1514 */
  /* @TS1: Invoke the CurrentLiteral() with BufferContext class .*/
  /* @TE1: CurrentLiteral() should equal to "foo". */
  /* @End: UT_CORE_1514 */
  ASSERT_STREQ("foo", BufferContext.CurrentLiteral());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1515
@Test_Description:    To test the functionality of current literal.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611088, 611104, 611111
*******************************************************************************/
TEST(minijson_reader_detail, parse_string_utf8)
{
  char buffer[] = "你好\"";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));
  ara::core::configreader::detail::ParseString(BufferContext);
  /* @Start: UT_CORE_1515 */
  /* @TS1: Invoke the CurrentLiteral() with BufferContext class. */
  /* @TE1: CurrentLiteral() should equal to given string. */
  /* @End: UT_CORE_1515 */
  ASSERT_STREQ("你好", BufferContext.CurrentLiteral());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1516
@Test_Description:    To test the functionality of current literal.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611104, 611113
*******************************************************************************/
TEST(minijson_reader_detail, parse_string_escape_sequences)
{
  char buffer[] = "\\\"\\\\\\/\\b\\f\\n\\r\\t\"";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));
  ara::core::configreader::detail::ParseString(BufferContext);
  /* @Start: UT_CORE_1516 */
  /* @TS1: Invoke the CurrentLiteral() with BufferContext class .*/
  /* @TE1: CurrentLiteral() should equal to "\"\\/\b\f\n\r\t". */
  /* @End: UT_CORE_1516 */
  ASSERT_STREQ("\"\\/\b\f\n\r\t", BufferContext.CurrentLiteral());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1517
@Test_Description:    To test the functionality of buffer context.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611138, 611137, 611116, 611114
*******************************************************************************/
TEST(minijson_reader_detail, parse_string_escape_sequences_utf16)
{
  char buffer[]
      = "\\u0001\\u0002a\\ud7ff\\uE000\\uFffFb\\u4F60\\uD800"
        "\\uDC00\\uDBFF\\uDFFFà\"";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));
  ara::core::configreader::detail::ParseString(BufferContext);
  /* @Start: UT_CORE_1517 */
  /* @TS1: Invoke the CurrentLiteral() with BufferContext class . */
  /* @TE1: CurrentLiteral() should equal to "\\u0001\\u0002a\\ud7ff\\uE000\\uFffFb\\u4F60\\uD800" */
  /*                                          "\\uDC00\\uDBFF\\uDFFFà\"". */
  /* @End: UT_CORE_1517 */
  ASSERT_STREQ(
      "\x01\x02"
      "a"
      "\xED\x9F\xBF\xEE\x80\x80\xEF\xBF\xBF"
      "b"
      "你"
      "\xF0\x90\x80\x80"
      "\xF4\x8F\xBF\xBF"
      "à",
      BufferContext.CurrentLiteral());
}

template <std::size_t Length>
void ParseStringInvalidHelper(const char (&buffer)[Length],
    const ara::core::configreader::ParseError::ErrorReason expectedReason, const std::size_t expectedOffset,
    const char* const expectedWhat)
{
  SCOPED_TRACE(buffer);
  bool isExceptionThrown = false;

  try
  {
    ara::core::configreader::ConstBufferContext context(buffer, Length - 1);
    ara::core::configreader::detail::ParseString(context);
  }
  catch (const ara::core::configreader::ParseError& ParseError)
  {
    isExceptionThrown = true;
    ASSERT_EQ(expectedReason, ParseError.Reason());
    ASSERT_EQ(expectedOffset, ParseError.Offset());
    ASSERT_STREQ(expectedWhat, ParseError.what());
  }

  ASSERT_TRUE(isExceptionThrown);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1518
@Test_Description:    To test the functionality of parsing string invalid.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611163, 611164, 611134
*******************************************************************************/
TEST(minijson_reader_detail, parse_string_invalid)
{
  /* @Start: UT_CORE_1518 */
  /* @TS1: Invoke the ParseStringInvalidHelper .*/
  /* @TE1: ParseStringInvalidHelper return the invalid values. */
  /* @End: UT_CORE_1518 */
  ParseStringInvalidHelper(
      "", ara::core::configreader::ParseError::kUnterminatedValue, 0, "Unterminated value");

  ParseStringInvalidHelper(
      "asd", ara::core::configreader::ParseError::kUnterminatedValue, 2, "Unterminated value");

  ParseStringInvalidHelper(
      "\\h\"", ara::core::configreader::ParseError::kInvalidEscapeSequence, 1, "Invalid escape sequence");

  ParseStringInvalidHelper(
      "\\u0rff\"", ara::core::configreader::ParseError::kInvalidEscapeSequence, 3, "Invalid escape sequence");

  ParseStringInvalidHelper("foo\nfoo\"", ara::core::configreader::ParseError::kUnescapedControlCharacter, 3,
      "Unescaped control character");

  ParseStringInvalidHelper(
      "foo\x01"
      "foo\"",
      ara::core::configreader::ParseError::kUnescapedControlCharacter, 3, "Unescaped control character");

  ParseStringInvalidHelper(
      "foo\x1F"
      "foo\"",
      ara::core::configreader::ParseError::kUnescapedControlCharacter, 3, "Unescaped control character");

  ParseStringInvalidHelper("\\uD800\\uD7FF\"", ara::core::configreader::ParseError::kInvalidUtf16Character,
      11, "Invalid UTF-16 character");

  ParseStringInvalidHelper("\\uD800\\u0000\"", ara::core::configreader::ParseError::kInvalidUtf16Character,
      11, "Invalid UTF-16 character");

  ParseStringInvalidHelper("\\uDC00\"", ara::core::configreader::ParseError::kInvalidUtf16Character, 5,
      "Invalid UTF-16 character");

  ParseStringInvalidHelper(
      "\\u0000\"", ara::core::configreader::ParseError::kNullUtf16Character, 5, "kNull UTF-16 character");

  ParseStringInvalidHelper("\\uD800\"", ara::core::configreader::ParseError::kExpectedUtf16LowSurrogate, 6,
      "Expected UTF-16 low surrogate");

  ParseStringInvalidHelper("\\uD800a\"", ara::core::configreader::ParseError::kExpectedUtf16LowSurrogate, 6,
      "Expected UTF-16 low surrogate");
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1519
@Test_Description:    To test the functionality of value default constructed.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611096, 611095, 611086, 611085, 611082, 611081, 611055
*******************************************************************************/
TEST(minijson_reader, value_default_constructed)
{
  const ara::core::configreader::value value;
  /* @Start: UT_CORE_1519 */
  /* @TS1: Invoke the Type() with value class.*/
  /* @TE1: Type() should equal to ara::core::configreader::kNull. */
  /* @End: UT_CORE_1519 */
  ASSERT_EQ(ara::core::configreader::kNull, value.Type());
  /* @Start: UT_CORE_1519 */
  /* @TS2: Invoke the Raw() with value class.*/
  /* @TE2: Raw() should equal to "null". */
  /* @End: UT_CORE_1519 */
  ASSERT_EQ("null", value.Raw());

  /* @Start: UT_CORE_1519 */
  /* @TS3: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE3: As<ara::core::StringView>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1519 */
  ASSERT_THROW(value.As<ara::core::StringView>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1519 */
  /* @TS4: Invoke the As<long>() with value class.*/
  /* @TE4: As<long>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1519 */
  ASSERT_THROW(value.As<long>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1519 */
  /* @TS5: Invoke the As<bool>() with value class.*/
  /* @TE5: As<bool>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1519 */
  ASSERT_THROW(value.As<bool>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1519 */
  /* @TS6: Invoke the As<double>() with value class.*/
  /* @TE6: As<double>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1519 */
  ASSERT_THROW(value.As<double>(), ara::core::configreader::BadValueCast);

  // ASSERT_EQ(std::nullopt, value.As<ara::core::Optional<ara::core::StringView>>());
  // ASSERT_EQ(std::nullopt, value.As<ara::core::Optional<long>>());
  // ASSERT_EQ(std::nullopt, value.As<ara::core::Optional<bool>>());
  // ASSERT_EQ(std::nullopt, value.As<ara::core::Optional<double>>());
}

template <typename Context>
void ParseUnquotedValueBadHelper(Context& context, const std::size_t expectedOffset,
    const ara::core::configreader::ParseError::ErrorReason reason)
{
  bool isExceptionThrown = false;

  try
  {
    ara::core::configreader::detail::ParseUnquotedValue(context, context.Read());
  }
  catch (const ara::core::configreader::ParseError& ParseError)
  {
    isExceptionThrown = true;

    ASSERT_EQ(reason, ParseError.Reason());
    ASSERT_EQ(expectedOffset, ParseError.Offset());
  }
  ASSERT_TRUE(isExceptionThrown);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1521
@Test_Description:    To test the functionality of parsing unquoted_value with whitespace.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611164
*******************************************************************************/
TEST(minijson_reader_detail, parse_unquoted_value_whitespace)
{
  char buffer[] = "  42";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));
  ParseUnquotedValueBadHelper(BufferContext, 0, ara::core::configreader::ParseError::kExpectedValue);
  /* @Start: UT_CORE_1521 */
  /* @TS1: Invoke the ParseUnquotedValueBadHelper .*/
  /* @TE1: ParseUnquotedValueBadHelper return the expected values. */
  /* @End: UT_CORE_1521 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1522
@Test_Description:    To test the functionality of parsing unquoted_value with ture.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611164, 611142
*******************************************************************************/
TEST(minijson_reader_detail, parse_unquoted_value_true)
{
  char buffer[] = "true  ";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  const auto [value, terminationChar]
      = ara::core::configreader::detail::ParseUnquotedValue(BufferContext, BufferContext.Read());
  /* @Start: UT_CORE_1522 */
  /* @TS1: Get value of terminationChar.*/
  /* @TE1: terminationChar should equal to  ' '. */
  /* @End: UT_CORE_1522 */
  ASSERT_EQ(terminationChar, ' ');
  /* @Start: UT_CORE_1522 */
  /* @TS2: Invoke the Type() with value class.*/
  /* @TE2: Type() should equal to  kBoolean. */
  /* @End: UT_CORE_1522 */
  ASSERT_EQ(ara::core::configreader::kBoolean, value.Type());
  /* @Start: UT_CORE_1522 */
  /* @TS3: Invoke the Raw() with value class.*/
  /* @TE3: Raw() should equal to  "true". */
  /* @End: UT_CORE_1522 */
  ASSERT_EQ("true", value.Raw());
  /* @Start: UT_CORE_1522 */
  /* @TS4: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE4: As<ara::core::StringView>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1522 */
  ASSERT_THROW(value.As<ara::core::StringView>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1522 */
  /* @TS5: Invoke the As<long>() with value class.*/
  /* @TE5: As<long>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1522 */
  ASSERT_THROW(value.As<long>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1522 */
  /* @TS6: Invoke the As<bool>() with value class. */
  /* @TE6: As<bool>() should return true. */
  /* @End: UT_CORE_1522 */
  ASSERT_TRUE(value.As<bool>());
  /* @Start: UT_CORE_1522 */
  /* @TS7: Invoke the As<double>() with value class.*/
  /* @TE7: As<double>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1522 */
  ASSERT_THROW(value.As<double>(), ara::core::configreader::BadValueCast);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1523
@Test_Description:    To test the functionality of parsing unquoted_value with flase.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611140, 611139, 611084, 611083
*******************************************************************************/
TEST(minijson_reader_detail, parse_unquoted_value_false)
{
  char buffer[] = "false}";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  const auto [value, terminationChar]
      = ara::core::configreader::detail::ParseUnquotedValue(BufferContext, BufferContext.Read());
  /* @Start: UT_CORE_1523 */
  /* @TS1: Get value of terminationChar.*/
  /* @TE1: terminationChar should equal to  ' '. */
  /* @End: UT_CORE_1523 */
  ASSERT_EQ(terminationChar, '}');
  /* @Start: UT_CORE_1523 */
  /* @TS2: Invoke the Type() with value class.*/
  /* @TE2: Type() should equal to  kBoolean. */
  /* @End: UT_CORE_1523 */
  ASSERT_EQ(ara::core::configreader::kBoolean, value.Type());
  /* @Start: UT_CORE_1523 */
  /* @TS3: Invoke the Raw() with value class.*/
  /* @TE3: Raw() should equal to  "false". */
  /* @End: UT_CORE_1523 */
  ASSERT_EQ("false", value.Raw());
  /* @Start: UT_CORE_1523 */
  /* @TS4: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE4: As<ara::core::StringView>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1523 */
  ASSERT_THROW(value.As<ara::core::StringView>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1523 */
  /* @TS5: Invoke the As<long>() with value class*/
  /* @TE5: As<long>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1523 */
  ASSERT_THROW(value.As<long>(), ara::core::configreader::BadValueCast);

  /* @Start: UT_CORE_1523 */
  /* @TS6: Invoke the As<bool>() with value class. */
  /* @TE6: As<bool>() should return flase. */
  /* @End: UT_CORE_1523 */
  ASSERT_FALSE(value.As<bool>());
  /* @Start: UT_CORE_1523 */
  /* @TS7: Invoke the As<double>() with value class.*/
  /* @TE7: As<double>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1523 */
  ASSERT_THROW(value.As<double>(), ara::core::configreader::BadValueCast);

  // ASSERT_THROW(
  //     value.As<ara::core::Optional<ara::core::StringView>>(),
  //     ara::core::configreader::BadValueCast);
  //  ASSERT_THROW(value.As<ara::core::Optional<long>>(), ara::core::configreader::BadValueCast);
  // ASSERT_EQ(false, value.As<ara::core::Optional<bool>>());
  // ASSERT_THROW(value.As<ara::core::Optional<double>>(), ara::core::configreader::BadValueCast);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1524
@Test_Description:    To test the functionality of parsing unquoted_value with null.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611162
*******************************************************************************/
TEST(minijson_reader_detail, parse_unquoted_value_null)
{
  char buffer[] = "null}";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  const auto [value, terminationChar]
      = ara::core::configreader::detail::ParseUnquotedValue(BufferContext, BufferContext.Read());
  /* @Start: UT_CORE_1524 */
  /* @TS1: Get value of terminationChar.*/
  /* @TE1: terminationChar should equal to  ' }'. */
  /* @End: UT_CORE_1524 */
  ASSERT_EQ(terminationChar, '}');
  /* @Start: UT_CORE_1524 */
  /* @TS2: Invoke the Type() with value class.*/
  /* @TE2: Type() should equal to  null. */
  /* @End: UT_CORE_1524 */
  ASSERT_EQ(ara::core::configreader::kNull, value.Type());
  /* @Start: UT_CORE_1524 */
  /* @TS3: Invoke the Raw() with value class.*/
  /* @TE3: Raw() should equal to  "null". */
  /* @End: UT_CORE_1524 */
  ASSERT_EQ("null", value.Raw());

  /* @Start: UT_CORE_1524 */
  /* @TS4: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE4: As<ara::core::StringView>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1524 */
  ASSERT_THROW(value.As<ara::core::StringView>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1524 */
  /* @TS5: Invoke the As<long>() with value class.*/
  /* @TE5: As<long>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1524 */
  ASSERT_THROW(value.As<long>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1524 */
  /* @TS6: Invoke the As<bool>() with value class.*/
  /* @TE6: As<bool>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1524 */
  ASSERT_THROW(value.As<bool>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1524 */
  /* @TS7: Invoke the As<double>() with value class.*/
  /* @TE7: As<double>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1524 */
  ASSERT_THROW(value.As<double>(), ara::core::configreader::BadValueCast);

  //ASSERT_EQ(value.As<ara::core::Optional<ara::core::StringView>>(), std::nullopt);
  // ASSERT_EQ(value.As<ara::core::Optional<long>>(), std::nullopt);
  // ASSERT_EQ(value.As<ara::core::Optional<bool>>(), std::nullopt);
  // ASSERT_EQ(value.As<ara::core::Optional<double>>(), std::nullopt);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1525
@Test_Description:    To test the functionality of parsing unquoted_value_integer with too large.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611162
*******************************************************************************/
TEST(minijson_reader_detail, parse_unquoted_value_integer_too_large)
{
  // Smallest positive value that does not fit in an int64_t.
  // Of course it "fits" in a double but cannot be represented accurately.

  char buffer[] = "9223372036854775808,";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  const auto [value, terminationChar]
      = ara::core::configreader::detail::ParseUnquotedValue(BufferContext, BufferContext.Read());
  /* @Start: UT_CORE_1525 */
  /* @TS1: Get value of terminationChar.*/
  /* @TE1: terminationChar should equal to  ' ,'. */
  /* @End: UT_CORE_1525 */
  ASSERT_EQ(terminationChar, ',');
  /* @Start: UT_CORE_1525 */
  /* @TS2: Invoke the Type() with value class.*/
  /* @TE2: Type() should equal to  kNumber. */
  /* @End: UT_CORE_1525 */
  ASSERT_EQ(ara::core::configreader::kNumber, value.Type());
  /* @Start: UT_CORE_1525 */
  /* @TS3: Invoke the Raw() with value class.*/
  /* @TE3: Raw() should equal to  "9223372036854775808". */
  /* @End: UT_CORE_1525 */
  ASSERT_EQ("9223372036854775808", value.Raw());
  /* @Start: UT_CORE_1525 */
  /* @TS4: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE4: As<ara::core::StringView>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1525 */
  ASSERT_THROW(value.As<ara::core::StringView>(), ara::core::configreader::BadValueCast);
  //ASSERT_THROW(value.As<int64_t>(), std::range_error);
  /* @Start: UT_CORE_1525 */
  /* @TS5: Invoke the As<bool>() with value class.*/
  /* @TE5: As<bool>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1525 */
  ASSERT_THROW(value.As<bool>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1525 */
  /* @TS6: Invoke the As<double>() with value class.*/
  /* @TE6: As<double>() should equal to  "9223372036854775808.0". */
  /* @End: UT_CORE_1525 */
  ASSERT_DOUBLE_EQ(9223372036854775808.0, value.As<double>());
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1526
@Test_Description:    To test the functionality ara::core::configreader parser.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611143, 611130, 611078
*******************************************************************************/
TEST(minijson_parser, minijson_parser)
{
  SUCCEED();
  /* @Start: UT_CORE_1526 */
  /* @TS1: Invoke the SUCCEED() .*/
  /* @TE1: SUCCEED() return the minijson_parser. */
  /* @End: UT_CORE_1526 */
} // ASSERT_THROW(
//     value.As<ara::core::Optional<ara::core::StringView>>(),
//     ara::core::configreader::BadValueCast);
// ASSERT_THROW(value.As<ara::core::Optional<long>>(), ara::core::configreader::BadValueCast);
// ASSERT_EQ(true, value.As<ara::core::Optional<bool>>());
// ASSERT_THROW(value.As<ara::core::Optional<double>>(), ara::core::configreader::BadValueCast);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t N>
void ParseNumberExhaustiveHelper(const char (&buffer)[N], const bool isValidDouble = true)
{
  SCOPED_TRACE(buffer);
  ara::core::configreader::ConstBufferContext context(buffer, N);
  const auto [value, terminationChar]
      = ara::core::configreader::detail::ParseUnquotedValue(context, context.Read());
  ASSERT_EQ(value.Raw(), ara::core::StringView(buffer, N - 2));
  if (isValidDouble)
  {
    ASSERT_NO_THROW(value.As<double>());
  }
  ASSERT_EQ(terminationChar, buffer[N - 2]);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1527
@Test_Description:    To test the functionality of parse number exhaustive helper method.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611162
*******************************************************************************/
TEST(minijson_reader_detail, parse_number_exhaustive)
{
  /* @Start: UT_CORE_1527 */
  /* @TS1: Invoke the ParseNumberExhaustiveHelper.*/
  /* @TE1: ParseNumberExhaustiveHelper() return the exhaustive numbers. */
  /* @End: UT_CORE_1527 */
  ParseNumberExhaustiveHelper("0,");
  ParseNumberExhaustiveHelper("-0,");
  ParseNumberExhaustiveHelper("1}");
  ParseNumberExhaustiveHelper("-1}");
  ParseNumberExhaustiveHelper("42]");
  ParseNumberExhaustiveHelper("-42 ");
  ParseNumberExhaustiveHelper("-1234567890 ");
  ParseNumberExhaustiveHelper("0.0,");
  ParseNumberExhaustiveHelper("-0.0,");
  ParseNumberExhaustiveHelper("4.2,");
  ParseNumberExhaustiveHelper("-4.2}");
  ParseNumberExhaustiveHelper("42.0]");
  ParseNumberExhaustiveHelper("-423.423 ");
  ParseNumberExhaustiveHelper("0.1,");
  ParseNumberExhaustiveHelper("-0.123}");
  ParseNumberExhaustiveHelper("0.123e75]");
  ParseNumberExhaustiveHelper("0.123E75]");
  ParseNumberExhaustiveHelper("-0e5}");
  ParseNumberExhaustiveHelper("0e+5}");
  ParseNumberExhaustiveHelper("0e-5}");
  ParseNumberExhaustiveHelper("12e5}");
  ParseNumberExhaustiveHelper("-1.123E+7 ");
  ParseNumberExhaustiveHelper("-1.123e+0 ");
  ParseNumberExhaustiveHelper("-1.123E-0 ");
  ParseNumberExhaustiveHelper("12.123e-7,");
  ParseNumberExhaustiveHelper("0.123e1234567890}", false);
  ParseNumberExhaustiveHelper("-1.123E+756]", false);
  ParseNumberExhaustiveHelper("1234567890.1234567890e-75 ");
  ParseNumberExhaustiveHelper("1234567890.1234567890e-756 ", false);
}
template <std::size_t N>
void ParseNumberExhaustiveFailHelper(const char (&buffer)[N])
{
  SCOPED_TRACE(buffer);
  ara::core::configreader::ConstBufferContext context(buffer, N);
  std::cout << __LINE__ << " " << __FUNCTION__ << std::endl;
  try
  {
    std::cout << __LINE__ << " " << __FUNCTION__ << std::endl;
    ara::core::configreader::detail::ParseUnquotedValue(context, context.Read());
    FAIL() << "Exception not thrown";
  }
  catch (const ara::core::configreader::ParseError& ex)
  {
    std::cout << __LINE__ << " " << __FUNCTION__ << std::endl;
    ASSERT_EQ(ara::core::configreader::ParseError::kInvalidValue, ex.Reason());
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1528
@Test_Description:    To test the functionality of parsing number_exhaustive_fail.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611070
*******************************************************************************/
TEST(minijson_reader_detail, parse_number_exhaustive_fail)
{
  /* @Start: UT_CORE_1528 */
  /* @TS1: Invoke the ParseNumberExhaustiveFailHelper() .*/
  /* @TE1: ParseNumberExhaustiveFailHelper() return the fail exhaustive numbers. */
  /* @End: UT_CORE_1528 */
  ParseNumberExhaustiveFailHelper("x,");
  ParseNumberExhaustiveFailHelper(".5,");
  ParseNumberExhaustiveFailHelper("e7,");
  ParseNumberExhaustiveFailHelper("01,");
  ParseNumberExhaustiveFailHelper("0x,");
  ParseNumberExhaustiveFailHelper("0.,");
  ParseNumberExhaustiveFailHelper("0e,");
  ParseNumberExhaustiveFailHelper("0e-,");
  ParseNumberExhaustiveFailHelper("0e+,");
  ParseNumberExhaustiveFailHelper("-01,");
  ParseNumberExhaustiveFailHelper("-0x,");
  ParseNumberExhaustiveFailHelper("-0.,");
  ParseNumberExhaustiveFailHelper("-0e,");
  ParseNumberExhaustiveFailHelper("-0e-,");
  ParseNumberExhaustiveFailHelper("-0e+,");
  ParseNumberExhaustiveFailHelper("+1,");
  ParseNumberExhaustiveFailHelper("1x,");
  ParseNumberExhaustiveFailHelper("1.,");
  ParseNumberExhaustiveFailHelper("1e,");
  ParseNumberExhaustiveFailHelper("1e-,");
  ParseNumberExhaustiveFailHelper("1e+,");
  ParseNumberExhaustiveFailHelper("1.e7,");
  ParseNumberExhaustiveFailHelper("1.e-5,");
  ParseNumberExhaustiveFailHelper("1.e+5,");
  ParseNumberExhaustiveFailHelper("0.0E,");
  ParseNumberExhaustiveFailHelper("0.0e-,");
  ParseNumberExhaustiveFailHelper("0.0E+,");
  ParseNumberExhaustiveFailHelper("0.0+,");
  ParseNumberExhaustiveFailHelper("1.0E,");
  ParseNumberExhaustiveFailHelper("1.0e-,");
  ParseNumberExhaustiveFailHelper("1.0E+,");
  ParseNumberExhaustiveFailHelper("1.0+,");
  ParseNumberExhaustiveFailHelper("-1.0E,");
  ParseNumberExhaustiveFailHelper("-1.0e-,");
  ParseNumberExhaustiveFailHelper("-1.0E+,");
  ParseNumberExhaustiveFailHelper("-1.0+,");
  ParseNumberExhaustiveFailHelper("1.+,");
  ParseNumberExhaustiveFailHelper("1+,");
  ParseNumberExhaustiveFailHelper("1..0e+5,");
  ParseNumberExhaustiveFailHelper("1.0ee+5,");
  ParseNumberExhaustiveFailHelper("1.0e++5,");
  ParseNumberExhaustiveFailHelper("1.0.5,");
  ParseNumberExhaustiveFailHelper("1234567890.1234567890e-7.5 ");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1529
@Test_Description:    To test the functionality of parsing invalid unquoted_value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611162
*******************************************************************************/
TEST(minijson_reader_detail, parse_unquoted_value_invalid)
{
  char buffer[] = "  asd,";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));
  BufferContext.Read();
  BufferContext.Read();

  ParseUnquotedValueBadHelper(BufferContext, 2, ara::core::configreader::ParseError::kInvalidValue);
  /* @Start: UT_CORE_1529 */
  /* @TS1: Invoke the ParseUnquotedValueBadHelper() .*/
  /* @TE1: ParseUnquotedValueBadHelper() return the invalid values. */
  /* @End: UT_CORE_1529 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1530
@Test_Description:    To test the functionality of parsing object value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611169, 611168, 611167, 611166, 611154, 611164, 611163,
                      611147, 611097
*******************************************************************************/
TEST(minijson_reader_detail, parse_value_object)
{
  char buffer[] = "{...";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));

  const char firstChar = BufferContext.Read();

  char c = firstChar;
  bool isMustRead = true;
  const auto value = ara::core::configreader::detail::ParseValue(BufferContext, c, isMustRead);
  /* @Start: UT_CORE_1530 */
  /* @TS1: Create value of firstChar.*/
  /* @TE1: firstChar should equal to c. */
  /* @End: UT_CORE_1530 */
  ASSERT_EQ(c, firstChar);
  /* @Start: UT_CORE_1530 */
  /* @TS2: Create the value of isMustRead */
  /* @TE2: isMustRead should return true. */
  /* @End: UT_CORE_1530 */
  ASSERT_TRUE(isMustRead);
  /* @Start: UT_CORE_1530 */
  /* @TS3: Invoke the Type() with value class.*/
  /* @TE3: Type() should equal to  kObject. */
  /* @End: UT_CORE_1530 */
  ASSERT_EQ(ara::core::configreader::kObject, value.Type());
  /* @Start: UT_CORE_1530 */
  /* @TS4: Invoke the Raw() with value class.*/
  /* @TE4: Raw() should equal to  "". */
  /* @End: UT_CORE_1530 */
  ASSERT_EQ("", value.Raw());

  /* @Start: UT_CORE_1530 */
  /* @TS5: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE5: As<ara::core::StringView>() throw BadValueCast. */
  /* @End: UT_CORE_1530 */
  ASSERT_THROW(value.As<ara::core::StringView>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1530 */
  /* @TS6: Invoke the As<long>() with value class.*/
  /* @TE6: As<long>() should throw BadValueCast. */
  /* @End: UT_CORE_1530 */
  ASSERT_THROW(value.As<long>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1530 */
  /* @TS7: Invoke the As<bool>() with value class.*/
  /* @TE7: As<bool>() should throw BadValueCast. */
  /* @End: UT_CORE_1530 */
  ASSERT_THROW(value.As<bool>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1530 */
  /* @TS8: Invoke the As<double>() with value class.*/
  /* @TE8: As<double>() should throw BadValueCast. */
  /* @End: UT_CORE_1530 */
  ASSERT_THROW(value.As<double>(), ara::core::configreader::BadValueCast);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1531
@Test_Description:    To test the functionality of parsing value array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611163, 611159, 611158, 611156, 611155
*******************************************************************************/
TEST(minijson_reader_detail, parse_value_array)
{
  char buffer[] = "[...";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer));

  const char firstChar = BufferContext.Read();

  char c = firstChar;
  bool isMustRead = true;
  const auto value = ara::core::configreader::detail::ParseValue(BufferContext, c, isMustRead);
  /* @Start: UT_CORE_1531 */
  /* @TS1: Create value of firstChar.*/
  /* @TE1: firstChar should equal to c. */
  /* @End: UT_CORE_1531 */
  ASSERT_EQ(c, firstChar);
  /* @Start: UT_CORE_1531 */
  /* @TS2: Create the value of isMustRead */
  /* @TE2: isMustRead should return true. */
  /* @End: UT_CORE_1531 */
  ASSERT_TRUE(isMustRead);
  /* @Start: UT_CORE_1531 */
  /* @TS3: Invoke the Type() with value class.*/
  /* @TE3: Type() should equal to kArray. */
  /* @End: UT_CORE_1531 */
  ASSERT_EQ(ara::core::configreader::kArray, value.Type());
  /* @Start: UT_CORE_1531 */
  /* @TS4: Invoke the Raw() with value class.*/
  /* @TE4: Raw() should equal to "". */
  /* @End: UT_CORE_1531 */
  ASSERT_EQ("", value.Raw());

  /* @Start: UT_CORE_1531 */
  /* @TS5: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE5: As<ara::core::StringView>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1531 */
  ASSERT_THROW(value.As<ara::core::StringView>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1531 */
  /* @TS6: Invoke the As<long>() with value class.*/
  /* @TE6: As<long>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1531 */
  ASSERT_THROW(value.As<long>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1531 */
  /* @TS7: Invoke the As<bool>() with value class.*/
  /* @TE7: As<bool>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1531 */
  ASSERT_THROW(value.As<bool>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1531 */
  /* @TS8: Invoke the As<double>() with value class.*/
  /* @TE8: As<double>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1531 */
  ASSERT_THROW(value.As<double>(), ara::core::configreader::BadValueCast);

  // ASSERT_THROW(
  //     value.As<ara::core::Optional<ara::core::StringView>>(),
  //     ara::core::configreader::BadValueCast);
  // ASSERT_THROW(
  //     value.As<ara::core::Optional<long>>(),
  //     ara::core::configreader::BadValueCast);
  // ASSERT_THROW(
  //     value.As<ara::core::Optional<bool>>(),
  //     ara::core::configreader::BadValueCast);
  // ASSERT_THROW(
  //     value.As<ara::core::Optional<double>>(),
  //     ara::core::configreader::BadValueCast);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1532
@Test_Description:    To test the functionality of parsing quoted_string value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611164, 611144, 611109, 611100, 611090
*******************************************************************************/
TEST(minijson_reader_detail, parse_value_quoted_string)
{
  char buffer[] = "\"Hello world\"";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  const char firstChar = BufferContext.Read();

  char c = firstChar;
  bool isMustRead = true;
  const auto value = ara::core::configreader::detail::ParseValue(BufferContext, c, isMustRead);
  /* @Start: UT_CORE_1532 */
  /* @TS1: Create value of firstChar.*/
  /* @TE1: firstChar should equal to c. */
  /* @End: UT_CORE_1532 */
  ASSERT_EQ(c, firstChar);
  /* @Start: UT_CORE_1532 */
  /* @TS2: Create the value of isMustRead */
  /* @TE2: isMustRead should return true. */
  /* @End: UT_CORE_1532 */
  ASSERT_TRUE(isMustRead);
  /* @Start: UT_CORE_1532 */
  /* @TS3: Invoke the Type() with value class.*/
  /* @TE3: Type() should equal to  kArray. */
  /* @End: UT_CORE_1532 */
  ASSERT_EQ(ara::core::configreader::kString, value.Type());
  /* @Start: UT_CORE_1532 */
  /* @TS4: Invoke the Type() with value class.*/
  /* @TE4: Raw() should equal to  "". */
  /* @End: UT_CORE_1532 */
  ASSERT_EQ("Hello world", value.Raw());
  /* @Start: UT_CORE_1532 */
  /* @TS5: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE5: As<ara::core::StringView>() should equal to  "Hello world". */
  /* @End: UT_CORE_1532 */
  ASSERT_EQ("Hello world", value.As<ara::core::StringView>());

  /* @Start: UT_CORE_1531 */
  /* @TS6: Invoke the As<long>() with value class.*/
  /* @TE6: As<long>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1531 */
  ASSERT_THROW(value.As<long>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1531 */
  /* @TS7: Invoke the As<bool>() with value class.*/
  /* @TE7: As<bool>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1531 */
  ASSERT_THROW(value.As<bool>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1531 */
  /* @TS8: Invoke the As<double>() with value class.*/
  /* @TE8: As<double>() is not valid, it throwing BadValueCast. */
  /* @End: UT_CORE_1531 */
  ASSERT_THROW(value.As<double>(), ara::core::configreader::BadValueCast);

  // ASSERT_EQ("Hello world", value.As<std::optional<ara::core::StringView>>());
  // ASSERT_THROW(value.As<std::optional<long>>(), ara::core::configreader::BadValueCast);
  // ASSERT_THROW(value.As<std::optional<bool>>(), ara::core::configreader::BadValueCast);
  // ASSERT_THROW(value.As<std::optional<double>>(), ara::core::configreader::BadValueCast);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1533
@Test_Description:    To test the functionality of parsing quoted_string empty value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611164, 611109, 611100, 611090
*******************************************************************************/
TEST(minijson_reader_detail, parse_value_quoted_string_empty)
{
  char buffer[] = "\"\"";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  const char firstChar = BufferContext.Read();

  char c = firstChar;
  bool isMustRead = true;
  const auto value = ara::core::configreader::detail::ParseValue(BufferContext, c, isMustRead);
  /* @Start: UT_CORE_1533 */
  /* @TS1: Create value of firstChar.*/
  /* @TE1: firstChar should equal to c. */
  /* @End: UT_CORE_1533 */
  ASSERT_EQ(c, firstChar);
  /* @Start: UT_CORE_1533 */
  /* @TS2: Create the value of isMustRead */
  /* @TE2: isMustRead should return true. */
  /* @End: UT_CORE_1533 */
  ASSERT_TRUE(isMustRead);
  /* @Start: UT_CORE_1533 */
  /* @TS3: Invoke the Type() with value class.*/
  /* @TE3: Type() should equal to  kString. */
  /* @End: UT_CORE_1533 */
  ASSERT_EQ(ara::core::configreader::kString, value.Type());
  /* @Start: UT_CORE_1533 */
  /* @TS4: Invoke the Raw() with value class.*/
  /* @TE4: Raw() should equal to  "". */
  /* @End: UT_CORE_1533 */
  ASSERT_EQ("", value.Raw());
  /* @Start: UT_CORE_1533 */
  /* @TS5: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE5: As<ara::core::StringView>() should equal to  "". */
  /* @End: UT_CORE_1533 */
  ASSERT_EQ("", value.As<ara::core::StringView>());

  /* @Start: UT_CORE_1533 */
  /* @TS6: Invoke the As<long>() with value class.*/
  /* @TE6: As<long>()should throw BadValueCast. */
  /* @End: UT_CORE_1533 */
  ASSERT_THROW(value.As<long>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1533 */
  /* @TS7: Invoke the As<bool>() with value class.*/
  /* @TE7: As<bool>() should throw BadValueCast. */
  /* @End: UT_CORE_1533 */
  ASSERT_THROW(value.As<bool>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1533 */
  /* @TS8: Invoke the As<double>() with value class.*/
  /* @TE8: As<double>() should throw BadValueCast. */
  /* @End: UT_CORE_1533 */
  ASSERT_THROW(value.As<double>(), ara::core::configreader::BadValueCast);

  // ASSERT_EQ("", value.As<std::optional<std::string_view>>());
  // ASSERT_THROW(value.As<std::optional<long>>(), ara::core::configreader::BadValueCast);
  // ASSERT_THROW(value.As<std::optional<bool>>(), ara::core::configreader::BadValueCast);
  // ASSERT_THROW(value.As<std::optional<double>>(), ara::core::configreader::BadValueCast);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1534
@Test_Description:    To test the functionality of parsing unquoted value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611164, 611109, 611100, 611090
*******************************************************************************/
TEST(minijson_reader_detail, parse_value_unquoted)
{
  char buffer[] = "true\n";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  const char firstChar = BufferContext.Read();

  char c = firstChar;
  bool isMustRead = true;
  const auto value = ara::core::configreader::detail::ParseValue(BufferContext, c, isMustRead);
  /* @Start: UT_CORE_1534 */
  /* @TS1: Create value of c.*/
  /* @TE1: c should equal to '\n'. */
  /* @End: UT_CORE_1534 */
  ASSERT_EQ(c, '\n');
  /* @Start: UT_CORE_1534 */
  /* @TS2: Create the value of isMustRead */
  /* @TE2: isMustRead should return flase. */
  /* @End: UT_CORE_1534 */
  ASSERT_FALSE(isMustRead);
  /* @Start: UT_CORE_1534 */
  /* @TS3: Invoke the Type() with value class.*/
  /* @TE3: Type() should equal to kBoolean. */
  /* @End: UT_CORE_1534 */
  ASSERT_EQ(ara::core::configreader::kBoolean, value.Type());
  /* @Start: UT_CORE_1534 */
  /* @TS4: Invoke the Raw() with value class.*/
  /* @TE4: Raw() should equal to "true". */
  /* @End: UT_CORE_1534 */
  ASSERT_EQ("true", value.Raw());
  /* @Start: UT_CORE_1534 */
  /* @TS5: Invoke the As<ara::core::StringView>() with value class.*/
  /* @TE5: As<ara::core::StringView>() should equal to  "". */
  /* @End: UT_CORE_1534 */
  ASSERT_THROW(value.As<ara::core::StringView>(), ara::core::configreader::BadValueCast);

  /* @Start: UT_CORE_1534 */
  /* @TS6: Invoke the As<long>() with value class.*/
  /* @TE6: As<long>() should throw BadValueCast. */
  /* @End: UT_CORE_1534 */
  ASSERT_THROW(value.As<long>(), ara::core::configreader::BadValueCast);
  /* @Start: UT_CORE_1534 */
  /* @TS7: Invoke the As<bool>() with value class. */
  /* @TE7: As<bool>() should return true. */
  /* @End: UT_CORE_1534 */
  ASSERT_TRUE(value.As<bool>());
  /* @Start: UT_CORE_1534 */
  /* @TS8: Invoke the As<double>() with value class.*/
  /* @TE8: As<double>() should throw BadValueCast. */
  /* @End: UT_CORE_1534 */
  ASSERT_THROW(value.As<double>(), ara::core::configreader::BadValueCast);

  // ASSERT_THROW(
  //     value.As<std::optional<ara::core::StringView>>(),
  //     ara::core::configreader::BadValueCast);
  //ASSERT_THROW(value.As<std::optional<long>>(), ara::core::configreader::BadValueCast);
  //ASSERT_EQ(true, value.As<std::optional<bool>>());
  //ASSERT_THROW(value.As<std::optional<double>>(), ara::core::configreader::BadValueCast);

  // boolean false, null, integer and double cases have been already tested
  // with parse_unquoted_value
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1535
@Test_Description:    To test the functionality of parsing invalid unquoted value.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader_detail
@Requirement Tracing: -
@Design Tracing:      611127, 611125, 611124, 611123, 611122, 611121, 611115,
                      611109, 611090, 611063
*******************************************************************************/
TEST(minijson_reader_detail, parse_value_unquoted_invalid)
{
  char buffer[] = ":xxx,";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);
  BufferContext.Read(); // discard initial :

  bool isExceptionThrown = false;

  try
  {
    char c = BufferContext.Read();
    bool isMustRead = false;
    ara::core::configreader::detail::ParseValue(BufferContext, c, isMustRead);
  }
  catch (const ara::core::configreader::ParseError& ParseError)
  {
    isExceptionThrown = true;
    /* @Start: UT_CORE_1535 */
    /* @TS1: Invoke the Reason() with ParseError class.*/
    /* @TE1: Reason() should equal to kInvalidValue. */
    /* @End: UT_CORE_1535 */
    ASSERT_EQ(ara::core::configreader::ParseError::kInvalidValue, ParseError.Reason());
    /* @Start: UT_CORE_1535 */
    /* @TS2: Invoke the Offset() with ParseError class.*/
    /* @TE2: Offset() should equal to 1. */
    /* @End: UT_CORE_1535 */
    ASSERT_EQ(1, ParseError.Offset());
    /* @Start: UT_CORE_1535 */
    /* @TS3: Invoke the what() with ParseError class.*/
    /* @TE3: what() should equal to "Invalid value". */
    /* @End: UT_CORE_1535 */
    ASSERT_STREQ("Invalid value", ParseError.what());
  }
  /* @Start: UT_CORE_1535 */
  /* @TS4: Get the value of isExceptionThrown */
  /* @TE4: isExceptionThrown should return true. */
  /* @End: UT_CORE_1535 */
  ASSERT_TRUE(isExceptionThrown);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1536
@Test_Description:    To test the functionality of parsing empty object.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611127, 611109, 611090, 611063
*******************************************************************************/
TEST(minijson_reader, parse_object_empty)
{
  char buffer[] = "{}";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  ara::core::configreader::ParseObject(
      BufferContext, [](ara::core::StringView, ara::core::configreader::value) {
        /* @Start: UT_CORE_1536 */
        /* @TS1: Create the empty buffer context .*/
        /* @TE1: ParseObject() return the fail. */
        /* @End: UT_CORE_1536 */
        FAIL();
      });
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1537
@Test_Description:    To test the functionality of parsing signal field object.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611083, 611079, 611066, 611065, 611059, 611058, 611057,
                      611056
*******************************************************************************/
TEST(minijson_reader, parse_object_single_field)
{
  char buffer[] = " {  \n\t\"field\" :\r \"value\"\t\n}  ";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  bool isReadField = false;
  ara::core::configreader::ParseObject(BufferContext,
      [&](const ara::core::StringView fieldName, const ara::core::configreader::value fieldValue) {
        ASSERT_FALSE(isReadField);
        isReadField = true;
        /* @Start: UT_CORE_1537 */
        /* @TS1: Create a buffer with string "field".*/
        /* @TE2: fieldName should equal to "field". */
        /* @End: UT_CORE_1537 */
        ASSERT_EQ("field", fieldName);
        /* @Start: UT_CORE_1537 */
        /* @TS2: Invoke the Type() with fieldName.*/
        /* @TE2: Type() should equal to kString. */
        /* @End: UT_CORE_1537 */
        ASSERT_EQ(ara::core::configreader::kString, fieldValue.Type());
        /* @Start: UT_CORE_1537 */
        /* @TS3: Invoke the As<ara::core::StringView>() with fieldValue.*/
        /* @TE3: As<ara::core::StringView>() should return "value". */
        /* @End: UT_CORE_1537 */
        ASSERT_EQ("value", fieldValue.As<ara::core::StringView>());
      });
  ASSERT_TRUE(isReadField);
}
struct ParseObjectMultipleFieldsHandler
{
  std::bitset<7> flags;

  void operator()(const ara::core::StringView n, const ara::core::configreader::value v)
  {
    if (n == "string")
    {
      flags[0] = 1;
      ASSERT_EQ(ara::core::configreader::kString, v.Type());
      ASSERT_EQ("value\"\\/\b\f\n\r\t", v.As<ara::core::StringView>());
    }
    else if (n == "integer")
    {
      flags[1] = 1;
      ASSERT_EQ(ara::core::configreader::kNumber, v.Type());
      ASSERT_EQ(42, v.As<long>());
    }
    else if (n == "floating_point")
    {
      flags[2] = 1;
      ASSERT_EQ(ara::core::configreader::kNumber, v.Type());
      ASSERT_DOUBLE_EQ(4261826387162873618273687126387.0, v.As<double>());
    }
    else if (n == "boolean_true")
    {
      flags[3] = 1;
      ASSERT_EQ(ara::core::configreader::kBoolean, v.Type());

      ASSERT_TRUE(v.As<bool>());
    }
    else if (n == "boolean_false")
    {
      flags[4] = 1;
      ASSERT_EQ(ara::core::configreader::kBoolean, v.Type());

      ASSERT_FALSE(v.As<bool>());
    }
    else if (n == "")
    {
      flags[5] = 1;
      ASSERT_EQ(ara::core::configreader::kNull, v.Type());
    }
    else if (n == "\xc3\xA0\x01\x02" "a"
                      "\xED\x9F\xBF\xEE\x80\x80\xEF\xBF\xBF"
                      "b" "你" "\xF0\x90\x80\x80" "\xF4\x8F\xBF\xBF" "à")
    {
      flags[6] = 1;
      ASSERT_EQ(ara::core::configreader::kString, v.Type());
      ASSERT_EQ("", v.As<ara::core::StringView>());
    }
    else
    {
      FAIL();
    }
  }
};
/*******************************************************************************
@TestCaseId:          UT_CORE_1538
@Test_Description:    To test the functionality of parsing multiple fields object.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611118
*******************************************************************************/
TEST(minijson_reader, parse_object_multiple_fields)
{
  char buffer[]
      = "{\"string\":\"value\\\"\\\\\\/\\b\\f\\n\\r\\t\",\"integer\":42,"
        "\"floating_point\":4261826387162873618273687126387,"
        "\"boolean_true\":true,\n\"boolean_false\":false,\"\":null,"
        "\"\\u00e0\\u0001\\u0002a\\ud7ff\\uE000\\uFffFb\\u4F60"
        "\\uD800\\uDC00\\uDBFF\\uDFFFà\":\"\"}";

  {
    ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

    ParseObjectMultipleFieldsHandler handler;
    const std::size_t read_bytes = ara::core::configreader::ParseObject(ConstBufferContext, handler);
    /* @Start: UT_CORE_1538 */
    /* @TS1: Create value of read_bytes.*/
    /* @TE1: read_bytes should equal to 223. */
    /* @End: UT_CORE_1538 */
    ASSERT_EQ(223, read_bytes);
    /* @Start: UT_CORE_1538 */
    /* @TS2: Invoke the handler.flags.all() */
    /* @TE2: handler.flags.all() should return true. */
    /* @End: UT_CORE_1538 */
    ASSERT_TRUE(handler.flags.all());
  }
  {
    std::istringstream ss(buffer);
    ara::core::configreader::IstreamContext IstreamContext(ss);

    ParseObjectMultipleFieldsHandler handler;
    const std::size_t read_bytes = ara::core::configreader::ParseObject(IstreamContext, handler);
    /* @Start: UT_CORE_1538 */
    /* @TS3: Create value of read_bytes.*/
    /* @TE3: read_bytes should equal to 223. */
    /* @End: UT_CORE_1538 */
    ASSERT_EQ(223, read_bytes);

    /* @Start: UT_CORE_1538 */
    /* @TS4: Invoke the handler.flags.all() */
    /* @TE4: handler.flags.all() should return true. */
    /* @End: UT_CORE_1538 */
    ASSERT_TRUE(handler.flags.all());
  }
  {
    // damage null terminator to test robustness
    buffer[sizeof(buffer) - 1] = 'x';
    ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

    ParseObjectMultipleFieldsHandler handler;
    const std::size_t read_bytes = ara::core::configreader::ParseObject(BufferContext, handler);
    /* @Start: UT_CORE_1538 */
    /* @TS5: Create value of read_bytes.*/
    /* @TE5: read_bytes should equal to 223. */
    /* @End: UT_CORE_1538 */
    ASSERT_EQ(223, read_bytes);

    /* @Start: UT_CORE_1538 */
    /* @TS6: Invoke the handler.flags.all() */
    /* @TE6: handler.flags.all() should return true. */
    /* @End: UT_CORE_1538 */
    ASSERT_TRUE(handler.flags.all());
  }
}

template <typename Context>
struct ParseObjectNestedHandler
{
  std::bitset<3> flags;
  Context& context;

  explicit ParseObjectNestedHandler(Context& context) : context(context)
  {
  }

  void operator()(ara::core::StringView n, ara::core::configreader::value v)
  {
    if (n == "")
    {
      ASSERT_EQ(ara::core::configreader::kObject, v.Type());

      ara::core::configreader::ParseObject(
          context, [&](ara::core::StringView n, ara::core::configreader::value v, auto& ctx) {
            ASSERT_EQ("nested2", n);
            ASSERT_EQ(ara::core::configreader::kObject, v.Type());

            ASSERT_FALSE(flags[0]);

            ara::core::configreader::ParseObject(
                ctx, [&](ara::core::StringView n, ara::core::configreader::value v) {
                  if (n == "val1")
                  {
                    ASSERT_FALSE(flags[0]);
                    flags[0] = 1;
                    ASSERT_EQ(ara::core::configreader::kNumber, v.Type());
                    ASSERT_EQ(42, v.As<uint16_t>());
                    ASSERT_EQ(42, v.As<float>());
                  }
                  else if (n == "nested3")
                  {
                    ASSERT_FALSE(flags[2]);
                    flags[2] = 1;
                    ASSERT_EQ(ara::core::configreader::kArray, v.Type());
                    ara::core::configreader::ParseArray(ctx, [](ara::core::configreader::value) { FAIL(); });
                  }
                  else
                  {
                    FAIL();
                  }
                });
          });
    }
    else if (n == "val2")
    {
      ASSERT_FALSE(flags[1]);
      flags[1] = 1;
      ASSERT_EQ(ara::core::configreader::kNumber, v.Type());
      ASSERT_DOUBLE_EQ(42.0, v.As<double>());
    }
    else
    {
      FAIL();
    }
  }
};
/*******************************************************************************
@TestCaseId:          UT_CORE_1539
@Test_Description:    To test the functionality of parsing nested object.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611118, 611098, 611077, 611076, 611073
*******************************************************************************/
TEST(minijson_reader, parse_object_nested)
{
  char buffer[] = "{\"\":{\"nested2\":{\"val1\":42,\"nested3\":[]}},\"val2\":42.0}";

  {
    ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

    ParseObjectNestedHandler<ara::core::configreader::ConstBufferContext> handler(ConstBufferContext);
    ara::core::configreader::ParseObject(ConstBufferContext, handler);
    /* @Start: UT_CORE_1539 */
    /* @TS1: Invoke the handler.flags.all() */
    /* @TE1: handler.flags.all() should return true. */
    /* @End: UT_CORE_1539 */
    ASSERT_TRUE(handler.flags.all());
  }
  {
    std::istringstream ss(buffer);
    ara::core::configreader::IstreamContext IstreamContext(ss);

    ParseObjectNestedHandler<ara::core::configreader::IstreamContext> handler(IstreamContext);
    ara::core::configreader::ParseObject(IstreamContext, handler);
    /* @Start: UT_CORE_1539 */
    /* @TS2: Invoke the handler.flags.all() */
    /* @TE2: handler.flags.all() should return true. */
    /* @End: UT_CORE_1539 */
    ASSERT_TRUE(handler.flags.all());
  }
  {
    // damage null terminator to test robustness
    buffer[sizeof(buffer) - 1] = 'x';
    ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

    ParseObjectNestedHandler<ara::core::configreader::BufferContext> handler(BufferContext);
    ara::core::configreader::ParseObject(BufferContext, handler);
    /* @Start: UT_CORE_1539 */
    /* @TS3: Invoke the handler.flags.all() */
    /* @TE3: handler.flags.all() should return true. */
    /* @End: UT_CORE_1539 */
    ASSERT_TRUE(handler.flags.all());
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1540
@Test_Description:    To test the functionality of reading parse object bytes .
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611094
*******************************************************************************/
TEST(minijson_reader, parse_object_bytes_read)
{
  char buffer[] = "{\"first\":1,\"nesting\":[]}{\"nesting\":{},\"second\":2}{\"3rd\":3}";

  std::size_t bytesRead = 0;
  {
    ara::core::configreader::BufferContext ctx(buffer, sizeof(buffer));
    bytesRead
        = ara::core::configreader::ParseObject(ctx, [&](auto...) { ara::core::configreader::Ignore(ctx); });
    /* @Start: UT_CORE_1540 */
    /* @TS1: Create value of read_bytes.*/
    /* @TE1: bytesRead should equal to 24. */
    /* @End: UT_CORE_1540 */
    ASSERT_EQ(24, bytesRead);
  }
  {
    ara::core::configreader::BufferContext ctx(buffer + bytesRead, sizeof(buffer) - bytesRead);
    bytesRead = ara::core::configreader::ParseObject(
        ctx, [&](const ara::core::StringView name, const ara::core::configreader::value value) {
          if (name == "second")
          {
            ASSERT_EQ(2, value.As<int>());
            return;
          }
          ara::core::configreader::Ignore(ctx);
        });
    /* @Start: UT_CORE_1540 */
    /* @TS2: Create value of bytesRead.*/
    /* @TE2: bytesRead should equal to 25. */
    /* @End: UT_CORE_1540 */
    ASSERT_EQ(25, bytesRead);

    // Keep on using the same context to parse the third object
    bytesRead = ara::core::configreader::ParseObject(
        ctx, [&](const ara::core::StringView name, const ara::core::configreader::value value) {
          /* @Start: UT_CORE_1540 */
          /* @TS3: Create value of name.*/
          /* @TE3: name should equal to "3rd". */
          /* @End: UT_CORE_1540 */
          ASSERT_EQ("3rd", name);
          /* @Start: UT_CORE_1540 */
          /* @TS4: Invoke the As<int>() with value class.*/
          /* @TE4: As<int>() should equal to 3 .*/
          /* @End: UT_CORE_1540 */
          ASSERT_EQ(3, value.As<int>());
        });
    /* @Start: UT_CORE_1540 */
    /* @TS5: Create value of bytesRead.*/
    /* @TE5: bytesRead should equal to 9. */
    /* @End: UT_CORE_1540 */
    ASSERT_EQ(9, bytesRead);
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1541
@Test_Description:    To test the functionality of parsing empty array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611148
*******************************************************************************/
TEST(minijson_reader, parse_array_empty)
{
  char buffer[] = "[]";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  ara::core::configreader::ParseArray(BufferContext, [](ara::core::configreader::value) {
    /* @Start: UT_CORE_1541 */
    /* @TS1: Invoke the ParseArray() .*/
    /* @TE1: ParseArray() should not throw exception. */
    /* @End: UT_CORE_1541 */
    FAIL();
  });
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1542
@Test_Description:    To test the functionality of parsing single element in array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611083, 611068, 611067
*******************************************************************************/
TEST(minijson_reader, parse_array_single_elem)
{
  char buffer[] = " [  \n\t\"value\"\t\n]  ";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  bool isReadElem = false;
  ara::core::configreader::ParseArray(BufferContext, [&](ara::core::configreader::value elemValue) {
    /* @Start: UT_CORE_1542 */
    /* @TS1: Create the value of isReadElem */
    /* @TE1: isReadElem should return false. */
    /* @End: UT_CORE_1542 */
    ASSERT_FALSE(isReadElem);
    isReadElem = true;
    /* @Start: UT_CORE_1542 */
    /* @TS2: Invoke the Type() with elemValue class.*/
    /* @TE2: Type() should equal to kString. */
    /* @End: UT_CORE_1542 */
    ASSERT_EQ(ara::core::configreader::kString, elemValue.Type());
    /* @Start: UT_CORE_1542 */
    /* @TS3: Invoke the As<ara::core::StringView>() with elemValue class.*/
    /* @TE3: As<ara::core::StringView>() should equal to "value". */
    /* @End: UT_CORE_1542 */
    ASSERT_EQ("value", elemValue.As<ara::core::StringView>());
  });
  /* @Start: UT_CORE_1542 */
  /* @TS4: Create the value of isReadElem */
  /* @TE4: isReadElem should return true. */
  /* @End: UT_CORE_1542 */
  ASSERT_TRUE(isReadElem);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1543
@Test_Description:    To test the functionality of parsing two elements in single array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611164, 611148, 611097
*******************************************************************************/
TEST(minijson_reader, parse_array_single_elem2)
{
  char buffer[] = "[1]";
  ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

  bool isReadElem = false;
  ara::core::configreader::ParseArray(BufferContext, [&](ara::core::configreader::value elemValue) {
    /* @Start: UT_CORE_1543 */
    /* @TS1: Create the value of isReadElem */
    /* @TE1: isReadElem should return false. */
    /* @End: UT_CORE_1543 */
    ASSERT_FALSE(isReadElem);
    isReadElem = true;
    /* @Start: UT_CORE_1543 */
    /* @TS2: Invoke the Type() with elemValue class.*/
    /* @TE2: Type() should equal to kNumber. */
    /* @End: UT_CORE_1543 */
    ASSERT_EQ(ara::core::configreader::kNumber, elemValue.Type());
    /* @Start: UT_CORE_1543 */
    /* @TS3: Invoke the As<int8_t>() elemValue class.*/
    /* @TE3: As<int8_t>() should equal to 1. */
    /* @End: UT_CORE_1543 */
    ASSERT_EQ(1, elemValue.As<int8_t>());
    /* @Start: UT_CORE_1543 */
    /* @TS4: Invoke the As<float>(). */
    /* @TE4: As<float>() should equal to 1. */
    /* @End: UT_CORE_1543 */
    ASSERT_EQ(1, elemValue.As<float>());
    /* @Start: UT_CORE_1543 */
    /* @TS5: Invoke the As<ara::core::StringView>().*/
    /* @TE5: As<ara::core::StringView>() should return BadValueCast. */
    /* @End: UT_CORE_1543 */
    ASSERT_THROW(elemValue.As<ara::core::StringView>(), ara::core::configreader::BadValueCast);
  });
  /* @Start: UT_CORE_1543 */
  /* @TS6: Get the value of isReadElem */
  /* @TE6: isReadElem should return true. */
  /* @End: UT_CORE_1543 */
  ASSERT_TRUE(isReadElem);
}

struct ParseArrayMultipleElemsHandler
{
  std::size_t counter = 0;

  void operator()(const ara::core::configreader::value v)
  {
    switch (counter++)
    {
    case 0:
      ASSERT_EQ(ara::core::configreader::kString, v.Type());
      ASSERT_EQ("value", v.As<ara::core::StringView>());
      break;
    case 1:
      ASSERT_EQ(ara::core::configreader::kNumber, v.Type());
      ASSERT_EQ(42, v.As<long>());
      break;
    case 2:
      ASSERT_EQ(ara::core::configreader::kNumber, v.Type());
      ASSERT_DOUBLE_EQ(42.0, v.As<double>());
      break;
    case 3:
      ASSERT_EQ(ara::core::configreader::kBoolean, v.Type());

      ASSERT_TRUE(v.As<bool>());
      break;
    case 4:
      ASSERT_EQ(ara::core::configreader::kBoolean, v.Type());

      ASSERT_FALSE(v.As<bool>());
      break;
    case 5:
      ASSERT_EQ(ara::core::configreader::kNull, v.Type());
      break;
    case 6:
      ASSERT_EQ(ara::core::configreader::kString, v.Type());
      ASSERT_EQ("", v.As<ara::core::StringView>());
      break;
    default:
      FAIL();
    }
  }
};
/*******************************************************************************
@TestCaseId:          UT_CORE_1544
@Test_Description:    To test the functionality of parsing multiple elements in array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611164, 611148, 611097
*******************************************************************************/
TEST(minijson_reader, parse_array_multiple_elems)
{
  char buffer[] = "[\"value\",42,42.0,true,\nfalse,null,\"\"]";

  {
    ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

    ParseArrayMultipleElemsHandler handler;
    ara::core::configreader::ParseArray(ConstBufferContext, handler);
    /* @Start: UT_CORE_1544 */
    /* @TS1: Invoke handler object and get counter value.*/
    /* @TE1: Counter value should equal to 7. */
    /* @End: UT_CORE_1544 */
    ASSERT_EQ(handler.counter, 7);
  }
  {
    std::istringstream ss(buffer);
    ara::core::configreader::IstreamContext IstreamContext(ss);

    ParseArrayMultipleElemsHandler handler;
    ara::core::configreader::ParseArray(IstreamContext, handler);
    /* @Start: UT_CORE_1544 */
    /* @TS2: Invoke handler object and get counter value.*/
    /* @TE2: Counter value should equal to 7. */
    /* @End: UT_CORE_1544 */
    ASSERT_EQ(handler.counter, 7);
  }
  {
    // damage null terminator to test robustness
    buffer[sizeof(buffer) - 1] = 'x';
    ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

    ParseArrayMultipleElemsHandler handler;
    ara::core::configreader::ParseArray(BufferContext, handler);
    /* @Start: UT_CORE_1544 */
    /* @TS3: Invoke handler object and get counter value.*/
    /* @TE3: Counter value should equal to 7. */
    /* @End: UT_CORE_1544 */
    ASSERT_EQ(handler.counter, 7);
  }
}

template <typename Context>
struct ParseArrayNestedHandler
{
  std::size_t counter = 0;
  Context& context;

  explicit ParseArrayNestedHandler(Context& context) : context(context)
  {
  }

  void operator()(const ara::core::configreader::value v)
  {
    switch (counter++)
    {
    case 0:
    {
      ASSERT_EQ(ara::core::configreader::kArray, v.Type());
      NestedHandler handler(context);
      ara::core::configreader::ParseArray(context, handler);

      ASSERT_TRUE(handler.isReadElem);
      break;
    }
    case 1:
      ASSERT_EQ(ara::core::configreader::kNumber, v.Type());
      ASSERT_DOUBLE_EQ(42.0, v.As<double>());
      break;
    default:
      FAIL();
    }
  }

  struct NestedHandler
  {
    Context& context;
    bool isReadElem = false;

    explicit NestedHandler(Context& context) : context(context)
    {
    }

    void operator()(const ara::core::configreader::value v)
    {
      ASSERT_FALSE(isReadElem);
      isReadElem = true;

      ASSERT_EQ(ara::core::configreader::kArray, v.Type());

      std::size_t counter_nested = 0;
      ara::core::configreader::ParseArray(context, [&](const ara::core::configreader::value v, auto& ctx) {
        switch (counter_nested++)
        {
        case 0:
          ASSERT_EQ(ara::core::configreader::kNumber, v.Type());
          ASSERT_EQ(42, v.As<long>());
          break;
        case 1:
          ASSERT_EQ(ara::core::configreader::kObject, v.Type());
          ara::core::configreader::ParseObject(ctx, [](auto...) { FAIL(); });
          break;
        default:
          FAIL();
        }
      });
      ASSERT_EQ(counter_nested, 2);
    }
  };
};
/*******************************************************************************
@TestCaseId:          UT_CORE_1545
@Test_Description:    To test the functionality of parse array method.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611161, 611108, 611080, 611072, 611071
*******************************************************************************/
TEST(minijson_reader, parse_array_nested)
{
  char buffer[] = "[[[42,{}]],42.0]";

  {
    ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

    ParseArrayNestedHandler<ara::core::configreader::ConstBufferContext> handler(ConstBufferContext);
    ara::core::configreader::ParseArray(ConstBufferContext, handler);
    /* @Start: UT_CORE_1545 */
    /* @TS1: Invoke handler object and get counter value.*/
    /* @TE1: handler.counter should equal to 2. */
    /* @End: UT_CORE_1545 */
    ASSERT_EQ(handler.counter, 2);
  }
  {
    std::istringstream ss(buffer);
    ara::core::configreader::IstreamContext IstreamContext(ss);

    ParseArrayNestedHandler<ara::core::configreader::IstreamContext> handler(IstreamContext);
    ara::core::configreader::ParseArray(IstreamContext, handler);
    /* @Start: UT_CORE_1545 */
    /* @TS2: Invoke handler object and get counter value. */
    /* @TE2: handler.counter should equal to 2. */
    /* @End: UT_CORE_1545 */
    ASSERT_EQ(handler.counter, 2);
  }
  {
    // damage null terminator to test robustness
    buffer[sizeof(buffer) - 1] = 'x';
    ara::core::configreader::BufferContext BufferContext(buffer, sizeof(buffer) - 1);

    ParseArrayNestedHandler<ara::core::configreader::BufferContext> handler(BufferContext);
    ara::core::configreader::ParseArray(BufferContext, handler);
    /* @Start: UT_CORE_1545 */
    /* @TS3: Invoke handler object and get counter value.*/
    /* @TE3: handler.counter should equal to 2. */
    /* @End: UT_CORE_1545 */
    ASSERT_EQ(handler.counter, 2);
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1546
@Test_Description:    To test the functionality of reading the parse array bytes.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611094
*******************************************************************************/
TEST(minijson_reader, parse_array_bytes_read)
{
  char buffer[] = "[1,2,3,{}][{},4,5,6][7]";

  std::size_t bytesRead = 0;
  {
    ara::core::configreader::BufferContext ctx(buffer, sizeof(buffer));
    bytesRead
        = ara::core::configreader::ParseArray(ctx, [&](auto...) { ara::core::configreader::Ignore(ctx); });
    /* @Start: UT_CORE_1546 */
    /* @TS1: Create value of bytesRead.*/
    /* @TE1: bytesRead should equal to 10. */
    /* @End: UT_CORE_1546 */
    ASSERT_EQ(10, bytesRead);
  }
  {
    ara::core::configreader::BufferContext ctx(buffer + bytesRead, sizeof(buffer) - bytesRead);
    bytesRead = ara::core::configreader::ParseArray(
        ctx, [&, x = 4](const ara::core::configreader::value value) mutable {
          if (value.Type() == ara::core::configreader::kObject)
          {
            ara::core::configreader::Ignore(ctx);
            return;
          }
          /* @Start: UT_CORE_1546 */
          /* @TS2: Invoke the As<int>() with value class.*/
          /* @TE2: As<int>() should equal to x++. */
          /* @End: UT_CORE_1546 */
          ASSERT_EQ(x++, value.As<int>());
        });
    /* @Start: UT_CORE_1546 */
    /* @TS3: Create value of bytesRead.*/
    /* @TE3: bytesRead should equal to 10. */
    /* @End: UT_CORE_1546 */
    ASSERT_EQ(10, bytesRead);

    // Keep on using the same context for the third array
    bytesRead = ara::core::configreader::ParseArray(ctx, [&](const ara::core::configreader::value value) {
      /* @Start: UT_CORE_1546 */
      /* @TS4: Invoke the As<int>() with value class.*/
      /* @TE4: As<int>() should equal to 7. */
      /* @End: UT_CORE_1546 */
      ASSERT_EQ(7, value.As<int>());
    });
    /* @Start: UT_CORE_1546 */
    /* @TS5: Create value of bytesRead.*/
    /* @TE5: bytesRead should equal to 3. */
    /* @End: UT_CORE_1546 */
    ASSERT_EQ(3, bytesRead);
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1547
@Test_Description:    To test the functionality of parsing truncated object.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611094, 611063
*******************************************************************************/
TEST(minijson_reader, parse_object_truncated)
{
  using ara::core::configreader::ParseError;

  char buffer[] = "{\"str\":\"val\",\"int\":42,\"null\":null}";

  for (std::size_t i = sizeof(buffer) - 2; i < sizeof(buffer); i--)
  {
    buffer[i] = 0;
    ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

    bool isExceptionThrown = false;
    try
    {
      ara::core::configreader::ParseObject(ConstBufferContext,
          ara::core::configreader::detail::Ignore<ara::core::configreader::ConstBufferContext>(
              ConstBufferContext));
    }
    catch (const ParseError& e)
    {
      isExceptionThrown = true;

      switch (i)
      {
      case 0:
        /* @Start: UT_CORE_1547 */
        /* @TS1: Invoke the Reason() with e.*/
        /* @TE1: Reason() should equal to kExpectedOpeningBracket. */
        /* @End: UT_CORE_1547 */
        ASSERT_EQ(ParseError::kExpectedOpeningBracket, e.Reason());
        /* @Start: UT_CORE_1547 */
        /* @TS2: Invoke the what() with e.*/
        /* @TE2: what() should equal to "Expected opening bracket". */
        /* @End: UT_CORE_1547 */
        ASSERT_STREQ("Expected opening bracket", e.what());
        break;
      case 1:
        /* @Start: UT_CORE_1547 */
        /* @TS3: Invoke the Reason() with e.*/
        /* @TE3: Reason() should equal to kExpectedOpeningQuote. */
        /* @End: UT_CORE_1547 */
        ASSERT_EQ(ParseError::kExpectedOpeningQuote, e.Reason());
        /* @Start: UT_CORE_1547 */
        /* @TS4: Invoke the what() with e.*/
        /* @TE4: what() should equal to "Expected opening quote". */
        /* @End: UT_CORE_1547 */
        ASSERT_STREQ("Expected opening quote", e.what());
        break;
        /* @Start: UT_CORE_1547 */
        /* @TS5: Invoke the Reason() with e.*/
        /* @TE5: e.Reason() should equal to kUnterminatedValue. */
        /* @End: UT_CORE_1547 */
      case 2:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 3:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 4:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 5:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 6:
        /* @Start: UT_CORE_1547 */
        /* @TS6: Invoke the Reason() with e.*/
        /* @TE6: Reason() should equal to kExpectedColon. */
        /* @End: UT_CORE_1547 */
        ASSERT_EQ(ParseError::kExpectedColon, e.Reason());
        /* @Start: UT_CORE_1547 */
        /* @TS7: Invoke the what() with e.*/
        /* @TE7: what() should equal to "Expected colon". */
        /* @End: UT_CORE_1547 */
        ASSERT_STREQ("Expected colon", e.what());
        break;
        /* @Start: UT_CORE_1547 */
        /* @TS8: Invoke the Reason() with e.*/
        /* @TE8: Reason() should equal to kUnterminatedValue. */
        /* @End: UT_CORE_1547 */
      case 7:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 8:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 9:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 10:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 11:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 12:
        /* @Start: UT_CORE_1547 */
        /* @TS9: Invoke the Reason() with e.*/
        /* @TE9: Reason() should equal to kExpectedCommaOrClosingBracket. */
        /* @End: UT_CORE_1547 */
        ASSERT_EQ(ParseError::kExpectedCommaOrClosingBracket, e.Reason());
        /* @Start: UT_CORE_1547 */
        /* @TS10: Invoke the what() with e.*/
        /* @TE10: what() should equal to "Expected comma or closing bracket". */
        /* @End: UT_CORE_1547 */
        ASSERT_STREQ("Expected comma or closing bracket", e.what());
        break;
      case 13:
        /* @Start: UT_CORE_1547 */
        /* @TS11: Invoke the Reason() with e.*/
        /* @TE11: Reason() should equal to kExpectedOpeningQuote. */
        /* @End: UT_CORE_1547 */
        ASSERT_EQ(ParseError::kExpectedOpeningQuote, e.Reason());
        break;
        /* @Start: UT_CORE_1547 */
        /* @TS12: Invoke the Reason() with e.*/
        /* @TE12: Reason() should equal to kUnterminatedValue. */
        /* @End: UT_CORE_1547 */
      case 14:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 15:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 16:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 17:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 18:
        /* @Start: UT_CORE_1547 */
        /* @TS13: Invoke the Reason() with e.*/
        /* @TE13: Reason() should equal to kExpectedColon. */
        /* @End: UT_CORE_1547 */
        ASSERT_EQ(ParseError::kExpectedColon, e.Reason());
        break;
        /* @Start: UT_CORE_1547 */
        /* @TS14: Invoke the Reason() with e.*/
        /* @TE14: Reason() should equal to kUnterminatedValue. */
        /* @End: UT_CORE_1547 */
      case 19:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 20:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 21:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
        /* @Start: UT_CORE_1547 */
        /* @TS15: Invoke the Reason() with e.*/
        /* @TE15: Reason() should equal to kExpectedOpeningQuote. */
        /* @End: UT_CORE_1547 */
      case 22:
        ASSERT_EQ(ParseError::kExpectedOpeningQuote, e.Reason());
        break;
        /* @Start: UT_CORE_1547 */
        /* @TS16: Invoke the Reason() with e.*/
        /* @TE16: Reason() should equal to kUnterminatedValue. */
        /* @End: UT_CORE_1547 */
      case 23:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 24:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 25:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 26:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 27:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
        /* @Start: UT_CORE_1547 */
        /* @TS17: Invoke the Reason() with e.*/
        /* @TE17: Reason() should equal to kExpectedColon. */
        /* @End: UT_CORE_1547 */
      case 28:
        ASSERT_EQ(ParseError::kExpectedColon, e.Reason());
        break;
        /* @Start: UT_CORE_1547 */
        /* @TS18: Invoke the Reason() with e.*/
        /* @TE18: Reason() should equal to kUnterminatedValue. */
        /* @End: UT_CORE_1547 */
      case 29:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 30:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 31:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 32:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 33:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      default:
        FAIL();
      }
    }
    /* @Start: UT_CORE_1547 */
    /* @TS19: Get the value of isExceptionThrown */
    /* @TE19: isExceptionThrown should return true. */
    /* @End: UT_CORE_1547 */
    ASSERT_TRUE(isExceptionThrown);
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1548
@Test_Description:    To test the functionality of parsing truncated array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611092, 611063
*******************************************************************************/
TEST(minijson_reader, parse_array_truncated)
{
  using ara::core::configreader::ParseError;

  char buffer[] = "[\"val\",42,null]";

  for (std::size_t i = sizeof(buffer) - 2; i < sizeof(buffer); i--)
  {
    buffer[i] = 0;
    ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

    bool isExceptionThrown = false;
    try
    {
      ara::core::configreader::ParseArray(ConstBufferContext,
          ara::core::configreader::detail::Ignore<ara::core::configreader::ConstBufferContext>(
              ConstBufferContext));
    }
    catch (const ParseError& e)
    {
      isExceptionThrown = true;

      switch (i)
      {
        /* @Start: UT_CORE_1548 */
        /* @TS1: Invoke the Reason() with e.*/
        /* @TE1: Reason() should equal to kExpectedOpeningBracket. */
        /* @End: UT_CORE_1548 */
      case 0:
        ASSERT_EQ(ParseError::kExpectedOpeningBracket, e.Reason());
        break;
        /* @Start: UT_CORE_1548 */
        /* @TS2: Invoke the Reason() with e.*/
        /* @TE2: Reason() should equal to kUnterminatedValue. */
        /* @End: UT_CORE_1548 */
      case 1:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 2:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 3:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 4:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 5:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 6:
        /* @Start: UT_CORE_1548 */
        /* @TS3: Invoke the Reason() with e.*/
        /* @TE3: Reason() should equal to kExpectedCommaOrClosingBracket. */
        /* @End: UT_CORE_1548 */
        ASSERT_EQ(ParseError::kExpectedCommaOrClosingBracket, e.Reason());
        break;
        /* @Start: UT_CORE_1548 */
        /* @TS4: Invoke the Reason() with e.*/
        /* @TE4: e.Reason() should equal to kUnterminatedValue. */
        /* @End: UT_CORE_1548 */
      case 7:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 8:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 9:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 10:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 11:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 12:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 13:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      case 14:
        ASSERT_EQ(ParseError::kUnterminatedValue, e.Reason());
        break;
      default:
        FAIL();
      }
    }
    /* @Start: UT_CORE_1548 */
    /* @TS5: Get the value of isExceptionThrown */
    /* @TE5: isExceptionThrown should return true. */
    /* @End: UT_CORE_1548 */
    ASSERT_TRUE(isExceptionThrown);
  }
}

template <std::size_t Length>
void ParseObjectInvalidHelper(const char (&buffer)[Length],
    const ara::core::configreader::ParseError::ErrorReason expectedReason,
    const char* const expectedWhat = nullptr)
{
  SCOPED_TRACE(buffer);
  ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

  bool isExceptionThrown = false;

  try
  {
    ara::core::configreader::ParseObject(ConstBufferContext,
        ara::core::configreader::detail::Ignore<ara::core::configreader::ConstBufferContext>(
            ConstBufferContext));
  }
  catch (const ara::core::configreader::ParseError& e)
  {
    isExceptionThrown = true;
    ASSERT_EQ(expectedReason, e.Reason());
    if (expectedWhat)
    {
      ASSERT_STREQ(expectedWhat, e.what());
    }
  }

  ASSERT_TRUE(isExceptionThrown);
}

template <std::size_t Length>
void ParseArrayInvalidHelper(const char (&buffer)[Length],
    const ara::core::configreader::ParseError::ErrorReason expectedReason,
    const char* const expectedWhat = nullptr)
{
  SCOPED_TRACE(buffer);
  ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

  bool isExceptionThrown = false;

  try
  {
    ara::core::configreader::ParseArray(ConstBufferContext,
        ara::core::configreader::detail::Ignore<ara::core::configreader::ConstBufferContext>(
            ConstBufferContext));
  }
  catch (const ara::core::configreader::ParseError& e)
  {
    isExceptionThrown = true;
    ASSERT_EQ(expectedReason, e.Reason());
    if (expectedWhat)
    {
      ASSERT_STREQ(expectedWhat, e.what());
    }
  }

  ASSERT_TRUE(isExceptionThrown);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1549
@Test_Description:    To test the functionality of parsing invalid object.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611192, 611170, 611147
*******************************************************************************/
TEST(minijson_reader, parse_object_invalid)
{
  /* @Start: UT_CORE_1549 */
  /* @TS1: Invoke the ParseObjectInvalidHelper.*/
  /* @TE1: ParseObjectInvalidHelper() return the invalid objects. */
  /* @End: UT_CORE_1549 */
  ParseObjectInvalidHelper("", ara::core::configreader::ParseError::kExpectedOpeningBracket);

  ParseObjectInvalidHelper("{\"x\":8.2e+62738", ara::core::configreader::ParseError::kUnterminatedValue);

  ParseObjectInvalidHelper("{\"x\":", ara::core::configreader::ParseError::kUnterminatedValue);

  ParseObjectInvalidHelper(
      "{\"x\":}", ara::core::configreader::ParseError::kExpectedValue, "Expected a value");

  ParseObjectInvalidHelper("{\"x\": }", ara::core::configreader::ParseError::kExpectedValue);

  ParseObjectInvalidHelper("{\"x\":,\"foo\"}", ara::core::configreader::ParseError::kExpectedValue);

  ParseObjectInvalidHelper("{:\"foo\"}", ara::core::configreader::ParseError::kExpectedOpeningQuote);

  ParseObjectInvalidHelper(
      "{\"x\":\"foo\",:\"bar\"}", ara::core::configreader::ParseError::kExpectedOpeningQuote);

  ParseObjectInvalidHelper("{\"x\": ,\"foo\"}", ara::core::configreader::ParseError::kExpectedValue);

  ParseObjectInvalidHelper("{\"x\":8.}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":8..2}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":8.2e}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":8.2e+-7}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":8.2e7e}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":8.2e7+}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":8.2e+}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":.2}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":0.e7}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":01}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":- 1}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":+1}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":3.4.5}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":0x1273}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":NaN}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":nuxl}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"x\":nulll}", ara::core::configreader::ParseError::kInvalidValue);

  ParseObjectInvalidHelper("{\"\\ufffx\":null}", ara::core::configreader::ParseError::kInvalidEscapeSequence);

  ParseObjectInvalidHelper(
      "{\"x\":\"\\ufffx\"}", ara::core::configreader::ParseError::kInvalidEscapeSequence);

  ParseObjectInvalidHelper("{\"x\":\"\\ufff", ara::core::configreader::ParseError::kUnterminatedValue);

  ParseObjectInvalidHelper("{\"\\u\":\"\"}", ara::core::configreader::ParseError::kInvalidEscapeSequence);

  ParseObjectInvalidHelper(
      "{\"foo\nfoo\":\"\"}", ara::core::configreader::ParseError::kUnescapedControlCharacter);

  ParseObjectInvalidHelper(
      "{\"foo\x01"
      "foo\":\"\"}",
      ara::core::configreader::ParseError::kUnescapedControlCharacter);

  ParseObjectInvalidHelper(
      "{\"foo\x1F"
      "foo\":\"\"}",
      ara::core::configreader::ParseError::kUnescapedControlCharacter);

  ParseObjectInvalidHelper(
      "{\"\\ud800\":null}", ara::core::configreader::ParseError::kExpectedUtf16LowSurrogate);

  ParseObjectInvalidHelper(
      "{\"\\ud800:null}", ara::core::configreader::ParseError::kExpectedUtf16LowSurrogate);

  ParseObjectInvalidHelper("{\"\\udc00\":null}", ara::core::configreader::ParseError::kInvalidUtf16Character);

  ParseObjectInvalidHelper("{\"\\u0000\":null}", ara::core::configreader::ParseError::kNullUtf16Character);

  ParseObjectInvalidHelper(
      "{\"\\ud800\\uee00\":null}", ara::core::configreader::ParseError::kInvalidUtf16Character);

  ParseObjectInvalidHelper(
      "{\"\\ud800\\u0000\":null}", ara::core::configreader::ParseError::kInvalidUtf16Character);

  ParseObjectInvalidHelper("{\"\\x\":null}", ara::core::configreader::ParseError::kInvalidEscapeSequence);

  ParseObjectInvalidHelper(
      "{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":"
      "[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":["
      "]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}",
      ara::core::configreader::ParseError::kExpectedNestingLimit, "Exceeded nesting limit (32)");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1550
@Test_Description:    To test the functionality of parsing invalid array.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611148
*******************************************************************************/
TEST(minijson_reader, parse_array_invalid)
{
  /* @Start: UT_CORE_1550 */
  /* @TS1: Invoke the ParseArrayInvalidHelper().*/
  /* @TE1: ParseArrayInvalidHelper() return the invalid arrays. */
  /* @End: UT_CORE_1550 */
  ParseArrayInvalidHelper("", ara::core::configreader::ParseError::kExpectedOpeningBracket);

  ParseArrayInvalidHelper("[8.2e+62738", ara::core::configreader::ParseError::kUnterminatedValue);

  ParseArrayInvalidHelper("[", ara::core::configreader::ParseError::kUnterminatedValue);

  ParseArrayInvalidHelper("[5,", ara::core::configreader::ParseError::kUnterminatedValue);

  ParseArrayInvalidHelper("[,]", ara::core::configreader::ParseError::kExpectedValue);

  ParseArrayInvalidHelper("[ ,]", ara::core::configreader::ParseError::kExpectedValue);

  ParseArrayInvalidHelper("[,42]", ara::core::configreader::ParseError::kExpectedValue);

  ParseArrayInvalidHelper("[ ,42]", ara::core::configreader::ParseError::kExpectedValue);

  ParseArrayInvalidHelper("[42,]", ara::core::configreader::ParseError::kExpectedValue);

  ParseArrayInvalidHelper("[42, ]", ara::core::configreader::ParseError::kExpectedValue);

  ParseArrayInvalidHelper("[e+62738]", ara::core::configreader::ParseError::kInvalidValue);

  ParseArrayInvalidHelper("[3.4.5]", ara::core::configreader::ParseError::kInvalidValue);

  ParseArrayInvalidHelper("[0x1273]", ara::core::configreader::ParseError::kInvalidValue);

  ParseArrayInvalidHelper("[NaN]", ara::core::configreader::ParseError::kInvalidValue);

  ParseArrayInvalidHelper("[nuxl]", ara::core::configreader::ParseError::kInvalidValue);

  ParseArrayInvalidHelper("[\"\\ufffx\"]", ara::core::configreader::ParseError::kInvalidEscapeSequence);

  ParseArrayInvalidHelper("[\"\\ufff\"]", ara::core::configreader::ParseError::kInvalidEscapeSequence);

  ParseArrayInvalidHelper("[\"foo\nfoo\"]", ara::core::configreader::ParseError::kUnescapedControlCharacter);

  ParseArrayInvalidHelper(
      "[\"foo\x01"
      "foo\"]",
      ara::core::configreader::ParseError::kUnescapedControlCharacter);

  ParseArrayInvalidHelper(
      "[\"foo\x1F"
      "foo\"]",
      ara::core::configreader::ParseError::kUnescapedControlCharacter);

  ParseArrayInvalidHelper("[\"\\ud800\"]", ara::core::configreader::ParseError::kExpectedUtf16LowSurrogate);

  ParseArrayInvalidHelper("[\"\\udc00\"]", ara::core::configreader::ParseError::kInvalidUtf16Character);

  ParseArrayInvalidHelper("[\"\\u0000\"]", ara::core::configreader::ParseError::kNullUtf16Character);

  ParseArrayInvalidHelper(
      "[\"\\ud800\\uee00\"]", ara::core::configreader::ParseError::kInvalidUtf16Character);

  ParseArrayInvalidHelper(
      "[\"\\ud800\\u0000\"]", ara::core::configreader::ParseError::kInvalidUtf16Character);

  ParseArrayInvalidHelper("[\"\\x\"]", ara::core::configreader::ParseError::kInvalidEscapeSequence);

  ParseArrayInvalidHelper(
      "[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":"
      "[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{\"\":[{"
      "}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]}]",
      ara::core::configreader::ParseError::kExpectedNestingLimit, "Exceeded nesting limit (32)");
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1551
@Test_Description:    To test the functionality of Nested object or array not parsed.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      
@Design Tracing:      611148, 611127, 611074, 611063
*******************************************************************************/
TEST(minijson_reader, nested_not_parsed)
{
  {
    char buffer[] = "{\"a\":[]}";
    ara::core::configreader::BufferContext context(buffer, sizeof(buffer));

    try
    {
      ara::core::configreader::ParseObject(context, [](auto...) {});
      FAIL(); // should never get here
    }
    catch (const ara::core::configreader::ParseError& e)
    {
      /* @Start: UT_CORE_1551 */
      /* @TS1: Invoke the Reason() with e.*/
      /* @TE1: Reason() should equal to kNestedObjectOrArrayNotParsed. */
      /* @End: UT_CORE_1551 */
      ASSERT_EQ(ara::core::configreader::ParseError::kNestedObjectOrArrayNotParsed, e.Reason());
      /* @Start: UT_CORE_1551 */
      /* @TS2: Invoke the what() with e.*/
      /* @TE2: what() should equal to "Nested object or array not parsed". */
      /* @End: UT_CORE_1551 */
      ASSERT_STREQ("Nested object or array not parsed", e.what());
    }
  }

  {
    char buffer[] = "[{}]";
    ara::core::configreader::BufferContext context(buffer, sizeof(buffer));

    try
    {
      ara::core::configreader::ParseArray(context, [](auto...) {});
      FAIL(); // should never get here
    }
    catch (const ara::core::configreader::ParseError& e)
    {
      /* @Start: UT_CORE_1551 */
      /* @TS3: Invoke the Reason() with e.*/
      /* @TE3: Reason() should equal to kNestedObjectOrArrayNotParsed. */
      /* @End: UT_CORE_1551 */
      ASSERT_EQ(ara::core::configreader::ParseError::kNestedObjectOrArrayNotParsed, e.Reason());
      /* @Start: UT_CORE_1551 */
      /* @TS4: Invoke the what() with e.*/
      /* @TE4: what() should equal to "Nested object or array not parsed". */
      /* @End: UT_CORE_1551 */
      ASSERT_STREQ("Nested object or array not parsed", e.what());
    }
  }
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1552
@Test_Description:    To test the functionality of hex digit test for A.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611153
*******************************************************************************/
TEST(minijson_reader, IsHexDigitTestForA)
{
  // char c;
  bool isB = ara::core::configreader::detail::IsHexDigit('a');
  /* @Start: UT_CORE_1552 */
  /* @TS1: Invoke the is_hex_digit with 'a'.*/
  /* @TE1: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1552 */
  ASSERT_EQ(isB, true);

  bool isD = ara::core::configreader::detail::IsHexDigit('A');
  /* @Start: UT_CORE_1552 */
  /* @TS2: Invoke the is_hex_digit with 'A'.*/
  /* @TE2: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1552 */
  ASSERT_EQ(isD, true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1553
@Test_Description:    To test the functionality of hex digit test for B.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611153
*******************************************************************************/
TEST(minijson_reader, IsHexDigitTestForB)
{
  bool isC = ara::core::configreader::detail::IsHexDigit('b');
  /* @Start: UT_CORE_1553 */
  /* @TS1: Invoke the is_hex_digit with 'b'.*/
  /* @TE1: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1553 */
  ASSERT_EQ(isC, true);

  bool isD = ara::core::configreader::detail::IsHexDigit('B');
  /* @Start: UT_CORE_1553 */
  /* @TS2: Invoke the is_hex_digit with 'B'.*/
  /* @TE2: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1553 */
  ASSERT_EQ(isD, true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1554
@Test_Description:    To test the functionality of hex digit test for C.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611517, 611157, 611153
*******************************************************************************/
TEST(minijson_reader, IsHexDigitTestForC)
{
  bool isA = ara::core::configreader::detail::IsHexDigit('c');
  /* @Start: UT_CORE_1554 */
  /* @TS1: Invoke the is_hex_digit with 'c'.*/
  /* @TE1: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1554 */
  ASSERT_EQ(isA, true);

  bool isD = ara::core::configreader::detail::IsHexDigit('C');
  /* @Start: UT_CORE_1554 */
  /* @TS2: Invoke the is_hex_digit with 'C'.*/
  /* @TE2: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1554 */
  ASSERT_EQ(isD, true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1555
@Test_Description:    To test the functionality of hex digit test for D.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611153
*******************************************************************************/
TEST(minijson_reader, IsHexDigitTestForD)
{
  bool isB = ara::core::configreader::detail::IsHexDigit('d');
  /* @Start: UT_CORE_1555 */
  /* @TS1: Invoke the is_hex_digit with 'd'.*/
  /* @TE1: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1555 */
  ASSERT_EQ(isB, true);

  bool isA = ara::core::configreader::detail::IsHexDigit('D');
  /* @Start: UT_CORE_1555 */
  /* @TS2: Invoke the is_hex_digit with 'D'.*/
  /* @TE2: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1555 */
  ASSERT_EQ(isA, true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1556
@Test_Description:    To test the functionality of hex digit test for E.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611153
*******************************************************************************/
TEST(minijson_reader, IsHexDigitTestForE)
{
  bool isD = ara::core::configreader::detail::IsHexDigit('e');
  /* @Start: UT_CORE_1556 */
  /* @TS1: Invoke the is_hex_digit with 'e'.*/
  /* @TE1: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1556 */
  ASSERT_EQ(isD, true);

  bool isE = ara::core::configreader::detail::IsHexDigit('E');
  /* @Start: UT_CORE_1556 */
  /* @TS2: Invoke the is_hex_digit with 'E'.*/
  /* @TE2: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1556 */
  ASSERT_EQ(isE, true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1557
@Test_Description:    To test the functionality of hex digit test for F.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611153
*******************************************************************************/
TEST(minijson_reader, IsHexDigitTestForF)
{
  bool isD = ara::core::configreader::detail::IsHexDigit('f');
  /* @Start: UT_CORE_1557 */
  /* @TS1: Invoke the is_hex_digit with 'f'.*/
  /* @TE1: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1557 */
  ASSERT_EQ(isD, true);

  bool isB = ara::core::configreader::detail::IsHexDigit('F');
  /* @Start: UT_CORE_1557 */
  /* @TS2: Invoke the is_hex_digit with 'F'.*/
  /* @TE2: is_hex_digit should equal to true. */
  /* @End: UT_CORE_1557 */
  ASSERT_EQ(isB, true);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_1558
@Test_Description:    To test the functionality of operator overloading test 2.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611145
*******************************************************************************/
TEST(minijson_reader, OperatorOverloadingTest2)
{
  ara::core::configreader::ParseError::ErrorReason expectedReason1
      = (ara::core::configreader::ParseError::kExpectedOpeningQuote);
  /* @Start: UT_CORE_1558 */
  /* @TS1: Get value of kExpectedOpeningQuote. */
  /* @TE1: expectedReason1 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason1);

  ara::core::configreader::ParseError::ErrorReason expectedReason2
      = (ara::core::configreader::ParseError::kExpectedUtf16LowSurrogate);
  /* @Start: UT_CORE_1558 */
  /* @TS2: Get value of kExpectedUtf16LowSurrogate. */
  /* @TE2: expectedReason2 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason2);

  ara::core::configreader::ParseError::ErrorReason expectedReason3
      = (ara::core::configreader::ParseError::kInvalidEscapeSequence);
  /* @Start: UT_CORE_1558 */
  /* @TS3: Get value of kInvalidEscapeSequence. */
  /* @TE3: expectedReason3 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason3);

  ara::core::configreader::ParseError::ErrorReason expectedReason4
      = (ara::core::configreader::ParseError::kInvalidUtf16Character);
  /* @Start: UT_CORE_1558 */
  /* @TS4: Get value of kInvalidUtf16Character. */
  /* @TE4: expectedReason4 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason4);

  ara::core::configreader::ParseError::ErrorReason expectedReason5
      = (ara::core::configreader::ParseError::kInvalidValue);
  /* @Start: UT_CORE_1558 */
  /* @TS5: Get value of kInvalidValue. */
  /* @TE5: expectedReason5 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason5);

  ara::core::configreader::ParseError::ErrorReason expectedReason6
      = (ara::core::configreader::ParseError::kUnterminatedValue);
  /* @Start: UT_CORE_1558 */
  /* @TS6: Get value of kUnterminatedValue. */
  /* @TE6: expectedReason6 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason6);

  ara::core::configreader::ParseError::ErrorReason expectedReason7
      = (ara::core::configreader::ParseError::kExpectedOpeningBracket);
  /* @Start: UT_CORE_1558 */
  /* @TS7: Get value of kExpectedOpeningBracket. */
  /* @TE7: expectedReason7 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason7);

  ara::core::configreader::ParseError::ErrorReason expectedReason8
      = (ara::core::configreader::ParseError::kExpectedColon);
  /* @Start: UT_CORE_1558 */
  /* @TS8: Get value of kExpectedColon. */
  /* @TE8: expectedReason8 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason8);

  ara::core::configreader::ParseError::ErrorReason expectedReason9
      = (ara::core::configreader::ParseError::kExpectedCommaOrClosingBracket);
  /* @Start: UT_CORE_1558 */
  /* @TS9: Get value of kExpectedCommaOrClosingBracket. */
  /* @TE9: expectedReason9 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason9);

  ara::core::configreader::ParseError::ErrorReason expectedReason10
      = (ara::core::configreader::ParseError::kNestedObjectOrArrayNotParsed);
  /* @Start: UT_CORE_1558 */
  /* @TS10: Get value of kNestedObjectOrArrayNotParsed. */
  /* @TE10: expectedReason10 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason10);

  ara::core::configreader::ParseError::ErrorReason expectedReason11
      = (ara::core::configreader::ParseError::kExpectedNestingLimit);
  /* @Start: UT_CORE_1558 */
  /* @TS11: Get value of kExpectedNestingLimit. */
  /* @TE11: expectedReason11 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason11);

  ara::core::configreader::ParseError::ErrorReason expectedReason12
      = (ara::core::configreader::ParseError::kNullUtf16Character);
  /* @Start: UT_CORE_1558 */
  /* @TS12: Get value of kNullUtf16Character. */
  /* @TE12: expectedReason12 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason12);

  ara::core::configreader::ParseError::ErrorReason expectedReason13
      = (ara::core::configreader::ParseError::kExpectedValue);
  /* @Start: UT_CORE_1558 */
  /* @TS13: Get value of kExpectedValue. */
  /* @TE13: expectedReason13 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason13);

  ara::core::configreader::ParseError::ErrorReason expectedReason14
      = (ara::core::configreader::ParseError::kUnescapedControlCharacter);
  /* @Start: UT_CORE_1558 */
  /* @TS14: Get value of kUnescapedControlCharacter. */
  /* @TE14: expectedReason14 should not throw the exceptions. */
  /* @End: UT_CORE_1558 */
  EXPECT_NO_THROW(std::cout << expectedReason14);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_1559
@Test_Description:    To test the functionality of Dispatcher methods test.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          minijson_reader
@Requirement Tracing: -
@Design Tracing:      611141, 611136, 611135, 611133, 611132, 611131, 611129,
                      611128
*******************************************************************************/
TEST(minijson_reader, DispatcherMethodsTest)
{
  std::tuple<int> m_handlers{1};
  ara::core::configreader::Dispatcher<int> obj1(1);
  std::tuple<int> a = obj1.Handlers();
  int x = std::get<0>(m_handlers);
  int y = std::get<0>(a);
  /* @Start: UT_CORE_1559 */
  /* @TS1: Invoke the Dispatcher class .*/
  /* @TE1: m_handlers should equal to Dispatcher handler. */
  /* @End: UT_CORE_1559 */
  ASSERT_EQ(x, y);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
