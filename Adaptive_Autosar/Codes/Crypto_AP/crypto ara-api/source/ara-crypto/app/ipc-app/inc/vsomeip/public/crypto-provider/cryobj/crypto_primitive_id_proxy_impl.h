// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_primitive_id_proxy_impl.h
//
// Purpose     : Crypto primitive id proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYOBJ_CRYPTO_PRIMITIVE_ID_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYOBJ_CRYPTO_PRIMITIVE_ID_PROXY_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"
#include "impl_type_cryptoprimitiveiduptrstructure.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Common interface for identification of all Crypto Primitives and their keys & parameters.
/// @trace SWS_CRYPT_20600
class CryptoPrimitiveIdProxyImpl : public ara::crypto::cryp::CryptoPrimitiveId
{
 public:
  /// @brief Constructor of CryptoPrimitiveIdProxyImpl class.
  /// @param[in] primitiveName of Crypto primitive name.
  /// @param[in] algorithmId of Crypto algorithm id.
  CryptoPrimitiveIdProxyImpl(const ara::core::StringView primitiveName, const AlgId algorithmId) noexcept;

  /// @brief Destructor used to destruct the object of CryptoPrimitiveIdProxyImpl.
  virtual ~CryptoPrimitiveIdProxyImpl() noexcept = default;

  /// @brief Get a unified name of the primitive.
  /// The crypto primitive name can be fully or partially specified (see "Crypto Primitives Naming
  /// Convention" for more details).
  /// The life-time of the returned @c StringView instance should not exceed the life-time of this
  /// @c CryptoPrimitiveId instance!
  /// @returns the unified name of the crypto primitive
  /// @trace SWS_CRYPT_20651
  const ara::core::StringView GetPrimitiveName() const noexcept override;

  /// @brief Get vendor specific ID of the primitive.
  /// @returns the binary Crypto Primitive ID
  /// @trace SWS_CRYPT_20652
  CryptoPrimitiveId::AlgId GetPrimitiveId() const noexcept override;

 private:
  /// @brief crypto primitive name.
  ara::core::String primitiveName_;

  /// @brief crypto primitive id.
  const AlgId algorithmId_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYOBJ_CRYPTO_PRIMITIVE_ID_PROXY_IMPL_H_
