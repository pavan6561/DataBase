// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : block_service.h
//
// Purpose     : This file contains definitions for BlockService abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_BLOCK_SERVICE_H_
#define SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_BLOCK_SERVICE_H_

#include "ara/crypto/cryp/extension_service.h"
#include "ara/core/optional.h"

/// @brief namespace for adaptive runtime application.
namespace ara
{
/// @brief namespace for crypto function cluster.
namespace crypto
{
/// @brief namespace for crypto services.
namespace cryp
{
	
/// @brief Extension meta-information service for block cipher contexts.
/// @trace SWS_CRYPT_29030
class BlockService: public ExtensionService
{
public:

    /// @brief Unique smart pointer of the interface.
    /// @trace SWS_CRYPT_29031     
    using Uptr = std::unique_ptr<BlockService>;

    /// @brief Get default expected size of the Initialization Vector (IV) or nonce.
    /// @returns default expected size of IV in bytes
    /// @trace SWS_CRYPT_29032
    virtual std::size_t GetIvSize() const noexcept = 0;

    /// @brief Get block (or internal buffer) size of the base algorithm.
    /// @returns size of the block in bytes
    /// @trace SWS_CRYPT_29033
    virtual std::size_t GetBlockSize() const noexcept = 0;

    /// @brief Verify validity of specific Initialization Vector (IV) length.
    /// @param[in] ivSize  the length of the IV in bytes
    /// @returns @c true if provided IV length is supported by the algorithm and @c false otherwise
    /// @trace SWS_CRYPT_29034
    virtual bool IsValidIvSize(std::size_t ivSize) const noexcept = 0;

    /// @brief Get actual bit-length of an IV loaded to the context.
    /// @param[in] ivUid  optional pointer to a buffer for saving an @a COUID of a IV object now loaded to 
    /// the context.
	/// If the context was initialized by a @c SecretSeed object then the output buffer @c *ivUid must be 
    /// filled by @a COUID of this loaded IV object, in other cases @c *ivUid must be filled by all zeros.
    /// @returns actual length of the IV (now set to the algorithm context) in bits
    /// @trace SWS_CRYPT_29035
    virtual std::size_t GetActualIvBitLength(ara::core::Optional<CryptoObjectUid> ivUid) const noexcept = 0;
};


}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif /* SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_BLOCK_SERVICE_H_ */
