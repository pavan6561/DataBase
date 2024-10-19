// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_key.h
//
// Purpose     : This file contains definitions for SymmetricKey abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_H_

#include <ara/log/logging.h>

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
  /// @brief constructor for SymmetricKey class.
  SymmetricKey(ReadWriteMemRegion key) noexcept : RestrictedUseObject{ key }
  {
    aralog_.LogDebug() << "In SymmetricKey constructor";
  }

  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_23801
  using Uptrc = std::unique_ptr<const SymmetricKey>;

  /// @brief const object type
  /// @trace SWS_CRYPT_23802
  static const CryptoObjectType kObjectType = CryptoObjectType::kSymmetricKey;

  /// @brief Getter method for SymmetricKey value.
  virtual ReadWriteMemRegion GetSymmetricKey() const noexcept
  {
    aralog_.LogDebug() << "In SymmetricKey::GetSymmetricKey()";
    return GetValue();
  }

 private:
  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "SKCO", "Symmetric Key Crypto Object", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_H_
