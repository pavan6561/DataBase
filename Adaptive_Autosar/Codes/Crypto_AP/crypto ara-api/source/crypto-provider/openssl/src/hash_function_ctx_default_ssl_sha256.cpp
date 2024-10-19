// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : hash_function_ctx_default_ssl_sha256.cpp
//
// Purpose     : This file contains implementation for HashFunctionCtxDefaultSSSLSha256 class.
// -----------------------------------------------------------------------------------------------------------

#include "hash_function_ctx_default_ssl_sha256.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
HashFunctionCtxDefaultSSSLSha256::HashFunctionCtxDefaultSSSLSha256() noexcept : HashFunctionCtx{}
{
  uint16_t constexpr ksha256HashSize{ 32 };

  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256 Constructor";

  // Create new hash context in openssl.
  hashSha256Ctx_ = EVP_MD_CTX_new();

  digestValue_.resize(ksha256HashSize);
}

HashFunctionCtxDefaultSSSLSha256::~HashFunctionCtxDefaultSSSLSha256() noexcept
{
  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256 Destructor";

  // Delete the openssl hash function context from the memory.
  EVP_MD_CTX_free(hashSha256Ctx_);
}

DigestService::Uptr HashFunctionCtxDefaultSSSLSha256::GetDigestService() const noexcept
{

  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256::GetDigestService()";

  bool isProcessStart{false};
  bool isProcessFinished{false};
    
  CryptoObjectUid objectID;

  if(hashingState_ >= HashExecutionStates::kHashStart)
  {
    isProcessStart = true;
  }

  if(hashingState_ == HashExecutionStates::kHashFinal)
  {
    isProcessFinished = true;
  }

  DigestService::Uptr localPtr {std::make_unique<DigestServiceImpl>(algorithmids::ksha256Hash, objectID,
  nullptr, 0, 0, 0, digestValue_, isCtxInitialized_, isProcessStart, isProcessFinished)};

  return localPtr;
}

ara::core::Result<void> HashFunctionCtxDefaultSSSLSha256::Start(ReadOnlyMemRegion iv) noexcept
{
  // According to SWS_CRYPT_21110 if the base algorithm (or its current implementation)
  // principally doesn't support the IV variation, but provided IV value is not empty,
  // then start() should return error SecurityErrorDomain::kUnsupported.
  if (iv.empty() == false)
  {
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kUnsupported);
  }
  else
  {
    return Start();
  }
}

ara::core::Result<void> HashFunctionCtxDefaultSSSLSha256::Start(const SecretSeed& iv) noexcept
{
  return Start(static_cast<ReadOnlyMemRegion>(iv.GetSeedValue()));
}

ara::core::Result<void> HashFunctionCtxDefaultSSSLSha256::Start() noexcept
{
  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256::Start()";

  // According to the note given in SWS_CRYPT_00908 Start can be called after Update. In this
  // case the HashFunctionCtx will not return an error, instead Start will start a new hash
  // value calculation.

  // Initialize openssl hash context with SHA256.
  if (opresults::kSuccess != EVP_DigestInit(hashSha256Ctx_, EVP_sha256()))
  {
    aralog_.LogError() << "Initialization for Hash function ctx failed!!!!";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kMissingArgument);
  }

  // Set digest value length to be 0 before calculation of hash. As it is going
  // to be updated in hash calculation.
  digestValueLength_ = 0;

  // Mark the start of new hash calculation.
  hashingState_ = HashExecutionStates::kHashStart;

  // Set isInitialized as true for this ctx.
  isCtxInitialized_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> HashFunctionCtxDefaultSSSLSha256::Update(ReadOnlyMemRegion in) noexcept
{
  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256::Update()";

  // According to SWS_CRYPT_00909 The user application can call Update multiple
  // times, each time providing a new chunk of data. Update will update the hash
  // value calculation with each new chunk. if Update is called without calling the
  // start then update should throw an error SecurityErrc::kProcessingNotStarted.

  if ((hashingState_ == HashExecutionStates::kHashStart)
    || (hashingState_ == HashExecutionStates::kHashUpdate))
  {
    aralog_.LogDebug() << "Data is being updated for Hash ctx";

    // Update data for hash calculation.
    if (opresults::kSuccess != EVP_DigestUpdate(hashSha256Ctx_, in.data(), in.size()))
    {
      aralog_.LogError() << "Digest Update for Hash function ctx is failed";
      return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
    }

    // Set flag to indication update has done.
    hashingState_ = HashExecutionStates::kHashUpdate;
  }
  else
  {
    aralog_.LogError() << "Data can not be updated for Hash ctx as Hash process is not started";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
  }

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> HashFunctionCtxDefaultSSSLSha256::Update(const RestrictedUseObject& in) noexcept
{
  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256::Update() with restricted used object";

  return Update(static_cast<ReadOnlyMemRegion>(in.GetValue()));
}

ara::core::Result<void> HashFunctionCtxDefaultSSSLSha256::Update(std::uint8_t in) noexcept
{
  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256::Update() with uint8_t";

  // According to SWS_CRYPT_00909 The user application can call Update multiple
  // times, each time providing a new chunk of data. Update will update the hash
  // value calculation with each new chunk. if Update is called without calling the
  // start then update should throw an error SecurityErrc::kProcessingNotStarted.

  if ((hashingState_ == HashExecutionStates::kHashStart)
    || (hashingState_ == HashExecutionStates::kHashUpdate))
  {
    aralog_.LogDebug() << "Data is being updated for Hash ctx";

    // Update data for hash calculation in openssl.
    if (opresults::kSuccess != EVP_DigestUpdate(hashSha256Ctx_, &in, sizeof(uint8_t)))
    {
      aralog_.LogError() << "Digest Update for Hash function ctx is failed";
      return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
    }

    // Set flag to indication update has done.
    hashingState_ = HashExecutionStates::kHashUpdate;
  }
  else
  {
    aralog_.LogError() << "Data can not be updated for Hash ctx as Hash process is not started";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
  }

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> HashFunctionCtxDefaultSSSLSha256::Finish() noexcept
{
  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256::Finish()";

  // According to SWS_CRYPT_00906 if Finish() is called without calling Update()
  // then Finish() should return the error as SecurityErrc::kProcessingNotStarted.
  if ((hashingState_ == HashExecutionStates::kHashUpdate)
    || (hashingState_ == HashExecutionStates::kHashFinal))
  {
    aralog_.LogDebug() << "Finishing hashing process";

    if (hashingState_ != HashExecutionStates::kHashFinal)
    {
      // Get calculated hash value by calling final of openssl.
      if (opresults::kSuccess != EVP_DigestFinal(hashSha256Ctx_, digestValue_.data(), &digestValueLength_))
      {
        aralog_.LogError() << "Digest Finish failed for hash function ctx";
        return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          ara::crypto::SecurityErrc::kProcessingNotStarted);
      }

      // Set flag to indicate the finalization of process.
      hashingState_ = HashExecutionStates::kHashFinal;
    }

    ara::core::Vector<ara::core::Byte> const digestResult{ digestValue_.begin(), digestValue_.end() };

    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(digestResult);
  }
  else if (hashingState_ == HashExecutionStates::kHashStart)
  {
    // According to SWS_CRYPT_00906 if Finish() is called without calling Update()
    // and Start() then Finish() should return the error as SecurityErrc::
    // kInvalidUsageOrder.
    aralog_.LogError() << "Can not finish hashing process as update is not called yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrc::kInvalidUsageOrder);
  }
  else
  {
    // According to RS_CRYPTO_02302 if the digest calculation was not initiat by a call of the
    // Start() then Finish() should return the error as SecurityErrc::kProcessingNotStarted.
    aralog_.LogError() << "Can not finish hashing process as hashing process is not started yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrc::kProcessingNotStarted);
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> HashFunctionCtxDefaultSSSLSha256::GetDigest(
  std::size_t offset) const noexcept
{
  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256::GetDigest()";

  ara::core::Vector<ara::core::Byte> digestResult;
  digestResult.resize(digestValueLength_);

  // According to SWS_CRYPT_00919 if GetDigest() is called without calling
  // Final() then GetDigest() should return the error SecurityErrc::
  // kProcessingNotFinished.
  if (hashingState_ == HashExecutionStates::kHashFinal)
  {
    aralog_.LogDebug() << "Getting digest value";

    for (std::uint32_t index{ offset }; index < digestValueLength_; index++)
    {
      // Store calculated hash in output buffer in form of ara::core::Byte.
      digestResult[index] = static_cast<ara::core::Byte>(digestValue_[index]);
    }
  }
  else
  {
    aralog_.LogError() << "Can not get digest value as Hashing process is not finished yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrc::kProcessingNotFinished);
  }

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(digestResult);
}

CryptoPrimitiveId::Uptr HashFunctionCtxDefaultSSSLSha256::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In HashFunctionCtxDefaultSSSLSha256::GetCryptoPrimitiveId()";

  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr{ std::make_unique<CryptoPrimitiveIdImpl>(
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algorithmids::ksha256Hash),
    algorithmids::ksha256Hash) };
  return primitiveInfoPtr;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
