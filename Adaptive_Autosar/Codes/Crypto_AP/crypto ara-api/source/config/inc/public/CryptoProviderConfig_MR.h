// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoProviderConfig_MR.h
//
// Purpose     : This file contains MR_HEADER for CrytpoProvider.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOPROVIDERCONFIG_MR_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOPROVIDERCONFIG_MR_H_

#include "CryptoProviderConfig.h"

/// @brief Get all the data from CryptoProviderConfig
MR_HEADER(ara::crypto::config::CryptoProviderConfig)
/// @brief Get the name of crypto provider.
MR_FIELD("name", shortName)
/// @brief Get the instance id of crypto provider.
MR_FIELD("instanceid", instanceId)
/// @brief Get the keys slots assigned to the crypto provder.
MR_FIELD("cryptokeyslots", cryptoKeySlot_)
MR_FOOTER

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOPROVIDERCONFIG_MR_H_
