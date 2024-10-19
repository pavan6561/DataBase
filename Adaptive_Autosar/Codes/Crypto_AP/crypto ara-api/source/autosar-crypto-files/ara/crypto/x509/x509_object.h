// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ocsp_request.h
//
// Purpose     : This file contains Cryptographic API. CryptoPrimitiveId interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_OBJECT_H_
#define SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_OBJECT_H_

#include "ara/crypto/common/serializable.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace x509
{
// Forward declaration of the X.509 Provider interface.
class X509Provider;

/// @brief Common interface of all objects created by X.509 Provider.
/// @trace SWS_CRYPT_40900
class X509Object : public Serializable
{
 public:
  /// @brief Get a reference to X.509 Provider of this object.
  /// @returns a reference to X.509 Provider instance that provides this object
  /// @trace SWS_CRYPT_40911
  //virtual X509Provider& MyProvider() const noexcept = 0;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // ARA_CRYPTO_X509_X509_OBJECT_H_
