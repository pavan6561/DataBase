// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoKeySlotAllowedModification_MR.h
//
// Purpose     : This file contains MR_HEADER for CryptoKeyslotAllowedModification.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTALLOWEDMODIFICATION_MR_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTALLOWEDMODIFICATION_MR_H_

#include "CryptoKeySlotAllowedModification.h"

/// @brief Get all the data from CryptoKeyslotAllowedModification.
MR_HEADER(ara::crypto::config::CryptoKeyslotAllowedModification)
/// @brief Get the allowContentTypeChange.
MR_FIELD("allowContentTypeChange", allowContentTypeChange_)
/// @brief Get the exportability.
MR_FIELD("exportability", exportability_)
/// @brief Get the maxNumberOfAllowedUpdates.
MR_FIELD("maxNumberOfAllowedUpdates", maxNumberOfAllowedUpdates_)
/// @brief Get the restrictUpdate.
MR_FIELD("restrictUpdate", restrictUpdate_)
MR_FOOTER

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTALLOWEDMODIFICATION_MR_H_
