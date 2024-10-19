// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2023 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : application.h
//
// Purpose     : Integration Testing implementation of core.
// -----------------------------------------------------------------------------------------------------------
#ifndef CORE_TEST_SCRIPTS_IT_MODULE_SCRIPTS_CORE_APP_INC_APPLICATION_H_
#define CORE_TEST_SCRIPTS_IT_MODULE_SCRIPTS_CORE_APP_INC_APPLICATION_H_

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <memory>
#include <thread>
#include <chrono>
#include <map>
#include <ara/core/span.h>
#include <ara/core/utility.h>
#include <ara/log/logging.h>
#include <ara/core/string.h>
#include <ara/core/vector.h>
#include <gtest/gtest.h>
#include <ara/core/initialization.h>
#include <ara/core/steady_clock.h>
#include <ara/core/timer/timer_thread.h>
#include <ara/core/thread/thread.h>
#include <condition_listener.h>
#include <condition_notifier.h>
#include <condition_variable_data.h>
#include "ara/core/configreader/config_reader.h"
#include "sm_server.hpp"

#include "ara/core/internal/pmr/utilities.h"
#include "ara/core/monotonic_buffer_resource.h"
#include "ara/core/polymorphic_allocator.h"
#include "ara/core/unsynchronized_pool_resource.h"

using namespace std;

namespace it
{
namespace core
{
namespace application
{
/// @brief It creates a CoreApp class.
class CoreApp
{
 public:
  /// @brief creates a CoreApp constructor.
  CoreApp();
  /// @brief creates a CoreApp destructor.
  ~CoreApp();
  /// @brief creates a Init method.
  void Init();
  /// @brief creates a handler_fun method.
  bool handler_fun(bool flag);

  void push(ara::core::pmr::Vector<int>& vec);
  void pushhandler();
  void pop(ara::core::pmr::Vector<int>& vec);
  void erase(ara::core::pmr::Vector<int>& vec);
  void DefaultUnsyncpmr();
  void NullMemorypmr();
  void NewDeleteMemory();

  /// @brief creates a IT_Core methods.
  void IT_Core_0001();
  void IT_Core_0002();
  void IT_Core_0003();
  void IT_Core_0004();
  void IT_Core_0005();
  void IT_Core_0006();
  void IT_Core_0007();
  void IT_Core_0008();
  void IT_Core_0009();
  void IT_Core_0010();
  void IT_Core_Pmr_0001();
  void IT_Core_Pmr_0002();
  void IT_Core_Pmr_0003();
  void IT_Core_Pmr_0004();

  template <std::size_t Length>
  /// @brief creates a ParseArrayInvalidHelper method.
  void ParseArrayInvalidHelper(const char (&buffer)[Length],
      const ara::core::configreader::ParseError::ErrorReason expectedReason,
      const char* const expectedWhat = nullptr);

  template <std::size_t Length>
  /// @brief creates a ParseObjectInvalidHelper method.
  void ParseObjectInvalidHelper(const char (&buffer)[Length],
      const ara::core::configreader::ParseError::ErrorReason expectedReason,
      const char* const expectedWhat = nullptr)
  {
    ara::core::configreader::ConstBufferContext ConstBufferContext(buffer, sizeof(buffer) - 1);

    try
    {
      ara::core::configreader::ParseObject(ConstBufferContext,
          ara::core::configreader::detail::Ignore<ara::core::configreader::ConstBufferContext>(
              ConstBufferContext));
    }
    catch (const ara::core::configreader::ParseError& e)
    {
      araLog_.LogVerbose() << "ParseObject () should return " << expectedWhat << " error.";
    }
  }

 private:
  /// @brief creates a unique ptr of TimerThread.
  std::unique_ptr<ara::core::timer::TimerThread> timerObj_;
  /// @brief creates a object of Thread.
  iox::cxx::optional<ara::core::thread::Thread> threadObj_;
  /// @brief creates a object of ConditionVariableData.
  ara::core::ConditionVariableData condVarData_{"CoreItCondVar"};
  /// @brief creates a object of ConditionListener.
  ara::core::ConditionListener condListener_{condVarData_};
  /// @brief creates a object of ConditionNotifier.
  ara::core::ConditionNotifier condSignaler_{condVarData_, 1};
  /// @brief creates a CreateLogger method.
  ara::log::Logger& araLog_{
      ara::log::CreateLogger("COREAP", "Context for CORE ", ara::log::LogLevel::kVerbose)};
};

} // namespace application
} // namespace core
} // namespace it

#endif // CORE_TEST_SCRIPTS_IT_MODULE_SCRIPTS_CORE_APP_INC_APPLICATION_H_