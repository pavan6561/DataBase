// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : application.cpp
//
// Purpose     : Test application for hanged application case
// -----------------------------------------------------------------------------------------------------------

#include <chrono>
#include <ctime>

#include <ara/exec/execution_client.h>
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
#include "supervised_entities/engine.h"
#include "health_channels/tyrepressure.h"
#include "ara/phm/supervised_entity.hpp"
#include "ara/phm/health_channel.hpp"

using namespace ara::core;
using namespace ara::phm;

// namespace with non-generated phm code
using namespace ara::phm;

enum class CheckPoint : uint32_t
{
  /// Supervision is active and no failure is present.
  Initial = 2
};

void handle_sigint(int sig)
{
  ara::exec::ExecutionClient execClient;
  printf("Caught signal %d\n", sig);

  if (execClient.ReportExecutionState(ara::exec::ExecutionState::kTerminating).HasValue())
  {
    std::cout << "Engine_app successfully reported ExecutionState::kTerminating.";
  }
}

/// @brief Function Group Lane Change Assistance On functions.
void Engine0(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  for (int i = 1; i <= 4; i++)
  {
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    logger.LogInfo() << "Checkpoint kInitializing reported(EngineAPP_0):: " << i << " - cycle time: " << 500
                     << "ms";
    engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kInitializing);
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
  }
}

void Engine1(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  for (int i = 1; i <= 3; i++)
  {
    logger.LogInfo() << "Checkpoint k2Initializing reported(EngineAPP_1):: " << i << " - cycle time: " << 330
                     << "ms";
    engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::k2Initializing);
    std::this_thread::sleep_for(std::chrono::milliseconds(330));
  }
}

void Engine10(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  for (int i = 1; i <= 3; i++)
  {
    logger.LogInfo() << "Checkpoint k2Initializing reported(EngineAPP_1):: " << i << " - cycle time: " << 330
                     << "ms";
    engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::k2Initializing);
    std::this_thread::sleep_for(std::chrono::milliseconds(330));
  }
}

void Engine3(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  for (int i = 1; i <= 1; i++)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1300));
    logger.LogInfo() << "Checkpoint kInitializing reported(EngineAPP_3):: " << i << " - cycle time: " << 1300
                     << "ms";
    engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kInitializing);
  }
}

void Engine4(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  for (int i = 1; i <= 3; i++)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(2050));
    logger.LogInfo() << "Checkpoint k2Initializing reported(EngineAPP_4):: " << i << " - cycle time: " << 2050
                     << "ms";
    engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::k2Initializing);
  }
}

void MT_PHM_009(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  logger.LogInfo() << "checkpoint kDeadlineOpen reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadlineOpen);
  std::this_thread::sleep_for(std::chrono::milliseconds(120));
  logger.LogInfo() << "checkpoint kDeadlineClose reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadlineClose);
}

void MT_PHM_014(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  logger.LogInfo() << "checkpoint kDeadlineOpen reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadlineOpen);
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  logger.LogInfo() << "checkpoint kDeadlineClose reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadlineClose);
}

void MT_PHM_015(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  logger.LogInfo() << "checkpoint kDeadlineOpen reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadlineOpen);
  std::this_thread::sleep_for(std::chrono::milliseconds(260));
  logger.LogInfo() << "checkpoint kDeadlineClose reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadlineClose);
}

void MT_PHM_028(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  logger.LogInfo() << "checkpoint kDeadlineOpen reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadlineOpen);
  std::this_thread::sleep_for(std::chrono::milliseconds(120));
  logger.LogInfo() << "checkpoint kDeadlineOpen reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadlineOpen);
}

void MT_PHM_010(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalOpen);
  logger.LogInfo() << "checkpoint kLogicalOpen reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalStuck);
  logger.LogInfo() << "checkpoint kLogicalStuck reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalClose);
  logger.LogInfo() << "checkpoint kLogicalClose reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void MT_PHM_018(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalOpen);
  logger.LogInfo() << "checkpoint kLogicalOpen reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalClose);
  logger.LogInfo() << "checkpoint kLogicalClose reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalStuck);
  logger.LogInfo() << "checkpoint kLogicalStuck reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalBroken);
  logger.LogInfo() << "checkpoint kLogicalBroken reported";
}

void MT_PHM_019(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalClose);
  logger.LogInfo() << "checkpoint kLogicalClose reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalOpen);
  logger.LogInfo() << "checkpoint kLogicalOpen reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalBroken);
  logger.LogInfo() << "checkpoint kLogicalBroken reported";
}

void MT_PHM_020(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalOpen);
  logger.LogInfo() << "checkpoint kLogicalOpen reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalClose);
  logger.LogInfo() << "checkpoint kLogicalClose reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalOpen);
  logger.LogInfo() << "checkpoint kLogicalOpen reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogicalBroken);
  logger.LogInfo() << "checkpoint kLogicalBroken reported";
}

/// @brief Function Group Lane Change Assistance Verify functions
void Engine2(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  logger.LogInfo() << "Verify Called";
  for (int i = 1; i <= 3; i++)
  {
    logger.LogInfo() << "Checkpoint kInitializing reported " << i << " - cycle time: " << 150 << "ms";
    engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kInitializing);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
  }
}

void Engine5(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  logger.LogInfo() << "Verify Called";
  for (int i = 1; i <= 6; i++)
  {
    logger.LogInfo() << "Checkpoint kInitializing1 reported " << i << " - cycle time: " << 1010 << "ms";
    engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kInitializing1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1010));
  }
}

void Engine6(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  logger.LogInfo() << "Verify Called";
  for (int i = 1; i <= 1; i++)
  {
    logger.LogInfo() << "Checkpoint kInitializing1 reported " << i << " - cycle time: " << 1010 << "ms";
    engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kInitializing1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1010));
  }
}

void MT_PHM_011(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  logger.LogInfo() << "checkpoint kDeadline2Open reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadline2Open);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  logger.LogInfo() << "checkpoint kDeadline2Close reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadline2Close);
}

void MT_PHM_017(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  logger.LogInfo() << "checkpoint kDeadline2Open reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadline2Open);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  logger.LogInfo() << "checkpoint kDeadline2Close reported";
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kDeadline2Close);
}

void MT_PHM_022(SupervisedEntity<supervised_entities::engine::Checkpoints>& engine, ara::log::Logger& logger)
{
  engine.ReportCheckpoint(supervised_entities::engine::Checkpoints::kLogical14);
  logger.LogInfo() << "checkpoint kLogical14 reported";
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

int main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  signal(SIGTERM, handle_sigint);

  ara::exec::ExecutionClient execClient;

  if (!execClient.ReportExecutionState(ara::exec::ExecutionState::kRunning).HasValue())
  {
    std::cout << "Engine_app failed to report ExecutionState::kRunning.";
    return 1;
  }
  else
  {
    std::cout << "Engine_app successfully reported ExecutionState::kRunning.";
  }

  ara::log::InitLogging("PHMDemo", "PHM demo Context", ara::log::LogLevel::kVerbose,
      (ara::log::LogMode::kRemote | ara::log::LogMode::kConsole));

  ara::log::Logger& logger = ara::log::LogManager::defaultLogContext();

  logger.LogInfo() << "Engine Application Started";

  std::this_thread::sleep_for(std::chrono::milliseconds(350));

  ara::core::InstanceSpecifier obj1(supervised_entities::engine::enginePrototype0);
  SupervisedEntity<supervised_entities::engine::Checkpoints> engine0(obj1);

  ara::core::InstanceSpecifier obj3(health_channels::tyrepressure::tyrePrototype0);
  HealthChannel<health_channels::tyrepressure::HealthStatuses> tyrePressureObj(obj3);

  tyrePressureObj.ReportHealthStatus(health_channels::tyrepressure::HealthStatuses::kLow);

  logger.LogInfo() << "Alive Supervision use case for SupervisedEntity engine ";

  if (std::strcmp(argv[1], "ON") == 0)
  {
    // Create two thread for supervision
    std::cout << "ON STATE FROM  APPLICATION::" << std::endl;
    std::thread t1(Engine0, std::ref(engine0), std::ref(logger));
    std::thread t2(Engine1, std::ref(engine0), std::ref(logger));
    std::thread t4(Engine3, std::ref(engine0), std::ref(logger));
    std::thread t5(Engine4, std::ref(engine0), std::ref(logger));

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    logger.LogInfo() << "Deadline Supervision use case for Supervised Entity engine (ON)";

    MT_PHM_009(std::ref(engine0), std::ref(logger));
    MT_PHM_014(std::ref(engine0), std::ref(logger));
    MT_PHM_015(std::ref(engine0), std::ref(logger));

    logger.LogInfo() << "Logical Supervision use case for Supervised Entity engine";
    MT_PHM_010(std::ref(engine0), std::ref(logger));
    MT_PHM_018(std::ref(engine0), std::ref(logger));
    MT_PHM_019(std::ref(engine0), std::ref(logger));
    MT_PHM_020(std::ref(engine0), std::ref(logger));
    MT_PHM_028(std::ref(engine0), std::ref(logger));
    t1.join();
    t2.join();
    t4.join();
    t5.join();

    logger.LogInfo() << "Incorrect Instance specifier passed";
    ara::core::InstanceSpecifier objN(
        ara::core::String("temperature_app/root_sw0/PhmSample/enginePrototype0"));
    SupervisedEntity<supervised_entities::engine::Checkpoints> engineN(objN);

    std::thread t10(Engine10, std::ref(engineN), std::ref(logger));
    t10.join();
  }
  else if (std::strcmp(argv[1], "VERIFY") == 0)
  {
    std::cout << "VERIFY STATE FROM  APPLICATION::" << std::endl;
    // Start execution for Function Group state Lane Change Assistance Verify
    std::this_thread::sleep_for(
        std::chrono::milliseconds(120)); // Note: Synchronisation is required instead of explicit sleep
    std::thread t3(Engine2, std::ref(engine0), std::ref(logger));
    std::thread t6(Engine5, std::ref(engine0), std::ref(logger));
    std::thread t7(Engine6, std::ref(engine0), std::ref(logger));
    logger.LogInfo() << "Deadline Supervision use case for Supervised Entity engine(VERIFY)";
    MT_PHM_011(std::ref(engine0), std::ref(logger));
    MT_PHM_017(std::ref(engine0), std::ref(logger));
    logger.LogInfo() << "Logical Supervision use case for Supervised Entity engine";
    logger.LogInfo() << "External Logical Supervision use case for Supervised Entity engine";
    MT_PHM_022(std::ref(engine0), std::ref(logger));
    MT_PHM_010(std::ref(engine0), std::ref(logger));
    t3.join();
    t6.join();
    t7.join();
  }
  else
  {
    /*Do Nothing*/
  }
  logger.LogInfo() << "Engine Thread processing for Alive supervisions ended!!";

  ara::core::Result<ara::phm::LocalSupervisionStatus> localSupervisionStatus
      = engine0.GetLocalSupervisionStatus();

  if (localSupervisionStatus.HasValue() == 1)
  {
    LocalSupervisionStatus localStatus = localSupervisionStatus.Value();

    switch (static_cast<uint32_t>(localStatus))
    {
    default:
    case 0:
      logger.LogInfo() << "Engine Local Supervision Status : OK";
      break;
    case 1:
      logger.LogInfo() << "Engine Local Supervision Status : FAILED";
      break;
    case 2:
      logger.LogInfo() << "Engine Local Supervision Status : EXPIRED";
      break;
    case 4:
      logger.LogInfo() << "Engine Local Supervision Status : DEACTIVATED";
      break;
    }
  }
  else
  {
    ara::core::ErrorCode obj = localSupervisionStatus.Error();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  ara::core::Result<GlobalSupervisionStatus> globalSupervisionStatus = engine0.GetGlobalSupervisionStatus();
  if (globalSupervisionStatus.HasValue() == 1)
  {
    GlobalSupervisionStatus globalStatus = globalSupervisionStatus.Value();
    switch (static_cast<uint32_t>(globalStatus))
    {
    default:
    case 0:
      logger.LogInfo() << "Engine GLOBAL Supervision Status : OK";
      break;
    case 1:
      logger.LogInfo() << "Engine GLOBAL Supervision Status : FAILED";
      break;
    case 2:
      logger.LogInfo() << "Engine GLOBAL Supervision Status : EXPIRED";
      break;
    case 3:
      logger.LogInfo() << "Engine GLOBAL Supervision Status : STOPPED";
      break;
    case 4:
      logger.LogInfo() << "Engine GLOBAL Supervision Status : DEACTIVATED";
      break;
    }
  }
  else
  {
    ara::core::ErrorCode obj = globalSupervisionStatus.Error();
  }

  while (1)
    ;
  return 0;
}
