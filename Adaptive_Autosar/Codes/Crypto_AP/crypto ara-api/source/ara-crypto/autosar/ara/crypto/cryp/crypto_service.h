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
/// @brief Adaptive Autosar Crypto Stack. Cryptographic API. CryptoService interface definition.

#ifndef ARA_CRYPTO_CRYP_CRYPTO_SERVICE_H_
#define ARA_CRYPTO_CRYP_CRYPTO_SERVICE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
	
//- @interface CryptoService
///
/// @traceid{SWS_CRYPT_29020}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02309}
/// @brief Extension meta-information service for cryptographic contexts.
///
 
class CryptoService: public ExtensionService
{
public:

    /// @traceid{SWS_CRYPT_29024}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Unique smart pointer of the interface.
     
    using Uptr = std::unique_ptr<CryptoService>;

    /// @traceid{SWS_CRYPT_29021}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Get maximum expected size of the input data block.
    ///   @c suppressPadding argument and it will be equal to the block size.
    /// @param[in] suppressPadding  if @c true then the method calculates the size for the case when the whole space of
    ///            the plain data block is used for the payload only
    /// @returns maximum size of the input data block in bytes
    ///
    /// @threadsafety{Thread-safe}
     
    virtual std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept = 0;

    /// @traceid{SWS_CRYPT_29022}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Get maximum possible size of the output data block.
	///  If <tt>(IsEncryption() == true)</tt> then a value returned by this method is independent from the
    ///       @c suppressPadding argument and will be equal to the block size.
    /// @param[in] suppressPadding  if @c true then the method calculates the size for the case when the whole space of
    ///            the plain data block is used for the payload only
    /// @returns maximum size of the output data block in bytes

    ///
    /// @threadsafety{Thread-safe}
     
    virtual std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept = 0;

    /// @traceid{SWS_CRYPT_29023}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Get block (or internal buffer) size of the base algorithm.
	///       For digest, byte-wise stream cipher and RNG contexts it is an informative method,
    ///       intended only for optimization of the interface usage.
    /// @returns size of the block in bytes
    ///
    /// @threadsafety{Thread-safe}
     
    virtual std::size_t GetBlockSize() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara



#endif /* ARA_CRYPTO_CRYP_CRYPTO_SERVICE_H_ */
