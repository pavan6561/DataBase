// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoDefaultConfig.h
//
// Purpose     : This file contains CryptoDefaultConfig structure.
// ------------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTODEFAULTCONFIG_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTODEFAULTCONFIG_H_

#include "CryptoDefaultConfig_MR.h"

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
class CryptoDefaultConfig
{
 public:
  /// @brief Variable to hold the crypto provider max buffer value.
  uint64_t maxBufferSizeAllowed{ 0 };

  /// @brief Variable to hold the crypto provider rng seed value.
  ara::core::String rngSeedValue{ " " };

  /// @brief Variable to hold the kdf iteraiton value.
  uint32_t kdfIterations{ 0 };
};

} // namespace config
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTODEFAULTCONFIG_H_
