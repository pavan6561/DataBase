// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : verifier_public_ctx_default_ssl.cpp
//
// Purpose     : This file contains implementation for VerifierPublicCtxdefaultssl class.
// -----------------------------------------------------------------------------------------------------------

#include "verifier_public_ctx_default_ssl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
SignatureService::Uptr VerifierPublicCtxdefaultssl::GetSignatureService() const noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::GetCryptoService()";
  CryptoObjectUid objectID;

  SignatureService::Uptr localPtr{
      std::make_unique<SignatureServiceImpl>(algId_, 2048, kAllowVerification, objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<void> VerifierPublicCtxdefaultssl::SetKey(const PublicKey& key) noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::SetKey()";

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_24115 if the provided key object is incompatible
  // with this Public Key context then SetKey() should return SecurityErrorDomain::kIncompatibleObject.
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)))
  {
    aralog_.LogError() << "This crypto object does not belongs to VerifierPublicCtxdefaultssl";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // According to SWS_CRYPT_24115 if the transformation type associated with this context
  // is prohibited by the "allowed usage" restrictions of provided key object then
  // SetKey should return SecurityErrorDomain::kUsageViolation
  if (!(setkeyusage & kAllowVerification))
  {
    aralog_.LogError() << "This key is not allowed to use for verification";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  publicKey_ = key.GetPublicKey();

  isInitialize_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<bool> VerifierPublicCtxdefaultssl::Verify(
    ReadOnlyMemRegion value, ReadOnlyMemRegion signature, ReadOnlyMemRegion context) const noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::Verify()";
  size_t const kRequiredHashSize{32};
  ara::core::Vector<uint8_t> input;
  static_cast<void>(context);

  // According to SWS_CRYPT_24112 if if the context was not initialized by a key value
  // then Verify() should return SecurityErrorDomain::kUninitializedContext.
  if (!isInitialize_)
  {
    aralog_.LogError() << "Context is not initialized yet !!!!";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_24112 if the context argument has unsupported size then
  // Verify should return SecurityErrorDomain::kInvalidInputSize.
  if (value.size() < kRequiredHashSize)
  {
    aralog_.LogError() << "Input size is not valid !!!!";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // According to SWS_CRYPT_02418 if input size is greater than then input size supported by crypto provider
  // crypto provider should truncate the input value and consider only the leading bytes.
  input.resize(kRequiredHashSize);

  for (size_t index{0}; index < kRequiredHashSize; index++)
  {
    input[index] = value[index];
  }

  EVP_PKEY_CTX* keyCtx = EVP_PKEY_CTX_new_from_pkey(NULL, publicKey_, NULL);

  if (1 != EVP_PKEY_verify_init(keyCtx))
  {
    aralog_.LogError() << "Error in Verify init!!!";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  ConfigureSignCtx(keyCtx);

  if (1 != EVP_PKEY_verify(keyCtx, signature.data(), signature.size(), input.data(), input.size()))
  {
    aralog_.LogError() << "Error in Verifying!!!!";
    return ara::core::Result<bool>::FromValue(false);
  }

  aralog_.LogDebug() << "Verification is succesful";

  return ara::core::Result<bool>::FromValue(true);
}

ara::core::Result<bool> VerifierPublicCtxdefaultssl::VerifyPrehashed(
    const HashFunctionCtx& hashFn, ReadOnlyMemRegion signature, ReadOnlyMemRegion context) const noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::VerifyPrehashed()";

  // According to SWS_CRYPT_24114 if the method hashFn.Finish() was not called
  // before this method call then VerifyPrehashed() should return SecurityErrorDomain::kProcessingNotFinished.
  ara::core::Result<ara::core::Vector<ara::core::Byte>> digestresult{hashFn.GetDigest(0)};
  if (!digestresult.HasValue())
  {
    return ara::core::Result<bool>::FromError(digestresult.Error());
  }
  ara::core::Vector<ara::core::Byte> digestValueinByte{digestresult.Value()};
  ara::core::Vector<uint8_t> digestValue{digestValueinByte.begin(), digestValueinByte.end()};
  ReadOnlyMemRegion value{digestValue};

  return Verify(value, signature, context);
}

ara::core::Result<bool> VerifierPublicCtxdefaultssl::VerifyPrehashed(
    const HashFunctionCtx& hashFn, const Signature& signature, ReadOnlyMemRegion context) const noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::VerifyPrehashed()";

  // According to SWS_CRYPT_24114 if if the CryptoAlgId of hashFn differs from the
  // CryptoAlgId of this context then VerifyPrehashed() should return kInvalidArgument.
  if (hashFn.GetCryptoPrimitiveId()->GetPrimitiveId() != algorithmids::ksha256Hash)
  {
    aralog_.LogError() << "Invalid argument received in VerifyPrehashed";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  return VerifyPrehashed(hashFn, static_cast<ReadOnlyMemRegion>(signature.GetValue()), context);
}

ara::core::Result<bool> VerifierPublicCtxdefaultssl::VerifyPrehashed(CryptoAlgId hashAlgId,
    ReadOnlyMemRegion hashValue, const Signature& signature, ReadOnlyMemRegion context) const noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::VerifyPrehashed()";

  // According to SWS_CRYPT_24113 if the CryptoAlgId of this context does not match the
  // CryptoAlgId of signature then VerifyPrehashed() should return kIncompatibleObject.
  if (CheckSignatureId(signature))
  {
    aralog_.LogError() << "Algorithm Id of the signature does not mathch with the algId of the ctx!!!!";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  // According to SWS_CRYPT_24113 if the required CryptoAlgId of the hash is not kAlgIdDefault
  // and the required hash CryptoAlgId of this context does not match hashAlgId or the hash
  // CryptoAlgId of signature then VerifyPrehashed() should return kIncompatibleObject.
  if (algorithmids::ksha256Hash != signature.GetHashAlgId())
  {
    aralog_.LogError() << "Hash  AlgId of the signature does not mathch with the Hash algId of the ctx!!!!";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  // According to SWS_CRYPT_24113 if the provided hashAlgId is not kAlgIdDefault
  // and the AlgId of the provided signature object does not match the provided hashAlgId
  // then VerifyPrehashed() should return kIncompatibleArguments.
  if (hashAlgId != algorithmids::ksha256Hash)
  {
    aralog_.LogError() << "Given Hash AlgId does not mathch with the Hash algId of the ctx!!!!";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleArguments);
  }

  return Verify(hashValue, static_cast<ReadOnlyMemRegion>(signature.GetValue()), context);
}

bool VerifierPublicCtxdefaultssl::CheckSignatureId(const Signature& signature) const noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::CheckSignatureId()";

  if (signature.GetCryptoPrimitiveId()->GetPrimitiveId() != algId_)
  {
    return true;
  }
  else
  {
    return false;
  }
}

ara::core::Result<void> VerifierPublicCtxdefaultssl::Reset() noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::Reset()";
  isInitialize_ = false;
  publicKey_ = nullptr;
  return ara::core::Result<void>::FromValue();
}

CryptoPrimitiveId::Uptr VerifierPublicCtxdefaultssl::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::GetCryptoPrimitiveId()";
  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr(std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_));
  return primitiveInfoPtr;
}

void VerifierPublicCtxdefaultssl::ConfigureSignCtx(EVP_PKEY_CTX* keyCtx) const noexcept
{
  aralog_.LogDebug() << "In VerifierPublicCtxdefaultssl::ConfigureSignCtx()";

  if (algId_ == algorithmids::kRSASSA2048PKCSV1_5)
  {
    if (1 != EVP_PKEY_CTX_set_rsa_padding(keyCtx, RSA_PKCS1_PADDING))
    {
      aralog_.LogError() << "Padding failed for RSA";
    }
  }

  if (1 != EVP_PKEY_CTX_set_signature_md(keyCtx, EVP_sha256()))
  {
    aralog_.LogError() << "md setting failed";
  }
  else
  {
    aralog_.LogDebug() << "md set succesfully";
  }
}

} // namespace cryp
} // namespace crypto
} // namespace ara
