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
/// @brief Adaptive Autosar Crypto Stack. Cryptographic API. CryptoPrimitiveId interface definition.

#ifndef ARA_CRYPTO_X509_X509_OBJECT_H_
#define ARA_CRYPTO_X509_X509_OBJECT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/serializable.h"

namespace ara
{
namespace crypto
{
namespace x509
{

    // Forward declaration of the X.509 Provider interface.
    class X509Provider;

    //- @interface X509Object
    ///
    /// @traceid{SWS_CRYPT_40900}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Common interface of all objects created by X.509 Provider.
    ///
     
    class X509Object : public Serializable
    {
    public:
        /// @traceid{SWS_CRYPT_40911}@tracestatus{draft}
        /// @uptrace{RS_CRYPTO_02401}
        /// @brief Get a reference to X.509 Provider of this object.
        /// @returns a reference to X.509 Provider instance that provides this object
        /// @threadsafety{Thread-safe}
        virtual X509Provider& MyProvider() const noexcept = 0;
    };

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_X509_OBJECT_H_
