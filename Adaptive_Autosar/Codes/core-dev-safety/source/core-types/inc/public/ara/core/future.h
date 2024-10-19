// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : future.h
//
// Purpose     : This file provides the implementation of class future.
// -----------------------------------------------------------------------------------------------------------

#ifndef APD_ARA_CORE_FUTURE_HPP_
#define APD_ARA_CORE_FUTURE_HPP_

#include <chrono>
#include <future>
#include <system_error>
#include <cassert>

#include "ara/core/future_error_domain.h"
#include "ara/core/error_code.h"
#include "ara/core/result.h"
#include "ara/core/core_error_domain.h"
#include "ara/core/exception.h"
#include "internal/state.h"

#if !defined(ATTR_NODISCARD)
#if __cplusplus >= 201703L
#define ar_attribute_nodiscard [[nodiscard]]
#else
#if defined(__GNUC__) || defined(__clang__)
#define ar_attribute_nodiscard __attribute__((warn_unused_result))
#else
#define ar_attribute_nodiscard /* nothing */
#endif
#endif
#define ATTR_NODISCARD ar_attribute_nodiscard
#endif

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief The class template std::promise provides a facility to store a value
/// or an exception that is later acquired asynchronously via a std::future object
/// created by the std::promise object.
template <typename, typename>
class Promise;

/// @brief Specifies the state of a Future as returned by wait_for() and wait_until().
/// These definitions are equivalent to the ones from std::future_status. However, the
/// item std::future_status::deferred is not supported here.
/// @trace SWS_CORE_S7.2.1.3.4_P2
/// @archtrace 609889
enum class future_status : uint8_t
{
  ready = 1, ///< the shared state is ready
  timeout, ///< the shared state did not become ready before the specified timeout has passed
};

/// @brief  The operator<< applied to an output stream is known as insertion operator, and performs formatted output.
/// @param[in] out  Output stream object where characters are inserted.
/// @param[in] ex  The type of the exception.
/// @returns It should be true.
/// @trace SWS_CORE_S7.2.1.3.4_P2
/// @archtrace 609889
/// @design 611842
inline std::ostream& operator<<(std::ostream& out, FutureException const& ex)
{
  return (out << "FutureException: " << ex.Error() << " (" << ex.what() << ")");
}

/// @brief Provides ara::core specific Future operations to collect the results of an asynchronous call.
/// Much of its functionality is delegated to std::future and all methods that resemble std::future are guaranteed to behave the same.
/// If the valid() member function of an instance returns true, all other methods are guaranteed to work on that
/// instance. Otherwise, they may fail with or without an exception. A thrown exception will be of type std::future_error.
/// Having an invalid future will usually happen when the future was moved from using the move constructor or move assignment.
/// @tparam T The type of value of class Future.
/// @tparam E The type of error of class Future.
/// @trace SWS_CORE_S7.2.1.3.4_P2 SWS_CORE_00002
/// @archtrace 609889
/// @design 611306 614039
template <typename T, typename E = ErrorCode>
class Future final
{
  /// @brief R function with return value.
  /// We will use the return statement to return some value.
  using R = Result<T, E>;

  /// @brief locks (i.e., takes ownership of) the associated mutex.
  using Lock = std::unique_lock<std::mutex>;

 public:
  /// @brief Alias type for T
  using ValueType = T;
  /// @brief Alias type for the Promise type collaborating with this Future type
  using PromiseType = Promise<T, E>;

  /// @brief Default constructor
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  Future() noexcept = default;

  /// @brief The destructor for the class Future.
  /// This will also disable any callback that has been set.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611307
  ~Future()
  {
    Lock lock(mutex_);

    if (extra_state_)
    {
      extra_state_->SetCallback(nullptr);
    }
  }

  /// @brief Future objects cannot be copied.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  Future(Future const&) = delete;

  /// @brief The future is not CopyAssignable.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  Future& operator=(Future const&) = delete;

  /// @brief  Move constructor. Constructs a std::future with the shared state of other
  /// using move semantics. After construction, other.valid() == false.
  /// @param[in] other A Future that will transfer state to future class.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611308
  Future(Future&& other) noexcept :
      lock_(other.mutex_), delegate_future_(std::move(other.delegate_future_)),
      extra_state_(std::move(other.extra_state_))
  {
    lock_.unlock();
  }

  /// @brief Releases any shared state and move-assigns the contents of other to *this.
  /// After the assignment, other.valid() == false and this->valid() will yield the same value
  /// as other.valid() before the assignment.
  /// @param[in] other A Future that will transfer state to future class.
  /// @returns Reference to future class.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611309
  Future& operator=(Future&& other) noexcept
  {
    if (this != &other)
    {
      Lock lhsLock(mutex_, std::defer_lock);
      Lock rhsLock(other.mutex_, std::defer_lock);
      std::lock(lhsLock, rhsLock);

      delegate_future_ = std::move(other.delegate_future_);
      extra_state_ = std::move(other.extra_state_);
    }
    return *this;
  }

  /// @brief Stop defining noexcept in order to let other exception be propageted to upper levels and fix some crashes.
  /// @returns It returns the content of r.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611310
  ATTR_NODISCARD R GetResult()
  {
#ifndef ARA_NO_EXCEPTIONS
    try
    {
      return delegate_future_.get();
    }
    catch (std::future_error const& ex)
    {
      std::error_code const& ec = ex.code();
      FutureErrc err;
      if (ec == std::future_errc::broken_promise)
      {
        err = FutureErrc::kBrokenPromise;
      }
      else if (ec == std::future_errc::future_already_retrieved)
      {
        err = FutureErrc::kFutureAlreadyRetrieved;
      }
      else if (ec == std::future_errc::promise_already_satisfied)
      {
        err = FutureErrc::kPromiseAlreadySatisfied;
      }
      else if (ec == std::future_errc::no_state)
      {
        err = FutureErrc::kNoState;
      }
      else
      {
        // Should rather use a vendor/demonstrator-specific ErrorDomain here?
        return R::FromError(CoreErrc::kInvalidArgument);
      }
      R r = R::FromError(err);
      return r;
    }
#else
    // TODO: Somehow query the future object whether it contains an exception,
    // and if so, translate it into a Result<...>
    // This does not seem possible with C++14's std::future, because it lacks boost::future's
    // has_exception() method. Unfortunately, boost::future cannot be compiled with
    // -fno-exceptions, so that one is out as well.
    return delegate_future_.get();
#endif
  }

#ifndef ARA_NO_EXCEPTIONS

  /// @brief Get the value.
  /// This call blocks until the result or an exception is available.
  /// @returns value of type T.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611311
  T get()
  {
    return GetResult().ValueOrThrow();
  }
#endif

  /// @brief Checks if the future is valid, i.e. if it has a shared state.
  /// @returns true if the future is usable, false otherwise
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611312
  bool valid() const noexcept
  {
    return delegate_future_.valid();
  }

  /// @brief Waits for a value or an exception to be available.
  /// After this method returns, get() is guaranteed to not block and is_ready() will return true.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611313
  void wait() const
  {
    delegate_future_.wait();
  }

  /// @brief Wait for the given period.
  /// If the Future becomes ready or the timeout is reached, the method returns.
  /// @tparam Rep Signed arithmetic type representing the number of ticks in the clock's duration.
  /// @tparam Period A ratio type representing the tick period of the clock, in seconds.
  /// @param[in] timeout_duration Maximum duration to wait for
  /// @returns status that indicates whether the timeout hit or if a value is available
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611314
  template <typename Rep, typename Period>
  future_status wait_for(std::chrono::duration<Rep, Period> const& timeout_duration) const
  {
    switch (delegate_future_.wait_for(timeout_duration))
    {
    case std::future_status::ready:
      return future_status::ready;
    case std::future_status::timeout:
      return future_status::timeout;
    default:
      assert(!"this std::future_status should not occur in our setup");
    }
  }
  /// @brief Wait until the given time. If the Future becomes ready or the time is reached, the method returns.
  /// @tparam Clock The clock on which this time point is measured.
  /// @tparam Duration Its used to measure the time since epoch.
  /// @param[in] deadline Latest point in time to wait.
  /// @returns status that indicates whether the time was reached or if a value is available
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611315
  template <typename Clock, typename Duration>
  future_status wait_until(std::chrono::time_point<Clock, Duration> const& deadline) const
  {
    switch (delegate_future_.wait_until(deadline))
    {
    case std::future_status::ready:
      return future_status::ready;
    case std::future_status::timeout:
      return future_status::timeout;
    default:
      assert(!"this std::future_status should not occur in our setup");
    }
  }

  /// @brief Register a function that gets called when the future becomes ready.
  /// When a func is called, it is guaranteed that get() will not block.
  /// A func may be called in the context of this call or in the context of Promise::set_value()
  /// or Promise::set_exception() or somewhere else.
  /// @tparam F It is useful for passing callbacks to other functions.
  /// @param[in] func A Callable to register to get the Future result or an exception
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611316
  template <typename F>
  void then(F&& func)
  {
    Lock lock(mutex_);

    extra_state_->SetCallback_NoLock(std::forward<F>(func));
    if (is_ready())
    {
      extra_state_->FireCallback_NoLock();
    }
  }

  //// True when the future contains either a result or an exception.
  /// @brief If is_ready() returns true, get() and the wait calls are guaranteed to not block.
  /// @returns true if the Future contains data, false otherwise
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611317
  bool is_ready() const
  {
    return std::future_status::ready == delegate_future_.wait_for(std::chrono::seconds::zero());
  }

 private:
  /// @brief Constructs a Future from a given std::future and a pointer to the extra state.
  /// @param[in] delegate_future It is future instance.
  /// @param[in] extra_state State that is shared with the Promise.
  /// @design 611306
  Future(std::future<R>&& delegate_future, internal::State::Ptr extra_state) :
      delegate_future_(std::move(delegate_future)), extra_state_(extra_state)
  {
  }

  /// @brief The mutex class is a synchronization primitive that can be used to protect
  /// shared data from being simultaneously accessed by multiple threads.
  std::mutex mutex_;
  Lock lock_; // this Lock is only necessary for the move constructor

  /// @brief A std::future<R> is a future that can be used to wait for the result of an
  /// asynchronous operation.
  /// The delegate_future_ member of a std::future<R> allows you to call a function that
  /// will be called when the future is ready.
  std::future<R> delegate_future_;
  /// @brief The identifier internal_state is local to some_state_function - it's not visible outside the function.
  internal::State::Ptr extra_state_;

  template <typename, typename>
  friend class Promise;
};

/// @brief Specialization of class Future for "void" values
/// @tparam E  The type of error.
/// @trace SWS_CORE_S7.2.1.3.4_P2
/// @archtrace 609889
/// @design 611496
template <typename E>
class Future<void, E> final
{
  /// @brief R function with return value.
  using R = Result<void, E>;

  /// @brief locks (i.e., takes ownership of) the associated mutex.
  using Lock = std::unique_lock<std::mutex>;

 public:
  /// @brief Alias type for T.
  using ValueType = void;
  /// @brief Alias type for the Promise type collaborating with this Future type.
  using PromiseType = Promise<void, E>;
  /// @brief Alias type for the future_status type.
  using Status = future_status;

  /// @brief The default constructor for Future objects.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  Future() noexcept = default;

  /// @brief The destructor for class Future.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611497
  ~Future()
  {
    Lock lock(mutex_);

    if (extra_state_)
    {
      extra_state_->SetCallback(nullptr);
    }
  }

  /// @brief Copy constructor shall be disabled.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  Future(Future const&) = delete;

  /// @brief Copy assignment operator shall be disabled.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  Future& operator=(Future const&) = delete;

  /// @brief Move assign from another instance.
  /// @param[in] other A Future that will transfer state to future class.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611498
  Future(Future&& other) noexcept :
      lock_(other.mutex_), delegate_future_(std::move(other.delegate_future_)),
      extra_state_(std::move(other.extra_state_))
  {
    lock_.unlock();
  }

  /// @brief Move assign from another instance.
  /// @param[in] other A Future that will transfer state to future class.
  /// @returns Reference to this.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611499
  Future& operator=(Future&& other) noexcept
  {
    if (this != &other)
    {
      Lock lhsLock(mutex_, std::defer_lock);
      Lock rhsLock(other.mutex_, std::defer_lock);
      std::lock(lhsLock, rhsLock);

      delegate_future_ = std::move(other.delegate_future_);
      extra_state_ = std::move(other.extra_state_);
    }
    return *this;
  }

#ifndef ARA_NO_EXCEPTIONS
  /// @brief Get the value.
  /// @brief It returns the element at the specified position in this list.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611500
  void get()
  {
    return GetResult().ValueOrThrow();
  }
#endif

  /// @brief Get the result.
  /// @returns A future object associated with the object's shared state.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611501
  ATTR_NODISCARD Result<void, E> GetResult() noexcept
  {
#ifndef ARA_NO_EXCEPTIONS
    try
    {
      return delegate_future_.get();
    }
    catch (std::future_error const& ex)
    {
      std::error_code const& ec = ex.code();
      FutureErrc err;
      if (ec == std::future_errc::broken_promise)
      {
        err = FutureErrc::kBrokenPromise;
      }
      else if (ec == std::future_errc::future_already_retrieved)
      {
        err = FutureErrc::kFutureAlreadyRetrieved;
      }
      else if (ec == std::future_errc::promise_already_satisfied)
      {
        err = FutureErrc::kPromiseAlreadySatisfied;
      }
      else if (ec == std::future_errc::no_state)
      {
        err = FutureErrc::kNoState;
      }
      else
      {
        // Should rather use a vendor/demonstrator-specific ErrorDomain here?
        return R::FromError(CoreErrc::kInvalidArgument);
      }
      R r = R::FromError(err);
      return r;
    }
#else
    return delegate_future_.get();
#endif
  }

  /// @brief Checks if the Future is valid, i.e. if it has a shared state.
  /// @returns true if the object is associated with a shared state, false otherwise.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611502
  bool valid() const noexcept
  {
    return delegate_future_.valid();
  }

  /// @brief Wait for a value or an error to be available.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611503
  void wait() const
  {
    delegate_future_.wait();
  }

  /// @brief Wait for the given period, or until a value or an error is available.
  /// @tparam Rep Signed arithmetic type representing the number of ticks in the clock's duration.
  /// @tparam Period A ratio type representing the tick period of the clock, in seconds.
  /// @param[in] timeoutDuration Maximum duration to wait for.
  /// @returns The shared state contains a deferred function using lazy evaluation,
  /// so the result will be computed only when explicitly requested
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611504
  template <typename Rep, typename Period>
  future_status wait_for(std::chrono::duration<Rep, Period> const& timeoutDuration) const
  {
    switch (delegate_future_.wait_for(timeoutDuration))
    {
    case std::future_status::ready:
      return future_status::ready;
    case std::future_status::timeout:
      return future_status::timeout;
    default:
      assert(!"this std::future_status should not occur in our setup");
    }
  }

  /// @brief Wait until the given time, or until a value or an error is available.
  /// @tparam Clock The clock on which this time point is measured.
  /// @tparam Duration Its used to measure the time since epoch.
  /// @param[in] deadline Latest point in time to wait.
  /// @returns The shared state contains a deferred function using lazy evaluation,
  /// so the result will be computed only when explicitly requested.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611505
  template <typename Clock, typename Duration>
  future_status wait_until(std::chrono::time_point<Clock, Duration> const& deadline) const
  {
    switch (delegate_future_.wait_until(deadline))
    {
    case std::future_status::ready:
      return future_status::ready;
    case std::future_status::timeout:
      return future_status::timeout;
    default:
      assert(!"this std::future_status should not occur in our setup");
    }
  }

  // not yet implemented, because the current general implementation above is nonsense anyway
  // @trace SWS_CORE_06231
  // template <typename F>
  // auto then(F&& func) -> Future<SEE_BELOW>;

  /// @brief Return whether the asynchronous operation has finished.
  /// @returns true if the associated shared state is ready, otherwise false.
  /// @trace SWS_CORE_S7.2.1.3.4_P2
  /// @archtrace 609889
  /// @design 611506
  bool is_ready() const
  {
    return std::future_status::ready == delegate_future_.wait_for(std::chrono::seconds::zero());
  }

 private:
  /// @brief Constructs a Future from a given std::future and a pointer to the extra state.
  /// @param[in] delegate_future It is future instance.
  /// @param[in] extra_state State that is shared with the Promise.
  /// @design 611496
  Future(std::future<R>&& delegate_future, internal::State::Ptr extra_state) :
      delegate_future_(std::move(delegate_future)), extra_state_(extra_state)
  {
  }

  /// @brief The mutex class is a synchronization primitive that can be used to protect
  /// shared data from being simultaneously accessed by multiple threads.
  std::mutex mutex_;
  Lock lock_; // this Lock is only necessary for the move constructor

  /// @brief A future that can be used to wait for the result of an
  /// asynchronous operation.
  /// The delegate_future_ member of a std::future<R> allows you to call a function that
  /// will be called when the future is ready.
  std::future<R> delegate_future_;
  /// @brief The identifier internal_state is local to some_state_function - it's not visible outside the function.
  internal::State::Ptr extra_state_;

  template <typename, typename>
  friend class Promise;
};

} // namespace core
} // namespace ara

#endif // APD_ARA_CORE_FUTURE_HPP_
