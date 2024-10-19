// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_block_cipher_ctx_ssl_aes.cpp
//
// Purpose     : This file contains implementation for SymmetricBlockCipherCtxsslAES class.
// -----------------------------------------------------------------------------------------------------------

#include "symmetric_block_cipher_ctx_ssl_aes.h"

#include <ara/core/string.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for blockcipher.
namespace blockcipher
{
uint8_t constexpr kblockSizeInBytes{16};
} // namespace blockcipher

SymmetricBlockCipherCtxsslAES::SymmetricBlockCipherCtxsslAES(CryptoPrimitiveId::AlgId algIdValue) noexcept :
    SymmetricBlockCipherCtx{}, algId_{algIdValue}, maxInputSize_{blockcipher::kblockSizeInBytes - 1}
{
  aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES constructor";

  // Create openssl cipher context.
  symetricBlockCipherCtx_ = EVP_CIPHER_CTX_new();

  keyValue_.resize(GetKeySizefromAlgoId(algId_));
}

SymmetricBlockCipherCtxsslAES::~SymmetricBlockCipherCtxsslAES() noexcept
{
  aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES Destructor";
  // Delete openssl cipher context.
  EVP_CIPHER_CTX_free(symetricBlockCipherCtx_);
}

CryptoService::Uptr SymmetricBlockCipherCtxsslAES::GetCryptoService() const noexcept
{
  aralog_.LogDebug() << "In EncryptorPublicCtxsslRsa::GetCryptoService()";
  CryptoObjectUid objectID;
  uint8_t const kByteSizeinBits{8};

  CryptoService::Uptr localPtr{std::make_unique<CryptoServiceImpl>(algId_, keyValue_.size() * kByteSizeinBits,
      kAllowDataEncryption | kAllowDataDecryption, objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<void> SymmetricBlockCipherCtxsslAES::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES::SetKey()";

  size_t const expectedKeySize{GetKeySizefromAlgoId(algId_)};
  size_t const actualKeySize{key.GetPayloadSize()};

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_23710 if the provided key object is incompatible
  // with this symmetric key context then SetKey should return SecurityErrorDomain::kIncompatibleObject
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)) || (expectedKeySize > actualKeySize))
  {
    aralog_.LogError() << "This crypto object does not belongs to symmetric block cipher ctx.";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // According to SWS_CRYPT_23710 if the transformation type associated with this context
  // is prohibited by the "allowed usage" restrictions of provided key object then
  // SetKey should return SecurityErrorDomain::kUsageViolation
  if (((transform == CryptoTransform::kEncrypt) && (!(setkeyusage & kAllowDataEncryption)))
      || ((transform == CryptoTransform::kDecrypt) && (!(setkeyusage & kAllowDataDecryption))))
  {
    aralog_.LogError() << "This key is not allowed to use for this transformation";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  // Store Key value from the symmetric key. So that even if symmetric key object get destroyed it will
  // not impact the functionality of the stream cipher ctx.
  ReadWriteMemRegion const symKeyValue{key.GetSymmetricKey()};

  for (size_t index{0}; index < symKeyValue.size(); index++)
  {
    keyValue_[index] = symKeyValue[index];
  }
  transformationValue_ = transform;

  // if decryption is to be performed it can be done on 16 bytes.
  if (transformationValue_ == CryptoTransform::kDecrypt)
  {
    maxInputSize_ = blockcipher::kblockSizeInBytes;
  }

  isInitialize_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> SymmetricBlockCipherCtxsslAES::Reset() noexcept
{
  aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES::Reset()";

  isInitialize_ = false;
  keyValue_.clear();
  transformationValue_ = CryptoTransform::kEncrypt;
  maxInputSize_ = blockcipher::kblockSizeInBytes - 1;

  // Reset the openssl cipher context.
  if (opresults::kSuccess != EVP_CIPHER_CTX_reset(symetricBlockCipherCtx_))
  {
    aralog_.LogError() << "Reset Operation Failed!!!!";
  }

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SymmetricBlockCipherCtxsslAES::ProcessBlocks(
    ReadOnlyMemRegion in) const noexcept
{
  aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES::ProcessBlocks()";

  ara::core::Vector<uint8_t> result;
  int32_t datalength{0};
  // Calculate number of blocks required for the input.
  std::uint32_t const kTotalNoOfBlocks{in.size() / blockcipher::kblockSizeInBytes};

  // According to SWS_CRYPT_23715 if the context was not initialized by a key value
  // then ProcessBlock() should return SecurityErrorDomain::kUninitializedContext.
  if (isInitialize_ == false)
  {
    aralog_.LogError() << "Context is not initialized yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  aralog_.LogDebug() << "The input size is " << in.size();

  // According to If SWS_CRYPT_23715 input size is not aligned with block boundary
  // and then return SecurityErrorDomain::kIncorrectInputSize.
  if ((in.size() % blockcipher::kblockSizeInBytes) != 0)
  {
    aralog_.LogError() << "Invalid input size";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  aralog_.LogDebug() << "Total no of blocks are " << kTotalNoOfBlocks;

  result.resize(kTotalNoOfBlocks * blockcipher::kblockSizeInBytes);

  if (transformationValue_ == CryptoTransform::kDecrypt)
  {
    // Initialize decryption process in openssl.
    if (opresults::kSuccess
        != EVP_DecryptInit_ex(symetricBlockCipherCtx_, GetCipherMode(), nullptr, keyValue_.data(), nullptr))
    {
      aralog_.LogError() << "Init Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Disable the the padding.
    if (opresults::kSuccess != EVP_CIPHER_CTX_set_padding(symetricBlockCipherCtx_, 0))
    {
      aralog_.LogError() << "Supressing of padding is failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Update data for decryption in openssl.
    if (opresults::kSuccess
        != EVP_DecryptUpdate(symetricBlockCipherCtx_, result.data(), &datalength, in.data(), in.size()))
    {
      aralog_.LogError() << "Update Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Finalize the decryption process.
    if (opresults::kSuccess
        != EVP_DecryptFinal_ex(symetricBlockCipherCtx_, result.data() + datalength, &datalength))
    {
      aralog_.LogError() << "Final Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
  else
  {
    // Initialize encryption process in openssl.
    if (opresults::kSuccess
        != EVP_EncryptInit_ex(symetricBlockCipherCtx_, GetCipherMode(), nullptr, keyValue_.data(), nullptr))
    {
      aralog_.LogError() << "Init Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Disable the the padding.
    if (opresults::kSuccess != EVP_CIPHER_CTX_set_padding(symetricBlockCipherCtx_, 0))
    {
      aralog_.LogError() << "Supressing of padding is failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Update data for decryption in openssl.
    if (opresults::kSuccess
        != EVP_EncryptUpdate(symetricBlockCipherCtx_, result.data(), &datalength, in.data(), in.size()))
    {
      aralog_.LogError() << "Update Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Finalize the decryption process.
    if (opresults::kSuccess
        != EVP_EncryptFinal_ex(symetricBlockCipherCtx_, result.data() + datalength, &datalength))
    {
      aralog_.LogError() << "Final Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }

  ara::core::Vector<ara::core::Byte> const output{result.begin(), result.end()};
  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(output);
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> SymmetricBlockCipherCtxsslAES::ProcessBlock(
    ReadOnlyMemRegion in, bool suppressPadding) const noexcept
{
  aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES::ProcessBlock()";

  ara::core::Vector<uint8_t> output;
  int32_t datalength{0};

  // According to SWS_CRYPT_23716 if the context was not initialized by a key value
  // then ProcessBlock() should return SecurityErrorDomain::kUninitializedContext.
  if (isInitialize_ == false)
  {
    aralog_.LogError() << "Context is not initialized yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // Here maximum input size is a block size, which is 128 bits ( 16 Byte ) in case of AES.
  // For encryption block size is limited at 120 bits ( 15 bytes ) and 8 bits are kept for padding.
  // For decryption block size is 128 bits ( 16 bytes ).

  // According to If SWS_CRYPT_23716 input size is not aligned with block boundary
  // and padding is suppressed then return SecurityErrorDomain::kIncorrectInputSize.
  if (((in.size() != maxInputSize_) && (suppressPadding == true))
      || (((in.size()) > maxInputSize_) && (suppressPadding == false)))
  {
    aralog_.LogError() << "Invalid input size";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // Keep 2 bytes extra in output buffer to avoid overflow because of padding.
  output.resize(blockcipher::kblockSizeInBytes);

  if (transformationValue_ == CryptoTransform::kDecrypt)
  {
    uint32_t finalLength;

    // Initialize decryption process in openssl.
    if (opresults::kSuccess
        != EVP_DecryptInit_ex(symetricBlockCipherCtx_, GetCipherMode(), nullptr, keyValue_.data(), nullptr))
    {
      aralog_.LogError() << "Init Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Update data for decryption in openssl.
    if (opresults::kSuccess
        != EVP_DecryptUpdate(symetricBlockCipherCtx_, output.data(), &datalength, in.data(), in.size()))
    {
      aralog_.LogError() << "Update Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    finalLength = datalength;

    // Finalize the decryption process.
    if (opresults::kSuccess
        != EVP_DecryptFinal_ex(symetricBlockCipherCtx_, output.data() + datalength, &datalength))
    {
      aralog_.LogError() << "Final Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    finalLength += datalength;

    output.resize(finalLength);
  }
  else
  {
    // Initialize encryption process in openssl.
    if (opresults::kSuccess
        != EVP_EncryptInit_ex(symetricBlockCipherCtx_, GetCipherMode(), nullptr, keyValue_.data(), nullptr))
    {
      aralog_.LogError() << "Init Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Update data for decryption in openssl.
    if (opresults::kSuccess
        != EVP_EncryptUpdate(symetricBlockCipherCtx_, output.data(), &datalength, in.data(), in.size()))
    {
      aralog_.LogError() << "Update Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Finalize the decryption process.
    if (opresults::kSuccess
        != EVP_EncryptFinal_ex(symetricBlockCipherCtx_, output.data() + datalength, &datalength))
    {
      aralog_.LogError() << "Final Failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }

  ara::core::Vector<ara::core::Byte> const retVal{output.begin(), output.end()};

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

CryptoPrimitiveId::Uptr SymmetricBlockCipherCtxsslAES::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES::GetCryptoPrimitiveId()";

  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr{std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_)};
  return primitiveInfoPtr;
}

EVP_CIPHER const* SymmetricBlockCipherCtxsslAES::GetCipherMode() const noexcept
{
  aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES::GetCipherMode()";

  EVP_CIPHER const* cipherMode;

  switch (algId_)
  {
  case algorithmids::kAES128CBC:
  {
    cipherMode = EVP_aes_128_cbc();
  }
  break;

  case algorithmids::kAES192CBC:
  {
    cipherMode = EVP_aes_192_cbc();
  }
  break;

  case algorithmids::kAES256CBC:
  {
    cipherMode = EVP_aes_256_cbc();
  }
  break;

  case algorithmids::kAES128ECB:
  {
    cipherMode = EVP_aes_128_ecb();
  }
  break;

  case algorithmids::kAES192ECB:
  {
    cipherMode = EVP_aes_192_ecb();
  }
  break;

  case algorithmids::kAES256ECB:
  {
    cipherMode = EVP_aes_256_ecb();
  }
  break;

  default:
  {
    cipherMode = EVP_aes_128_cbc();
  }
  break;
  }
  return cipherMode;
}

size_t SymmetricBlockCipherCtxsslAES::GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept
{
  aralog_.LogDebug() << "In SymmetricBlockCipherCtxsslAES::GetKeySizefromAlgoId()";

  static uint8_t const kBlockCipherKeySize128Bit{16};
  static uint8_t const kBlockCipherKeySize192Bit{24};
  static uint8_t const kBlockCipherKeySize256Bit{32};

  size_t keySize{kBlockCipherKeySize128Bit};

  if ((targetAlgId == algorithmids::kAES128CBC) || (targetAlgId == algorithmids::kAES128ECB))
  {
    keySize = kBlockCipherKeySize128Bit;
  }

  // If target algorithm Id requires 192 bit key.
  if ((targetAlgId == algorithmids::kAES192CBC) || (targetAlgId == algorithmids::kAES192ECB))
  {
    keySize = kBlockCipherKeySize192Bit;
  }

  // If target algorithm Id requires 256 bit key.
  if ((targetAlgId == algorithmids::kAES256CBC) || (targetAlgId == algorithmids::kAES256ECB))
  {
    keySize = kBlockCipherKeySize256Bit;
  }

  return keySize;
}

} // namespace cryp
} // namespace crypto
} // namespace ara