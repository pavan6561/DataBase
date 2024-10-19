// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sm_handler.hpp
//
// Purpose     : This file provides SM Handler class declaration and definition of methods.
// -----------------------------------------------------------------------------------------------------------

#ifndef PHM_SOURCE_CORE_PROCESS_MANAGER_HEALTH_CHANNEL_INC_SM_HANDLER_HPP_
#define PHM_SOURCE_CORE_PROCESS_MANAGER_HEALTH_CHANNEL_INC_SM_HANDLER_HPP_

#include <ara/core/map.h>
#include <ara/log/logging.h>
#include <thread>
#include <memory>

#include "iceoryx_hoofs/cxx/convert.hpp"
#include "iceoryx_hoofs/cxx/helplets.hpp"
#include "iceoryx_hoofs/posix_wrapper/posix_access_rights.hpp"
#include "ipc_interface_user.hpp"
#include "ipc_interface_creator.hpp"
#include "ipc_message_types.hpp"
#include "version_info.hpp"
#include "iceoryx_hoofs/posix_wrapper/thread.hpp"
#include "iceoryx_hoofs/internal/concurrent/smart_lock.hpp"
#include <ipc/phm_ipc_message_types.hpp>
#include <ara/core/optional.h>
#include "ara/core/fixed_string.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Platform Health Management.
namespace phm
{
using namespace iox::units::duration_literals;

enum class RegSmHandler
{
  kWaitForUser,
  kFinished
};

class SMHandler
{
 public:
  /// @brief Constructor for the SMHandler class.
  SMHandler();

  /// @brief Destructor of the SMHandler class.
  ~SMHandler() noexcept;

  /// @brief Copy Constructor of SMHandler.
  SMHandler(SMHandler const&) = default;

  /// @brief Move Constructor of SMHandler.
  SMHandler(SMHandler&&) noexcept = default;

  /// @brief Copy Assignment operator of SMHandler.
  SMHandler& operator=(SMHandler const&) = default;

  /// @brief Move Assignment operator of SMHandler.
  SMHandler& operator=(SMHandler&&) = default;

  /// @brief Send and receive the  messages from the ipc interface user and creator.
  ara::core::Optional<ara::phm::PhmIpcMessageType> SendReceiveViaIpcSMChannel(
      const ara::core::ipc::IpcMessage& data) noexcept;

  /// @brief Ipc Channel Initialization for SMServer handler.
  void IpcSMHandlerChannelInit();

  void Run();

  void StartProcessRuntimeMessagesThread() noexcept;

  void ProcessRuntimeMessages() noexcept;

  //iox::cxx::optional<ara::phm::PHMErrorsErrc> SMRecoveryHandler(std::uint32_t executionError, ara::core::FixedString<ara::phm::kFixedStringMaxSize> functionGroupState, std::uint32_t supervisionType, uint32_t healthChannelRef);

  //iox::cxx::optional<ara::phm::PHMErrorsErrc> SMHealthChannel(uint32_t supervisionStatus, uint32_t healthChannelRef);

  //iox::cxx::optional<ara::phm::PHMErrorsErrc> SMGlobal(uint32_t supervisionStatus, uint32_t healthChannelRef);

 private:
  /// @brief This member Used to store the phm configuration.
  //const ara::phm::config::PhmConfig& phmConfig_;

  /// @brief To Create Ipc User.
  ara::core::ipc::IpcInterfaceUser m_ipcChannelUser;

  /// @brief To Create Ipc Creator.
  ara::core::ipc::IpcInterfaceCreator m_ipcChannelCreator;

  /// @brief To store thread runtime timeout.
  const iox::units::Duration m_runtimeMessagesThreadTimeout_ = 50_ms;

  /// @brief To store thread thread of optional type.
  iox::cxx::optional<iox::posix::Thread> smServerThread_;

  /// @brief Object of the Logger class.
  ara::log::Logger& logger_;

  bool exitRequested_{false};

  /// @brief Mutex object.
  std::mutex lock_;

  /// @brief To check ipc channel is initialized or not.
  bool isIpcHandlerChannelInit{false};
};
} // namespace phm
} // namespace ara

#endif // PHM_SOURCE_CORE_PROCESS_MANAGER_HEALTH_CHANNEL_INC_SM_HANDLER_HPP_