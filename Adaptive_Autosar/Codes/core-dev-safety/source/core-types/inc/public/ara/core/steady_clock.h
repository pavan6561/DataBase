// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : steady_clock.h
//
// Purpose     : This file provides the implementation of class steady_clock.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_STEADY_CLOCK_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_STEADY_CLOCK_H_

#include <chrono>
#include <cstdint>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief This clock represents a monotonic clock.
/// The time points of this clock cannot decrease as physical time moves
/// forward and the time between ticks of this clock is constant.
/// @trace SWS_CORE_11800
/// @archtrace 609895
/// @design 611288 614039
class SteadyClock final
{
 public:
  /// @brief Arithmetic type representing the number of ticks in the clocks Duration.
  /// @trace SWS_CORE_11800
  /// @archtrace 609895
  using Rep = std::int64_t;

  /// @brief A std::ratio type representing the tick Period of the clock, in seconds.
  /// @trace SWS_CORE_11800
  /// @archtrace 609895
  using Period = std::nano;

  /// @brief To represents a time interval.
  /// @trace SWS_CORE_11800
  /// @archtrace 609895
  using Duration = std::chrono::duration<Rep, Period>;

  /// @brief It is implemented as if it stores a value of type Duration indicating
  /// the time interval from the start of the Clock's.
  /// @trace SWS_CORE_11800
  /// @archtrace 609895
  using TimePoint = std::chrono::time_point<SteadyClock, Duration>;

  /// @brief Steady clock flag, always true.
  /// @trace SWS_CORE_11800
  /// @archtrace 609895
  static constexpr bool stkisSteady{true};

  /// @brief Return a TimePoint representing the current value of the clock.
  /// @returns A TimePoint representing the current value of the clock.
  /// @trace SWS_CORE_11801
  /// @archtrace 609895
  /// @design 611289
  static TimePoint Now() noexcept
  {
    Duration const dur{std::chrono::steady_clock::now().time_since_epoch()};
    TimePoint tp{dur};
    return tp;
  }
};

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_STEADY_CLOCK_H_
