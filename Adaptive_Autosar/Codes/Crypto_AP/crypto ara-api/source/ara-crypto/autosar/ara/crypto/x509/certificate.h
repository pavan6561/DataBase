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
/// @brief Adaptive Autosar Crypto Stack. X.509 API. Certificate interface definition.

#ifndef ARA_CRYPTO_X509_CERTIFICATE_H_
#define ARA_CRYPTO_X509_CERTIFICATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/x509/basic_cert_info.h"

#include "ara/core/optional.h"
#include "ara/core/result.h"

namespace ara
{
namespace crypto
{
namespace x509
{

//- @interface Certificate
///
/// @traceid{SWS_CRYPT_40200}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02306}
/// @brief X.509 Certificate interface.
///

class Certificate : public BasicCertInfo
{
public:
    /// @traceid{SWS_CRYPT_40201}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Unique smart pointer of the interface.
    
    using Uptr = std::unique_ptr<Certificate>;

    /// @traceid{SWS_CRYPT_40202}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Unique smart pointer of the interface.
    
    using Uptrc = std::unique_ptr<const Certificate>;

    /// @traceid{SWS_CRYPT_40203}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Certificate verification status.
    
    enum class Status : std::uint32_t
    {
    

        /// The certificate is valid
        kValid = 0,
        /// The certificate is invalid
        kInvalid = 1,
        /// Status of the certificate is unknown yet
        kUnknown = 2,
        /// The certificate has correct signature, but the ECU has no a root of trust for this certificate
        kNoTrust = 3,
        /// The certificate has correct signature, but it is already expired (its validity period has ended)
        kExpired = 4,
        /// The certificate has correct signature, but its validity period is not started yet
        kFuture = 5
 
    };
    
    /// @traceid{SWS_CRYPT_40211}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the X.509 version of this certificate object.
    /// @returns X.509 version
    /// @threadsafety{Thread-safe}
    
    virtual std::uint32_t X509Version() const noexcept = 0;

    /// @traceid{SWS_CRYPT_40212}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Check whether this certificate belongs to a root CA.
    /// @returns @c true if the TrustMaster has set this certificate as root
    /// @threadsafety{Thread-safe}
    
    virtual bool IsRoot() const noexcept = 0;

    /// @traceid{SWS_CRYPT_40213}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the issuer certificate DN.
    /// @returns Issuer DN of this certificate
    /// @threadsafety{Thread-safe}
    
    virtual const X509DN& IssuerDn() const = 0;

    /// @traceid{SWS_CRYPT_40214}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the "Not Before" of the certificate.
    /// @returns "Not Before" of the certificate
    /// @threadsafety{Thread-safe}
    
    virtual time_t StartTime() const noexcept = 0;

    /// @traceid{SWS_CRYPT_40215}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the "Not After" of the certificate.
    /// @returns "Not After" of the certificate
    /// @threadsafety{Thread-safe}
    
    virtual time_t EndTime() const noexcept = 0;

    /// @traceid{SWS_CRYPT_40216}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the serial number of this certificate.
    ///       If <tt>(sn.empty() == true)</tt> then this method only returns required size of the output buffer.
    /// @param[out] sn  an optional output buffer for storing the serial number
    /// @returns size of the certificate serial number in bytes
    /// @error SecurityErrorDomain::kInsufficientCapacity  if <tt>(sn.empty() == false)</tt>, but its size
    ///                is not enough for storing the output value
    /// @threadsafety{Thread-safe}
    
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> SerialNumber()
        const noexcept = 0;

    /// @traceid{SWS_CRYPT_40217}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the DER encoded AuthorityKeyIdentifier of this certificate.
    ///       If <tt>(id.empty() == true)</tt> then this method only returns required size of the output buffer.
    /// @param[out] id  the optional output buffer
    /// @returns size of the DER encoded AuthorityKeyIdentifier in bytes
    /// @error SecurityErrorDomain::kInsufficientCapacity  if <tt>(id.empty() == false)</tt>, but its size is
    ///                not enough for storing the output value
    /// @threadsafety{Thread-safe}
    
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> AuthorityKeyId() const noexcept = 0;

    /// @traceid{SWS_CRYPT_40218}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the DER encoded SubjectKeyIdentifier of this certificate.
    ///       If <tt>(id.empty() == true)</tt> then this method only returns required size of the output buffer.
    /// @param[out] id  the optional output buffer
    /// @returns size of the DER encoded SubjectKeyIdentifier in bytes
    /// @error SecurityErrorDomain::kInsufficientCapacity  if <tt>(id.empty() == false)</tt>, but its size is
    ///                not enough for storing the output value
    /// @threadsafety{Thread-safe}
    
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> SubjectKeyId()
        const noexcept = 0;

    /// @traceid{SWS_CRYPT_40219}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Verify signature of the certificate.
    ///       Call with <tt>(caCert == nullptr)</tt> is applicable only if this is a certificate of a root CA.
    /// @param[in] caCert  the optional pointer to a Certification Authority certificate used for signature of
    ///            the current one
    /// @returns @c true if this certificate was verified successfully and @c false otherwise
    /// @threadsafety{Thread-safe}
    
    //virtual bool VerifyMe(ara::core::Optional<const Certificate> caCert) const noexcept = 0;

    /// @traceid{SWS_CRYPT_40220}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Calculate a fingerprint from the whole certificate.
    ///       The produced fingerprint value saved to the output buffer starting from leading bytes of the hash value.
    ///       If the capacity of the output buffer is less than the digest size then the digest will be truncated
    ///       and only leading bytes will be saved.
    ///       If the capacity of the output buffer is higher than the digest size then only leading bytes of the
    ///       buffer will be updated.
    /// @param[out] fingerprint  output buffer for the fingerprint storage
    /// @param[in] hashCtx  an initialized hash function context
    /// @returns number of bytes actually saved to the output buffer
    /// @error SecurityErrorDomain::kIncompleteArgState  if the @c hashCtx context is not initialized
    /// @threadsafety{Thread-safe}
    
    virtual ara::core::Result<std::size_t> GetFingerprint(ReadWriteMemRegion fingerprint
        , cryp::HashFunctionCtx& hashCtx
    ) const noexcept = 0;

    /// @traceid{SWS_CRYPT_40221}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Return last verification status of the certificate.
    /// @returns the certificate verification status
    /// @threadsafety{Thread-safe}
    
    virtual Status GetStatus() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_CERTIFICATE_H_
