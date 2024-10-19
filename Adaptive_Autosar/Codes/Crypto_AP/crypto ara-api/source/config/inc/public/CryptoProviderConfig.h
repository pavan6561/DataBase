// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoProviderConfig.h
//
// Purpose     : This file contains CryptoProviderConfig structure.
// ------------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOPROVIDERCONFIG_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOPROVIDERCONFIG_H_

#include "CryptoKeySlot_MR.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for config.
namespace config
{
/// @brief Class for CryptoProviderConfig
class CryptoProviderConfig final
{
 public:
  /// @brief Constructor for CryptoProviderConfig class.
  CryptoProviderConfig() noexcept
  {
  }

  /// @brief Copy constructor for CryptoProviderConfig class.
  CryptoProviderConfig(CryptoProviderConfig const& other) = default;

  /// @brief Move constructor for CryptoProviderConfig class.
  CryptoProviderConfig(CryptoProviderConfig&& other) noexcept = default;

  /// @brief Destructor for CryptoProviderConfig class.
  ~CryptoProviderConfig()
  {
  }

  /// @brief Copy-assign another CryptoProviderConfig to this instance.
  CryptoProviderConfig& operator=(CryptoProviderConfig& other) & noexcept = default;

  /// @brief Move-assign another CryptoProviderConfig to this instance.
  CryptoProviderConfig& operator=(CryptoProviderConfig&& other) & noexcept = default;

  /// @brief Variable to hold the crypto provider name.
  ara::core::String shortName{ " " };

  /// @brief Variable to hold the crypto provider instance id.
  ara::core::String instanceId{ " " };

  /// @brief Variable to hold the key slots assigned to crypto provider.
  ara::core::Vector<CryptoKeyslots> cryptoKeySlot_;
};

} // namespace config
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOPROVIDERCONFIG_H_
