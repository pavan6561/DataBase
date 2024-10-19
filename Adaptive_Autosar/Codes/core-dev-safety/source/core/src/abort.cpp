// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : abort.cpp
//
// Purpose     : This file provides the implimentation of ara::core::Abort().
// -----------------------------------------------------------------------------------------------------------
#include "ara/core/abort.h"
#include "ara/log/logger.h"
#include "ara/log/logging.h"

#include <atomic>
#include <mutex>
#include <cstdlib>
#include <cassert>

namespace ara
{
namespace core
{
static std::atomic<AbortHandler> g_abortHandler;
static std::mutex g_abortMutex;
AbortHandler SetAbortHandler(AbortHandler handler) noexcept
{
  assert(g_abortHandler.is_lock_free());

  AbortHandler oldHandler{g_abortHandler.exchange(handler)};
  return oldHandler;
}

void Abort(const char* text) noexcept
// AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
{
  /// @uptrace{SWS_CORE_12407, 8d8980d37b3374d9cf6f075acb4d33e27e3f70e0}
  std::lock_guard<std::mutex> lock{g_abortMutex};

  // The Logger that is being used for this log message output should better be obtained in Initialize(),
  // so that we don't have to perform a potentially expensive and sensitive memory allocation here.
  // For simplicity, we obtain it here for now.
  {
    ara::log::Logger& lg{ara::log::CreateLogger("ABRT", "ara::core Abort", ara::log::LogLevel::kFatal)};
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.

    /// @uptrace{SWS_CORE_12403, 5c062ce2fd015a09b985b95db4263fdc368fe416}
    lg.LogFatal() << "Explicit Operation Abortion:" << text;
  }

  /// @uptrace{SWS_CORE_12404, c8884f9b87907e7434cae7669305f2835f526bcd}
  AbortHandler const handler{g_abortHandler.load()};
  if (handler != nullptr)
  {
    handler();
  }
  /// @uptrace{SWS_CORE_12402, f88afcbb8a55f16a8d71c6a29f62ff04be7c2ccb}
  /// @uptrace{SWS_CORE_12405, b234e43370a81ce97d6f02aa4342bbdb9311f95a}
  /// @uptrace{SWS_CORE_12406, 89d21f1e6fb68f50773051c43e56e92bcd92c12e}
  std::abort();
}

} // namespace core
} // namespace ara
