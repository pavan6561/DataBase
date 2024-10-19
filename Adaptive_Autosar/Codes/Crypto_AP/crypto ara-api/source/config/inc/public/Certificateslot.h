// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : Certificateslot.h
//
// Purpose     : This file contains CertificateslotConfig structure.
// ------------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CERTIFICATESLOT_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CERTIFICATESLOT_H_

#include <cstdint>

#include "CryptoKeySlotAllowedModification_MR.h"
#include "CryptoKeyslotContentAllowedUsage_MR.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for config.
namespace config
{
/// @brief class for CertificateKeyslotMappingConfig.
class CertificateSlotConfig
{
 public:
  /// @brief Variable to hold the certificate Name.
  ara::core::String certificateSlotName{ " " };

  /// @brief Variable to hold the Certificate keys slot Id.
  ara::core::String certKeyslotId{ " " };

  /// @brief Variable to store allocateShadowCopy flag.
  bool allocateShadowCopy_{ false };

  /// @brief Variable to store crypto algorithm Id.
  ara::core::String cryptoAlgId_{ " " };

  /// @brief Variable to store crypto object type.
  ara::core::String cryptoObjectType_{ " " };

  /// @brief Variable to store slot type.
  ara::core::String slotType_{ " " };

  /// @brief Variable to store slot capacity.
  uint64_t slotCapacity_{ 0 };

  /// @brief Variable to Crypto Key slot allowed Modification.
  //ara::core::Vector<CryptoKeyslotAllowedModification> cryptoKeyslotAllowedModification_{};

  /// @brief Variable to Crypto Key slot content allowed usage.
  //ara::core::Vector<CryptoKeyslotContentallowedUsage> cryptokeyslotcontentallowedusage_{};
};

} // namespace config
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CERTIFICATESLOT_H_
