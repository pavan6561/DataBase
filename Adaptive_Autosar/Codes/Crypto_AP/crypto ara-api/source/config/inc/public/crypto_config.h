// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_config.h
//
// Purpose     : This file contains definitions for CryptoConfig class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTO_CONFIG_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTO_CONFIG_H_

#include <apd/manifestreader/manifest_reader.h>
#include <ara/log/logging.h>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "CryptoCertificate_MR.h"
#include "CryptoDefaultConfig_MR.h"
#include "CryptoProviderConfig_MR.h"
#include "algorithm_identification.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for config.
namespace config
{
/// @brief Class for crypto configuration
class CryptoConfig final
{
 public:
  /// @brief constructor for CryptoConfig class.
  CryptoConfig();

  /// @brief Destructor for CryptoConfig class.
  ~CryptoConfig() = default;

  /// @brief Load configuration from given file.
  void LoadConfig(ara::core::StringView fileName) noexcept;

  /// @brief Load certificate configuration from given file.
  void LoadCertConfig(ara::core::StringView fileName) noexcept;

  /// @brief Load default crypto configuration.
  void LoadCryptoDefaultConfigurations() noexcept;

  /// @brief Load configuration for crypto provider.
  void LoadCryptoProviderConfig() noexcept;

  /// @brief Load configuration for cerificate provider.
  void LoadCryptoCertificateConfig() noexcept;

  /// @brief Load configuration for cerificate provider to key slot mapping.
  void LoadCertificateKeyslotMappingConfig() noexcept;

  /// @brief Console all the information on logger.
  void ConsoleInfo() noexcept;

  /// @brief Console all the information on logger.
  void ConsoleCertInfo() noexcept;

  /// @brief Method Get Crypto  config instance
  static std::shared_ptr<CryptoConfig> GetCryptoConfigInstance();

  /// @brief Method to get Crypto provider configuration
  ara::core::Vector<CryptoProviderConfig>& GetCryptoProviders()
  {
    return cryptoProviderStruct_;
  }

  /// @brief Method to get Certificate provider configuration
  ara::core::Vector<CryptoCertificateConfig>& GetCertificateProviders()
  {
    return cryptoCertificateStruct_;
  }

  /// @brief Method to get Max Buffer Size Configuration
  uint64_t GetCryptoMaxBufferSize() const
  {
    return defaultConfig_.maxBufferSizeAllowed;
  }

  /// @brief Method to get RNG Default Seed Configuration.
  ara::core::String GetRngSeedValue() const
  {
    return defaultConfig_.rngSeedValue;
  }

  /// @brief Method to get KDF default iteration Configuration.
  uint32_t GetKdfIterationValue() const
  {
    return defaultConfig_.kdfIterations;
  }

 private:
  /// @brief manifest reader pointer.
  std::unique_ptr<apd::manifestreader::Manifest> manifest_{ nullptr };

  /// @brief manifest reader pointer.
  std::unique_ptr<apd::manifestreader::Manifest> certManifest_{ nullptr };

  /// @brief varible to store crypto provider configurations.
  ara::core::Vector<CryptoProviderConfig> cryptoProviderStruct_{};

  /// @brief varible to store crypto certificate configurations.
  ara::core::Vector<CryptoCertificateConfig> cryptoCertificateStruct_{};

  /// @brief variable to store the Max buffer size allowed by the crypto provider.
  CryptoDefaultConfig defaultConfig_{};

  /// @brief Shared pointer to the crypto config instance.
  static std::shared_ptr<CryptoConfig> stCryptoConfigInstance_;

  /// @brief Logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "CRDE", "CRYPTO daemon context", ara::log::LogLevel::kVerbose) };
};

} // namespace config
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTO_CONFIG_H_
