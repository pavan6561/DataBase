// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ocsp_request_impl.cpp
//
// Purpose     : This file provides OcspRequestImpl class definitions.
// -----------------------------------------------------------------------------------------------------------

#include "ocsp_request_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
namespace crypto
{
namespace x509
{

ara::core::Result<ara::core::Vector<ara::core::Byte>> OcspRequestImpl::ExportPublicly(Serializable::FormatId formatId) 
const noexcept
{
  aralog_.LogDebug() << "OcspRequestImpl::ExportPublicly()";

  if((formatId != Serializable::kFormatDefault) && (formatId != Serializable::kFormatRawValueOnly) && 
  (formatId != Serializable::kFormatDerEncoded) && (formatId != Serializable::kFormatPemEncoded))
  {
    // According to SWS_CRYPT_10711 if an unknown format ID was specified
    // then ExportPublicly() should return SecurityErrorDomain::Errc::kUnknownIdentifier.
    aralog_.LogError() << "Unknown format id is received!!!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if ((formatId == Serializable::kFormatDerEncoded) || (formatId == Serializable::kFormatDefault))
  {
    uint8_t* ocspPtr{ nullptr };

    int32_t len{ i2d_OCSP_REQUEST(request_, &ocspPtr) };

    if (len < 0)
    {
      aralog_.LogError() << "Error in DER encoding";
      len = 0;
    }

    ara::core::Vector<uint8_t> tempResult{ ocspPtr, ocspPtr + len };
    ara::core::Vector<ara::core::Byte> const result{ tempResult.begin(), tempResult.end() };

    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(result);
  }
  else
  {
    // According to SWS_CRYPT_10711 if the specified format ID is not supported for this object type
    // then ExportPublicly() should return SecurityErrorDomain::Errc::kUnsupportedFormat.
    aralog_.LogError() << "Unsupported format Id";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kUnsupportedFormat);
  }
}

} // namespace x509
} // namespace crypto
} // namespace ara
