// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_daemon.cpp
//
// Purpose     : This file provides functionality for crypto daemon.
// -----------------------------------------------------------------------------------------------------------
#include "crypto_daemon.h"

#include <chrono>

#include <thread>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
CryptoDaemon::CryptoDaemon()
{
  // Constructor of the CryptoDaemon.
  aralog_.LogDebug() << __func__;
}

CryptoDaemon::~CryptoDaemon() noexcept
{
  // Destructor of the CryptoDaemon.
  aralog_.LogDebug() << __func__;
  Shutdown();
}

void CryptoDaemon::Initialize()
{
  // Method to initialize the CryptoDaemon.
  aralog_.LogDebug() << __func__;
  // Initialize crypto core for crypto daemon.
  cryptoCore_.Initialize();

  isInitializationDone_ = true;

  if (true == isInitializationDone_)
  {
    // TODO : Handle Multiple Server
  }
}

void CryptoDaemon::Run()
{
  // Method to run the CryptoDaemon.
  aralog_.LogDebug() << __func__;
  cryptoCore_.Run();
  if (true == isInitializationDone_)
  {
    std::unique_lock<std::mutex> lck{ shutdownMutex_ };

    while (isExitRequested_ == false)
    {
      conditionalVariable_.wait(lck);
    }
  }
}

void CryptoDaemon::RequestShutdown() noexcept
{
  // Method to request shutdown of the CryptoDaemon.
  const std::unique_lock<std::mutex> lck{ shutdownMutex_ };
  aralog_.LogDebug() << __func__;
  isExitRequested_ = true;
  conditionalVariable_.notify_all();
}

void CryptoDaemon::Shutdown()
{
  // Method to shutdown the CryptoDaemon.
  aralog_.LogDebug() << __func__;
  cryptoCore_.Shutdown();
  isExitRequested_ = true;
}
} // namespace crypto
} // namespace ara
