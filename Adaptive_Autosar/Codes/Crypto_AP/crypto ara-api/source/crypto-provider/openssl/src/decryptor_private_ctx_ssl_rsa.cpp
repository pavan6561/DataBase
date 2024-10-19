// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2020 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : decryptor_private_ctx_ssl_rsa.cpp
//
// Purpose     : This file contains implementation for decryptorPublicCtxsslAes class.
// -----------------------------------------------------------------------------------------------------------

#include "decryptor_private_ctx_ssl_rsa.h"
/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
DecryptorPrivateCtxRSA::DecryptorPrivateCtxRSA(CryptoPrimitiveId::AlgId algIdValue) noexcept :
    algId_{algIdValue}
{
  aralog_.LogDebug() << "In DecryptorPrivateCtxRSA Constructor";
  algoIdentification_ = AlgorithmIdentification::GetAlogIdentification();
}

CryptoService::Uptr DecryptorPrivateCtxRSA::GetCryptoService() const noexcept
{
  aralog_.LogDebug() << "In EncryptorPublicCtxsslRsa::GetCryptoService()";
  CryptoObjectUid objectID;

  CryptoService::Uptr localPtr{
      std::make_unique<CryptoServiceImpl>(algId_, 2048, kAllowDataDecryption, objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<void> DecryptorPrivateCtxRSA::SetKey(const PrivateKey& key) noexcept
{
  aralog_.LogDebug() << "In DecryptorPrivateCtxRSA::SetKey()";

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_20810 if the provided key object is incompatible
  // with this Public Key context then SetKey() should return SecurityErrorDomain::kIncompatibleObject.
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)))
  {
    aralog_.LogError() << "This crypto object does not belongs to DecryptorPrivateCtxRSA";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // According to SWS_CRYPT_20810 if the transformation type associated with this context
  // is prohibited by the "allowed usage" restrictions of provided key object then
  // SetKey should return SecurityErrorDomain::kUsageViolation
  if (!(setkeyusage & kAllowDataDecryption))
  {
    aralog_.LogError() << "This key is not allowed to use for public decryption";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  privateKey_ = key.GetPrivateKey();
  isInitialize_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> DecryptorPrivateCtxRSA::Reset() noexcept
{
  aralog_.LogDebug() << "In DecryptorPrivateCtxRSA::Reset()";
  isInitialize_ = false;
  privateKey_ = nullptr;
  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> DecryptorPrivateCtxRSA::ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding) const noexcept
{
  aralog_.LogDebug() << "In DecryptorPrivateCtxRSA::ProcessBlock()";

  static_cast<void>(suppressPadding);

  ara::core::Vector<uint8_t> output;
  size_t datalength{0};

  size_t const kRSABlockSize{256};

  // According to SWS_CRYPT_20812 if if the context was not initialized by a key value
  // then SignAndEncode() should return SecurityErrorDomain::kUninitializedContext.
  if (!isInitialize_)
  {
    aralog_.LogError() << "EncryptorPublicCtx is not initialized yet!!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_20812 if then input size is not equal to the block size of the
  // the RSA_PCKSV1_5.
  if (in.size() != kRSABlockSize)
  {
    aralog_.LogError() << "Incorrect input size.";
    aralog_.LogError() << "Please pass input with size equal to " << kRSABlockSize << " bytes";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  EVP_PKEY_CTX* keyCtx = EVP_PKEY_CTX_new(privateKey_, NULL);

  if (1 != EVP_PKEY_decrypt_init(keyCtx))
  {
    aralog_.LogError() << "Init Failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  ConfigureEncryptorCtx(keyCtx);

  if (1 != EVP_PKEY_decrypt(keyCtx, NULL, &datalength, in.data(), in.size()))
  {
    aralog_.LogError() << "Decrypt Failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }
  output.resize(datalength);
  if (1 != EVP_PKEY_decrypt(keyCtx, output.data(), &datalength, in.data(), in.size()))
  {
    aralog_.LogError() << "Decrypt Failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  output.resize(datalength);

  ara::core::Vector<ara::core::Byte> retVal{output.begin(), output.end()};

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

CryptoPrimitiveId::Uptr DecryptorPrivateCtxRSA::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In DecryptorPrivateCtxRSA::GetCryptoPrimitiveId()";

  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this perticular ctx.
  CryptoPrimitiveId::Uptr localPtr(std::make_unique<CryptoPrimitiveIdImpl>(
      algoIdentification_->GetAlgoName(algorithmids::kRSA2048PKCSV1_5), algorithmids::kRSA2048PKCSV1_5));
  return localPtr;
}

void DecryptorPrivateCtxRSA::ConfigureEncryptorCtx(EVP_PKEY_CTX* keyCtx) const noexcept
{
  aralog_.LogDebug() << "In DecryptorPrivateCtxRSA::ConfigureEncryptorCtx()";

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
