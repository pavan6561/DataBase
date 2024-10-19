// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : initialize.cpp
//
// Purpose     : This file provides the implementation of class Initialize.
// -----------------------------------------------------------------------------------------------------------

#include "apd/manifestreader/manifest_reader.h"
#include "ara/log/logging.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"
#include "ara/core/result.h"
#include "ara/core/initialization.h"
#include "ara/core/initialization_impl.h"

#include <string>
#include <memory>

namespace
{
static constexpr ara::core::StringView MANIFEST_PATH{"./etc/MANIFEST.json"};
static constexpr ara::core::StringView USES_LOGGING_KEY{"executable.uses_logging"};
static constexpr ara::core::StringView LOG_PROCESS_ID_KEY{"logtrace_process_id"};
static constexpr ara::core::StringView LOG_PROCESS_DESC_KEY{"logtrace_process_desc"};
static constexpr ara::core::StringView LOG_MODES_KEY{"logtrace_log_modes"};
static constexpr ara::core::StringView LOG_DEFAULT_LEVEL_KEY{"logtrace_default_log_level"};
static constexpr ara::core::StringView LOG_FILE_PATH_KEY{"logtrace_file_path"};

static constexpr ara::log::LogMode DEFAULT_LOG_MODE{ara::log::LogMode::kConsole};
static constexpr auto DEFAULT_LOG_LEVEL{ara::log::LogLevel::kVerbose};

ara::log::LogMode ConvertStringVectorToLogMode(const ara::core::Vector<ara::core::String>& input)
{
  static ara::core::Map<ara::core::String, ara::log::LogMode> stringToLogMode{
      {"NETWORK", ara::log::LogMode::kRemote}, {"CONSOLE", ara::log::LogMode::kConsole},
      {"FILE", ara::log::LogMode::kFile}};

  ara::log::LogMode mode{DEFAULT_LOG_MODE};

  for (ara::core::String const modeStr : input)
  {
    auto const iter{stringToLogMode.find(modeStr)};
    if (iter != stringToLogMode.end())
    {
      mode = mode | iter->second;
    }
  }

  return mode;
}

ara::log::LogLevel ConvertStringToLogLevel(const ara::core::String& input)
{
  static ara::core::Map<ara::core::String, ara::log::LogLevel> stringToLogLevel{
      {"FATAL", ara::log::LogLevel::kFatal}, {"ERROR", ara::log::LogLevel::kError},
      {"WARN", ara::log::LogLevel::kWarn}, {"INFO", ara::log::LogLevel::kInfo},
      {"DEBUG", ara::log::LogLevel::kDebug}, {"VERBOSE", ara::log::LogLevel::kVerbose}};

  auto const iter{stringToLogLevel.find(input)};
  if (iter != stringToLogLevel.end())
  {
    return iter->second;
  }

  return DEFAULT_LOG_LEVEL;
}

ara::core::Result<void> ConfigureLogging(std::unique_ptr<apd::manifestreader::Manifest> manifest)
{
  auto const idRes{manifest->GetValue<ara::core::String>(LOG_PROCESS_ID_KEY)};
  if (!idRes)
  {
    return ara::core::Result<void>::FromError(idRes.Error());
  }

  auto const descRes{manifest->GetValue<ara::core::String>(LOG_PROCESS_DESC_KEY)};

  if (!descRes)
  {
    return ara::core::Result<void>::FromError(descRes.Error());
  }

  auto const modesRes{manifest->GetArray<ara::core::String>(LOG_MODES_KEY)};
  if (!modesRes)
  {
    return ara::core::Result<void>::FromError(modesRes.Error());
  }

  auto const defaultRes{manifest->GetValue<ara::core::String>(LOG_DEFAULT_LEVEL_KEY)};
  if (!defaultRes)
  {
    return ara::core::Result<void>::FromError(defaultRes.Error());
  }

  auto const pathRes{manifest->GetValue<ara::core::String>(LOG_FILE_PATH_KEY)};
  if (!pathRes)
  {
    return ara::core::Result<void>::FromError(pathRes.Error());
  }

  // Convertation from ara::core::String to std::string block
  std::string const id(idRes.Value().data(), idRes.Value().size());
  std::string const desc(descRes.Value().data(), descRes.Value().size());
  std::string const path(pathRes.Value().data(), pathRes.Value().size());
  ara::log::LogMode const mode{ConvertStringVectorToLogMode(modesRes.Value())};
  ara::log::LogLevel const defaultLevel{ConvertStringToLogLevel(defaultRes.Value())};
  ara::log::InitLogging(id, desc, defaultLevel, mode, path);

  return {};
}
} // namespace

namespace ara
{
namespace core
{
Result<void> Initialize()
{
  return ara::core::internal::Initialize(MANIFEST_PATH);
}

Result<void> Deinitialize()
{
  return Result<void>{};
}

namespace internal
{
Result<void> Initialize(ara::core::StringView path)
{
  auto manifestRes{apd::manifestreader::OpenManifest(path)};
  if (!manifestRes)
  {
    return Result<void>::FromError(manifestRes.Error());
  }

  auto manifest{std::move(manifestRes).Value()};
  auto const usesLogRes{manifest->GetValue<bool>(USES_LOGGING_KEY)};

  if (!usesLogRes)
  {
    return Result<void>::FromError(usesLogRes.Error());
  }

  if (usesLogRes.Value())
  {
    return ConfigureLogging(std::move(manifest));
  }

  return Result<void>{};
}
} // namespace internal

} // namespace core
} // namespace ara
