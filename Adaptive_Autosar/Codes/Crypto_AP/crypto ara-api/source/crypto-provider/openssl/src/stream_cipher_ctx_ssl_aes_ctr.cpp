// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : stream_cipher_ctx_ssl_aes_ctr.cpp
//
// Purpose     : This file contains implementation for StreamCipherCtxsslCtr class.
// -----------------------------------------------------------------------------------------------------------

#include "stream_cipher_ctx_ssl_aes_ctr.h"

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
/// @brief namespace for streamcipher.
namespace streamcipher
{
uint8_t constexpr kAesBlockSizeinBytes{16};
} // namespace streamcipher

StreamCipherCtxsslCtr::StreamCipherCtxsslCtr(CryptoPrimitiveId::AlgId algIdValue) noexcept :
    StreamCipherCtx{}, algId_{algIdValue}
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr constructor";

  keyValue_.resize(GetKeySizefromAlgoId(algId_));

  // Create openssl cipher context.
  symetricStreamCipherCtx_ = EVP_CIPHER_CTX_new();
}

StreamCipherCtxsslCtr::~StreamCipherCtxsslCtr() noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr Destructor";
  // Delete openssl cipher context.
  EVP_CIPHER_CTX_free(symetricStreamCipherCtx_);
}

BlockService::Uptr StreamCipherCtxsslCtr::GetBlockService() const noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr::GetBlockService()";

  CryptoObjectUid objectID;

  bool isKeyDeployed{false};

  uint8_t const kByteSizeinBits{8};

  if (scProcessState_ >= streamCipherExecutionStates::kScInitialize)
  {
    isKeyDeployed = true;
  }

  BlockService::Uptr localPtr{
      std::make_unique<BlockServiceImpl>(algId_, objectID, nullptr, keyValue_.size() * kByteSizeinBits,
          IvValue_.size() * kByteSizeinBits, kAllowDataEncryption | kAllowDataDecryption, isKeyDeployed)};

  return localPtr;
}

ara::core::Result<void> StreamCipherCtxsslCtr::Seek(std::int64_t offset, bool fromBegin) noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr Seek()";

  // According to SWS_CRYPT_23613 if the data processing was not started by a call to
  // Start() then Seek() should return SecurityErrorDomain::kProcessingNotStarted
  if (scProcessState_ < streamCipherExecutionStates::kScProcessingStarted)
  {
    aralog_.LogError() << "Stream cipher Processing is not started yet";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
  }

  // According to SWS_CRYPT_23613 if seek operation need to be performed from begin and offset is in negative
  // or if seek operation need to be performed from current position and offset goes below the lower boundary
  // of the block then Seek() should return SecurityErrorDomain::kBelowBoundary.
  if ((fromBegin && (offset < 0)) || ((static_cast<int64_t>(indexValue_) + offset) < 0))
  {
    aralog_.LogError() << "Offset is below boundary";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kBelowBoundary);
  }

  // According to SWS_CRYPT_23613 if offset is greater than block size or it is below the lower boundary of
  // the block or if offset from current position is going beyond the upper boundary of the block then Seek()
  // should return SecurityErrorDomain::kInvalidArgument.
  if ((offset > streamcipher::kAesBlockSizeinBytes)
      || (offset < -static_cast<int64_t>(streamcipher::kAesBlockSizeinBytes))
      || ((static_cast<int64_t>(indexValue_) + offset) > streamcipher::kAesBlockSizeinBytes))
  {
    aralog_.LogError() << "Geiven offset is invalid";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  // Set all the parameters for seek operation
  std::lock_guard<std::mutex> const lock{seekMutex_};
  seekOffset_ = offset;
  isFromBigin_ = fromBegin;
  currentindex_ = indexValue_;
  isseekActionRequired_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> StreamCipherCtxsslCtr::ProcessBlocks(
    ReadOnlyMemRegion in) noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr ProcessBlocks()";

  ara::core::Vector<uint8_t> output;
  int32_t outputlength{0};

  // According to SWS_CRYPT_23614 if the data processing was not started by a call to
  // Start(), ProcessBlocks() should return SecurityErrorDomain::kProcessingNotStarted.
  if (scProcessState_ < streamCipherExecutionStates::kScProcessingStarted)
  {
    aralog_.LogError() << "Stream cipher Processing is not started yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
  }

  // According to SWS_CRYPT_23614 if this method is called after processing of
  // non-aligned data then ProcessBlocks() should return SecurityErrorDomain::kInvalidUsageOrder
  if (scProcessState_ == streamCipherExecutionStates::kScProcessingFinished)
  {
    aralog_.LogError() << "Can not process this block as processing is finished";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidUsageOrder);
  }

  // According to SWS_CRYPT_23614 if size of the input buffer is not divisible by
  // the block size then ProcessBlocks() should return SecurityErrorDomain::kInvalidInputSize
  if (((in.size()) % streamcipher::kAesBlockSizeinBytes) != 0)
  {
    aralog_.LogError() << "Invalid input size";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  output.resize(in.size());

  if (transformationValue_ == CryptoTransform::kEncrypt)
  {
    // Update data for encryption.
    if (opresults::kSuccess
        != EVP_EncryptUpdate(symetricStreamCipherCtx_, output.data(), &outputlength, in.data(), in.size()))
    {
      aralog_.LogError() << "Block update failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
    }
  }
  else
  {
    // Update data for decryption.
    if (opresults::kSuccess
        != EVP_DecryptUpdate(symetricStreamCipherCtx_, output.data(), &outputlength, in.data(), in.size()))
    {
      aralog_.LogError() << "Block update failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
    }
  }

  // According to SWS_CRYPT_23614 if sizes of the input and output buffers
  // are not equal then ProcessBlocks() should return SecurityErrorDomain::kIncompatibleArguments
  if (static_cast<size_t>(outputlength) != in.size())
  {
    aralog_.LogError() << "Incompatible arguments";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kIncompatibleArguments);
  }

  ara::core::Vector<ara::core::Byte> const retVal{output.begin(), output.end()};

  aralog_.LogDebug() << "Block processed successfully";

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

ara::core::Result<void> StreamCipherCtxsslCtr::ProcessBlocks(ReadWriteMemRegion inOut) noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr ProcessBlocks() with inout parameter";

  ara::core::Vector<uint8_t> output;
  int32_t outputlength{0};

  // According to SWS_CRYPT_23615 if the data processing was not started by a call to
  // Start(), ProcessBlocks() should return SecurityErrorDomain::kProcessingNotStarted.
  if (scProcessState_ < streamCipherExecutionStates::kScProcessingStarted)
  {
    aralog_.LogError() << "Stream cipher Processing is not started yet";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
  }

  // According to SWS_CRYPT_23615 if this method is called after processing of
  // non-aligned data then ProcessBlocks() should return SecurityErrorDomain::kInvalidUsageOrder
  if (scProcessState_ == streamCipherExecutionStates::kScProcessingFinished)
  {
    aralog_.LogError() << "Can not process this block as processing is finished";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidUsageOrder);
  }

  // According to SWS_CRYPT_23615 if size of the input buffer is not divisible by
  // the block size then ProcessBlocks() should return SecurityErrorDomain::kInvalidInputSize
  if ((inOut.size() % streamcipher::kAesBlockSizeinBytes) != 0)
  {
    aralog_.LogError() << "Invalid input size";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  ReadOnlyMemRegion const in(inOut.data(), inOut.data() + inOut.size());
  output.resize(in.size());

  if (transformationValue_ == CryptoTransform::kEncrypt)
  {
    // Update data for encryption.
    if (opresults::kSuccess
        != EVP_EncryptUpdate(symetricStreamCipherCtx_, output.data(), &outputlength, in.data(), in.size()))
    {
      aralog_.LogError() << "Block update failed";
      return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
    }
  }
  else
  {
    // Update data for decryption.
    if (opresults::kSuccess
        != EVP_DecryptUpdate(symetricStreamCipherCtx_, output.data(), &outputlength, in.data(), in.size()))
    {
      aralog_.LogError() << "Block update failed";
      return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
    }
  }

  // According to SWS_CRYPT_23615 if sizes of the input and output buffers
  // are not equal then ProcessBlocks() should return SecurityErrorDomain::kIncompatibleArguments
  if (static_cast<size_t>(outputlength) != (in.size()))
  {
    aralog_.LogError() << "Incompatible arguments";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleArguments);
  }

  for (size_t index{0}; index < inOut.size(); index++)
  {
    inOut[index] = output[index];
  }

  aralog_.LogDebug() << "Block processed successfully";

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> StreamCipherCtxsslCtr::ProcessBytes(
    ReadOnlyMemRegion in) noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr ProcessBytes()";

  ara::core::Vector<uint8_t> output;
  int32_t outputlength{0};
  size_t startingPoint{0};

  // According to SWS_CRYPT_23616 if the data processing was not started by a call to
  // Start(), ProcessBytes() should return SecurityErrorDomain::kProcessingNotStarted.
  if (scProcessState_ < streamCipherExecutionStates::kScProcessingStarted)
  {
    aralog_.LogError() << "Stream cipher Processing is not started yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
  }

  output.resize(in.size() + 1);

  if (transformationValue_ == CryptoTransform::kEncrypt)
  {
    size_t outputIndex{0};
    size_t jumpLocation{0};

    // Adjust the index of the stream according to the offset given for seek operation.
    for (indexValue_ = 0; indexValue_ < in.size(); indexValue_++)
    {
      if (isseekActionRequired_)
      {
        std::lock_guard<std::mutex> const lock{seekMutex_};
        if (isFromBigin_)
        {
          indexValue_ = 0;
          indexValue_ += static_cast<size_t>(seekOffset_);
        }
        else
        {
          jumpLocation = 0;
          jumpLocation += seekOffset_;
        }
        isseekActionRequired_ = false;
      }

      // Update Data for encryption.
      if (opresults::kSuccess
          != EVP_EncryptUpdate(symetricStreamCipherCtx_, output.data() + outputIndex, &outputlength,
                 in.data() + indexValue_ + jumpLocation, sizeof(uint8_t)))
      {
        aralog_.LogError() << "Stream update failed";
        return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
            ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
      }

      outputIndex++;
    }

    // Store the output length for next operation.
    outputlength += outputIndex;
  }
  else
  {
    for (indexValue_ = 0; indexValue_ < in.size(); indexValue_++)
    {
      // Update Data for decryption.
      if (opresults::kSuccess
          != EVP_DecryptUpdate(symetricStreamCipherCtx_, output.data() + indexValue_, &outputlength,
                 in.data() + indexValue_, sizeof(uint8_t)))
      {
        aralog_.LogError() << "Stream update failed";
        return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
            ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
      }
    }

    // Store the output length for next operation.
    outputlength += indexValue_;

    // Set the starting point to the offset location if seek operation is required.
    if (isseekActionRequired_)
    {
      std::lock_guard<std::mutex> const lock{seekMutex_};
      if (!isFromBigin_)
      {
        startingPoint = currentindex_;
      }
      isseekActionRequired_ = false;

      startingPoint += seekOffset_;
    }
  }

  ara::core::Vector<ara::core::Byte> const retVal{output.begin() + startingPoint, output.end()};

  aralog_.LogDebug() << "Byetes processed successfully";

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> StreamCipherCtxsslCtr::FinishBytes(
    ReadOnlyMemRegion in) noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr FinishBytes()";

  ara::core::Vector<uint8_t> output;
  int32_t outputlength{0};
  size_t startingPoint{0};

  // According to SWS_CRYPT_23618 if the data processing was not started by a call to
  // Start(), FinishBytes() should return SecurityErrorDomain::kProcessingNotStarted.
  if (scProcessState_ < streamCipherExecutionStates::kScProcessingStarted)
  {
    aralog_.LogError() << "Stream cipher Processing is not started yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
  }

  output.resize(in.size());

  if (transformationValue_ == CryptoTransform::kEncrypt)
  {
    size_t outputindex{0};
    size_t jumpLocation{0};
    // Adjust the index of the stream according to the offset given for seek operation.
    for (indexValue_ = 0; indexValue_ < in.size(); indexValue_++)
    {
      // If Seek action is required them adjust the index according to the offset given
      // for the seek operation.
      if (isseekActionRequired_)
      {
        std::lock_guard<std::mutex> const lock{seekMutex_};

        if (isFromBigin_)
        {
          indexValue_ = 0;
          indexValue_ += static_cast<size_t>(seekOffset_);
        }
        else
        {
          jumpLocation = 0;
          jumpLocation += seekOffset_;
        }

        isseekActionRequired_ = false;
      }

      // Update data for encryption.
      if (opresults::kSuccess
          != EVP_EncryptUpdate(symetricStreamCipherCtx_, output.data() + outputindex, &outputlength,
                 in.data() + indexValue_ + jumpLocation, 1))
      {
        aralog_.LogError() << "Stream update failed";
        return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
            ara::crypto::SecurityErrorDomain::Errc::kInsufficientCapacity);
      }

      outputindex++;
    }

    // Finalize the encryption operation.
    if (opresults::kSuccess
        != EVP_EncryptFinal_ex(symetricStreamCipherCtx_, output.data() + outputindex, &outputlength))
    {
      aralog_.LogError() << "Stream final failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kInsufficientCapacity);
    }

    outputlength += outputindex;
  }
  else
  {
    for (indexValue_ = 0; indexValue_ < (in.size() + 1); indexValue_++)
    {
      // Update data for decryption.
      if (opresults::kSuccess
          != EVP_DecryptUpdate(symetricStreamCipherCtx_, output.data() + indexValue_, &outputlength,
                 in.data() + indexValue_, 1))
      {
        aralog_.LogError() << "Stream update failed";
        return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
            ara::crypto::SecurityErrorDomain::Errc::kInsufficientCapacity);
      }
    }

    // Finalize the decryption operation.
    if (opresults::kSuccess
        != EVP_DecryptFinal_ex(symetricStreamCipherCtx_, output.data() + indexValue_, &outputlength))
    {
      aralog_.LogError() << "Stream final failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kInsufficientCapacity);
    }

    outputlength += indexValue_;

    // Set the starting point to the offset location if seek operation is required.
    if (isseekActionRequired_)
    {
      std::lock_guard<std::mutex> const lock{seekMutex_};

      if (!isFromBigin_)
      {
        startingPoint = currentindex_;
      }
      isseekActionRequired_ = false;
      startingPoint += seekOffset_;
    }
  }

  ara::core::Vector<ara::core::Byte> const retVal{output.begin() + startingPoint, output.end()};
  scProcessState_ = streamCipherExecutionStates::kScProcessingFinished;
  aralog_.LogDebug() << "Prossesing finished successfully";

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

std::size_t StreamCipherCtxsslCtr::CountBytesInCache() const noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr CountBytesInCache()";
  return 0;
}

ara::core::Result<void> StreamCipherCtxsslCtr::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr SetKey()";

  size_t const expectedKeySize{GetKeySizefromAlgoId(algId_)};
  size_t const actualKeySize{key.GetPayloadSize()};

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_23623 if the provided key object is incompatible
  // with this symmetric key context then SetKey should return SecurityErrorDomain::kIncompatibleObject
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)) || (expectedKeySize != actualKeySize))
  {
    aralog_.LogError() << "This crypto object does not belongs to stream cipher ctx.";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // According to SWS_CRYPT_23623 if the transformation type associated with this context
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

  for (size_t index{0}; index < GetKeySizefromAlgoId(algId_); index++)
  {
    keyValue_[index] = symKeyValue[index];
  }

  transformationValue_ = transform;
  scProcessState_ = streamCipherExecutionStates::kScInitialize;
  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> StreamCipherCtxsslCtr::Start(ReadOnlyMemRegion iv) noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr Start()";

  // According to SWS_CRYPT_23625 if the context was not initialized by deploying a key
  // then Start() should return SecurityErrorDomain::kUninitializedContext
  if (scProcessState_ < streamCipherExecutionStates::kScInitialize)
  {
    aralog_.LogError() << "Ctx is not initialize";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_23625 if the size of provided IV is not supported
  // then Start() should return SecurityErrorDomain::kInvalidInputSize
  if (iv.size() != streamcipher::kAesBlockSizeinBytes)
  {
    aralog_.LogError() << "Invalid IV size";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // Store IV value from the symmetric key. So that even if ReadOnlyMemRegion object get destroyed it will
  // not impact the functionality of the stream cipher ctx.
  IvValue_.resize(iv.size());

  for (size_t index{0}; index < iv.size(); index++)
  {
    IvValue_[index] = iv[index];
  }

  if (transformationValue_ == CryptoTransform::kEncrypt)
  {
    // Initialize encrypt context for openssl.
    if (opresults::kSuccess
        != EVP_EncryptInit_ex(
               symetricStreamCipherCtx_, GetCipherMode(), nullptr, keyValue_.data(), IvValue_.data()))
    {
      aralog_.LogError() << "Encrypt Initialization failed!!!";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
  else
  {
    // Initialize decrypt context for openssl.
    if (opresults::kSuccess
        != EVP_DecryptInit_ex(
               symetricStreamCipherCtx_, GetCipherMode(), nullptr, keyValue_.data(), IvValue_.data()))
    {
      aralog_.LogError() << "Decrypt Initialization failed!!!";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }

  scProcessState_ = streamCipherExecutionStates::kScProcessingStarted;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> StreamCipherCtxsslCtr::Start(const SecretSeed& iv) noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr Start() with Secret Seed";

  AllowedUsageFlags const ivUsage{iv.GetAllowedUsage()};

  // According to SWS_CRYPT_23626 if this transformation type is prohibited by
  // the "allowed usage" restrictions of the provided SecretSeed object
  // then Start() should return SecurityErrorDomain::kUsageViolation.
  if (((transformationValue_ == CryptoTransform::kEncrypt) && (!(ivUsage & kAllowDataEncryption)))
      || ((transformationValue_ == CryptoTransform::kDecrypt) && (!(ivUsage & kAllowDataDecryption))))
  {
    aralog_.LogError() << "This IV is not allowed to use for this transformation";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  return Start(static_cast<ReadOnlyMemRegion>(iv.GetSeedValue()));
}

ara::core::Result<void> StreamCipherCtxsslCtr::Reset() noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr Reset()";

  scProcessState_ = streamCipherExecutionStates::kScIdle;
  IvValue_.clear();
  keyValue_.clear();
  transformationValue_ = CryptoTransform::kEncrypt;
  std::lock_guard<std::mutex> const lock{seekMutex_};
  indexValue_ = 0;
  currentindex_ = 0;
  isseekActionRequired_ = 0;
  isFromBigin_ = false;
  seekOffset_ = 0;
  // Reset the openssl cipher context.
  if (opresults::kSuccess != EVP_CIPHER_CTX_reset(symetricStreamCipherCtx_))
  {
    aralog_.LogError() << "Reset Operation failed!!!";
  }
  return ara::core::Result<void>::FromValue();
}

CryptoPrimitiveId::Uptr StreamCipherCtxsslCtr::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr::GetCryptoPrimitiveId()";
  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr{std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_)};
  return primitiveInfoPtr;
}

EVP_CIPHER const* StreamCipherCtxsslCtr::GetCipherMode() const noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr::GetCipherMode()";

  EVP_CIPHER const* cipherMode;

  // If algorithm Id is AES128CTR.
  if (algId_ == algorithmids::kAES128CTR)
  {
    cipherMode = EVP_aes_128_ctr();
  }
  // If algorithm Id is AES192CTR.
  else if (algId_ == algorithmids::kAES192CTR)
  {
    cipherMode = EVP_aes_192_ctr();
  }
  // If algorithm Id is AES256CTR.
  else if (algId_ == algorithmids::kAES256CTR)
  {
    cipherMode = EVP_aes_256_ctr();
  }
  // else use default mode.
  else
  {
    cipherMode = EVP_aes_128_ctr();
  }
  return cipherMode;
}

size_t StreamCipherCtxsslCtr::GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept
{
  aralog_.LogDebug() << "In StreamCipherCtxsslCtr::GetKeySizefromAlgoId()";

  static size_t const kStreamCipherKeySize128Bit{16};
  static size_t const kStreamCipherKeySize192Bit{24};
  static size_t const kStreamCipherKeySize256Bit{32};

  size_t keySize;

  // If target algorithm Id requires 128 bit key.
  if (targetAlgId == algorithmids::kAES128CTR)
  {
    keySize = kStreamCipherKeySize128Bit;
  }
  // If target algorithm Id requires 192 bit key.
  else if (targetAlgId == algorithmids::kAES192CTR)
  {
    keySize = kStreamCipherKeySize192Bit;
  }
  // If target algorithm Id requires 256 bit key.
  else if (targetAlgId == algorithmids::kAES256CTR)
  {
    keySize = kStreamCipherKeySize256Bit;
  }
  // else use default size.
  else
  {
    keySize = kStreamCipherKeySize128Bit;
  }

  return keySize;
}

} // namespace cryp
} // namespace crypto
} // namespace ara