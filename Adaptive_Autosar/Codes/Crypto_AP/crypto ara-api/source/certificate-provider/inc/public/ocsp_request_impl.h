// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ocsp_request_impl.h
//
// Purpose     : This file contains X.509 API. OcspRequestImpl interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CRYPTO_X509_OCSP_REQUEST_IMPL_H_
#define SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CRYPTO_X509_OCSP_REQUEST_IMPL_H_

#include <openssl/ocsp.h>
#include <ara/log/logging.h>

#include "ara/crypto/x509/ocsp_request.h"
#include "ara/crypto/common/security_error_domain.h"


namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
/// @brief On-line Certificate Status Protocol Request.
/// @trace SWS_CRYPT_40700
class OcspRequestImpl : public OcspRequest
{
 public:

  /// @brief Constructor for OcspRequestImpl class.
  OcspRequestImpl(OCSP_REQUEST *req) noexcept : OcspRequest{}, request_{ req }, aralog_{ 
  ara::log::CreateLogger("OCPR", "OCSP Request Impl", ara::log::LogLevel::kVerbose)}
  {}

  /// @brief Get version of the OCSP request format.
  /// @returns OCSP request format version
  /// @trace SWS_CRYPT_40711
  //std::uint32_t Version() const noexcept;

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

  /// @brief Variable to store the request pointer.
  OCSP_REQUEST* request_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CRYPTO_X509_OCSP_REQUEST_IMPL_H_
