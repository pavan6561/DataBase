// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_context.h
//
// Purpose     : This file contains definitions for CryptoContext abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYPTO_CONTEXT_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYPTO_CONTEXT_H_

#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"

class CryptoProvider;

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief A common interface of a mutable cryptographic context, i.e. that is not binded to a single crypto
/// object.
/// @trace SWS_CRYPT_20400
class CryptoContext
{
 public:
  /// @brief Constructor for CryptoContext class.
  CryptoContext() noexcept = default;

  /// @brief Move constructor for CryptoContext class.
  CryptoContext(CryptoContext&& other) noexcept = default;

  /// @brief Destructor.
  /// @trace SWS_CRYPT_20401
  virtual ~CryptoContext() noexcept = default;

  /// @brief Copy-assign another CryptoContext to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30214
  CryptoContext& operator=(const CryptoContext& other) = default;

  /// @brief Move-assign another CryptoContext to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30215
  CryptoContext& operator=(CryptoContext&& other) = default;

  /// @brief Type definition of vendor specific binary Crypto Primitive ID.
  /// @trace SWS_CRYPT_20402
  using AlgId = CryptoAlgId;

  /// @brief Return CryptoPrimitivId instance containing instance identification.
  /// @trace SWS_CRYPT_20411
  virtual CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept = 0;

  /// @brief Check if the crypto context is already initialized and ready to use.
  /// It checks all required values, including: key value, IV/seed, etc.
  /// @returns @c true if the crypto context is completely initialized and ready to use, and @c false
  /// otherwise.
  /// @trace SWS_CRYPT_20412
  virtual bool IsInitialized() const noexcept = 0;

  /// @brief Get a reference to Crypto Provider of this context.
  /// @returns a reference to Crypto Provider instance that provides this context
  /// @trace SWS_CRYPT_20654
  // virtual CryptoProvider& MyProvider() const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYPTO_CONTEXT_H_
