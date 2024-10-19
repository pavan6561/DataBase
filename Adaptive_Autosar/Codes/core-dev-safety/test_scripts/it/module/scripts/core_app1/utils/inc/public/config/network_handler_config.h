// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : network_handler_config.h
//
// Purpose     : Header file to store data, parsed from json file.
// -----------------------------------------------------------------------------------------------------------

#ifndef SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_NETWORK_HANDLER_CONFIG_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_NETWORK_HANDLER_CONFIG_H_

#include <ara/core/string.h>
#include "sm_config_macros.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief State Manager.
namespace sm
{
/// @brief config.
namespace config
{
/// @brief Class used to store the NetworkHandler in sm configuration.
/// @archtrace 510875
/// @design 529455
class NetworkHandler final
{
 public:
  /// @brief Default constructor of the NetworkHandler class.
  /// @archtrace 510875
  /// @design 529456
  NetworkHandler() noexcept
  {
  }

  /// @brief Destructor of the NetworkHandler class.
  /// @archtrace 510875
  /// @design 529457
  ~NetworkHandler()
  {
  }

  /// @brief NetworkHandler copy constructor.
  NetworkHandler(NetworkHandler const&) = default;

  /// @brief NetworkHandler move constructor.
  NetworkHandler(NetworkHandler&&) noexcept = default;

  /// @brief NetworkHandler copy assignment.
  NetworkHandler& operator=(NetworkHandler const&) = delete;

  /// @brief NetworkHandler move assignment.
  NetworkHandler& operator=(NetworkHandler&&) noexcept = default;

  /// @brief Used to store the NM handle name.
  ara::core::FixedString<ara::sm::kSmNMHandleName> handleName;

  /// @brief Used to store the NM handle instance id.
  ara::core::FixedString<ara::sm::kSmMaxInstanceId> instanceId;

  /// @brief NM handle mapping with machine state.
  ara::core::FixedVector<ara::core::FixedString<ara::sm::kSmMaxMachineState>, ara::sm::kSmMaxMachineStateList>
      machineStates;

  /// @brief Used to store the NM handle mapping with functional group state.
  ara::core::FixedVector<ara::core::FixedString<ara::sm::kSmMaxFgState>, ara::sm::kSmMaxFgStateList>
      functionGroupStates;

  /// @brief Used to store the NM handle after run configuration.
  uint32_t afterRunTimeOut{0U};
};

} // namespace config
} // namespace sm
} // namespace ara

#endif // SM_SOURCE_UTILS_INC_PUBLIC_CONFIG_NETWORK_HANDLER_CONFIG_H_
