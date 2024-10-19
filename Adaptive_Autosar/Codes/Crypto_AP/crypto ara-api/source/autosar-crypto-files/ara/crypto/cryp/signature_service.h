// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : digest_service.h
//
// Purpose     : This file contains definitions for SignatureService abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef SOURCE_AUTOSAR_CRYPTO_FILES_CRYPTO_ARA_CRYPTO_CRYP_SIGNATURE_SERVICE_H_
#define SOURCE_AUTOSAR_CRYPTO_FILES_CRYPTO_ARA_CRYPTO_CRYP_SIGNATURE_SERVICE_H_

#include "ara/crypto/cryp/extension_service.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Extension meta-information service for signature contexts.
/// @trace SWS_CRYPT_29000
class SignatureService: public ExtensionService
{
public:

    /// @brief Unique smart pointer of the interface.
    /// @trace SWS_CRYPT_29001     
    using Uptr = std::unique_ptr<SignatureService>;


    /// @brief Get the hash size required by current signature algorithm.
    /// @returns required hash size in bytes
    /// @trace SWS_CRYPT_29002     
    virtual std::size_t GetRequiredHashSize() const noexcept = 0;

    /// @brief Get an ID of hash algorithm required by current signature algorithm.
    /// @returns required hash algorithm ID or @c kAlgIdAny if the signature algorithm specification does not 
    /// include a concrete hash function
    /// @trace SWS_CRYPT_29003
    virtual CryptoPrimitiveId::AlgId GetRequiredHashAlgId() const noexcept = 0;

    /// @brief Get size of the signature value produced and required by the current algorithm.
    /// @returns size of the signature value in bytes
    /// @trace SWS_CRYPT_29004
    virtual std::size_t GetSignatureSize() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara


#endif /* ARA_CRYPTO_CRYP_SIGNATURE_SERVICE_H_ */
