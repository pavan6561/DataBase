// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : timer_thread_test.cpp
//
// Purpose     : This file provides Unit Test cases for TimerThread class.
// -----------------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <gmock/gmock.h>

//#include "ara/core/timer/timer_thread.h"
#include "condition_notifier.h"
#include "condition_listener.h"

namespace ara
{
namespace core
{
TEST(Dummy, Dummy)
{
  iox::popo::ConditionVariableData condVarData;
  std::cout << "Dummy" << std::endl;
}
} // namespace core
} // namespace ara