// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoKeySlot_MR.h
//
// Purpose     : This file contains MR_HEADER for CryptoKeyslots.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOT_MR_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOT_MR_H_

#include "CryptoKeySlot.h"

/// @brief Get all the data from CryptoKeyslots
MR_HEADER(ara::crypto::config::CryptoKeyslots)
/// @brief Get the uuid.
MR_FIELD("uuid", uuid_)
/// @brief Get allocateShadowCopy flag.
MR_FIELD("allocateShadowCopy", allocateShadowCopy_)
/// @brief Get cryptoAlgId.
MR_FIELD("cryptoAlgId", cryptoAlgId_)
/// @brief Get cryptoObjectType.
MR_FIELD("cryptoObjectType", cryptoObjectType_)
/// @brief Get slotCapacity.
MR_FIELD("slotCapacity", slotCapacity_)
/// @brief Get slotType.
MR_FIELD("slotType", slotType_)
/// @brief Get cryptokeyslotallowedmodification.
MR_OPTIONAL_FIELD("cryptokeyslotallowedmodification", cryptoKeyslotAllowedModification_)
/// @brief Get cryptokeyslotcontentallowedusage.
MR_OPTIONAL_FIELD("cryptokeyslotcontentallowedusage", cryptokeyslotcontentallowedusage_)
MR_FOOTER

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOT_MR_H_
