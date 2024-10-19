// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sm_config.cpp
//
// Purpose     : Source file of SmConfig Class.
// -----------------------------------------------------------------------------------------------------------

#include "sm_config.h"
#include <ara/core/array.h>

namespace ara
{
namespace sm
{
namespace config
{
using ara::core::configreader::value;

SmConfig::SmConfig() noexcept :
    updateSessionTimeout_{}, resetRequestTimeout_{}, powerOnFunctionGrp_{}, diagnosticAddress_{},
    networkHandler_{}, powerModes_{}, triggerIn_{}, triggerOut_{},
    triggerInOut_{}, logger_{ara::log::CreateLogger("PCFG", "SMCONFIG Context", ara::log::LogLevel::kVerbose)}

{
}

void SmConfig::LoadConfig(std::string fileName) noexcept
{
  void (*const smConfigFun)(ara::sm::config::SmConfig&, ara::core::configreader::value const,
      ara::core::configreader::BufferContext&)
      = [](ara::sm::config::SmConfig& sConfigObj, ara::core::configreader::value val,
            ara::core::configreader::BufferContext& context) {
          ParseObject(context, [&](ara::core::StringView view, ara::core::configreader::value val) {
            if (view == "PowerOnFunctionGrp")
            {
              sConfigObj.LoadPowerOnFunctionGrpConfig(context);
            }

            if (view == "UpdateSessionTimeout")
            {
              sConfigObj.updateSessionTimeout_ = val.As<uint32_t>();
            }

            if (view == "ResetRequestTimeout")
            {
              sConfigObj.resetRequestTimeout_ = val.As<uint32_t>();
            }

            if (view == "DiagnosticAddress")
            {
              sConfigObj.LoadDiagnosticAddressConfig(context);
            }

            if (view == "NetworkHandler")
            {
              sConfigObj.LoadNetworkHandlerConfig(context);
            }

            if (view == "PowerModes")
            {
              sConfigObj.LoadPowerMode(context);
            }

            if (view == "TriggerIn")
            {
              sConfigObj.LoadTriggerIn(context);
            }

            if (view == "TriggerOut")
            {
              sConfigObj.LoadTriggerOut(context);
            }
            if (view == "TriggerInOut")
            {
              sConfigObj.LoadTriggerInOut(context);
            }
          });
        };
  // Handler Object for Phm Configutaion.
  ara::core::configreader::handlers::Handler<void (*)(ara::sm::config::SmConfig&, value,
      ara::core::configreader::BufferContext & context)> const smHandler{"SMConfiguration", smConfigFun};

  const ara::core::configreader::Dispatcher<ara::core::configreader::handlers::Handler<void (*)(
      ara::sm::config::SmConfig&, value, ara::core::configreader::BufferContext & context)>>
      smDispatcher{smHandler};

  // File Reading
  fStream_.open(fileName);

  if (fStream_.is_open())
  {
    /// @todo C-style array need to be replaced with ara::core::Array.
    char jsonData[100000];
    fStream_.read(jsonData, sizeof(jsonData));
    fStream_.close();
    // Context for Json Buffer.
    ara::core::configreader::BufferContext context{jsonData, sizeof(jsonData)};
    smDispatcher.Run<ara::core::configreader::BufferContext, ara::sm::config::SmConfig>(context, *this);
    ConsoleInfo();
  }
}

void SmConfig::ConsoleInfo() noexcept
{
  for (const PowerOnFunctionGrp& functionGrp : powerOnFunctionGrp_)
  {
    logger_.LogDebug() << "poweron function group sm config" << functionGrp.function_group_name;
    logger_.LogDebug() << "poweron function group sm config" << functionGrp.function_group_state;
  }

  logger_.LogDebug() << updateSessionTimeout_;

  logger_.LogDebug() << resetRequestTimeout_;
  for (DiagnosticAddress& diagAddress : diagnosticAddress_)
  {
    logger_.LogDebug() << "address" << diagAddress.address;
    for (const ara::sm::config::FunctionGroup& fg : diagAddress.functionGrp)
    {
      logger_.LogDebug() << "functiongroup" << fg.function_group_name;
      logger_.LogDebug() << "functiongreoupstates" << fg.function_group_state;
    }
  }
  for (NetworkHandler& network : networkHandler_)
  {
    logger_.LogDebug() << "networkHandler" << network.handleName;
    logger_.LogDebug() << "networkHandler.instance_id" << network.instanceId;
    for (const ara::core::FixedString<ara::sm::kSmMaxMachineState> fg : network.machineStates)
    {
      logger_.LogDebug() << "networkHandler machineStates state " << fg;
    }
    for (const ara::core::FixedString<ara::sm::kSmMaxFgState> fg : network.functionGroupStates)
    {
      logger_.LogDebug() << "networkHandler functiongroup state " << fg;
    }
    logger_.LogDebug() << "networkHandler afterrun_timeout_ns " << network.afterRunTimeOut;
  }
  for (PowerModes& powermode : powerModes_)
  {
    logger_.LogDebug() << "Power mode instance id " << powermode.instance_id;
    logger_.LogDebug() << "Power mode function group name " << powermode.function_group_name;
  }
  for (TriggerIn& triggerIn : triggerIn_)
  {
    logger_.LogDebug() << "triggerIn instance specifier name " << triggerIn.instanceSpecifier;
    logger_.LogDebug() << "triggerIn functiongroup " << triggerIn.functionGroupName;
  }
  for (TriggerOut& triggerOut : triggerOut_)
  {
    logger_.LogDebug() << "triggerOut instance specifier name " << triggerOut.instanceSpecifier;
    logger_.LogDebug() << "triggerOut functiongroup " << triggerOut.functionGroupName;
  }
  for (TriggerInOut& triggerIOVect : triggerInOut_)
  {
    logger_.LogDebug() << "triggerInOut instance specifier name " << triggerIOVect.instanceSpecifier;
    logger_.LogDebug() << "triggerInOut functiongroup " << triggerIOVect.functionGroupName;
  }
}

// To load the config data of PowerOnFunctionGrp.
void SmConfig::LoadPowerOnFunctionGrpConfig(ara::core::configreader::BufferContext& context) noexcept
{
  ParseArray(context, [&](value val) {
    static_cast<void>(val);
    ara::sm::config::PowerOnFunctionGrp poweronconfig;
    ParseObject(context, [&](ara::core::StringView fgView, value fgVal) {
      if (fgView == "function_group_name")
      {
        ara::core::FixedString<ara::sm::kSmMaxFgName> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        poweronconfig.function_group_name = std::move(localFixedString);
      }
      if (fgView == "function_group_state")
      {
        ara::core::FixedString<ara::sm::kSmMaxFgState> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        poweronconfig.function_group_state = std::move(localFixedString);
      }
    });
    powerOnFunctionGrp_.push_back(poweronconfig);
  });
}

// To load the config data of DiagnosticAddress.
void SmConfig::LoadDiagnosticAddressConfig(ara::core::configreader::BufferContext& context) noexcept
{
  ParseArray(context, [&](value val) {
    static_cast<void>(val);
    ara::sm::config::DiagnosticAddress diagconfig;
    ParseObject(context, [&](ara::core::StringView fgView, value fgVal) {
      if (fgView == "address")
      {
        ara::core::FixedString<ara::sm::kSmDiagAddress> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        diagconfig.address = std::move(localFixedString);
      }
      if (fgView == "function_group_states")
      {
        ParseArray(context, [&](value v) {
          static_cast<void>(v);
          ara::sm::config::FunctionGroup diagfunconfig;
          ParseObject(context, [&](ara::core::StringView fgView, value fgVal) {
            if (fgView == "function_group_name")
            {
              ara::core::FixedString<ara::sm::kSmMaxFgName> localFixedString(
                  ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
              diagfunconfig.function_group_name = std::move(localFixedString);
            }
            if (fgView == "function_group_state")
            {
              ara::core::FixedString<ara::sm::kSmMaxFgState> localFixedString(
                  ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
              diagfunconfig.function_group_state = std::move(localFixedString);
            }
          });
          diagconfig.functionGrp.push_back(diagfunconfig);
        });
      }
    });
    diagnosticAddress_.push_back(diagconfig);
  });
}

// To load the config data of NetworkHandler.
void SmConfig::LoadNetworkHandlerConfig(ara::core::configreader::BufferContext& context) noexcept
{
  ParseArray(context, [&](value v) {
    static_cast<void>(v);
    ara::sm::config::NetworkHandler networkhandlerconfig;
    ParseObject(context, [&](ara::core::StringView networkhandView, value ntVal) {
      if (networkhandView == "name")
      {
        ara::core::FixedString<ara::sm::kSmNMHandleName> localFixedString(
            ara::core::TruncateToCapacity, ntVal.As<ara::core::StringView>().data());
        networkhandlerconfig.handleName = std::move(localFixedString);
      }
      if (networkhandView == "instance_id")
      {
        ara::core::FixedString<ara::sm::kSmMaxInstanceId> localFixedString(
            ara::core::TruncateToCapacity, ntVal.As<ara::core::StringView>().data());
        networkhandlerconfig.instanceId = std::move(localFixedString);
      }
      if (networkhandView == "machine_states")
      {
        ParseArray(context, [&](value v) {
          static_cast<void>(v);
          ara::core::FixedString<ara::sm::kSmMaxMachineState> localFixedString(
              ara::core::TruncateToCapacity, v.As<ara::core::StringView>().data());
          networkhandlerconfig.machineStates.push_back(std::move(localFixedString));
        });
      }
      if (networkhandView == "function_group_states")
      {
        ParseArray(context, [&](value val) {
          static_cast<void>(val);
          ara::core::FixedString<ara::sm::kSmMaxFgState> localFixedString(
              ara::core::TruncateToCapacity, val.As<ara::core::StringView>().data());
          networkhandlerconfig.functionGroupStates.push_back(std::move(localFixedString));
        });
      }
      if (networkhandView == "afterrun_timeout_ms")
      {
        networkhandlerconfig.afterRunTimeOut = ntVal.As<uint32_t>();
      }
    });
    networkHandler_.push_back(networkhandlerconfig);
  });
}

// To load the config data of PowerModes.
void SmConfig::LoadPowerMode(ara::core::configreader::BufferContext& context) noexcept
{
  ParseArray(context, [&](value val) {
    static_cast<void>(val);
    ara::sm::config::PowerModes powermodeconfig;
    ParseObject(context, [&](ara::core::StringView fgView, value fgVal) {
      if (fgView == "instance_id")
      {
        ara::core::FixedString<ara::sm::kSmMaxInstanceId> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        powermodeconfig.instance_id = std::move(localFixedString);
      }
      if (fgView == "function_group_name")
      {
        ara::core::FixedString<ara::sm::kSmMaxFgName> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        powermodeconfig.function_group_name = std::move(localFixedString);
      }
    });
    powerModes_.push_back(powermodeconfig);
  });
}

void SmConfig::LoadTriggerIn(ara::core::configreader::BufferContext& context) noexcept
{
  ParseArray(context, [&](value val) {
    static_cast<void>(val);
    ara::sm::config::TriggerIn inconfig;
    ParseObject(context, [&](ara::core::StringView fgView, value fgVal) {
      if (fgView == "InstanceSpecifier")
      {
        ara::core::FixedString<ara::sm::kSmMaxInstanceId> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        inconfig.instanceSpecifier = std::move(localFixedString);
      }
      if (fgView == "FunctionGroupName")
      {
        ara::core::FixedString<ara::sm::kSmMaxFgName> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        inconfig.functionGroupName = std::move(localFixedString);
      }
    });
    triggerIn_.push_back(inconfig);
  });
}

void SmConfig::LoadTriggerOut(ara::core::configreader::BufferContext& context) noexcept
{
  ParseArray(context, [&](value val) {
    static_cast<void>(val);
    ara::sm::config::TriggerOut outconfig;
    ParseObject(context, [&](ara::core::StringView fgView, value fgVal) {
      if (fgView == "InstanceSpecifier")
      {
        ara::core::FixedString<ara::sm::kSmMaxInstanceId> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        outconfig.instanceSpecifier = std::move(localFixedString);
      }
      if (fgView == "FunctionGroupName")
      {
        ara::core::FixedString<ara::sm::kSmMaxFgName> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        outconfig.functionGroupName = std::move(localFixedString);
      }
    });
    triggerOut_.push_back(outconfig);
  });
}

void SmConfig::LoadTriggerInOut(ara::core::configreader::BufferContext& context) noexcept
{
  ParseArray(context, [&](value val) {
    static_cast<void>(val);
    ara::sm::config::TriggerInOut inoutconfig;
    ParseObject(context, [&](ara::core::StringView fgView, value fgVal) {
      if (fgView == "InstanceSpecifier")
      {
        ara::core::FixedString<ara::sm::kSmMaxInstanceId> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        inoutconfig.instanceSpecifier = std::move(localFixedString);
      }
      if (fgView == "FunctionGroupName")
      {
        ara::core::FixedString<ara::sm::kSmMaxFgName> localFixedString(
            ara::core::TruncateToCapacity, fgVal.As<ara::core::StringView>().data());
        inoutconfig.functionGroupName = std::move(localFixedString);
      }
    });
    triggerInOut_.push_back(inoutconfig);
  });
}

} // namespace config
} // namespace sm
} // namespace ara
