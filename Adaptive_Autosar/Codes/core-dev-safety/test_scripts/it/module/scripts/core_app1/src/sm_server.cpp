// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sm_server.cpp
//
// Purpose     : This file provides SMServer class declaration and definition,Job of SMServer server to
// invoke recovery handler requested by phm and provide the acknowledement back to phm.
// -----------------------------------------------------------------------------------------------------------

#include "sm_server.hpp"
#include <chrono>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace ara
{
namespace phm
{
SMServer::SMServer() :
    logger_{
        ara::log::CreateLogger("sm_server_daemon", "SM_SERVER_DAEMON Context", ara::log::LogLevel::kVerbose)}
{
  Run();
}

SMServer::~SMServer()
{
  exitRequested_ = true;
}

void SMServer::Run()
{
  StartProcessRuntimeMessagesThread();
  const std::chrono::steady_clock::time_point next_run(std::chrono::steady_clock::now());
  const std::chrono::steady_clock::duration duration(std::chrono::milliseconds(500));
}

void SMServer::StartProcessRuntimeMessagesThread() noexcept
{
  iox::posix::ThreadBuilder()
      .create(smServerThread_, std::bind(&SMServer::ProcessRuntimeMessages, this))
      .expect("Couldn't create a thread.");
}

void SMServer::ProcessRuntimeMessages() noexcept
{
  std::cout << "COREAPP1:: Inside the ProcessRuntimeMessage" << std::endl;
  const ara::core::ipc::IpcInterfaceCreator roudiIpcInterface{"Phm-State-Server2"};
  ara::core::ipc::IpcInterfaceUser m_ipcChannelUser{"Phm-State-Client2"};

  while (!exitRequested_)
  {
    // Read SMServer's IPC channel
    ara::core::ipc::IpcMessage message;

    if (roudiIpcInterface.TimedReceive(m_runtimeMessagesThreadTimeout_, message))
    {
      logger_.LogDebug() << "COREAPP1:: Message Received in SM Server with command as : "
                         << message.GetElementAtIndex(1);
      ProcessMessage(message, &m_ipcChannelUser);
    }
  }
}

void SMServer::ProcessMessage(
    const ara::core::ipc::IpcMessage& message, ara::core::ipc::IpcInterfaceUser* ipcSMUser) noexcept
{
  // Waiting for server to connect within configured timeout.
  if (isIpcHandlerChannelInit == false)
  {
    IpcSMHandlerChannelInit(ipcSMUser);
  }
  uint32_t executionError{};
  uint32_t supervisionType{};
  uint32_t status{};
  ara::core::FixedString<kStringSize> instance{};
  ara::core::FixedString<kStringSize> pport{};

  uint32_t cmd{0};
  /// @todo: Change iox::cxx to ara::core.
  iox::cxx::convert::fromString(message.GetElementAtIndex(0).c_str(), cmd);
  std::cout << "APPCORE2 ::ProcessMessage  " << cmd << std::endl;
  EXPECT_EQ(0, 0);
  ara::core::ipc::RunTimeNameT runtimeName(iox::cxx::TruncateToCapacity, message.GetElementAtIndex(1));

  if (isIpcHandlerChannelInit == true)
  {
    SMServerAck(ipcSMUser, cmd);
  }
}

void SMServer::IpcSMHandlerChannelInit(ara::core::ipc::IpcInterfaceUser* m_ipcChannelUser)
{
  /// @todo: Change iox::cxx to ara::core.
  iox::cxx::DeadlineTimer timer{500_ms};
  while ((!timer.hasExpired()) && (!isIpcHandlerChannelInit))
  {
    if (!m_ipcChannelUser->IsInitialized())
    {
      logger_.LogDebug() << "COREAPP1:: Reopen PHM-SM Handler IPC channel!";
      m_ipcChannelUser->Reopen();
    }
    else
    {
      isIpcHandlerChannelInit = true;
      logger_.LogDebug() << "COREAPP1:: PHM-SM Server IPC channel found!";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}
void SMServer::SMLocalRecovery(const ara::core::FixedString<kStringSize> pport,
    const ara::core::FixedString<kStringSize> instance, const uint32_t executionError,
    const uint32_t supervisionType)
{
}

void SMServer::SMLocalHealthChannel(
    ara::core::FixedString<kStringSize> const& instance, uint32_t smLocalStatus)
{
}

void SMServer::SMSetGlobal(const ara::core::FixedString<kStringSize> pport, const uint32_t smLocalStatus)
{
}

void SMServer::Shutdown()
{
  exitRequested_ = true;
}

void SMServer::SMServerAck(ara::core::ipc::IpcInterfaceUser* smInterfaceUser, uint32_t& cmd) const
{
  constexpr iox::units::Duration SM_SERVER_HANDLER_TIMEOUT_IPC = 60_ms;
  ara::core::ipc::IpcMessage sendBuffer;
  sendBuffer << ara::core::ipc::IpcMessageTypeToString<ara::core::ipc::IpcMessageType>(
                    ara::core::ipc::IpcMessageType::kSmHandkerAck)
             << cmd;

  if (!smInterfaceUser->TimedSend(sendBuffer, SM_SERVER_HANDLER_TIMEOUT_IPC))
  {
    logger_.LogError() << "COREAPP1:: SM Server Could not Sent request via  IPC channel interface.";
  }
  else
  {
    logger_.LogDebug() << "COREAPP1:: Message sent to SM Server Handler for requested command " << cmd;
  }
}

} // namespace phm
} // namespace ara