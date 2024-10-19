// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypro_provider_ssl.cpp
//
// Purpose     : This file contains implementation for CryptoProviderSSL class.
// -----------------------------------------------------------------------------------------------------------

#include "crypto_provider_ssl.h"

#include <ara/core/result.h>
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
/// @brief namespace for provider.
namespace provider
{
uint8_t constexpr kCryptoProviderAes128BitKeySize{ 16 };
uint8_t constexpr kCryptoProviderAes192BitKeySize{ 24 };
uint8_t constexpr kCryptoProviderAes256BitKeySize{ 32 };
uint8_t constexpr kCryptoProviderAesGcmIVSize{ 12 };
uint16_t constexpr kSha256HashSize{ 32 };
} // namespace provider.

ara::core::Result<SymmetricKey::Uptrc> CryptoProviderSSL::GenerateSymmetricKey(
  AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept
{
  aralog_.LogDebug() << "In CryptoProviderSSL::GenerateSymmetricKey()";
  static uint8_t constexpr kNoOfKDKParametersForKey{ 5 };
  ara::core::Vector<uint8_t> derivedKey;
  ara::core::Vector<uint8_t> salt{ GenerateRandomData(provider::kCryptoProviderAes128BitKeySize) };
  ara::core::Vector<uint8_t> secretKey{ GenerateRandomData(provider::kCryptoProviderAes128BitKeySize) };
  OSSL_PARAM params[kNoOfKDKParametersForKey];

  // Find and allocate a context for the HKDF algorithm.
  EVP_KDF* const kdf{ EVP_KDF_fetch(nullptr, "hkdf", nullptr) };
  EVP_KDF_CTX* const kctx{ EVP_KDF_CTX_new(kdf) };
  // The kctx keeps a reference so this is safe.
  EVP_KDF_free(kdf);

  char sha[]{ "sha-256" };
  char info[]{ "label" };
  uint8_t paramIndex{ 0 };

  // Build up the parameters for the derivation.
  params[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST, sha, strlen(sha));
  params[paramIndex++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT, salt.data(), salt.size());
  params[paramIndex++]
    = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_KEY, secretKey.data(), secretKey.size());
  params[paramIndex++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_INFO, info, strlen(info));
  params[paramIndex] = OSSL_PARAM_construct_end();

  if ((algId == algorithmids::kAES128CBC) || (algId == algorithmids::kAES128ECB)
    || (algId == algorithmids::kAES128CTR) || (algId == algorithmids::kAES128GCM)
    || (algId == algorithmids::kKeyWrap128) || (algId == algorithmids::kKeyWrapPad128)
    || (algId == algorithmids::kDrbgCtrBasedRng) || (algId == algorithmids::kPBKDF2)
    || (algId == algorithmids::kHMAC) || (algId == algorithmids::kCMAC128)
    || (algId == algorithmids::kGMAC128))
  {
    derivedKey.resize(provider::kCryptoProviderAes128BitKeySize);
  }
  else if ((algId == algorithmids::kAES192CBC) || (algId == algorithmids::kAES192ECB)
    || (algId == algorithmids::kAES192CTR) || (algId == algorithmids::kAES192GCM)
    || (algId == algorithmids::kKeyWrap192) || (algId == algorithmids::kKeyWrapPad192)
    || (algId == algorithmids::kCMAC192) || (algId == algorithmids::kGMAC192))
  {
    derivedKey.resize(provider::kCryptoProviderAes192BitKeySize);
  }
  else if ((algId == algorithmids::kAES256CBC) || (algId == algorithmids::kAES256ECB)
    || (algId == algorithmids::kAES256CTR) || (algId == algorithmids::kAES256GCM)
    || (algId == algorithmids::kKeyWrap256) || (algId == algorithmids::kKeyWrapPad256)
    || (algId == algorithmids::kCMAC256) || (algId == algorithmids::kGMAC256))
  {
    derivedKey.resize(provider::kCryptoProviderAes256BitKeySize);
  }
  else
  {
    // According to SWS_CRYPT_20721 if algId has an unsupported value then
    // GenerateSymmetricKey() shall return SecurityErrorDomain::kUnknownIdentifier.
    aralog_.LogError() << "Unknown algorithmId is received!!!!";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  // According to SWS_CRYPT_20721 if @c allowedUsage argument is incompatible with target algorithm
  // algId then GenerateSymmetricKey() shall return SecurityErrorDomain::kIncompatibleArguments.
  if (((algId == algorithmids::kDrbgCtrBasedRng) && ((allowedUsage & kAllowRngInit) == opresults::kFailure))
    || ((algId == algorithmids::kPBKDF2) && ((allowedUsage & kAllowKdfMaterial) == opresults::kFailure))
    || (((algId >= algorithmids::kHMAC) && (algId <= algorithmids::kGMAC256))
         && ((allowedUsage & (kAllowSignature | kAllowDerivedSignature)) == opresults::kFailure))
    || (((algId >= algorithmids::kAES128CBC) && (algId <= algorithmids::kKeyWrapPad256))
         && ((allowedUsage
               & (kAllowDataEncryption | kAllowDataDecryption | kAllowDerivedDataEncryption
                   | kAllowDerivedDataDecryption))
              == opresults::kFailure)))
  {
    aralog_.LogError() << "Invalid Usage Permissions!!!!";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
      SecurityErrorDomain::Errc::kIncompatibleArguments);
  }

  // Do the derivation.
  if (opresults::kSuccess != EVP_KDF_derive(kctx, derivedKey.data(), derivedKey.size(), params))
  {
    aralog_.LogError() << "Symmetric Key Generation Failed!!!!";
  }

  EVP_KDF_CTX_free(kctx);

  ReadWriteMemRegion key{ derivedKey.data(), derivedKey.size() };

  CryptoObjectUid objectID;

  SymmetricKey::Uptrc symKey{ std::make_unique<SymmetricKeyImpl>(key, allowedUsage,
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId), algId, isSession, isExportable,
    objectID) };

  return ara::core::Result<SymmetricKey::Uptrc>::FromValue(std::move(symKey));
}

ara::core::Result<SecretSeed::Uptrc> CryptoProviderSSL::GenerateSeed(
  AlgId algId, SecretSeed::Usage allowedUsage, bool isSession, bool isExportable) noexcept
{
  aralog_.LogDebug() << "In CryptoProviderSSL::GenerateSeed()";
  ara::core::Vector<uint8_t> derivedSeed;
  static uint8_t constexpr kNoOfKDKParametersForSeed{ 5 };
  ara::core::Vector<uint8_t> salt{ GenerateRandomData(provider::kCryptoProviderAes128BitKeySize) };
  ara::core::Vector<uint8_t> secretKey{ GenerateRandomData(provider::kCryptoProviderAes128BitKeySize) };
  OSSL_PARAM params[kNoOfKDKParametersForSeed];

  // Find and allocate a context for the HKDF algorithm.
  EVP_KDF* const kdf{ EVP_KDF_fetch(nullptr, "hkdf", nullptr) };
  EVP_KDF_CTX* const kctx{ EVP_KDF_CTX_new(kdf) };
  // The kctx keeps a reference so this is safe.
  EVP_KDF_free(kdf);

  char sha[]{ "sha-256" };
  char info[]{ "label" };
  uint8_t paramIndex{ 0 };

  // Build up the parameters for the derivation
  params[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST, sha, strlen(sha));
  params[paramIndex++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT, salt.data(), salt.size());
  params[paramIndex++]
    = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_KEY, secretKey.data(), secretKey.size());
  params[paramIndex++] = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_INFO, info, strlen(info));
  params[paramIndex] = OSSL_PARAM_construct_end();

  if ((algId == algorithmids::kAES128CBC) || (algId == algorithmids::kAES128ECB)
    || (algId == algorithmids::kAES128CTR) || (algId == algorithmids::kKeyWrap128)
    || (algId == algorithmids::kKeyWrapPad128) || (algId == algorithmids::kDrbgCtrBasedRng)
    || (algId == algorithmids::kPBKDF2) || (algId == algorithmids::kAES192CBC)
    || (algId == algorithmids::kAES192ECB) || (algId == algorithmids::kAES192CTR)
    || (algId == algorithmids::kKeyWrap192) || (algId == algorithmids::kKeyWrapPad192)
    || (algId == algorithmids::kAES256CBC) || (algId == algorithmids::kAES256ECB)
    || (algId == algorithmids::kAES256CTR) || (algId == algorithmids::kKeyWrap256)
    || (algId == algorithmids::kKeyWrapPad256))
  {
    derivedSeed.resize(provider::kCryptoProviderAes128BitKeySize);
  }
  else if ((algId == algorithmids::kAES128GCM) || (algId == algorithmids::kAES192GCM)
    || (algId == algorithmids::kAES256GCM) || (algId == algorithmids::kGMAC128)
    || (algId == algorithmids::kGMAC192) || (algId == algorithmids::kGMAC256))
  {
    derivedSeed.resize(provider::kCryptoProviderAesGcmIVSize);
  }
  else if ((algId == algorithmids::ksha256Hash) || (algId == algorithmids::kHMAC)
    || (algId == algorithmids::kCMAC128) || (algId == algorithmids::kCMAC192)
    || (algId == algorithmids::kCMAC256))
  {
    derivedSeed.resize(0);
  }
  else
  {
    aralog_.LogError() << "Unknown Identifier received!!!";
    // According to SWS_CRYPT_20723 if algId has an unsupported value then
    // GenerateSeed() shall return SecurityErrorDomain::kUnknownIdentifier.
    return ara::core::Result<SecretSeed::Uptrc>::FromError(SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  // According to SWS_CRYPT_20723 if allowedUsage argument is incompatible with target algorithm
  // algId then GenerateSeed() shall return SecurityErrorDomain::kIncompatibleArguments.
  if (((algId == algorithmids::kDrbgCtrBasedRng) && ((allowedUsage & kAllowRngInit) == opresults::kFailure))
    || ((algId == algorithmids::kPBKDF2) && ((allowedUsage & kAllowKdfMaterial) == opresults::kFailure))
    || (((algId >= algorithmids::kHMAC) && (algId <= algorithmids::kGMAC256))
         && ((allowedUsage & (kAllowSignature | kAllowDerivedSignature)) == opresults::kFailure))
    || (((algId >= algorithmids::kAES128CBC) && (algId <= algorithmids::kKeyWrapPad256))
         && ((allowedUsage
               & (kAllowDataEncryption | kAllowDataDecryption | kAllowDerivedDataEncryption
                   | kAllowDerivedDataDecryption))
              == opresults::kFailure)))
  {
    aralog_.LogError() << "Incompatible Arguments received!!!!";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(SecurityErrorDomain::Errc::kIncompatibleArguments);
  }

  // Do the derivation.
  if ((algId != algorithmids::ksha256Hash) && (algId != algorithmids::kHMAC)
    && (algId != algorithmids::kCMAC128) && (algId != algorithmids::kCMAC192)
    && (algId != algorithmids::kCMAC256))
  {
    if (opresults::kSuccess != EVP_KDF_derive(kctx, derivedSeed.data(), derivedSeed.size(), params))
    {
      aralog_.LogError() << "Secret Seed Generation Failed!!!!";
    }
    EVP_KDF_CTX_free(kctx);
  }

  ReadWriteMemRegion seed{ derivedSeed.data(), derivedSeed.size() };

  CryptoObjectUid objectID;

  SecretSeed::Uptrc secSeed{ std::make_unique<SecretSeedImpl>(seed, allowedUsage,
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId), algId, isSession, isExportable,
    objectID) };

  return ara::core::Result<SecretSeed::Uptrc>::FromValue(std::move(secSeed));
}

ara::core::Result<PrivateKey::Uptrc> CryptoProviderSSL::GeneratePrivateKey(
  AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept
{
  // create private/public key pair
  // init RSA context, so we can generate a key pair
  EVP_PKEY_CTX* keyCtx{ nullptr };
  EVP_PKEY* paramKey{ nullptr };

  if (algId >= algorithmids::kRSA2048 && algId < algorithmids::kEC)
  {
    if ((allowedUsage & (kAllowDataEncryption | kAllowDataDecryption | kAllowSignature | kAllowVerification))
      == opresults::kFailure)
    {
      aralog_.LogError() << "Incompatible Arguments received!!!!";
      // According to SWS_CRYPT_20722 if allowedUsage argument is incompatible with target algorithm
      // algId then GeneratePrivateKey() shall return SecurityErrorDomain::kIncompatibleArguments.
      return ara::core::Result<PrivateKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    keyCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_CTX_set_rsa_keygen_bits(keyCtx, 2048);
  }
  else if (algId >= algorithmids::kEC && algId < algorithmids::kDSA)
  {
    if ((allowedUsage & (kAllowDataEncryption | kAllowDataDecryption | kAllowSignature | kAllowVerification))
      == opresults::kFailure)
    {
      aralog_.LogError() << "Incompatible Arguments received!!!!";
      // According to SWS_CRYPT_20722 if allowedUsage argument is incompatible with target algorithm
      // algId then GeneratePrivateKey() shall return SecurityErrorDomain::kIncompatibleArguments.
      return ara::core::Result<PrivateKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    EVP_PKEY_CTX* keygenCtx{ EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL) };
    EVP_PKEY_paramgen_init(keygenCtx);
    EVP_PKEY_CTX_set_dsa_paramgen_type(keygenCtx, "NID_secp256k1");
    EVP_PKEY_paramgen(keygenCtx, &paramKey);
  }
  else if (algId >= algorithmids::kDSA && algId < algorithmids::kDH)
  {
    if ((allowedUsage & (kAllowDataEncryption | kAllowDataDecryption | kAllowSignature | kAllowVerification))
      == opresults::kFailure)
    {
      aralog_.LogError() << "Incompatible Arguments received!!!!";
      // According to SWS_CRYPT_20722 if allowedUsage argument is incompatible with target algorithm
      // algId then GeneratePrivateKey() shall return SecurityErrorDomain::kIncompatibleArguments.
      return ara::core::Result<PrivateKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    EVP_PKEY_CTX* keygenCtx{ EVP_PKEY_CTX_new_id(EVP_PKEY_DSA, NULL) };
    EVP_PKEY_paramgen_init(keygenCtx);
    EVP_PKEY_CTX_set_dsa_paramgen_bits(keygenCtx, 2048);
    EVP_PKEY_paramgen(keygenCtx, &paramKey);
  }
  else if (algId >= algorithmids::kDH && algId < algorithmids::kECDSA)
  {
    if ((allowedUsage & (kAllowDataEncryption | kAllowDataDecryption | kAllowSignature | kAllowVerification))
      == opresults::kFailure)
    {
      aralog_.LogError() << "Incompatible Arguments received!!!!";
      // According to SWS_CRYPT_20722 if allowedUsage argument is incompatible with target algorithm
      // algId then GeneratePrivateKey() shall return SecurityErrorDomain::kIncompatibleArguments.
      return ara::core::Result<PrivateKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    aralog_.LogDebug() << "Generating DH keys";
    EVP_PKEY_CTX* keygenCtx{ EVP_PKEY_CTX_new_id(EVP_PKEY_DH, NULL) };
    EVP_PKEY_paramgen_init(keygenCtx);
    aralog_.LogDebug() << "Setting DH parameters";
    EVP_PKEY_CTX_set_dh_paramgen_prime_len(keygenCtx, 2048);
    EVP_PKEY_CTX_set_dh_paramgen_subprime_len(keygenCtx, 2048);
    EVP_PKEY_CTX_set_dh_kdf_type(keygenCtx, EVP_PKEY_DH_KDF_NONE);
    EVP_PKEY_CTX_set_dh_kdf_md(keygenCtx, EVP_sha256());
    aralog_.LogDebug() << "Generating DH parameters";
    EVP_PKEY_paramgen(keygenCtx, &paramKey);
  }
  else if (algId >= algorithmids::kECDSA && algId < algorithmids::kECDH)
  {
    if ((allowedUsage & (kAllowDataEncryption | kAllowDataDecryption | kAllowSignature | kAllowVerification))
      == opresults::kFailure)
    {
      aralog_.LogError() << "Incompatible Arguments received!!!!";
      // According to SWS_CRYPT_20722 if allowedUsage argument is incompatible with target algorithm
      // algId then GeneratePrivateKey() shall return SecurityErrorDomain::kIncompatibleArguments.
      return ara::core::Result<PrivateKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    EVP_PKEY_CTX* keygenCtx{ EVP_PKEY_CTX_new_id(EVP_PKEY_DSA, NULL) };
    EVP_PKEY_paramgen_init(keygenCtx);
    EVP_PKEY_CTX_set_dsa_paramgen_type(keygenCtx, "NID_secp256k1");
    EVP_PKEY_CTX_set_dsa_paramgen_bits(keygenCtx, 2048);
    EVP_PKEY_paramgen(keygenCtx, &paramKey);
  }
  else if (algId >= algorithmids::kECDH)
  {
    if ((allowedUsage & kAllowKeyAgreement) == opresults::kFailure)
    {
      aralog_.LogError() << "Incompatible Arguments received!!!!";
      // According to SWS_CRYPT_20722 if allowedUsage argument is incompatible with target algorithm
      // algId then GeneratePrivateKey() shall return SecurityErrorDomain::kIncompatibleArguments.
      return ara::core::Result<PrivateKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    EVP_PKEY_CTX* keygenCtx{ EVP_PKEY_CTX_new_id(EVP_PKEY_DH, NULL) };
    EVP_PKEY_paramgen_init(keygenCtx);
    EVP_PKEY_CTX_set_dh_paramgen_type(keygenCtx, NID_X9_62_prime256v1);
    EVP_PKEY_CTX_set_dh_paramgen_prime_len(keygenCtx, 2048);
    EVP_PKEY_paramgen(keygenCtx, &paramKey);
  }
  else
  {
    aralog_.LogError() << "Unknown Identifier received!!!";
    // According to SWS_CRYPT_20722 if algId has an unsupported value then
    // GeneratePrivateKey() shall return SecurityErrorDomain::kUnknownIdentifier.
    return ara::core::Result<PrivateKey::Uptrc>::FromError(SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (keyCtx == nullptr)
  {
    aralog_.LogDebug() << "Creating new key ctx";
    keyCtx = EVP_PKEY_CTX_new(paramKey, NULL);
  }

  aralog_.LogDebug() << "Initializing key ctx";
  EVP_PKEY_keygen_init(keyCtx);

  // variable that will hold both private and public keys
  EVP_PKEY* key{ nullptr };

  // generate key
  aralog_.LogInfo() << "Generating private kya pair";
  if (EVP_PKEY_keygen(keyCtx, &key) <= 0)
  {
    aralog_.LogError() << "Key generation failed";
  }
  // free up key context
  EVP_PKEY_CTX_free(keyCtx);

  aralog_.LogInfo() << "Creating BIO";
  // extract private key as string
  // create a place to dump the IO, in this case in memory
  BIO* privateBIO = BIO_new(BIO_s_mem());
  // dump key to IO
  aralog_.LogInfo() << "Writing private key to bio";
  PEM_write_bio_PrivateKey(privateBIO, key, NULL, NULL, 0, 0, NULL);
  // get buffer length
  int privateKeyLen = BIO_pending(privateBIO);
  aralog_.LogInfo() << "Reading Private key from bio";

  ara::core::Vector<uint8_t> privateKeyChar;
  privateKeyChar.resize(privateKeyLen);
  ReadWriteMemRegion keyValue{ privateKeyChar };
  // read the key from the buffer and put it in the char reference
  BIO_read(privateBIO, privateKeyChar.data(), privateKeyLen);
  // at this point we can save the private key somewhere

  aralog_.LogInfo() << "Got Key value from bio";

  CryptoObjectUid objectID;

  PrivateKey::Uptrc privateKey = std::make_unique<PrivateKeyImpl>(keyValue, allowedUsage,
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId), algId, isSession, isExportable,
    objectID);

  BIO_free(privateBIO);

  return ara::core::Result<PrivateKey::Uptrc>::FromValue(std::move(privateKey));
}

ara::core::Result<RandomGeneratorCtx::Uptr> CryptoProviderSSL::CreateRandomGeneratorCtx(
  AlgId algId, bool initialize) noexcept
{
  RandomGeneratorCtx::Uptr rngctxPtr{ nullptr };
  static_cast<void>(algId);

  aralog_.LogDebug() << "In CryptoProviderSSL::CreateRandomGeneratorCtx()";
  rngctxPtr = std::make_unique<RandomGeneratorCtxDefaultSSLCtrDrbg>(initialize);

  // According to SWS_CRYPT_00504 if CreateRandomGeneratorCtx is called with
  // initialize == true then internal states of the random number generator
  // should be seeded by Crypto provider before returning.
  if (initialize == true)
  {
    ara::core::String seedValue{
      ara::crypto::config::CryptoConfig::GetCryptoConfigInstance()->GetRngSeedValue()
    };
    ara::core::Vector<uint8_t> seedValueVtr{ seedValue.begin(), seedValue.end() };
    ReadOnlyMemRegion const seed{ seedValueVtr };

    if (false == rngctxPtr->Seed(seed))
    {
      // According to SWS_CRYPT_00505 if RNG ctx is created with initialize == true
      // but created local RNG ctx can not be seeded then CreateRandomGeneratorCtx
      // should return error as SecurityErrorDomain::kBusyResource.
      aralog_.LogError() << "Context can not be seeded";
      return ara::core::Result<RandomGeneratorCtx::Uptr>::FromError(SecurityErrorDomain::Errc::kBusyResource);
    }
  }

  return ara::core::Result<RandomGeneratorCtx::Uptr>::FromValue(std::move(rngctxPtr));
}

ara::core::Result<HashFunctionCtx::Uptr> CryptoProviderSSL::CreateHashFunctionCtx(AlgId algId) noexcept
{
  HashFunctionCtx::Uptr hashctxPtr{ nullptr };
  static_cast<void>(algId);

  aralog_.LogDebug() << "In CryptoProviderSSL::CreateHashFunctionCtx()";

  hashctxPtr = std::make_unique<HashFunctionCtxDefaultSSSLSha256>();

  return ara::core::Result<HashFunctionCtx::Uptr>(std::move(hashctxPtr));
}

ara::core::Result<SymmetricBlockCipherCtx::Uptr> CryptoProviderSSL::CreateSymmetricBlockCipherCtx(
  AlgId algId) noexcept
{
  aralog_.LogDebug() << "In CryptoProviderSSL::CreateSymmetricBlockCipherCtx()";

  SymmetricBlockCipherCtx::Uptr sbctxPtr{ std::make_unique<SymmetricBlockCipherCtxsslAES>(algId) };

  return ara::core::Result<SymmetricBlockCipherCtx::Uptr>(std::move(sbctxPtr));
}

ara::core::Result<StreamCipherCtx::Uptr> CryptoProviderSSL::CreateStreamCipherCtx(AlgId algId) noexcept
{
  aralog_.LogDebug() << "In CryptoProviderSSL::CreateStreamCipherCtx()";

  StreamCipherCtx::Uptr scctxPtr{ std::make_unique<StreamCipherCtxsslCtr>(algId) };

  return ara::core::Result<StreamCipherCtx::Uptr>(std::move(scctxPtr));
}

ara::core::Result<AuthCipherCtx::Uptr> CryptoProviderSSL::CreateAuthCipherCtx(AlgId algId) noexcept
{
  aralog_.LogDebug() << "In CryptoProviderSSL::CreateAuthCipherCtx()";

  AuthCipherCtx::Uptr acctxPtr{ std::make_unique<AuthCipherCtxSslAesGCM>(algId) };

  return ara::core::Result<AuthCipherCtx::Uptr>(std::move(acctxPtr));
}

ara::core::Result<KeyDerivationFunctionCtx::Uptr> CryptoProviderSSL::CreateKeyDerivationFunctionCtx(
  AlgId algId) noexcept
{
  static_cast<void>(algId);

  aralog_.LogDebug() << "In CryptoProviderSSL::CreateKeyDerivationFunctionCtx()";

  KeyDerivationFunctionCtx::Uptr pbKdfCtxPtr{ std::make_unique<KeyDerivationFunctionCtxSSLPBKDF>() };

  return ara::core::Result<KeyDerivationFunctionCtx::Uptr>(std::move(pbKdfCtxPtr));
}

ara::core::Result<MessageAuthnCodeCtx::Uptr> CryptoProviderSSL::CreateMessageAuthCodeCtx(AlgId algId) noexcept
{
  aralog_.LogDebug() << "In CryptoProviderSSL::CreateMessageAuthCodeCtx()";

  MessageAuthnCodeCtx::Uptr macCtx{ std::make_unique<MessageAuthnCodeCtxDefaultSsl>(algId) };

  return ara::core::Result<MessageAuthnCodeCtx::Uptr>(std::move(macCtx));
}

ara::core::Result<SymmetricKeyWrapperCtx::Uptr> CryptoProviderSSL::CreateSymmetricKeyWrapperCtx(
  AlgId algId) noexcept
{
  aralog_.LogDebug() << "In CryptoProviderSSL::CreateMessageAuthCodeCtx()";

  SymmetricKeyWrapperCtx::Uptr symmetricKeyWrapCtx{ std::make_unique<SymmetricKeyWrapperCtxWrapDefaultSSL>(
    algId) };

  return ara::core::Result<SymmetricKeyWrapperCtx::Uptr>(std::move(symmetricKeyWrapCtx));
}

ara::core::Result<EncryptorPublicCtx::Uptr> CryptoProviderSSL::CreateEncryptorPublicCtx(AlgId algId) noexcept
{
  EncryptorPublicCtx::Uptr localPtr{ std::make_unique<EncryptorPublicCtxsslRsa>(algId) };
  return ara::core::Result<EncryptorPublicCtx::Uptr>(std::move(localPtr));
}

ara::core::Result<DecryptorPrivateCtx::Uptr> CryptoProviderSSL::CreateDecryptorPrivateCtx(
  AlgId algId) noexcept
{
  DecryptorPrivateCtx::Uptr localPtr{ std::make_unique<DecryptorPrivateCtxRSA>(algId) };
  return ara::core::Result<DecryptorPrivateCtx::Uptr>(std::move(localPtr));
}

ara::core::Result<KeyEncapsulatorPublicCtx::Uptr> CryptoProviderSSL::CreateKeyEncapsulatorPublicCtx(
  AlgId algId) noexcept
{
  KeyEncapsulatorPublicCtx::Uptr localPtr{ std::make_unique<KeyEncapsulatorPublicCtxDefaultSsl>(algId) };
  return ara::core::Result<KeyEncapsulatorPublicCtx::Uptr>(std::move(localPtr));
}

ara::core::Result<KeyDecapsulatorPrivateCtx::Uptr> CryptoProviderSSL::CreateKeyDecapsulatorPrivateCtx(
  AlgId algId) noexcept
{
  KeyDecapsulatorPrivateCtx::Uptr localPtr{ std::make_unique<KeyDecapsulatorPrivateCtxDefaultSsl>(algId) };
  return ara::core::Result<KeyDecapsulatorPrivateCtx::Uptr>(std::move(localPtr));
}

ara::core::Result<SigEncodePrivateCtx::Uptr> CryptoProviderSSL::CreateSigEncodePrivateCtx(
  AlgId algId) noexcept
{
  SigEncodePrivateCtx::Uptr localPtr{ std::make_unique<SigEncodePrivateCtxDefaultSsl>(algId) };
  return ara::core::Result<SigEncodePrivateCtx::Uptr>::FromValue(std::move(localPtr));
}

ara::core::Result<MsgRecoveryPublicCtx::Uptr> CryptoProviderSSL::CreateMsgRecoveryPublicCtx(
  AlgId algId) noexcept
{
  MsgRecoveryPublicCtx::Uptr localPtr{ std::make_unique<MsgRecoveryPublicCtxDefaultssl>(algId) };
  return ara::core::Result<MsgRecoveryPublicCtx::Uptr>::FromValue(std::move(localPtr));
}

ara::core::Result<SignerPrivateCtx::Uptr> CryptoProviderSSL::CreateSignerPrivateCtx(AlgId algId) noexcept
{
  SignerPrivateCtx::Uptr localPtr{ std::make_unique<SignerPrivateCtxDefaultssl>(algId) };
  return ara::core::Result<SignerPrivateCtx::Uptr>::FromValue(std::move(localPtr));
}

ara::core::Result<VerifierPublicCtx::Uptr> CryptoProviderSSL::CreateVerifierPublicCtx(AlgId algId) noexcept
{
  VerifierPublicCtx::Uptr localPtr{ std::make_unique<VerifierPublicCtxdefaultssl>(algId) };
  return ara::core::Result<VerifierPublicCtx::Uptr>::FromValue(std::move(localPtr));
}

ara::core::Result<KeyAgreementPrivateCtx::Uptr> CryptoProviderSSL::CreateKeyAgreementPrivateCtx(
  AlgId algId) noexcept
{
  KeyAgreementPrivateCtx::Uptr localPtr{ std::make_unique<KeyAgreementPrivateCtxDefaultssl>(algId) };
  return ara::core::Result<KeyAgreementPrivateCtx::Uptr>::FromValue(std::move(localPtr));
}

ara::core::Result<CryptoObject::Uptrc> CryptoProviderSSL::LoadObject(const IOInterface& container) noexcept
{
  ara::core::Vector<uint8_t> keybuffer;

  if (container.GetPayloadSize() == 0)
  {
    aralog_.LogError() << "Container is empty";
    return ara::core::Result<CryptoObject::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kEmptyContainer);
  }

  if (container.GetPayloadSize() != container.GetValue().size())
  {
    aralog_.LogError() << "Resource size is not equal to the payload size";
    return ara::core::Result<CryptoObject::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kResourceFault);
  }

  if (!container.IsValid())
  {
    aralog_.LogError() << "Underlying resource is modified";
    return ara::core::Result<CryptoObject::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
  }

  if ((container.GetCryptoObjectType() != CryptoObjectType::kSymmetricKey)
    && (container.GetCryptoObjectType() != CryptoObjectType::kPublicKey)
    && (container.GetCryptoObjectType() != CryptoObjectType::kPrivateKey)
    && (container.GetCryptoObjectType() != CryptoObjectType::kSecretSeed)
    && (container.GetCryptoObjectType() != CryptoObjectType::kSecretSeed)
    && (container.GetCryptoObjectType() != CryptoObjectType::kUndefined))
  {
    aralog_.LogError() << "Object type does not belong to any crypto object type";
    return ara::core::Result<CryptoObject::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  keybuffer.resize(container.GetPayloadSize());
  ReadWriteMemRegion keyValue{ container.GetValue() };

  CryptoObject::Uptrc crypObj = std::make_unique<SymmetricKeyImpl>(keyValue, container.GetAllowedUsage(),
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(container.GetPrimitiveId()),
    container.GetPrimitiveId(), container.IsObjectSession(), container.IsObjectExportable(),
    container.GetObjectId());

  return ara::core::Result<CryptoObject::Uptrc>::FromValue(std::move(crypObj));
}

ara::core::Result<SymmetricKey::Uptrc> CryptoProviderSSL::LoadSymmetricKey(
  const IOInterface& container) noexcept
{
  ara::core::Vector<uint8_t> keybuffer;
  if (container.GetPayloadSize() == 0)
  {
    aralog_.LogError() << "Container is empty";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kEmptyContainer);
  }

  if (container.GetPayloadSize() != container.GetValue().size())
  {
    aralog_.LogError() << "Resource size is not equal to the payload size";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kResourceFault);
  }

  if (!container.IsValid())
  {
    aralog_.LogError() << "Underlying resource is modified";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
  }

  if (container.GetCryptoObjectType() != CryptoObjectType::kSymmetricKey)
  {
    aralog_.LogError() << "Object type is not SymmetricKey";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  keybuffer.resize(container.GetPayloadSize());
  ReadWriteMemRegion keyValue{ container.GetValue() };

  SymmetricKey::Uptrc symKey = std::make_unique<SymmetricKeyImpl>(keyValue, container.GetAllowedUsage(),
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(container.GetPrimitiveId()),
    container.GetPrimitiveId(), container.IsObjectSession(), container.IsObjectExportable(),
    container.GetObjectId());

  return ara::core::Result<SymmetricKey::Uptrc>::FromValue(std::move(symKey));
}

ara::core::Result<PublicKey::Uptrc> CryptoProviderSSL::LoadPublicKey(const IOInterface& container) noexcept
{
  ara::core::Vector<uint8_t> keybuffer;

  if (container.GetPayloadSize() == 0)
  {
    aralog_.LogError() << "Container is empty";
    return ara::core::Result<PublicKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kEmptyContainer);
  }

  if (!container.IsValid())
  {
    aralog_.LogError() << "Underlying resource is modified";
    return ara::core::Result<PublicKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
  }

  if (container.GetCryptoObjectType() != CryptoObjectType::kPublicKey)
  {
    aralog_.LogError() << "Object type is not PublicKey";
    return ara::core::Result<PublicKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  keybuffer.resize(container.GetPayloadSize());
  ReadWriteMemRegion keyValue{ container.GetValue() };

  PublicKey::Uptrc pubkey = std::make_unique<PublicKeyImpl>(keyValue, container.GetAllowedUsage(),
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(container.GetPrimitiveId()),
    container.GetPrimitiveId(), container.IsObjectSession(), container.IsObjectExportable(),
    container.GetObjectId());
  return ara::core::Result<PublicKey::Uptrc>::FromValue(std::move(pubkey));
}

ara::core::Result<PrivateKey::Uptrc> CryptoProviderSSL::LoadPrivateKey(const IOInterface& container) noexcept
{
  ara::core::Vector<uint8_t> keybuffer;

  if (container.GetPayloadSize() == 0)
  {
    aralog_.LogError() << "Container is empty";
    return ara::core::Result<PrivateKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kEmptyContainer);
  }

  if (container.GetPayloadSize() < container.GetValue().size())
  {
    aralog_.LogError() << "Resource size is not equal to the payload size";
    return ara::core::Result<PrivateKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kResourceFault);
  }

  if (!container.IsValid())
  {
    aralog_.LogError() << "Underlying resource is modified";
    return ara::core::Result<PrivateKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
  }

  if (container.GetCryptoObjectType() != CryptoObjectType::kPrivateKey)
  {
    aralog_.LogError() << "Object type is not PrivateKey";
    return ara::core::Result<PrivateKey::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  keybuffer.resize(container.GetPayloadSize());
  ReadWriteMemRegion keyValue{ container.GetValue() };

  PrivateKey::Uptrc privekey = std::make_unique<PrivateKeyImpl>(keyValue, container.GetAllowedUsage(),
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(container.GetPrimitiveId()),
    container.GetPrimitiveId(), container.IsObjectSession(), container.IsObjectExportable(),
    container.GetObjectId());
  return ara::core::Result<PrivateKey::Uptrc>::FromValue(std::move(privekey));
}

ara::core::Result<SecretSeed::Uptrc> CryptoProviderSSL::LoadSecretSeed(const IOInterface& container) noexcept
{
  ara::core::Vector<uint8_t> keybuffer;

  if (container.GetPayloadSize() == 0)
  {
    aralog_.LogError() << "Container is empty";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kEmptyContainer);
  }

  if (container.GetPayloadSize() != container.GetValue().size())
  {
    aralog_.LogError() << "Resource size is not equal to the payload size";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kResourceFault);
  }

  if (!container.IsValid())
  {
    aralog_.LogError() << "Underlying resource is modified";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
  }

  if (container.GetCryptoObjectType() != CryptoObjectType::kSecretSeed)
  {
    aralog_.LogError() << "Object type is not SecretSeed";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  keybuffer.resize(container.GetPayloadSize());
  ReadWriteMemRegion keyValue{ container.GetValue() };

  SecretSeed::Uptrc secretObj = std::make_unique<SecretSeedImpl>(keyValue, container.GetAllowedUsage(),
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(container.GetPrimitiveId()),
    container.GetPrimitiveId(), container.IsObjectSession(), container.IsObjectExportable(),
    container.GetObjectId());

  return ara::core::Result<SecretSeed::Uptrc>::FromValue(std::move(secretObj));
}

ara::core::Result<VolatileTrustedContainer::Uptr> CryptoProviderSSL::AllocVolatileContainer(
  std::size_t capacity) noexcept
{
  VolatileTrustedContainer::Uptr container;

  if (capacity == 0)
  {
    container = std::make_unique<VolatileTrustedContainerImpl>(1000);
  }
  else
  {
    container = std::make_unique<VolatileTrustedContainerImpl>(capacity);
  }

  return ara::core::Result<VolatileTrustedContainer::Uptr>::FromValue(std::move(container));
}

ara::core::Result<VolatileTrustedContainer::Uptr> CryptoProviderSSL::AllocVolatileContainer(
  std::pair<AlgId, CryptoObjectType> theObjectDef) noexcept
{
  VolatileTrustedContainer::Uptr container;
  size_t capacity{ 0 };

  if (theObjectDef.second == CryptoObjectType::kSymmetricKey)
  {
    if ((theObjectDef.first == algorithmids::kAES128CBC) || (theObjectDef.first == algorithmids::kAES128ECB)
      || (theObjectDef.first == algorithmids::kAES128CTR) || (theObjectDef.first == algorithmids::kKeyWrap128)
      || (theObjectDef.first == algorithmids::kKeyWrapPad128)
      || (theObjectDef.first == algorithmids::kAES128GCM)
      || (theObjectDef.first == algorithmids::kDrbgCtrBasedRng))
    {
      capacity = 16;
    }

    else if ((theObjectDef.first == algorithmids::kAES192CBC)
      || (theObjectDef.first == algorithmids::kAES192ECB) || (theObjectDef.first == algorithmids::kAES192CTR)
      || (theObjectDef.first == algorithmids::kKeyWrap192)
      || (theObjectDef.first == algorithmids::kKeyWrapPad192)
      || (theObjectDef.first == algorithmids::kAES192GCM))
    {
      capacity = 24;
    }

    else if ((theObjectDef.first == algorithmids::kAES256CBC)
      || (theObjectDef.first == algorithmids::kAES256ECB) || (theObjectDef.first == algorithmids::kAES256CTR)
      || (theObjectDef.first == algorithmids::kKeyWrap256)
      || (theObjectDef.first == algorithmids::kKeyWrapPad256)
      || (theObjectDef.first == algorithmids::kAES192GCM))
    {
      capacity = 32;
    }
  }

  else if (theObjectDef.second == CryptoObjectType::kSecretSeed)
  {
    if ((theObjectDef.first == algorithmids::kAES128CBC) || (theObjectDef.first == algorithmids::kAES128ECB)
      || (theObjectDef.first == algorithmids::kAES128CTR) || (theObjectDef.first == algorithmids::kKeyWrap128)
      || (theObjectDef.first == algorithmids::kKeyWrapPad128)
      || (theObjectDef.first == algorithmids::kAES192CBC) || (theObjectDef.first == algorithmids::kAES192ECB)
      || (theObjectDef.first == algorithmids::kAES192CTR) || (theObjectDef.first == algorithmids::kKeyWrap192)
      || (theObjectDef.first == algorithmids::kKeyWrapPad192)
      || (theObjectDef.first == algorithmids::kAES256CBC) || (theObjectDef.first == algorithmids::kAES256ECB)
      || (theObjectDef.first == algorithmids::kAES256CTR) || (theObjectDef.first == algorithmids::kKeyWrap256)
      || (theObjectDef.first == algorithmids::kKeyWrapPad256) || (theObjectDef.first == algorithmids::kHMAC)
      || (theObjectDef.first == algorithmids::kCMAC128) || (theObjectDef.first == algorithmids::kCMAC192)
      || (theObjectDef.first == algorithmids::kCMAC256) || (theObjectDef.first == algorithmids::ksha256Hash)
      || (theObjectDef.first == algorithmids::kCMAC256) || (theObjectDef.first == algorithmids::kPBKDF2))
    {
      capacity = 16;
    }

    else if ((theObjectDef.first == algorithmids::kAES128GCM)
      || (theObjectDef.first == algorithmids::kAES192GCM) || (theObjectDef.first == algorithmids::kAES256GCM)
      || (theObjectDef.first == algorithmids::kGMAC128) || (theObjectDef.first == algorithmids::kGMAC192)
      || (theObjectDef.first == algorithmids::kGMAC256))
    {
      capacity = 12;
    }
  }

  else if (theObjectDef.second == CryptoObjectType::kSignature)
  {
    if ((theObjectDef.first == algorithmids::kHMAC) || (theObjectDef.first == algorithmids::kCMAC128)
      || (theObjectDef.first == algorithmids::kCMAC192) || (theObjectDef.first == algorithmids::kCMAC256)
      || (theObjectDef.first == algorithmids::kCMAC256) || (theObjectDef.first == algorithmids::kGMAC128)
      || (theObjectDef.first == algorithmids::kGMAC192) || (theObjectDef.first == algorithmids::kGMAC256))
    {
      capacity = 16;
    }

    else if ((theObjectDef.first == algorithmids::kCMAC192) || (theObjectDef.first == algorithmids::kGMAC192))
    {
      capacity = 24;
    }

    else if ((theObjectDef.first == algorithmids::kCMAC256) || (theObjectDef.first == algorithmids::kGMAC256))
    {
      capacity = 32;
    }
  }

  else if (theObjectDef.second == CryptoObjectType::kPrivateKey)
  {
    capacity = 1000;
  }

  else if (theObjectDef.second == CryptoObjectType::kPublicKey)
  {
    capacity = 500;
  }

  if (capacity == 0)
  {
    container = std::make_unique<VolatileTrustedContainerImpl>(1000);
  }
  else
  {
    container = std::make_unique<VolatileTrustedContainerImpl>(capacity);
  }

  return ara::core::Result<VolatileTrustedContainer::Uptr>::FromValue(std::move(container));
}

// ara::core::Result<ara::core::Vector<ara::core::Byte>> CryptoProviderSSL::ExportSecuredObject(const IOInterface& container,
//     SymmetricKeyWrapperCtx& transportContext) noexcept
// {
//   ara::core::String keyStringValue;
//   ReadWriteMemRegion rawKeyValue{container.GetValue()};
//   CryptoObject::COIdentifier objectID;

//   //Check if container is not empty.
//   if(rawKeyValue.size() == 0)
//   {
//     return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
//       ara::crypto::SecurityErrorDomain::Errc::kEmptyContainer);
//   }

//   //Check if ionterface is still valid.
//   if(!container.IsValid())
//   {
//     return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
//       ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
//   }

//   //Create symmetric key for key wrapping.
//   RestrictedUseObject::Uptrc keyPtr = std::make_unique<SymmetricKeyImpl>(rawKeyValue, container.GetAllowedUsage(),
//   AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(container.GetPrimitiveId()), container.GetPrimitiveId(), false, true,
//   container.GetObjectId());

//   auto result = transportContext.WrapKeyMaterial(*keyPtr);

//   if(result.HasValue())
//   {
//     ara::core::Vector<ara::core::Byte> retVal;
//     ara::core::Vector<uint8_t> serialized;
//     ara::com::internal::vsomeip::common::Marshaller marshaller(serialized);
//     ara::core::Vector<ara::core::Byte> encryptValue = result.Value();

//     for(size_t index = 0; index < encryptValue.size(); index++)
//     {
//       keyStringValue.push_back(static_cast<uint8_t>(encryptValue[index]));
//     }
//     objectID.mCOType = container.GetCryptoObjectType();
//     objectID.mCouid = container.GetObjectId();
//     marshaller.marshal(objectID, AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(container.GetPrimitiveId()),
//     container.GetPrimitiveId(), container.GetAllowedUsage(),keyStringValue);

//     for(size_t index = 0; index < serialized.size(); index++)
//     {
//       retVal.push_back(static_cast<ara::core::Byte>(serialized[index]));
//     }
//     return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
//   }
//   else
//   {
//     return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(result.Error());
//   }
// }

// ara::core::Result<void> CryptoProviderSSL::ImportSecuredObject(IOInterface& container, ReadOnlyMemRegion serialized,
//         SymmetricKeyWrapperCtx& transportContext, bool isExportable, CryptoObjectType expectedObject) noexcept
// {
//   static_cast<void>(isExportable);
//   CryptoObject::COIdentifier objectID;
//   ara::core::String primitiveName;
//   CryptoPrimitiveId::AlgId algoId;
//   AllowedUsageFlags usageFlags;
//   ara::core::String objectStringValue;
//   ara::core::Vector<uint8_t> objectValue;

//   try
//   {
//     ara::com::internal::vsomeip::common::Unmarshaller<CryptoObject::COIdentifier, ara::core::String,
//     CryptoPrimitiveId::AlgId, AllowedUsageFlags, ara::core::String> deserialize(serialized.data(), serialized.size());
//     objectID = deserialize.unmarshal<0>();
//     primitiveName = deserialize.unmarshal<1>();
//     algoId = deserialize.unmarshal<2>();
//     usageFlags = deserialize.unmarshal<3>();
//     objectStringValue = deserialize.unmarshal<4>();
//   }
//   catch(...)
//   {
//     aralog_.LogError() << "Unexpected Value";
//     return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnexpectedValue);
//   }

//   char const* strngPtr{objectStringValue.data()};

//   for(size_t index = 0; index < objectStringValue.size(); index++)
//   {
//     objectValue.push_back(strngPtr[index]);
//   }

//   if((expectedObject != CryptoObjectType::kUndefined) && (expectedObject != objectID.mCOType))
//   {
//     aralog_.LogError() << "Bad object type";
//     return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kBadObjectType);
//   }

//   if(!container.IsValid())
//   {
//     aralog_.LogError() << "Underlying resource is modified after opening the iointerface";
//     return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
//   }

//   ReadOnlyMemRegion wrappedKey{objectValue.data(), objectValue.data() + objectValue.size()};
//   ara::core::Result<RestrictedUseObject::Uptrc> result =
//         transportContext.UnwrapKey(wrappedKey, algoId, usageFlags);
//   if(result.HasValue())
//   {
//     RestrictedUseObject::Uptrc cryptoObject = std::move(result).Value();

//     if(container.GetCapacity() < cryptoObject->GetPayloadSize())
//     {
//       aralog_.LogError() << "Insufficient capcity";
//       return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInsufficientCapacity);
//     }
//     if(!container.SetCryptoObject(objectID.mCouid ,objectID.mCOType, usageFlags, algoId, cryptoObject->GetValueInVector()))
//     {
//       aralog_.LogError() << "Iointerface is not opened as writable";
//       return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnreservedResource);
//     }
//   }
//   else
//   {
//     return ara::core::Result<void>::FromError(result.Error());
//   }

//   return ara::core::Result<void>::FromValue();
// }

// ara::core::Result<ara::core::Vector<ara::core::Byte>> CryptoProviderSSL::ExportPublicObject(const IOInterface& container,
//   Serializable::FormatId formatId) noexcept
// {
//   if(container.GetPayloadSize() == 0)
//   {
//     return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
//       ara::crypto::SecurityErrorDomain::Errc::kEmptyContainer);
//   }

//   if((container.GetCryptoObjectType() != CryptoObjectType::kPublicKey) &&
//     (container.GetCryptoObjectType() != CryptoObjectType::kSignature))
//   {
//     return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
//       ara::crypto::SecurityErrorDomain::Errc::kUnexpectedValue);
//   }

//   if(!container.IsValid())
//   {
//         return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
//     ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
//   }

//   PublicKey::Uptrc pubKeyPtr = LoadPublicKey(container).Value();

//   return pubKeyPtr->ExportPublicly(formatId);
// }

// ara::core::Result<void> CryptoProviderSSL::ImportPublicObject(IOInterface& container, ReadOnlyMemRegion serialized, CryptoObjectType expectedObject) noexcept
// {
//   CryptoObject::COIdentifier objectID;
//   ara::core::String primitiveName;
//   CryptoPrimitiveId::AlgId algoId;
//   AllowedUsageFlags usageFlags;
//   ara::core::String objectStringValue;
//   ara::core::Vector<uint8_t> objectValue;

//   try
//   {
//     ara::com::internal::vsomeip::common::Unmarshaller<CryptoObject::COIdentifier, ara::core::String,
//     CryptoPrimitiveId::AlgId, AllowedUsageFlags, ara::core::String> deserialize(serialized.data(), serialized.size());
//     objectID = deserialize.unmarshal<0>();
//     primitiveName = deserialize.unmarshal<1>();
//     algoId = deserialize.unmarshal<2>();
//     usageFlags = deserialize.unmarshal<3>();
//     objectStringValue = deserialize.unmarshal<4>();
//   }
//   catch(...)
//   {
//     aralog_.LogError() << "Unexpected Value";
//     return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnexpectedValue);
//   }

//   char const* strngPtr{objectStringValue.data()};

//   for(size_t index = 0; index < objectStringValue.size(); index++)
//   {
//     objectValue.push_back(strngPtr[index]);
//   }

//   if((objectID.mCOType != expectedObject) && (expectedObject != CryptoObjectType::kUndefined))
//   {
//     aralog_.LogError() << "Bad object type";
//     return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kBadObjectType);
//   }

//   if(container.GetCapacity() < objectValue.size())
//   {
//     aralog_.LogError() << "Insufficient capcity";
//     return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInsufficientCapacity);
//   }

//   if(!container.IsValid())
//   {
//     aralog_.LogError() << "Underlying resource is modified after opening the iointerface";
//     return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
//   }

//   if(!container.SetCryptoObject(objectID.mCouid ,objectID.mCOType, usageFlags, algoId, objectValue))
//   {
//     aralog_.LogError() << "Iointerface is not opened as writable";
//     return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnreservedResource);
//   }

//   return ara::core::Result<void>::FromValue();
// }

ara::core::Result<Signature::Uptrc> CryptoProviderSSL::CreateSignature(
  AlgId signAlgId, ReadOnlyMemRegion value, const RestrictedUseObject& key, AlgId hashAlgId) noexcept
{
  // According to SWS_CRYPT_20760 if signAlgId or hashAlgId arguments specify
  // crypto algorithms different from the signature/MAC/AE/AEAD and message digest respectively
  // then CreateSignature() should return SecurityErrorDomain::kInvalidArgument.
  if (((signAlgId != algorithmids::kHMAC) && (signAlgId != algorithmids::kCMAC128)
        && (signAlgId != algorithmids::kCMAC192) && (signAlgId != algorithmids::kCMAC256)
        && (signAlgId != algorithmids::kGMAC128) && (signAlgId != algorithmids::kGMAC192)
        && (signAlgId != algorithmids::kGMAC256) && (signAlgId != algorithmids::kAES128GCM)
        && (signAlgId != algorithmids::kAES192GCM) && (signAlgId != algorithmids::kAES256GCM))
    || (((signAlgId == algorithmids::kHMAC) && (hashAlgId != algorithmids::ksha256Hash))
         || ((signAlgId != algorithmids::kHMAC) && (hashAlgId != kAlgIdNone))))
  {
    aralog_.LogError() << "Invalid Arugment for AlgId is received!!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kInvalidArgument);
  }

  AlgId const kKeyalgId{ key.GetCryptoPrimitiveId()->GetPrimitiveId() };

  // According to SWS_CRYPT_20760 if signAlgId and hashAlgId arguments specify incompatible
  // algorithms (if signAlgId includes hash function specification) or if a crypto primitive
  // associated with the key argument is incompatible with provided signAlgId or hashAlgId arguments
  // then CreateSignature() should return SecurityErrorDomain::kIncompatibleArguments.
  if (((signAlgId != algorithmids::kHMAC) && (hashAlgId != kAlgIdNone))
    || ((signAlgId == algorithmids::kHMAC) && (hashAlgId == kAlgIdNone))
    || ((kKeyalgId != algorithmids::kHMAC) && (kKeyalgId != algorithmids::kCMAC128)
         && (kKeyalgId != algorithmids::kCMAC192) && (kKeyalgId != algorithmids::kCMAC256)
         && (kKeyalgId != algorithmids::kGMAC128) && (kKeyalgId != algorithmids::kGMAC192)
         && (kKeyalgId != algorithmids::kGMAC256) && (kKeyalgId != algorithmids::kAES128GCM)
         && (kKeyalgId != algorithmids::kAES192GCM) && (kKeyalgId != algorithmids::kAES256GCM)
         && (kKeyalgId != kAlgIdAny)))
  {
    aralog_.LogError() << "Incompatible arguments received!!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kIncompatibleArguments);
  }

  // According to SWS_CRYPT_20760 if the value argument has invalid size (i.e. incompatible with the
  // signAlgId argument) then CreateSignature() should return SecurityErrorDomain::kInvalidInputSize.
  if (((signAlgId == algorithmids::kHMAC) && (value.size() != provider::kSha256HashSize))
    || (((signAlgId == algorithmids::kCMAC128) || (signAlgId == algorithmids::kGMAC128)
          || (signAlgId == algorithmids::kAES128GCM) || (signAlgId == algorithmids::kCMAC192)
          || (signAlgId == algorithmids::kGMAC192) || (signAlgId == algorithmids::kAES192GCM)
          || (signAlgId == algorithmids::kCMAC256) || (signAlgId == algorithmids::kGMAC256)
          || (signAlgId == algorithmids::kAES256GCM))
         && (value.size() != provider::kCryptoProviderAes128BitKeySize)))
  {
    aralog_.LogError() << "Invalid input size!!!!";
    return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  ara::core::Vector<uint8_t> disgetVector{ value.begin(), value.end() };
  ReadWriteMemRegion digest{ disgetVector };

  Signature::Uptrc signObj{ std::make_unique<SignatureImpl>(digest,
    AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(signAlgId), signAlgId, hashAlgId,
    value.size(), key.IsSession(), key.IsExportable(), &key) };

  return ara::core::Result<Signature::Uptrc>::FromValue(std::move(signObj));
}

ara::core::Vector<uint8_t> CryptoProviderSSL::GenerateRandomData(std::uint32_t count)
{
  OSSL_PARAM params[2];
  char cipherName[]{ SN_aes_256_ctr };
  ara::core::Vector<uint8_t> randomData;

  randomData.resize(count);

  EVP_RAND* const kRngInstance{ EVP_RAND_fetch(nullptr, "CTR-DRBG", nullptr) };

  EVP_RAND_CTX* const kRandomNumberGeneratorCtx{ EVP_RAND_CTX_new(kRngInstance, nullptr) };

  params[0] = OSSL_PARAM_construct_utf8_string(OSSL_DRBG_PARAM_CIPHER, cipherName, 0);
  params[1] = OSSL_PARAM_construct_end();

  if (opresults::kSuccess != EVP_RAND_instantiate(kRandomNumberGeneratorCtx, 0, 0, nullptr, 0, params))
  {
    aralog_.LogError() << "RNG instantiation failed!!!!!";
  }

  EVP_RAND_free(kRngInstance);

  if (opresults::kSuccess
    != EVP_RAND_generate(kRandomNumberGeneratorCtx, randomData.data(), count, 0, 0, nullptr, 0))
  {
    aralog_.LogError() << "Random data generation failed!!!!";
  }

  if (opresults::kSuccess != EVP_RAND_uninstantiate(kRandomNumberGeneratorCtx))
  {
    aralog_.LogError() << "Uninstantiation failed for RNG ctx!!!!";
  }

  EVP_RAND_CTX_free(kRandomNumberGeneratorCtx);

  return randomData;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
