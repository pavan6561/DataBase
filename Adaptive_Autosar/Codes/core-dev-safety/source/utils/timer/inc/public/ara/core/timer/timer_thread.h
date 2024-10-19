// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : timer_thread.h
//
// Purpose     : This file provides portable one-shot or periodic timers declaration.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_UTILS_TIMER_INC_PUBLIC_ARA_CORE_TIMER_TIMER_THREAD_H_
#define CORE_SOURCE_UTILS_TIMER_INC_PUBLIC_ARA_CORE_TIMER_TIMER_THREAD_H_

#include <algorithm>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <set>
#include <cstdint>
#include <thread>
#include <mutex>
#include <atomic>

#include "condition_notifier.h"
#include "condition_listener.h"
#include "condition_variable_data.h"
#include "iceoryx_hoofs/internal/units/duration.hpp"
#include <ara/log/logging.h>
#include "ara/core/core_error_domain.h"

using namespace iox::units::duration_literals;
using namespace std::chrono_literals;
constexpr iox::units::Duration DISCOVERY_INTERVAL{100_ms};

/// @brief ara namespace
namespace ara
{
namespace core
{
/// @brief An enumeration with errors that can occur within this Functional Cluster.
/// @trace SWS_CORE_10901 SWS_CORE_10902 SWS_CORE_10903 SWS_CORE_10999 SWS_CORE_10900
enum class TimerErrc : ErrorDomain::CodeType
{
  kInvalidReqRequestTimer = 301, ///< invalid argument
  kInvalidHandler = 302,
  kInvalidId = 303,

};

/// @brief Implementation of the TimerErrorDomain class.
/// @design 611299
class TimerErrorDomain final : public ErrorDomain
{
  /// @brief To store the IdType.
  constexpr static ErrorDomain::IdType kId{0x8000000000000020};

 public:
  /// @brief Alias for the error code value enumeration.
  /// @trace SWS_CORE_10933
  using Errc = TimerErrc;

  /// @brief Alias for the exception base class.
  /// @trace SWS_CORE_10934
  using Exception = CoreException;

  /// @brief Its a default constructor.
  /// @trace SWS_CORE_00014
  /// @design 611299
  constexpr TimerErrorDomain() noexcept : ErrorDomain(kId)
  {
  }

  /// @brief Return the "shortname" ApApplicationErrorDomain.SN of this error domain.
  /// @returns Return the Name as "Core".
  /// @trace SWS_CORE_00014
  /// @design 611808
  char const* Name() const noexcept override
  {
    return "Timer";
  }

  /// @brief Translate an error code value into a text message.
  /// @param[in] errorCode The error code value.
  /// @returns The text message never nullptr.
  /// @trace SWS_CORE_00014
  /// @design 611809
  char const* Message(ErrorDomain::CodeType errorCode) const noexcept override
  {
    TimerErrc const code{static_cast<Errc>(errorCode)};
    switch (code)
    {
    case TimerErrc::kInvalidReqRequestTimer:
      return "Invalid Request Timer";
    case TimerErrc::kInvalidHandler:
      return "Invalid Handler";
    case TimerErrc::kInvalidId:
      return "Invalid Id";
    default:
      return "Unknown error";
    }
  }

  /// @brief Throw the exception type corresponding to the given ErrorCode.
  /// @param[in] errorCode Platform dependent error code to construct.
  /// @trace SWS_CORE_00014
  /// @design 611302
  void ThrowAsException(ErrorCode const& errorCode) const noexcept(false) override
  {
    ThrowOrTerminate<Exception>(errorCode);
  }
};
namespace internal
{
/// @brief Variable to store the TimerErrorDomain.
constexpr TimerErrorDomain g_timerErrorDomain;
} // namespace internal

/// @brief Return a reference to the global TimerErrorDomain.
/// @returns To return the TimerErrorDomain.
/// @trace SWS_CORE_00014 SWS_CORE_10982 SWS_CORE_10981 SWS_CORE_10980
/// @design 611822
inline constexpr ErrorDomain const& GetTimerErrorDomain() noexcept
{
  return internal::g_timerErrorDomain;
}
/// @brief Return a reference to the global ErrorCode.
/// @design 611299
inline constexpr ErrorCode MakeErrorCode(TimerErrc code, ErrorDomain::SupportDataType data) noexcept
{
  return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetTimerErrorDomain(), data);
}
/// @brief Utils for timer thread.
namespace timer
{
/// @brief Alias to eight byte of data.
using TimerId = std::size_t;

/// @brief The handler type for Timer class.
using HandlerType = std::function<void()>;

/// @brief Function object that we boil down to HandlerType with std::bind.
/// @tparam Args The types of arguments given to this function.
/// @param[in] Args The arguments used for constructing the value.
template <typename... Args>
using BoundHandlerType = std::function<void(Args...)>;

/// @brief Values that are a large-range millisecond count.
using MilliSeconds = std::uint64_t;

constexpr iox::units::Duration DURATION_INTERVAL_NANOSEC{1_ns};

/// @brief Implementation of the TimerThread class.
/// @trace SWS_CORE_0005
/// @archtrace 608894 609205 608952
/// @design 611218 614034
class TimerThread final
{
 public:
  /// @brief Constructor does not start thread_ until there is a start of timer.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952
  /// @design 611219
  explicit TimerThread() noexcept;

  /// @brief Destructor is thread safe, even if a timer callback is running.
  /// All callbacks are guaranteed to have returned before this destructor returns.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952
  /// @design 611220
  ~TimerThread() noexcept;

  /// @brief To create timer using milliseconds.
  /// @param[in] msDelay The timer delay in milliseconds.
  /// @param[in] msPeriod The delay for periodic timer in milliseconds.
  /// @param[in] handler Handler that is to be called on timer expiry.
  /// @returns TimerId.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952
  /// @design 611221
  TimerId StartTimer(MilliSeconds msDelay, MilliSeconds msPeriod, HandlerType handler);

  /// @brief Create timer using std::chrono delay and period.
  /// Optionally binds additional arguments to the callback.
  /// @tparam SRep An arithmetic type, or a class emulating an arithmetic type, to be used as the type
  /// for the internal count.
  /// @tparam SPer A ratio type that represents the period in seconds.
  /// @tparam PRep An arithmetic type, or a class emulating an arithmetic type, to be used as the type
  /// for the internal count.
  /// @tparam PPer A ratio type that represents the period in seconds.
  /// @tparam Args The types of arguments given to this function.
  /// @param[in] delay A time is delayed for periodic time.
  /// @param[in] period A ratio type that represents the period in seconds.
  /// @param[in] handler Function that is to be called after timer expiry.
  /// @param[in] args The arguments used for constructing the value.
  /// The arguments used for constructing the value.
  /// @returns TimerId.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952
  /// @design 611222
  template <typename SRep, typename SPer, typename PRep, typename PPer, typename... Args>
  TimerId StartTimer(typename std::chrono::duration<SRep, SPer> const& delay,
      typename std::chrono::duration<PRep, PPer> const& period, BoundHandlerType<Args...> handler,
      Args&&... args) noexcept(true);

  /// @brief StartSingleShotTimerWithResultManager API, Optionally binds additional arguments to the callback
  /// @param[in] rtimeout Time out for single shot timer.
  /// @param[in] rhandler Function that is to be called after timer expiry.
  /// @returns TimerId.
  /// @trace SWS_CORE_0005
  /// @archtrace 608885 608894 608952 609215
  /// @design 611223
  ara::core::Result<TimerId> StartSingleShotTimerWithResult(MilliSeconds rtimeout, HandlerType rhandler);

  /// @brief StartSingleShotTimerManager API, Optionally binds additional arguments to the callback
  /// @param[in] timeout Time out for single shot timer.
  /// @param[in] handler Function that is to be called after timer expiry.
  /// @returns TimerId.
  /// @trace SWS_CORE_0005
  /// @archtrace 608885 608894 608952 609215
  /// @design 611223
  TimerId StartSingleShotTimer(MilliSeconds timeout, HandlerType handler);

  /// @brief StartPeriodicTimerManager API. Call handler every period milliseconds,
  /// starting period milliseconds from now Optionally binds additional arguments to the callback.
  /// @param[in] period A ratio type that represents the period in seconds.
  /// @param[in] handler Function that is to be called after timer expiry.
  /// @returns TimerId.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952 609210 609212
  /// @design 611224
  TimerId StartPeriodicTimer(MilliSeconds period, HandlerType handler);

  /// @brief StartPeriodicTimerManager API. Call handler every period milliseconds,
  /// starting period milliseconds from now Optionally binds additional arguments to the callback.
  /// @param[in] rperiod A ratio type that represents the period in seconds.
  /// @param[in] rhandler Function that is to be called after timer expiry.
  /// @returns TimerId.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952 609210 609212
  /// @design 611225
  ara::core::Result<TimerId> StartPeriodicTimerWithResult(MilliSeconds rperiod, HandlerType rhandler);

  /// @brief StartPeriodicTimerManager API Call handler every
  /// period milliseconds, starting period milliseconds from now.
  /// @tparam Args The types of arguments given to this function.
  /// @param[in] period A ratio type that represents the period in seconds.
  /// @param[in] handler Function that is to be called after timer expiry.
  /// @param[in] args The arguments used for constructing the value.
  /// @returns TimerId.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952 609210 609212
  /// @design 611225
  template <typename... Args>
  TimerId StartPeriodicTimer(MilliSeconds period, BoundHandlerType<Args...> handler, Args&&... args) noexcept(
      true);

  /// @brief Get the remaining time with result.
  /// @param[in] rid It specify the timer id.
  /// @returns It return the time in milliseconds.
  /// @trace SWS_CORE_0005
  /// @design 611226
  ara::core::Result<MilliSeconds> GetRemainingTimeWithResult(TimerId rid) noexcept;

  /// @brief Get the remaining time.
  /// @param[in] id It specify the timer id.
  /// @returns It return the time in milliseconds.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952 609210 609213 
  /// @design 611226
  MilliSeconds GetRemainingTime(TimerId id) noexcept;

  /// @brief Destroy the specified timer Synchronizes with the thread_ thread if the callback for this timer
  /// is running, which guarantees that the handler for that callback is not running before StopTimerManager
  /// returns. You are not required to StopAllTimerManagers any timers. You can forget their TimerId if you
  /// do not need to cancel them. The only time you need this is when you want to stop a timer that has a
  /// repetition period, or you want to cancel a timeout that has not fired yet See StopAllTimerManagers() to
  /// wipe out all timers in one go.
  /// @param[in] id It specify the timer id.
  /// @returns true if this Timer is running, false otherwise.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952 609210 609214
  /// @design 611227
  bool StopTimer(TimerId id);

  /// @brief Destroy the specified timer Synchronizes with the thread_ thread if the callback for this timer
  /// is running, which guarantees that the handler for that callback is not running before StopTimerManager
  /// returns. You are not required to StopAllTimerManagers any timers. You can forget their TimerId if you
  /// do not need to cancel them. The only time you need this is when you want to stop a timer that has a
  /// repetition period, or you want to cancel a timeout that has not fired yet See StopAllTimerManagers() to
  /// wipe out all timers in one go.
  /// @param[in] rid It specify the timer id.
  /// @returns true if this Timer is running, false otherwise.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952 609210 609214
  /// @design 611227
  ara::core::Result<bool> StopTimerWithResult(TimerId rid);

  /// @brief Get timer thread instance.
  /// @returns Returns lazily initialized singleton.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952 609210 609211
  /// @design 611229
  static TimerThread& GetTimerThreadInstance() noexcept;

  /// @brief Get head queue timer id.
  /// @returns TimerId.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952
  /// @design 611228
  TimerId GetHeadQueueTimerId() const;

 private:
  /// @brief The Lock for Timer class.
  using Lock = std::mutex;

  /// @brief The ScopedLock for Timer class.
  using ScopedLock = std::unique_lock<Lock>;

  /// @brief The ChronoClock for Timer class.
  using ChronoClock = std::chrono::steady_clock;

  /// @brief The timestamp for Timer class.
  using ChronoTimestamp = std::chrono::time_point<ChronoClock>;

  /// @brief The alias to the milliseconds.
  using MillisecondDuration = std::chrono::milliseconds;

  /// @brief The logging context instance.
  ara::log::Logger& aralog_{ara::log::CreateLogger("TTC", "TimerThread Context")};
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.

  /// @brief Implementation of the TimerManager structure.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 609205 608952
  /// @design 611209
  class TimerManager final
  {
   public:
    /// @brief Implementation of the TimerManager move constructor.
    /// @param[in] r Object of timer manager.
    /// @trace SWS_CORE_0005
	  /// @archtrace 608894 608952
    /// @design 611210
    TimerManager(TimerManager&& r) noexcept :
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A12.1.5:A12.1.5-1 Use delegating constructor for common
        // initialization.
        timerIdentity_(std::move(r.timerIdentity_)), nextExpiryPoint_(std::move(r.nextExpiryPoint_)),
        period_(std::move(r.period_)), handler_(std::move(r.handler_)), running_(std::move(r.running_))
    {
    }

    /// @brief Implementation of the TimerManager constructor.
    /// @param[in] id The id of timer thread.
    /// @param[in] next The expiry of the timer.
    /// @param[in] durationValue This is a traits class to provide the limits and zero value of the type
    /// used to represent the count in a duration object.
    /// @param[in] handlerValue The handler Function that is to be called after timer expiry.
    /// @trace SWS_CORE_0005
	  /// @archtrace 608894 608952
    /// @design 611211
    TimerManager(TimerId id, ara::mepoo::DurationNsT next, uint64_t durationValue,
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A12.1.2:A12.1.2-1 Both NSDMI and a non-static member
        // initializer in a constructor shall not be used in the same type.
        HandlerType handlerValue) noexcept :
        timerIdentity_(id),
        nextExpiryPoint_(next), period_(durationValue), handler_(std::move(handlerValue)), running_(false)
    {
    }

    /// @brief Assignment operator of the TimerManager.
    /// @param[in] r Object of timer manager.
    TimerManager& operator=(TimerManager&& r) & = default;
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A15.5.1:A15.5.1-1 Function shall be explicitly declared
    // noexcept if appropriate.

    /// @brief The default copy constructor deleted.
    /// @param[in] r Object of timer manager.
    TimerManager(TimerManager const& r) = delete;

    /// @brief Assignment operator of TimerManager deleted.
    /// @param[in] r Object of timer manager.
    TimerManager& operator=(TimerManager const& r) = delete;

    /// @brief The destructor for TimerManager class.
    ~TimerManager() = default;

    /// @brief The expiry of the timer.
    /// @returns Alias to nextExpiryPoint_.
    /// @trace SWS_CORE_0005
	  /// @archtrace 608894 608952
    /// @design 611212
    ara::mepoo::DurationNsT GetNextExpiryPoint()
    {
      return nextExpiryPoint_;
    }

    /// @brief The expiry of the timer.
    /// @param[in] setValue Sets the data value.
    /// @trace SWS_CORE_0005
	  /// @archtrace 608894 608952
    /// @design 611213
    void SetNextExpiryPoint(ara::mepoo::DurationNsT setValue)
    {
      nextExpiryPoint_ = setValue;
    }

    /// @brief It specify timer is running.
    /// @returns Alias to the running_.
    /// @trace SWS_CORE_0005
    /// @archtrace 608894 608952
    /// @design 611214
    bool& GetRunning()
    {
      return running_;
    }

    /// @brief The handler Function that is to be called after timer expiry.
    /// @returns Alias to the handler_.
    /// @trace SWS_CORE_0005
	  /// @archtrace 608894 608952
    /// @design 611215
    HandlerType& GetHandler()
    {
      return handler_;
    }

    /// @brief After which timer is repeated.
    /// @returns Alias to the period_.
    /// @trace SWS_CORE_0005
	  /// @archtrace 608894 608952
    /// @design 611216
    uint64_t GetMillisecond()
    {
      return period_;
    }

    /// @brief The ID of timer thread.
    /// @returns Alias to the timerIdentity_.
    /// @trace SWS_CORE_0005
	  /// @archtrace 608894 608952
    /// @design 611217
    TimerId& GetTimerIdentity()
    {
      return timerIdentity_;
    }

   private:
    /// @brief The ID of timer thread.
    TimerId timerIdentity_;

    /// @brief The expiry of the timer.
    ara::mepoo::DurationNsT nextExpiryPoint_;

    /// @brief After which timer is repeated.
    uint64_t period_;

    /// @brief The handler Function that is to be called after timer expiry.
    HandlerType handler_;

    /// @brief It specify timer is running.
    bool running_;
  };

  /// @brief Comparison structure to sort the timer "queue_" by TimerManager::next.
  /// @trace SWS_CORE_0005
  /// @archtrace 608894 608952
  /// @design 611207
  class NextActiveComparator
  {
   public:
    /// @brief The operator '()' overloaded with comparison operator.
    /// @param[in] currentInstance The current instance of time manager.
    /// @param[in] newInstance New instance of time manager.
    /// @returns true, If new instance created after current instance.
    /// @trace SWS_CORE_0005
	  /// @archtrace 608894 608952
    /// @design 611208
    bool operator()(TimerManager& currentInstance, TimerManager& newInstance) noexcept
    {
      return currentInstance.GetNextExpiryPoint() < newInstance.GetNextExpiryPoint();
    }
  };

  /// @brief Queue is a set of references to TimerManager objects, sorted by next.
  using QueueValue = std::reference_wrapper<TimerManager>;

  /// @brief Queue is a set of references to Queue value and next active comparator objects.
  using Queue = std::multiset<QueueValue, NextActiveComparator>;

  /// @brief Mapping of timer manager with timer id.
  using TimerManagerMap = std::unordered_map<TimerId, TimerManager>;

  /// @brief Worker method for timer thread.
  /// @design 611218
  void TimerThreadWorker();

  /// @brief To destroy the timer instance.
  /// @param[in] lockCurrentInstance The locking mechanism for current instance.
  /// @param[in] timerItr The index of current instance.
  /// @return true, If timer instance destroyed successfully.
  /// @design 611218
  bool DestroyTimerInstance(ScopedLock& lockCurrentInstance, TimerManagerMap::iterator timerItr);

  /// @brief Inexhaustible source of unique IDs.
  TimerId nextId_{1U};

  /// @brief The TimerManager objects are physically stored in this map.
  TimerManagerMap activeTimerMap_{};

  /// @brief The ordering queue_ holds references to items in activeTimerMap_ .
  Queue queue_{};

  /// @brief One thread_ thread for an unlimited number of timers is acceptable.
  /// Lazily started when first timer is started.
  mutable Lock syncloc_{};

  /// @brief The locking mechanism for dependent thread.
  /// @brief Conditional Variable alternative
  ara::core::ConditionVariableData condVarData_;
  /// @brief Conditional listener alternative
  ara::core::ConditionListener condWaiter_;
  /// @brief Conditional notifier alternative
  ara::core::ConditionNotifier condSignaler_;

  /// @brief The instance of the thread.
  std::thread thread_{};
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A15.5.3:A15.5.3-1 This thread is possibly join-able on
  // destructor call.

  /// @brief The flag to notify exit is requested.
  std::atomic_bool isExitRequested_{false};
};

} // namespace timer
} // namespace core
} // namespace ara

#endif // CORE_SOURCE_UTILS_TIMER_INC_PUBLIC_ARA_CORE_TIMER_TIMER_THREAD_H_

// -----------------------------------------------------------------------------------------------------------
// AVIN Guideline Exception(s)
// -----------------------------------------------------------------------------------------------------------
// - A12.1.5-1
//   Guideline:           AUTOSAR CPP14 Guidelines, A12.1.5 required
//   Description:         Use delegating constructor for common initialization.
//   Justification:       Without using delegating constructor there is no code repetition, accidental
//                        differences and maintenance problems.
//   Risk:                None
//   Verification Method: Code Review
//
// - A12.1.2-1
//   Guideline:           AUTOSAR CPP14 Guidelines, A12.1.2 required
//   Description:         Both NSDMI and a non-static member initializer in a constructor shall not be used
//                        in the same type.
//   Justification:       This will create another violation for initialization with NSDMI. This has been
//                        analysed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
//
// - A15.5.1-1
//   Guideline:           AUTOSAR CPP14 Guidelines, A15.5.1 required
//   Description:         Function shall be explicitly declared noexcept if appropriate.
//   Justification:       The implementation of function is ensured that not exit with an exception.
//   Risk:                None
//   Verification Method: Code Review
//
// - A15.5.3-1
//   Guideline:           AUTOSAR CPP14 Guidelines, A15.5.3 required
//   Description:         This thread is possibly join-able on destructor call.
//   Justification:       The destructors of automatic thread will be called.
//   Risk:                None
//   Verification Method: Code Review
//
// - A5.1.1-2
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A5.1.1 required
//   Description:         Use of magic string literal.
//   Justification:       This will create another violation for initialization with magic string literal.This
//                        has been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
