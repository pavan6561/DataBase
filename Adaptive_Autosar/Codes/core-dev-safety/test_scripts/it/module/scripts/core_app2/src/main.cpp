// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : main.cpp
//
// Purpose     : This file provides functionality for Core Application
// -----------------------------------------------------------------------------------------------------------

#include <chrono>
#include <stdint.h>
#include <thread>
#include "stdio.h"

#include <ara/exec/execution_client.h>
#include <ara/log/logging.h>
#include <ara/log/logmanager.h>

#include "ara/core/initialization.h"
#include "application.h"

using namespace ara::log;

void CoreAppThread()
{
  it::core::application::CoreApp coreApp;

  coreApp.Init();

  while (1)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
}

int main(int argc, char* argv[])
{
  ara::log::InitLogging(
      "COREAP", "Core Application", LogLevel::kVerbose, (LogMode::kRemote | LogMode::kConsole));

  ara::log::Logger& logger = ara::log::LogManager::defaultLogContext();

  if (!ara::core::Initialize())
  {
    // No interaction with ARA is possible here since initialization failed
    logger.LogInfo() << "ara::core::Initialize() failed";
    return EXIT_FAILURE;
  }

  ara::exec::ExecutionClient exec_client;

  exec_client.ReportExecutionState(ara::exec::ExecutionState::kRunning);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  std::thread coreActivity(CoreAppThread);

  coreActivity.join();

  if (!ara::core::Deinitialize())
  {
    // No interaction with ARA is possible here since some ARA resources can be destroyed already
    logger.LogInfo() << "ara::core::Deinitialize() failed";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
