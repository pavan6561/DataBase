// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : state.h
//
// Purpose     : This file provides the implementation of class State.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_STATE_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_STATE_H_

#include <memory>
#include <mutex>
#include <functional>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Internal.
namespace internal
{
/// @brief Implementation of class State.
/// @trace SWS_CORE_00002
/// @archtrace 608891
/// @design 611351 614039
class State
{
 public:
  /// @brief The default constructor for State objects.
  typedef std::shared_ptr<State> Ptr;
  /// @brief The default constructor of State object.
  /// @trace SWS_CORE_00002
  /// @archtrace 608891
  /// @design 611352
  State()
  {
  }

  /// @brief The destructor for State objects.
  /// @trace SWS_CORE_00002
  /// @archtrace 608891
  /// @design 611353
  ~State()
  {
  }

  /// @brief Executes the stored callback.
  /// @note The callback is never exposed outside this object so that.
  /// no one may have a dangling pointer to the stored callback.
  /// @trace SWS_CORE_00002
  /// @archtrace 608891
  /// @design 611354
  void FireCallback()
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (HasCallback_NoLock())
    {
      callback_();
    }
  }

  /// @brief Executes the stored callback without mutex lock.
  /// @note As the name suggests, this is a version which is not doing.
  /// the mutex lock when running callback.
  /// @trace SWS_CORE_00002
  /// @archtrace 608891
  /// @design 611355
  void FireCallback_NoLock()
  {
    if (HasCallback_NoLock())
    {
      callback_();
    }
  }

  /// @brief Sets the callback. Any callback that was set before is removed and replaced by the new callback.
  /// @tparam F The function to be callback.
  /// @param[in] callback The callback to be set.
  /// @trace SWS_CORE_00002
  /// @archtrace 608891
  /// @design 611356
  template <typename F>
  void SetCallback(F&& callback)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    SetCallback_NoLock(std::forward<F>(callback));
  }

  /// @brief Returns if a callback was set previously.
  /// @returns true if a callback is set, false otherwise.
  /// @trace SWS_CORE_00002
  /// @archtrace 608891
  /// @design 611357
  bool HasCallback()
  {
    std::unique_lock<std::mutex> lock(mutex_);
    return HasCallback_NoLock();
  }

  /// @brief Sets the callback. Any callback that was set before is removed and replaced by the new callback.
  /// @tparam F The function to be callback.
  /// @param[in] callback The callback to be set.
  /// @note A mutex lock should be set before calling this function.
  /// @trace SWS_CORE_00002
  /// @archtrace 608891
  /// @design 611358
  template <typename F>
  void SetCallback_NoLock(F&& callback)
  {
    callback_ = std::forward<F>(callback);
  }

 private:
  /// @brief Returns if a callback was set previously.
  /// @note This function assumes a mutex lock is already set.
  /// @returns true if a callback is set, false otherwise.
  /// @trace SWS_CORE_00002
  /// @archtrace 608891
  /// @design 611357
  bool HasCallback_NoLock() const
  {
    return static_cast<bool>(callback_);
  }

  /// @brief Is a general-purpose polymorphic function wrapper.
  std::function<void(void)> callback_;
  /// @brief The mutex class is a synchronization primitive that can be used to protect
  /// shared data from being simultaneously accessed by multiple threads.
  std::mutex mutex_;
};

} // namespace internal
} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INTERNAL_STATE_H_