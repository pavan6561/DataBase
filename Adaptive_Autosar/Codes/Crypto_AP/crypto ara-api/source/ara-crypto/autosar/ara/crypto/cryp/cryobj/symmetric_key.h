// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_key.h
//
// Purpose     : This file contains definitions for SymmetricKey abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Symmetric Key interface.
/// @trace SWS_CRYPT_23800
class SymmetricKey : public RestrictedUseObject
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_23801
  using Uptrc = std::unique_ptr<const SymmetricKey>;

  /// @brief const object type
  /// @trace SWS_CRYPT_23802
  static const CryptoObjectType kObjectType = CryptoObjectType::kSymmetricKey;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_H_
