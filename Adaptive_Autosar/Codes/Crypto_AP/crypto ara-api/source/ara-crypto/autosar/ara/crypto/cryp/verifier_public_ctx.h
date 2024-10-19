// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : verifier_public_ctx.h
//
// Purpose     : This file contains definitions for VerifierPublicCtx abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_VERIFIER_PUBLIC_CTX_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_VERIFIER_PUBLIC_CTX_H_

#include <ara/core/result.h>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/signature_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
/// @brief Signature Verification Public key Context interface.
/// @trace SWS_CRYPT_24100
class VerifierPublicCtx : public CryptoContext
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_24101
  using Uptr = std::unique_ptr<VerifierPublicCtx>;

  /// @brief Extension service member class
  /// @trace SWS_CRYPT_24102
  virtual SignatureService::Uptr GetSignatureService() const noexcept = 0;

  /// @brief Verify signature by a digest value stored in the hash-function context. This is a pass-through
  /// interface to SWS_CRYPT_24113 for developer convenience, i.e. it adds additional input checks
  /// amd then calls the verify() interface from SWS_CRYPT_24113.
  /// @param[in] hashFn  hash function to be used for hashing
  /// @param[in] signature  the signature object for verification
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns @c true if the signature was verified successfully and @c false otherwise
  /// @error  SecurityErrorDomain::kProcessingNotFinished  if the method @c hashFn.Finish() was not called
  /// before this method call <br>
  /// @error  SecurityErrorDomain::kInvalidArgument  if the CryptoAlgId of @c hashFn differs from the
  /// CryptoAlgId of this context <br>
  /// @trace SWS_CRYPT_24111
  virtual ara::core::Result<bool> VerifyPrehashed(const HashFunctionCtx& hashFn, const Signature& signature,
    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept
    = 0;

  /// @brief Verify signature BLOB by a directly provided hash or message value.
  /// This method can be used for implementation of the "multiple passes" signature algorithms that process
  /// a message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
  /// implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA).
  /// If the target algorithm doesn't support the @c context argument then the empty (default) value must be
  /// supplied! The user supplied @c context may be used for such algorithms as: Ed25519ctx, Ed25519ph
  /// , Ed448ph.
  /// @param[in] value  the (pre-)hashed or direct message value that should be verified
  /// @param[in] signature  the signature BLOB for the verification (the BLOB contains a plain sequence of
  /// the digital
  /// signature components located in fixed/maximum length fields defined by the algorithm specification,
  /// and each component is presented by a raw bytes sequence padded by zeroes to full length of the field;
  /// e.g. in case of (EC)DSA-256 (i.e. length of the q module is 256 bits) the signature BLOB must have two
  /// fixed-size fields: 32 + 32 bytes, for R and S components respectively, i.e. total BLOB size is 64 bytes)
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns @c true if the signature was verified successfully and @c false otherwise
  /// @error  SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key
  /// value <br>
  /// @error  SecurityErrorDomain::kInvalidInputSize  if the @c context argument has unsupported size <br>
  /// @trace SWS_CRYPT_24112
  virtual ara::core::Result<bool> Verify(ReadOnlyMemRegion value, ReadOnlyMemRegion signature,
    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept
    = 0;

  /// @brief Verify signature by a digest value stored in the hash-function context. This is a pass-through
  /// interface to SWS_CRYPT_24112 for developer convenience, i.e. it adds additional input checks
  /// amd then calls the default verify() interface.
  /// @param[in] hashAlgId  hash function algorithm ID
  /// @param[in] hashValue  hash function value (resulting digest without any truncations)
  /// @param[in] signature  the signature object for the verification
  /// @param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns @c true if the signature was verified successfully and @c false otherwise
  /// @error  SecurityErrorDomain::kIncompatibleObject if the CryptoAlgId of this context does not match the
  /// CryptoAlgId of @c signature; or the required CryptoAlgId of the hash is not kAlgIdDefault
  /// and the required hash CryptoAlgId of this context does not match @c hashAlgId or the hash
  /// CryptoAlgId of @c signature <br>
  /// @error  SecurityErrorDomain::kIncompatibleArguments if the provided hashAlgId is not kAlgIdDefault
  /// and the AlgId of the provided signature object does not match the provided hashAlgId
  /// @error  SecurityErrorDomain::kBadObjectReference  if the provided signature object does not
  /// reference the public key loaded to the context,
  /// i.e. if the COUID of the public key in the context is not equal to the COUID referenced from the
  /// signature object.
  /// @trace SWS_CRYPT_24113
  virtual ara::core::Result<bool> VerifyPrehashed(CryptoAlgId hashAlgId, ReadOnlyMemRegion hashValue,
    const Signature& signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept
    = 0;

  /// @brief Verify signature by a digest value stored in the hash-function context. This is a pass-through
  /// interface to SWS_CRYPT_24112 for developer convenience, i.e. it adds additional input checks
  /// and then calls the default verify() interface.
  /// @param[in] hashFn hash function to be used for hashing
  /// @param[in] signature the data BLOB to be verified
  /// @param[in] context an optional user supplied "context" (its support depends from concrete algorithm)
  /// @returns @c true if the signature was verified successfully and @c false otherwise
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the method @c hashFn.Finish() was not called
  /// before this method call <br>
  /// @error SecurityErrorDomain::kInvalidArgument  if the CryptoAlgId of @c hashFn differs from the
  /// CryptoAlgId of this context <br>
  /// @trace SWS_CRYPT_24114
  virtual ara::core::Result<bool> VerifyPrehashed(const HashFunctionCtx& hashFn, ReadOnlyMemRegion signature,
    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept
    = 0;

  /// @brief Set (deploy) a key to the verifier public algorithm context.
  /// @param[in] key  the source key object
  /// @error  SecurityErrorDomain::kIncompatibleObject  if the provided key object is
  /// incompatible with this symmetric key context <br>
  /// @error  SecurityErrorDomain::kUsageViolation  if the transformation type associated with this context
  /// is prohibited by the "allowed usage" restrictions of provided key object <br>
  /// @trace SWS_CRYPT_24115
  virtual ara::core::Result<void> SetKey(const PublicKey& key) noexcept = 0;

  /// @brief Clear the crypto context.
  /// @trace SWS_CRYPT_24116
  virtual ara::core::Result<void> Reset() noexcept = 0;

  // virtual bool CheckSignatureId(const Signature& signature) const noexcept = 0;
};

//inline ara::core::Result<bool> VerifierPublicCtx::Verify(CryptoAlgId hashAlgId
//    , ReadOnlyMemRegion hashValue
//    , const Signature& signature
//    , ReadOnlyMemRegion context
//) const noexcept
//{
//
//    using Res = ara::core::Result<bool>;
//    if(this->GetCryptoPrimitiveId()->GetPrimitiveId() != signature.GetCryptoPrimitiveId()->GetPrimitiveId())
//    {
//        return Res::FromError(ara::crypto::MakeErrorCode(SecurityErrc::kIncompatibleObject, 1));
//    }
//    if(this->GetSignatureService()->GetRequiredHashAlgId() != kAlgIdDefault)
//    {
//        if(signature.GetHashAlgId() != this->GetSignatureService()->GetRequiredHashAlgId())
//        {
//            return Res::FromError(ara::crypto::MakeErrorCode(SecurityErrc::kIncompatibleObject, 1));
//        }
//        if(hashAlgId != this->GetSignatureService()->GetRequiredHashAlgId())
//        {
//            return Res::FromError(ara::crypto::MakeErrorCode(SecurityErrc::kIncompatibleObject, 2));
//        }
//    }
//    else
//    {
//        if(hashAlgId != signature.GetHashAlgId())
//        {
//            //return Res::FromError(ara::crypto::MakeErrorCode(SecurityErrc::kIncompatibleArguments, (2 << 8) | 1));
//        }
//    }
//    /* The supplied Signature object must depend on the same PublicKey used to verify it. */
//    CryptoObject::COIdentifier appliedID = signature.HasDependence();
//    if(CryptoObjectType::kPublicKey != appliedID.mCOType ||
//       this->GetSignatureService()->GetActualKeyCOUID() != appliedID.mCouid)
//    {
//        return Res::FromError(ara::crypto::MakeErrorCode(SecurityErrc::kBadObjectReference, 1));
//    }
//
//    return Verify(hashValue, signature.GetValue(), context);
//}
//
//
//inline ara::core::Result<bool> VerifierPublicCtx::Verify(const HashFunctionCtx& hashFn
//    , const Signature& signature
//    , ReadOnlyMemRegion context
//) const noexcept
//{
//    if(!hashFn.GetDigestService()->IsFinished())
//    {
//        return ara::core::Result<bool>::FromError(ara::crypto::MakeErrorCode(SecurityErrc::kProcessingNotFinished, 1));
//    }
//    if( this->GetSignatureService()->GetRequiredHashAlgId() != kAlgIdDefault &&
//    		this->GetSignatureService()->GetRequiredHashAlgId() != hashFn.GetCryptoPrimitiveId()->GetPrimitiveId())
//    {
//    	return ara::core::Result<bool>::FromError(ara::crypto::MakeErrorCode(SecurityErrc::kInvalidArgument, 1));
//    }
//    return Verify(hashFn.GetCryptoPrimitiveId()->GetPrimitiveId(), hashFn.GetValue(), signature, context);
//}
//
//inline ara::core::Result<bool> VerifierPublicCtx::Verify(const HashFunctionCtx& hashFn
//    , ReadOnlyMemRegion signature
//    , ReadOnlyMemRegion context
//) const noexcept
//{
//    using Res = ara::core::Result<bool>;
//    if(!hashFn.GetDigestService()->IsFinished())
//    {
//        return Res::FromError(ara::crypto::MakeErrorCode(SecurityErrc::kProcessingNotFinished, 1));
//    }
//    if( this->GetSignatureService()->GetRequiredHashAlgId() != kAlgIdDefault &&
//        this->GetSignatureService()->GetRequiredHashAlgId() != hashFn.GetCryptoPrimitiveId()->GetPrimitiveId())
//    {
//        return Res::FromError(ara::crypto::MakeErrorCode(SecurityErrc::kInvalidArgument, 1));
//    }
//    return Verify(hashFn.GetValue(), signature, context);
//}

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_VERIFIER_PUBLIC_CTX_H_
