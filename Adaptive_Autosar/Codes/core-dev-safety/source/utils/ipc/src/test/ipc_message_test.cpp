// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ipc_message_test.cpp
//
// Purpose     : This file contains Unit testing test cases for IpcMessageTest file.
// -----------------------------------------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <gtest/gtest.h>
#include "ipc_message.hpp"
#include "ipc_message_types.hpp"
namespace ara
{
namespace core
{
namespace ipc
{
/*******************************************************************************
@TestCaseId:          UT_CORE_2911
@Test_Description:    Validate IpcMessage instance and constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611193, 611194, 611205, 613181
*******************************************************************************/

TEST(IpcMessage, IpcMessage)
{
  /* @Start: UT_CORE_0911 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class constructor should be called successfully.*/
  /* @End: UT_CORE_0911 */
  const std::initializer_list<std::string> msg{"Avin"};
  IpcMessage obj(msg);
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_2912
@Test_Description:    Validate IpcMessage instance and constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611192, 611193, 611194, 613183
*******************************************************************************/
TEST(IpcMessage, IpcMessage_1)
{
  /* @Start: UT_CORE_0912 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class constructor should be called successfully.*/
  /* @End: UT_CORE_0912 */
  const std::string msg{"Avin"};
  IpcMessage obj(msg);
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0913
@Test_Description:    Validate GetNumberOfElements function of IpcMessage class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611196
*******************************************************************************/
TEST(IpcMessage, GetNumberOfElements)
{
  /* @Start: UT_CORE_0913 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class instance should invoke GetNumberOfElements().*/
  /* @End: UT_CORE_0913 */
  IpcMessage obj;
  obj.GetNumberOfElements();
  uint32_t m_numberOfElements{10};
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0914
@Test_Description:    Validate GetElementAtIndex, SetMessage functions of IpcMessage class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611197, 611201
*******************************************************************************/
TEST(IpcMessage, GetElementAtIndex)
{
  /* @Start: UT_CORE_0914 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class instance should invoke SetMessage().*/
  /* @TE1.2: IpcMessage class instance should invoke GetElementAtIndex().*/
  /* @End: UT_CORE_0914 */
  IpcMessage obj;
  const uint32_t index = 0;
  std::string msg = "Avin,Systems";
  obj.SetMessage(msg);
  obj.GetElementAtIndex(index);
  obj.GetElementAtIndex(10);
  EXPECT_EQ(obj.GetElementAtIndex(index), "Avin");
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0915
@Test_Description:    Validate IsValidEntry function of IpcMessage class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611198
*******************************************************************************/
TEST(IpcMessage, IsValidEntry)
{
  /* @Start: UT_CORE_0915 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class instance should invoke IsValidEntry().*/
  /* @End: UT_CORE_0915 */
  const std::string entry = "String";
  IpcMessage obj;
  obj.IsValidEntry(entry);
  ASSERT_EQ(obj.IsValidEntry(entry), 1);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0916
@Test_Description:    Validate AddEntry function of IpcMessage class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611203
*******************************************************************************/
TEST(IpcMessage, AddEntry)
{
  /* @Start: UT_CORE_0916 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class instance should invoke AddEntry().*/
  /* @End: UT_CORE_0916 */
  const std::string entry = "String,String";
  IpcMessage obj;
  obj.AddEntry(entry);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0917
@Test_Description:    Validate IsValid function of IpcMessage class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611198, 611199
*******************************************************************************/
TEST(IpcMessage, IsValid)
{
  /* @Start: UT_CORE_0917 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class instance should invoke IsValid().*/
  /* @End: UT_CORE_0917 */
  IpcMessage obj;
  obj.IsValid();
  ASSERT_EQ(obj.IsValid(), 1);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0918
@Test_Description:    Validate GetMessage function of IpcMessage class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611200, 611201
*******************************************************************************/
TEST(IpcMessage, GetMessage)
{
  /* @Start: UT_CORE_0918 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class instance should invoke IsValid().*/
  /* @End: UT_CORE_0918 */
  IpcMessage obj;
  obj.GetMessage();
  ASSERT_EQ(obj.GetMessage(), "");
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0919
@Test_Description:    Validate SetMessage function of IpcMessage class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611201
*******************************************************************************/
TEST(IpcMessage, SetMessage)
{
  /* @Start: UT_CORE_0919 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class instance should invoke SetMessage().*/
  /* @End: UT_CORE_0919 */
  const std::string msg = "string,string";
  IpcMessage obj;
  obj.SetMessage(msg);
  EXPECT_EQ(obj.GetNumberOfElements(), 0);
  const std::string msg1 = "";
  obj.SetMessage(msg1);
  EXPECT_EQ(obj.GetNumberOfElements(), 0);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0920
@Test_Description:    Validate ClearMessage function of IpcMessage class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611202, 611204
*******************************************************************************/
TEST(IpcMessage, ClearMessage)
{
  /* @Start: UT_CORE_0920 */
  /* @TS1: Initialize IpcMessage class instance. */
  /* @TE1.1: IpcMessage class instance should invoke ClearMessage().*/
  /* @End: UT_CORE_0920 */
  IpcMessage obj;
  obj.ClearMessage();
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0921
@Test_Description:    Validate operator function of IpcMessage class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcMessageTest
@Requirement Tracing: -
@Design Tracing:      611204
*******************************************************************************/
TEST(IpcMessage, operator)
{
  /* @Start: UT_CORE_0921 */
  /* @TS1: Initialize two instances of IpcMessage class. */
  /* @TE1.1: Both instances of IpcMessage class should be equal.*/
  /* @End: UT_CORE_0921 */
  IpcMessage obj, obj1;
  obj == obj1;
  ASSERT_EQ(obj, obj1);
}

} // namespace ipc
} // namespace core
} // namespace ara
