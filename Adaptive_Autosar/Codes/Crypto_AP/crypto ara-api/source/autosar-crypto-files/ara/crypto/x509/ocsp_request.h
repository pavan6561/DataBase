// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ocsp_request.h
//
// Purpose     : This file contains X.509 API. OcspRequest interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_OCSP_REQUEST_H_
#define SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_OCSP_REQUEST_H_

#include "ara/crypto/x509/x509_object.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
/// @brief On-line Certificate Status Protocol Request.
/// @trace SWS_CRYPT_40700
class OcspRequest : public X509Object
{
 public:
  /// @brief Shared smart pointer of the interface.
  /// @trace SWS_CRYPT_40701
  using Uptr = std::unique_ptr<OcspRequest>;

  /// @brief Shared smart pointer of the interface.
  /// @trace SWS_CRYPT_40702
  using Uptrc = std::unique_ptr<const OcspRequest>;

  /// @brief Get version of the OCSP request format.
  /// @returns OCSP request format version
  /// @trace SWS_CRYPT_40711
  //virtual std::uint32_t Version() const noexcept = 0;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // ARA_CRYPTO_X509_OCSP_REQUEST_H_
