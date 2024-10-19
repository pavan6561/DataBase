// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ipc_interface_base_test.cpp
//
// Purpose     : This file contains Unit testing test cases for IpcInterfaceBaseTest file.
// -----------------------------------------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <gtest/gtest.h>
#include "version_info.hpp"
#include <algorithm>
#include "ipc_interface_base.hpp"
#include "iceoryx_hoofs/cxx/convert.hpp"
#include "iceoryx_hoofs/internal/posix_wrapper/message_queue.hpp"
#include "iceoryx_hoofs/internal/units/duration.hpp"
#include "ipc_logging.hpp"
#include "ipc_message.hpp"

namespace ara
{
namespace core
{
namespace ipc
{
/*******************************************************************************
@TestCaseId:          UT_CORE_0940
@Test_Description:    Validate IpcInterfaceBase instance and constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611177
*******************************************************************************/
TEST(IpcInterfaceBase, IpcInterfaceBase)
{
  /* @Start: UT_CORE_0940 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class constructor should be called successfully.*/
  /* @End: UT_CORE_0940 */
  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0941
@Test_Description:    Validate Receive function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611178
*******************************************************************************/
TEST(IpcInterfaceBase, Receive)
{
  /* @Start: UT_CORE_0941 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke Receive().*/
  /* @End: UT_CORE_0941 */

  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  core::ipc::IpcMessage answer;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.Receive(answer);
  EXPECT_EQ(obj.Receive(answer), false);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0942
@Test_Description:    Validate SetMessageFromString function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611191
*******************************************************************************/
TEST(IpcInterfaceBase, SetMessageFromString)
{
  /* @Start: UT_CORE_0942 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke SetMessageFromString().*/
  /* @End: UT_CORE_0942 */
  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  core::ipc::IpcMessage answer;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.SetMessageFromString("timeout", answer);
  EXPECT_EQ(obj.SetMessageFromString("timeout", answer), true);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0943
@Test_Description:    Validate SetMessageFromString function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611191
*******************************************************************************/
TEST(IpcInterfaceBase, setMessageFromString_error)
{
  /* @Start: UT_CORE_0943 */
  /* @TS1: Initialize IpcInterfaceBase Class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke SetMessageFromString().*/
  /* @End: UT_CORE_0943 */
  std::cout << __LINE__ << "SetMessageFromString" << std::endl;
  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  core::ipc::IpcMessage answer;
  answer.SetMessage("text");
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.SetMessageFromString("timeout", answer);
  EXPECT_EQ(obj.SetMessageFromString("timeout", answer), true);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0944
@Test_Description:    Validate Send function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611180
*******************************************************************************/
TEST(IpcInterfaceBase, Send)
{
  /* @Start: UT_CORE_0944 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke Send().*/
  /* @End: UT_CORE_0944 */
  RunTimeNameT runtimeName{"engine"};
  uint64_t maxMessages{2};
  uint64_t messageSize{200};
  core::ipc::IpcMessage msg;
  const iox::posix::IpcChannelError error{iox::posix::IpcChannelError::MESSAGE_TOO_LONG};
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.Send(msg);
  EXPECT_EQ(obj.Send(msg), 0);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0945
@Test_Description:    Validate CloseIpcChannel function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611185
*******************************************************************************/
TEST(IpcInterfaceBase, CloseIpcChannel)
{
  /* @Start: UT_CORE_0945 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke CloseIpcChannel().*/
  /* @End: UT_CORE_0945 */
  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.CloseIpcChannel();
  EXPECT_EQ(obj.CloseIpcChannel(), true);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0946
@Test_Description:    Validate Reopen function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611184
*******************************************************************************/
TEST(IpcInterfaceBase, Reopen)
{
  /* @Start: UT_CORE_0946 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke Reopen().*/
  /* @End: UT_CORE_0946 */
  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.Reopen();
  EXPECT_EQ(obj.Reopen(), 0);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0947
@Test_Description:    Validate IpcChannelMapsToFile function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611186
*******************************************************************************/
TEST(IpcInterfaceBase, IpcChannelMapsToFile)
{
  /* @Start: UT_CORE_0947 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke IpcChannelMapsToFile().*/
  /* @End: UT_CORE_0947 */
  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.IpcChannelMapsToFile();
  EXPECT_EQ(obj.IpcChannelMapsToFile(), true);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0948
@Test_Description:    Validate HasClosableIpcChannel function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611189
*******************************************************************************/
TEST(IpcInterfaceBase, HasClosableIpcChannel)
{
  /* @Start: UT_CORE_0948 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke HasClosableIpcChannel().*/
  /* @End: UT_CORE_0948 */
  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.HasClosableIpcChannel();
  EXPECT_NE(obj.HasClosableIpcChannel(), true);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0949
@Test_Description:    Validate CleanupOutdatedIpcChannel function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611190
*******************************************************************************/
TEST(IpcInterfaceBase, CleanupOutdatedIpcChannel)
{
  /* @Start: UT_CORE_0949 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke CleanupOutdatedIpcChannel().*/
  /* @End: UT_CORE_0949 */
  RunTimeNameT runtimeName;
  RunTimeNameT name = "";
  uint64_t maxMessages;
  uint64_t messageSize;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.CleanupOutdatedIpcChannel(name);
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0950
@Test_Description:    Validate TimedReceive function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611179
*******************************************************************************/
TEST(IpcInterfaceBase, TimedReceive)
{
  /* @Start: UT_CORE_0950 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke TimedReceive().*/
  /* @End: UT_CORE_0950 */
  RunTimeNameT runtimeName;
  RunTimeNameT name = "";
  uint64_t maxMessages;
  uint64_t messageSize;
  core::ipc::IpcMessage answer;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  const struct timeval value
  {
    500
  };
  const iox::units::Duration timeout(value);
  obj.TimedReceive(timeout, answer);
  EXPECT_EQ(obj.TimedReceive(timeout, answer), false);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0951
@Test_Description:    Validate TimedSend function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611181
*******************************************************************************/
TEST(IpcInterfaceBase, TimedSend)
{
  /* @Start: UT_CORE_0951 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke TimedSend().*/
  /* @End: UT_CORE_0951 */
  RunTimeNameT runtimeName;
  RunTimeNameT name = "";
  uint64_t maxMessages;
  uint64_t messageSize;
  const IpcMessage msg;
  core::ipc::IpcMessage answer;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  const struct timeval value
  {
    500
  };
  const iox::units::Duration timeout(value);
  obj.TimedSend(msg, timeout);
  EXPECT_EQ(obj.TimedSend(msg, timeout), false);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0952
@Test_Description:    Validate GetRuntimeName function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611182
*******************************************************************************/
TEST(IpcInterfaceBase, GetRuntimeName)
{
  /* @Start: UT_CORE_0952 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke GetRuntimeName().*/
  /* @End: UT_CORE_0952 */
  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.GetRuntimeName();
  EXPECT_EQ(obj.GetRuntimeName(), "");
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0953
@Test_Description:    Validate isInitialized function of IpcInterfaceBase class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceBaseTest
@Requirement Tracing: -
@Design Tracing:      611183
*******************************************************************************/
TEST(IpcInterfaceBase, IsInitialized)
{
  /* @Start: UT_CORE_0953 */
  /* @TS1: Initialize IpcInterfaceBase class instance. */
  /* @TE1.1: IpcInterfaceBase class instance should invoke IsInitialized().*/
  /* @End: UT_CORE_0953 */
  RunTimeNameT runtimeName;
  uint64_t maxMessages;
  uint64_t messageSize;
  IpcInterfaceBase obj(runtimeName, maxMessages, messageSize);
  obj.IsInitialized();
  EXPECT_EQ(obj.IsInitialized(), false);
}

} // namespace ipc
} // namespace core
} // namespace ara
