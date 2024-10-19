// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : initialization_impl.h
//
// Purpose     : This file provides the implementation of class InitializationImpl.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_INC_PRIVATE_ARA_CORE_INITIALIZATION_IMPL_H_
#define CORE_SOURCE_CORE_INC_PRIVATE_ARA_CORE_INITIALIZATION_IMPL_H_

#include "ara/core/string_view.h"
#include "ara/core/result.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Internal.
namespace internal
{
/// @brief Initialize method used to initialize the app.
/// @param[in] path Path to exec manifest to read.
/// @returns empty ara::core::Result in case of success or error.
/// @trace SWS_CORE_10001
/// @archtrace 608891
Result<void> Initialize(ara::core::StringView path);

} // namespace internal
} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_INC_PRIVATE_ARA_CORE_INITIALIZATION_IMPL_H_
