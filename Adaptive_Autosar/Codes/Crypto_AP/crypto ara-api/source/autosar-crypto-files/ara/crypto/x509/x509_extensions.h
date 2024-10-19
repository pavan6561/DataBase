// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_extensions.h
//
// Purpose     : This file contains X.509 API. X509Extensions interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_EXTENSIONS_H_
#define SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_EXTENSIONS_H_

#include "ara/crypto/x509/x509_object.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
/// @brief Interface of X.509 Extensions.
/// @trace SWS_CRYPT_40500
class X509Extensions : public X509Object
{
 public:
  /// @brief Shared smart pointer of the interface.
  /// @trace SWS_CRYPT_40501
  using Uptr = std::unique_ptr<X509Extensions>;

  /// @brief Count number of elements in the sequence.
  /// @returns number of elements in the sequence
  /// @trace SWS_CRYPT_40511
  virtual std::size_t Count() const noexcept = 0;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_EXTENSIONS_H_
