// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_core.h
//
// Purpose     : This file contains definitions for CryptoCore class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_ARA_CRYPTO_CORE_INC_PUBLIC_CRYPTO_CORE_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_ARA_CRYPTO_CORE_INC_PUBLIC_CRYPTO_CORE_H_

#include <ara/core/map.h>
#include <ara/log/logging.h>

#include "crypto_config.h"
#include "crypto_provider_skeleton_impl.h"
#include "x509_provider_skeleton_impl.h"
#include "crypto_provider_ssl.h"
#include "x509_provider_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for ara-crypto core
namespace core
{
/// @brief Class for loading crypto daemon configurations and initialization of crypto daemon instances.
class CryptoCore final
{
 public:
  /// @brief Constructor used to construct the object.
  CryptoCore();

  /// @brief Copy Constructor of CryptoCore.
  CryptoCore(CryptoCore const&) = delete;

  /// @brief Move Constructor of CryptoCore.
  CryptoCore(CryptoCore&&) = delete;

  /// @brief Copy Assignment operator of CryptoCore.
  CryptoCore& operator=(CryptoCore const&) = delete;

  /// @brief Move Assignment operator of CryptoCore.
  CryptoCore& operator=(CryptoCore&&) = delete;

  /// @brief Destructor of a class.
  ~CryptoCore() noexcept;

  /// @brief This method loads crypto daemon configuration.
  void Initialize();

  /// @brief This method starts crypto core instances.
  void Run() const;

  /// @brief This method stops crypto core instances.
  void Shutdown() const;

 private:
  /// @brief Method to instantiate the crypto provider.
  void InstantiateCryptoProvider();

  /// @brief Method to instantiate the key storage provider.
  void InstantiateKeyStorageProvider();

  /// @brief Method to instantiate the key certificate provider.
  void InstantiateCertificateManagerProvider();

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "CORE", "CRYPTOCORE Context", ara::log::LogLevel::kVerbose) };

  /// @brief Shared pointer of CryptoConfig.
  std::shared_ptr<ara::crypto::config::CryptoConfig> crypto_config_{};

  /// @brief InstanceId to cryptoprovider mapping.
  ara::core::Map<ara::core::String, std::unique_ptr<ara::crypto::cryp::CryptoProviderSkeletonImpl>>
      cryptoProviderSkeletonObj_{};

  /// @brief InstanceId to x509provider mapping.
  ara::core::Map<ara::core::String, std::unique_ptr<ara::crypto::x509::X509ProviderSkeletonImpl>>
      x509ProviderSkeletonObj_{};
};
} // namespace core
} // namespace crypto
} // namespace ara

#endif //SOURCE_ARA_CRYPTO_CORE_ARA_CRYPTO_CORE_INC_PUBLIC_CRYPTO_CORE_H_
