// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
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

#ifndef CORE_SOURCE_UTILS_COND_VAR_SRC_TEST_MOCK_INC_WATCH_DOG_H
#define CORE_SOURCE_UTILS_COND_VAR_SRC_TEST_MOCK_INC_WATCH_DOG_H

#include "iceoryx_hoofs/internal/units/duration.hpp"
#include "iceoryx_hoofs/log/logging.hpp"
#include "iceoryx_hoofs/posix_wrapper/unnamed_semaphore.hpp"

#include <functional>
#include <gtest/gtest.h>
#include <thread>

using namespace iox::units::duration_literals;

// class for killing the application if a test takes too much time to finish
class Watchdog
{
  public:
    explicit Watchdog(const iox::units::Duration& timeToWait) noexcept
        : m_timeToWait(timeToWait)
    {
        iox::posix::UnnamedSemaphoreBuilder()
            .initialValue(0U)
            .isInterProcessCapable(false)
            .create(m_watchdogSemaphore)
            .expect("unable to create semaphore for Watchdog");
    }

    Watchdog(const Watchdog&) = delete;
    Watchdog(Watchdog&&) = delete;
    Watchdog& operator=(const Watchdog&) = delete;
    Watchdog& operator=(Watchdog&&) = delete;

    ~Watchdog() noexcept
    {
        reset();
    }

    void reset() noexcept
    {
        if (m_watchdog.joinable())
        {
            IOX_DISCARD_RESULT(m_watchdogSemaphore->post());
            m_watchdog.join();
        }
    }

    void watchAndActOnFailure(const std::function<void()>& actionOnFailure = std::function<void()>()) noexcept
    {
        reset();

        m_watchdog = std::thread([=] {
            m_watchdogSemaphore->timedWait(m_timeToWait)
                .and_then([&](auto& result) {
                    if (result == iox::posix::SemaphoreWaitState::TIMEOUT)
                    {
                        std::cerr << "Watchdog observed no reaction after " << m_timeToWait.toSeconds()
                                  << "s. Taking measures!" << std::endl;
                        if (actionOnFailure)
                        {
                            actionOnFailure();
                        }
                        else
                        {
                            std::terminate();
                        }
                        EXPECT_TRUE(false);
                    }
                })
                .or_else([](auto&) { EXPECT_TRUE(false); });
        });
    }

  private:
    iox::units::Duration m_timeToWait{0_s};
    iox::cxx::optional<iox::posix::UnnamedSemaphore> m_watchdogSemaphore;
    std::thread m_watchdog;
};

#endif // CORE_SOURCE_UTILS_COND_VAR_SRC_TEST_MOCK_INC_WATCH_DOG_H
