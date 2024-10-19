// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : private_key.h
//
// Purpose     : This file contains definitions for PrivateKey abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_PRIVATE_KEY_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_PRIVATE_KEY_H_

#include <openssl/evp.h>
#include <openssl/pem.h>

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
/// @brief Generalized Asymmetric Private %Key interface.
/// @trace SWS_CRYPT_22500
class PrivateKey : public RestrictedUseObject
{
 public:
  /// @brief PrivateKey Constructor.
  PrivateKey(ReadWriteMemRegion key) : RestrictedUseObject(key)
  {
  }

  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_22501
  using Uptrc = std::unique_ptr<const PrivateKey>;

  /// @brief Static mapping of this interface to specific value of @c CryptoObjectType enumeration.
  /// @trace SWS_CRYPT_22503
  static const CryptoObjectType kObjectType = CryptoObjectType::kPrivateKey;

  /// @brief Get the public key correspondent to this private key.
  /// @returns unique smart pointer to the public key correspondent to this private key
  /// @trace SWS_CRYPT_22511
  virtual ara::core::Result<PublicKey::Uptrc> GetPublicKey() const noexcept = 0;

  /// @brief Getter method for openssl PrivateKey pointer.
  virtual EVP_PKEY* GetPrivateKey() const noexcept = 0;

  /// @brief Getter method for PrivateKey value.
  ReadWriteMemRegion GetPrivateKeyValue() const
  {
    return GetValue();
  }
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_PRIVATE_KEY_H_
