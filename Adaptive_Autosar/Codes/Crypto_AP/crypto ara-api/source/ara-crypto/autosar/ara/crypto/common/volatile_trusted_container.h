// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : volatile_trusted_container.h
//
// Purpose     : This file contains VolatileTrustedContainer interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_VOLATILE_TRUSTED_CONTAINER_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_VOLATILE_TRUSTED_CONTAINER_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/io_interface.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief This explicit interface of a volatile Trusted Container is used for buffering CryptoAPI objects in
/// RAM. This class represents a "smart buffer" in that it provides access to the IOInterface, which can be
/// used for querying meta-data of the buffer content.
/// @trace SWS_CRYPT_10850
class VolatileTrustedContainer
{
 public:
  /// @brief Destructor.
  /// @trace SWS_CRYPT_10851
  virtual ~VolatileTrustedContainer() noexcept = default;

  /// @brief Copy-assign another VolatileTrustedContainer to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30206
  VolatileTrustedContainer& operator=(const VolatileTrustedContainer& other) = default;

  /// @brief Move-assign another VolatileTrustedContainer to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30207
  VolatileTrustedContainer& operator=(VolatileTrustedContainer&& other) = default;

  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_10852
  using Uptr = std::unique_ptr<VolatileTrustedContainer>;

  /// @brief Retrieve the IOInterface used for importing/exporting objects into this container
  /// @returns a reference to the IOInterface of this container
  /// @trace SWS_CRYPT_10853
  virtual IOInterface& GetIOInterface() const noexcept = 0;
};

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_VOLATILE_TRUSTED_CONTAINER_H_
