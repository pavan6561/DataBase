// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : restricted_use_object.h
//
// Purpose     : This file contains definitions for RestrictedUseObject abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_RESTRICTED_USE_OBJECT_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_RESTRICTED_USE_OBJECT_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/cryobj/crypto_object.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief A common interface for all objects supporting the usage restriction.
/// @trace SWS_CRYPT_24800
class RestrictedUseObject : public CryptoObject
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_24802
  using Uptrc = std::unique_ptr<RestrictedUseObject>;

  /// @brief Alias to the container type for bit-flags of allowed usages of the object.
  /// @trace SWS_CRYPT_24801
  using Usage = AllowedUsageFlags;

  /// @brief Get allowed usages of this object.
  /// @returns a combination of bit-flags that specifies allowed applications of the object
  /// @trace SWS_CRYPT_24811
  virtual Usage GetAllowedUsage() const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_RESTRICTED_USE_OBJECT_H_
