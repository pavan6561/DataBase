// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoKeySlotAllowedModification.h
//
// Purpose     : This file contains CryptoKeyslotAllowedModification structure.
// ------------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTALLOWEDMODIFICATION_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTALLOWEDMODIFICATION_H_

#include <cstdint>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for config.
namespace config
{
/// @brief class for CryptoKeyslotAllowedModification
class CryptoKeyslotAllowedModification
{
 public:
  /// @brief variable to store the max number of updates.
  uint64_t maxNumberOfAllowedUpdates_{ 0 };

  /// @brief flag to indicate if content type change is allowed.
  bool allowContentTypeChange_{ false };

  /// @brief flag to indicate if key slot is exprotable.
  bool exportability_{ false };

  /// @brief flag to indicate if key slot update is restricted.
  bool restrictUpdate_{ false };
};

} // namespace config
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTALLOWEDMODIFICATION_H_
