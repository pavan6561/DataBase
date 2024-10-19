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
/// @brief Adaptive Autosar Crypto Stack. X.509 API. X509Provider interface definition.

#ifndef ARA_CRYPTO_X509_OCSP_REQUEST_H_
#define ARA_CRYPTO_X509_OCSP_REQUEST_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/x509/x509_object.h"

namespace ara
{
namespace crypto
{
namespace x509
{

//- @interface OcspRequest
///
/// @traceid{SWS_CRYPT_40700}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02306}
/// @brief On-line Certificate Status Protocol Request.
///
 
class OcspRequest : public X509Object
{
public:
    /// @traceid{SWS_CRYPT_40701}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Shared smart pointer of the interface.
     
    using Uptr = std::unique_ptr<OcspRequest>;

    /// @traceid{SWS_CRYPT_40702}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Shared smart pointer of the interface.
     
    using Uptrc = std::unique_ptr<const OcspRequest>;

    /// @traceid{SWS_CRYPT_40711}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get version of the OCSP request format.
    /// @returns OCSP request format version
    /// @threadsafety{Thread-safe}
     
    virtual std::uint32_t Version() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_OCSP_REQUEST_H_
