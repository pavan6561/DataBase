// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name : timer_thread.cpp
//
// Purpose : This file provides the implementation of class timer.
// -----------------------------------------------------------------------------------------------------------

#include <cassert>

#include "ara/core/timer/timer_thread.h"

namespace ara
{
namespace core
{
namespace timer
{
void TimerThread::TimerThreadWorker()
{
  ScopedLock threadLock{syncloc_};

  while (!isExitRequested_)
  {
    if (static_cast<bool>(queue_.empty()))
    {
      threadLock.unlock();

      condWaiter_.TimedWait(10_s);
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.

      threadLock.lock();

      continue;
    }

    TimerThread::Queue::iterator const queueHead{queue_.begin()};

    TimerManager& timer(*queueHead);

    std::chrono::duration<long int, std::ratio<1, 1000000000> > count{std::chrono::duration<int64_t, std::nano>(ara::mepoo::BaseClockT::now().time_since_epoch())};

    if (count >= timer.GetNextExpiryPoint())
    {
      static_cast<void>(queue_.erase(queueHead));

      // Mark it as running to handle racing destroy.
      timer.GetRunning() = true;

      // Call the handler outside the lock.
      threadLock.unlock();

      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.

      (timer.GetHandler())();

      threadLock.lock();

      if (timer.GetRunning())
      {
        timer.GetRunning() = false;
        // If it is periodic, schedule a new one.
        if (timer.GetMillisecond() > 0)
        {
          timer.SetNextExpiryPoint(ara::mepoo::DurationNsT(
              (timer.GetNextExpiryPoint().count() + (timer.GetMillisecond() * 1000000))));
          // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.

          static_cast<void>(queue_.emplace(timer));
        }
        else
        {
          // Not rescheduling, destruct it.
          static_cast<void>(activeTimerMap_.erase(timer.GetTimerIdentity()));
        }
      }
      else
      {
        // The StopAllTimersTimerManager call expects us to remove the instance
        // when it detects that it is racing with its callback.
        static_cast<void>(activeTimerMap_.erase(timer.GetTimerIdentity()));
      }
    }
    else
    {
      std::chrono::duration<long int, std::ratio<1, 1000000000> > remainingTime{timer.GetNextExpiryPoint() - ara::mepoo::BaseClockT::now().time_since_epoch()};
      if (remainingTime > (std::chrono::nanoseconds(0U)))
      {
        threadLock.unlock();

        condWaiter_.TimedWait(iox::units::Duration(remainingTime));

        threadLock.lock();
      }
    }
  }
}

TimerId TimerThread::GetHeadQueueTimerId() const
{
  TimerId id{0};

  if (!static_cast<bool>(queue_.empty()))
  {
    TimerThread::Queue::iterator const queueHead{queue_.begin()};

    TimerManager& timer(*queueHead);

    id = timer.GetTimerIdentity();
  }
  return id;
}

// AVIN_VERIFIED_NEXT AutosarC++19_03-A12.1.2:A12.1.2-1 Both NSDMI and a non-static member
// initializer in a constructor shall not be used in the same type.
TimerThread::TimerThread() noexcept :
    queue_(), condVarData_{"TimerThreadCondVar"}, condWaiter_{condVarData_}, condSignaler_{condVarData_, 0U}
{
}

TimerThread::~TimerThread()
{
  ScopedLock threadLock{syncloc_};

  // The thread_ might not be running.
  if (static_cast<bool>(thread_.joinable()))
  {
    isExitRequested_ = true;

    threadLock.unlock();

    condSignaler_.notify_all();

    // If a timer handler is running, this will make sure it has returned before allowing any
    // deallocations to happen.
    thread_.join();

    // Note that any timers still in the queue_ will be destructed properly but they will not be invoked.
  }
}

TimerId TimerThread::StartSingleShotTimer(MilliSeconds timeout, HandlerType handler)
{
  return StartTimer(timeout, 0, std::move(handler));
}

ara::core::Result<TimerId> TimerThread::StartSingleShotTimerWithResult(
    MilliSeconds rtimeout, HandlerType rhandler)
{
  if ((rtimeout > 10) && (rhandler != nullptr))
  {
    TimerId val = StartTimer(rtimeout, 0, std::move(rhandler));
    return ara::core::Result<TimerId>::FromValue(val);
  }
  else if (rtimeout < 10)
  {
    return ara::core::Result<TimerId>::FromError(ErrorCode(ara::core::TimerErrc::kInvalidReqRequestTimer));
  }
  else
  {
    return ara::core::Result<TimerId>::FromError(ErrorCode(ara::core::TimerErrc::kInvalidHandler));
  }
}

ara::core::Result<TimerId> TimerThread::StartPeriodicTimerWithResult(
    MilliSeconds rperiod, HandlerType rhandler)
{
  if ((rperiod > 10) && (rhandler != nullptr))
  {
    TimerId val = StartTimer(rperiod, 0, std::move(rhandler));
    return ara::core::Result<TimerId>::FromValue(val);
  }
  else if (rperiod < 10)
  {
    return ara::core::Result<TimerId>::FromError(ErrorCode(ara::core::TimerErrc::kInvalidReqRequestTimer));
  }
  else
  {
    return ara::core::Result<TimerId>::FromError(ErrorCode(ara::core::TimerErrc::kInvalidHandler));
  }
}

TimerId TimerThread::StartPeriodicTimer(MilliSeconds period, HandlerType handler)
{
  return StartTimer(period, period, std::move(handler));
}

// AVIN_VERIFIED_NEXT AutosarC++19_03-A13.3.1:A13.3.1-1 Functions that contain
// forwarding reference parameters shall not be overloaded
TimerId TimerThread::StartTimer(uint64_t msDelay, uint64_t msPeriod, HandlerType handler)
{
  ScopedLock threadLock{syncloc_};

  // Lazily start thread when first timer is requested.
  if (!(static_cast<bool>(thread_.joinable())))
  {
    thread_ = std::thread(&TimerThread::TimerThreadWorker, this);

    static_cast<void>(pthread_setname_np(thread_.native_handle(), "TimerThread"));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }

  // Assign an ID and insert it into function storage.
  // AVIN_VERIFIED_NEXT AutosarC++19_03-A4.7.1:A4.7.1-1 Cast on result of arithmetic may cause overflow.
  std::size_t id{nextId_++};

  std::chrono::duration<long int, std::ratio<1, 1000000000> > currentTimestamp{
      ara::mepoo::BaseClockT::now().time_since_epoch() + ara::mepoo::DurationNsT(msDelay * 1000000)};
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.

  const std::pair<std::__detail::_Node_iterator<std::pair<const long unsigned int, ara::core::timer::TimerThread::TimerManager>, false, false>, bool>
   iter{
      activeTimerMap_.emplace(id, TimerManager(id, currentTimestamp, (msPeriod), std::move(handler)))};
  
  // Insert a reference to the TimerManager into ordering queue_.
  Queue::iterator const place{queue_.emplace(iter.first->second)};

  // We need to notify the timer thread only if we inserted this timer into the front of the timer queue_.
  const bool needNotify{(place == queue_.begin())};

  if (needNotify)
  {
    condSignaler_.notify_all();

    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
  else
  {
    // Do nothing.
  }
  return id;
}

bool TimerThread::StopTimer(TimerId id)
{
  ScopedLock threadLock{syncloc_};

  TimerThread::TimerManagerMap::iterator const itr{activeTimerMap_.find(id)};

  return DestroyTimerInstance(threadLock, itr);
}

ara::core::Result<bool> TimerThread::StopTimerWithResult(TimerId rid)
{
  ScopedLock threadLock{syncloc_};

  TimerThread::TimerManagerMap::iterator const itr{activeTimerMap_.find(rid)};
  if (itr == activeTimerMap_.end())
  {
    return ara::core::Result<bool>::FromError(ErrorCode(ara::core::TimerErrc::kInvalidId));
  }
  else
  {
    return ara::core::Result<bool>::FromValue(DestroyTimerInstance(threadLock, itr));
  }
}

bool TimerThread::DestroyTimerInstance(ScopedLock& lockCurrentInstance, TimerManagerMap::iterator timerItr)
{
  // NOTE: if notify is true, returns with lock unlocked.
  // AVIN_VERIFIED_NEXT AutosarC++19_03-M0.1.1:M0.1.1-1 Unreachable code
  assert(lockCurrentInstance.owns_lock());

  if (timerItr == activeTimerMap_.end())
  {
    return false;
  }
  else
  {
    // Do nothing.
  }
  TimerManager& timer{timerItr->second};

  if (timer.GetRunning())
  {
    // A callback is in progress for this TimerManager, so flag it for deletion in the thread_.
    timer.GetRunning() = false;

    try
    {
      // Assign a condition variable to this timer.
      // TODO: Check usage of conditional variable here
    }
    catch (std::bad_alloc& ba)
    {
      aralog_.LogError() << "bad_alloc caught: " << ba.what();
    }
  }
  else
  {
    // Find the instance of timer and remove it form queue.
    for ( std::_Rb_tree_const_iterator<std::reference_wrapper<ara::core::timer::TimerThread::TimerManager> >
     itr{queue_.find(timer)}; itr != queue_.end(); itr++)
    
    {
      TimerManager& timer_(*itr);
      if (timer_.GetTimerIdentity() == timer.GetTimerIdentity())
      {
        queue_.erase(itr);
        break;
      }
    }

    static_cast<void>(activeTimerMap_.erase(timerItr));

    lockCurrentInstance.unlock();

    condSignaler_.notify_all();

    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }

  return true;
}

ara::core::Result<MilliSeconds> TimerThread::GetRemainingTimeWithResult(TimerId rid) noexcept
{
  ScopedLock const threadLock{syncloc_};

  TimerThread::TimerManagerMap::iterator const timerMap{activeTimerMap_.find(rid)};

  if (timerMap == activeTimerMap_.end())
  {
    return ara::core::Result<MilliSeconds>::FromError(ErrorCode(ara::core::TimerErrc::kInvalidId));
  }

  TimerManager& timer{timerMap->second};

  std::chrono::duration<long int, std::ratio<1, 1000000000> > remainingTime{timer.GetNextExpiryPoint() - ara::mepoo::BaseClockT::now().time_since_epoch()};
  return ara::core::Result<MilliSeconds>::FromValue(
      static_cast<ara::core::timer::MilliSeconds>(remainingTime.count() / 1000000));
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic literal.
}

MilliSeconds TimerThread::GetRemainingTime(TimerId id) noexcept
{
  ScopedLock const threadLock{syncloc_};

  TimerThread::TimerManagerMap::iterator const timerMap{activeTimerMap_.find(id)};

  if (timerMap == activeTimerMap_.end())
  {
    return 0;
  }

  TimerManager& timer{timerMap->second};

  std::chrono::duration<long int, std::ratio<1, 1000000000> > remainingTime{timer.GetNextExpiryPoint() - ara::mepoo::BaseClockT::now().time_since_epoch()};
  // AVIN_VERIFIED_NEXT AutosarC++19_03-A4.7.1:A4.7.1-2 Cast may truncate value
  return static_cast<ara::core::timer::MilliSeconds>(remainingTime.count() / 1000000);
}

TimerThread& TimerThread::GetTimerThreadInstance() noexcept
{
  static TimerThread singleton;

  return singleton;
}

} // namespace timer
} // namespace core
} // namespace ara

// -----------------------------------------------------------------------------------------------------------
// AVIN Guideline Exception(s)
// -----------------------------------------------------------------------------------------------------------
// - M6.6.3-1
//   Guideline:           AUTOSAR CPP14 Guidelines, M6.6.3 required
//   Description:         The continue statement shall only be used within a well-formed for loop.
//   Justification:       Using this continue statement there is no unnecessary complexity within the code.
//   Risk:                None.
//   Verification Method: Code Review.
//
// - A12.1.2-1
//   Guideline:           AUTOSAR CPP14 Guidelines, A12.1.2 required
//   Description:         Both NSDMI and a non-static member initializer in a constructor shall not be used
//                        in the same type.
//   Justification:       This will create another violation for initialization with NSDMI. This has been
//                        analysed and verified manually. There is no impact on code quality.
//   Risk:                None.
//   Verification Method: Code Review.
//
// - A13.3.1-1
//   Guideline:           AUTOSAR CPP14 Guidelines, A13.3.1 required
//   Description:         A function that contains forwarding reference as its argument shall not be
//                        overloaded.
//   Justification:       Using this overloading functions with forwarding reference argument it is clear on
//                        which function will be called.
//   Risk:                None.
//   Verification Method: Code Review.
//
// - A4.7.1-1
//   Guideline:           AUTOSAR CPP14 Guidelines, A4.7.1 required
//   Description:         Cast on result of arithmetic may cause overflow.
//   Justification:       This unique id value will always be in signed 8 bit and it is manually verified
//                        that it will not overflow.
//   Risk:                None.
//   Verification Method: Code Review.
//
// - M0.1.1-1
//   Guideline:           AUTOSAR CPP14 Guidelines, M0.1.1 required
//   Description:         A project shall not contain unreachable code.
//   Justification:       This violation present in MACRO which is standard C++ MACRO so can not be modified.
//   Risk:                None.
//   Verification Method: Code Review.
//
// - A4.7.1-2
//   Guideline:           AUTOSAR CPP14 Guidelines, A4.7.1 required
//   Description:         Cast may truncate value.
//   Justification:       No Data loss will be negligible.
//   Risk:                None.
//   Verification Method: Code Review.
//
// - A5.1.1-1
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A5.1.1 required
//   Description:         Use of magic literal.
//   Justification:       This will create another violation for initialization with magic literal. This has
//                        been analyzed and verified manually. There is no impact on code quality.
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
