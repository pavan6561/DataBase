// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_service_impl.h
//
// Purpose     : This file contains the definition required for cryptoservice application.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYPTO_SERVICE_IMPL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYPTO_SERVICE_IMPL_H_

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_service.h"
#include "ara/core/result.h"
#include "extension_service_impl.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
/// @brief Extension meta-information service for cryptographic contexts.
/// @trace SWS_CRYPT_29020
class CryptoServiceImpl: public CryptoService
{
public:
    /// @brief Constructor for CryptoServiceImpl class.
    CryptoServiceImpl(CryptoAlgId algoid, size_t currentKeyBitSize, AllowedUsageFlags 
    currentUsageFlags, CryptoObjectUid id, bool isKeyAvailableinCrrentCtx) noexcept;

    /// @brief Distructor for CryptoServiceImpl class.
    ~CryptoServiceImpl() noexcept;

    /// @brief Get maximum expected size of the input data block.
    /// @c suppressPadding argument and it will be equal to the block size.
    /// @param[in] suppressPadding  if @c true then the method calculates the size for the case when the 
    /// whole space of the plain data block is used for the payload only
    /// @returns maximum size of the input data block in bytes
    /// @trace SWS_CRYPT_29021
    std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept override;

    /// @brief Get maximum possible size of the output data block.
    /// If <tt>(IsEncryption() == true)</tt> then a value returned by this method is independent from the
    /// @c suppressPadding argument and will be equal to the block size.
    /// @param[in] suppressPadding  if @c true then the method calculates the size for the case when the 
    /// whole space of the plain data block is used for the payload only
    /// @returns maximum size of the output data block in bytes
    /// @trace SWS_CRYPT_29022
    std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept override;

    /// @brief Get block (or internal buffer) size of the base algorithm.
    /// For digest, byte-wise stream cipher and RNG contexts it is an informative method,
    /// intended only for optimization of the interface usage.
    /// @returns size of the block in bytes
    /// @trace SWS_CRYPT_29023
    std::size_t GetBlockSize() const noexcept;

    /// @brief Verify supportness of specific key length by the context.
    /// @param[in] keyBitLength  length of the key in bits
    /// @returns @c true if provided value of the key length is supported by the context
    /// @trace SWS_CRYPT_29048
    bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept override;

    /// @brief Get minimal supported key length in bits.
    /// @returns minimal supported length of the key in bits
    /// @trace SWS_CRYPT_29043 
    std::size_t GetMinKeyBitLength() const noexcept override;

    /// @brief Get maximal supported key length in bits.
    /// @returns maximal supported length of the key in bits
    /// @trace SWS_CRYPT_29044
    std::size_t GetMaxKeyBitLength() const noexcept override;

    /// @brief Get actual bit-length of a key loaded to the context. If no key was set to the context yet 
    /// then 0 is returned.
    /// @returns actual length of a key (now set to the algorithm context) in bits
    /// @trace SWS_CRYPT_29045
    std::size_t GetActualKeyBitLength() const noexcept override;

    /// @brief Get allowed usages of this context (according to the key object attributes loaded to this 
    /// context).
    /// If the context is not initialized by a key object yet then zero (all flags are reset) must be 
    /// returned.
    /// @returns a combination of bit-flags that specifies allowed usages of the context
    /// @trace SWS_CRYPT_29046
    AllowedUsageFlags GetAllowedUsage() const noexcept override;

    /// @brief Get the COUID of the key deployed to the context this extension service is attached to. 
    /// If no key was set to the context yet then an empty COUID (Nil) is returned.
    /// @returns the COUID of the CryptoObject
    /// @trace SWS_CRYPT_29047
    CryptoObjectUid GetActualKeyCOUID() const noexcept override;

    /// @brief Check if a key has been set to this context.
    /// @returns FALSE if no key has been set
    /// @trace SWS_CRYPT_29049
    bool IsKeyAvailable() const noexcept override;

    private:

    /// @brief Store algId for which the current instance of extension services is created.
    CryptoAlgId algId_;

    /// @brief Pointer for extension service.
    ExtensionServiceImpl* extensionServiInstance_;

    /// @brief logger variable for ara::log.
    ara::log::Logger& aralog_;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYPTO_SERVICE_IMPL_H_
