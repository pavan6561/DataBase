// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_key_wrapper_ctx_ssl.cpp
//
// Purpose     : This file contains implementation for SymmetricKeyWrapperCtxWrapSSL128 class.
// -----------------------------------------------------------------------------------------------------------

#include "symmetric_key_wrapper_ctx_ssl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for keywrapper.
namespace keywrapper
{
size_t constexpr KBlockSize{16};
size_t constexpr KBlockSizewithPad{1};
uint8_t constexpr kMaxPlainTextSize{128};
} // namespace keywrapper

SymmetricKeyWrapperCtxWrapDefaultSSL::SymmetricKeyWrapperCtxWrapDefaultSSL(AlgId algoId) noexcept :
    SymmetricKeyWrapperCtx{}, algorithmId_{algoId}
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL constructor";
  // Create openssl cipher context.
  symmetricKeyWrapperctx_ = EVP_CIPHER_CTX_new();
}

SymmetricKeyWrapperCtxWrapDefaultSSL::~SymmetricKeyWrapperCtxWrapDefaultSSL() noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL Destructor";
  // Delete openssl cipher context.
  EVP_CIPHER_CTX_free(symmetricKeyWrapperctx_);
}

ExtensionService::Uptr SymmetricKeyWrapperCtxWrapDefaultSSL::GetExtensionService() const noexcept
{
  aralog_.LogDebug() << "SymmetricKeyWrapperCtxWrapDefaultSSL::GetExtensionService()";
  uint8_t const kByteSizeinBits{8};

  CryptoObjectUid objectID;

  ExtensionService::Uptr localPtr{std::make_unique<ExtensionServiceImpl>(algorithmId_,
      GetKeySizefromAlgoId(algorithmId_) * kByteSizeinBits, (kAllowDataEncryption | kAllowDataDecryption),
      objectID, isCtxInitialized_)};

  return localPtr;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SymmetricKeyWrapperCtxWrapDefaultSSL::WrapKeyMaterial(
    const RestrictedUseObject& key) const noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::WrapKeyMaterial()";

  int32_t datalength{0};
  ara::core::Vector<uint8_t> ciphertext;

  // According to SWS_CRYPT_24014 if the context was not initialized by a key value
  // then WrapKeyMaterial() should return SecurityErrorDomain::kUninitializedContext.
  if (isCtxInitialized_ == false)
  {
    aralog_.LogError() << "Key Wrapper Context was not initialized";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // Calculate key size.
  size_t const keySize{(key.GetValue()).size()};

  // According to SWS_CRYPT_24014 if Granularity is of block size and key size is
  // not align with block boundary then WrapKeyMaterial()
  // should return SecurityErrorDomain::kInvalidInputSize
  if ((GetTargetKeyGranularity() == keywrapper::KBlockSize))
  {
    if ((keySize < keywrapper::KBlockSize) || ((keySize % (keywrapper::KBlockSize / 2)) != 0))
    {
      aralog_.LogError() << "Invalid input size";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
    }
    // According to SWS_CRYPT_24014 if Granularity is not block size and key size is
    // is less than 1 byte then WrapKeyMaterial() should return SecurityErrorDomain::kInvalidInputSize
  }
  else if (keySize < 1)
  {
    aralog_.LogError() << "Invalid input size";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }
  else
  {
    // No operation is required.
  }

  // Keep half size of block for extra bytes in output buffer for padding.
  ciphertext.resize(keySize + (keywrapper::KBlockSize / 2));

  // Initialize the openssl encryption process
  if (opresults::kSuccess
      != EVP_EncryptInit_ex(symmetricKeyWrapperctx_, GetCipherMode(), nullptr, kek_.data(), nullptr))
  {
    aralog_.LogError() << " Init Encryption Failed. ";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // Disable padding in openssl.
  if (opresults::kSuccess != EVP_CIPHER_CTX_set_padding(symmetricKeyWrapperctx_, 0))
  {
    aralog_.LogError() << "Padding can not be disabled!!!!";
  }

  // Update data in openssl
  if (opresults::kSuccess
      != EVP_EncryptUpdate(
             symmetricKeyWrapperctx_, ciphertext.data(), &datalength, (key.GetValue()).data(), keySize))
  {
    aralog_.LogError() << " Update Encryption Failed. ";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInsufficientCapacity);
  }

  // Finalize the encryption process.
  if (opresults::kSuccess
      != EVP_EncryptFinal_ex(symmetricKeyWrapperctx_, ciphertext.data() + datalength, &datalength))
  {
    aralog_.LogError() << " Encryption Final Failed. ";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInsufficientCapacity);
  }

  ara::core::Vector<ara::core::Byte> const cipherData{ciphertext.begin(), ciphertext.end()};

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>(cipherData);
}

ara::core::Result<SecretSeed::Uptrc> SymmetricKeyWrapperCtxWrapDefaultSSL::UnwrapSeed(
    ReadOnlyMemRegion wrappedSeed, AlgId targetAlgId, SecretSeed::Usage allowedUsage) const noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::UnwrapSeed()";

  int32_t datalength{0};
  uint8_t plaintext[keywrapper::kMaxPlainTextSize];
  bool isSession{true};
  bool isExportable{false};

  // According to SWS_CRYPT_24015 if the context was not initialized by a key value
  // then UnwrapSeed() should return SecurityErrorDomain::kUninitializedContext.
  if (isCtxInitialized_ == false)
  {
    aralog_.LogError() << "Key Wrapper Context was not initialized";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_24015 if Granularity is of block size and input size is
  // not align with block boundary then UnwrapSeed() should return SecurityErrorDomain::kInvalidInputSize
  if ((GetTargetKeyGranularity() == keywrapper::KBlockSize))
  {
    if ((wrappedSeed.size() < (keywrapper::KBlockSize + (keywrapper::KBlockSize / 2)))
        || ((wrappedSeed.size() % (keywrapper::KBlockSize / 2)) != 0))
    {
      aralog_.LogError() << "Invalid output size";
      return ara::core::Result<SecretSeed::Uptrc>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
    }
  }

  // According to SWS_CRYPT_24014 if Granularity is not block size and input size is
  // is less than block size then UnwrapSeed() should return SecurityErrorDomain::kInvalidInputSize
  else if (wrappedSeed.size() < keywrapper::KBlockSize)
  {
    aralog_.LogError() << "Invalid output size";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }
  else
  {
    // No operation is required.
  }

  // Initialize openssl decryption process.
  if (opresults::kSuccess
      != EVP_DecryptInit_ex(symmetricKeyWrapperctx_, GetCipherMode(), nullptr, kek_.data(), nullptr))
  {
    aralog_.LogError() << " Init Decrypt Failed. ";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // Update data for decryption in openssl.
  if (opresults::kSuccess
      != EVP_DecryptUpdate(
             symmetricKeyWrapperctx_, plaintext, &datalength, wrappedSeed.data(), wrappedSeed.size()))
  {
    aralog_.LogError() << " Decrypt Update Failed. ";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // Store the length of decrypted plain text for further calculation.
  int32_t finalLength{datalength};

  // Finalize the openssl decryption process.
  if (opresults::kSuccess
      != EVP_DecryptFinal_ex(symmetricKeyWrapperctx_, plaintext + datalength, &datalength))
  {
    aralog_.LogError() << " Decrypt Final Failed. ";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // Calculate the final length of the decrypted text.
  finalLength += datalength;

  ara::crypto::ReadWriteMemRegion seedArray(plaintext, finalLength);

  CryptoObjectUid objectID;

  // Create secret seed object.
  SecretSeed::Uptrc secretSeedPtr{std::make_unique<ara::crypto::cryp::SecretSeedImpl>(seedArray, allowedUsage,
      algoIdentification_->GetAlgoName(targetAlgId), targetAlgId, isSession, isExportable, objectID)};

  return ara::core::Result<SecretSeed::Uptrc>::FromValue(std::move(secretSeedPtr));
}

ara::core::Result<RestrictedUseObject::Uptrc> SymmetricKeyWrapperCtxWrapDefaultSSL::UnwrapKey(
    ReadOnlyMemRegion wrappedKey, AlgId algId, AllowedUsageFlags allowedUsage) const noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::UnwrapKey()";

  int32_t datalength{0};
  uint8_t plaintext[keywrapper::kMaxPlainTextSize];
  bool isSession{true};
  bool isExportable{false};

  // According to SWS_CRYPT_24016 if the context was not initialized by a key value
  // then UnwrapKey() should return SecurityErrorDomain::kUninitializedContext.
  if (isCtxInitialized_ == false)
  {
    aralog_.LogError() << "Key Wrapper Context was not initialized";
    return ara::core::Result<RestrictedUseObject::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_24016 if Granularity is of block size and input size is
  // not align with block boundary then UnwrapKey() should return SecurityErrorDomain::kInvalidInputSize
  if ((GetTargetKeyGranularity() == keywrapper::KBlockSize))
  {
    static uint8_t constexpr kPaddingApplyedByOpenssl{8};
    if ((wrappedKey.size() < (keywrapper::KBlockSize + kPaddingApplyedByOpenssl))
        || ((wrappedKey.size() % (keywrapper::KBlockSize / 2)) != 0))
    {
      aralog_.LogError() << "Invalid output size";
      return ara::core::Result<RestrictedUseObject::Uptrc>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
    }
  }
  // According to SWS_CRYPT_24016 if Granularity is not block size and input size is
  // is less than block size then UnwrapKey() should return SecurityErrorDomain::kInvalidInputSize
  else if (wrappedKey.size() < keywrapper::KBlockSize)
  {
    aralog_.LogError() << "Invalid output size";
    return ara::core::Result<RestrictedUseObject::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }
  else
  {
    // No operation is required.
  }

  // Initialize openssl decryption process.
  if (opresults::kSuccess
      != EVP_DecryptInit_ex(symmetricKeyWrapperctx_, GetCipherMode(), nullptr, kek_.data(), nullptr))
  {
    aralog_.LogError() << " Init Decrypt Failed. ";
    return ara::core::Result<RestrictedUseObject::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // Update data for decryption in openssl.
  if (opresults::kSuccess
      != EVP_DecryptUpdate(
             symmetricKeyWrapperctx_, plaintext, &datalength, wrappedKey.data(), wrappedKey.size()))
  {
    aralog_.LogError() << " Decrypt Update Failed. ";
    return ara::core::Result<RestrictedUseObject::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // Store the length of decrypted plain text for further calculation.
  int32_t finalLength{datalength};

  // Finalize the openssl decryption process.
  if (opresults::kSuccess
      != EVP_DecryptFinal_ex(symmetricKeyWrapperctx_, plaintext + datalength, &datalength))
  {
    aralog_.LogError() << " Decrypt Final Failed. ";
    return ara::core::Result<RestrictedUseObject::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // Calculate the final length of the decrypted text.
  finalLength += datalength;

  ara::crypto::ReadWriteMemRegion keyArray(plaintext, finalLength);

  CryptoObjectUid objectID;

  // Create restricted used object
  RestrictedUseObject::Uptrc restricteUsedPtr{std::make_unique<ara::crypto::cryp::SymmetricKeyImpl>(keyArray,
      allowedUsage, algoIdentification_->GetAlgoName(algId), algId, isSession, isExportable, objectID)};

  return ara::core::Result<RestrictedUseObject::Uptrc>::FromValue(std::move(restricteUsedPtr));
}

ara::core::Result<void> SymmetricKeyWrapperCtxWrapDefaultSSL::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::SetKey()";

  size_t const expectedKeySize{GetKeySizefromAlgoId(algorithmId_)};
  size_t const actualKeySize{key.GetPayloadSize()};

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_24018 if the provided key object is incompatible
  // with this symmetric key context, then SetKey() should return SecurityErrorDomain::kIncompatibleObject.
  if (((keyAlgId != algorithmId_) && (keyAlgId != kAlgIdAny)) || (expectedKeySize != actualKeySize))
  {
    aralog_.LogError() << "This crypto object does not belongs to symmetric key wrapper ctx";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // According to SWS_CRYPT_24018 if the transformation type associated with
  // this context is prohibited by the "allowed usage" restrictions of provided
  // key object then SetKey() should return SecurityErrorDomain::kUsageViolation.
  if (((transform == CryptoTransform::kEncrypt) && (!(setkeyusage & kAllowDataEncryption)))
      || ((transform == CryptoTransform::kDecrypt) && (!(setkeyusage & kAllowDataDecryption))))
  {
    aralog_.LogError() << "This key is not allowed to use for this transformation";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }
  ctxTransform_ = transform;
  ReadWriteMemRegion const symKey{key.GetValue()};
  kek_.resize(symKey.size());

  for (size_t index{0}; index < symKey.size(); index++)
  {
    kek_[index] = symKey[index];
  }

  isCtxInitialized_ = true;

  return ara::core::Result<void>();
}

ara::core::Result<void> SymmetricKeyWrapperCtxWrapDefaultSSL::Reset() noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL Reset()";

  kek_.clear();
  ctxTransform_ = CryptoTransform::kEncrypt;
  isCtxInitialized_ = false;
  if (opresults::kSuccess != EVP_CIPHER_CTX_reset(symmetricKeyWrapperctx_))
  {
    aralog_.LogError() << "Reset Operation failed!!!";
  }
  return ara::core::Result<void>::FromValue();
}

std::size_t SymmetricKeyWrapperCtxWrapDefaultSSL::GetTargetKeyGranularity() const noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::GetTargetKeyGranularity()";

  std::size_t granularity;

  // According to SWS_CRYPT_24011 if algorithm does not support the padding then
  // the granularity will be of block size.
  if ((algorithmId_ == algorithmids::kKeyWrap128) || (algorithmId_ == algorithmids::kKeyWrap192)
      || (algorithmId_ == algorithmids::kKeyWrap256))
  {
    granularity = keywrapper::KBlockSize;
  }

  // According to SWS_CRYPT_24011 if algorithm supports the padding then
  // the granularity will be of 1 byte.
  else if ((algorithmId_ == algorithmids::kKeyWrapPad128) || (algorithmId_ == algorithmids::kKeyWrapPad192)
      || (algorithmId_ == algorithmids::kKeyWrapPad256))
  {
    granularity = keywrapper::KBlockSizewithPad;
  }

  // use default granularity.
  else
  {
    granularity = keywrapper::KBlockSize;
  }

  return granularity;
}

std::size_t SymmetricKeyWrapperCtxWrapDefaultSSL::CalculateWrappedKeySize(std::size_t keyLength) const
    noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::CalculateWrappedKeySize()";

  keyLength /= (keywrapper::KBlockSize / 2);

  // if granularity is of 1 byte then according to openssl the wrapped key size
  // will be equal to key size + no of bytes used for padding to align the
  // output to the block size boundary.
  std::size_t wrapperdKeysize{(((keyLength / keywrapper::KBlockSize) + 1) * keywrapper::KBlockSize)};

  // If granularity is of block size then according to openssl the wrapped key size
  // will be 8 bytes more than the actual key size.
  if (GetTargetKeyGranularity() == keywrapper::KBlockSize)
  {
    wrapperdKeysize = (keyLength + (keywrapper::KBlockSize / 2));
  }

  return wrapperdKeysize;
}

std::size_t SymmetricKeyWrapperCtxWrapDefaultSSL::GetMaxTargetKeyLength() const noexcept
{
  static uint16_t constexpr kMaxTargetKeyLength{256};
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::GetMaxTargetKeyLength()";
  return kMaxTargetKeyLength;
}

EVP_CIPHER const* SymmetricKeyWrapperCtxWrapDefaultSSL::GetCipherMode() const noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::GetCipherMode()";

  EVP_CIPHER const* cipherMode;

  switch (algorithmId_)
  {
  case algorithmids::kKeyWrap128:
  {
    cipherMode = EVP_aes_128_wrap();
  }
  break;

  case algorithmids::kKeyWrap192:
  {
    cipherMode = EVP_aes_192_wrap();
  }
  break;

  case algorithmids::kKeyWrap256:
  {
    cipherMode = EVP_aes_256_wrap();
  }
  break;

  case algorithmids::kKeyWrapPad128:
  {
    cipherMode = EVP_aes_128_wrap_pad();
  }
  break;

  case algorithmids::kKeyWrapPad192:
  {
    cipherMode = EVP_aes_192_wrap_pad();
  }
  break;

  case algorithmids::kKeyWrapPad256:
  {
    cipherMode = EVP_aes_256_wrap_pad();
  }
  break;

  default:
  {
    cipherMode = EVP_aes_128_wrap();
  }
  break;
  }

  return cipherMode;
}

size_t SymmetricKeyWrapperCtxWrapDefaultSSL::GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyWrapperCtxWrapDefaultSSL::GetKeySizefromAlgoId()";

  static uint8_t const kKeyWrapperKeySize128Bit{16};
  static uint8_t const kKeyWrapperKeySize192Bit{24};
  static uint8_t const kKeyWrapperKeySize256Bit{32};

  size_t keySize;

  // If target algorithm Id requires 128 bit key.
  if ((targetAlgId == algorithmids::kKeyWrap128) || (targetAlgId == algorithmids::kKeyWrapPad128))
  {
    keySize = kKeyWrapperKeySize128Bit;
  }

  // If target algorithm Id requires 192 bit key.
  else if ((targetAlgId == algorithmids::kKeyWrap192) || (targetAlgId == algorithmids::kKeyWrapPad192))
  {
    keySize = kKeyWrapperKeySize192Bit;
  }

  // If target algorithm Id requires 256 bit key.
  else if ((targetAlgId == algorithmids::kKeyWrap256) || (targetAlgId == algorithmids::kKeyWrapPad256))
  {
    keySize = kKeyWrapperKeySize256Bit;
  }

  // else use default key size
  else
  {
    keySize = kKeyWrapperKeySize128Bit;
  }
  return keySize;
}

} // namespace cryp
} // namespace crypto
} // namespace ara