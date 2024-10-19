// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_string_view.cpp
//
// Purpose     : This file provides the unit test cases of class StringView.
// -----------------------------------------------------------------------------------------------------------

#include "gmock_matchers.h"

#include "ara/core/string_view.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <sstream>
#include <iomanip>
#include <cstring>

using ::testing::Eq;
using ::testing::Ge;
using ::testing::Gt;
using ::testing::IsEmpty;
using ::testing::IsNull;
using ::testing::Le;
using ::testing::Lt;
using ::testing::Not;
using ::testing::StartsWith;

using ara::core::StringView;

// Explicitly instantiate the StringView template, so that gcov instruments
// all member functions, even those that are never executed.
template class ara::core::internal::basic_string_view<char>;

// ------------------------------------------------------------------------
// Constructors and assignment

/*******************************************************************************
@TestCaseId:          UT_CORE_0201
@Test_Description:    To test the functionality of sv of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611556
*******************************************************************************/
TEST(StringViewTest, DefaultConstructor)
{
  StringView const sv;
  EXPECT_THAT(sv, HasSize(0));
  EXPECT_THAT(sv, IsEmpty());
  EXPECT_THAT(sv.data(), IsNull());

  /* @Start: UT_CORE_0201 */
  /* @TS1: Create an instance of StringView as sv. */
  /* @TE1: Instance sv should be created. */
  /* @TS2: Invoke the HasSize() of sv. */
  /* @TE2: HasSize() of sv should be 0. */
  /* @TS3: Invoke the IsEmpty() of sv. */
  /* @TE3: IsEmpty() of sv should be n Interface Testull. */
  /* @TS4: Invoke the data() of sv. */
  /* @TE4: Data() should be null. */
  /* @End: UT_CORE_0201 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0202
@Test_Description:    To test the functionality of Arraay with size of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611608, 611609, 611610, 611611, 611612, 611613
*******************************************************************************/
TEST(StringViewTest, ConstructFromArrayWithSize)
{
  char const text[] = "The Quick Brown Fox";
  size_t const totalLen = strlen(text);

  StringView const sv1(text, totalLen);
  EXPECT_THAT(sv1, HasSize(totalLen));
  EXPECT_THAT(sv1.data(), text);

  StringView const sv2(text, 3);
  EXPECT_THAT(sv2, HasSize(3));
  EXPECT_THAT(sv2.data(), text);

  char const textWithNuls[] = "The\0Quick\0Brown\0Fox";
  size_t const realLen = sizeof(textWithNuls) - 1; // discard trailing NUL
  StringView const sv3(text, realLen);
  EXPECT_THAT(sv3, HasSize(realLen));

  StringView const sv4(text, 0);
  EXPECT_THAT(sv4, HasSize(0));
  EXPECT_THAT(sv4, IsEmpty());
  EXPECT_THAT(sv4.data(), text);

  /* @Start: UT_CORE_0202 */
  /* @TS1: Create the instance of StringView sv1 and pass the char array and its length. */
  /* @TE1: StringView instance sv1 should be created.  */
  /* @TS2: Invoke the HasSize(). */
  /* @TE2: size of sv1 should be same as size of char array. */
  /* @TS3: Invoke the IsEmpty(). */
  /* @TE3: IsEmpty() should return true of StringView is empty. */
  /* @End: UT_CORE_0202 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0203
@Test_Description:    To test the functionality of Array without size of StringView.
@Test_Type:           Fault Injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611608, 611609, 611610, 611611, 611612, 611613
*******************************************************************************/
TEST(StringViewTest, ConstructFromArrayWithoutSize)
{
  char const text[] = "The Quick Brown Fox";
  size_t const totalLen = strlen(text);

  StringView const sv1(text);
  EXPECT_THAT(sv1, HasSize(totalLen));
  EXPECT_THAT(sv1.data(), text);

  StringView const sv2(nullptr);
  EXPECT_THAT(sv2, HasSize(0));
  EXPECT_THAT(sv2, IsEmpty());
  EXPECT_THAT(sv2.data(), IsNull());

  /* @Start: UT_CORE_0203 */
  /* @TS1: Create the instance of StringView sv1 and pass the char array. */
  /* @TE1: StringView instance sv1 should be created.  */
  /* @TS2: Invoke the HasSize(). */
  /* @TE2: size of sv1 should be same as size of char array. */
  /* @TS3: Invoke the IsEmpty(). */
  /* @TE3: IsEmpty() should return true. */
  /* @End: UT_CORE_0203 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0204
@Test_Description:    To test the functionality of copy constructor of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611561
*******************************************************************************/
TEST(StringViewTest, CopyConstructor)
{
  char const text[] = "The Quick Brown Fox";

  StringView const sv1(text);
  StringView const sv2(sv1);
  EXPECT_THAT(sv2, HasSize(sv1.size()));
  EXPECT_THAT(sv2.data(), sv1.data());

  /* @Start: UT_CORE_0204 */
  /* @TS1: Invoke the copy constructor of StringView sv1 by passing char array. */
  /* @TE1: StringView copy constructor should be invoked.  */
  /* @TS2: Invoke the copy constructor of StringView sv2 by passing sv1. */
  /* @TE2: StringView copy constructor should be invoked. */
  /* @TS3: Invoke the size() of sv1 and size() of sv2. */
  /* @TE3: size() of sv1 should be equal to size() of sv2.  */
  /* @TS4: Invoke the data() of sv1 and data() of sv2. */
  /* @TE4: data() of sv1 should be equal to data() of sv2.  */
  /* @End: UT_CORE_0204 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0205
@Test_Description:    To test the functionality of Assignment operator of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611922, 611924
*******************************************************************************/
TEST(StringViewTest, AssignmentOperator)
{
  char const text[] = "The Quick Brown Fox";

  StringView const sv1(text);
  StringView sv2;

  sv2 = sv1;
  EXPECT_THAT(sv2, HasSize(sv1.size()));
  EXPECT_THAT(sv2.data(), sv1.data());

  /* @Start: UT_CORE_0205 */
  /* @TS1: Invoke the copy constructor of StringView sv1 by passing char array. */
  /* @TE1: StringView copy constructor should be invoked.  */
  /* @TS2: Invoke the assignment operator of StringView. */
  /* @TE2: Assignment operator should copy the data from one object to another */
  /* @TS3: Invoke the size() of sv1 and sv2. */
  /* @TE3: size() of sv1 and sv2 should be same. */
  /* @End: UT_CORE_0205 */
}

// ------------------------------------------------------------------------
// Element access

/*******************************************************************************
@TestCaseId:          UT_CORE_0206
@Test_Description:    To test the functionality of Index operator of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611574
*******************************************************************************/
TEST(StringViewTest, IndexOperator)
{
  char const text[] = "The Quick Brown Fox";

  StringView const sv(text);
  EXPECT_THAT(sv[0], 'T');
  EXPECT_THAT(sv[1], 'h');
  EXPECT_THAT(sv[18], 'x');

  /* @Start: UT_CORE_0206 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to char array text. */
  /* @TE1: char array text should be created. */
  /* @TS2: Create an instance of StringView as sv and insert text in it. */
  /* @TE2: Instance sv should be created with the text inserted. */
  /* @TS3: Invoke the sv[0]. */
  /* @TE3: sv[0] should be 'T'.*/
  /* @TS4: Invoke the sv[1]. */
  /* @TE4: sv[1] should be 'h'.*/
  /* @TS5: Invoke the sv[18]. */
  /* @TE5: sv[18] should be 'x'.*/
  /* @End: UT_CORE_0206 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0207
@Test_Description:    To test the functionality of At() of StringView.
@Test_Type:           Fault Injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611575
*******************************************************************************/
TEST(StringViewTest, At)
{
  char const text[] = "The Quick Brown Fox";

  StringView const sv(text);
  EXPECT_THAT(sv.at(0), 'T');
  EXPECT_THAT(sv.at(1), 'h');
  EXPECT_THAT(sv.at(18), 'x');
  EXPECT_THROW(sv.at(19), std::out_of_range);

  /* @Start: UT_CORE_0207 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to char array text. */
  /* @TE1: char array text should be created. */
  /* @TS2: Create an instance of StringView as sv and insert text in it. */
  /* @TE2: Instance sv should be created with the text inserted. */
  /* @TS3: Invoke the at[0] of sv. */
  /* @TE3: at[0] of sv should be 'T'.*/
  /* @TS4: Invoke the at[1] of sv. */
  /* @TE4: at[1] of sv should be 'h'.*/
  /* @TS5: Invoke the at[18] of sv. */
  /* @TE5: at[18] of sv should be 'x'.*/
  /* @End: UT_CORE_0207 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0208
@Test_Description:    To test the functionality of front() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611576
*******************************************************************************/
TEST(StringViewTest, Front)
{
  char const text[] = "The Quick Brown Fox";

  StringView const sv(text + 4, 5);
  EXPECT_THAT(sv.front(), 'Q');

  /* @Start: UT_CORE_0208 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to char array text. */
  /* @TE1: char array text should be created. */
  /* @TS2: Create an instance of StringView as sv by passing array and size. */
  /* @TE2: Instance sv should be created. */
  /* @TS3: Invoke the front() of sv. */
  /* @TE3: front() of sv should return 'Q'. */
  /* @End: UT_CORE_0208 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0209
@Test_Description:    To test the functionality of back() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611577
*******************************************************************************/
TEST(StringViewTest, Back)
{
  char const text[] = "The Quick Brown Fox";

  StringView const sv(text + 4, 5);
  EXPECT_THAT(sv.back(), 'k');

  /* @Start: UT_CORE_0209 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to char array text. */
  /* @TE1: char array text should be created. */
  /* @TS2: Create an instance of StringView as sv by passing array and size. */
  /* @TE2: Instance sv should be created. */
  /* @TS3: Invoke the back() of sv. */
  /* @TE3: back() of sv should return 'k'. */
  /* @End: UT_CORE_0209 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0210
@Test_Description:    To test the functionality of data() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611578
*******************************************************************************/
TEST(StringViewTest, Data)
{
  char const text[] = "The Quick Brown Fox";
  size_t const textSize = strlen(text);

  StringView const sv(text + 4);
  EXPECT_THAT(sv.data(), StartsWith("Quick Brown Fox"));
  EXPECT_THAT(sv, HasSize(textSize - 4));

  /* @Start: UT_CORE_0210 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to char array text. */
  /* @TE1: char array text should be created. */
  /* @TS2: Create an instance of StringView as sv by passing array. */
  /* @TE2: Instance sv should be created. */
  /* @TS3: Invoke the data() of sv. */
  /* @TE3: data() of sv should be "Quick Brown Fox". */
  /* @End: UT_CORE_0210 */
}

// ------------------------------------------------------------------------
// Capacity

/*******************************************************************************
@TestCaseId:          UT_CORE_0211
@Test_Description:    To test the functionality of size() and length() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611570, 611571
*******************************************************************************/
TEST(StringViewTest, SizeAndLengthAreIdentical)
{
  char const text[] = "The Quick Brown Fox";

  StringView const sv1(text + 4);
  EXPECT_THAT(sv1, HasSize(sv1.length()));

  StringView const sv2;
  EXPECT_THAT(sv2, HasSize(0));

  /* @Start: UT_CORE_0211 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to char array text. */
  /* @TE1: char array text should be created. */
  /* @TS2: Create an instance of StringView as sv1 by passing array. */
  /* @TE2: Instance sv1 should be created. */
  /* @TS3: Invoke the length() of sv1. */
  /* @TE3: length() of sv1 should be equal to size of char array. */
  /* @End: UT_CORE_0211 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0212
@Test_Description:    To test the functionality of Empty() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611573
*******************************************************************************/
TEST(StringViewTest, Empty)
{
  char const text[] = "The Quick Brown Fox";

  StringView const sv1(text);
  EXPECT_THAT(sv1, Not(IsEmpty()));

  StringView const sv2;
  EXPECT_THAT(sv2, IsEmpty());
  /* @Start: UT_CORE_0212 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to text. */
  /* @TE1: Parameter "The Quick Brown Fox" should be passed to text. */
  /* @TS2: Create an instance of StringView as sv1 and insert text in it. */
  /* @TE2: Instance sv1 should be created by inserted text, textsize. */
  /* @TS3: Create an instance of StringView as sv2. */
  /* @TE3: Instance sv2 should be created. */
  /* @TS4: Invoke the Not of sv1. */
  /* @TE4: Not of sv1 should be IsEmpty. */
  /* @TS5: Invoke the empty parameter of sv2. */
  /* @TE5: Empty parameter of sv2 should be null. */
  /* @End: UT_CORE_0212 */
}

// ------------------------------------------------------------------------
// Modifiers

/*******************************************************************************
@TestCaseId:          UT_CORE_0213
@Test_Description:    To test the functionality of remove_prefix of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611579
*******************************************************************************/
TEST(StringViewTest, RemovePrefix)
{
  char const text[] = "The Quick Brown Fox\xAA";
  size_t const textSize = strlen(text) - 1;

  StringView sv(text, textSize);
  sv.remove_prefix(4);
  EXPECT_THAT(sv.data(), StartsWith("Quick Brown Fox"));
  EXPECT_THAT(sv, HasSize(textSize - 4));

  /* @Start: UT_CORE_0213 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to text. */
  /* @TE1: Parameter "The Quick Brown Fox" should be passed to text. */
  /* @TS2: Assign strlen(text) - 1 to textSize. */
  /* @TE2: strlen(text) - 1 should be assigned to textSize. */
  /* @TS3: Create an instance of StringView as sv and insert text, textSize. */
  /* @TE3: Instance sv should be created by inserted text, textsize. */
  /* @TS4: Assign remove_prefix(4) to sv. */
  /* @TE4: remove_prefix(4) should be assigned to sv. */
  /* @TS5: Invoke the data of sv. */
  /* @TE5: Data of sv should be StartsWith("Quick Brown Fox"). */
  /* @TS6: Invoke the HasSize of sv. */
  /* @TE6: HasSize of sv should be textSize by decreasing 4. */
  /* @End: UT_CORE_0213 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0214
@Test_Description:    To test the functionality of remove_suffix of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611580
*******************************************************************************/
TEST(StringViewTest, RemoveSuffix)
{
  char const text[] = "The Quick Brown Fox\xAA";
  size_t const textSize = strlen(text) - 1;

  StringView sv(text, textSize);
  sv.remove_suffix(4);
  EXPECT_THAT(sv.data(), StartsWith("The Quick Brown Fox"));
  EXPECT_THAT(sv, HasSize(textSize - 4));

  /* @Start: UT_CORE_0214 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to text. */
  /* @TE1: Parameter "The Quick Brown Fox" should be passed to text. */
  /* @TS2: Assign strlen(text) - 1 to textSize. */
  /* @TE2: strlen(text) - 1 should be assigned to textSize. */
  /* @TS3: Create an instance of StringView as sv and insert text, textSize. */
  /* @TE3: Instance sv should be created by inserted text, textsize. */
  /* @TS4: Assign remove_suffix(4) to sv. */
  /* @TE4: Remove_prefix(4) should be assigned to sv. */
  /* @TS5: Invoke the data of sv. */
  /* @TE5: Data of sv should be StartsWith("The Quick Brown Fox"). */
  /* @TS6: Invoke the HasSize of sv. */
  /* @TE6: HasSize of sv should be textSize by decreasing 4. */
  /* @End: UT_CORE_0214 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0215
@Test_Description:    To test the functionality of swap of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611581
*******************************************************************************/
TEST(StringViewTest, Swap)
{
  char const text[] = "The Quick Brown Fox";
  size_t const textSize = strlen(text);

  StringView sv1(text);
  StringView sv2;

  sv2.swap(sv1);
  EXPECT_THAT(sv2, HasSize(textSize));
  EXPECT_THAT(sv2, Not(IsEmpty()));
  EXPECT_THAT(sv2.data(), text);
  EXPECT_THAT(sv1, HasSize(0));
  EXPECT_THAT(sv1, IsEmpty());
  EXPECT_THAT(sv1.data(), nullptr);

  /* @Start: UT_CORE_0215 */
  /* @TS1: Invoke the swap() of StringView class. */
  /* @TE1: swap() should exchange the data between to StringView objects. */
  /* @End: UT_CORE_0215 */
}

// ------------------------------------------------------------------------
// Operations

/*******************************************************************************
@TestCaseId:          UT_CORE_0216
@Test_Description:    To test the functionality of substr() of StringView.
@Test_Type:           Fault Injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611583
*******************************************************************************/
TEST(StringViewTest, Substr)
{
  char const text[] = "The Quick Brown Fox";
  StringView const sv(text);

  // count is huge, thus copying size()-pos==size()
  StringView sv1 = sv.substr(0);
  EXPECT_THAT(sv1.data(), text);
  EXPECT_THAT(sv1, HasSize(sv.size()));

  // count==1000, thus copying size()-size()==0
  StringView sv2 = sv.substr(sv.size(), 1000);
  EXPECT_THAT(sv2, IsEmpty());

  // count==14 < size()-4[=19-4=15], thus copying 14
  StringView sv3 = sv.substr(4, 14);
  EXPECT_THAT(sv3, HasSize(14));
  EXPECT_THAT(sv3.data(), StartsWith("Quick Brown Fo"));

  // count==15 <= size()-4[=19-4=15], thus copying 15
  StringView sv4 = sv.substr(4, 15);
  EXPECT_THAT(sv4, HasSize(15));
  EXPECT_THAT(sv4.data(), StartsWith("Quick Brown Fox"));

  // count==16 > size()-4[=19-4=15], thus copying 15
  StringView sv5 = sv.substr(4, 16);
  EXPECT_THAT(sv5, HasSize(15));
  EXPECT_THAT(sv5.data(), StartsWith("Quick Brown Fox"));

  EXPECT_THROW(sv.substr(sv.size() + 1), std::out_of_range);

  /* @Start: UT_CORE_0216 */
  /* @TS1: Pass the parameter "The Quick Brown Fox" to text. */
  /* @TE1: Parameter "The Quick Brown Fox" should be passed to text. */
  /* @TS2: Create an instance of StringView as sv1 and insert text in it. */
  /* @TE2: Instance sv1 should be created with text inserted. */
  /* @TS3: Create an instance of StringView as sv1 and assign sv.substr(0) to it. */
  /* @TE3: Instance sv1 should be created with assigned sv.substr(0). */
  /* @TS4: Invoke the data() of sv1. */
  /* @TE4: data of sv1 should be text. */
  /* @TS5: Invoke the HasSize() of sv1. */
  /* @TE5: HasSize() of sv1 should be sv.size(). */
  /* @TS6: Create an instance of StringView as sv2 and assign sv.substr(sv.size(), 1000) to  it. */
  /* @TE6: Instance sv2 should be created with assigned sv.substr(sv.size(), 1000). */
  /* @TS7: Invoke the IsEmpty() of sv2. */
  /* @TE7: IsEmpty() of sv2 should be null. */
  /* @TS8: Create an instance of StringView as sv3 and assign sv.substr(4, 14) to it. */
  /* @TE8: Instance sv3 should be created with assigned sv.substr(4, 14). */
  /* @TS9: Invoke the HasSize() of sv3. */
  /* @TE9: HasSize() of sv3 should be 14. */
  /* @TS10: Invoke the data() of sv3. */
  /* @TE10: Data of sv3 should be StartsWith("Quick Brown Fo"). */
  /* @TS11: Create an instance of StringView as sv4 and assign sv.substr(4, 15) to it. */
  /* @TE11: Instance sv3 should be created with assigned sv.substr(4, 15). */
  /* @TS12: Invoke the HasSize of sv4. */
  /* @TE12: HasSize of sv4 should be 15. */
  /* @TS13: Invoke the data() of sv4. */
  /* @TE13: Data of sv4 should be StartsWith("Quick Brown Fox"). */
  /* @TS14: Create an instance of StringView as sv5 and assign sv.substr(4, 16) to it. */
  /* @TE14: Instance sv2 should be created with assigned sv.substr(4, 16). */
  /* @TS15: Invoke the HasSize() of sv5. */
  /* @TE15: size of sv5 should be 16. */
  /* @TS16: Invoke the data() of sv5. */
  /* @TE16: Data of sv5 should be StartsWith("Quick Brown Fox"). */
  /* @TS17: Invoke the substr(sv.size() + 1) of sv. */
  /* @TE17: substr(sv.size() + 1) of sv should be out_of_range. */
  /* @End: UT_CORE_0216 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0217
@Test_Description:    To test the functionality of compare of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611584
*******************************************************************************/
TEST(StringViewTest, compare)
{
  // testing:
  // int compare(SizeType pos1, SizeType count1, basic_string_view v) const;
  // int compare(SizeType pos1, SizeType count1, basic_string_view v, SizeType pos2, SizeType count2) const;
  // int compare(CharT const* s) const;
  // int compare(SizeType pos1, SizeType count1, CharT const* s) const;
  // int compare(SizeType pos1, SizeType count1, CharT const* s, SizeType count2) const;

  StringView const sv6("aaabbb");

  StringView const xxx("bbxx");

  /* @Start: UT_CORE_0217 */
  /* @TS1: Invoke the compare() of StringView */
  /* @TE1: compare() should return 4. */
  /* @End: UT_CORE_0217 */
  int const res1 = sv6.compare(4, 2, xxx);
  EXPECT_THAT(res1, Lt(0));

  /* @Start: UT_CORE_0217 */
  /* @TS2: Invoke the find() of StringView */
  /* @TE2: find() should return 4. */
  /* @End: UT_CORE_0217 */
  int const res2 = sv6.compare(4, 2, xxx, 0, 2);
  EXPECT_THAT(res2, Eq(0));

  /* @Start: UT_CORE_0217 */
  /* @TS3: Invoke the find() of StringView */
  /* @TE3: find() should return 4. */
  /* @End: UT_CORE_0217 */
  int const res3 = sv6.compare("aaabbb");
  EXPECT_THAT(res3, Eq(0));

  /* @Start: UT_CORE_0217 */
  /* @TS4: Invoke the find() of StringView */
  /* @TE4: find() should return 4. */
  /* @End: UT_CORE_0217 */
  int const res4 = sv6.compare(4, 2, "bb");
  EXPECT_THAT(res4, Eq(0));

  /* @Start: UT_CORE_0217 */
  /* @TS5: Invoke the find() of StringView */
  /* @TE5: find() should return 4. */
  /* @End: UT_CORE_0217 */
  int const res5 = sv6.compare(4, 2, "bbxx", 2);
  EXPECT_THAT(res5, Eq(0));
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0218
@Test_Description:    To test the functionality of find() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611590, 611591, 611592, 611593
*******************************************************************************/
TEST(StringViewTest, FindStringView)
{
  // testing:
  // SizeType find(basic_string_view v, SizeType pos = 0) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  StringView const needle = sv.substr(4, 5);
  /* @Start: UT_CORE_0218 */
  /* @TS1: Invoke the find() of StringView */
  /* @TE1: find() should return 4. */
  /* @End: UT_CORE_0218 */
  size_t const offset1 = sv.find(needle);
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0218 */
  /* @TS2: Invoke the find() of StringView */
  /* @TE2: find() should return 4. */
  /* @End: UT_CORE_0218 */
  size_t const offset2 = sv.find(needle, 0);
  EXPECT_THAT(offset2, 4);

  /* @Start: UT_CORE_0218 */
  /* @TS3: Invoke the find() of StringView */
  /* @TE3: find() should return 4. */
  /* @End: UT_CORE_0218 */
  size_t const offset3 = sv.find(needle, 4);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0218 */
  /* @TS4: Invoke the find() of StringView */
  /* @TE4: find() should return 16*/
  /* @End: UT_CORE_0218 */
  size_t const offset4 = sv.find(needle, 5);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0218 */
  /* @TS5: Invoke the find() of StringView */
  /* @TE5: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0218 */
  size_t const offset5 = sv.find(needle, 17);
  EXPECT_THAT(offset5, StringView::stkNpos);

  /* @Start: UT_CORE_0218 */
  /* @TS6: Invoke the find() of StringView */
  /* @TE6: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0218 */
  size_t const offset6 = sv.find(needle, 1000);
  EXPECT_THAT(offset6, StringView::stkNpos);

  StringView const needle2;

  /* @Start: UT_CORE_0218 */
  /* @TS7: Invoke the find() of StringView */
  /* @TE7: find() should return 0. */
  /* @End: UT_CORE_0218 */
  // An empty string is found everywhere within the string...
  size_t const offset7 = sv.find(needle2, 0);
  EXPECT_THAT(offset7, 0);
  // ...except when the initial offset is outside of the string bounds.

  /* @Start: UT_CORE_0218 */
  /* @TS8: Invoke the find() of StringView */
  /* @TE8: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0218 */
  size_t const offset8 = sv.find(needle2, 1000);
  EXPECT_THAT(offset8, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0219
@Test_Description:    To test the functionality of find char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611591
*******************************************************************************/
TEST(StringViewTest, FindChar)
{
  // testing:
  // SizeType find(CharT ch, SizeType pos = 0) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  /* @Start: UT_CORE_0219 */
  /* @TS1: Invoke the find() of StringView */
  /* @TE1: find() should return 4. */
  /* @End: UT_CORE_0219 */
  size_t const offset1 = sv.find('Q');
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0219 */
  /* @TS2: Invoke the find() of StringView */
  /* @TE2: find() should return 4. */
  /* @End: UT_CORE_0219 */
  size_t const offset2 = sv.find('Q', 0);
  EXPECT_THAT(offset2, 4);

  /* @Start: UT_CORE_0219 */
  /* @TS3: Invoke the find() of StringView */
  /* @TE3: find() should return 4. */
  /* @End: UT_CORE_0219 */
  size_t const offset3 = sv.find('Q', 4);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0219 */
  /* @TS4: Invoke the find() of StringView */
  /* @TE4: find() should return 16. */
  /* @End: UT_CORE_0219 */
  size_t const offset4 = sv.find('Q', 5);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0219 */
  /* @TS5: Invoke the find() of StringView */
  /* @TE5: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0219 */
  size_t const offset5 = sv.find('Q', 17);
  EXPECT_THAT(offset5, StringView::stkNpos);

  /* @Start: UT_CORE_0219 */
  /* @TS6: Invoke the find() of StringView */
  /* @TE6: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0219 */
  size_t const offset6 = sv.find('Q', 1000);
  EXPECT_THAT(offset6, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0220
@Test_Description:    To test the functionality of sized char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611591
*******************************************************************************/
TEST(StringViewTest, FindSizedCharSegment)
{
  // testing:
  // SizeType find(CharT const* s, SizeType pos, SizeType count) const;

  StringView const sv = "The Quick Brown Quick";
  /* @Start: UT_CORE_0220 */
  /* @TS1: Invoke the find() of StringView */
  /* @TE1: find() should return 4. */
  /* @End: UT_CORE_0220 */
  size_t const offset1 = sv.find("Quick Yellow Beard", 0, 5);
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0220 */
  /* @TS2: Invoke the find() of StringView */
  /* @TE2: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0220 */
  size_t const offset2 = sv.find("Quick Yellow Beard", 0, 7);
  EXPECT_THAT(offset2, StringView::stkNpos);

  /* @Start: UT_CORE_0220 */
  /* @TS3: Invoke the find() of StringView */
  /* @TE3: find() should return 4. */
  /* @End: UT_CORE_0220 */
  size_t const offset3 = sv.find("Quick Yellow Beard", 4, 5);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0220 */
  /* @TS4: Invoke the find() of StringView */
  /* @TE4: find() should return 16. */
  /* @End: UT_CORE_0220 */
  size_t const offset4 = sv.find("Quick Yellow Beard", 5, 5);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0220 */
  /* @TS5: Invoke the find() of StringView */
  /* @TE5: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0220 */
  size_t const offset5 = sv.find("Quick Yellow Beard", 17, 5);
  EXPECT_THAT(offset5, StringView::stkNpos);

  /* @Start: UT_CORE_0220 */
  /* @TS6: Invoke the find() of StringView */
  /* @TE6: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0220 */
  size_t const offset6 = sv.find("Quick Yellow Beard", 1000, 5);
  EXPECT_THAT(offset6, StringView::stkNpos);

  char const needle[] = "The Quick Brown Quick Blah";
  size_t const needleSize = std::char_traits<char>::length(needle);

  /* @Start: UT_CORE_0220 */
  /* @TS7: Invoke the find() of StringView */
  /* @TE7: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0220 */
  size_t const offset7 = sv.find(needle, 0, needleSize);
  EXPECT_THAT(offset7, StringView::stkNpos);

  /* @Start: UT_CORE_0220 */
  /* @TS8: Invoke the find() of StringView */
  /* @TE8: find() should return 0. */
  /* @End: UT_CORE_0220 */
  // An empty string is found everywhere within the string...
  char const needle2[] = "";
  size_t const offset8 = sv.find(needle2, 0, 0);
  EXPECT_THAT(offset8, 0);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0221
@Test_Description:    To test the functionality of find Char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611556
*******************************************************************************/
TEST(StringViewTest, FindCharSegment)
{
  // testing:
  // SizeType find(CharT const* s, SizeType pos = 0) const;

  StringView const sv = "The Quick Brown Quick";

  /* @Start: UT_CORE_0221 */
  /* @TS1: Invoke the find() of StringView */
  /* @TE1: find() should return 4. */
  /* @End: UT_CORE_0221 */
  size_t const offset1 = sv.find("Quick");
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0221 */
  /* @TS2: Invoke the find() of StringView */
  /* @TE2: find() should return 4. */
  /* @End: UT_CORE_0221 */
  size_t const offset2 = sv.find("Quick", 0);
  EXPECT_THAT(offset2, 4);

  /* @Start: UT_CORE_0221 */
  /* @TS3: Invoke the find() of StringView */
  /* @TE3: find() should return 4. */
  /* @End: UT_CORE_0221 */
  size_t const offset3 = sv.find("Quick", 4);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0221 */
  /* @TS4: Invoke the find() of StringView */
  /* @TE4: find() should return 16. */
  /* @End: UT_CORE_0221 */
  size_t const offset4 = sv.find("Quick", 5);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0221 */
  /* @TS5: Invoke the find() of StringView */
  /* @TE5: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0221 */
  size_t const offset5 = sv.find("Quick", 17);
  EXPECT_THAT(offset5, StringView::stkNpos);

  /* @Start: UT_CORE_0221 */
  /* @TS6: Invoke the find() of StringView */
  /* @TE6: find() should return StringView::stkNpos. */
  /* @End: UT_CORE_0221 */
  size_t const offset6 = sv.find("Quick", 1000);
  EXPECT_THAT(offset6, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0222
@Test_Description:    To test the functionality of rfind() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611594, 611595, 611596, 611597
*******************************************************************************/
TEST(StringViewTest, RFindStringView)
{
  // testing:
  // SizeType rfind(basic_string_view v, SizeType pos = stkNpos) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  StringView const needle = "Quick";

  /* @Start: UT_CORE_0222 */
  /* @TS1: Invoke the rfind() of StringView */
  /* @TE1: rfind() should return 16. */
  /* @End: UT_CORE_0222 */
  size_t const offset1 = sv.rfind(needle);
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0222 */
  /* @TS2: Invoke the rfind() of StringView */
  /* @TE2: rfind() should return 16. */
  /* @End: UT_CORE_0222 */
  size_t const offset2 = sv.rfind(needle, StringView::stkNpos);
  EXPECT_THAT(offset2, 16);

  /* @Start: UT_CORE_0222 */
  /* @TS3: Invoke the rfind() of StringView */
  /* @TE3: rfind() should return 16. */
  /* @End: UT_CORE_0222 */
  size_t const offset3 = sv.rfind(needle, 21);
  EXPECT_THAT(offset3, 16);

  /* @Start: UT_CORE_0222 */
  /* @TS4: Invoke the rfind() of StringView */
  /* @TE4: rfind() should return 16. */
  /* @End: UT_CORE_0222 */
  size_t const offset4 = sv.rfind(needle, 17);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0222 */
  /* @TS5: Invoke the rfind() of StringView */
  /* @TE5: rfind() should return 16. */
  /* @End: UT_CORE_0222 */
  size_t const offset5 = sv.rfind(needle, 16);
  EXPECT_THAT(offset5, 16);

  /* @Start: UT_CORE_0222 */
  /* @TS6: Invoke the rfind() of StringView */
  /* @TE6: rfind() should return 4. */
  /* @End: UT_CORE_0222 */
  size_t const offset6 = sv.rfind(needle, 15);
  EXPECT_THAT(offset6, 4);

  /* @Start: UT_CORE_0222 */
  /* @TS7: Invoke the rfind() of StringView */
  /* @TE7: rfind() should return 16. */
  /* @End: UT_CORE_0222 */
  size_t const offset7 = sv.rfind(needle, 0);
  EXPECT_THAT(offset7, StringView::stkNpos);

  StringView const needle2 = "The Quick Brown QuickX";

  /* @Start: UT_CORE_0222 */
  /* @TS8: Invoke the rfind() of StringView */
  /* @TE8: rfind() should return StringView::stkNpos. */
  /* @End: UT_CORE_0222 */
  size_t const offset8 = sv.rfind(needle2, StringView::stkNpos);
  EXPECT_THAT(offset8, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0223
@Test_Description:    To test the functionality of rfind Char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611594, 611595, 611596, 611597
*******************************************************************************/
TEST(StringViewTest, RFindChar)
{
  // testing:
  // SizeType rfind(CharT c, SizeType pos = stkNpos) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  /* @Start: UT_CORE_0223 */
  /* @TS1: Invoke the rfind() of StringView */
  /* @TE1: rfind() should return 16. */
  /* @End: UT_CORE_0223 */
  size_t const offset1 = sv.rfind('Q');
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0223 */
  /* @TS2: Invoke the rfind() of StringView */
  /* @TE2: rfind() should return 16. */
  /* @End: UT_CORE_0223 */
  size_t const offset2 = sv.rfind('Q', StringView::stkNpos);
  EXPECT_THAT(offset2, 16);

  /* @Start: UT_CORE_0223 */
  /* @TS3: Invoke the rfind() of StringView. */
  /* @TE3: rfind() should return 16. */
  /* @End: UT_CORE_0223 */
  size_t const offset3 = sv.rfind('Q', 21);
  EXPECT_THAT(offset3, 16);

  /* @Start: UT_CORE_0223 */
  /* @TS4: Invoke the rfind() of StringView */
  /* @TE4: rfind() should return 4. */
  /* @End: UT_CORE_0223 */
  size_t const offset4 = sv.rfind('Q', 16);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0223 */
  /* @TS5: Invoke the rfind() of StringView */
  /* @TE5: rfind() should return 4. */
  /* @End: UT_CORE_0223 */
  size_t const offset5 = sv.rfind('Q', 15);
  EXPECT_THAT(offset5, 4);

  /* @Start: UT_CORE_0223 */
  /* @TS6: Invoke the rfind() of StringView */
  /* @TE6: rfind() should return StringView::stkNpos. */
  /* @End: UT_CORE_0223 */
  size_t const offset6 = sv.rfind('Q', 3);
  EXPECT_THAT(offset6, StringView::stkNpos);

  StringView const sv2;

  /* @Start: UT_CORE_0223 */
  /* @TS7: Invoke the rfind() of StringView */
  /* @TE7: rfind() should return StringView::stkNpos. */
  /* @End: UT_CORE_0223 */
  size_t const offset7 = sv2.rfind('Q', StringView::stkNpos);
  EXPECT_THAT(offset7, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0224
@Test_Description:    To test the functionality of rfind Sized Char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611594, 611595, 611596, 611597
*******************************************************************************/
TEST(StringViewTest, RFindSizedCharSegment)
{
  // testing:
  // SizeType rfind(CharT const* s, SizeType pos, SizeType count) const;

  StringView const sv = "The Quick Brown Quick";

  char const needle[] = "Quick Yellow Beard";

  /* @Start: UT_CORE_0224 */
  /* @TS1: Invoke the rfind() of StringView */
  /* @TE1: rfind() should return 16. */
  /* @End: UT_CORE_0224 */
  size_t const offset1 = sv.rfind(needle, StringView::stkNpos, 5);
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0224 */
  /* @TS2: Invoke the rfind() of StringView */
  /* @TE2: rfind() should return StringView::stkNpos. */
  /* @End: UT_CORE_0224 */
  size_t const offset2 = sv.rfind(needle, StringView::stkNpos, 7);
  EXPECT_THAT(offset2, StringView::stkNpos);

  /* @Start: UT_CORE_0224 */
  /* @TS3: Invoke the rfind() of StringView */
  /* @TE3: rfind() should return 16. */
  /* @End: UT_CORE_0224 */
  size_t const offset3 = sv.rfind(needle, 21, 5);
  EXPECT_THAT(offset3, 16);

  /* @Start: UT_CORE_0224 */
  /* @TS4: Invoke the rfind() of StringView */
  /* @TE4: rfind() should return 16. */
  /* @End: UT_CORE_0224 */
  size_t const offset4 = sv.rfind(needle, 17, 5);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0224 */
  /* @TS5: Invoke the rfind() of StringView */
  /* @TE5: rfind() should return 16. */
  /* @End: UT_CORE_0224 */
  size_t const offset5 = sv.rfind(needle, 16, 5);
  EXPECT_THAT(offset5, 16);

  /* @Start: UT_CORE_0224 */
  /* @TS6: Invoke the rfind() of StringView */
  /* @TE6: rfind() should return 4. */
  /* @End: UT_CORE_0224 */
  size_t const offset6 = sv.rfind(needle, 15, 5);
  EXPECT_THAT(offset6, 4);

  /* @Start: UT_CORE_0224 */
  /* @TS7: Invoke the rfind() of StringView */
  /* @TE7: rfind() should return StringView::stkNpos. */
  /* @End: UT_CORE_0224 */
  size_t const offset7 = sv.rfind(needle, 0, 5);
  EXPECT_THAT(offset7, StringView::stkNpos);

  char const needle2[] = "The Quick Brown QuickX";

  /* @Start: UT_CORE_0224 */
  /* @TS8: Invoke the rfind() of StringView */
  /* @TE8: rfind() should return 0. */
  /* @End: UT_CORE_0224 */
  size_t const offset8 = sv.rfind(needle2, StringView::stkNpos, 21);
  EXPECT_THAT(offset8, 0);

  /* @Start: UT_CORE_0224 */
  /* @TS9: Invoke the rfind() of StringView */
  /* @TE9: rfind() should return StringView::stkNpos. */
  /* @End: UT_CORE_0224 */
  size_t const offset9 = sv.rfind(needle2, StringView::stkNpos, 22);
  EXPECT_THAT(offset9, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0225
@Test_Description:    To test the functionality of rfind() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611594, 611595, 611596, 611597
*******************************************************************************/
TEST(StringViewTest, RFindCharSegment)
{
  // testing:
  // SizeType rfind(CharT const* s, SizeType pos = stkNpos) const;

  StringView const sv = "The Quick Brown Quick";

  char const needle[] = "Quick";

  /* @Start: UT_CORE_0225 */
  /* @TS1: Invoke the rfind() of StringView */
  /* @TE1: rfind() should return 16. */
  /* @End: UT_CORE_0225 */
  size_t const offset1 = sv.rfind(needle);
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0225 */
  /* @TS2: Invoke the rfind() of StringView */
  /* @TE2: rfind() should return 16. */
  /* @End: UT_CORE_0225 */
  size_t const offset2 = sv.rfind(needle, StringView::stkNpos);
  EXPECT_THAT(offset2, 16);

  /* @Start: UT_CORE_0225 */
  /* @TS3: Invoke the rfind() of StringView */
  /* @TE3: rfind() should return 16. */
  /* @End: UT_CORE_0225 */
  size_t const offset3 = sv.rfind(needle, 21);
  EXPECT_THAT(offset3, 16);

  /* @Start: UT_CORE_0225 */
  /* @TS4: Invoke the rfind() of StringView */
  /* @TE4: rfind() should return 16. */
  /* @End: UT_CORE_0225 */
  size_t const offset4 = sv.rfind(needle, 17);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0225 */
  /* @TS5: Invoke the rfind() of StringView */
  /* @TE5: rfind() should return 16. */
  /* @End: UT_CORE_0225 */
  size_t const offset5 = sv.rfind(needle, 16);
  EXPECT_THAT(offset5, 16);

  /* @Start: UT_CORE_0225 */
  /* @TS6: Invoke the rfind() of StringView */
  /* @TE6: rfind() should return 4. */
  /* @End: UT_CORE_0225 */
  size_t const offset6 = sv.rfind(needle, 15);
  EXPECT_THAT(offset6, 4);

  /* @Start: UT_CORE_0225 */
  /* @TS7: Invoke the rfind() of StringView */
  /* @TE7: rfind() should return StringView::stkNpos. */
  /* @End: UT_CORE_0225 */
  size_t const offset7 = sv.rfind(needle, 0);
  EXPECT_THAT(offset7, StringView::stkNpos);

  char const needle2[] = "The Quick Brown QuickX";

  /* @Start: UT_CORE_0225 */
  /* @TS8: Invoke the rfind() of StringView */
  /* @TE8: rfind() should return StringView::stkNpos. */
  /* @End: UT_CORE_0225 */
  size_t const offset8 = sv.rfind(needle2, StringView::stkNpos);
  EXPECT_THAT(offset8, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0226
@Test_Description:    To test the functionality of find_first_of() StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611598, 611599, 611601
*******************************************************************************/
TEST(StringViewTest, FindFirstOfStringView)
{
  // testing:
  // SizeType find_first_of(basic_string_view v, SizeType pos = 0) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  StringView const needles = "nwQB";

  /* @Start: UT_CORE_0226 */
  /* @TS1: Invoke the find_first_of() of StringView */
  /* @TE1: find_first_of() should return 4. */
  /* @End: UT_CORE_0226 */
  size_t const offset1 = sv.find_first_of(needles);
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0226 */
  /* @TS2: Invoke the find_first_of() of StringView */
  /* @TE2: find_first_of() should return 4. */
  /* @End: UT_CORE_0226 */
  size_t const offset2 = sv.find_first_of(needles, 0);
  EXPECT_THAT(offset2, 4);

  /* @Start: UT_CORE_0226 */
  /* @TS3: Invoke the find_first_of() of StringView */
  /* @TE3: find_first_of() should return 4. */
  /* @End: UT_CORE_0226 */
  size_t const offset3 = sv.find_first_of(needles, 4);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0226 */
  /* @TS4: Invoke the find_first_of() of StringView */
  /* @TE4: find_first_of() should return 10. */
  /* @End: UT_CORE_0226 */
  size_t const offset4 = sv.find_first_of(needles, 5);
  EXPECT_THAT(offset4, 10);

  /* @Start: UT_CORE_0226 */
  /* @TS5: Invoke the find_first_of() of StringView */
  /* @TE5: find_first_of() should return 13. */
  /* @End: UT_CORE_0226 */
  size_t const offset5 = sv.find_first_of(needles, 11);
  EXPECT_THAT(offset5, 13);

  /* @Start: UT_CORE_0226 */
  /* @TS6: Invoke the find_first_of() of StringView */
  /* @TE6: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0226 */
  size_t const offset6 = sv.find_first_of(needles, 1000);
  EXPECT_THAT(offset6, StringView::stkNpos);

  StringView const needles2;

  /* @Start: UT_CORE_0226 */
  /* @TS7: Invoke the find_first_of() of StringView */
  /* @TE7: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0226 */
  size_t const offset7 = sv.find_first_of(needles2, 0);
  EXPECT_THAT(offset7, StringView::stkNpos);

  /* @Start: UT_CORE_0226 */
  /* @TS8: Invoke the find_first_of() of StringView */
  /* @TE8: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0226 */
  size_t const offset8 = sv.find_first_of(needles2, 1000);
  EXPECT_THAT(offset8, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0227
@Test_Description:    To test the functionality of find_first_of char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611598, 611599, 611601
*****************************************************************************/
TEST(StringViewTest, FindFirstOfChar)
{
  // testing:
  // SizeType find_first_of(CharT c, SizeType pos = 0) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  /* @Start: UT_CORE_0227 */
  /* @TS1: Invoke the find_first_of() of StringView */
  /* @TE1: find_first_of() should return 4. */
  /* @End: UT_CORE_0227 */
  size_t const offset1 = sv.find_first_of('Q');
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0227 */
  /* @TS2: Invoke the find_first_of() of StringView */
  /* @TE2: find_first_of() should return 4. */
  /* @End: UT_CORE_0227 */
  size_t const offset2 = sv.find_first_of('Q', 0);
  EXPECT_THAT(offset2, 4);

  /* @Start: UT_CORE_0227 */
  /* @TS3: Invoke the find_first_of() of StringView */
  /* @TE3: find_first_of() should return 4. */
  /* @End: UT_CORE_0227 */
  size_t const offset3 = sv.find_first_of('Q', 4);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0227 */
  /* @TS4: Invoke the find_first_of() of StringView */
  /* @TE4: find_first_of() should return 16. */
  /* @End: UT_CORE_0227 */
  size_t const offset4 = sv.find_first_of('Q', 5);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0227 */
  /* @TS4: Invoke the find_first_of() of StringView */
  /* @TE4: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0227 */
  size_t const offset5 = sv.find_first_of('Q', 17);
  EXPECT_THAT(offset5, StringView::stkNpos);

  /* @Start: UT_CORE_0227 */
  /* @TS5: Invoke the find_first_of() of StringView */
  /* @TE5: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0227 */
  size_t const offset6 = sv.find_first_of('Q', 1000);
  EXPECT_THAT(offset6, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0228
@Test_Description:    To test the functionality of find_first_of sized char StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611598, 611599, 611601
*****************************************************************************/
TEST(StringViewTest, FindFirstOfSizedCharSegment)
{
  // testing:
  // SizeType find_first_of(CharT const* s, SizeType pos, SizeType count) const;

  StringView const sv = "The Quick Brown Quick";

  char const needles[] = "nwQB The";

  /* @Start: UT_CORE_0228*/
  /* @TS1: Invoke the find_first_of() of StringView */
  /* @TE1: find_first_of() should return 4. */
  /* @End: UT_CORE_0228 */
  size_t const offset1 = sv.find_first_of(needles, 0, 4);
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0228*/
  /* @TS2: Invoke the find_first_of() of StringView */
  /* @TE2: find_first_of() should return 0. */
  /* @End: UT_CORE_0228 */
  size_t const offset2 = sv.find_first_of(needles, 0, 6);
  EXPECT_THAT(offset2, 0);

  /* @Start: UT_CORE_0228*/
  /* @TS3: Invoke the find_first_of() of StringView */
  /* @TE3: find_first_of() should return 4. */
  /* @End: UT_CORE_0228 */
  size_t const offset3 = sv.find_first_of(needles, 4, 4);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0228*/
  /* @TS4: Invoke the find_first_of() of StringView */
  /* @TE4: find_first_of() should return 10. */
  /* @End: UT_CORE_0228 */
  size_t const offset4 = sv.find_first_of(needles, 5, 4);
  EXPECT_THAT(offset4, 10);

  /* @Start: UT_CORE_0228*/
  /* @TS5: Invoke the find_first_of() of StringView */
  /* @TE5: find_first_of() should return 13. */
  /* @End: UT_CORE_0228 */
  size_t const offset5 = sv.find_first_of(needles, 11, 4);
  EXPECT_THAT(offset5, 13);

  /* @Start: UT_CORE_0228*/
  /* @TS6: Invoke the find_first_of() of StringView */
  /* @TE6: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0228 */
  size_t const offset6 = sv.find_first_of(needles, 1000, 4);
  EXPECT_THAT(offset6, StringView::stkNpos);

  char const needles2[] = "";

  /* @Start: UT_CORE_0228*/
  /* @TS7: Invoke the find_first_of() of StringView */
  /* @TE7: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0228 */
  size_t const offset7 = sv.find_first_of(needles2, 0, 0);
  EXPECT_THAT(offset7, StringView::stkNpos);

  /* @Start: UT_CORE_0228*/
  /* @TS8: Invoke the find_first_of() of StringView */
  /* @TE8: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0228 */
  size_t const offset8 = sv.find_first_of(needles2, 1000, 0);
  EXPECT_THAT(offset8, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0229
@Test_Description:    To test the functionality of find_first_of char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611598, 611599, 611601
*****************************************************************************/
TEST(StringViewTest, FindFirstOfCharSegment)
{
  // testing:
  // SizeType find_first_of(CharT const* s, SizeType pos = 0) const;

  StringView const sv = "The Quick Brown Quick";

  char const needles[] = "nwQB";

  /* @Start: UT_CORE_0229*/
  /* @TS1: Invoke the find_first_of() of StringView */
  /* @TE1: find_first_of() should return 4. */
  /* @End: UT_CORE_0229 */
  size_t const offset1 = sv.find_first_of(needles);
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0229*/
  /* @TS2: Invoke the find_first_of() of StringView */
  /* @TE2: find_first_of() should return 4. */
  /* @End: UT_CORE_0229 */
  size_t const offset2 = sv.find_first_of(needles, 0);
  EXPECT_THAT(offset2, 4);

  /* @Start: UT_CORE_0229*/
  /* @TS3: Invoke the find_first_of() of StringView */
  /* @TE3: find_first_of() should return 4. */
  /* @End: UT_CORE_0229 */
  size_t const offset3 = sv.find_first_of(needles, 4);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0229*/
  /* @TS4: Invoke the find_first_of() of StringView */
  /* @TE4: find_first_of() should return 10. */
  /* @End: UT_CORE_0229 */
  size_t const offset4 = sv.find_first_of(needles, 5);
  EXPECT_THAT(offset4, 10);

  /* @Start: UT_CORE_0229*/
  /* @TS5: Invoke the find_first_of() of StringView */
  /* @TE5: find_first_of() should return 13. */
  /* @End: UT_CORE_0229 */
  size_t const offset5 = sv.find_first_of(needles, 11);
  EXPECT_THAT(offset5, 13);

  /* @Start: UT_CORE_0229*/
  /* @TS6: Invoke the find_first_of() of StringView */
  /* @TE6: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0229 */
  size_t const offset6 = sv.find_first_of(needles, 1000);
  EXPECT_THAT(offset6, StringView::stkNpos);

  StringView const needles2;

  /* @Start: UT_CORE_0229*/
  /* @TS7: Invoke the find_first_of() of StringView */
  /* @TE7: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0229 */
  size_t const offset7 = sv.find_first_of(needles2, 0);
  EXPECT_THAT(offset7, StringView::stkNpos);

  /* @Start: UT_CORE_0229*/
  /* @TS8: Invoke the find_first_of() of StringView */
  /* @TE8: find_first_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0229 */
  size_t const offset8 = sv.find_first_of(needles2, 1000);
  EXPECT_THAT(offset8, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0230
@Test_Description:    To test the functionality of find_last_of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611602, 611603
*****************************************************************************/
TEST(StringViewTest, FindLastOfStringView)
{
  // testing:
  // SizeType find_first_of(basic_string_view v, SizeType pos = 0) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  StringView const needles = "nwQB";

  /* @Start: UT_CORE_0230 */
  /* @TS1: Invoke the find_last_of() of StringView */
  /* @TE1: find_last_of() should return 16. */
  /* @End: UT_CORE_0230 */
  size_t const offset1 = sv.find_last_of(needles);
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0230 */
  /* @TS2: Invoke the find_last_of() of StringView */
  /* @TE2: find_last_of() should return 16. */
  /* @End: UT_CORE_0230 */
  size_t const offset2 = sv.find_last_of(needles, StringView::stkNpos);
  EXPECT_THAT(offset2, 16);

  /* @Start: UT_CORE_0230 */
  /* @TS3: Invoke the find_last_of() of StringView */
  /* @TE3: find_last_of() should return 16. */
  /* @End: UT_CORE_0230 */
  size_t const offset3 = sv.find_last_of(needles, 21);
  EXPECT_THAT(offset3, 16);

  /* @Start: UT_CORE_0230 */
  /* @TS4: Invoke the find_last_of() of StringView */
  /* @TE4: find_last_of() should return 16. */
  /* @End: UT_CORE_0230 */
  size_t const offset4 = sv.find_last_of(needles, 16);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0230 */
  /* @TS5: Invoke the find_last_of() of StringView */
  /* @TE5: find_last_of() should return 14. */
  /* @End: UT_CORE_0230 */
  size_t const offset5 = sv.find_last_of(needles, 15);
  EXPECT_THAT(offset5, 14);

  /* @Start: UT_CORE_0230 */
  /* @TS6: Invoke the find_last_of() of StringView */
  /* @TE6: find_last_of() should return 10. */
  /* @End: UT_CORE_0230 */
  size_t const offset6 = sv.find_last_of(needles, 12);
  EXPECT_THAT(offset6, 10);

  /* @Start: UT_CORE_0230 */
  /* @TS7: Invoke the find_last_of() of StringView */
  /* @TE7: find_last_of() should return 4. */
  /* @End: UT_CORE_0230 */
  size_t const offset7 = sv.find_last_of(needles, 9);
  EXPECT_THAT(offset7, 4);

  /* @Start: UT_CORE_0230 */
  /* @TS8: Invoke the find_last_of() of StringView */
  /* @TE8: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0230 */
  size_t const offset8 = sv.find_last_of(needles, 3);
  EXPECT_THAT(offset8, StringView::stkNpos);

  StringView const sv2;

  /* @Start: UT_CORE_0230 */
  /* @TS9: Invoke the find_last_of() of StringView */
  /* @TE9: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0230 */
  size_t const offset9 = sv2.find_last_of(needles, StringView::stkNpos);
  EXPECT_THAT(offset9, StringView::stkNpos);

  StringView const needles2;

  /* @Start: UT_CORE_0230 */
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0230 */
  size_t const offsetA = sv.find_last_of(needles2, StringView::stkNpos);
  EXPECT_THAT(offsetA, StringView::stkNpos);

  /* @Start: UT_CORE_0230 */
  /* @TS11: Invoke the find_last_of() of StringView */
  /* @TE11: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0230 */
  size_t const offsetB = sv.find_last_of(needles2, 0);
  EXPECT_THAT(offsetB, StringView::stkNpos);

  /* @Start: UT_CORE_0230 */
  /* @TS12: Invoke the find_last_of() of StringView */
  /* @TE12: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0230 */
  size_t const offsetC = sv.find_last_of(needles2, 4);
  EXPECT_THAT(offsetC, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0231
@Test_Description:    To test the functionality of find_first_of char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611598, 611599, 611601
*****************************************************************************/
TEST(StringViewTest, FindLastOfChar)
{
  // testing:
  // SizeType find_first_of(CharT c, SizeType pos = 0) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  /* @Start: UT_CORE_0231 */
  /* @TS1: Invoke the find_last_of() of StringView */
  /* @TE1: find_last_of() should return 16. */
  /* @End: UT_CORE_0231 */
  size_t const offset1 = sv.find_last_of('Q');
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0231 */
  /* @TS1: Invoke the find_last_of() of StringView */
  /* @TE1: find_last_of() should return 16. */
  /* @End: UT_CORE_0231 */
  size_t const offset2 = sv.find_last_of('Q', StringView::stkNpos);
  EXPECT_THAT(offset2, 16);

  /* @Start: UT_CORE_0231 */
  /* @TS1: Invoke the find_last_of() of StringView */
  /* @TE1: find_last_of() should return 4. */
  /* @End: UT_CORE_0231 */
  size_t const offset3 = sv.find_last_of('Q', 15);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0231 */
  /* @TS1: Invoke the find_last_of() of StringView */
  /* @TE1: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0231 */
  size_t const offset4 = sv.find_last_of('Q', 3);
  EXPECT_THAT(offset4, StringView::stkNpos);

  StringView const sv2;

  /* @Start: UT_CORE_0231 */
  /* @TS1: Invoke the find_last_of() of StringView */
  /* @TE1: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0231 */
  size_t const offset5 = sv2.find_last_of('Q', 3);
  EXPECT_THAT(offset5, StringView::stkNpos);

  StringView const sv3 = "Q";

  /* @Start: UT_CORE_0231 */
  /* @TS1: Invoke the find_last_of() of StringView */
  /* @TE1: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0231 */
  size_t const offset6 = sv3.find_last_not_of('Q', 0);
  EXPECT_THAT(offset6, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0232
@Test_Description:    To test the functionality of find_first_of sized char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611598, 611599, 611601
*****************************************************************************/
TEST(StringViewTest, FindLastOfSizedCharSegment)
{
  // testing:
  // SizeType find_first_of(CharT const* s, SizeType pos, SizeType count) const;

  StringView const sv = "The Quick Brown Quick";

  char const needles[] = "nwQB ick";

  /* @Start: UT_CORE_0232*/
  /* @TS1: Invoke the find_last_of() of StringView */
  /* @TE1: find_last_of() should return 16. */
  /* @End: UT_CORE_0232 */
  size_t const offset1 = sv.find_last_of(needles, StringView::stkNpos, 4);
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0232*/
  /* @TS2: Invoke the find_last_of() of StringView */
  /* @TE2: find_last_of() should return 18. */
  /* @End: UT_CORE_0232 */
  size_t const offset2 = sv.find_last_of(needles, StringView::stkNpos, 6);
  EXPECT_THAT(offset2, 18);

  /* @Start: UT_CORE_0232*/
  /* @TS3: Invoke the find_last_of() of StringView */
  /* @TE3: find_last_of() should return 16. */
  /* @End: UT_CORE_0232 */
  size_t const offset3 = sv.find_last_of(needles, 21, 4);
  EXPECT_THAT(offset3, 16);

  /* @Start: UT_CORE_0232*/
  /* @TS4: Invoke the find_last_of() of StringView */
  /* @TE4: find_last_of() should return 16. */
  /* @End: UT_CORE_0232 */
  size_t const offset4 = sv.find_last_of(needles, 16, 4);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0232*/
  /* @TS5: Invoke the find_last_of() of StringView */
  /* @TE5: find_last_of() should return 14. */
  /* @End: UT_CORE_0232 */
  size_t const offset5 = sv.find_last_of(needles, 15, 4);
  EXPECT_THAT(offset5, 14);

  /* @Start: UT_CORE_0232*/
  /* @TS6: Invoke the find_last_of() of StringView */
  /* @TE6: find_last_of() should return 10. */
  /* @End: UT_CORE_0232 */
  size_t const offset6 = sv.find_last_of(needles, 12, 4);
  EXPECT_THAT(offset6, 10);

  /* @Start: UT_CORE_0232*/
  /* @TS7: Invoke the find_last_of() of StringView */
  /* @TE7: find_last_of() should return 4. */
  /* @End: UT_CORE_0232 */
  size_t const offset7 = sv.find_last_of(needles, 9, 4);
  EXPECT_THAT(offset7, 4);

  /* @Start: UT_CORE_0232*/
  /* @TS8: Invoke the find_last_of() of StringView */
  /* @TE8: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0232 */
  size_t const offset8 = sv.find_last_of(needles, 3, 4);
  EXPECT_THAT(offset8, StringView::stkNpos);

  char const needles2[] = "";

  /* @Start: UT_CORE_0232*/
  /* @TS9: Invoke the find_last_of() of StringView */
  /* @TE9: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0232 */
  size_t const offset9 = sv.find_last_of(needles2, StringView::stkNpos, 0);
  EXPECT_THAT(offset9, StringView::stkNpos);

  /* @Start: UT_CORE_0232*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0232 */
  size_t const offsetA = sv.find_last_of(needles2, 0, 0);
  EXPECT_THAT(offsetA, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0233
@Test_Description:    To test the functionality of find_first_of char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611598, 611599, 611601
*****************************************************************************/
TEST(StringViewTest, FindLastOfCharSegment)
{
  // testing:
  // SizeType find_first_of(CharT const* s, SizeType pos = 0) const;

  StringView const sv = "The Quick Brown Quick";

  char const needles[] = "nwQB";

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return 16. */
  /* @End: UT_CORE_0233 */
  size_t const offset1 = sv.find_last_of(needles);
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return 16. */
  /* @End: UT_CORE_0233 */
  size_t const offset2 = sv.find_last_of(needles, StringView::stkNpos);
  EXPECT_THAT(offset2, 16);

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return 16. */
  /* @End: UT_CORE_0233 */
  size_t const offset3 = sv.find_last_of(needles, 21);
  EXPECT_THAT(offset3, 16);

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return 16. */
  /* @End: UT_CORE_0233 */
  size_t const offset4 = sv.find_last_of(needles, 16);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return 14. */
  /* @End: UT_CORE_0233 */
  size_t const offset5 = sv.find_last_of(needles, 15);
  EXPECT_THAT(offset5, 14);

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return 10. */
  /* @End: UT_CORE_0233 */
  size_t const offset6 = sv.find_last_of(needles, 12);
  EXPECT_THAT(offset6, 10);

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return 4. */
  /* @End: UT_CORE_0233 */
  size_t const offset7 = sv.find_last_of(needles, 9);
  EXPECT_THAT(offset7, 4);

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0233 */
  size_t const offset8 = sv.find_last_of(needles, 3);
  EXPECT_THAT(offset8, StringView::stkNpos);

  StringView const needles2;

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0233 */
  size_t const offset9 = sv.find_last_of(needles2, StringView::stkNpos);
  EXPECT_THAT(offset9, StringView::stkNpos);

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0233 */
  size_t const offsetA = sv.find_last_of(needles2, 0);
  EXPECT_THAT(offsetA, StringView::stkNpos);

  /* @Start: UT_CORE_0233*/
  /* @TS10: Invoke the find_last_of() of StringView */
  /* @TE10: find_last_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0233 */
  size_t const offsetB = sv.find_last_of(needles2, 4);
  EXPECT_THAT(offsetB, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0234
@Test_Description:    To test the functionality of find_first_not_of() StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611606, 611607, 611608, 611609
*****************************************************************************/
TEST(StringViewTest, FindFirstNotOfStringView)
{
  // testing:
  // SizeType find_first_not_of(basic_string_view v, SizeType pos = 0) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  StringView const needles = "The uick ro";
  /* @Start: UT_CORE_0234 */
  /* @TS10: Invoke the find_first_not_of() of StringView */
  /* @TE10: find_first_not_of() should return 4. */
  /* @End: UT_CORE_0234 */
  size_t const offset1 = sv.find_first_not_of(needles);
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0234 */
  /* @TS10: Invoke the find_first_not_of() of StringView */
  /* @TE10: find_first_not_of() should return 4. */
  /* @End: UT_CORE_0234 */
  size_t const offset2 = sv.find_first_not_of(needles, 0);
  EXPECT_THAT(offset2, 4);

  /* @Start: UT_CORE_0234 */
  /* @TS10: Invoke the find_first_not_of() of StringView */
  /* @TE10: find_first_not_of() should return 4. */
  /* @End: UT_CORE_0234 */
  size_t const offset3 = sv.find_first_not_of(needles, 4);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0234 */
  /* @TS10: Invoke the find_first_not_of() of StringView */
  /* @TE10: find_first_not_of() should return 10. */
  /* @End: UT_CORE_0234 */
  size_t const offset4 = sv.find_first_not_of(needles, 5);
  EXPECT_THAT(offset4, 10);

  /* @Start: UT_CORE_0234 */
  /* @TS10: Invoke the find_first_not_of() of StringView */
  /* @TE10: find_first_not_of() should return 13. */
  /* @End: UT_CORE_0234 */
  size_t const offset5 = sv.find_first_not_of(needles, 11);
  EXPECT_THAT(offset5, 13);

  /* @Start: UT_CORE_0234 */
  /* @TS10: Invoke the find_first_not_of() of StringView */
  /* @TE10: find_first_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0234 */
  size_t const offset6 = sv.find_first_not_of(needles, 1000);
  EXPECT_THAT(offset6, StringView::stkNpos);

  StringView const needles2;

  /* @Start: UT_CORE_0234 */
  /* @TS10: Invoke the find_first_not_of() of StringView */
  /* @TE10: find_first_not_of() should return 0. */
  /* @End: UT_CORE_0234 */
  size_t const offset7 = sv.find_first_not_of(needles2, 0);
  EXPECT_THAT(offset7, 0);

  /* @Start: UT_CORE_0234 */
  /* @TS10: Invoke the find_first_not_of() of StringView */
  /* @TE10: find_first_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0234 */
  size_t const offset8 = sv.find_first_not_of(needles2, 1000);
  EXPECT_THAT(offset8, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0235
@Test_Description:    To test the functionality of find_first_not_of char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611606, 611607, 611608, 611609
*****************************************************************************/
TEST(StringViewTest, FindFirstNotOfChar)
{
  // testing:
  // SizeType find_first_not_of(CharT c, SizeType pos = 0) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  /* @Start: UT_CORE_0235*/
  /* @TS1: Invoke the find_first_not_of() of StringView */
  /* @TE1: find_first_not_of() should return 1. */
  /* @End: UT_CORE_0235 */
  size_t const offset1 = sv.find_first_not_of('T');
  EXPECT_THAT(offset1, 1);

  /* @Start: UT_CORE_0235*/
  /* @TS2: Invoke the find_first_not_of() of StringView */
  /* @TE2: find_first_not_of() should return 1. */
  /* @End: UT_CORE_0235 */
  size_t const offset2 = sv.find_first_not_of('T', 0);
  EXPECT_THAT(offset2, 1);

  /* @Start: UT_CORE_0235*/
  /* @TS3: Invoke the find_first_not_of() of StringView */
  /* @TE3: find_first_not_of() should return 0. */
  /* @End: UT_CORE_0235 */
  size_t const offset3 = sv.find_first_not_of('Q', 0);
  EXPECT_THAT(offset3, 0);

  /* @Start: UT_CORE_0235*/
  /* @TS4: Invoke the find_first_not_of() of StringView */
  /* @TE4: find_first_not_of() should return 5. */
  /* @End: UT_CORE_0235 */
  size_t const offset4 = sv.find_first_not_of('Q', 4);
  EXPECT_THAT(offset4, 5);

  /* @Start: UT_CORE_0235*/
  /* @TS5: Invoke the find_first_not_of() of StringView */
  /* @TE5: find_first_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0235 */
  size_t const offset5 = sv.find_first_not_of('Q', 21);
  EXPECT_THAT(offset5, StringView::stkNpos);

  /* @Start: UT_CORE_0235*/
  /* @TS6: Invoke the find_first_not_of() of StringView */
  /* @TE6: find_first_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0235 */
  size_t const offset6 = sv.find_first_not_of('Q', 1000);
  EXPECT_THAT(offset6, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0236
@Test_Description:    To test the functionality of find_first_not_of sized char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611606, 611607, 611608, 611609
*****************************************************************************/
TEST(StringViewTest, FindFirstNotOfSizedCharSegment)
{
  // testing:
  // SizeType find_first_not_of(CharT const* s, SizeType pos, SizeType count) const;

  StringView const sv = "The Quick Brown Quick";

  char const needles[] = "The uick ro QnwB";

  /* @Start: UT_CORE_0236 */
  /* @TS1: Invoke the find_first_not_of() of StringView */
  /* @TE1: find_first_not_of() should return 4. */
  /* @End: UT_CORE_0236 */
  size_t const offset1 = sv.find_first_not_of(needles, 0, 11);
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0236 */
  /* @TS2: Invoke the find_first_not_of() of StringView */
  /* @TE2: find_first_not_of() should return 10. */
  /* @End: UT_CORE_0236 */
  size_t const offset2 = sv.find_first_not_of(needles, 0, 13);
  EXPECT_THAT(offset2, 10);

  /* @Start: UT_CORE_0236 */
  /* @TS3: Invoke the find_first_not_of() of StringView */
  /* @TE3: find_first_not_of() should return 4. */
  /* @End: UT_CORE_0236 */
  size_t const offset3 = sv.find_first_not_of(needles, 4, 11);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0236 */
  /* @TS4: Invoke the find_first_not_of() of StringView */
  /* @TE4: find_first_not_of() should return 10. */
  /* @End: UT_CORE_0236 */
  size_t const offset4 = sv.find_first_not_of(needles, 5, 11);
  EXPECT_THAT(offset4, 10);

  /* @Start: UT_CORE_0236 */
  /* @TS5: Invoke the find_first_not_of() of StringView */
  /* @TE5: find_first_not_of() should return 13. */
  /* @End: UT_CORE_0236 */
  size_t const offset5 = sv.find_first_not_of(needles, 11, 11);
  EXPECT_THAT(offset5, 13);

  /* @Start: UT_CORE_0236 */
  /* @TS6: Invoke the find_first_not_of() of StringView */
  /* @TE6: find_first_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0236 */
  size_t const offset6 = sv.find_first_not_of(needles, 1000, 11);
  EXPECT_THAT(offset6, StringView::stkNpos);

  char const needles2[] = "";

  /* @Start: UT_CORE_0236 */
  /* @TS7: Invoke the find_first_not_of() of StringView */
  /* @TE7: find_first_not_of() should return 0. */
  /* @End: UT_CORE_0236 */
  size_t const offset7 = sv.find_first_not_of(needles2, 0, 0);
  EXPECT_THAT(offset7, 0);

  /* @Start: UT_CORE_0236 */
  /* @TS8: Invoke the find_first_not_of() of StringView */
  /* @TE8: find_first_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0236 */
  size_t const offset8 = sv.find_first_not_of(needles2, 1000, 0);
  EXPECT_THAT(offset8, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0237
@Test_Description:    To test the functionality of find_first_not_of char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611606, 611607, 611608, 611609
*****************************************************************************/
TEST(StringViewTest, FindFirstNotOfCharSegment)
{
  // testing:
  // SizeType find_first_not_of(CharT const* s, SizeType pos = 0) const;

  StringView const sv = "The Quick Brown Quick";

  char const needles[] = "The uick ro";
  /* @Start: UT_CORE_0237*/
  /* @TS1: Invoke the find_first_not_of() of StringView */
  /* @TE1: find_first_not_of() should return 4. */
  /* @End: UT_CORE_0237 */
  size_t const offset1 = sv.find_first_not_of(needles);
  EXPECT_THAT(offset1, 4);

  /* @Start: UT_CORE_0237*/
  /* @TS2: Invoke the find_first_not_of() of StringView */
  /* @TE2: find_first_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0237 */
  size_t const offset2 = sv.find_first_not_of(needles, 0);
  EXPECT_THAT(offset2, 4);

  /* @Start: UT_CORE_0237*/
  /* @TS3: Invoke the find_first_not_of() of StringView */
  /* @TE3: find_first_not_of() should return 4. */
  /* @End: UT_CORE_0237 */
  size_t const offset3 = sv.find_first_not_of(needles, 4);
  EXPECT_THAT(offset3, 4);

  /* @Start: UT_CORE_0237*/
  /* @TS4: Invoke the find_first_not_of() of StringView */
  /* @TE4: find_first_not_of() should return 10. */
  /* @End: UT_CORE_0237 */
  size_t const offset4 = sv.find_first_not_of(needles, 5);
  EXPECT_THAT(offset4, 10);

  /* @Start: UT_CORE_0237*/
  /* @TS5: Invoke the find_first_not_of() of StringView */
  /* @TE5: find_first_not_of() should return 13. */
  /* @End: UT_CORE_0237 */
  size_t const offset5 = sv.find_first_not_of(needles, 11);
  EXPECT_THAT(offset5, 13);

  /* @Start: UT_CORE_0237*/
  /* @TS6: Invoke the find_first_not_of() of StringView */
  /* @TE6: find_first_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0237 */
  size_t const offset6 = sv.find_first_not_of(needles, 1000);
  EXPECT_THAT(offset6, StringView::stkNpos);

  char const needles2[] = "";

  /* @Start: UT_CORE_0237*/
  /* @TS7: Invoke the find_first_not_of() of StringView */
  /* @TE7: find_first_not_of() should return 0. */
  /* @End: UT_CORE_0237 */
  size_t const offset7 = sv.find_first_not_of(needles2, 0);
  EXPECT_THAT(offset7, 0);

  /* @Start: UT_CORE_0237*/
  /* @TS8: Invoke the find_first_not_of() of StringView */
  /* @TE8: find_first_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0237 */
  size_t const offset8 = sv.find_first_not_of(needles2, 1000);
  EXPECT_THAT(offset8, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0238
@Test_Description:    To test the functionality of find_last_not_of() StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611610, 611611, 611612, 611613
*******************************************************************************/
TEST(StringViewTest, FindLastNotOfStringView)
{
  // testing:
  // SizeType find_last_not_of(basic_string_view v, SizeType pos = stkNpos) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  StringView const needles = "The uick ro";

  /* @Start: UT_CORE_0238*/
  /* @TS1: Invoke the find_last_not_of() of StringView */
  /* @TE1: find_last_not_of() should return 16. */
  /* @End: UT_CORE_0238 */
  size_t const offset1 = sv.find_last_not_of(needles);
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0238*/
  /* @TS2: Invoke the find_last_not_of() of StringView */
  /* @TE2: find_last_not_of() should return 16. */
  /* @End: UT_CORE_0238 */
  size_t const offset2 = sv.find_last_not_of(needles, StringView::stkNpos);
  EXPECT_THAT(offset2, 16);

  /* @Start: UT_CORE_0238*/
  /* @TS3: Invoke the find_last_not_of() of StringView */
  /* @TE3: find_last_not_of() should return 16. */
  /* @End: UT_CORE_0238 */
  size_t const offset3 = sv.find_last_not_of(needles, 21);
  EXPECT_THAT(offset3, 16);

  /* @Start: UT_CORE_0238*/
  /* @TS4: Invoke the find_last_not_of() of StringView */
  /* @TE4: find_last_not_of() should return 16. */
  /* @End: UT_CORE_0238 */
  size_t const offset4 = sv.find_last_not_of(needles, 16);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0238*/
  /* @TS5: Invoke the find_last_not_of() of StringView */
  /* @TE5: find_last_not_of() should return 14. */
  /* @End: UT_CORE_0238 */
  size_t const offset5 = sv.find_last_not_of(needles, 15);
  EXPECT_THAT(offset5, 14);

  /* @Start: UT_CORE_0238*/
  /* @TS6: Invoke the find_last_not_of() of StringView */
  /* @TE6: find_last_not_of() should return 10. */
  /* @End: UT_CORE_0238 */
  size_t const offset6 = sv.find_last_not_of(needles, 12);
  EXPECT_THAT(offset6, 10);

  /* @Start: UT_CORE_0238*/
  /* @TS7: Invoke the find_last_not_of() of StringView */
  /* @TE7: find_last_not_of() should return 4. */
  /* @End: UT_CORE_0238 */
  size_t const offset7 = sv.find_last_not_of(needles, 9);
  EXPECT_THAT(offset7, 4);

  /* @Start: UT_CORE_0238*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0238 */
  size_t const offset8 = sv.find_last_not_of(needles, 3);
  EXPECT_THAT(offset8, StringView::stkNpos);

  StringView const sv2;

  /* @Start: UT_CORE_0238*/
  /* @TS9: Invoke the find_last_not_of() of StringView */
  /* @TE9: find_last_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0238 */
  size_t const offset9 = sv2.find_last_not_of(needles, StringView::stkNpos);
  EXPECT_THAT(offset9, StringView::stkNpos);

  StringView const needles2;

  /* @Start: UT_CORE_0238*/
  /* @TS10: Invoke the find_last_not_of() of StringView */
  /* @TE10: find_last_not_of() should return 20. */
  /* @End: UT_CORE_0238 */
  size_t const offsetA = sv.find_last_not_of(needles2, StringView::stkNpos);
  EXPECT_THAT(offsetA, 20);

  /* @Start: UT_CORE_0238*/
  /* @TS11: Invoke the find_last_not_of() of StringView */
  /* @TE11: find_last_not_of() should return 0. */
  /* @End: UT_CORE_0238 */
  size_t const offsetB = sv.find_last_not_of(needles2, 0);
  EXPECT_THAT(offsetB, 0);

  /* @Start: UT_CORE_0238*/
  /* @TS12: Invoke the find_last_not_of() of StringView */
  /* @TE12: find_last_not_of() should return 4. */
  /* @End: UT_CORE_0238 */
  size_t const offsetC = sv.find_last_not_of(needles2, 4);
  EXPECT_THAT(offsetC, 4);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0239
@Test_Description:    To test the functionality of find_last_not_of char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611610, 611611, 611612, 611613
*******************************************************************************/
TEST(StringViewTest, FindLastNotOfChar)
{
  // testing:
  // SizeType find_last_not_of(CharT c, SizeType pos = stkNpos) const noexcept;

  StringView const sv = "The Quick Brown Quick";

  /* @Start: UT_CORE_0239*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 19. */
  /* @End: UT_CORE_0239 */
  size_t const offset1 = sv.find_last_not_of('k');
  EXPECT_THAT(offset1, 19);

  /* @Start: UT_CORE_0239*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 19. */
  /* @End: UT_CORE_0239 */
  size_t const offset2 = sv.find_last_not_of('k', StringView::stkNpos);
  EXPECT_THAT(offset2, 19);

  /* @Start: UT_CORE_0239*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 20. */
  /* @End: UT_CORE_0239 */
  size_t const offset3 = sv.find_last_not_of('Q', StringView::stkNpos);
  EXPECT_THAT(offset3, 20);

  /* @Start: UT_CORE_0239*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 15. */
  /* @End: UT_CORE_0239 */
  size_t const offset4 = sv.find_last_not_of('Q', 16);
  EXPECT_THAT(offset4, 15);

  /* @Start: UT_CORE_0239*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 0. */
  /* @End: UT_CORE_0239 */
  size_t const offset5 = sv.find_last_not_of('Q', 0);
  EXPECT_THAT(offset5, 0);

  StringView const sv2;

  /* @Start: UT_CORE_0239*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0239 */
  size_t const offset6 = sv2.find_last_not_of('Q', 3);
  EXPECT_THAT(offset6, StringView::stkNpos);

  StringView const sv3 = "Q";

  /* @Start: UT_CORE_0239*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0239 */
  size_t const offset7 = sv3.find_last_not_of('Q', 0);
  EXPECT_THAT(offset7, StringView::stkNpos);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0240
@Test_Description:    To test the functionality of find_last_not_of sized char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611610, 611611, 611612, 611613
*******************************************************************************/
TEST(StringViewTest, FindLastNotOfSizedCharSegment)
{
  // testing:
  // SizeType find_last_not_of(CharT const* s, SizeType pos, SizeType count) const;

  StringView const sv = "The Quick Brown Quick";

  char const needles[] = "The uick ro QnwB";

  /* @Start: UT_CORE_0240*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 16. */
  /* @End: UT_CORE_0240 */
  size_t const offset1 = sv.find_last_not_of(needles, StringView::stkNpos, 11);
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0240*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 14. */
  /* @End: UT_CORE_0240 */
  size_t const offset2 = sv.find_last_not_of(needles, StringView::stkNpos, 13);
  EXPECT_THAT(offset2, 14);

  /* @Start: UT_CORE_0240*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 14. */
  /* @End: UT_CORE_0240 */
  size_t const offset3 = sv.find_last_not_of(needles, 15, 11);
  EXPECT_THAT(offset3, 14);

  /* @Start: UT_CORE_0240*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 10. */
  /* @End: UT_CORE_0240 */
  size_t const offset4 = sv.find_last_not_of(needles, 12, 11);
  EXPECT_THAT(offset4, 10);

  /* @Start: UT_CORE_0240*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 4. */
  /* @End: UT_CORE_0240 */
  size_t const offset5 = sv.find_last_not_of(needles, 9, 11);
  EXPECT_THAT(offset5, 4);

  /* @Start: UT_CORE_0240*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0240 */
  size_t const offset6 = sv.find_last_not_of(needles, 3, 11);
  EXPECT_THAT(offset6, StringView::stkNpos);

  StringView const sv2;

  /* @Start: UT_CORE_0240*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0240 */
  size_t const offset7 = sv2.find_last_not_of(needles, StringView::stkNpos, 11);
  EXPECT_THAT(offset7, StringView::stkNpos);

  char const needles2[] = "";

  /* @Start: UT_CORE_0240*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 20. */
  /* @End: UT_CORE_0240 */
  size_t const offsetA = sv.find_last_not_of(needles2, StringView::stkNpos, 0);
  EXPECT_THAT(offsetA, 20);

  /* @Start: UT_CORE_0240*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 0. */
  /* @End: UT_CORE_0240 */
  size_t const offsetB = sv.find_last_not_of(needles2, 0, 0);
  EXPECT_THAT(offsetB, 0);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0241
@Test_Description:    To test the functionality of find_first_not_of char of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611606, 611607, 611608, 611609
*******************************************************************************/
TEST(StringViewTest, FindLastNotOfCharSegment)
{
  // testing:
  // SizeType find_last_not_of(CharT const* s, SizeType pos = stkNpos) const;

  StringView const sv = "The Quick Brown Quick";

  char const needles[] = "The uick ro";

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 16. */
  /* @End: UT_CORE_0241 */
  size_t const offset1 = sv.find_last_not_of(needles);
  EXPECT_THAT(offset1, 16);

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 16. */
  /* @End: UT_CORE_0241 */
  size_t const offset2 = sv.find_last_not_of(needles, StringView::stkNpos);
  EXPECT_THAT(offset2, 16);

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 16. */
  /* @End: UT_CORE_0241 */
  size_t const offset3 = sv.find_last_not_of(needles, 21);
  EXPECT_THAT(offset3, 16);

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 16. */
  /* @End: UT_CORE_0241 */
  size_t const offset4 = sv.find_last_not_of(needles, 16);
  EXPECT_THAT(offset4, 16);

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 14. */
  /* @End: UT_CORE_0241 */
  size_t const offset5 = sv.find_last_not_of(needles, 15);
  EXPECT_THAT(offset5, 14);

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 10. */
  /* @End: UT_CORE_0241 */
  size_t const offset6 = sv.find_last_not_of(needles, 12);
  EXPECT_THAT(offset6, 10);

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 4. */
  /* @End: UT_CORE_0241 */
  size_t const offset7 = sv.find_last_not_of(needles, 9);
  EXPECT_THAT(offset7, 4);

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0241 */
  size_t const offset8 = sv.find_last_not_of(needles, 3);
  EXPECT_THAT(offset8, StringView::stkNpos);

  StringView const sv2;

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return StringView::stkNpos. */
  /* @End: UT_CORE_0241 */
  size_t const offset9 = sv2.find_last_not_of(needles, StringView::stkNpos);
  EXPECT_THAT(offset9, StringView::stkNpos);

  char const needles2[] = "";

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 20. */
  /* @End: UT_CORE_0241 */
  size_t const offsetA = sv.find_last_not_of(needles2, StringView::stkNpos);
  EXPECT_THAT(offsetA, 20);

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 0. */
  /* @End: UT_CORE_0241 */
  size_t const offsetB = sv.find_last_not_of(needles2, 0);
  EXPECT_THAT(offsetB, 0);

  /* @Start: UT_CORE_0241*/
  /* @TS8: Invoke the find_last_not_of() of StringView */
  /* @TE8: find_last_not_of() should return 4. */
  /* @End: UT_CORE_0241 */
  size_t const offsetC = sv.find_last_not_of(needles2, 4);
  EXPECT_THAT(offsetC, 4);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0242
@Test_Description:    To test the functionality of text of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611557
*******************************************************************************/
TEST(StringViewTest, ConstexprCreate)
{
  // Make sure that compilation in a constexpr context also works.
  constexpr StringView const sv1 = "The Quick Brown Fox";

  /* @Start: UT_CORE_0242 */
  /* @TS1: Check the size of sv1. */
  /* @TE1: size of sv1 should be 19. */
  /* @TS2: Invoke the data() of StringView. */
  /* @TE2: data() should be equal to "The Quick Brown Fox". */
  /* @End: UT_CORE_0242 */
  EXPECT_THAT(sv1, HasSize(19));
  EXPECT_THAT(sv1.data(), "The Quick Brown Fox");

  constexpr char const* text3 = "The Fox";
  constexpr StringView const sv3(text3);
  /* @Start: UT_CORE_0242 */
  /* @TS1: Check the size of sv3. */
  /* @TE1: size of sv3 should be 7. */
  /* @TS2: Invoke the data() of StringView. */
  /* @TE2: data() should be equal to "The Fox". */
  /* @End: UT_CORE_0242 */
  EXPECT_THAT(sv3, HasSize(7));
  EXPECT_THAT(sv3.data(), "The Fox");

  constexpr StringView sv4{"The"};
  /* @Start: UT_CORE_0242 */
  /* @TS1: Check the size of sv4. */
  /* @TE1: size of sv4 should be 3. */
  /* @TS2: Invoke the data() of StringView. */
  /* @TE2: data() should be equal to "The". */
  /* @End: UT_CORE_0242 */
  EXPECT_THAT(sv4, HasSize(3));
  EXPECT_THAT(sv4.data(), "The");
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0243
@Test_Description:    To test the functionality of substr of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611583
*******************************************************************************/
TEST(StringViewTest, ConstexprSubstr)
{
  constexpr StringView const sv1 = "The Quick Brown Fox";
  constexpr StringView const sv2 = sv1.substr(4, 5);
  EXPECT_THAT(sv2, HasSize(5));
  EXPECT_THAT(sv2.data(), StartsWith("Quick"));

  /* @Start: UT_CORE_0243 */
  /* @TS1: Create an instance of StringView as sv1 and pass parameter "The Quick Brown Fox". */
  /* @TE1: Instance sv1 should be created with passed parameter "The Quick Brown Fox". */
  /* @TS2: Create an instance for StringView as sv2 and pass parameter sv1.substr(4, 5) */
  /* @TE2: Instance sv2 should be created with passed parameter sv1.substr(4, 5). */
  /* @TS3: Invoke the HasSize of sv2. */
  /* @TE3: HasSize of sv should be 5 */
  /* @TS4: Invoke the data of sv2. */
  /* @TE4: Data of sv2 should be StartsWith("Quick"). */
  /* @End: UT_CORE_0243 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0244
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, ConstexprCompare)
{
  constexpr StringView const sv1 = "The Quick Brown Fox";
  constexpr StringView const sv2 = "Is The Quick Brown Fox Awake?";

  constexpr int res = sv2.compare(3, 19, sv1);
  EXPECT_EQ(res, 0);

  /* @Start: UT_CORE_0244 */
  /* @TS1: Create an instance of StringView as sv1 and pass parameter "The Quick Brown Fox". */
  /* @TE1: Instance sv1 should be created with passed parameter "The Quick Brown Fox". */
  /* @TS2: Create an instance for StringView as sv2 and pass parameter "Is The Quick Brown Fox Awake?" */
  /* @TE2: Instance sv2 should be created with passed parameter "Is The Quick Brown Fox Awake?". */
  /* @TS3: Create an instance of res as sv2 compare 3, 19, sv1 and compare. */
  /* @TE3: Instance sv2 should be created as sv2 and compared with 3, 19, sv1. */
  /* @TS4: Invoke the res. */
  /* @TE4: res should be 0. */
  /* @End: UT_CORE_0244 */
}

//For the below three testcases we need to write test specifications.
/*******************************************************************************
@TestCaseId:          UT_CORE_0500
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Fault Injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, Copy)
{
  char const text[] = "The Quick Brown Fox";

  StringView const sv1(text);

  constexpr size_t BUFSIZE = 64;
  char buf[BUFSIZE];
  std::fill(buf, buf + BUFSIZE, '\0');

  sv1.copy(buf, 5, 4);
  /* @Start: UT_CORE_0500 */
  /* @TS1: Invoke the copy() of StringView . */
  /* @TE1: copy() should be able to copy the data into StringView from the given range */
  /* @End: UT_CORE_0500 */
  EXPECT_THAT(buf, StartsWith("Quick"));
  EXPECT_THAT(buf[5], '\0');

  std::fill(buf, buf + BUFSIZE, '\0');

  sv1.copy(buf, 1000, 4);
  /* @Start: UT_CORE_0500 */
  /* @TS2: Invoke the copy() of StringView . */
  /* @TE2: copy() should be able to copy the data into StringView from the given range. */
  /* @End: UT_CORE_0500 */
  EXPECT_THAT(buf, StartsWith("Quick Brown Fox"));
  EXPECT_THAT(buf[15], '\0');

  std::fill(buf, buf + BUFSIZE, '\0');

  sv1.copy(buf, 0, 4);
  /* @Start: UT_CORE_0500 */
  /* @TS3: Invoke the copy() of StringView . */
  /* @TE3: copy() should be able to copy the data into StringView from the given range. */
  /* @End: UT_CORE_0500 */
  EXPECT_THAT(buf[0], '\0');

  EXPECT_THROW(sv1.copy(buf, 0, sv1.size() + 1), std::out_of_range);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0501
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, CompareWithStringView)
{
  // testing:
  // int compare(basic_string_view v) const noexcept;

  StringView const sv1("aaa");
  StringView const sv2("aba");
  /* @Start: UT_CORE_0501 */
  /* @TS1: Invoke compare() of StringView. */
  /* @TE1: compare() should return less than 0. */
  /* @End: UT_CORE_0501 */
  int const res1 = sv1.compare(sv2);
  EXPECT_THAT(res1, Lt(0));
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0502
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, StreamOperator)
{
  // testing:
  // std::ostream& operator<<(std::ostream& os, StringView const& v);

  // Make sure to use a StringView that is not null-terminated.
  char const text[] = "The Fox Jumps";
  StringView const sv(text + 4, 3);

  std::ostringstream out;
  out << sv;
  EXPECT_THAT(out.str(), Eq("Fox"));
  out.str(std::string());

  // change padding

  out << std::setw(5);
  out << sv;
  EXPECT_THAT(out.str(), Eq("  Fox"));
  out.str(std::string());

  // std::setw() sets minimum size, so it may overflow
  out << std::setw(2);
  out << sv;
  EXPECT_THAT(out.str(), Eq("Fox"));
  out.str(std::string());

  // Setting to 0 ensures that no padding is being done
  out << std::setw(0);
  out << sv;
  EXPECT_THAT(out.str(), Eq("Fox"));
  out.str(std::string());

  // The behavior when width is <0 is implementation-defined
  out << std::setw(-1);
  out << sv;
  EXPECT_THAT(out.str(), Eq("Fox"));
  out.str(std::string());

  // ----

  // change alignment (std::left/right/internal)
  // these are sticky
  out << std::left;
  out << std::setw(6);
  out << sv;
  EXPECT_THAT(out.str(), Eq("Fox   "));
  out.str(std::string());

  out << std::setw(7);
  out << sv;
  EXPECT_THAT(out.str(), Eq("Fox    "));
  out.str(std::string());

  out << std::internal;
  out << std::setw(8);
  out << sv;
  EXPECT_THAT(out.str(), Eq("     Fox"));
  out.str(std::string());

  out << std::right;
  out << std::setw(9);
  out << sv;
  EXPECT_THAT(out.str(), Eq("      Fox"));
  out.str(std::string());

  // ----

  // change fill character
  // this is sticky
  out << std::setw(5);
  out << std::setfill('*');
  out << sv;
  EXPECT_THAT(out.str(), Eq("**Fox"));
  out.str(std::string());

  out << std::setw(6);
  out << sv;
  EXPECT_THAT(out.str(), Eq("***Fox"));
  out.str(std::string());

  /* @Start: UT_CORE_0502 */
  /* @TS1: Invoke the stream operator for StringView. */
  /* @TE1: stream operators should support for the StringView. */
  /* @End: UT_CORE_0502 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0503
@Test_Description:    To test the functionality of cbegin() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611563
*******************************************************************************/
TEST(StringViewTest, cbegin)
{
  char const text[] = "The Fox Jumps";
  StringView const sv(text);
  const char* it = sv.cbegin();
  EXPECT_THAT(*it, 'T');
  EXPECT_THAT(it, text);
  /* @Start: UT_CORE_0503 */
  /* @TS1: Invoke cbegin() of StringView. */
  /* @TE1: cbegin() should return "The Fox Jumps". */
  /* @End: UT_CORE_0503 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0504
@Test_Description:    To test the functionality of cend() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611565
*******************************************************************************/
TEST(StringViewTest, cend)
{
  char const text[] = "The Fox Jumps";

  StringView const sv(text);
  const char* it = sv.cend();
  EXPECT_THAT(*it, '\0');
  /* @Start: UT_CORE_0504 */
  /* @TS1: Invoke cend() of StringView. */
  /* @TE1: cend() should point to the last '\0' of string. */
  /* @End: UT_CORE_0504 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0505
@Test_Description:    To test the functionality of rbegin() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611566
*******************************************************************************/
TEST(StringViewTest, rbegin)
{
  char const text[] = "The Fox Jumps";

  StringView const sv(text);
  StringView::ConstReverseIterator it = sv.rbegin();
  EXPECT_THAT(*it, 's');
  /* @Start: UT_CORE_0505 */
  /* @TS1: Invoke rbegin() of StringView. */
  /* @TE1: rbegin() should point to the last char of string. */
  /* @End: UT_CORE_0505 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0506
@Test_Description:    To test the functionality of crbegin() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611567
*******************************************************************************/
TEST(StringViewTest, crbegin)
{
  char const text[] = "The Fox Jumps";

  StringView const sv(text);
  StringView::ConstReverseIterator it = sv.crbegin();
  EXPECT_THAT(*it, 's');
  /* @Start: UT_CORE_0506 */
  /* @TS1: Invoke crbegin() of StringView. */
  /* @TE1: crbegin() should be point to the last char of string. */
  /* @End: UT_CORE_0506 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0507
@Test_Description:    To test the functionality of max_size() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611572
*******************************************************************************/
TEST(StringViewTest, max_size)
{
  char const text[] = "The Fox Jumps";

  StringView const sv(text);

  size_t ret = sv.max_size();
  EXPECT_THAT(ret, 18446744073709551615);
  /* @Start: UT_CORE_0507 */
  /* @TS1: Invoke max_size() of StringView. */
  /* @TE1: max_size() should return 18446744073709551615. */
  /* @End: UT_CORE_0507 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0508
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611584, 611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, compare1)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  StringView const sv2("The Quick Brown Fox");

  int ret = sv1.compare(1, 2, sv2);
  EXPECT_THAT(ret, 20);
  /* @Start: UT_CORE_0508 */
  /* @TS1: Invoke compare() of StringView. */
  /* @TE1: compare() should return 20. */
  /* @End: UT_CORE_0508 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0509
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, compare2)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  StringView const sv2("The Quick Brown Fox");
  int ret = sv1.compare(1, 2, sv2, 3, 4);

  EXPECT_THAT(ret, 72);
  /* @Start: UT_CORE_0509 */
  /* @TS1: Invoke compare() of StringView. */
  /* @TE1: compare() should return 72. */
  /* @End: UT_CORE_0509 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0510
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, compare3)
{
  char const text[] = "The Fox Jumps";
  const char* p = "The Fox Jumps";
  StringView const sv1(text);

  int ret = sv1.compare(p);

  EXPECT_THAT(ret, 0);
  /* @Start: UT_CORE_0510 */
  /* @TS1: Invoke compare() of StringView. */
  /* @TE1: compare() should return 0. */
  /* @End: UT_CORE_0510 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0511
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, compare4)
{
  char const text[] = "The Fox Jumps";
  const char* p = "The Fox Jumps";
  StringView const sv1(text);
  int ret = sv1.compare(1, 1, p);

  EXPECT_THAT(ret, 20);
  /* @Start: UT_CORE_0511 */
  /* @TS1: Invoke compare() of StringView. */
  /* @TE1: compare() should return 20. */
  /* @End: UT_CORE_0511 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0512
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, compare5)
{
  char const text[] = "The Fox Jumps";
  const char* p = "The Fox Jumps";
  StringView const sv1(text);

  int ret = sv1.compare(1, 1, p, 1);

  EXPECT_THAT(ret, 20);
  /* @Start: UT_CORE_0512 */
  /* @TS1: Invoke compare() of StringView. */
  /* @TE1: compare() should return 20. */
  /* @End: UT_CORE_0512 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0513
@Test_Description:    To test the functionality of compare() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611586, 611587, 611588, 611589
*******************************************************************************/
TEST(StringViewTest, basic_string_view)
{
  StringView sv1;
  /* @Start: UT_CORE_0513 */
  /* @TS1: To Create the empty StringView instance. */
  /* @TE1: Empty StringView instance should be created. */
  /* @End: UT_CORE_0513 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0514
@Test_Description:    To test the functionality of begin() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611562
*******************************************************************************/
TEST(StringViewTest, begin)
{
  char const text[] = "The Fox Jumps";
  StringView const sv(text);
  const char* it = sv.begin();
  EXPECT_THAT(*it, 'T');
  EXPECT_THAT(it, text);
  /* @Start: UT_CORE_0514 */
  /* @TS1: Invoke begin() of StringView. */
  /* @TE1: begin() should return "The Fox Jumps". */
  /* @End: UT_CORE_0514 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0515
@Test_Description:    To test the functionality of crend() of StringView.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611569
*******************************************************************************/
TEST(StringViewTest, crend)
{
  char const text[] = "TAhe Fox Jumps";

  StringView const sv(text);

  StringView::ConstReverseIterator it = sv.crend();
  /* @Start: UT_CORE_0515 */
  /* @TS1: Invoke crend() of StringView. */
  /* @TE1: crend() should return "The Fox Jumps". */
  /* @End: UT_CORE_0515 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0516
@Test_Description:    To test the functionality of operator==().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611922, 611923, 611924
*******************************************************************************/
TEST(StringViewTest, ComparatorOperator)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  StringView const sv2(text);

  EXPECT_THAT(sv1 == sv2, true);
  /* @Start: UT_CORE_0516 */
  /* @TS1: Invoke operator==() of StringView. */
  /* @TE1: operator==() should return true. */
  /* @End: UT_CORE_0516 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0517
@Test_Description:    To test the functionality of operator!=().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611925, 611926
*******************************************************************************/
TEST(StringViewTest, NotEqualToOperator)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  StringView const sv2(text);

  EXPECT_THAT(sv1 != sv2, false);
  /* @Start: UT_CORE_0517 */
  /* @TS1: Invoke operator!=() of StringView. */
  /* @TE1: operator!=() should return false. */
  /* @End: UT_CORE_0517 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0518
@Test_Description:    To test the functionality of operator==().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611922, 611923, 611924
*******************************************************************************/
TEST(StringViewTest, StringViewAndIdentity)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  ara::core::internal::Identity<StringView> sv2(sv1);

  EXPECT_THAT(sv1 == sv2, true);
  /* @Start: UT_CORE_0518 */
  /* @TS1: Invoke operator==() of StringView. */
  /* @TE1: operator==() should return true. */
  /* @End: UT_CORE_0518 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0519
@Test_Description:    To test the functionality of operator==().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611922, 611923, 611924
*******************************************************************************/
TEST(StringViewTest, IdentityAndStringView)
{
  char const text[] = "The Fox Jumps";
  char const text1[] = "The";
  StringView const sv1(text);
  ara::core::internal::Identity<StringView> sv2(text1);
  EXPECT_THAT(sv2 == sv1, false);
  /* @Start: UT_CORE_0519 */
  /* @TS1: Invoke operator==() of StringView. */
  /* @TE1: operator==() should return false. */
  /* @End: UT_CORE_0519 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0520
@Test_Description:    To test the functionality of operator!=().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611925, 611926
*******************************************************************************/
TEST(StringViewTest, NotEqualToStringViewAndIdentity)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  ara::core::internal::Identity<StringView> sv2;

  EXPECT_THAT(sv1 != sv2, true);
  /* @Start: UT_CORE_0520 */
  /* @TS1: Invoke operator!=() of StringView. */
  /* @TE1: operator!=() should return true. */
  /* @End: UT_CORE_0520 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0521
@Test_Description:    To test the functionality of operator!=().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611925, 611926
*******************************************************************************/
TEST(StringViewTest, NotEqualToIdentityAndStringView)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  ara::core::internal::Identity<StringView> sv2(sv1);
  EXPECT_THAT(sv2 != sv1, false);
  /* @Start: UT_CORE_0521 */
  /* @TS1: Invoke operator!=() of StringView. */
  /* @TE1: operator!=() should return false. */
  /* @End: UT_CORE_0521 */
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0522
@Test_Description:    To test the functionality of rend().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611568
*******************************************************************************/
TEST(StringViewTest, rend)
{
  char const text[] = "The Fox Jumps";

  StringView const sv(text);

  StringView::ConstReverseIterator it = sv.rend();
  /* @Start: UT_CORE_0522 */
  /* @TS1: Invoke rend() of StringView. */
  /* @TE1: rend() should return "The Fox Jumps". */
  /* @End: UT_CORE_0522 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0523
@Test_Description:    To test the functionality of operator<().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611928, 611929, 611930
*******************************************************************************/
TEST(StringViewTest, LessThanOperator)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  StringView const sv2(text);
  /* @Start: UT_CORE_0523 */
  /* @TS1: Invoke operator<() of StringView. */
  /* @TE1: operator<() should return false. */
  /* @End: UT_CORE_0523 */
  EXPECT_THAT(sv1 < sv2, false);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0524
@Test_Description:    To test the functionality of operator>().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611931, 611932, 611933
*******************************************************************************/
TEST(StringViewTest, GreaterThanOperator)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  StringView const sv2(text);
  /* @Start: UT_CORE_0524 */
  /* @TS1: Invoke operator>() of StringView. */
  /* @TE1: operator>() should return false. */
  /* @End: UT_CORE_0524 */
  EXPECT_THAT(sv1 > sv2, false);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0525
@Test_Description:    To test the functionality of operator<=().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611935, 611936
*******************************************************************************/
TEST(StringViewTest, LessThanEqualToOperator)
{
  char const text[] = "The Fox Jumps";

  StringView const sv1(text);
  StringView const sv2(text);
  /* @Start: UT_CORE_0525 */
  /* @TS1: Invoke operator<=() of StringView. */
  /* @TE1: operator<=() should return true. */
  /* @End: UT_CORE_0525 */
  EXPECT_THAT(sv1 <= sv2, true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0526
@Test_Description:    To test the functionality of operator>=().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611937, 611938
*******************************************************************************/
TEST(StringViewTest, GreaterThanEqualToOperator)
{
  char const text[] = "The Fox Jumps";
  StringView const sv1(text);
  StringView const sv2(text);
  /* @Start: UT_CORE_0526 */
  /* @TS1: Invoke operator>=() of StringView. */
  /* @TE1: operator>=() should return false. */
  /* @End: UT_CORE_0526 */
  EXPECT_THAT(sv1 >= sv2, true);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0527
@Test_Description:    To test the functionality of max_size().
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          StringViewTest
@Requirement Tracing: -
@Design Tracing:      611572
*******************************************************************************/
TEST(StringViewTest, max_size1)
{
  std::cout << "AA" << std::endl;
  char const text[] = "The Fox Jumps";

  StringView const sv(text);

  size_t ret = sv.max_size();
  /* @Start: UT_CORE_0527 */
  /* @TS1: Invoke max_size() of StringView. */
  /* @TE1: max_size() should return 18446744073709551615. */
  /* @End: UT_CORE_0527 */
  EXPECT_THAT(ret, 18446744073709551615);
}