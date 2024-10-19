// --------------------------------------------------------------------------
// |              _    _ _______     .----.      _____         _____        |
// |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
// |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
// |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
// |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
// |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
// |                              . _ _  .                                  |
// --------------------------------------------------------------------------
//
// All Rights Reserved.
// Any use of this source code is subject to a license agreement with the
// AUTOSAR development cooperation.
// More information is available at www.autosar.org.
//
// Disclaimer
//
// This work (specification and/or software implementation) and the material
// contained in it, as released by AUTOSAR, is for the purpose of information
// only. AUTOSAR and the companies that have contributed to it shall not be
// liable for any use of the work.
//
// The material contained in this work is protected by copyright and other
// types of intellectual property rights. The commercial exploitation of the
// material contained in this work requires a license to such intellectual
// property rights.
//
// This work may be utilized or reproduced without any modification, in any
// form or by any means, for informational purposes only. For any other
// purpose, no part of the work may be utilized or reproduced, in any form
// or by any means, without permission in writing from the publisher.
//
// The work has been developed for automotive applications only. It has
// neither been developed, nor tested for non-automotive applications.
//
// The word AUTOSAR and the AUTOSAR logo are registered trademarks.
// --------------------------------------------------------------------------

/// @file
/// @brief Adaptive Autosar Crypto Stack. X.509 API. CertSignRequest interface definition.

#ifndef ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H_
#define ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/x509/basic_cert_info.h"

namespace ara
{
namespace crypto
{
namespace x509
{

//- @interface CertSignRequest
///
/// @traceid{SWS_CRYPT_40300}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02306}
/// @brief Certificate Signing Request (CSR) object interface
///       This interface is dedicated for complete parsing of the request content.
///
 
class CertSignRequest : public BasicCertInfo
{
public:
    /// @traceid{SWS_CRYPT_40301}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Unique smart pointer of the constant interface.
     
    using Uptrc = std::unique_ptr<const CertSignRequest>;

    /// @traceid{SWS_CRYPT_40302}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Unique smart pointer of the interface.
     
    using Uptr = std::unique_ptr<CertSignRequest>;

    /// @traceid{SWS_CRYPT_40311}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Verifies self-signed signature of the certificate request.
    /// @returns @c true if the signature is correct
    /// @threadsafety{Thread-safe}
     
    virtual bool Verify() const noexcept = 0;

//    /// @traceid{SWS_CRYPT_40312}@tracestatus{draft}
//    /// @uptrace{RS_CRYPTO_02306}
//    /// @brief Get the challenge password for this request (if it was included to the request).
//    /// @param[out] password  the optional pointer to an output string
//    /// @returns length of the password if it was provided or 0 otherwise
//    /// @error SecurityErrorDomain::kInsufficientCapacity  if <tt>(password != nullptr)</tt>, but its capacity
//    ///                is less then required for storing the password value
//    /// @threadsafety{Thread-safe}
//     
//    virtual ara::core::Result<std::size_t> ChallengePassword(ara::core::String* password = nullptr
//    ) const noexcept  = 0;

    /*  */
    /// @traceid{SWS_CRYPT_40313}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Export this certificate signing request in DER encoded ASN1 format.
    ///           Note: this is the CSR that can be sent to the CA for obtaining the certificate.
    /// @returns a buffer with the formatted CSR
    /// @error SecurityErrorDomain::kInvalidUsageOrder  this error will be returned in case not all required
    ///                information has been provided
    ///
    /// @threadsafety{Thread-safe}
     
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportASN1CertSignRequest() noexcept  = 0;

    /// @traceid{SWS_CRYPT_40314}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Return format version of the certificate request.
    /// @returns format version of the certificate request
    /// @threadsafety{Thread-safe}
     
    virtual unsigned Version() const noexcept = 0;

    /// @traceid{SWS_CRYPT_40315}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Return signature object of the request.
    /// @returns signature object of the request
    /// @threadsafety{Thread-safe}
     
    virtual const ara::crypto::cryp::Signature& GetSignature() const noexcept = 0;

};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H_
