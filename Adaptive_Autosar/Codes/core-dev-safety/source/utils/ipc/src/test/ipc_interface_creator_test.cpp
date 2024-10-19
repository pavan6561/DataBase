// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ipc_interface_creator_test.cpp
//
// Purpose     : This file contains Unit testing test cases for IpcInterfaceCreatorTest file.
// -----------------------------------------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "ipc_interface_creator.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::SetArgReferee;
namespace ara
{
namespace core
{
namespace ipc
{
/*******************************************************************************
@TestCaseId:          UT_CORE_0937
@Test_Description:    Validate IpcInterfaceCreator instance and constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcIneterfaceCreatorTest
@Requirement Tracing: -
@Design Tracing:      611174, 611175
*******************************************************************************/
TEST(IpcInterfaceCreator, IpcInterfaceCreator)
{
  /* @Start: UT_CORE_0937 */
  /* @TS1: Initialize IpcInterfaceCreator class instance. */
  /* @TE1.1: IpcInterfaceCreator class constructor should be called successfully.*/
  /* @End: UT_CORE_0937 */
  const RunTimeNameT runtimeName{"String"};
  const uint64_t maxMessages{100};
  const uint64_t messageSize{10};
  IpcInterfaceCreator obj(runtimeName, maxMessages, messageSize);
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0938
@Test_Description:    Validate CleanupResource function of IpcInterfaceCreator class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcIneterfaceCreatorTest
@Requirement Tracing: -
@Design Tracing:      611176
*******************************************************************************/
TEST(IpcInterfaceCreator, CleanupResource)
{
  /* @Start: UT_CORE_0938 */
  /* @TS1: Initialize IpcInterfaceCreator class instance. */
  /* @TE1.1: IpcInterfaceCreator class instance should invoke CleanupResource().*/
  /* @End: UT_CORE_0938 */
  const RunTimeNameT runtimeName{"String"};
  const uint64_t maxMessages{100};
  const uint64_t messageSize{10};
  IpcInterfaceCreator obj(runtimeName, maxMessages, messageSize);
  obj.CleanupResource();
}

} // namespace ipc
} // namespace core
} // namespace ara
