
// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sm_server_daemon.hpp
//
// Purpose     : This file provides SMServer class declaration and definition of methods.
// -----------------------------------------------------------------------------------------------------------
#ifndef CORE_TEST_SCRIPTS_IT_MODULE_SCRIPTS_CORE_APP_INC_SM_SERVER_HPP_
#define CORE_TEST_SCRIPTS_IT_MODULE_SCRIPTS_CORE_APP_INC_SM_SERVER_HPP_

#include <ara/log/logging.h>
#include <ara/core/map.h>
#include <cstdint>
#include <cstdio>
#include <thread>
#include <mutex>

#include "iceoryx_hoofs/cxx/convert.hpp"
#include "iceoryx_hoofs/cxx/helplets.hpp"
#include "iceoryx_hoofs/posix_wrapper/posix_access_rights.hpp"
#include "ipc_interface_user.hpp"
#include "ipc_interface_creator.hpp"
#include "ipc_message_types.hpp"
#include "version_info.hpp"
#include "iceoryx_hoofs/posix_wrapper/thread.hpp"
#include <ipc/phm_ipc_message_types.hpp>
#include "ara/core/fixed_string.h"
#include <ara/core/fixedmap.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Platform Health Management.
namespace phm
{
  constexpr uint32_t kFixedMapSize {10U};
  constexpr uint32_t kStringSize {20U};
  using namespace iox::units::duration_literals;
  constexpr iox::units::Duration PROCESS_WAITING_FOR_ROUDI_TIMEOUT_IPC = 50_ms;

/// @brief Enum class for Register SmServer.
enum class RegSmServer : uint8_t
{
    kWaitForUser,
    kFinished
};

class RecoveryAction;

/// @brief HealthChannelAction class emplate.
/// @tparam EnumT An enum type that contains HealthChannelAction class.
template <typename EnumT>
class HealthChannelAction;

/// @brief Class for loading phm configurations and initialization of phm instances.
/// @archtrace 504119 504508 504115
/// @design 565286
class SMServer 
{
 public:
    /// @brief Constructor of the SMServer class.
    /// @returns Returns the IPCDaemon client.
    /// @archtrace 504119 504508 504115
    /// @design 565287
  static SMServer& GetInstance()
  {
    static SMServer pClient;
    return pClient;
  }

  /// @brief Copy constructor of the SMServer.
  SMServer(SMServer const&) = default;

  /// @brief Move constructor of the SMServer.
  SMServer(SMServer&&) noexcept = default;


  /// @brief Copy assignment operator of SMServer.
  SMServer& operator=(SMServer const&) = default;

  /// @brief Move assignment operator of SMServer.
  SMServer& operator=(SMServer&&) = default;

  /// @brief Destructor of the PCDeamon class.
  /// @archtrace 504119 504508 504115
  /// @design 565291
  virtual ~SMServer() noexcept;

  /// @brief Offers Recovery handler.
  /// @param[in] key Which releases the offer invocations of RecoveryHandler.
  /// @param[in] value Which stores the invocations to the HealthChannel.
  /// @archtrace 504119 504508 504115
  /// @design 565292
  void OfferRecoveryHandler( const ara::core::FixedString<kStringSize>& key, 
                        ara::phm::RecoveryAction * value )
  {
    // std::lock_guard<std::mutex> const lock{mutex_};
    // RecoveryActionMap.insert({key, value});
  }

  /// @brief Stops offer Recovery handler.
  /// @param[in] key Which stops the offer invocations of RecoveryHandler.
  /// @archtrace 504119 504508 504115
  /// @design 565293
  void StopOfferRecoveryHandler( const ara::core::FixedString<kStringSize>& key )
  {
    // Remove instance from recovery list.
    // std::lock_guard<std::mutex> const lock{mutex_};
    // RecoveryActionMap.erase(key);
  }

  /// @brief Template that contains the OfferHealthChannelAction.
  /// @tparam EnumT An enum type that contains HealthChannelAction class. 
  /// @brief Offers the HealthChannelAction class.
  /// @param[in] key Which releases the offer invocations to the HealthChannel.
  /// @param[in] value Which stores the invocations to the HealthChannel.
  /// @archtrace 504119 504508 504115
  /// @design 565294
  template <typename EnumT>
  void OfferHealthChannelAction( const ara::core::FixedString<kStringSize>& key, 
                ara::phm::HealthChannelAction<EnumT> * value )
  {
    // Add instance into recovery list.
    // std::lock_guard<std::mutex> lock{mutex_};
    // HealthChannelActionRecoveryMap<EnumT>.insert({key, value});
  }

  /// @brief Template that contains the OfferHealthChannelAction.
  /// @tparam EnumT An enum type that contains HealthChannelAction class. 
  /// @brief Stops Offers from the HealthChannelAction class.
  /// @param[in] key Which releases the offer invocations to the HealthChannel.
  /// @archtrace 504119 504508 504115
  /// @design 565295
  template <typename EnumT>
  void StopOfferHealthChannelAction( const ara::core::FixedString<kStringSize>& key )
  {
    // Remove instance from recovery list.
    // std::lock_guard<std::mutex> lock{mutex_};
    // HealthChannelActionRecoveryMap<EnumT>.erase(key);
  }

  /// @brief The StateManager local RecoveryAction.
  /// @param[in] pport Which contains the pport prototype for error recovery.
  /// @param[in] instance Create an instance of SMLocalRecovery.
  /// @param[in] executionError Used to store the execution errors.
  /// @param[in] supervisionType Used to store the types of supervision.  
  /// @archtrace 504119 504508 504115
  /// @design 565288
  void SMLocalRecovery(const ara::core::FixedString<kStringSize> pport,
                       const ara::core::FixedString<kStringSize> instance,
                       const uint32_t executionError, 
                       const uint32_t supervisionType);
 
  /// @brief Defines the StateManager local Health Channel. 
  /// @param[in] instance Create an instance of SMLocalHealthChannel.
  /// @param[in] status Used to store the status of SMLocalHealthChannel.
  /// @archtrace 504119 504508 504115
  /// @design 565289
  void SMLocalHealthChannel( ara::core::FixedString<kStringSize> const& instance, 
                             uint32_t smLocalStatus);

  /// @brief Sets the StateManager Global status.
  /// @param[in] pport Which contains the pport prototype for StateManager Global status.
  /// @param[in] status Used to store the status of SMLocalHealthChannel.
  /// @archtrace 504119 504508 504115  
  /// @design 565290
  void SMSetGlobal(const ara::core::FixedString<kStringSize> pport,
                   const uint32_t smLocalStatus);

 private:

  /// @brief To Run the SMServer.
  void Run();

  /// @brief To Shutdowns the SMServer.
  void Shutdown();

  /// @brief To Process the RuntimeMessages.
  void ProcessRuntimeMessages() noexcept;

  /// @brief To send acknowledgment to SM Handler .
  void SMServerAck(ara::core::ipc::IpcInterfaceUser* smInterfaceUser,uint32_t& cmd) const; 

  /// @brief Method to check whether the channel is reopened or not for communication.
  void IpcSMHandlerChannelInit(ara::core::ipc::IpcInterfaceUser* smInterfaceUser);
 
  /// @brief Logger_ is used to print the logs.
  ara::log::Logger& logger_;
  
  /// @brief To store thread runtime timeout.
  const iox::units::Duration m_runtimeMessagesThreadTimeout_=50_ms;

  /// @brief To store thread thread of optional type.
  iox::cxx::optional<iox::posix::Thread> smServerThread_;

  /// @brief Mutex for Construction
   std::mutex mutex_;

  /// @brief Exit from the requested messages.
  /// atomic insted of bool
   bool exitRequested_{false};

  /// @brief Exit from the requested messages.
   bool isIpcHandlerChannelInit{false};

  /// @brief Map to store Base Object of RecoveryActionHandler.
   ara::core::FixedMap<ara::core::FixedString<kStringSize>, 
      ara::phm::RecoveryAction *,kFixedMapSize> RecoveryActionMap;

  /// @brief Map to store Base object of HealthChannelAction.
  /// @tparam EnumT An enum type that contains HealthChannelAction class.
  /// @param[in] HealthChannelActionRecoveryMap It is reference to the HealthChannelAction.
  /// @archtrace 504119 504508 504115
  template <typename EnumT>
    static ara::core::FixedMap<ara::core::FixedString<kStringSize>, 
    ara::phm::HealthChannelAction<EnumT> *,kFixedMapSize> 
    HealthChannelActionRecoveryMap;

  /// @brief Optional thread for health status
  iox::cxx::optional<iox::posix::Thread> smRecoveryThread; 
  
  /// @brief Template class that contains Handles the HealthChannelHandler.
  /// @tparam EnumT An enum type that contains HealthChannelAction class. 
  /// @param[in] status Used to store status of the HealthChannelHandler. 
  /// @param[in] instance Instance of the HealthChannelHandler
  /// @archtrace 504119 504508 504115
  /// @design 565289
  template <typename EnumT>
   void HealthChannelHandler( uint32_t& smLocalStatus, 
      ara::core::FixedString<kStringSize> const& instance)
  {

    // typename std::pair<ara::core::FixedString<kStringSize>, 
    // ara::phm::HealthChannelAction<EnumT>*> *isConfigured 
    // = HealthChannelActionRecoveryMap<EnumT>.find(instance);

    // // Check existence
    // if (isConfigured != HealthChannelActionRecoveryMap<EnumT>.end())
    // {
    //   isConfigured->second-> HealthRecoveryHandler(smLocalStatus);
    // }
    // else
    // {
    //   std::cerr << "Health Channel <" << instance << "> Not Configured\n";
    // }

  }

  protected:
    /// @brief Starts the thread processing messages from the runtimes
    /// Once this is done, applications can register and PhmDaemon is fully operational.
	  /// @param[in] message Messages of IpcInterfaceUser.
    /// @param[in] user Is the reference to IpcInterfaceUser.
    /// @archtrace 504119 504508 504115  
    /// @design 565296
    virtual void ProcessMessage(const ara::core::ipc::IpcMessage& message,
        ara::core::ipc::IpcInterfaceUser* user) noexcept;

    /// @brief Start the Process of RuntimeMessagesThread.
    /// @archtrace 504119 504508 504115
    /// @design 565297
    void StartProcessRuntimeMessagesThread() noexcept;

    /// @brief Handles the IpcMessageError.
    /// @archtrace 504119 504508 504115 
    /// @design 565298
    void IpcMessageErrorHandler() noexcept;

    /// @brief Instance of an SMServer.
    /// @archtrace 504119 504508 504115
    /// @design 565299
    SMServer();

};

} // namespace phm
} // namespace ara

#endif // CORE_TEST_SCRIPTS_IT_MODULE_SCRIPTS_CORE_APP_INC_SM_SERVER_HPP_