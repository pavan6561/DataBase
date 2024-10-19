// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : auth_cipher_ctx_ssl_aes_gcm.cpp
//
// Purpose     : This file contains implementation for AuthCipherCtxSslAesGCM class.
// -----------------------------------------------------------------------------------------------------------

#include "auth_cipher_ctx_ssl_aes_gcm.h"

#include <ara/core/string.h>

#include "ara/crypto/common/mem_region.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for authcipher.
namespace authcipher
{
uint8_t constexpr kIVlengthForGCM{12};
uint8_t constexpr kAesBlockSizeInbytes{16};
} // namespace authcipher

AuthCipherCtxSslAesGCM::AuthCipherCtxSslAesGCM(CryptoPrimitiveId::AlgId algIdValue) noexcept :
    AuthCipherCtx{}, algId_{algIdValue}
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM constructor";

  keyValue_.resize(GetKeySizefromAlgoId(algId_));
  IvValue_.resize(authcipher::kIVlengthForGCM);

  // Create new openssl cipher context.
  symmetricAuthenticationCipherCtx_ = EVP_CIPHER_CTX_new();
}

AuthCipherCtxSslAesGCM::~AuthCipherCtxSslAesGCM() noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM Destructor";

  // Delete openssl cipher context.
  EVP_CIPHER_CTX_free(symmetricAuthenticationCipherCtx_);
}

BlockService::Uptr AuthCipherCtxSslAesGCM::GetBlockService() const noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::GetBlockService()";
  uint8_t const kByteSizeinBits{8};

  CryptoObjectUid objectID;

  bool isKeyDeployed{false};

  if (acProcessState_ >= authenticationCipherExecutionStates::kAcInitialize)
  {
    isKeyDeployed = true;
  }

  BlockService::Uptr localPtr{
      std::make_unique<BlockServiceImpl>(algId_, objectID, nullptr, keyValue_.size() * kByteSizeinBits,
          IvValue_.size() * kByteSizeinBits, kAllowDataEncryption | kAllowDataDecryption, isKeyDeployed)};

  return localPtr;
}

ara::core::Result<void> AuthCipherCtxSslAesGCM::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::SetKey()";

  size_t const expectedKeySize{GetKeySizefromAlgoId(algId_)};
  size_t const actualKeySize{key.GetPayloadSize()};

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_23911 if the provided key object is incompatible
  // with this symmetric key context then SetKey() should return SecurityErrorDomain::kIncompatibleObject.
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)) || (expectedKeySize != actualKeySize))
  {
    aralog_.LogError() << "This crypto object does not belongs to authentication cipher symmetric key";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // According to SWS_CRYPT_23911 & SWS_CRYPT_01800 if the transformation type associated
  // with this context is prohibited by the "allowed usage" restrictions of provided key
  // object then SetKey() should return SecurityErrorDomain::kUsageViolation
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

  allowedKeyUsage_ = key.GetAllowedUsage();

  acProcessState_ = authenticationCipherExecutionStates::kAcInitialize;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> AuthCipherCtxSslAesGCM::Start(ReadOnlyMemRegion iv) noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM Start()";

  // According to SWS_CRYPT_24714 & SWS_CRYPT_01808 if the context was not
  // initialized by deploying a key then Start() should return SecurityErrorDomain::kUninitializedContext
  if (acProcessState_ < authenticationCipherExecutionStates::kAcInitialize)
  {
    aralog_.LogError() << "Authentication Cipher Ctx is not initialize";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_24714 & SWS_CRYPT_01808 if the size of provided
  // IV is not supported then Start() should return SecurityErrorDomain::kInvalidInputSize
  if (iv.size() < authcipher::kIVlengthForGCM)
  {
    aralog_.LogError() << "Invalid IV size";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  for (size_t index{0}; index < authcipher::kIVlengthForGCM; index++)
  {
    IvValue_[index] = iv[index];
  }

  if (transformationValue_ == CryptoTransform::kEncrypt)
  {
    // Initialize encryption with given cipher ctx and cipher mode.
    if (opresults::kSuccess
        != EVP_EncryptInit_ex(symmetricAuthenticationCipherCtx_, GetCipherMode(), nullptr, nullptr, nullptr))
    {
      aralog_.LogError() << "Encrypt Initialization failed!!!";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Set IV length for GCM mode.
    if (opresults::kSuccess
        != EVP_CIPHER_CTX_ctrl(symmetricAuthenticationCipherCtx_, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr))
    {
      aralog_.LogError() << "IV length set operation failed!!!";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Initialize encryption with key value and IV value.
    if (opresults::kSuccess
        != EVP_EncryptInit_ex(
               symmetricAuthenticationCipherCtx_, nullptr, nullptr, keyValue_.data(), IvValue_.data()))
    {
      aralog_.LogError() << "Encrypt Initialization failed after setting IV length!!!";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
  else
  {
    // Initialize decryption with given cipher ctx and cipher mode.
    if (opresults::kSuccess
        != EVP_DecryptInit_ex(symmetricAuthenticationCipherCtx_, GetCipherMode(), nullptr, nullptr, nullptr))
    {
      aralog_.LogError() << "Decrypt Initialization failed!!!";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Set IV length for GCM mode.
    if (opresults::kSuccess
        != EVP_CIPHER_CTX_ctrl(symmetricAuthenticationCipherCtx_, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr))
    {
      aralog_.LogError() << "IV length set operation failed!!!";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Initialize decryption with key value and IV value.
    if (opresults::kSuccess
        != EVP_DecryptInit_ex(
               symmetricAuthenticationCipherCtx_, nullptr, nullptr, keyValue_.data(), IvValue_.data()))
    {
      aralog_.LogError() << "Decrypt Initialization failed after setting IV length!!!";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }

  acProcessState_ = authenticationCipherExecutionStates::kAcProcessingStarted;
  aralog_.LogDebug() << "Process Started successfully";

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> AuthCipherCtxSslAesGCM::Start(const SecretSeed& iv) noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::Start() with secret seed";

  AllowedUsageFlags const setkeyusage{iv.GetAllowedUsage()};

  // According to SWS_CRYPT_24715  & SWS_CRYPT_01808 if this transformation
  // type is prohibited by the "allowed usage" restrictions of the provided
  // SecretSeed object then Start() should return SecurityErrorDomain::
  // kUsageViolation.
  if (((transformationValue_ == CryptoTransform::kEncrypt) && (!(setkeyusage & kAllowDataEncryption)))
      || ((transformationValue_ == CryptoTransform::kDecrypt) && (!(setkeyusage & kAllowDataDecryption))))
  {
    aralog_.LogError() << "This IV is not allowed to use for this transformation";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  return Start(static_cast<ReadOnlyMemRegion>(iv.GetSeedValue()));
}

ara::core::Result<void> AuthCipherCtxSslAesGCM::UpdateAssociatedData(const RestrictedUseObject& in) noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::UpdateAssociatedData() with restricted used object";

  // Design decision taken on 07/05/2021 that if UpdateAssociatedData is called then
  // for authentication encryption, mac will be calculated with associated data. if this method
  // is not called then for authentication encryption, mac will be calculated based on confidential
  // data.
  return UpdateAssociatedData(static_cast<ReadOnlyMemRegion>(in.GetValue()));
}

ara::core::Result<void> AuthCipherCtxSslAesGCM::UpdateAssociatedData(ReadOnlyMemRegion in) noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::UpdateAssociatedData()";

  // Design decision taken on 07/05/2021 that if UpdateAssociatedData is called then
  // for authentication encryption, mac will be calculated with associated data. if this method
  // is not called then for authentication encryption, mac will be calculated based on confidential
  // data.

  // According to SWS_CRYPT_20312 if the digest calculation was not initiated by a call
  // to Start(), then UpdateAssociatedData() should return SecurityErrorDomain::kProcessingNotStarted
  if (acProcessState_ < authenticationCipherExecutionStates::kAcProcessingStarted)
  {
    aralog_.LogError() << "Processing is not started yet";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
  }

  // According to SWS_CRYPT_20312 & SWS_CRYPT_01800 if ProcessConfidentialData has already
  // been called then UpdateAssociatedData() should return SecurityErrorDomain::kInvalidUsageOrder.
  if (acProcessState_ >= authenticationCipherExecutionStates::kAcProcessingFinished)
  {
    aralog_.LogError() << "Assosiated data is already updated";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidUsageOrder);
  }

  int32_t outputlen{0};

  if (transformationValue_ == CryptoTransform::kEncrypt)
  {
    // Update associated data in openssl encrypt context.
    if (opresults::kSuccess
        != EVP_EncryptUpdate(symmetricAuthenticationCipherCtx_, nullptr, &outputlen, in.data(), in.size()))
    {
      aralog_.LogError() << "Updation of assosiated data is failed";

      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
  }
  else
  {
    // Update associated data in openssl decrypt context.
    if (opresults::kSuccess
        != EVP_DecryptUpdate(symmetricAuthenticationCipherCtx_, nullptr, &outputlen, in.data(), in.size()))
    {
      aralog_.LogError() << "Updation of assosiated data is failed";

      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }
  }

  aralog_.LogDebug() << "Assosiated dataupdated successfully";

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> AuthCipherCtxSslAesGCM::UpdateAssociatedData(std::uint8_t in) noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::UpdateAssociatedData() with uint8_t";

  // Design decision taken on 07/05/2021 that if UpdateAssociatedData is called then
  // for authentication encryption, mac will be calculated with associated data. if this method
  // is not called then for authentication encryption, mac will be calculated based on confidential
  // data.
  ReadOnlyMemRegion const inData(&in, sizeof(std::uint8_t));
  return UpdateAssociatedData(inData);
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> AuthCipherCtxSslAesGCM::ProcessConfidentialData(
    ReadOnlyMemRegion in, ara::core::Optional<ReadOnlyMemRegion> expectedTag) noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::ProcessConfidentialData()";

  // Design decision taken on 07/05/2021 that this method will be used for both
  // encryption and decryption. For encryption value of expectedTag will be ignored,
  // and calculated mac can be obtained by the application using Getdigest method.
  //  For decryption it is expected that expectedTag should contain the mac value
  // for authentication.

  // According to SWS_CRYPT_23634 if size of the input buffer is not divisible by
  // the block size then ProcessConfidentialData should return SecurityErrorDomain::kInvalidInputSize.
  if (((in.size()) % authcipher::kAesBlockSizeInbytes) != 0)
  {
    aralog_.LogError() << "Incorrect input size ";
    aralog_.LogDebug() << "Expected data size : " << authcipher::kAesBlockSizeInbytes;
    aralog_.LogDebug() << "Actual size data size : " << in.size();
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // According to SWS_CRYPT_23634 if the digest calculation was not initiated by a call
  // to Start(), then UpdateAssociatedData() should return SecurityErrorDomain::kProcessingNotStarted
  if (acProcessState_ < authenticationCipherExecutionStates::kAcProcessingStarted)
  {
    aralog_.LogError() << "Processing not started ";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
  }

  // Calculate the output size in blocks.
  size_t const outputsizeinBlocks{((in.size() + 1) / authcipher::kAesBlockSizeInbytes)};

  ara::core::Vector<uint8_t> output;
  // resize the output buffer according to output size in bytes.
  output.resize(outputsizeinBlocks * authcipher::kAesBlockSizeInbytes);

  int32_t outputlength{0};

  if (transformationValue_ == CryptoTransform::kEncrypt)
  {
    // Update data for encryption in openssl.
    if (opresults::kSuccess
        != EVP_EncryptUpdate(
               symmetricAuthenticationCipherCtx_, output.data(), &outputlength, in.data(), in.size()))
    {
      aralog_.LogError() << "Update failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }

    // Finalize encryption process in openssl.
    if (opresults::kSuccess
        != EVP_EncryptFinal_ex(
               symmetricAuthenticationCipherCtx_, output.data() + outputlength, &outputlength))
    {
      aralog_.LogError() << "Final failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }

    // Get the calculated tag value from the openssl
    if (opresults::kSuccess
        != EVP_CIPHER_CTX_ctrl(symmetricAuthenticationCipherCtx_, EVP_CTRL_GCM_GET_TAG,
               authcipher::kAesBlockSizeInbytes, tagValue_))
    {
      aralog_.LogError() << "Authentication is not valid";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kAuthTagNotValid);
    }
  }
  else
  {
    // Store the given tag value.
    if (expectedTag.has_value())
    {
      for (size_t index{0}; index < stkMacSizeForAuthentication_; index++)
      {
        tagValue_[index] = (*expectedTag)[index];
      }
    }

    // Update data for decryption in openssl.
    if (opresults::kSuccess
        != EVP_DecryptUpdate(
               symmetricAuthenticationCipherCtx_, output.data(), &outputlength, in.data(), in.size()))
    {
      aralog_.LogError() << "Update failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }

    // Set the tag value in openssl for decryption.
    if (opresults::kSuccess
        != EVP_CIPHER_CTX_ctrl(symmetricAuthenticationCipherCtx_, EVP_CTRL_GCM_SET_TAG,
               authcipher::kAesBlockSizeInbytes, tagValue_))
    {
      aralog_.LogError() << "Setting Tag is failed";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }

    // Finalize decryption process in openssl.
    if (opresults::kSuccess
        != EVP_DecryptFinal_ex(
               symmetricAuthenticationCipherCtx_, output.data() + outputlength, &outputlength))
    {
      aralog_.LogError() << "Authentication is not Valid ";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kAuthTagNotValid);
    }
  }

  ara::core::Vector<ara::core::Byte> const retVal{output.begin(), output.end()};

  acProcessState_ = authenticationCipherExecutionStates::kAcProcessingFinished;

  aralog_.LogDebug() << "Processing is finished successfully";

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

ara::core::Result<void> AuthCipherCtxSslAesGCM::ProcessConfidentialData(
    ReadWriteMemRegion inOut, ara::core::Optional<ReadOnlyMemRegion> expectedTag) noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::ProcessConfidentialData() with inout parameter";

  // Design decision taken on 07/05/2021 that this method will be used for both
  // encryption and decryption. For encryption value of expectedTag will be ignored,
  // and calculated mac can be obtained by the application using Getdigest method.
  // For decryption it is expected that expectedTag should contain the mac value
  // for authentication.

  ReadOnlyMemRegion const in(inOut.data(), inOut.data() + inOut.size());

  // According to SWS_CRYPT_23635 if the digest calculation was not initiated by a call
  // to Start(), then UpdateAssociatedData() should return SecurityErrorDomain::kProcessingNotStarted
  if (acProcessState_ < authenticationCipherExecutionStates::kAcProcessingStarted)
  {
    aralog_.LogError() << "Processing not started ";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
  }

  // According to SWS_CRYPT_23635 if size of the input buffer is not divisible by
  // the block size then ProcessConfidentialData should return SecurityErrorDomain::kInvalidInputSize.
  if ((in.size() % authcipher::kAesBlockSizeInbytes) != 0)
  {
    aralog_.LogError() << "Incorrect input size ";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  // Calculate output size in blocks.
  size_t const outputsizeinBlocks{((in.size() + 1) / authcipher::kAesBlockSizeInbytes)};

  ara::core::Vector<uint8_t> output;
  // Resize the output buffer according to the output size.
  output.resize(outputsizeinBlocks * authcipher::kAesBlockSizeInbytes);

  int32_t outputlength{0};

  if (transformationValue_ == CryptoTransform::kEncrypt)
  {
    // Update data for encryption in openssl.
    if (opresults::kSuccess
        != EVP_EncryptUpdate(
               symmetricAuthenticationCipherCtx_, output.data(), &outputlength, in.data(), in.size()))
    {
      aralog_.LogError() << "Update failed";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }

    // Finalize encryption process in openssl.
    if (opresults::kSuccess
        != EVP_EncryptFinal_ex(
               symmetricAuthenticationCipherCtx_, output.data() + outputlength, &outputlength))
    {
      aralog_.LogError() << "Final failed";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }

    // Get the calculated tag value from the openssl
    if (opresults::kSuccess
        != EVP_CIPHER_CTX_ctrl(symmetricAuthenticationCipherCtx_, EVP_CTRL_GCM_GET_TAG,
               stkMacSizeForAuthentication_, tagValue_))
    {
      aralog_.LogError() << "Authentication is not Valid ";
      return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kAuthTagNotValid);
    }
  }
  else
  {
    // Store the given tag value.
    if (expectedTag.has_value())
    {
      for (size_t index{0}; index < authcipher::kAesBlockSizeInbytes; index++)
      {
        tagValue_[index] = (*expectedTag)[index];
      }
    }

    // Update data for decryption in openssl.
    if (opresults::kSuccess
        != EVP_DecryptUpdate(
               symmetricAuthenticationCipherCtx_, output.data(), &outputlength, in.data(), in.size()))
    {
      aralog_.LogError() << "Update failed";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }

    // Set the tag value in openssl for decryption.
    if (opresults::kSuccess
        != EVP_CIPHER_CTX_ctrl(symmetricAuthenticationCipherCtx_, EVP_CTRL_GCM_SET_TAG,
               authcipher::kAesBlockSizeInbytes, tagValue_))
    {
      aralog_.LogError() << "Setting Tag is failed";
      return ara::core::Result<void>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kProcessingNotStarted);
    }

    // Finalize decryption process in openssl.
    if (opresults::kSuccess
        != EVP_DecryptFinal_ex(
               symmetricAuthenticationCipherCtx_, output.data() + outputlength, &outputlength))
    {
      aralog_.LogError() << "Authentication is not Valid ";
      return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kAuthTagNotValid);
    }
  }

  // Store output data in inout buffer.
  for (size_t index{0}; index < inOut.size(); index++)
  {
    inOut[index] = output[index];
  }

  acProcessState_ = authenticationCipherExecutionStates::kAcProcessingFinished;

  aralog_.LogDebug() << "Processing done successfully";

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> AuthCipherCtxSslAesGCM::GetDigest(
    std::size_t offset) const noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::GetDigest()";

  // According to SWS_CRYPT_20316 if the digest calculation was not finished by the call
  // to ProcessConfidentialData(), then GetDigest() should return SecurityErrorDomain::kProcessingNotFinished.
  if (acProcessState_ != authenticationCipherExecutionStates::kAcProcessingFinished)
  {
    aralog_.LogError() << "Can not get digest value as processing is not finished yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kProcessingNotFinished);
  }

  // According to SWS_CRYPT_20316 if context initialized by a key without kAllowSignature permission
  // then GetDigest() should return SecurityErrorDomain::kUsageViolation.
  if (!(allowedKeyUsage_ & kAllowSignature))
  {
    aralog_.LogError() << "No allowed usage permissions for signature generation";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  ara::core::Vector<ara::core::Byte> retVal;
  retVal.resize(authcipher::kAesBlockSizeInbytes);

  for (size_t index{offset}; index < authcipher::kAesBlockSizeInbytes; index++)
  {
    retVal[index - offset] = static_cast<ara::core::Byte>(tagValue_[index]);
  }

  aralog_.LogDebug() << "Digest value received successfully";
  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

ara::core::Result<void> AuthCipherCtxSslAesGCM::Reset() noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::Reset()";

  acProcessState_ = authenticationCipherExecutionStates::kAcIdle;
  if (opresults::kSuccess != EVP_CIPHER_CTX_reset(symmetricAuthenticationCipherCtx_))
  {
    aralog_.LogError() << "Reset operation failed!!!";
  }

  keyValue_.clear();

  IvValue_.clear();

  for (size_t index{0}; index < authcipher::kAesBlockSizeInbytes; index++)
  {
    tagValue_[index] = 0;
  }

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<bool> AuthCipherCtxSslAesGCM::Check(const Signature& expected) const noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::Check()";

  // According to SWS_CRYPT_20319 if the digest calculation was not finished by the call
  // to ProcessConfidentialData(), then Check() should return SecurityErrorDomain::kProcessingNotFinished.
  if (acProcessState_ != authenticationCipherExecutionStates::kAcProcessingFinished)
  {
    aralog_.LogError() << "Can not check digest value as processing is not finished yet";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kProcessingNotFinished);
  }

  // According to SWS_CRYPT_20319 if if the provided "signature" object was produced by
  // another crypto primitive type, then Check() should return SecurityErrorDomain::kIncompatibleObject.
  if (expected.GetCryptoPrimitiveId()->GetPrimitiveId() != algId_)
  {
    aralog_.LogError() << "Can not check digest value as primitive type of signatue object is different";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  ReadWriteMemRegion const expectedValue{expected.GetSignatureValue()};

  for (size_t index{0}; index < authcipher::kAesBlockSizeInbytes; index++)
  {
    if (expectedValue[index] != tagValue_[index])
    {
      return ara::core::Result<bool>::FromValue(false);
    }
  }
  return ara::core::Result<bool>::FromValue(true);
}

CryptoPrimitiveId::Uptr AuthCipherCtxSslAesGCM::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::GetCryptoPrimitiveId()";
  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr{std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_)};
  return primitiveInfoPtr;
}

EVP_CIPHER const* AuthCipherCtxSslAesGCM::GetCipherMode() const noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::GetCipherMode()";

  EVP_CIPHER const* cipherMode;

  if (algId_ == algorithmids::kAES128GCM)
  {
    cipherMode = EVP_aes_128_gcm();
  }

  else if (algId_ == algorithmids::kAES192GCM)
  {
    cipherMode = EVP_aes_192_gcm();
  }

  else if (algId_ == algorithmids::kAES256GCM)
  {
    cipherMode = EVP_aes_256_gcm();
  }

  else
  {
    cipherMode = EVP_aes_128_gcm();
  }

  return cipherMode;
}

size_t AuthCipherCtxSslAesGCM::GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept
{
  aralog_.LogDebug() << "In AuthCipherCtxSslAesGCM::GetKeySizefromAlgoId()";

  static uint8_t const kAuthCipherKeySize128Bit{16};
  static uint8_t const kAuthCipherKeySize192Bit{24};
  static uint8_t const kAuthCipherKeySize256Bit{32};

  size_t keySize;

  // If target algorithm Id requires 128 bit key.
  if (targetAlgId == algorithmids::kAES128GCM)
  {
    keySize = kAuthCipherKeySize128Bit;
  }

  // If target algorithm Id requires 192 bit key.
  else if (targetAlgId == algorithmids::kAES192GCM)
  {
    keySize = kAuthCipherKeySize192Bit;
  }

  // If target algorithm Id requires 256 bit key.
  else if (targetAlgId == algorithmids::kAES256GCM)
  {
    keySize = kAuthCipherKeySize256Bit;
  }

  // else use default size.
  else
  {
    keySize = kAuthCipherKeySize128Bit;
  }

  return keySize;
}
} // namespace cryp
} // namespace crypto
} // namespace ara
