// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signer_private_ctx_default_ssl.cpp
//
// Purpose     : This file contains implementation for SignerPrivateCtxDefaultssl class.
// -----------------------------------------------------------------------------------------------------------

#include "signer_private_ctx_default_ssl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
SignatureService::Uptr SignerPrivateCtxDefaultssl::GetSignatureService() const noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::GetCryptoService()";
  CryptoObjectUid objectID;

  SignatureService::Uptr localPtr{
      std::make_unique<SignatureServiceImpl>(algId_, 2048, kAllowSignature, objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<void> SignerPrivateCtxDefaultssl::SetKey(const PrivateKey& key) noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::SetKey()";

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_23515 if the provided key object is incompatible
  // with this Publikey key context then SetKey() should return SecurityErrorDomain::kIncompatibleObject.
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)))
  {
    aralog_.LogError() << "This crypto object does not belongs to SignerPrivateCtxDefaultssl";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // According to SWS_CRYPT_23515 if if the transformation type associated with this context
  // is prohibited by the "allowed usage" restrictions of provided key object
  // then SetKey should return SecurityErrorDomain::kUsageViolation
  if (!(setkeyusage & kAllowSignature))
  {
    aralog_.LogError() << "This key is not allowed to use for signature production";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  privateKey_ = key.GetPrivateKey();

  isInitialize_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<Signature::Uptrc> SignerPrivateCtxDefaultssl::SignPreHashed(
    const HashFunctionCtx& hashFn, ReadOnlyMemRegion context) const noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::SignPreHashed()";

  static_cast<void>(context);

  // According to SWS_CRYPT_23511 if if the context was not initialized by a key value
  // then SignPreHashed() should return SecurityErrorDomain::kUninitializedContext.
  if (!isInitialize_)
  {
    aralog_.LogError() << "Context is not initialized yet !!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_23511 if if the method hash.Finish() was not called before the call of this
  /// method SignPreHashed() should return SecurityErrorDomain::kProcessingNotFinished.
  ara::core::Result<ara::core::Vector<ara::core::Byte>> digestresult{hashFn.GetDigest(0)};
  if (!digestresult.HasValue())
  {
    return ara::core::Result<Signature::Uptrc>::FromError(digestresult.Error());
  }

  ara::core::Vector<ara::core::Byte> digestValueinByte{digestresult.Value()};
  ara::core::Vector<uint8_t> digestValue{digestValueinByte.begin(), digestValueinByte.end()};
  ara::core::Vector<uint8_t> signatureValue;
  ara::core::Vector<uint8_t> input;
  size_t signatureLength{0};

  size_t const kRequiredHashSize{32};

  if (hashFn.GetCryptoPrimitiveId()->GetPrimitiveId() != algorithmids::ksha256Hash)
  {
    aralog_.LogError() << "Hash alg Id is does not comply with signature algorithm!!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  // According to SWS_CRYPT_23513 if the user supplied context has incorrect
  // (or unsupported) size then SignPreHashed() should return SecurityErrorDomain::kInvalidInputSize.
  if (digestValue.size() < kRequiredHashSize)
  {
    aralog_.LogError() << "Input size is invalid !!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // According to SWS_CRYPT_02418 if input size is greater than then input size supported by crypto provider
  // crypto provider should truncate the input value and consider only the leading bytes.
  input.resize(kRequiredHashSize);

  for (size_t index{0}; index < kRequiredHashSize; index++)
  {
    input[index] = digestValue[index];
  }

  EVP_PKEY_CTX* keyCtx = EVP_PKEY_CTX_new_from_pkey(NULL, privateKey_, NULL);

  ConfigureSignCtx(keyCtx);

  if (1 != EVP_PKEY_sign_init(keyCtx))
  {
    aralog_.LogError() << "Error in Sign init!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  if (1 != EVP_PKEY_sign(keyCtx, NULL, &signatureLength, input.data(), input.size()))
  {
    aralog_.LogError() << "Error in signing!!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  signatureValue.resize(signatureLength);

  if (1 != EVP_PKEY_sign(keyCtx, signatureValue.data(), &signatureLength, input.data(), input.size()))
  {
    aralog_.LogError() << "Error in signing!!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  signatureValue.resize(signatureLength);

  ReadWriteMemRegion signatureMemory{signatureValue};

  Signature::Uptrc signatureObj{std::make_unique<SignatureImpl>(signatureMemory,
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_,
      hashFn.GetCryptoPrimitiveId()->GetPrimitiveId(), kRequiredHashSize, false, true)};

  return ara::core::Result<Signature::Uptrc>::FromValue(std::move(signatureObj));
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SignerPrivateCtxDefaultssl::Sign(
    ReadOnlyMemRegion value, ReadOnlyMemRegion context) const noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::Sign()";

  static_cast<void>(context);
  ara::core::Vector<uint8_t> signatureValue;
  ara::core::Vector<uint8_t> input;
  size_t signatureLength{0};
  size_t const kRequiredHashSize{32};

  // According to SWS_CRYPT_23512 if if the context was not initialized by a key value
  // then SignPreHashed() should return SecurityErrorDomain::kUninitializedContext.
  if (!isInitialize_)
  {
    aralog_.LogError() << "Context is not initialized yet !!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_23513 if the user supplied context has incorrect
  // (or unsupported) size then SignPreHashed() should return SecurityErrorDomain::kInvalidInputSize.
  if (value.size() < kRequiredHashSize)
  {
    aralog_.LogError() << "Input size is invalid !!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // According to SWS_CRYPT_02418 if input size is greater than then input size supported by crypto provider
  // crypto provider should truncate the input value and consider only the leading bytes.
  input.resize(kRequiredHashSize);

  for (size_t index{0}; index < kRequiredHashSize; index++)
  {
    input[index] = value[index];
  }

  EVP_PKEY_CTX* keyCtx = EVP_PKEY_CTX_new_from_pkey(NULL, privateKey_, NULL);

  if (1 != EVP_PKEY_sign_init(keyCtx))
  {
    aralog_.LogError() << "Error in Sign init!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  ConfigureSignCtx(keyCtx);

  if (1 != EVP_PKEY_sign(keyCtx, NULL, &signatureLength, input.data(), input.size()))
  {
    aralog_.LogError() << "Error in signing!!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  signatureValue.resize(signatureLength);

  if (1 != EVP_PKEY_sign(keyCtx, signatureValue.data(), &signatureLength, input.data(), input.size()))
  {
    aralog_.LogError() << "Error in signing!!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  signatureValue.resize(signatureLength);

  ara::core::Vector<ara::core::Byte> retVal{signatureValue.begin(), signatureValue.end()};

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

ara::core::Result<Signature::Uptrc> SignerPrivateCtxDefaultssl::SignPreHashed(
    AlgId hashAlgId, ReadOnlyMemRegion hashValue, ReadOnlyMemRegion context) const noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::SignPreHashed()";
  static_cast<void>(context);
  ara::core::Vector<uint8_t> signatureValue;
  ara::core::Vector<uint8_t> input;
  size_t signatureLength{0};
  size_t const kRequiredHashSize{32};

  // According to SWS_CRYPT_23513 if the context was not initialized by a key value
  // then SignPreHashed() should return SecurityErrorDomain::kUninitializedContext.
  if (!isInitialize_)
  {
    aralog_.LogError() << "Context is not initialized yet !!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_23513 if hash-function algorithm does not comply with
  // the signature algorithm specification of this context then SignPreHashed()
  // should return SecurityErrorDomain::kInvalidArgument.
  if (hashAlgId != algorithmids::ksha256Hash)
  {
    aralog_.LogError() << "Hash alg Id is does not comply with signature algorithm!!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  // According to SWS_CRYPT_23513 if the user supplied context has incorrect
  // (or unsupported) size then SignPreHashed() should return SecurityErrorDomain::kInvalidInputSize.
  if (hashValue.size() < kRequiredHashSize)
  {
    aralog_.LogError() << "Input size is invalid !!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // According to SWS_CRYPT_02418 if input size is greater than then input size supported by crypto provider
  // crypto provider should truncate the input value and consider only the leading bytes.
  input.resize(kRequiredHashSize);

  for (size_t index{0}; index < kRequiredHashSize; index++)
  {
    input[index] = hashValue[index];
  }

  EVP_PKEY_CTX* keyCtx = EVP_PKEY_CTX_new_from_pkey(NULL, privateKey_, NULL);

  ConfigureSignCtx(keyCtx);

  if (1 != EVP_PKEY_sign_init(keyCtx))
  {
    aralog_.LogError() << "Error in Sign init!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  if (1 != EVP_PKEY_sign(keyCtx, NULL, &signatureLength, input.data(), input.size()))
  {
    aralog_.LogError() << "Error in signing!!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  signatureValue.resize(signatureLength);

  if (1 != EVP_PKEY_sign(keyCtx, signatureValue.data(), &signatureLength, input.data(), input.size()))
  {
    aralog_.LogError() << "Error in signing!!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  signatureValue.resize(signatureLength);

  ReadWriteMemRegion signatureMemory{signatureValue};

  Signature::Uptrc signatureObj{std::make_unique<SignatureImpl>(signatureMemory,
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_, hashAlgId,
      kRequiredHashSize, false, true)};

  aralog_.LogDebug() << "Data signed succesfully and created signature object";

  return ara::core::Result<Signature::Uptrc>::FromValue(std::move(signatureObj));
}

ara::core::Result<void> SignerPrivateCtxDefaultssl::Reset() noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::Reset()";
  isInitialize_ = false;
  privateKey_ = nullptr;
  return ara::core::Result<void>::FromValue();
}

CryptoPrimitiveId::Uptr SignerPrivateCtxDefaultssl::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::GetCryptoPrimitiveId()";
  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this perticular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr(std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_));
  return primitiveInfoPtr;
}

void SignerPrivateCtxDefaultssl::ConfigureSignCtx(EVP_PKEY_CTX* keyCtx) const noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::ConfigureSignCtx()";
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