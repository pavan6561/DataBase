// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CertificateKeyslotMapping_MR.h
//
// Purpose     : This file contains MR_HEADER for CertificateKeyslotMapping.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CERTIFICATESLOT_MR_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CERTIFICATESLOT_MR_H_

#include "Certificateslot.h"

/// @brief Get all the data from CertificateKeyslotMapping.
MR_HEADER(ara::crypto::config::CertificateSlotConfig)
/// @brief Get the Certificate Id.
MR_FIELD("Name", certificateSlotName)
/// @brief Get the Certificate keys slot Id.
MR_FIELD("KeySlotId", certKeyslotId)
/// @brief Get allocateShadowCopy flag.
MR_FIELD("AllocateShadowCopy", allocateShadowCopy_)
/// @brief Get cryptoAlgId.
MR_FIELD("CryptoAlgId", cryptoAlgId_)
/// @brief Get cryptoObjectType.
MR_FIELD("CryptoObjectType", cryptoObjectType_)
/// @brief Get slotCapacity.
MR_FIELD("SlotCapacity", slotCapacity_)
/// @brief Get slotType.
MR_FIELD("SlotType", slotType_)
/// @brief Get cryptokeyslotallowedmodification.
//MR_OPTIONAL_FIELD("cryptokeyslotallowedmodification", cryptoKeyslotAllowedModification_)
/// @brief Get cryptokeyslotcontentallowedusage.
//MR_OPTIONAL_FIELD("cryptokeyslotcontentallowedusage", cryptokeyslotcontentallowedusage_)
MR_FOOTER

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CERTIFICATESLOT_MR_H_
