// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : application.h
//
// Purpose     : Intergration Testing implementation of core.
// -----------------------------------------------------------------------------------------------------------
#ifndef CORE_TEST_SCRIPTS_IT_MODULE_SCRIPTS_CORE_APP_INC_APPLICATION_H_
#define CORE_TEST_SCRIPTS_IT_MODULE_SCRIPTS_CORE_APP_INC_APPLICATION_H_

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <memory>
#include <thread>
#include <chrono>
#include <map>
#include <ara/core/span.h>
#include <ara/core/utility.h>
#include <ara/log/logging.h>
#include <ara/core/string.h>
#include <ara/core/vector.h>
#include <gtest/gtest.h>
#include <ara/core/initialization.h>
#include <ara/core/steady_clock.h>
#include "sm_handler.h"

using namespace std;

namespace it
{
namespace core
{
namespace application
{
class CoreApp
{
 public:
  CoreApp();

  ~CoreApp();

  void Init();

  void IT_Core_0001();

 private:
  ara::phm::SMHandler smHandlerObj_;

  ara::log::Logger& araLog_{
      ara::log::CreateLogger("COREAP", "Context for CORE ", ara::log::LogLevel::kVerbose)};
};

} // namespace application
} // namespace core
} // namespace it

#endif // CORE_TEST_SCRIPTS_IT_MODULE_SCRIPTS_CORE_APP_INC_APPLICATION_H_
