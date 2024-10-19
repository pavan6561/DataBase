// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_primitive_id_impl.h
//
// Purpose     : This file contains definitions for CryptoPrimitiveIdImpl class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYPTO_PRIMITIVE_ID_IMPL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYPTO_PRIMITIVE_ID_IMPL_H_

#include <ara/core/string_view.h>
#include <memory>

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
class CryptoPrimitiveIdImpl : public CryptoPrimitiveId
{
 public:
  /// @brief Constructor for CryptoPrimitiveIdImpl class.
  CryptoPrimitiveIdImpl(ara::core::String primitiveNameVal, AlgId primitiveIdVal) noexcept :
    CryptoPrimitiveId{}, primitiveName{ primitiveNameVal }, primitiveId{ primitiveIdVal }
  {
  }

  /// @brief Move constructor for CryptoPrimitiveIdImpl class.
  CryptoPrimitiveIdImpl(CryptoPrimitiveIdImpl&& otehr) noexcept = default;

  /// @brief Destructor.
  /// @trace SWS_CRYPT_10808
  ~CryptoPrimitiveIdImpl() noexcept override = default;

  /// @brief Get a unified name of the primitive.
  /// The crypto primitive name can be fully or partially specified (see "Crypto Primitives Naming
  /// Convention" for more details).
  /// The life-time of the returned @c StringView instance should not exceed the life-time of this
  /// @c CryptoPrimitiveId instance!
  /// @returns the unified name of the crypto primitive
  /// @trace SWS_CRYPT_20651
  ara::core::StringView const GetPrimitiveName() const noexcept override
  {
    return primitiveName;
  }

  /// @brief Get vendor specific ID of the primitive.
  /// @returns the binary Crypto Primitive ID
  /// @trace SWS_CRYPT_20652
  AlgId GetPrimitiveId() const noexcept override
  {
    return primitiveId;
  }

  /// @brief Copy-assign another CryptoPrimitiveIdImpl to this instance.
  CryptoPrimitiveIdImpl& operator=(CryptoPrimitiveIdImpl& other) & noexcept = default;

  /// @brief Move-assign another CryptoPrimitiveIdImpl to this instance.
  CryptoPrimitiveIdImpl& operator=(CryptoPrimitiveIdImpl&& other) & noexcept = default;

 private:
  /// @brief variable to store the primitivename.
  ara::core::String primitiveName;

  /// @brief variable to store the primitiId.
  AlgId primitiveId;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYPTO_PRIMITIVE_ID_IMPL_H_
