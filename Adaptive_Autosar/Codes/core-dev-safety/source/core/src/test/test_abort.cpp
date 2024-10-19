// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_abort.cpp
//
// Purpose     : This file provides the unit test cases of class Abort.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/abort.h"
#include "gtest/gtest.h"

#include <csignal>
#include <csetjmp>
#include <iostream>

static struct sigaction g_originalHandler;
static volatile std::sig_atomic_t g_gotSigAbort;

static bool g_enteredAbortHandler;

static std::jmp_buf g_jmpbuf;

constexpr bool DEBUG = false;

template <typename... Args>
static void xtrace(Args const&... args)
{
  if (DEBUG)
  {
    std::cerr << "ABORT TEST: ";
    using expander = int[];
    static_cast<void>(expander{0, (static_cast<void>(std::cerr << args), 0)...});
    std::cerr << "\n";
  }
}

static void sigabrtHandler(int signo)
{
  static_cast<void>(signo);
  xtrace("caught SIGABRT");

  g_gotSigAbort = 1;
  std::longjmp(g_jmpbuf, 1);
}

static void MyHandler() noexcept
{
  xtrace("entering ", __func__);

  g_enteredAbortHandler = true;

  xtrace("exiting ", __func__);
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0375
@Test_Description:    Test the functionality of SetAbortHandler().
@Test_Type:           Fault Injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          AbortTest
@Requirement Tracing: -
@Design Tracing:      -
*******************************************************************************/
TEST(AbortTest, CustomAbortHandler_AbortSignalIsRaised)
{
  ara::core::AbortHandler oldHandler = ara::core::SetAbortHandler(MyHandler);
  static_cast<void>(oldHandler);

  xtrace("start");
  if (setjmp(g_jmpbuf) == 0)
  {
    xtrace("setting up setjmp");

    struct sigaction act
    {
    };
    act.sa_handler = sigabrtHandler;
    sigaction(SIGABRT, &act, &g_originalHandler);

    xtrace("calling Abort...");
    ara::core::Abort("foobar");

    // never reached, unless no SIGABRT is raised/caught
    xtrace("returned from Abort?!?");
  }
  else
  {
    xtrace("returned from signal handler via longjmp");
  }
  xtrace("end");

  sigaction(SIGABRT, &g_originalHandler, nullptr);
  /* @Start: UT_CORE_0375 */
  /* @TS1: Invoke the SetAbortHandler() and Abort(). */
  /* @TE1: g_enteredAbortHandler should be return true. */
  /* @End: UT_CORE_0371 */
  EXPECT_TRUE(g_enteredAbortHandler);
  EXPECT_EQ(g_gotSigAbort, 1);
}