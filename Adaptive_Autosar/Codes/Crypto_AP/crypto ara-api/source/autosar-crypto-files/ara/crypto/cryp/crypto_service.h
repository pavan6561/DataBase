// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_service.h
//
// Purpose     : This file contains definitions for CryptoService abstract class.
// -----------------------------------------------------------------------------------------------------------


#ifndef SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYPTO_SERVICE_H_
#define SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYPTO_SERVICE_H_

#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
	

/// @brief Extension meta-information service for cryptographic contexts.
/// @trace SWS_CRYPT_29020
class CryptoService: public ExtensionService
{
public:


    /// @brief Unique smart pointer of the interface.
    /// @trace SWS_CRYPT_29024     
    using Uptr = std::unique_ptr<CryptoService>;

    /// @brief Get maximum expected size of the input data block.
    /// @c suppressPadding argument and it will be equal to the block size.
    /// @param[in] suppressPadding  if @c true then the method calculates the size for the case when the 
    /// whole space of the plain data block is used for the payload only.
    /// @returns maximum size of the input data block in bytes
    /// @trace SWS_CRYPT_29021
    virtual std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept = 0;

    /// @brief Get maximum possible size of the output data block.
	/// If <tt>(IsEncryption() == true)</tt> then a value returned by this method is independent from the
    /// @c suppressPadding argument and will be equal to the block size.
    /// @param[in] suppressPadding  if @c true then the method calculates the size for the case when the 
    /// whole space of the plain data block is used for the payload only
    /// @returns maximum size of the output data block in bytes
    /// @trace SWS_CRYPT_29022
    virtual std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept = 0;

    /// @brief Get block (or internal buffer) size of the base algorithm.
	/// For digest, byte-wise stream cipher and RNG contexts it is an informative method,
    /// intended only for optimization of the interface usage.
    /// @returns size of the block in bytes
    /// @trace SWS_CRYPT_29023
    virtual std::size_t GetBlockSize() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara



#endif /* SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_CRYP_CRYPTO_SERVICE_H_ */
