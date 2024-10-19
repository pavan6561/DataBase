// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : application.cpp
//
// Purpose     : Intergration Testing implementation of core.
// -----------------------------------------------------------------------------------------------------------
#include <ara/core/vector.h>
#include <gtest/gtest.h>
#include <ara/core/steady_clock.h>
#include <ara/log/logging.h>

#include <chrono>
#include <csignal>

#include <atomic>
#include <thread>
#include <unistd.h>
#include <vector>
#include <application.h>

namespace it
{
namespace core
{
namespace application
{
CoreApp::CoreApp()
{
}

CoreApp::~CoreApp()
{
}

void CoreApp::Init()
{
  std::cout << "CoreApp2 Init()" << std::endl;
  sleep(5);
  for (std::uint8_t i = 0; i < 2; i++)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    IT_Core_0001();
  }
}
void CoreApp::IT_Core_0001()
{
  std::cout << "CoreApp2 IT_Core_0001()" << std::endl;
  ara::core::ipc::IpcMessage sendBuffer;
  uint32_t cmd{50};

  sendBuffer << ara::core::ipc::IpcMessageTypeToString<ara::core::ipc::IpcMessageType>(
                    ara::core::ipc::IpcMessageType::kSmHandkerAck)
             << cmd;
  smHandlerObj_.SendReceiveViaIpcSMChannel(sendBuffer);
}

} // namespace application
} // namespace core
} // namespace it
