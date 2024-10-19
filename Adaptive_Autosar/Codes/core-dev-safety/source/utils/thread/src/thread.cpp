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

#include "iceoryx_hoofs/posix_wrapper/posix_call.hpp"
#include "iceoryx_hoofs/cxx/helplets.hpp"
#include "iceoryx_hoofs/log/logging.hpp"
#include "../inc/public/ara/core/thread/thread.h"

namespace ara
{
namespace core
{
namespace thread
{
void SetThreadName(pthread_t thread, const ThreadName& name) noexcept
{
  iox::posix::posixCall(iox_pthread_setname_np)(thread, name.c_str())
      .successReturnValue(0)
      .evaluate()
      .or_else([](auto& r) {
        // String length limit is ensured through iox::cxx::string
        // ERANGE (string too long) intentionally not handled to avoid untestable and dead code
        std::cerr << "This should never happen! " << r.getHumanReadableErrnum() << std::endl;
        iox::cxx::Ensures(false && "internal logic error");
      });
}

ThreadName GetThreadName(pthread_t thread) noexcept
{
  char tempName[MAX_THREAD_NAME_LENGTH + 1U];

  iox::posix::posixCall(pthread_getname_np)(thread, tempName, MAX_THREAD_NAME_LENGTH + 1U)
      .successReturnValue(0)
      .evaluate()
      .or_else([](auto& r) {
        // String length limit is ensured through MAX_THREAD_NAME_LENGTH
        // ERANGE (string too small) intentionally not handled to avoid untestable and dead code
        std::cerr << "This should never happen! " << r.getHumanReadableErrnum() << std::endl;
        iox::cxx::Ensures(false && "internal logic error");
      });

  return ThreadName(iox::cxx::TruncateToCapacity, tempName);
}
iox::cxx::expected<ThreadError> ThreadBuilder::create(
    iox::cxx::optional<Thread>& uninitializedThread, const Thread::Callable& callable) noexcept
{
  uninitializedThread.emplace(m_name, callable);

  const pthread_attr_t* threadAttributes{nullptr};

  auto createResult{iox::posix::posixCall(iox_pthread_create)(&uninitializedThread->mThreadHandle_,
      threadAttributes, Thread::StartRoutine, &uninitializedThread.value())
                        .successReturnValue(0)
                        .evaluate()};
  uninitializedThread->mIsThreadConstructed_ = !createResult.has_error();
  if (!uninitializedThread->mIsThreadConstructed_)
  {
    uninitializedThread.reset();
    return iox::cxx::error<ThreadError>(Thread::ErrnoToEnum(createResult.get_error().errnum));
  }
  return iox::cxx::success<>();
}

Thread::Thread(const ThreadName& name, const Callable& callable) noexcept :
    mThreadHandle_{}, mCallable_{callable}, mIsThreadConstructed_{false}, mThreadName_{name}
{
}

Thread::~Thread() noexcept
{
  if (mIsThreadConstructed_)
  {
    auto joinResult
        = iox::posix::posixCall(iox_pthread_join)(mThreadHandle_, nullptr).successReturnValue(0).evaluate();
    if (joinResult.has_error())
    {
      switch (joinResult.get_error().errnum)
      {
      case EDEADLK:
        //IOX_LOG(ERROR) << "A deadlock was detected when attempting to join the thread.";
        break;
      default:
        // IOX_LOG(ERROR) << "This should never happen. An unknown error occurred.";
        break;
      }
    }
  }
  else
  {
  }
}

ThreadName Thread::GetName() const noexcept
{
  return mThreadName_;
}

ThreadError Thread::ErrnoToEnum(const int errnoValue) noexcept
{
  ThreadError error;
  switch (errnoValue)
  {
  case EAGAIN:
  {
    error = ThreadError::kInsufficientResources;
    break;
  }
  case EINVAL:
  {
    error = ThreadError::kInvalidAttributes;
    break;
  }
  case ENOMEM:
  {
    error = ThreadError::kInsufficientMemory;
    break;
  }
  case EPERM:
  {
    error = ThreadError::kInsufficientPermissions;
    break;
  }
  default:
  {
    return ThreadError::kUndefined;
  }
  }
  return error;
}

void* Thread::StartRoutine(void* callable)
{
  auto* self{static_cast<Thread*>(callable)};
  auto threadHandle{iox_pthread_self()};

  iox::posix::posixCall(iox_pthread_setname_np)(threadHandle, self->mThreadName_.c_str())
      .successReturnValue(0)
      .evaluate()
      .or_else([&self](auto&) { self->mThreadName_.clear(); });

  self->mCallable_();
  return nullptr;
}

} // namespace thread
} // namespace core
} // namespace ara
