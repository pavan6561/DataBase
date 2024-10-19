// Copyright (c) 2020 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef CORE_SOURCE_UTILS_THREAD_INC_PUBLIC_ARA_CORE_THREAD_THREAD_H_
#define CORE_SOURCE_UTILS_THREAD_INC_PUBLIC_ARA_CORE_THREAD_THREAD_H_

#include "iceoryx_hoofs/cxx/expected.hpp"
#include "iceoryx_hoofs/cxx/function.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"
#include "iceoryx_hoofs/design_pattern/builder.hpp"
#include "iceoryx_hoofs/posix_wrapper/posix_call.hpp"
#include "iceoryx_hoofs/platform/pthread.hpp"

namespace ara
{
namespace core
{
namespace thread
{
constexpr uint64_t MAX_THREAD_NAME_LENGTH{15U};
/// @brief Namespace of string.Bring a specific member from the
/// namespace into the current scope.
using ThreadName = iox::cxx::string<MAX_THREAD_NAME_LENGTH>;
/// @brief Declaration of SetThreadName method.
/// @design 611253
void SetThreadName(pthread_t thread, const ThreadName& name) noexcept;
/// @brief Declaration of GetThreadName method.
/// @design 611253
ThreadName GetThreadName(pthread_t thread) noexcept;
/// @brief Implementation of ThreadError enum class.
enum class ThreadError
{
  kInsufficientMemory,
  kInsufficientPermissions,
  kInsufficientResources,
  kInvalidAttributes,
  kUndefined
};

/// @brief Class to represent individual threads of execution.
/// @trace REQ_AP_EC_CORE_0004
/// @archtrace 608894 609206
/// @design 611249 614041
class Thread
{
 public:
  /// @brief Namespace of function.Bring a specific member from the.
  /// namespace into the current scope.
  using Callable = iox::cxx::function<void()>;
  /// @brief Copy construction shall be disabled.
  Thread(const Thread&) = delete;
  /// @brief  Copy assignment operator shall be disabled.
  Thread& operator=(const Thread&) = delete;
  /// @brief Move constructor shall be disabled
  Thread(Thread&&) = delete;
  /// @brief Move assignment operator of the class Thread shall be disabled.
  Thread& operator=(Thread&&) = delete;

  /// @brief Destructor of class Thread.
  /// @trace REQ_AP_EC_CORE_0004
  /// @archtrace 608894
  /// @design 611250
  ~Thread() noexcept;

  /// @brief Returns the name of the thread.
  /// @returns An iox::cxx::string containing the name of the thread.
  /// @trace REQ_AP_EC_CORE_0004
  /// @archtrace 608894 609216
  /// @design 611251
  ThreadName GetName() const noexcept;

  /// @brief Declaration of friend class Thread.
  friend class ThreadBuilder;
  friend class iox::cxx::optional<Thread>;

 private:
  /// @brief Thread construction with parameters.
  /// @design 611249
  Thread(const ThreadName& name, const Callable& callable) noexcept;
  /// @brief Declaration of ErrnoToEnum method with static.
  /// @design 611249
  static ThreadError ErrnoToEnum(const int errnoValue) noexcept;
  /// @brief Declaration of StartRoutine method with static.
  /// @design 611249
  static void* StartRoutine(void* callable);
  /// @brief Declaration of pthread_t parameter.
  pthread_t mThreadHandle_;
  /// @brief Declaration of Callable parameter.
  Callable mCallable_;
  /// @brief Declaration of mIsThreadConstructed_ bool data type.
  bool mIsThreadConstructed_{false};
  /// @brief Declaration of ThreadName parameter.
  ThreadName mThreadName_;
};

/// @brief Declaration of class Thread.
/// @trace REQ_AP_EC_CORE_0004
/// @archtrace 608894 609206
/// @design 611252
class ThreadBuilder
{
  /// @brief Set the name of the thread.
  IOX_BUILDER_PARAMETER(ThreadName, name, "")

 public:
  /// @brief It creates a thread.
  /// @param[in] uninitializedThread Is an iox::cxx::optional where the thread is stored.
  /// @param[in] callable Is the callable that is invoked by the thread.
  /// @returns an error describing the failure or success.
  /// @trace REQ_AP_EC_CORE_0004
  /// @archtrace 608894 609217 608887 608953
  /// @design 611253
  iox::cxx::expected<ThreadError> create(
      iox::cxx::optional<Thread>& uninitializedThread, const Thread::Callable& callable) noexcept;
};

} // namespace thread
} // namespace core
} // namespace ara

#endif // CORE_SOURCE_UTILS_THREAD_INC_PUBLIC_ARA_CORE_THREAD_THREAD_H_
