// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : mem_region.h
//
// Purpose     : This file contains definitions for memory regions.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_MEM_REGION_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_MEM_REGION_H_

#include <ara/core/span.h>
#include <ara/core/utility.h>
#include <ara/core/vector.h>
#include <functional>
#include <memory>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief Read-Write Memory Region (intended for [in/out] arguments)
/// @trace SWS_CRYPT_10031
using ReadWriteMemRegion = ara::core::Span<std::uint8_t>;

/// @brief Read-Only Memory Region (intended for [in] arguments)
/// @trace SWS_CRYPT_10033
using ReadOnlyMemRegion = ara::core::Span<const std::uint8_t>;

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_MEM_REGION_H_
