// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : application_lifecycle.h
//
// Purpose     : This file provides ApplicationLifeCycle class declaration
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_DAEMON_INC_PRIVATE_APPLICATION_LIFECYCLE_H_
#define CRYPTO_SOURCE_CRYPTO_DAEMON_INC_PRIVATE_APPLICATION_LIFECYCLE_H_

#include <csignal>

#include <apd/logconfig/initialization.h>
#include <ara/log/logging.h>
#include <atomic>
#include <thread>
#include <utility>

#include "crypto_daemon.h"

#ifdef USE_ARA
#include "ara/exec/execution_client.h"
using ExecutionClient = ara::exec::ExecutionClient;
using ExecutionState = ara::exec::ExecutionState;
#endif

/// @brief Signal handler.
namespace signal_handler
{
/// @brief Class for handling posix signals.
class SignalHandler final
{
 public:
  /// @brief Constructor for the class SignalHandler.
  SignalHandler() noexcept
  {
    static_cast<void>(::sigemptyset(&exitSignalSet_));
    static_cast<void>(::sigaddset(&exitSignalSet_, SIGTERM));
    static_cast<void>(::sigaddset(&exitSignalSet_, SIGINT));
  }

  /// @brief Constructor for the class SignalHandler.
  SignalHandler(const SignalHandler& other) = delete;

  /// @brief Constructor for the class SignalHandler.
  SignalHandler(SignalHandler&& other) = delete;

  /// @brief SignalHandler shall not be copy assignable.
  SignalHandler& operator=(const SignalHandler&) = delete;

  /// @brief SignalHandler shall not be move assignable.
  SignalHandler& operator=(SignalHandler&&) = delete;

  /// @brief Destructor for the class SignalHandler.
  ~SignalHandler() = default;

  /// @brief Handler of signals. Blocks until exit signal is received.
  void WaitForExitSignals() noexcept
  {
    aralog_.LogDebug() << "SignalHandler::" << __func__ << ": Waiting for Signals SIGINT or SIGTERM.";
    running_ = true;
    std::int32_t signal;
    while (running_)
    {
      static_cast<void>(sigwait(&exitSignalSet_, &signal));
      std::string const signalLog{ std::to_string(signal) };
      switch (signal)
      {
        case SIGINT:
          aralog_.LogInfo() << "SignalHandler::" << __func__ << ": SIGINT received";
          running_ = false;
          break;
        case SIGTERM:
          aralog_.LogInfo() << "SignalHandler::" << __func__ << ": SIGTERM received";
          running_ = false;
          break;
        default:
          aralog_.LogInfo() << "SignalHandler::" << __func__ << ": Signal received (" << signalLog
                            << "), but ignored.";
          break;
      }
    }
  }

  /// @brief Blocks exit signals on current thread.Signal handling requires blocking exit signals
  /// on all threads. It is sufficient to block signals on main
  /// thread, which implicitly also blocks signals for child threads.
  void BlockExitSignals() const noexcept
  {
    static_cast<void>(pthread_sigmask(SIG_BLOCK, &exitSignalSet_, nullptr));
  }

  /// @brief Stops waiting for exit signals.
  void StopWaitingForSignals() const noexcept
  {
    if (running_)
    {
      static_cast<void>(std::raise(SIGTERM));
    }
  }

 private:
  /// @brief Used as reference for exit signal.
  std::atomic_bool running_{ false };

  /// @brief Logging context instance.
  ara::log::Logger const& aralog_{ ara::log::CreateLogger("SIGS", "Signal handling logger.") };

  /// @brief Exit signal set.
  sigset_t exitSignalSet_{};
};

} // namespace signal_handler

namespace lifecycle
{
/// @brief LifeCycle for an Adaptive Application.
/// @param[in] Application type of the adaptive application.
/// @brief *Application* needs to implement.
/// Initialize method Initializes the application. Called once at beginning of life cycle of application.
/// Run method Handles long term running phase of application. Called once after Initialization.
/// RequestShutdown method Triggers return of Run() call. Called asynchronously during run phase of
/// application. Shutdown method Shuts down the application. Called after application returned from Run()
/// call.
template <class Application>
class ApplicationLifeCycle
{
 public:
  /// @brief Main method of the ApplicationLifeCycle.
  /// @param[in] argc Command line argument count.
  /// @param[in] argv Array of pointers to command line arguments.
  /// @returns Return value of the application run.
  static int Main(const std::int32_t argc, char const* argv[])
  {
    signal_handler::SignalHandler signalHandler{};
    signalHandler.BlockExitSignals();

    // provided static cast for unused variable.
    static_cast<void>(argc);
    static_cast<void>(argv);
    // Initialize Logger.
    static_cast<void>(apd::logconfig::Initialize());

    ara::log::Logger& aralog{ ara::log::CreateLogger(
      "CRLC", "Crypto Daemon Life Cycle context", ara::log::LogLevel::kVerbose) };

    // Create the application object.
    Application application;

    // Start application lifecycle.
    aralog.LogDebug() << "Start of Application Life Cycle";

    // Application life cycle worker.
    std::thread appLifeCycleWorker{ &ApplicationLifeCycle::Execute, std::ref(application), std::ref(aralog),
      std::ref(signalHandler) };

    // Wait for exit signals.
    signalHandler.WaitForExitSignals();

    // Request for application to shutdown.
    application.RequestShutdown();

    aralog.LogDebug() << "Wait for Application to Shutdown.";
    // Wait for worker thread to complete its shut down.
    appLifeCycleWorker.join();
    aralog.LogDebug() << "Application Shutdown completed.";

    return 0;
  }

  /// @brief Starts life-cycle of given application.
  /// @param[in] app the application to be started.
  /// @param[in] args command line arguments.
  /// @param[in] signal_handler signal handler.
  static void Execute(
    Application& app, ara::log::Logger& aralog, signal_handler::SignalHandler const& signalHandler)
  {
    // Execute the application lifecycle.

#ifdef USE_ARA
    // Instantiate the ApplicationClient.
    ara::exec::ExecutionClient exec_client;
#endif // USE_ARA
    try
    {
      aralog.LogDebug() << "Initialization of Application Life Cycle";
      // Initialize the application.
      app.Initialize();

      aralog.LogDebug() << "Run(Start) of Application Life Cycle ";
      // Run the application.
      app.Run();

#ifdef USE_ARA
      // Report application state to execution management.
      exec_client.ReportExecutionState(ara::exec::ExecutionState::kRunning);
#endif // USE_ARA
    }
    catch (const std::system_error& sysException)
    {
      std::string const sysExceptionLog{ sysException.what() };
      aralog.LogError() << __func__ << ": system_error exception occurred (check if someipd is running): "
                        << sysExceptionLog.c_str();
    }
    catch (const std::exception& ex)
    {
      std::string const exceptionLog{ ex.what() };
      aralog.LogError() << __func__ << ": exception occurred: " << exceptionLog.c_str();
    }

#ifdef USE_ARA
    // Report application state to execution management.
    exec_client.ReportExecutionState(ara::exec::ExecutionState::kTerminating);
#endif // USE_ARA.

    // Shutdown the application.
    app.Shutdown();

    // Stop signal handler in case the application shutdown was not triggered by signal handling
    // in order to also shut down signal handling.
    signalHandler.StopWaitingForSignals();
  }
};
} // namespace lifecycle

#endif // CRYPTO_SOURCE_CRYPTO_DAEMON_INC_PRIVATE_APPLICATION_LIFECYCLE_H_
