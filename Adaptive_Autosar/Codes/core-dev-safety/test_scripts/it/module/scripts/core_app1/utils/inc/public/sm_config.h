// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sm_config.h
//
// Purpose     : Header file of class SmConfig.
// -----------------------------------------------------------------------------------------------------------

#ifndef SM_SOURCE_UTILS_INC_PUBLIC_SM_CONFIG_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_SM_CONFIG_H_

#include <cstdint>

#include <ara/log/logging.h>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include "config/poweron_config.h"
#include "config/diagnostic_address_config.h"
#include "config/network_handler_config.h"
#include "config/powermode_config.h"
#include "config/triggerin_config.h"
#include "config/triggerout_config.h"
#include "config/triggerinout_config.h"
#include "ara/core/configreader/config_reader.h"
#include <fstream>
#include <ara/core/fixed_vector.h>
#include <ara/core/fixed_string.h>
#include "sm_config_macros.h"
/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief State Manager.
namespace sm
{
/// @brief Config.
namespace config
{
/// @brief Class SmConfig used to store the sm configuration.
/// @archtrace 511941
/// @design 529464
class SmConfig final
{
 public:
  /// @brief Constructor used to construct the object.
  /// @archtrace 511941
  /// @design 529465
  SmConfig() noexcept;

  /// @brief Destructor used to destruct the object.
  ~SmConfig() = default;

  /// @brief Copy constructor of SmConfigData.
  SmConfig(SmConfig const&) = default;

  /// @brief Move constructor of SmConfigData.
  SmConfig(SmConfig&&) noexcept = default;

  /// @brief Copy assignment operator of .
  SmConfig& operator=(SmConfig const&) = default;

  /// @brief Move assignment operator of .
  SmConfig& operator=(SmConfig&&) = delete;

  /// @brief To Function to load the json file and to collect data.
  /// @param[in] fileName File name of the sm configuration.
  /// @archtrace 511941
  /// @design 529469
  void LoadConfig(std::string fileName) noexcept;

  /// @brief Used to store the updatesession timeout value from the json.
  uint32_t updateSessionTimeout_;

  /// @brief Used to store the ResetRequest timeout value from the json.
  uint32_t resetRequestTimeout_;

  /// @brief Used to store the PowerOnFunctionGrps from the json.
  ara::core::FixedVector<ara::sm::config::PowerOnFunctionGrp, ara::sm::kSmJsonPowerOnFunctionGrpList>
      powerOnFunctionGrp_;

  /// @brief Used to store the DiagnosticAddress from the json.
  ara::core::FixedVector<ara::sm::config::DiagnosticAddress, ara::sm::kSmJsonDiagnosticAddressList>
      diagnosticAddress_;

  /// @brief Used to store the NetworkHandler from the json.
  ara::core::FixedVector<ara::sm::config::NetworkHandler, ara::sm::kSmJsonNetworkHandlerList> networkHandler_;

  /// @brief Used to store the PowerMode from the json.
  ara::core::FixedVector<ara::sm::config::PowerModes, ara::sm::kSmJsonPowerModesList> powerModes_;

  /// @brief Used to store the TriggerIn from the json.
  ara::core::FixedVector<ara::sm::config::TriggerIn, ara::sm::kSmMaxTriggerIn> triggerIn_;

  /// @brief Used to store the TriggerOut from the json.
  ara::core::FixedVector<ara::sm::config::TriggerOut, ara::sm::kSmMaxTriggerOut> triggerOut_;

  /// @brief Used to store the TriggerOut from the json.
  ara::core::FixedVector<ara::sm::config::TriggerInOut, ara::sm::kSmMaxTriggerInOut> triggerInOut_;

 private:
  /// @brief filestream.
  std::ifstream fStream_;

  /// @brief To collect the UpdatesessionTimeout.
  /// @param[in] context Context contains the message to be parsed.
  /// @archtrace 511941
  /// @design 529469
  void LoadPowerOnFunctionGrpConfig(ara::core::configreader::BufferContext& context) noexcept;

  /// @brief To collect the DiagnosticAddress.
  /// @param[in] context Context contains the message to be parsed.
  /// @archtrace 511941
  /// @design 529469
  void LoadDiagnosticAddressConfig(ara::core::configreader::BufferContext& context) noexcept;

  /// @brief To collect the NetworkHandlerData.
  /// @param[in] context Context contains the message to be parsed.
  /// @archtrace 511941
  /// @design 529469
  void LoadNetworkHandlerConfig(ara::core::configreader::BufferContext& context) noexcept;

  /// @brief To collect the PowerModedata.
  /// @param[in] context Context contains the message to be parsed.
  /// @archtrace 511941
  /// @design 529469
  void LoadPowerMode(ara::core::configreader::BufferContext& context) noexcept;

  /// @brief To collect the TriggerIndata.
  /// @param[in] context Context contains the message to be parsed.
  /// @archtrace 511941
  /// @design 529466
  void LoadTriggerIn(ara::core::configreader::BufferContext& context) noexcept;

  /// @brief To collect the TriggerOutdata.
  /// @param[in] context Context contains the message to be parsed.
  /// @archtrace 511941
  /// @design 529467
  void LoadTriggerOut(ara::core::configreader::BufferContext& context) noexcept;

  /// @brief To collect the TriggerInOutdata.
  /// @param[in] context Context contains the message to be parsed.
  /// @archtrace 511941
  /// @design 529468
  void LoadTriggerInOut(ara::core::configreader::BufferContext& context) noexcept;

  /// @brief Used to print the logs.
  ara::log::Logger& logger_;

  /// @brief Print the logs.
  /// @archtrace 511941
  /// @design 529469
  void ConsoleInfo() noexcept;
};

} // namespace config
} // namespace sm
} // namespace ara

#endif //SM_SOURCE_UTILS_INC_PUBLIC_SM_CONFIG_H_
