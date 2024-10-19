// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoDefaultConfig_MR.h
//
// Purpose     : This file contains MR_HEADER for crypto default config.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTODEFAULTCONFIG_MR_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTODEFAULTCONFIG_MR_H_

#include "CryptoDefaultConfig.h"

/// @brief Get all the data from CryptoProviderConfig
MR_HEADER(ara::crypto::config::CryptoDefaultConfig)
/// @brief Get the maximum buffer size for crypto.
MR_FIELD("maxalloweddatabuffer", maxBufferSizeAllowed)
/// @brief Get the rng seed value for crypto.
MR_FIELD("rngseedvalue", rngSeedValue)
/// @brief Get the kdf iteration value for crypto.
MR_FIELD("defaultkdfIterations", kdfIterations)
MR_FOOTER

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTODEFAULTCONFIG_MR_H_
