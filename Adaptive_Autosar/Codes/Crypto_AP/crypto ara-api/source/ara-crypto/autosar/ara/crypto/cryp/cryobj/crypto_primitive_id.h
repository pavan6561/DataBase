// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_primitive_id.h
//
// Purpose     : This file contains definitions for CryptoPrimitiveId abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_PRIMITIVE_ID_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_PRIMITIVE_ID_H_

#include <memory>

#include "ara/core/string_view.h"
#include "ara/crypto/common/base_id_types.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
// Forward declaration of the Crypto Provider interface.
class CryptoProvider;

/// @brief Common interface for identification of all Crypto Primitives and their keys & parameters.
/// @trace SWS_CRYPT_20600
class CryptoPrimitiveId
{
 public:
  /// @brief Destructor.
  /// @trace SWS_CRYPT_10808
  virtual ~CryptoPrimitiveId() noexcept = default;

  /// @brief Copy-assign another CryptoPrimitiveId to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30212
  CryptoPrimitiveId& operator=(const CryptoPrimitiveId& other) = default;

  /// @brief Move-assign another CryptoPrimitiveId to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30213
  CryptoPrimitiveId& operator=(CryptoPrimitiveId&& other) = default;

  /// @brief Type definition of vendor specific binary Crypto Primitive ID.
  /// @trace SWS_CRYPT_20641
  using AlgId = CryptoAlgId;

  /// @brief type definition pointer to const
  /// @trace SWS_CRYPT_20643
  using Uptr = std::unique_ptr<CryptoPrimitiveId>;

  /// @brief type definition pointer
  /// @trace SWS_CRYPT_20644
  using Uptrc = std::unique_ptr<const CryptoPrimitiveId>;

  /// @brief Get a unified name of the primitive.
  /// The crypto primitive name can be fully or partially specified (see "Crypto Primitives Naming
  /// Convention" for more details).
  /// The life-time of the returned @c StringView instance should not exceed the life-time of this
  /// @c CryptoPrimitiveId instance!
  /// @returns the unified name of the crypto primitive
  /// @trace SWS_CRYPT_20651
  virtual const ara::core::StringView GetPrimitiveName() const noexcept = 0;

  /// @brief Get vendor specific ID of the primitive.
  /// @returns the binary Crypto Primitive ID
  /// @trace SWS_CRYPT_20652
  virtual AlgId GetPrimitiveId() const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_PRIMITIVE_ID_H_
