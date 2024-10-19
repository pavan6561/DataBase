// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sm_config_macros.h
//
// Purpose     : This file provides sm macro configuration class declaration,parsed from json file.
// -----------------------------------------------------------------------------------------------------------
#ifndef SM_SOURCE_UTILS_INC_PUBLIC_SM_CONFIG_MACROS_H_
#define SM_SOURCE_UTILS_INC_PUBLIC_SM_CONFIG_MACROS_H_
#include <cstdint>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief State Manager.
namespace sm
{
/// @brief Used to store the TriggerIn value.
constexpr uint64_t kSmMaxTriggerIn{10U};

/// @brief Used to store the TriggerOut value.
constexpr uint64_t kSmMaxTriggerOut{10U};

/// @brief Used to store the TriggerInOut value.
constexpr uint64_t kSmMaxTriggerInOut{10U};

/// @brief Used to store SMActivityIn class objects.
constexpr uint32_t kSmSMActivityIn{10U};

/// @brief Used to store NMActivityIn class objects.
constexpr uint32_t kSmNMActivity{10U};

/// @brief Used to store NMHandles.
constexpr uint32_t kSmMaxNMHandles{10U};

/// @brief Used to store PowerModeActivity class.
constexpr uint32_t kSmPowerModeActivity{10U};

/// @brief Used to store PowerModeHandles.
constexpr uint32_t kSmPowerModeHandles{10U};

/// @brief Used to store SMActivityInout class.
constexpr uint32_t kSmSMActivityInout{10U};

/// @brief Used to store SMActivityInout class.
constexpr uint32_t kSmSMActivityOut{10U};

/// @brief Used to store FunctionGroupList.
constexpr uint32_t kSmFunctionGroupList{10U};

/// @brief Used to store SubscribersList.
constexpr uint32_t kSmSubscribersList{10U};

/// string

/// @brief Used to store InstanceId.
constexpr uint32_t kSmMaxInstanceId{100U};

/// @brief Used to store FunctionGroupName.
constexpr uint32_t kSmMaxFgName{100U};

/// @brief Used to store FunctionGroupState.
constexpr uint32_t kSmMaxFgState{100U};

/// @brief Used to store FunctionGroup with state and name.
constexpr uint32_t kSmMaxFgGroup{100U};

/// @brief Used to store FunctionGroupState.
constexpr uint32_t kSmMaxFgStateList{100U};

/// @brief Used to store FunctionGroupState.
constexpr uint32_t kSmMaxMachineState{100U};

/// @brief Used to store FunctionGroupState.
constexpr uint32_t kSmMaxMachineStateList{10U};

/// @brief Used to store Diag Security data.
constexpr uint32_t kSmDiagSecurity{100U};

/// @brief Used to store Diag session info.
constexpr uint32_t kSmDiagSession{100U};

/// @brief Used to store Diag address.
constexpr uint32_t kSmDiagAddress{100U};

/// @brief Used to store Network Id.
constexpr uint32_t kSmNetworkId{100U};

/// @brief Used to store local buffer data.
constexpr uint32_t kSmLocalBuffer{40U};

/// @brief Used to store PowerMode Request State.
constexpr uint32_t kSmPowerModeRequestState{100U};

/// @brief Used to store NMHandles.
constexpr uint32_t kSmNMHandleName{100U};

/// @brief Used to store NMHandle List.
constexpr uint32_t kSmNMHandleList{10U};

/// @brief Used to store PowerModeHandles.
constexpr uint32_t kSmPowerModeHandleName{100U};

/// @brief Used to store reset signature.
constexpr uint32_t kSmResetSignature{100U};

/// json

/// @brief Used to store PowerOnFunctionGrpList.
constexpr uint32_t kSmJsonPowerOnFunctionGrpList{10U};

/// @brief Used to store DiagnosticAddressList.
constexpr uint32_t kSmJsonDiagnosticAddressList{10U};

/// @brief Used to store NetworkHandlerList.
constexpr uint32_t kSmJsonNetworkHandlerList{10U};

/// @brief Used to store PowerModesList.
constexpr uint32_t kSmJsonPowerModesList{10U};

} // namespace sm
} // namespace ara
#endif //SM_SOURCE_UTILS_INC_PUBLIC_SM_CONFIG_MACROS_H_

// -----------------------------------------------------------------------------------------------------------
// AVIN Guideline Exception(s)
// -----------------------------------------------------------------------------------------------------------
// - M11.0.1
//   Guideline:           AUTOSAR CPP14 Guidelines 19.11, M11.0.1 required
//   Description:         Member data in non-POD class types shall be private.
//   Justification:
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
