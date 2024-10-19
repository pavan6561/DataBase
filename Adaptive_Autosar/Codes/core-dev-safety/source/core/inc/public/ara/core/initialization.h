// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : initialization.h
//
// Purpose     : This file provides the implementation of class Initialization.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_INC_PRIVATE_ARA_CORE_INITIALIZATION_H_
#define CORE_SOURCE_CORE_INC_PRIVATE_ARA_CORE_INITIALIZATION_H_

#include "ara/core/result.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Initializes data structures and threads of the AUTOSAR Adaptive Runtime
/// for Applications.
/// @returns A Result object that indicates whether the AUTOSAR Adaptive Runtime
/// for Applications was successfully initialized.
/// Note that this is the only way for the ARA to report an error that is guaranteed
/// to be available, e.g., in case ara::log failed to correctly initialize.
/// The user is not expected to be able to recover from such an error.
/// However, the user may have a project-specific way of recording
/// errors during initialization without ara::log.
/// @trace SWS_CORE_12402
/// @archtrace 608891
/// @design 614039
Result<void> Initialize();

/// @brief Destroy all data structures and threads of the AUTOSAR Adaptive Runtime
/// for Applications.
/// @returns A Result object that indicates whether the ARA was successfully destroyed.
/// Typical error cases to be reported here are that the user is still holding some resource
/// inside the ARA. Note that this Result is the only way for the ARA to report an error that
/// is guaranteed to be available, e.g., in case ara::log has already been deinitialized.
/// The user is not expected to be able to recover from such an error.
/// However, the user may have a project-specific way of recording
/// errors during deinitialization without ara::log.
/// @trace SWS_CORE_12402
/// @archtrace 608891
/// @design 614039
Result<void> Deinitialize();

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_INC_PRIVATE_ARA_CORE_INITIALIZATION_H_
