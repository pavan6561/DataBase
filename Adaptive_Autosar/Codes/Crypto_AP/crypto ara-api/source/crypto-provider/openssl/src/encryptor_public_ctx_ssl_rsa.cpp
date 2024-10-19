// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2020 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : encryptor_public_ctx_ssl_rsa.cpp
//
// Purpose     : This file contains implementation for EncryptorPublicCtxsslRsa class.
// -----------------------------------------------------------------------------------------------------------

#include "encryptor_public_ctx_ssl_rsa.h"
/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
EncryptorPublicCtxsslRsa::EncryptorPublicCtxsslRsa(CryptoPrimitiveId::AlgId algIdValue) noexcept :
  algId_{ algIdValue }
{
  aralog_.LogDebug() << "In EncryptorPublicCtxsslRsa Constructor";
  algoIdentification_ = AlgorithmIdentification::GetAlogIdentification();
}

CryptoService::Uptr EncryptorPublicCtxsslRsa::GetCryptoService() const noexcept
{
  aralog_.LogDebug() << "In EncryptorPublicCtxsslRsa::GetCryptoService()";
  CryptoObjectUid objectID;

  CryptoService::Uptr localPtr {std::make_unique<CryptoServiceImpl>(algId_, 2048, kAllowDataEncryption, objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<void> EncryptorPublicCtxsslRsa::SetKey(const PublicKey& key) noexcept
{
  aralog_.LogDebug() << "In EncryptorPublicCtxsslRsa::SetKey()";

  AlgId const keyAlgId{ key.GetCryptoPrimitiveId()->GetPrimitiveId() };

  // According to SWS_CRYPT_21010 if the provided key object is incompatible
  // with this Public Key context then SetKey() should return SecurityErrorDomain::kIncompatibleObject.
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)))
  {
    aralog_.LogError() << "This crypto object does not belongs to EncryptorPublicCtxsslRsa";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{ key.GetAllowedUsage() };

  // According to SWS_CRYPT_21010 if the transformation type associated with this context
  // is prohibited by the "allowed usage" restrictions of provided key object then
  // SetKey should return SecurityErrorDomain::kUsageViolation
  if (!(setkeyusage & kAllowDataEncryption))
  {
    aralog_.LogError() << "This key is not allowed to use for public encryption";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  publicKey_ = key.GetPublicKey();
  isInitialize_ = true;
  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> EncryptorPublicCtxsslRsa::EncryptorPublicCtxsslRsa::Reset() noexcept
{
  aralog_.LogDebug() << "In EncryptorPublicCtxsslRsa::Reset()";
  isInitialize_ = false;
  publicKey_ = nullptr;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> EncryptorPublicCtxsslRsa::ProcessBlock(
  ReadOnlyMemRegion in, bool suppressPadding) const noexcept
{
  aralog_.LogDebug() << "In EncryptorPublicCtxsslRsa::ProcessBlock()";
  ara::core::Vector<uint8_t> output;
  ara::core::Vector<ara::core::Byte> retVal;
  size_t datalength{ 0 };

  // According to SWS_CRYPT_21012 if if the context was not initialized by a key value
  // then SignAndEncode() should return SecurityErrorDomain::kUninitializedContext.
  if (!isInitialize_)
  {
    aralog_.LogError() << "EncryptorPublicCtx is not initialized yet!!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  size_t const kMaxInputsize{ 245 };

  // According to SWS_CRYPT_21012 if supress padding is true and input size is not equal to MaxInputSize
  // then ProcessBlock() should return SecurityErrorDomain::kIncorrectInputSize.
  if ((in.size() != kMaxInputsize) && (suppressPadding == true))
  {
    aralog_.LogError() << "Input size is not correct as suppress padding is set as true.";
    aralog_.LogError() << "Please pass input with size : " << kMaxInputsize << " bytes";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // According to SWS_CRYPT_21012 if supress padding is false and input size is 0 or greater than MaxInputSize
  // then ProcessBlock() should return SecurityErrorDomain::kIncorrectInputSize.
  if (((in.size() == 0) || (in.size() > kMaxInputsize)) && (suppressPadding == false))
  {
    aralog_.LogError() << "Incorrect input size.";
    aralog_.LogError() << "Please pass input with size greater than 0 bytes and less than " << kMaxInputsize
                       << " bytes";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  EVP_PKEY_CTX* keyCtx = EVP_PKEY_CTX_new(publicKey_, NULL);

  if (1 != EVP_PKEY_encrypt_init(keyCtx))
  {
    aralog_.LogError() << "Init Failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  ConfigureEncryptorCtx(keyCtx);

  if (1 != EVP_PKEY_encrypt(keyCtx, NULL, &datalength, in.data(), in.size()))
  {
    aralog_.LogError() << "Encrypt Failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  output.resize(datalength);
  if (1 != EVP_PKEY_encrypt(keyCtx, output.data(), &datalength, in.data(), in.size()))
  {
    aralog_.LogError() << "Encrypt Failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  for (size_t index = 0; index < datalength; index++)
  {
    retVal.push_back(static_cast<ara::core::Byte>(output[index]));
  }

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

CryptoPrimitiveId::Uptr EncryptorPublicCtxsslRsa::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In EncryptorPublicCtxsslRsa::GetCryptoPrimitiveId()";
  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr localPtr(std::make_unique<CryptoPrimitiveIdImpl>(
    algoIdentification_->GetAlgoName(algorithmids::kRSA2048PKCSV1_5), algorithmids::kRSA2048PKCSV1_5));
  return localPtr;
}

void EncryptorPublicCtxsslRsa::ConfigureEncryptorCtx(EVP_PKEY_CTX* keyCtx) const noexcept
{
  aralog_.LogDebug() << "In EncryptorPublicCtxsslRsa::ConfigureEncryptorCtx()";

  if (algId_ == algorithmids::kRSA2048PKCSV1_5)
  {
    if (1 != EVP_PKEY_CTX_set_rsa_padding(keyCtx, RSA_PKCS1_PADDING))
    {
      aralog_.LogError() << "Padding failed for RSA";
    }

    if (1 != EVP_PKEY_CTX_set_rsa_mgf1_md(keyCtx, EVP_sha1()))
    {
      aralog_.LogError() << "md failed for RSA";
    }
  }
}
} // namespace cryp
} // namespace crypto
} // namespace ara
