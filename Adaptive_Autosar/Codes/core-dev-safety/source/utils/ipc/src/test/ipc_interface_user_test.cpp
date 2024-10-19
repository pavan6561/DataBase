// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ipc_interface_user_test.cpp
//
// Purpose     : This file contains Unit testing test cases for IpcInterfaceUserTest file.
// -----------------------------------------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "ipc_message.hpp"
#include "ipc_interface_user.hpp"

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
@TestCaseId:          UT_CORE_0936
@Test_Description:    Validate IpcInterfaceUser instance and constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcInterfaceUserTest
@Requirement Tracing: -
@Design Tracing:      611172, 611173
*******************************************************************************/
TEST(IpcInterfaceUser, IpcInterfaceUser)
{
  /* @Start: UT_CORE_0936 */
  /* @TS1: Initialize IpcInterfaceUser class instance. */
  /* @TE1.1: IpcInterfaceUser class constructor should be called successfully.*/
  /* @End: UT_CORE_0936 */
  const RunTimeNameT name{""};
  const uint64_t maxMessages{};
  const uint64_t messageSize{};
  IpcInterfaceUser obj(name, maxMessages, messageSize);
  SUCCEED();
}

} // namespace ipc
} // namespace core
} // namespace ara
