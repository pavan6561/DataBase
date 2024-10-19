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
/// @brief Adaptive Autosar Crypto Stack. X.509 API. BasicCertInfo interface definition.

#ifndef ARA_CRYPTO_X509_BASIC_CERT_INFO_H_
#define ARA_CRYPTO_X509_BASIC_CERT_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/crypto_provider.h"

#include "ara/crypto/x509/x509_public_key_info.h"
#include "ara/crypto/x509/x509_dn.h"

namespace ara
{
namespace crypto
{
namespace x509
{

// -@interface BasicCertInfo
///
/// @traceid{SWS_CRYPT_40100}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02306}
/// @brief Basic Certificate Information interface.
///
 
class BasicCertInfo : public X509Object
{
public:
    /// @traceid{SWS_CRYPT_40101}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief X.509 v3 Key Constraints type definition.
    using KeyConstraints = std::uint32_t;

    /// @name Constants of X.509 v3 Key Constraints
    /// @{

    /// @traceid{SWS_CRYPT_40150}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief No key constraints.
    static const KeyConstraints kConstrNone = 0;

    /// @traceid{SWS_CRYPT_40151}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief The key can be used for digital signature production.
    static const KeyConstraints kConstrDigitalSignature = 0x8000;

    /// @traceid{SWS_CRYPT_40152}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief The key can be used in cases requiring the "non-repudiation" guarantee
    static const KeyConstraints kConstrNonRepudiation = 0x4000;

    /// @traceid{SWS_CRYPT_40153}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief The key can be used for key encipherment.
    static const KeyConstraints kConstrKeyEncipherment = 0x2000;

    /// @traceid{SWS_CRYPT_40154}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief The key can be used for data encipherment.
    static const KeyConstraints kConstrDataEncipherment = 0x1000;

    /// @traceid{SWS_CRYPT_40155}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief The key can be used for a key agreement protocol execution.
    static const KeyConstraints kConstrKeyAgreement = 0x0800;

    /// @traceid{SWS_CRYPT_40156}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief The key can be used for certificates signing.
    static const KeyConstraints kConstrKeyCertSign = 0x0400;

    /// @traceid{SWS_CRYPT_40157}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief The key can be used for Certificates Revokation Lists (CRL) signing.
    static const KeyConstraints kConstrCrlSign = 0x0200;

    /// @traceid{SWS_CRYPT_40158}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief The enciphermet key can be used for enciphering only.
    static const KeyConstraints kConstrEncipherOnly = 0x0100;

    /// @traceid{SWS_CRYPT_40159}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief The enciphermet key can be used for deciphering only.
    static const KeyConstraints kConstrDecipherOnly = 0x0080;

    /// @}  // Constants of X.509 v3 Key Constraints


    /// @traceid{SWS_CRYPT_40111}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Load the subject public key information object to realm of specified crypto provider.    
	/// If <tt>(cryptoProvider == nullptr)</tt> then @c X509PublicKeyInfo object will be loaded in realm of
    ///       the Stack-default Crypto Provider
    /// @param[in] cryptoProvider  unique pointer of a target Crypto Provider, where the public key will be used
    /// @returns constant reference of the subject public key interface
    /// @threadsafety{Thread-safe}
    virtual const X509PublicKeyInfo& SubjectPubKey(
        cryp::CryptoProvider::Uptr cryptoProvider = nullptr) const noexcept = 0;

    /// @traceid{SWS_CRYPT_40112}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the subject DN.
    /// @returns subject DN
    /// @threadsafety{Thread-safe}
    virtual const X509DN& SubjectDn() const noexcept = 0;

    /// @traceid{SWS_CRYPT_40113}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Check whether the CA attribute of X509v3 Basic Constraints is true (i.e. pathlen=0).
    /// @returns @c true if it is a CA request and @c false otherwise
    /// @threadsafety{Thread-safe}
    virtual bool IsCa() const noexcept = 0;

    /// @traceid{SWS_CRYPT_40114}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the constraint on the path length defined in the Basic Constraints extension.
    /// @returns certification path length limit
    /// @threadsafety{Thread-safe}
    virtual std::uint32_t GetPathLimit() const noexcept = 0;

    /// @traceid{SWS_CRYPT_40115}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get the key constraints for the key associated with this PKCS#10 object.
    /// @returns key constraints
    /// @threadsafety{Thread-safe}
    virtual KeyConstraints GetConstraints() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_BASIC_CERT_INFO_H_
