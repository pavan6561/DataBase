// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : sig_encode_private_ctx_default_ssl.cpp
//
// Purpose     : This file contains implementation for SigEncodePrivateCtxDefaultSsl class.
// -----------------------------------------------------------------------------------------------------------

#include "sig_encode_private_ctx_default_ssl.h"

#include <iomanip>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
ExtensionService::Uptr SigEncodePrivateCtxDefaultSsl::GetExtensionService() const noexcept
{
  aralog_.LogDebug() << "SigEncodePrivateCtxDefaultSsl::GetExtensionService()";

  CryptoObjectUid objectID;

  ExtensionService::Uptr localPtr{std::make_unique<ExtensionServiceImpl>(
      algId_, 2048, (kAllowDataEncryption | kAllowSignature), objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<void> SigEncodePrivateCtxDefaultSsl::SetKey(const PrivateKey& key) noexcept
{
  aralog_.LogDebug() << "In SigEncodePrivateCtxDefaultSsl::SetKey()";

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_23211 if the provided key object is incompatible
  // with this Publickey key context then SetKey() should return SecurityErrorDomain::kIncompatibleObject.
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)))
  {
    aralog_.LogError() << "This crypto object does not belongs to SigEncodePrivateCtxDefaultSsl";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // According to SWS_CRYPT_23211 if the transformation type associated with this context
  // is prohibited by the "allowed usage" restrictions of provided key object then
  // SetKey should return SecurityErrorDomain::kUsageViolation
  if (!(setkeyusage & (kAllowDataEncryption | kAllowSignature)))
  {
    aralog_.LogError() << "This key is not allowed to use for signature production";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  privateKey_ = key.GetPrivateKey();

  isInitialize_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> SigEncodePrivateCtxDefaultSsl::Reset() noexcept
{
  aralog_.LogDebug() << "In SigEncodePrivateCtxDefaultSsl::Reset()";
  isInitialize_ = false;
  privateKey_ = nullptr;
  return ara::core::Result<void>::FromValue();
}

std::size_t SigEncodePrivateCtxDefaultSsl::GetMaxInputSize(bool suppressPadding) const noexcept
{
  aralog_.LogDebug() << "In SigEncodePrivateCtxDefaultSsl::GetMaxInputSize()";
  std::size_t const kmaxInputSize{32};
  static_cast<void>(suppressPadding);
  return kmaxInputSize;
}

std::size_t SigEncodePrivateCtxDefaultSsl::GetMaxOutputSize(bool suppressPadding) const noexcept
{
  aralog_.LogDebug() << "In SigEncodePrivateCtxDefaultSsl::GetMaxOutputSize()";
  std::size_t const kmaxOutputSize{402};
  static_cast<void>(suppressPadding);
  return kmaxOutputSize;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SigEncodePrivateCtxDefaultSsl::SignAndEncode(
    ReadOnlyMemRegion in) const noexcept
{
  aralog_.LogDebug() << "In SigEncodePrivateCtxDefaultSsl::SignAndEncode()";

  EVP_PKEY_CTX* keyctx = EVP_PKEY_CTX_new(privateKey_, NULL);
  ara::core::Vector<uint8_t> signature;
  ara::core::Vector<uint8_t> encodedValue;
  size_t signatureLenght{0};

  // According to SWS_CRYPT_23215 if if the context was not initialized by a key value
  // then SignAndEncode() should return SecurityErrorDomain::kUninitializedContext.
  if (!isInitialize_)
  {
    aralog_.LogError() << "Contex is not initialized yet!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_23215 if input size is not same as the maximum input size then SignAndEncode()
  // should return SecurityErrorDomain::kInvalidInputSize.
  if (in.size() != GetMaxInputSize(false))
  {
    aralog_.LogError() << "Input size is invalid!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  if (1 != EVP_PKEY_sign_init(keyctx))
  {
    aralog_.LogError() << "Init Failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }
  else
  {
    aralog_.LogDebug() << "Init success";
  }

  ConfigureSignCtx(keyctx);

  if (1 != EVP_PKEY_sign(keyctx, NULL, &signatureLenght, in.data(), in.size()))
  {
    aralog_.LogError() << "Sign Failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }
  else
  {
    aralog_.LogDebug() << "Sign success with NULL";
  }

  signature.resize(signatureLenght);

  if (1 != EVP_PKEY_sign(keyctx, signature.data(), &signatureLenght, in.data(), in.size()))
  {
    aralog_.LogError() << "Sign Failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }
  else
  {
    aralog_.LogDebug() << "Sign success with signature";
  }

  signature.resize(signatureLenght);

  EVP_PKEY_CTX_free(keyctx);

  // Design decision taken on 5th August 2021 that for encoding purpose PEM encoding
  // will be used with openssl BIO. As earlier when tried with openssl's EVP_encode
  // APIs, segmentation faults occurred, as these API were too heavy.

  BIO* signatureBIO = BIO_new(BIO_s_mem());

  PEM_write_bio(signatureBIO, "SIGNATURE", " ", signature.data(), signature.size());

  int signatureLen = BIO_pending(signatureBIO);

  ara::core::Vector<uint8_t> output;

  output.resize(signatureLen);

  BIO_read(signatureBIO, output.data(), output.size());

  BIO_free(signatureBIO);

  ara::core::Vector<ara::core::Byte> retVal{output.begin(), output.end()};

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

CryptoPrimitiveId::Uptr SigEncodePrivateCtxDefaultSsl::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::GetCryptoPrimitiveId()";
  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr(std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_));
  return primitiveInfoPtr;
}

void SigEncodePrivateCtxDefaultSsl::ConfigureSignCtx(EVP_PKEY_CTX* keyCtx) const noexcept
{
  aralog_.LogDebug() << "In SignerPrivateCtxDefaultssl::ConfigureSignCtx()";

  if (algId_ == algorithmids::kRSASSA2048PKCSV1_5)
  {
    if (1 != EVP_PKEY_CTX_set_rsa_padding(keyCtx, RSA_PKCS1_PADDING))
    {
      aralog_.LogError() << "Padding failed for RSA";
    }

    if (1 != EVP_PKEY_CTX_set_signature_md(keyCtx, EVP_sha256()))
    {
      aralog_.LogError() << "md failed for RSA";
    }
  }
}

} // namespace cryp
} // namespace crypto
} // namespace ara
