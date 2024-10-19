// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sm_handler.cpp
//
// Purpose     : Source file of SMHandler.
// -----------------------------------------------------------------------------------------------------------

#include "sm_handler.h"
#include "iceoryx_hoofs/cxx/optional.hpp"

namespace ara
{
namespace phm
{
constexpr iox::units::Duration PROCESS_WAITING_FOR_ROUDI_TIMEOUT_IPC{100_ms};
using namespace iox::units::duration_literals;

SMHandler::SMHandler() :
    m_ipcChannelUser("Phm-State-Server2"),
    m_ipcChannelCreator("Phm-State-Client2"), logger_{ara::log::CreateLogger("SMH",
                                                  "SM Hanlder Context Context", ara::log::LogLevel::kVerbose)}
{
  Run();
}

SMHandler::~SMHandler() noexcept
{
}

// Method to send and receive messages through IPC channel.
ara::core::Optional<ara::phm::PhmIpcMessageType> SMHandler::SendReceiveViaIpcSMChannel(
    const ara::core::ipc::IpcMessage& data) noexcept
{
  std::unique_lock<std::mutex> l{lock_};
  if (!isIpcHandlerChannelInit)
  {
    IpcSMHandlerChannelInit();
  }

  ara::core::ipc::IpcMessage message;
  uint32_t cmd{0};
  ara::core::Optional<ara::phm::PhmIpcMessageType> result;
  if (isIpcHandlerChannelInit == true)
  {
    if (!m_ipcChannelUser.TimedSend(data, PROCESS_WAITING_FOR_ROUDI_TIMEOUT_IPC))
    {
      logger_.LogError() << "SM handler Could not Sent request via  IPC channel interface.";
      result = ara::core::make_optional<ara::phm::PhmIpcMessageType>(
          ara::phm::PhmIpcMessageType::SMMESSAGENOTSENT);
    }
    else
    {
      logger_.LogDebug() << "COREAPP2:: Message sent to SM Server with command " << data.GetElementAtIndex(0);
    }
  }
}

void SMHandler::IpcSMHandlerChannelInit()
{
  /// @todo: Change iox::cxx to ara::core.
  iox::cxx::DeadlineTimer timer{500_ms};

  while ((!timer.hasExpired()) && (!isIpcHandlerChannelInit))
  {
    if (!m_ipcChannelUser.IsInitialized())
    {
      logger_.LogDebug() << "COREAPP2: Reopen SM Handler IPC channel!";
      m_ipcChannelUser.Reopen();
    }
    else
    {
      isIpcHandlerChannelInit = true;
      logger_.LogDebug() << "COREAPP2: SM Server IPC channel found!";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

void SMHandler::Run()
{
  StartProcessRuntimeMessagesThread();
  const std::chrono::steady_clock::time_point next_run(std::chrono::steady_clock::now());
  const std::chrono::steady_clock::duration duration(std::chrono::milliseconds(500));
}

void SMHandler::StartProcessRuntimeMessagesThread() noexcept
{
  iox::posix::ThreadBuilder()
      .create(smServerThread_, std::bind(&SMHandler::ProcessRuntimeMessages, this))
      .expect("Couldn't create a thread.");
}

void SMHandler::ProcessRuntimeMessages() noexcept
{
  std::cout << "COREAPP2:: Inside the ProcessRuntimeMessage" << std::endl;

  while (!exitRequested_)
  {
    // Read SMHandler's IPC channel
    ara::core::ipc::IpcMessage message;

    if (m_ipcChannelCreator.TimedReceive(m_runtimeMessagesThreadTimeout_, message))
    {
      logger_.LogDebug() << "COREAPP2:: Message Received in SM Server with command as : "
                         << message.GetElementAtIndex(1);
    }
  }
}

} // namespace phm
} // namespace ara