// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_decapsulator_private_ctx_default_ssl.cpp
//
// Purpose     : This file contains implementation for KeyDecapsulatorPrivateCtxDefaultSsl class.
// -----------------------------------------------------------------------------------------------------------

#include "key_decapsulator_private_ctx_default_ssl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
/// @brief namespace for keydecapsulation.
namespace keydecapsulation
{
uint8_t constexpr kWRAPPADDING{8};
uint8_t constexpr KBlockSize{16};
} // namespace keydecapsulation

ara::core::Result<void> KeyDecapsulatorPrivateCtxDefaultSsl::SetKey(const PrivateKey& key) noexcept
{
  if (key.kObjectType != CryptoObjectType::kPrivateKey)
  {
    aralog_.LogError() << "This crypto object does not belongs to private key";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  if (!(key.GetAllowedUsage() & kAllowDataDecryption))
  {
    aralog_.LogError() << "This key is not allowed to use for public decryption";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  privateKey_ = key.GetPrivateKey();

  isInitialize_ = true;

  return ara::core::Result<void>::FromValue();
}

ExtensionService::Uptr KeyDecapsulatorPrivateCtxDefaultSsl::GetExtensionService() const noexcept
{
  aralog_.LogDebug() << "KeyDecapsulatorPrivateCtxDefaultSsl::GetExtensionService()";

  CryptoObjectUid objectID;

  ExtensionService::Uptr localPtr{
      std::make_unique<ExtensionServiceImpl>(algId_, 2048, kAllowDataDecryption, objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<SecretSeed::Uptrc> KeyDecapsulatorPrivateCtxDefaultSsl::DecapsulateSeed(
    ReadOnlyMemRegion input, SecretSeed::Usage allowedUsage) const noexcept
{
  if (!isInitialize_)
  {
    aralog_.LogError() << "Context is not initialize yet!!!";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  ReadOnlyMemRegion encapsulatedseedValue{input.data(), 256};

  ara::core::Vector<uint8_t> plainText{DecryptCipherText(encapsulatedseedValue)};

  ReadWriteMemRegion seedValue{plainText};

  CryptoObjectUid objectID;
  SecretSeed::Uptrc seed{std::make_unique<SecretSeedImpl>(seedValue, allowedUsage,
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algorithmids::kPBKDF2),
      algorithmids::kPBKDF2, true, false, objectID)};

  return ara::core::Result<SecretSeed::Uptrc>::FromValue(std::move(seed));
}

ara::core::Result<SymmetricKey::Uptrc> KeyDecapsulatorPrivateCtxDefaultSsl::DecapsulateKey(
    ReadOnlyMemRegion input, KeyDerivationFunctionCtx& kdf, AlgId kekAlgId, ReadOnlyMemRegion salt,
    ReadOnlyMemRegion ctxLabel) const noexcept
{
  if (!isInitialize_)
  {
    aralog_.LogError() << "Context is not initialize yet!!!";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  ReadOnlyMemRegion entropy{input.data(), 256};

  ara::core::Vector<uint8_t> entropyValue{DecryptCipherText(entropy)};

  entropy = entropyValue;

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
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(initResult.Error());
  }

  SymmetricKey::Uptrc kek{kdf.DeriveKey(true, false).Value()};

  ReadOnlyMemRegion wrapeedKey{input.data() + 256, input.size() - 256};

  ara::core::Vector<uint8_t> output{UnWrapKey(*kek, wrapeedKey, kekAlgId)};

  ReadWriteMemRegion encapsulatedValue{output};

  SymmetricKey::Uptrc encapsulatedkey{std::make_unique<SymmetricKeyImpl>(encapsulatedValue,
      kAllowKeyImporting, AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(kAlgIdAny), kAlgIdAny,
      true, false, objectID)};

  return ara::core::Result<SymmetricKey::Uptrc>::FromValue(std::move(encapsulatedkey));
}

ara::core::Vector<uint8_t> KeyDecapsulatorPrivateCtxDefaultSsl::UnWrapKey(
    const SymmetricKey& kek, ReadOnlyMemRegion in, AlgId kekalgId) const noexcept
{
  int datalength{0};
  int finalLength{0};
  ara::core::Vector<uint8_t> plaintext;
  ReadWriteMemRegion kekKey{kek.GetSymmetricKey()};

  EVP_CIPHER_CTX* symmetricKeyWrapperctx = EVP_CIPHER_CTX_new();

  // According to SWS_CRYPT_24011 if algorithm does not support the padding then
  // the granularity will be of block size.
  if ((kekalgId == algorithmids::kKeyWrap128) || (kekalgId == algorithmids::kKeyWrap192)
      || (kekalgId == algorithmids::kKeyWrap256))
  {
    plaintext.resize(in.size() + keydecapsulation::kWRAPPADDING);
  }

  // According to SWS_CRYPT_24011 if algorithm supports the padding then
  // the granularity will be of 1 byte.
  if ((kekalgId == algorithmids::kKeyWrapPad128) || (kekalgId == algorithmids::kKeyWrapPad192)
      || (kekalgId == algorithmids::kKeyWrapPad256))
  {
    plaintext.resize(((in.size() / (keydecapsulation::KBlockSize + 1)) + 1) * keydecapsulation::KBlockSize);
  }

  if (1 != EVP_DecryptInit_ex(symmetricKeyWrapperctx, GetCipherMode(kekalgId), NULL, kekKey.data(), nullptr))
  {
    aralog_.LogError() << "Key wrap init Failed";
  }

  // Update data in openssl
  if (1 != EVP_DecryptUpdate(symmetricKeyWrapperctx, plaintext.data(), &datalength, in.data(), in.size()))
  {
    aralog_.LogError() << "Key wrap Update Failed";
  }

  // Store the length of cipher text for further calculation.
  finalLength = datalength;

  // Finalize the encryption process.
  if (1 != EVP_DecryptFinal_ex(symmetricKeyWrapperctx, plaintext.data() + datalength, &datalength))
  {
    aralog_.LogError() << "Key wrap Final Failed";
  }

  // Calculate the final length for encrypted data.
  finalLength += datalength;

  plaintext.resize(finalLength);

  EVP_CIPHER_CTX_free(symmetricKeyWrapperctx);

  return plaintext;
}

ara::core::Vector<uint8_t> KeyDecapsulatorPrivateCtxDefaultSsl::DecryptCipherText(ReadOnlyMemRegion in) const
    noexcept
{
  ara::core::Vector<uint8_t> plainText;
  size_t datalength{0};

  EVP_PKEY_CTX* keyCtx = EVP_PKEY_CTX_new(privateKey_, NULL);

  EVP_PKEY_decrypt_init(keyCtx);

  ConfigureDecryptorCtx(keyCtx);

  EVP_PKEY_decrypt(keyCtx, NULL, &datalength, in.data(), in.size());

  plainText.resize(datalength);

  EVP_PKEY_decrypt(keyCtx, plainText.data(), &datalength, in.data(), in.size());

  plainText.resize(datalength);

  return plainText;
}

void KeyDecapsulatorPrivateCtxDefaultSsl::ConfigureDecryptorCtx(EVP_PKEY_CTX* keyCtx) const noexcept
{
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

ara::core::Result<void> KeyDecapsulatorPrivateCtxDefaultSsl::Reset() noexcept
{
  privateKey_ = nullptr;
  isInitialize_ = false;
  return ara::core::Result<void>::FromValue();
}

const EVP_CIPHER* KeyDecapsulatorPrivateCtxDefaultSsl::GetCipherMode(AlgId algoId) const noexcept
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
