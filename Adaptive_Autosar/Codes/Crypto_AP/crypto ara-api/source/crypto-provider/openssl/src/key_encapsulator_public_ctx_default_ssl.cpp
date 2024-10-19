// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_encapsulator_public_ctx_default_ssl.cpp
//
// Purpose     : This file contains implementation for KeyEncapsulatorPublicCtxDefaultSsl class.
// -----------------------------------------------------------------------------------------------------------

// NOTE:- This file is out of scope for MS5-IR1 plese do not consider this file for review or static analysis.

#include "key_encapsulator_public_ctx_default_ssl.h"

#include <iomanip>

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
/// @brief namespace for keyencapsulation.
namespace keyencapsulation
{
uint8_t constexpr kWRAPPADDING{8};
uint8_t constexpr KBlockSize{16};
} // namespace keyencapsulation

ExtensionService::Uptr KeyEncapsulatorPublicCtxDefaultSsl::GetExtensionService() const noexcept
{
  aralog_.LogDebug() << "KeyEncapsulatorPublicCtxDefaultSsl::GetExtensionService()";

  CryptoObjectUid objectID;

  ExtensionService::Uptr localPtr{std::make_unique<ExtensionServiceImpl>(
      algorithmids::kRSA2048PKCSV1_5, 2048, kAllowDataEncryption, objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<void> KeyEncapsulatorPublicCtxDefaultSsl::AddKeyingData(
    RestrictedUseObject& keyingData) noexcept
{
  keyingData_ = keyingData.GetValue();
  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> KeyEncapsulatorPublicCtxDefaultSsl::SetKey(const PublicKey& key) noexcept
{
  if (key.kObjectType != CryptoObjectType::kPublicKey)
  {
    aralog_.LogError() << "This crypto object does not belongs to public key";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  if (!(key.GetAllowedUsage() & kAllowDataEncryption))
  {
    aralog_.LogError() << "This key is not allowed to use for public encryption";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  publicKey_ = key.GetPublicKey();

  publicKeyLength_ = key.GetPayloadSize();
  isInitialize_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> KeyEncapsulatorPublicCtxDefaultSsl::Encapsulate(
    KeyDerivationFunctionCtx& kdf, AlgId kekAlgId, ReadOnlyMemRegion salt, ReadOnlyMemRegion ctxLabel) const
    noexcept
{
  if (!isInitialize_)
  {
    aralog_.LogError() << "Context is not initialize yet!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  ara::core::Vector<uint8_t> entropyValue{GenerateEntropy()};

  ReadOnlyMemRegion entropy{entropyValue};

  ara::core::Vector<uint8_t> cipherText{GenerateCipherText(entropy)};

  kdf.AddSalt(salt);

  CryptoObjectUid objectID;
  RestrictedUseObject::Uptrc KMaterial{std::make_unique<SymmetricKeyImpl>(entropyValue, kAllowKdfMaterial,
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algorithmids::kPBKDF2),
      algorithmids::kPBKDF2, true, false, objectID)};

  kdf.SetSourceKeyMaterial(*KMaterial);

  ReadOnlyMemRegion targeKeyId;

  ara::core::Result<void> initResult{kdf.Init(targeKeyId, kekAlgId, kAllowKeyExporting, ctxLabel)};

  if (!initResult.HasValue())
  {
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(initResult.Error());
  }

  SymmetricKey::Uptrc kek{kdf.DeriveKey(true, false).Value()};

  ara::core::Vector<uint8_t> output{WrapKey(*kek, kekAlgId)};

  ara::core::Vector<ara::core::Byte> retValue;

  retValue.resize(cipherText.size() + output.size());

  for (size_t index = 0; index < retValue.size(); index++)
  {
    if (index < cipherText.size())
    {
      retValue[index] = static_cast<ara::core::Byte>(cipherText[index]);
    }
    else
    {
      retValue[index] = static_cast<ara::core::Byte>(output[index - cipherText.size()]);
    }
  }

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retValue);
}

ara::core::Vector<uint8_t> KeyEncapsulatorPublicCtxDefaultSsl::WrapKey(
    const SymmetricKey& kek, AlgId kekalgId) const noexcept
{
  int datalength{0};
  int finalLength{0};
  ara::core::Vector<uint8_t> ciphertext;
  ReadWriteMemRegion kekKey{kek.GetSymmetricKey()};

  EVP_CIPHER_CTX* symmetricKeyWrapperctx = EVP_CIPHER_CTX_new();

  // According to SWS_CRYPT_24011 if algorithm does not support the padding then
  // the granularity will be of block size.
  if ((kekalgId == algorithmids::kKeyWrap128) || (kekalgId == algorithmids::kKeyWrap192)
      || (kekalgId == algorithmids::kKeyWrap256))
  {
    ciphertext.resize(keyingData_.size() + keyencapsulation::kWRAPPADDING);
  }

  // According to SWS_CRYPT_24011 if algorithm supports the padding then
  // the granularity will be of 1 byte.
  if ((kekalgId == algorithmids::kKeyWrapPad128) || (kekalgId == algorithmids::kKeyWrapPad192)
      || (kekalgId == algorithmids::kKeyWrapPad256))
  {
    ciphertext.resize(
        ((keyingData_.size() / (keyencapsulation::KBlockSize + 1)) + 1) * keyencapsulation::KBlockSize);
  }

  if (1 != EVP_EncryptInit_ex(symmetricKeyWrapperctx, GetCipherMode(kekalgId), NULL, kekKey.data(), nullptr))
  {
    aralog_.LogError() << "Encrypt Init is failed";
  }

  EVP_CIPHER_CTX_set_padding(symmetricKeyWrapperctx, 0);

  // Update data in openssl
  if (1
      != EVP_EncryptUpdate(
             symmetricKeyWrapperctx, ciphertext.data(), &datalength, keyingData_.data(), keyingData_.size()))
  {
    aralog_.LogError() << "Encrypt Update failed";
  }

  // Store the length of cipher text for further calculation.
  finalLength = datalength;

  // Finalize the encryption process.
  if (1 != EVP_EncryptFinal_ex(symmetricKeyWrapperctx, ciphertext.data() + datalength, &datalength))
  {
    aralog_.LogError() << "Encrypt Final is failed";
  }

  // Calculate the final length for encrypted data.
  finalLength += datalength;

  EVP_CIPHER_CTX_free(symmetricKeyWrapperctx);

  return ciphertext;
}

ara::core::Vector<uint8_t> KeyEncapsulatorPublicCtxDefaultSsl::GenerateCipherText(ReadOnlyMemRegion in) const
    noexcept
{
  ara::core::Vector<uint8_t> cipherText;
  size_t datalength{0};

  EVP_PKEY_CTX* keyCtx = EVP_PKEY_CTX_new(publicKey_, NULL);

  EVP_PKEY_encrypt_init(keyCtx);

  ConfigureEncryptorCtx(keyCtx);

  EVP_PKEY_encrypt(keyCtx, NULL, &datalength, in.data(), in.size());

  cipherText.resize(datalength);

  EVP_PKEY_encrypt(keyCtx, cipherText.data(), &datalength, in.data(), in.size());

  return cipherText;
}

ara::core::Vector<uint8_t> KeyEncapsulatorPublicCtxDefaultSsl::GenerateEntropy() const noexcept
{
  EVP_RAND* rngInstance;
  EVP_RAND_CTX* randomNumberGeneratorCtx;
  OSSL_PARAM params[2];
  ara::core::Vector<uint8_t> entropy;
  size_t count = GetKekEntropy();
  char cipherName[] = {SN_aes_256_ctr};

  // Create openssl RNG instance.
  rngInstance = EVP_RAND_fetch(NULL, "CTR-DRBG", NULL);

  // Create openssl RNG context.
  randomNumberGeneratorCtx = EVP_RAND_CTX_new(rngInstance, NULL);

  // Set aes-256-ctr cipher for RNG.
  params[0] = OSSL_PARAM_construct_utf8_string(OSSL_DRBG_PARAM_CIPHER, cipherName, 0);

  // end of parameters.
  params[1] = OSSL_PARAM_construct_end();

  // Initialize RNG context.
  EVP_RAND_instantiate(randomNumberGeneratorCtx, 0, 0, NULL, 0, params);

  // Delete openssl RNG instance.
  EVP_RAND_free(rngInstance);

  entropy.resize(count);

  if (1 != EVP_RAND_generate(randomNumberGeneratorCtx, entropy.data(), count, 0, 0, NULL, 0))
  {
    aralog_.LogError() << "Error in random number generation";
  }

  return entropy;
}

void KeyEncapsulatorPublicCtxDefaultSsl::ConfigureEncryptorCtx(EVP_PKEY_CTX* keyCtx) const noexcept
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

ara::core::Result<void> KeyEncapsulatorPublicCtxDefaultSsl::Reset() noexcept
{
  publicKey_ = nullptr;
  isInitialize_ = false;
  return ara::core::Result<void>::FromValue();
}

const EVP_CIPHER* KeyEncapsulatorPublicCtxDefaultSsl::GetCipherMode(AlgId algoId) const noexcept
{
  switch (algoId)
  {
  case algorithmids::kKeyWrap128:
  {
    return EVP_aes_128_wrap();
  }

  case algorithmids::kKeyWrap192:
  {
    return EVP_aes_192_wrap();
  }

  case algorithmids::kKeyWrap256:
  {
    return EVP_aes_256_wrap();
  }

  case algorithmids::kKeyWrapPad128:
  {
    return EVP_aes_128_wrap_pad();
  }

  case algorithmids::kKeyWrapPad192:
  {
    return EVP_aes_192_wrap_pad();
  }

  case algorithmids::kKeyWrapPad256:
  {
    return EVP_aes_256_wrap_pad();
  }

  default:
  {
    return EVP_aes_128_wrap();
  }
  }
}
} // namespace cryp
} // namespace crypto
} // namespace ara
