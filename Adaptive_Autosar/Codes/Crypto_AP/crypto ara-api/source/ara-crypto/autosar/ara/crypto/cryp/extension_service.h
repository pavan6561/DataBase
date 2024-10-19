// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : extension_service.h
//
// Purpose     : This file contains definitions for ExtensionService abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_EXTENSION_SERVICE_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_EXTENSION_SERVICE_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"
#include "ara/crypto/common/security_error_domain.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Basic meta-information service for all contexts.
/// @trace SWS_CRYPT_29040
class ExtensionService
{
 public:
  /// @brief Destructor
  /// @trace SWS_CRYPT_29041
  virtual ~ExtensionService() noexcept = default;

  /// @brief Copy-assign another ExtensionService to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of other
  /// @trace SWS_CRYPT_30218
  ExtensionService& operator=(const ExtensionService& other) = default;

  /// @brief Move-assign another ExtensionService to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of other
  /// @trace SWS_CRYPT_30219
  ExtensionService& operator=(ExtensionService&& other) = default;

  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_29042
  using Uptr = std::unique_ptr<ExtensionService>;

  /// @brief Verify supportness of specific key length by the context.
  /// @param[in] keyBitLength  length of the key in bits
  /// @returns @c true if provided value of the key length is supported by the context
  /// @trace SWS_CRYPT_29048
  virtual bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept = 0;

  /// @brief Get minimal supported key length in bits.
  /// @returns minimal supported length of the key in bits
  /// @trace SWS_CRYPT_29043
  virtual std::size_t GetMinKeyBitLength() const noexcept = 0;

  /// @brief Get maximal supported key length in bits.
  /// @returns maximal supported length of the key in bits
  /// @trace SWS_CRYPT_29044
  virtual std::size_t GetMaxKeyBitLength() const noexcept = 0;

  /// @brief Get actual bit-length of a key loaded to the context. If no key was set to the
  /// context yet then 0 is returned.
  /// @returns actual length of a key (now set to the algorithm context) in bits
  /// @trace SWS_CRYPT_29045
  virtual std::size_t GetActualKeyBitLength() const noexcept = 0;

  /// @brief Get allowed usages of this context (according to the key object attributes loaded to this
  /// context).
  /// If the context is not initialized by a key object yet then zero (all flags are reset) must be returned.
  /// @returns a combination of bit-flags that specifies allowed usages of the context
  /// @trace SWS_CRYPT_29046
  virtual AllowedUsageFlags GetAllowedUsage() const noexcept = 0;

  /// @brief Get the COUID of the key deployed to the context this extension service is attached to.
  /// If no key was set to the context yet then an empty COUID (Nil) is returned.
  /// @returns the COUID of the CryptoObject
  /// @trace SWS_CRYPT_29047
  virtual CryptoObjectUid GetActualKeyCOUID() const noexcept = 0;

  /// @uptrace{RS_CRYPTO_02309}
  /// @brief Check if a key has been set to this context.
  /// @returns FALSE if no key has been set
  /// @trace SWS_CRYPT_29049
  virtual bool IsKeyAvailable() const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_EXTENSION_SERVICE_H_
