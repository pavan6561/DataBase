// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : CryptoCertificate_MR.h
//
// Purpose     : This file contains MR_HEADER for CryptoCertificate.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CRYPTOCERTIFICATE_MR_H_
#define CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CRYPTOCERTIFICATE_MR_H_

#include "CryptoCertificate.h"

/// @brief Get all the data from CryptoCertificate
MR_HEADER(ara::crypto::config::CryptoCertificateConfig)
/// @brief Get Cerificate Name.
MR_FIELD("Name", certificateName)
/// @brief Get Cerificate Catagory.
MR_FIELD("Category", certificateCatagory)
/// @brief Get Cerificate InstanceSpecifier.
MR_FIELD("InstanceSpecifier", certificateInstanceSpceifier)
/// @brief Get isPrivate flag.
MR_FIELD("IsPrivate", isPrivate)
/// @brief Get isWriteAccess flag.
MR_FIELD("IsWriteAccess", isWriteAccess)
/// @brief Get Certificateslot.
MR_FIELD("CertificateSlot", certSlot)
MR_FOOTER

#endif // CRYPTO_SOURCE_CONFIG_INC_PUBLIC_CONFIG_CRYPTOCERTIFICATE_MR_H_
