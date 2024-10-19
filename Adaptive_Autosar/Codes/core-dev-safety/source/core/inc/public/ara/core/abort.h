// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : abort.h
//
// Purpose     : This file provides the implementation of class Abort.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_INC_PRIVATE_ARA_CORE_ABORT_H_
#define CORE_SOURCE_CORE_INC_PRIVATE_ARA_CORE_ABORT_H_

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief A function declaration with the correct prototype for SetAbortHandler().
/// @trace SWS_CORE_12404 SWS_CORE_12403 SWS_CORE_12402 SWS_CORE_12407 SWS_CORE_00003 SWS_CORE_00004
/// @archtrace 608891
/// @design 611811 614039
void AbortHandlerPrototype() noexcept;

/// @brief The type of a handler for SetAbortHandler().
/// @trace SWS_CORE_12406
using AbortHandler = decltype(&AbortHandlerPrototype);

/// @brief Set a custom global Abort handler function and return the previously installed one.
/// @param[in] handler A custom Abort handler (or nullptr).
/// @returns the previously installed Abort handler (or nullptr if none was installed).
/// @trace SWS_CORE_12406
/// @design 614039
AbortHandler SetAbortHandler(AbortHandler handler) noexcept;

/// @brief Abort the current operation.
/// @param[in] text A custom text to include in the log message being output.
/// @trace SWS_CORE_12405
/// @design 611811
[[noreturn]] void Abort(char const* text) noexcept;
// AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_INC_PRIVATE_ARA_CORE_ABORT_H_
