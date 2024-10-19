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
/// @brief Adaptive Autosar Crypto Stack. X.509 API. X509Extensions interface definition.

#ifndef ARA_CRYPTO_X509_X509_EXTENSIONS_H_
#define ARA_CRYPTO_X509_X509_EXTENSIONS_H_

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

//- @interface X509Extensions
///
/// @traceid{SWS_CRYPT_40500}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02306}
/// @brief Interface of X.509 Extensions.
///
 
class X509Extensions : public X509Object
{
public:
    /// @traceid{SWS_CRYPT_40501}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Shared smart pointer of the interface.
     
    using Uptr = std::unique_ptr<X509Extensions>;

    /// @traceid{SWS_CRYPT_40511}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Count number of elements in the sequence.
    /// @returns number of elements in the sequence
    /// @threadsafety{Thread-safe}
     
    virtual std::size_t Count() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_X509_EXTENSIONS_H_
