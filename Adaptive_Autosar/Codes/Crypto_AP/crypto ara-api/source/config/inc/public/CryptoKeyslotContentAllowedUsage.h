// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoKeyslotContentAllowedUsage.h
//
// Purpose     : This file contains CryptoKeyslotContentallowedUsage structure.
// ------------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTCONTENTALLOWEDUSAGE_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTCONTENTALLOWEDUSAGE_H_

#include <ara/core/string.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for config.
namespace config
{
/// @brief Class for CryptoKeyslotContentallowedUsage.
class CryptoKeyslotContentallowedUsage
{
 public:
  /// @brief variable to store allowedKeyslotUsage_.
  ara::core::String allowedKeyslotUsage_{ " " };
};

} // namespace config
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTCONTENTALLOWEDUSAGE_H_
