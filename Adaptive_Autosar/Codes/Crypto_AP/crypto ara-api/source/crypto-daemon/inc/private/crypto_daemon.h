// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_daemon.h
//
// Purpose     : This file provides CryptoDaemon class definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_DAEMON_INC_PRIVATE_CRYPTO_DAEMON_H_
#define CRYPTO_SOURCE_CRYPTO_DAEMON_INC_PRIVATE_CRYPTO_DAEMON_H_

#include <condition_variable>

#include <ara/log/logging.h>
#include <atomic>
#include <mutex>

#include "crypto_config.h"
#include "crypto_core.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief Implementation CryptoDaemon.
class CryptoDaemon final
{
 public:
  /// @brief Constructor of the CryptoDaemon.
  CryptoDaemon();

  /// @brief Destructor of the CryptoDaemon.
  ~CryptoDaemon() noexcept;

  /// @brief CryptoDaemon shall not be copy constructible.
  /// @param[in] other Object deletion to copy-construct from.
  CryptoDaemon(const CryptoDaemon&) = delete;

  /// @brief CryptoDaemon shall not be move constructible.
  /// @param[in] other Object deletion to move-construct from.
  CryptoDaemon(const CryptoDaemon&&) = delete;

  /// @brief CryptoDaemon shall not be copy assignable.
  /// @param[in] other Object deletion to assignable.
  CryptoDaemon operator=(const CryptoDaemon&) = delete;

  /// @brief CryptoDaemon shall not be move assignable.
  /// @param[in] other Object deletion to assignable.
  CryptoDaemon operator=(const CryptoDaemon&&) = delete;

  /// @brief Method to initialize the CryptoDaemon.
  void Initialize();

  /// @brief Method to run the CryptoDaemon.
  void Run();

  /// @brief Method to request shutdown of the CryptoDaemon.
  void RequestShutdown() noexcept;

  /// @brief Method to shutdown the CryptoDaemon.
  void Shutdown();

 private:
  /// @brief Variable to hold the instance of the CryptoCore object
  ara::crypto::core::CryptoCore cryptoCore_{};

  /// @brief Variable to store the flag which indicates whether exit is requested or not.
  std::atomic_bool isExitRequested_{ false };

  /// @brief Variable to store the flag which indicates whether initialization is done or not.
  std::atomic_bool isInitializationDone_{ false };

  /// @brief Mutex used for conditional variable.
  std::mutex shutdownMutex_{};

  /// @brief Conditional variable.
  std::condition_variable conditionalVariable_{};

  /// @brief Logging instance reference.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "CRDE", "CRYPTO daemon context", ara::log::LogLevel::kVerbose) };
};

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_DAEMON_INC_PRIVATE_CRYPTO_DAEMON_H_
