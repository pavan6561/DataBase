// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoKeyslotContentAllowedUsage_MR.h
//
// Purpose     : This file contains MR_HEADER for CryptoKeyslotContentallowedUsage.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTCONTENTALLOWEDUSAGE_MR_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTCONTENTALLOWEDUSAGE_MR_H_

#include "CryptoKeyslotContentAllowedUsage.h"

/// @brief Get all the data from CryptoKeyslotContentallowedUsage.
MR_HEADER(ara::crypto::config::CryptoKeyslotContentallowedUsage)
/// @brief Get allowedKeyslotUsage.
MR_FIELD("allowedKeyslotUsage", allowedKeyslotUsage_)
MR_FOOTER

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CRYPTOKEYSLOTCONTENTALLOWEDUSAGE_MR_H_
