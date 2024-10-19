// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_byte.cpp
//
// Purpose     : This file provides the unit test cases of class Byte.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/utility.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <type_traits>
#include <new>

using ara::core::Byte;

using ::testing::Eq;
using ::testing::Ne;

/*******************************************************************************
@TestCaseId:          UT_CORE_0020
@Test_Description:    To test the functionality of Static Type Properties of Byte.
@Test_Type:           Fault Injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ByteTest
@Requirement Tracing: -
@Design Tracing:      611303
*******************************************************************************/
TEST(ByteTest, StaticTypeProperties)
{
  // "is not an integral type"
  /* @Start: UT_CORE_0020 */
  /* @TS1: Get the value of std::is_integral<Byte> of Byte. */
  /* @TE1: std::is_integral<Byte> should return false. */
  /* @End: UT_CORE_0020 */
  EXPECT_FALSE(std::is_integral<Byte>::value);

  // "is not a character type"
  /* @Start: UT_CORE_0020 */
  /* @TS2: Get the value of std::is_same<Byte, char> of Byte.. */
  /* @TE2: std::is_same<Byte, char> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isChar = std::is_same<Byte, char>::value;
  EXPECT_FALSE(isChar);

  /* @Start: UT_CORE_0020 */
  /* @TS3: Get the value of std::is_same<Byte, signed char> of Byte.  */
  /* @TE3: std::is_same<Byte, signed char> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isSignedChar = std::is_same<Byte, signed char>::value;
  EXPECT_FALSE(isSignedChar);

  /* @Start: UT_CORE_0020 */
  /* @TS4: Get the value of std::is_same<Byte, unsigned char> of Byte. */
  /* @TE4: std::is_same<Byte, signed char> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isUnsignedChar = std::is_same<Byte, unsigned char>::value;
  EXPECT_FALSE(isUnsignedChar);

  /* @Start: UT_CORE_0020 */
  /* @TS5: Get the value of std::is_same<Byte, wchar_t> of Byte. */
  /* @TE5: std::is_same<Byte, wchar_t> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isWchar = std::is_same<Byte, wchar_t>::value;
  EXPECT_FALSE(isWchar);

#ifdef __cpp_unicode_characters
  /* @Start: UT_CORE_0020 */
  /* @TS6: Get th value of std::is_same<Byte, char32_t> of Byte.*/
  /* @TE6: std::is_same<Byte, char32_t> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isChar32 = std::is_same<Byte, char32_t>::value;
  EXPECT_FALSE(isChar32);

  /* @Start: UT_CORE_0020 */
  /* @TS7: Get the value of std::is_same<Byte, char16_t> of Byte. */
  /* @TE7: std::is_same<Byte, char16_t> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isChar16 = std::is_same<Byte, char16_t>::value;
  EXPECT_FALSE(isChar16);
#endif
#ifdef __cpp_char8_t
  /* @Start: UT_CORE_0020 */
  /* @TS8: Get the value of std::is_same<Byte, char8_t> of Byte. */
  /* @TE8: std::is_same<Byte, char8_t> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isChar8 = std::is_same<Byte, char8_t>::value;
  EXPECT_FALSE(isChar8);
#endif

  // "not implicitly convertible from any other type, including unsigned char"
  /* @Start: UT_CORE_0020 */
  /* @TS9: Get the value of std::is_convertible<char, Byte> of Byte. */
  /* @TE9: std::is_convertible<char, Byte> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleFromChar = std::is_convertible<char, Byte>::value;
  EXPECT_FALSE(isConvertibleFromChar);

  /* @Start: UT_CORE_0020 */
  /* @TS10: Get the value of std::is_convertible<unsigned char, Byte> of Byte. */
  /* @TE10: std::is_convertible<unsigned char, Byte> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleFromUnsignedChar = std::is_convertible<unsigned char, Byte>::value;
  EXPECT_FALSE(isConvertibleFromUnsignedChar);

  /* @Start: UT_CORE_0020 */
  /* @TS11: Get the value of std::is_convertible<signed char, Byte> of Byte. */
  /* @TE11: std::is_convertible<signed char, Byte> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleFromSignedChar = std::is_convertible<signed char, Byte>::value;
  EXPECT_FALSE(isConvertibleFromSignedChar);

  /* @Start: UT_CORE_0020 */
  /* @TS12: Get the value of std::is_convertible<int, Byte> of Byte. */
  /* @TE12: std::is_convertible<int, Byte> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleFromInt = std::is_convertible<int, Byte>::value;
  EXPECT_FALSE(isConvertibleFromInt);

  /* @Start: UT_CORE_0020 */
  /* @TS13: Get the value of std::is_convertible<unsigned, Byte> of Byte. */
  /* @TE13: std::is_convertible<unsigned, Byte> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleFromUnsignedInt = std::is_convertible<unsigned, Byte>::value;
  EXPECT_FALSE(isConvertibleFromUnsignedInt);

  // TODO: not yet in the spec: triviality:
  //       (triviality is not really a requirement, though - only a strong recommendation)

  /* @Start: UT_CORE_0020 */
  /* @TS14: Get the value of std::is_trivial<Byte> of Byte. */
  /* @TE14: std::is_trivial<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_trivial<Byte>::value);

  /* @Start: UT_CORE_0020 */
  /* @TS15: Get the value of std::is_trivially_default_constructible<Byte> of Byte. */
  /* @TE15: std::is_trivially_default_constructible<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_trivially_default_constructible<Byte>::value);

  /* @Start: UT_CORE_0020 */
  /* @TS16: Get the value of std::is_trivially_copy_constructible<Byte> of Byte. */
  /* @TE16: std::is_trivially_copy_constructible<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_trivially_copy_constructible<Byte>::value);
  /* @Start: UT_CORE_0020 */
  /* @TS17: Get the value of std::is_trivially_move_constructible<Byte> of Byte. */
  /* @TE17: std::is_trivially_move_constructible<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_trivially_move_constructible<Byte>::value);
  /* @Start: UT_CORE_0020 */
  /* @TS18: Get the value of std::is_trivially_copy_assignable<Byte> of Byte. */
  /* @TE18: std::is_trivially_copy_assignable<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_trivially_copy_assignable<Byte>::value);
  /* @Start: UT_CORE_0020 */
  /* @TS19: Get the value of std::is_trivially_move_assignable<Byte> of Byte. */
  /* @TE19: std::is_trivially_move_assignable<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_trivially_move_assignable<Byte>::value);

  /* @Start: UT_CORE_0020 */
  /* @TS20: Get the value of std::is_trivially_copyable<Byte> of Byte. */
  /* @TE20: std::is_trivially_copyable<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_trivially_copyable<Byte>::value); // with memcpy() etc.

  /* @Start: UT_CORE_0020 */
  /* @TS21: Get the value of std::is_standard_layout<Byte> of Byte. */
  /* @TE21: std::is_standard_layout<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_standard_layout<Byte>::value); // fully compatible with C

  // TODO: not yet in the spec: nothrow:
  /* @Start: UT_CORE_0020 */
  /* @TS22: Get the value of std::is_nothrow_default_constructible<Byte> of Byte. */
  /* @TE22: std::is_nothrow_default_constructible<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_nothrow_default_constructible<Byte>::value);
  /* @Start: UT_CORE_0020 */
  /* @TS23: Get the value of std::is_nothrow_copy_constructible<Byte> of Byte. */
  /* @TE23: std::is_nothrow_copy_constructible<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_nothrow_copy_constructible<Byte>::value);
  /* @Start: UT_CORE_0020 */
  /* @TS24: Get the value of std::is_nothrow_move_constructible<Byte> of Byte. */
  /* @TE24: std::is_nothrow_move_constructible<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_nothrow_move_constructible<Byte>::value);
  /* @Start: UT_CORE_0020 */
  /* @TS25: Get the value of std::is_nothrow_copy_assignable<Byte> of Byte. */
  /* @TE25: std::is_nothrow_copy_assignable<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_nothrow_copy_assignable<Byte>::value);
  /* @Start: UT_CORE_0020 */
  /* @TS26: Get the value of std::is_nothrow_move_assignable<Byte> of Byte. */
  /* @TE26: std::is_nothrow_move_assignable<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_nothrow_move_assignable<Byte>::value);
  /* @Start: UT_CORE_0020 */
  /* @TS27: Get the value of std::is_nothrow_destructible<Byte> of Byte. */
  /* @TE27: std::is_nothrow_destructible<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_nothrow_destructible<Byte>::value);
#if __cplusplus >= 201703L
  /* @Start: UT_CORE_0020 */
  /* @TS28: Get the value of std::is_nothrow_swappable<Byte> of Byte. */
  /* @TE28: std::is_nothrow_swappable<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_nothrow_swappable<Byte>::value);
#endif

  // "destructor is trivial"
  /* @Start: UT_CORE_0020 */
  /* @TS29: Get the value of std::is_trivially_destructible<Byte> of Byte. */
  /* @TE29: std::is_trivially_destructible<Byte> should return true. */
  /* @End: UT_CORE_0020 */
  EXPECT_TRUE(std::is_trivially_destructible<Byte>::value);

  // "is not implicitly convertible to any other type, including bool"
  // Obviously, we cannot check this for "any" type, so we only check for some obvious candidates.

  /* @Start: UT_CORE_0020 */
  /* @TS30: Get the value of std::is_convertible<Byte, bool> of Byte. */
  /* @TE30: std::is_convertible<Byte, bool> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleToBool = std::is_convertible<Byte, bool>::value;
  EXPECT_FALSE(isConvertibleToBool);

  /* @Start: UT_CORE_0020 */
  /* @TS31: Get the value of std::is_convertible<Byte, char> of Byte. */
  /* @TE31: std::is_convertible<Byte, char> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleToChar = std::is_convertible<Byte, char>::value;
  EXPECT_FALSE(isConvertibleToChar);

  /* @Start: UT_CORE_0020 */
  /* @TS32: Get the value of std::is_convertible<Byte, unsigned char> of Byte. */
  /* @TE32: std::is_convertible<Byte, unsigned char> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleToUnsignedChar = std::is_convertible<Byte, unsigned char>::value;
  EXPECT_FALSE(isConvertibleToUnsignedChar);

  /* @Start: UT_CORE_0020 */
  /* @TS33: Get the value of std::is_convertible<Byte, signed char> of Byte. */
  /* @TE33: std::is_convertible<Byte, signed char> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleToSignedChar = std::is_convertible<Byte, signed char>::value;
  EXPECT_FALSE(isConvertibleToSignedChar);

  /* @Start: UT_CORE_0020 */
  /* @TS34: Get the value of std::is_convertible<Byte, int> of Byte. */
  /* @TE34: std::is_convertible<Byte, int> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleToInt = std::is_convertible<Byte, int>::value;
  EXPECT_FALSE(isConvertibleToInt);

  /* @Start: UT_CORE_0020 */
  /* @TS35: Get the value of std::is_convertible<Byte, unsigned> of Byte. */
  /* @TE35: std::is_convertible<Byte, unsigned> should return false. */
  /* @End: UT_CORE_0020 */
  constexpr bool isConvertibleToUnsignedInt = std::is_convertible<Byte, unsigned>::value;
  EXPECT_FALSE(isConvertibleToUnsignedInt);

  /* @Start: UT_CORE_0020 */
  /* @TS36: Get the value of sizeof(Byte). */
  /* @TE36: sizeof(Byte) should return 1. */
  /* @End: UT_CORE_0020 */

  // "size of an instance shall be 1"
  EXPECT_TRUE(sizeof(Byte) == 1);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0021
@Test_Description:    To test the functionality of a Conversion To UnsignedChar of Byte.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ByteTest
@Requirement Tracing: -
@Design Tracing:      611304, 611305
*******************************************************************************/
TEST(ByteTest, ConversionToUnsignedChar)
{
  /* @Start: UT_CORE_0021 */
  /* @TS1: Static cast the Byte class into the unsigned char type. */
  /* @TE1: It should return the value same as Byte object. */
  /* @End: UT_CORE_0021 */
  constexpr Byte b{42};
  // Assignment to unsigned char via static_cast<> shall be possible:
  constexpr unsigned char ch = static_cast<unsigned char>(b);
  EXPECT_THAT(ch, Eq(42));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0022
@Test_Description:    To test the functionality of HasValue of Byte.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ByteTest
@Requirement Tracing: -
@Design Tracing:      611304
*******************************************************************************/
TEST(ByteTest, BraceInitialize_HasValue)
{
  /* @Start: UT_CORE_0022 */
  /* @TS1: Convert the Byte into an unsigned char type. */
  /* @TE1: It should return the value as 0. */
  /* @End: UT_CORE_0022 */
  constexpr Byte b{};
  EXPECT_THAT(static_cast<unsigned char>(b), Eq(0));

  /* @Start: UT_CORE_0022 */
  /* @TS2: Convert the Byte into an unsigned char type. */
  /* @TE2: It should return the value as 0. */
  /* @End: UT_CORE_0022 */
  constexpr Byte b0{0};
  EXPECT_THAT(static_cast<unsigned char>(b0), Eq(0));

  /* @Start: UT_CORE_0022 */
  /* @TS3: Convert the Byte into an unsigned char type. */
  /* @TE3: It should return the value as 1. */
  /* @End: UT_CORE_0022 */
  constexpr Byte b1{1};
  EXPECT_THAT(static_cast<unsigned char>(b1), Eq(1));

  /* @Start: UT_CORE_0022 */
  /* @TS4: Convert the Byte into an unsigned char type. */
  /* @TE4: It should return the value as 255. */
  /* @End: UT_CORE_0022 */
  constexpr Byte b255{255};
  EXPECT_THAT(static_cast<unsigned char>(b255), Eq(255));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0023
@Test_Description:    To test the functionality of Default Construction of Byte.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ByteTest
@Requirement Tracing: -
@Design Tracing:      611304
*******************************************************************************/
TEST(ByteTest, DefaultConstruction_HasIndeterminateValue)
{
  /* @Start: UT_CORE_0023 */
  /* @TS1: Create the pointer of Byte and point on the unsigned char buf . */
  /* @TE1: unsigned char buf should be have the value as 0x55.*/
  /* @End: UT_CORE_0023 */
  unsigned char buf = 0x55;
  Byte* b = new (&buf) Byte;
  static_cast<void>(b);
  // "default constructed instance shall have indeterminate value"
  EXPECT_THAT(buf, Eq(0x55));
}

// TODO: not yet in the spec
/*******************************************************************************
@TestCaseId:          UT_CORE_0024
@Test_Description:    To test the functionality of Value Initialization is Zero of Byte.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ByteTest
@Requirement Tracing: -
@Design Tracing:      611304
*******************************************************************************/
TEST(ByteTest, ValueInitialization_IsZero)
{
  /* @Start: UT_CORE_0024 */
  /* @TS1: Create the pointer of Byte() and point on the unsigned char buf . */
  /* @TE1: unsigned char buf should be have the value as 0.*/
  /* @End: UT_CORE_0024 */
  unsigned char buf = 0x55;
  Byte* b = new (&buf) Byte();
  static_cast<void>(b);
  EXPECT_THAT(buf, Eq(0));
}

// Return a temporary Byte instance, i.e. an rvalue
constexpr static Byte getByte() noexcept
{
  return Byte{1};
}

// TODO: not yet in the spec
/*******************************************************************************
@TestCaseId:          UT_CORE_0025
@Test_Description:    To test the functionality of Assignment operator of Byte.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ByteTest
@Requirement Tracing: -
@Design Tracing:      611940
*******************************************************************************/
TEST(ByteTest, Assignment)
{
  Byte b1{0x7f};
  Byte b2{0x01};

  b1 = b2;
  /* @Start: UT_CORE_0025 */
  /* @TS1: Create an instance of Byte as b1 and b2 and call the =operator. */
  /* @TE1: b1 should be equal to b2. */
  /* @End: UT_CORE_0025 */
  EXPECT_THAT(b1, Eq(b2));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0026
@Test_Description:    To test the functionality of Comparison of Byte.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ByteTest
@Requirement Tracing: -
@Design Tracing:      611941
*******************************************************************************/
TEST(ByteTest, Comparison)
{
  constexpr Byte b1{0};
  constexpr Byte b2{1};
  constexpr Byte b3{1};
  constexpr Byte b4{255};
  constexpr Byte b5{255};

  /* @Start: UT_CORE_0026 */
  /* @TS1: Create an instance of ara::core::Byte as b1,b2,b3,b4,b5 and compare the data. */
  /* @TE1: b1 should not be equal to b2. */
  /* @End: UT_CORE_0026 */
  EXPECT_THAT(b1, Ne(b2));
  /* @Start: UT_CORE_0026 */
  /* @TS2: Compare b1 and b4. */
  /* @TE2: b1 should not be equal to b4. */
  /* @End: UT_CORE_0026 */
  EXPECT_THAT(b1, Ne(b4));
  /* @Start: UT_CORE_0026 */
  /* @TS3: Compare b2 and b1. */
  /* @TE3: b2 should not be equal to b1. */
  /* @End: UT_CORE_0026 */
  EXPECT_THAT(b2, Ne(b1));
  /* @Start: UT_CORE_0026 */
  /* @TS4: Compare b2 and b2. */
  /* @TE4: b2 should be equal to b2. */
  /* @End: UT_CORE_0026 */
  EXPECT_THAT(b2, Eq(b2));
  /* @Start: UT_CORE_0026 */
  /* @TS5: Compare b2 and b3. */
  /* @TE5: b2 should be equal to b3. */
  /* @End: UT_CORE_0026 */
  EXPECT_THAT(b2, Eq(b3));
  /* @Start: UT_CORE_0026 */
  /* @TS6: Compare b2 and b4. */
  /* @TE6: b2 should not be equal to b4. */
  /* @End: UT_CORE_0026 */
  EXPECT_THAT(b2, Ne(b4));

  /* @Start: UT_CORE_0026 */
  /* @TS7: Compare b4 and b5. */
  /* @TE7: b4 should not be equal to b5. */
  /* @End: UT_CORE_0026 */
  EXPECT_THAT(b4, Eq(b5));
}

// This use case is taken from p0298r3
/*******************************************************************************
@TestCaseId:          UT_CORE_0027
@Test_Description:    To test the functionality of ==operator of Byte.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          ByteTest
@Requirement Tracing: -
@Design Tracing:      611940
*******************************************************************************/
TEST(ByteTest, CompareWithZero)
{
  constexpr Byte b1{0};
  constexpr Byte b2{1};

  constexpr Byte zero_Byte{0};
  constexpr bool b1EqualToZero = (b1 == zero_Byte);
  constexpr bool b2EqualToZero = (b2 == zero_Byte);
  /* @Start: UT_CORE_0027 */
  /* @TS1: Create an instance of Byte as b1,b2,zero_Byte and compare the instatnces. */
  /* @TE1: b1 should be equal to zero_Byte and b2 should not be be equal to zero_Byte. */
  /* @End: UT_CORE_0027 */
  EXPECT_TRUE(b1EqualToZero);
  EXPECT_FALSE(b2EqualToZero);
}