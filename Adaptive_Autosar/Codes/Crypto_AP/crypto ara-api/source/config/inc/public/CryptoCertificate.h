// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoCertificate.h
//
// Purpose     : This file contains CryptoCertificateConfig structure.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CRYPTOCERTIFICATE_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CRYPTOCERTIFICATE_H_

#include <cstdint>

#include "Certificateslot_MR.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for config.
namespace config
{
/// @brief class for class CryptoCertificateConfig.
class CryptoCertificateConfig
{
 public:
  /// @brief Variable to hold the certificate Name.
  ara::core::String certificateName{ " " };

  /// @brief Variable to hold the certificate Catagory.
  ara::core::String certificateCatagory{ " " };

  /// @brief Variable to hold the certificate InstanceSpecifier.
  ara::core::String certificateInstanceSpceifier{ " " };

  /// @brief flag to indicate if this certificate is private.
  bool isPrivate{ false };

  /// @brief flag to indicate if this certificate is writable.
  bool isWriteAccess{ false };

  /// @brief flag to indicate if this certificate is private.
  ara::core::Vector<CertificateSlotConfig> certSlot;
};

} // namespace config
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CRYPTOCERTIFICATE_H_
