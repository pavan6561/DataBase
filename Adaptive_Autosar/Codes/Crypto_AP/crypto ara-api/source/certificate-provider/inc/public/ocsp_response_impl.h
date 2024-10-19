// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ocsp_response.h
//
// Purpose     : This file contains X.509 API. OcspResponseImpl interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_OCSP_RESPONSE_H_
#define SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_OCSP_RESPONSE_H_

#include <openssl/ocsp.h>
#include <ara/log/logging.h>

#include "ara/crypto/x509/x509_object.h"
#include "ara/crypto/x509/ocsp_response.h"
#include "ara/crypto/common/security_error_domain.h"

/// @brief namespace for ara
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace x509 Certificate provide.
namespace x509
{
/// @brief On-line Certificate Status Protocol Response.
/// @trace SWS_CRYPT_40800
class OcspResponseImpl : public OcspResponse
{
 public:

  OcspResponseImpl(OCSP_RESPONSE* resp) noexcept : OcspResponse{}, responce_{ resp }, 
  aralog_{ ara::log::CreateLogger( "OCRS", "OCSP Response Impl", ara::log::LogLevel::kVerbose)}
  {}

  /// @brief Get version of the OCSP response format.
  /// @returns OCSP response format version
  /// @trace SWS_CRYPT_40811
  //virtual std::uint32_t Version() const noexcept = 0;

  /// @brief Serialize itself publicly.
  /// @param[in] formatId  the Crypto Provider specific identifier of the output format
  /// @returns a buffer with the serialized object
  /// @error SecurityErrorDomain::kInsufficientCapacity
  /// if <tt>(output.empty() == false)</tt>, but it's capacity is less than required
  /// @error SecurityErrorDomain::kUnknownIdentifier  if an unknown format ID was specified
  /// @error SecurityErrorDomain::kUnsupportedFormat
  /// if the specified format ID is not supported for this object type
  /// @trace SWS_CRYPT_10711
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
    Serializable::FormatId formatId = Serializable::kFormatDefault) const noexcept;

  private:

  /// @brief Variable to store the response pointer.
  OCSP_RESPONSE* responce_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_OCSP_RESPONSE_H_
