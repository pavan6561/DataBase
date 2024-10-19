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
/// @brief Adaptive Autosar Crypto Stack. Cryptographic API. SignatureService interface definition.

#ifndef ARA_CRYPTO_CRYP_SIGNATURE_SERVICE_H_
#define ARA_CRYPTO_CRYP_SIGNATURE_SERVICE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/cryp/extension_service.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
//- @interface SignatureService
///
/// @traceid{SWS_CRYPT_29000}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02309}
/// @brief Extension meta-information service for signature contexts.
///
 
class SignatureService: public ExtensionService
{
public:

    /// @traceid{SWS_CRYPT_29001}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Unique smart pointer of the interface.
     
    using Uptr = std::unique_ptr<SignatureService>;

    /// @traceid{SWS_CRYPT_29002}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Get the hash size required by current signature algorithm.
    /// @returns required hash size in bytes
     
    virtual std::size_t GetRequiredHashSize() const noexcept = 0;

    /// @traceid{SWS_CRYPT_29003}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Get an ID of hash algorithm required by current signature algorithm.
    /// @returns required hash algorithm ID or @c kAlgIdAny if the signature algorithm specification does not include
    ///          a concrete hash function
    ///
    /// @threadsafety{Thread-safe}
     
    virtual CryptoPrimitiveId::AlgId GetRequiredHashAlgId() const noexcept = 0;

    /// @traceid{SWS_CRYPT_29004}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Get size of the signature value produced and required by the current algorithm.
    /// @returns size of the signature value in bytes
    ///
    /// @threadsafety{Thread-safe}
     
    virtual std::size_t GetSignatureSize() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara


#endif /* ARA_CRYPTO_CRYP_SIGNATURE_SERVICE_H_ */
