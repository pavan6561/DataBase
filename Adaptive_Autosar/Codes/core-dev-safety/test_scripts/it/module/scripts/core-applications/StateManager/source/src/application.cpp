// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : application.cpp
//
// Purpose     : Dummy SM application
// -----------------------------------------------------------------------------------------------------------

#include <chrono>
#include <ctime>

#include <ara/exec/state_client.h>
#include <ara/exec/execution_client.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <thread>
#include <typeinfo>
#include <unistd.h>

#include "ara/core/instance_specifier.h"
#include "ara/core/result.h"
#include "ara/log/logging.h"
#include "ara/log/logmanager.h"

#include "supervised_entities/statemanager.h"
#include "ara/phm/supervised_entity.hpp"
#include "ara/phm/health_channel_action.h"
#include "ara/phm/recovery_action.h"

using namespace ara::core;
using namespace ara::phm;

void handle_sigint(int sig)
{
  ara::exec::ExecutionClient execClient;
  printf("Caught signal %d\n", sig);

  if (execClient.ReportExecutionState(ara::exec::ExecutionState::kTerminating).HasValue())
  {
    std::cout << "SM successfully reported ExecutionState::kTerminating.\n";
    // return 1;
  }
}

/// @brief Function Group Machine State Alive Checkpoint Thread
void SM0(SupervisedEntity<supervised_entities::StateManager::Checkpoints>& sm, ara::log::Logger& logger)
{
  uint32_t i = 0;
  for (;;)
  {
    logger.LogInfo() << "SM - Checkpoint kInitializing reported " << i << " - cycle time: " << 500 << "ms";
    sm.ReportCheckpoint(supervised_entities::StateManager::Checkpoints::kInitializing);
    i += 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (i >= 2)
      break;
  }
}

bool ChangeFunctionGroupState(ara::core::String const& functionGroup, ara::core::String const& strState)
{
  std::cout << "Change request for functiongrp State to " << functionGroup << "." << strState << "\n";
  ara::exec::StateClient state_client;

  auto getMachineStateFuture{state_client.GetInitialMachineStateTransitionResult()};
  auto const getMachineStateResult{getMachineStateFuture.GetResult()};
  if (getMachineStateResult.HasValue())
  {
    ara::core::Result<ara::exec::FunctionGroup::CtorToken> const groupTokenResult{
        ara::exec::FunctionGroup::Preconstruct(functionGroup)};
    ara::exec::FunctionGroup::CtorToken groupToken(groupTokenResult.ValueOrThrow());
    ara::exec::FunctionGroup const group(std::move(groupToken));

    ara::core::Result<ara::exec::FunctionGroupState::CtorToken> const stateTokenResult{
        ara::exec::FunctionGroupState::Preconstruct(group, strState)};
    ara::exec::FunctionGroupState::CtorToken stateToken{stateTokenResult.ValueOrThrow()};
    ara::exec::FunctionGroupState const state{std::move(stateToken)};

    ara::core::Future<void> setStateFuture{state_client.SetState(state)};
    ara::core::Result<void> const setStateResult{setStateFuture.GetResult()};
    if (setStateResult.HasValue())
    {
      std::cout << "FunctionGroup State successfully changed to " << functionGroup << "." << strState << "\n";
      return true;
    }
    std::cout << " Error changing Machine State!" << setStateResult.Error().Message() << "\n";

    return false;
  }
  std::cout << "Error getting Machine State!" << getMachineStateResult.Error().Message() << "\n";

  return false;
}

class SMSample : public ara::phm::RecoveryAction
{
 public:
  SMSample(const ara::core::InstanceSpecifier& instance) : RecoveryAction(instance)
  {
  }

  void RecoveryHandler(
      const ara::exec::ExecutionErrorEvent& executionError, ara::phm::TypeOfSupervision supervision)
  {
    std::cout << "\nIn Recovery Handler of Sample Application" << std::endl;
    std::cout << "\nExecution Error in Sample Application  " << executionError.executionError << std::endl;
    std::cout << "\nFunction Group Name in Sample Application  "
              << executionError.functionGroup.GetMetaModelIdentifier() << std::endl;
    std::cout << "\nSupervision Type in Sample Application  " << static_cast<uint32_t>(supervision)
              << std::endl;

    ChangeFunctionGroupState("Lane_Change_Assistance", "Off");
    ChangeFunctionGroupState("Lane_Change_Assistance", "On");
  };

  ~SMSample()
  {
  }
};

template <typename T>
class SMHealthChannelSample : public ara::phm::HealthChannelAction<T>
{
 public:
  SMHealthChannelSample(const ara::core::InstanceSpecifier& instance) :
      ara::phm::HealthChannelAction<T>(instance)
  {
  }

  void RecoveryHandler(std::uint32_t healthStatus)
  {
    std::cout << "\nIn Recovery Handler for Health Channel Action of Sample Application" << std::endl;
    std::cout << "\nHealth Status from PHM " << healthStatus << std::endl;
  }

  ~SMHealthChannelSample()
  {
  }
};

int main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;

  signal(SIGTERM, handle_sigint);

  ara::exec::ExecutionClient execClient;

  if (!execClient.ReportExecutionState(ara::exec::ExecutionState::kRunning).HasValue())
  {
    std::cout << "\nSM failed to report ExecutionState::kRunning.";
    return 1;
  }
  else
  {
    std::cout << "\nSM successfully reported ExecutionState::kRunning.";
  }

  ara::log::InitLogging("PHMSM", "PHM SM SAMPLE APP", ara::log::LogLevel::kVerbose,
      (ara::log::LogMode::kRemote | ara::log::LogMode::kConsole));

  ara::log::Logger& logger = ara::log::LogManager::defaultLogContext();

  logger.LogInfo() << "SM Sample Application Started";

  ara::core::InstanceSpecifier obj("PhmSm_Executable/root_SW1/PhmSample/Phm_supervision_PPort");
  ara::core::InstanceSpecifier obj1("PhmSm_Executable/root_SW1/PhmSample/PhmHealthChannel_PPort");

  SMSample smobj(obj);
  smobj.Offer();

  SMHealthChannelSample<uint32_t> smhcobj(obj1);
  smhcobj.Offer();
  std::cout << "Offer from SM application:::: " << std::endl;

  ChangeFunctionGroupState("Lane_Change_Assistance", "Verify");
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  logger.LogInfo() << "SM - Thread processing for Alive supervisions ended!!";

#if 0
  ara::core::Result<LocalSupervisionStatus> localSupervisionStatus = sm0.GetLocalSupervisionStatus();
  std::cout<<"State Manager Application:: GetLocalSupervisionStatus   "<<localSupervisionStatus.HasValue()<<std::endl;
  if (localSupervisionStatus.HasValue()) {
    LocalSupervisionStatus localStatus = localSupervisionStatus.Value();
    switch( static_cast<uint32_t>(localStatus) )
    {
      case 0: logger.LogInfo() << "SM Local Supervision Status : OK";
      break;
      case 1: logger.LogInfo() << "SM Local Supervision Status : FAILED";
      break;
      case 2: logger.LogInfo() << "SM Local Supervision Status : EXPIRED";
      break;
      case 4: logger.LogInfo() << "SM Local Supervision Status : DEACTIVATED";
      break;
      default: logger.LogInfo() << "SM Local Supervision Status : UNDEFINED"; break;
    }
  } else {
    ara::core::ErrorCode obj = localSupervisionStatus.Error();
    std::cout << "Error Value of local " << obj.Value() << std::endl;
  }

  ara::core::Result<GlobalSupervisionStatus> globalSupervisionStatus = sm0.GetGlobalSupervisionStatus();
  if (globalSupervisionStatus.HasValue() == 1) {
    GlobalSupervisionStatus globalStatus = globalSupervisionStatus.Value();
    switch( static_cast<uint32_t>(globalStatus) )
    {
      case 0: logger.LogInfo() << "SM GLOBAL Supervision Status : OK";
      break;
      case 1: logger.LogInfo() << "SM GLOBAL Supervision Status : FAILED";
      break;
      case 2: logger.LogInfo() << "SM GLOBAL Supervision Status : EXPIRED";
      break;
      case 3: logger.LogInfo() << "SM GLOBAL Supervision Status : STOPPED";
      break;
      case 4: logger.LogInfo() << "SM GLOBAL Supervision Status : DEACTIVATED";
      break;
      default: logger.LogInfo() << "SM GLOBAL Supervision Status : UNDEFINED"; break;
    }
  } else {
    ara::core::ErrorCode obj = globalSupervisionStatus.Error();
    std::cout << "Error Value of global " << obj.Value() << std::endl;
  }

  // Note: For Test only 
  // Timings are known for maneuvering the execution

  ara::core::Result<ara::phm::type::GlobalSupervisionStatus> globalStatus = smobj.GetGlobalSupervisionStatus();
  switch( (globalStatus.Value()))
  {
    case ara::phm::type::GlobalSupervisionStatus::OK:
      logger.LogInfo() << "GLOBAL Supervision Status received from PHM" << " :   OK";
    break;
    case ara::phm::type::GlobalSupervisionStatus::FAILED:
      logger.LogInfo() << "GLOBAL Supervision Status received from PHM" << ":   FAILED";
    break;
    case ara::phm::type::GlobalSupervisionStatus::EXPIRED:
      logger.LogInfo() << "GLOBAL Supervision Status received from PHM" << ":   EXPIRED";
    break;
    case ara::phm::type::GlobalSupervisionStatus::STOPPED:
      logger.LogInfo() << "GLOBAL Supervision Status received from PHM" << ":   STOPPED";
    break;
    case ara::phm::type::GlobalSupervisionStatus::DEACTIVATED:
      logger.LogInfo() << "GLOBAL Supervision Status received from PHM" << ":   DEACTIVATED";
    break;
    default: logger.LogInfo() << "undefined global status";
    break;
  }

#endif

  std::this_thread::sleep_for(std::chrono::milliseconds(9000));
  ChangeFunctionGroupState("Lane_Change_Assistance", "Off");
  std::this_thread::sleep_for(std::chrono::milliseconds(600));
  ChangeFunctionGroupState("Lane_Change_Assistance", "Verify");
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));
  while (1)
    ;

  return 0;
}
