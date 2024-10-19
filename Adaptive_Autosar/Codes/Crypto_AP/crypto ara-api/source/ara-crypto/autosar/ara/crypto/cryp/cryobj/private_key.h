// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : private_key.h
//
// Purpose     : This file contains definitions for Private Key abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_PRIVATE_KEY_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_PRIVATE_KEY_H_

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
//- @interface PrivateKey
///
/// @traceid{SWS_CRYPT_22500}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02002}
/// @uptrace{RS_CRYPTO_02403}
/// @brief Generalized Asymmetric Private %Key interface.
///

class PrivateKey : public RestrictedUseObject
{
 public:
  /// @traceid{SWS_CRYPT_22501}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02306}
  /// @brief Unique smart pointer of the interface.

  using Uptrc = std::unique_ptr<const PrivateKey>;

  /// @traceid{SWS_CRYPT_22503}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02306}
  /// @brief Static mapping of this interface to specific value of @c CryptoObjectType enumeration.

  static const CryptoObjectType kObjectType = CryptoObjectType::kPrivateKey;

  /// @traceid{SWS_CRYPT_22511}@tracestatus{draft}
  /// @uptrace{RS_CRYPTO_02108}
  /// @uptrace{RS_CRYPTO_02115}
  /// @brief Get the public key correspondent to this private key.
  /// @returns unique smart pointer to the public key correspondent to this private key
  /// @threadsafety{Thread-safe}

  virtual ara::core::Result<PublicKey::Uptrc> GetPublicKey() const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_PRIVATE_KEY_H_
