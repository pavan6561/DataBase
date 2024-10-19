// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : uuid_impl.h
//
// Purpose     : This file contains definitions related to uuidImpl class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_UTILS_INC_PUBLIC_VOLATILE_TRUSTED_CONTAINER_IMPL_H_
#define CRYPTO_SOURCE_UTILS_INC_PUBLIC_VOLATILE_TRUSTED_CONTAINER_IMPL_H_

#include "ara/crypto/common/volatile_trusted_container.h"
#include "io_interface_volatile.h"

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
class VolatileTrustedContainerImpl : public VolatileTrustedContainer
{
 public:
  /// @brief constructor of VolatileTrustedContainerImpl class.
  VolatileTrustedContainerImpl(size_t capacity) noexcept;

  /// @brief Destructor.
  /// @trace SWS_CRYPT_10851
  ~VolatileTrustedContainerImpl() noexcept = default;

  /// @brief Retrieve the IOInterface used for importing/exporting objects into this container
  /// @returns a reference to the IOInterface of this container
  /// @trace SWS_CRYPT_10853
  IOInterface& GetIOInterface() const noexcept;

 private:
  /// @brief variable to hold the container capacity.
  size_t containerCapacity_{ 0 };
};

} // namespace crypto
} // namespace ara

#endif //CRYPTO_SOURCE_UTILS_INC_PUBLIC_VOLATILE_TRUSTED_CONTAINER_IMPL_H_