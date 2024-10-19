// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : promise.h
//
// Purpose     : This file provides the implementation of class promise.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_PROMISE_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_PROMISE_H_

#include <exception>
#include <future>
#include <mutex>
#include <system_error>

#include "internal/state.h"
#include "ara/core/error_code.h"
#include "ara/core/result.h"
#include "future.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core
namespace core
{
/// @brief The class template std::promise provides a facility to store a value or an exception
/// that is later acquired asynchronously via a std::future object created by the std::promise object.
/// @tparam T The type of an value.
/// @tparam E The error code instance.
/// @trace SWS_CORE_S7.2.1.3.4_P1
/// @archtrace 609890
/// @design 611337 614039
template <typename T, typename E = ErrorCode>
class Promise final
{
  /// @brief R function with return value.
  /// We will use the return statement to return some value
  using R = Result<T, E>;

  /// @brief locks (i.e., takes ownership of) the associated mutex.
  using Lock = std::unique_lock<std::mutex>;

 public:
  /// @brief The default constructor of Promise object.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611338
  Promise() : extra_state_(std::make_shared<internal::State>())
  {
  }

  /// @brief The destructor for Promise objects.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611339
  ~Promise()
  {
  }

  /// @brief Copy constructor shall be disabled.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  Promise(Promise const&) = delete;

  /// @brief Copy assignment operator shall be disabled.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  Promise& operator=(Promise const&) = delete;

  /// @brief The move constructor of Promise object.
  /// @param[in] other The other source instance.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611340
  Promise(Promise&& other) noexcept :
      lock_(other.mutex_), delegate_promise_(std::move(other.delegate_promise_)),
      extra_state_(std::move(other.extra_state_))
  {
    lock_.unlock();
  }

  /// @brief The move assignment of Promise object.
  /// @param[in] other The other source instance.
  /// @returns Reference to promise class.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611341
  Promise& operator=(Promise&& other) noexcept
  {
    if (this != &other)
    {
      Lock lhsLock(mutex_, std::defer_lock);
      Lock rhsLock(other.mutex_, std::defer_lock);
      std::lock(lhsLock, rhsLock);

      delegate_promise_ = std::move(other.delegate_promise_);
      extra_state_ = std::move(other.extra_state_);
    }
    return *this;
  }

  /// @brief Swap the contents of this instance with another one’s.
  /// @param[in] other The other source instance.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611342
  void swap(Promise& other) noexcept
  {
    Lock lhsLock(mutex_, std::defer_lock);
    Lock rhsLock(other.mutex_, std::defer_lock);
    std::lock(lhsLock, rhsLock);

    using std::swap;
    swap(delegate_promise_, other.delegate_promise_);
    swap(extra_state_, other.extra_state_);
  }

  /// @brief Returns an associated Future for type T.
  /// The returned Future is set as soon as this Promise receives the result or an exception. This method must only be
  /// called once as it is not allowed to have multiple Futures per Promise.
  /// @returns Returns a future object associated with the object's shared state.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611343
  Future<T, E> get_future()
  {
    return Future<T, E>(delegate_promise_.get_future(), extra_state_);
  }

  /// @brief Move an error into the shared state and make the state ready.
  /// @param[in] err The error to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611344
  void SetError(E&& err)
  {
    R r = R::FromError(std::move(err));
    SetResult(std::move(r));
  }

  /// @brief Copy an error into the shared state and make the state ready.
  /// @param[in] err The error to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611345
  void SetError(E const& err)
  {
    R r = R::FromError(err);
    SetResult(r);
  }

  /// @brief Copy a Result into the shared state and make the state ready.
  /// @param[in] result The result to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611346
  void SetResult(const Result<T, E>& result)
  {
    Lock lock(mutex_);

    delegate_promise_.set_value(result);
    extra_state_->FireCallback_NoLock();
  }

  /// @brief Move a Result into the shared state and make the state ready.
  /// @param[in] result The result to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611347
  void SetResult(Result<T, E>&& result)
  {
    Lock lock(mutex_);

    delegate_promise_.set_value(std::move(result));
    extra_state_->FireCallback_NoLock();
  }

  /// @brief Sets an exception. Calling Get() on the associated Future will rethrow the exception
  /// in the context the Future's method was called in.
  /// @param[in] p An exception ptr object. It is a smart pointer type designed to reference exception objects.
  /// @note This method is DEPRECATED. The exception is defined by the error code.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611348
  void set_exception(std::exception_ptr p)
  {
    Lock lock(mutex_);

    delegate_promise_.set_exception(p);
    extra_state_->FireCallback_NoLock();
  }

  /// @brief Move the result into the future.
  /// @param[in] value The value to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611349
  void set_value(T&& value)
  {
    R r = std::move(value);
    SetResult(std::move(r));
  }

  /// @brief Copy result into the future.
  /// @param[in] value The value to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611350
  void set_value(T const& value)
  {
    R r = value;
    SetResult(r);
  }

 private:
  /// @brief The mutex class is a synchronization primitive that can be used to protect
  /// shared data from being simultaneously accessed by multiple threads.
  std::mutex mutex_;
  Lock lock_; // this Lock is only necessary for the move constructor

  /// @brief A promise that can be used to wait for the result of an asynchronous operation.
  /// The delegate_promise_ member of a std::promise<R> allows you to call a function that
  /// will be called when the promise is ready.
  std::promise<R> delegate_promise_;
  /// @brief The identifier internal_state is local to some_state_function - it's not visible outside the function.
  internal::State::Ptr extra_state_;
};

/// @brief Specialization of class promise for void values.
/// @tparam E The type of an error.
/// @trace SWS_CORE_S7.2.1.3.4_P1
/// @archtrace 609890
/// @design 611535
template <typename E>
class Promise<void, E> final
{
  /// @brief R function with return value.
  using R = Result<void, E>;

  /// @brief locks (i.e., takes ownership of) the associated mutex.
  using Lock = std::unique_lock<std::mutex>;

 public:
  /// @brief The default constructor for Promise objects.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611536
  Promise() : extra_state_(std::make_shared<internal::State>())
  {
  }

  /// @brief Copy constructor shall be disabled.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  Promise(Promise const&) = delete;

  /// @brief The move constructor for Promise objects.
  /// @param[in] other The another promise to acquire the state from.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611537
  Promise(Promise&& other) noexcept :
      lock_(other.mutex_), delegate_promise_(std::move(other.delegate_promise_)),
      extra_state_(std::move(other.extra_state_))
  {
    lock_.unlock();
  }

  /// @brief The destructor for Promise objects.
  /// @trace SWS_CORE_06349
  /// @archtrace 609890
  /// @design 611538
  ~Promise()
  {
  }

  /// @brief Copy assignment operator shall be disabled.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  Promise& operator=(Promise const&) = delete;

  /// @brief The move assignment for Promise objects.
  /// @param[in] other The another promise to acquire the state from.
  /// @returns Reference to Promise class.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611539
  Promise& operator=(Promise&& other) noexcept
  {
    if (this != &other)
    {
      Lock lhsLock(mutex_, std::defer_lock);
      Lock rhsLock(other.mutex_, std::defer_lock);
      std::lock(lhsLock, rhsLock);

      delegate_promise_ = std::move(other.delegate_promise_);
      extra_state_ = std::move(other.extra_state_);
    }
    return *this;
  }

  /// @brief Swap the contents of this instance with another one’s.
  /// @param[in] other The promise to swap with.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611540
  void swap(Promise& other) noexcept
  {
    Lock lhsLock(mutex_, std::defer_lock);
    Lock rhsLock(other.mutex_, std::defer_lock);
    std::lock(lhsLock, rhsLock);

    using std::swap;
    swap(delegate_promise_, other.delegate_promise_);
    swap(extra_state_, other.extra_state_);
  }

  /// @brief Return the associated Future.
  /// @returns It returns a Future.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611541
  Future<void, E> get_future()
  {
    return Future<void, E>(delegate_promise_.get_future(), extra_state_);
  }

  /// @brief Make the shared state ready.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611542
  void set_value()
  {
    R r = R::FromValue();
    SetResult(std::move(r));
  }

  /// @brief Move an error into the shared state and make the state ready.
  /// @param[in] err The error to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611543
  void SetError(E&& err)
  {
    R r = R::FromError(std::move(err));
    SetResult(std::move(r));
  }

  /// @brief Copy an error into the shared state and make the state ready.
  /// @param[in] err The error to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611544
  void SetError(E const& err)
  {
    R r = R::FromError(err);
    SetResult(r);
  }

  /// @brief Copy a Result into the shared state and make the state ready.
  /// @param[in] result The result to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611545
  void SetResult(const Result<void, E>& result)
  {
    Lock lock(mutex_);

    delegate_promise_.set_value(result);
    extra_state_->FireCallback_NoLock();
  }

  /// @brief Move a Result into the shared state and make the state ready.
  /// @param[in] result The result to store.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611546
  void SetResult(Result<void, E>&& result)
  {
    Lock lock(mutex_);

    delegate_promise_.set_value(std::move(result));
    extra_state_->FireCallback_NoLock();
  }

  // unspecified, remove this!
  /// @brief Stores the exception pointer p in the shared state, which becomes ready.
  /// @param[in] p Is a smart pointer type designed to reference exception objects.
  /// @trace SWS_CORE_S7.2.1.3.4_P1
  /// @archtrace 609890
  /// @design 611547
  void set_exception(std::exception_ptr p)
  {
    Lock lock(mutex_);

    delegate_promise_.set_exception(p);
    extra_state_->FireCallback_NoLock();
  }

 private:
  /// @brief The mutex class is a synchronization primitive that can be used to protect
  /// shared data from being simultaneously accessed by multiple threads.
  std::mutex mutex_;
  Lock lock_; // this Lock is only necessary for the move constructor

  /// @brief A promise that can be used to wait for the result of an asynchronous operation.
  /// The delegate_promise_ member of a std::promise<R> allows you to call a function that
  /// will be called when the promise is ready.
  std::promise<R> delegate_promise_;
  /// @brief The identifier internal_state is local to some_state_function - it's not visible outside the function.
  internal::State::Ptr extra_state_;
};

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_PROMISE_H_
