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
/// @brief Adaptive Autosar Crypto Stack. Cryptographic API. X509PublicKeyInfo interface definition.

#ifndef ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H_
#define ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/serializable.h"

#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"
#include "ara/crypto/cryp/cryobj/public_key.h"

#include "ara/core/result.h"

namespace ara
{
namespace crypto
{
namespace x509
{

//- @interface X509PublicKeyInfo
///
/// @traceid{SWS_CRYPT_24400}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02307}
/// @brief X.509 Public Key Information interface.
 
class X509PublicKeyInfo : public Serializable
{

public:
    /// @traceid{SWS_CRYPT_24401}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02307}
    /// @brief Unique smart pointer of the interface.
     
    using Uptrc = std::unique_ptr<const X509PublicKeyInfo>;

	/// @traceid{SWS_CRYPT_24410}@tracestatus{draft}
	/// @uptrace{RS_CRYPTO_02307}
	/// @brief Get the CryptoPrimitiveId instance of this class.
	///
	 
    virtual ara::crypto::cryp::CryptoPrimitiveId::Uptrc GetAlgorithmId() = 0;
	
	/// @traceid{SWS_CRYPT_24411}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Get the hash size required by current signature algorithm.
    /// @returns required hash size in bytes
     
    virtual std::size_t GetRequiredHashSize() const noexcept = 0;

    /// @traceid{SWS_CRYPT_24412}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Get an ID of hash algorithm required by current signature algorithm.
    /// @returns required hash algorithm ID or @c kAlgIdAny if the signature algorithm specification does not include
    ///          a concrete hash function
    /// @threadsafety{Thread-safe}
     
    virtual CryptoAlgId GetRequiredHashAlgId() const noexcept = 0;

    /// @traceid{SWS_CRYPT_24413}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02309}
    /// @brief Get size of the signature value produced and required by the current algorithm.
    /// @returns size of the signature value in bytes
    /// @threadsafety{Thread-safe}
     
    virtual std::size_t GetSignatureSize() const noexcept = 0;
	
    /// @traceid{SWS_CRYPT_24414}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02108}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Get public key object of the subject.
	/// Created @c PublicKey object is @b session and @c non-exportable, because generic X.509 certificate or
    ///       certificate signing request (CSR) doesn't have COUID of the public key, therefore it should be saved
    ///       or transmitted only as a part of correspondent certificate or CSR.
    /// @returns unique smart pointer to the created public key of the subject

    /// @threadsafety{Thread-safe}
     
    virtual ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc> GetPublicKey() const noexcept  = 0;

    /// @traceid{SWS_CRYPT_24415}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Verify the sameness of the provided and kept public keys.
	/// This method compare the public key values only.
    /// @param[in] publicKey  the public key object for comparison
    /// @returns @c true if values of the stored public key and object provided by the argument are identical and
    ///          @c false otherwise

    /// @threadsafety{Thread-safe}
     
    virtual bool IsSameKey(const ara::crypto::cryp::PublicKey& publicKey) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H_
