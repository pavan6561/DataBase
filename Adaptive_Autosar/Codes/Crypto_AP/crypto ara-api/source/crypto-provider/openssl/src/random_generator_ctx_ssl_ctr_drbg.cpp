// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : random_generator_ctx_ssl_ctr_drbg.cpp
//
// Purpose     : This file contains implementation for RandomGeneratorCtxDefaultSSLCtrDrbg class.
// -----------------------------------------------------------------------------------------------------------

#include "random_generator_ctx_ssl_ctr_drbg.h"

#include <cstdint>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
EVP_RAND_CTX* RandomGeneratorCtxDefaultSSLCtrDrbg::stGlobalRandomNumberGeneratorCtx_{nullptr};
bool RandomGeneratorCtxDefaultSSLCtrDrbg::stIsGenerationInProcess_{false};
ara::log::Logger& RandomGeneratorCtxDefaultSSLCtrDrbg::stAralog_{
    ara::log::CreateLogger("RNGC", "Random Number Generator context", ara::log::LogLevel::kVerbose)};

RandomGeneratorCtxDefaultSSLCtrDrbg::RandomGeneratorCtxDefaultSSLCtrDrbg(bool initialize) noexcept :
    RandomGeneratorCtx{}, keySize_{0}
{
  stAralog_.LogDebug() << "In RandomGeneratorCtxDefaultSSLCtrDrbg::Constructor";

  // According to SWS_CRYPT_00504 if initialize is set true RNG context
  // should create local RNG context else it should create global rng ctx.
  if (initialize)
  {
    stAralog_.LogDebug() << "Creating local state";

    OSSL_PARAM params[2];
    char cipherName[]{SN_aes_256_ctr};

    // Create openssl RNG instance.
    EVP_RAND* const rngInstance{EVP_RAND_fetch(nullptr, "CTR-DRBG", nullptr)};

    // Create openssl RNG context.
    randomNumberGeneratorCtx_ = EVP_RAND_CTX_new(rngInstance, nullptr);

    // Set aes-256-ctr cipher for RNG.
    params[0] = OSSL_PARAM_construct_utf8_string(OSSL_DRBG_PARAM_CIPHER, cipherName, 0);

    // end of parameters.
    params[1] = OSSL_PARAM_construct_end();

    // Initialize RNG context.
    if (opresults::kSuccess != EVP_RAND_instantiate(randomNumberGeneratorCtx_, 0, 0, nullptr, 0, params))
    {
      stAralog_.LogError() << "Local RNG instantiation failed!!!!";
    }

    // Delete openssl RNG instance.
    EVP_RAND_free(rngInstance);
  }
  else
  {
    stAralog_.LogDebug() << "Creating global state";
    IntstantiateGlobalCtx();
  }
  isLocalCtx_ = initialize;
}

void RandomGeneratorCtxDefaultSSLCtrDrbg::IntstantiateGlobalCtx()
{
  static bool stisglobalctxintatiate{false};

  stAralog_.LogDebug() << "In RandomGeneratorCtxDefaultSSLCtrDrbg::IntstantiateGlobalCtx()";

  if (stisglobalctxintatiate == false)
  {
    OSSL_PARAM params[2];
    char cipherName[]{SN_aes_256_ctr};

    // Create openssl RNG instance.
    EVP_RAND* const rngInstance{EVP_RAND_fetch(nullptr, "CTR-DRBG", nullptr)};

    // Create openssl RNG context.
    RandomGeneratorCtxDefaultSSLCtrDrbg::stGlobalRandomNumberGeneratorCtx_
        = EVP_RAND_CTX_new(rngInstance, nullptr);

    // Set aes-256-ctr cipher for RNG.
    params[0] = OSSL_PARAM_construct_utf8_string(OSSL_DRBG_PARAM_CIPHER, cipherName, 0);

    // end of parameters.
    params[1] = OSSL_PARAM_construct_end();

    if (opresults::kSuccess
        != EVP_RAND_instantiate(RandomGeneratorCtxDefaultSSLCtrDrbg::stGlobalRandomNumberGeneratorCtx_, 0, 0,
               nullptr, 0, params))
    {
      stAralog_.LogError() << "Global RNG Instantiation Failed!!!";
    }

    EVP_RAND_free(rngInstance);

    stisglobalctxintatiate = true;
  }
}

RandomGeneratorCtxDefaultSSLCtrDrbg::~RandomGeneratorCtxDefaultSSLCtrDrbg() noexcept
{
  stAralog_.LogDebug() << "RNG ctx destructor";
  if (isLocalCtx_ == true)
  {
    // Uninitializes the openssl RNG context.
    if (opresults::kSuccess != EVP_RAND_uninstantiate(randomNumberGeneratorCtx_))
    {
      stAralog_.LogError() << "Local RNG un - instantiation failed";
    }

    // Delete the openssl RNG context.
    EVP_RAND_CTX_free(randomNumberGeneratorCtx_);
  }
}

ExtensionService::Uptr RandomGeneratorCtxDefaultSSLCtrDrbg::GetExtensionService() const noexcept
{
  stAralog_.LogDebug() << "RandomGeneratorCtxDefaultSSLCtrDrbg::GetExtensionService()";
  uint8_t const kByteSizeinBits{8};

  CryptoObjectUid objectID;

  ExtensionService::Uptr localPtr{std::make_unique<ExtensionServiceImpl>(
      algorithmids::kDrbgCtrBasedRng, keySize_ * kByteSizeinBits, kAllowRngInit, objectID, isLocalCtx_)};

  return localPtr;
}

bool RandomGeneratorCtxDefaultSSLCtrDrbg::AddEntropy(ReadOnlyMemRegion entropy)
{
  stAralog_.LogDebug() << "In RandomGeneratorCtxDefaultSSLCtrDrbg::AddEntropy()";

  bool retVal{false};

  // According to SWS_CRYPT_00503 AddEntropy should return false without any
  // modification if given RNG ctx is a global RNG ctx.
  if (isLocalCtx_ == true)
  {
    stAralog_.LogDebug() << "adding Entropy in local state";
    entropyValue_.clear();
    entropyValue_.resize(entropy.size());
    for (size_t index{0}; index < entropy.size(); index++)
    {
      entropyValue_[index] = entropy[index];
    }
    retVal = true;
  }
  else
  {
    stAralog_.LogWarn() << "Can not add entropy as it is a global state";
  }

  return retVal;
}

bool RandomGeneratorCtxDefaultSSLCtrDrbg::Seed(const SecretSeed& seed) noexcept
{
  stAralog_.LogDebug() << " In RandomGeneratorCtxDefaultSSLCtrDrbg::Seed() with secret seed object ";

  if ((seed.GetAllowedUsage() & kAllowRngInit) == opresults::kFailure)
  {
    stAralog_.LogError() << "Secret Seed does not have rng init permissions";
    return false;
  }

  return Seed(static_cast<ReadOnlyMemRegion>(seed.GetSeedValue()));
}

bool RandomGeneratorCtxDefaultSSLCtrDrbg::Seed(ReadOnlyMemRegion seed) noexcept
{
  stAralog_.LogDebug() << " In RandomGeneratorCtxDefaultSSLCtrDrbg::Seed()";

  bool retVal{false};

  // According to SWS_CRYPT_00503 Seed should return false without any
  // modification if given RNG ctx is a global RNG ctx.
  if (isLocalCtx_ == true)
  {
    stAralog_.LogDebug() << "Seeding local state";
    std::uint32_t preidctionResistacne{0};
    static std::uint8_t constexpr klowerByteMask{8};
    std::uint32_t loopEnd{sizeof(std::uint32_t)};

    if (seed.size() < loopEnd)
    {
      loopEnd = seed.size();
    }

    for (std::uint32_t byteIndex{0}; byteIndex < loopEnd; byteIndex++)
    {
      preidctionResistacne |= seed[byteIndex];
      if (loopEnd != (byteIndex + 1))
      {
        preidctionResistacne = (preidctionResistacne << klowerByteMask);
      }
    }

    // Seed openssl RNG context.
    if (true
        == EVP_RAND_reseed(randomNumberGeneratorCtx_, preidctionResistacne, nullptr, 0, entropyValue_.data(),
               entropyValue_.size()))
    {
      stAralog_.LogDebug() << "Local state seeded successfully";
      isCtxinitialize_ = true;
      retVal = true;
    }
  }
  else
  {
    stAralog_.LogWarn() << "Can not be seeded as it is a global state";
  }
  return retVal;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> RandomGeneratorCtxDefaultSSLCtrDrbg::Generate(
    std::uint32_t count)
{
  ara::core::Vector<uint8_t> output;

  stAralog_.LogDebug() << "In RandomGeneratorCtxDefaultSSLCtrDrbg::Generate()";

  if (isLocalCtx_)
  {
    // According to SWS_CRYPT_00507 if given number of random bytes can not be served
    // by the local RNG ctx then Generate should return the error as SecurityErrorDomain::
    // kUninitializedContext.

    if (count > configInstance_->GetCryptoMaxBufferSize())
    {
      stAralog_.LogError() << "Can not generate given number of random bytes";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }

    output.resize(count);

    stAralog_.LogDebug() << "Generating Random number in local state";

    if (!isCtxinitialize_)
    {
      stAralog_.LogError() << "Can not generate random number as context is not initialize ";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }

    // Generate random data in output for given number of bytes.
    if (opresults::kSuccess
        != EVP_RAND_generate(randomNumberGeneratorCtx_, output.data(), count, 0, 0, nullptr, 0))
    {
      stAralog_.LogError() << "Can not generate given number of random bytes";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kUninitializedContext);
    }
  }
  else
  {
    stAralog_.LogDebug() << "Generating Random number in global state";
    // According to SWS_CRYPT_00507 if given number of random bytes can not be served
    // by the global RNG ctx then Generate should return the error as SecurityErrorDomain::
    // kBusyResource.
    if (!stIsGenerationInProcess_)
    {
      stIsGenerationInProcess_ = true;
      output.resize(count);

      // Generate random data in output for given number of bytes.
      if (opresults::kSuccess
          != EVP_RAND_generate(stGlobalRandomNumberGeneratorCtx_, output.data(), count, 0, 0, nullptr, 0))
      {
        stAralog_.LogError() << "Can not generate given number of random bytes";
        return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
            SecurityErrorDomain ::Errc::kBusyResource);
      }

      stIsGenerationInProcess_ = false;
    }
    else
    {
      stAralog_.LogError() << "Can not generate random number as global resource is busy";
      return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
          SecurityErrorDomain::Errc::kBusyResource);
    }
  }

  ara::core::Vector<ara::core::Byte> const randomNumber{output.begin(), output.end()};

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(randomNumber);
}

bool RandomGeneratorCtxDefaultSSLCtrDrbg::SetKey(const SymmetricKey& key) noexcept
{
  stAralog_.LogDebug() << "In RandomGeneratorCtxDefaultSSLCtrDrbg::SetKey()";

  if ((key.GetAllowedUsage() & kAllowRngInit) == opresults::kFailure)
  {
    stAralog_.LogError() << "Symmetric Key does not have rng init permissions";
    return false;
  }

  keySize_ = key.GetPayloadSize();

  return Seed(static_cast<ReadOnlyMemRegion>(key.GetSymmetricKey()));
}

CryptoPrimitiveId::Uptr RandomGeneratorCtxDefaultSSLCtrDrbg::GetCryptoPrimitiveId() const noexcept
{
  stAralog_.LogDebug() << "In RandomGeneratorCtxDefaultSSLCtrDrbg::GetCryptoPrimitiveId()";

  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr{std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algorithmids::kDrbgCtrBasedRng),
      algorithmids::kDrbgCtrBasedRng)};
  return primitiveInfoPtr;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
