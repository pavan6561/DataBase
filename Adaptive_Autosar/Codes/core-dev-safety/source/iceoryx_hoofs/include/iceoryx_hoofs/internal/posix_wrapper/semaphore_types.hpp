// Copyright (c) 2022 by Apex.AI Inc. All rights reserved.
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
#ifndef IOX_HOOFS_POSIX_WRAPPER_SEMAPHORE_HEADER_HPP
#define IOX_HOOFS_POSIX_WRAPPER_SEMAPHORE_HEADER_HPP

namespace iox
{
namespace posix
{
enum class SemaphoreError
{
    CREATION_FAILED,
    NAME_TOO_LONG,
    UNABLE_TO_OPEN_HANDLE,
    INVALID_NAME,
    INVALID_SEMAPHORE_HANDLE,
    SEMAPHORE_OVERFLOW,
    INTERRUPTED_BY_SIGNAL_HANDLER,
    PERMISSION_DENIED,
    ALREADY_EXIST,
    FILE_DESCRIPTOR_LIMIT_REACHED,
    NO_SEMAPHORE_WITH_THAT_NAME_EXISTS,
    OUT_OF_MEMORY,
    UNDEFINED
};

enum class SemaphoreWaitState
{
    TIMEOUT,
    NO_TIMEOUT,
};
} // namespace posix
} // namespace iox

#endif
