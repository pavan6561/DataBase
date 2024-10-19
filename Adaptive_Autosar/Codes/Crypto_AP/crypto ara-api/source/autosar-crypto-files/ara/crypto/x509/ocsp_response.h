// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ocsp_response.h
//
// Purpose     : This file contains X.509 API. OcspResponse interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_OCSP_RESPONSE_H_
#define SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_OCSP_RESPONSE_H_

#include "ara/crypto/x509/x509_object.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
/// @brief On-line Certificate Status Protocol (OCSP) Response Status.
/// @trace SWS_CRYPT_40001
enum class OcspResponseStatus : std::uint32_t
{

  // Response has valid confirmations
  kSuccessful = 0,
  // Illegal confirmation request
  kMalformedRequest = 1,
  // Internal error in issuer
  kInternalError = 2,
  // Try again later
  kTryLater = 3,
  // (4)  is not used
  // Must sign the request
  kSigRequired = 5,
  // Request unauthorized
  kUnauthorized = 6
};

/// @brief On-line Certificate Status Protocol (OCSP) Certificate Status.
/// @trace SWS_CRYPT_40002
enum class OcspCertStatus : std::uint32_t
{
  // The certificate is not revoked
  kGood = 0,
  // The certificate has been revoked (either permanantly or temporarily (on hold))
  kRevoked = 1,
  // The responder doesn't know about the certificate being requested
  kUnknown = 2
};

/// @brief On-line Certificate Status Protocol Response.
/// @trace SWS_CRYPT_40800
class OcspResponse : public X509Object
{
 public:
  /// @brief Shared smart pointer of the interface.
  /// @trace SWS_CRYPT_40801
  using Uptr = std::unique_ptr<OcspResponse>;

  /// @brief Shared smart pointer of the interface.
  /// @trace SWS_CRYPT_40802
  using Uptrc = std::unique_ptr<const OcspResponse>;

  /// @brief Get version of the OCSP response format.
  /// @returns OCSP response format version
  /// @trace SWS_CRYPT_40811
  //virtual std::uint32_t Version() const noexcept = 0;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_OCSP_RESPONSE_H_
