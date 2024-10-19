// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : elementary_types.h
//
// Purpose     : This file contains Elementary types definitions.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_ELEMENTARY_TYPES_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_ELEMENTARY_TYPES_H_

#include "ara/core/vector.h"
#include "ara/crypto/keys/keyslot.h"

namespace ara
{
namespace crypto
{
namespace keys
{
/// @brief Definition of a transaction identifier type.
/// The zero value should be reserved for especial cases.
/// @trace SWS_CRYPT_30010
using TransactionId = std::uint64_t;

/// @brief Definition of a "transaction scope" type.
/// The "transaction scope" defines a list of key slots that are target for update in a transaction.
/// @trace SWS_CRYPT_30011
using TransactionScope = ara::core::Vector<KeySlot>;

} // namespace keys
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_KEYS_ELEMENTARY_TYPES_H_
