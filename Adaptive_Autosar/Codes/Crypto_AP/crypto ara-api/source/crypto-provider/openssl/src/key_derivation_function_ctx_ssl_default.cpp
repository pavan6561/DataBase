// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_derivation_function_ctx_ssl_default.cpp
//
// Purpose     : This file contains implementation for KeyDerivationFunctionCtxSSLPBKDF class.
// -----------------------------------------------------------------------------------------------------------

#include "key_derivation_function_ctx_ssl_default.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for pbkdf2.
namespace pbkdf2
{
uint8_t constexpr kSymmetricIvSize{16};
} // namespace pbkdf2

KeyDerivationFunctionCtxSSLPBKDF::KeyDerivationFunctionCtxSSLPBKDF() noexcept : KeyDerivationFunctionCtx{}
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF constructor";

  // Create openssl KDF instance for PBKDF2.
  EVP_KDF* const kdfInstance{EVP_KDF_fetch(nullptr, "PBKDF2", nullptr)};

  // Create openssl KDF context for PBKDF2.
  kdfctx_ = EVP_KDF_CTX_new(kdfInstance);

  // Delete KDF instance
  EVP_KDF_free(kdfInstance);
}

KeyDerivationFunctionCtxSSLPBKDF::~KeyDerivationFunctionCtxSSLPBKDF()
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF Destructor";

  // Delete openssl KDF context for PBKDF2.
  EVP_KDF_CTX_free(kdfctx_);
}

ara::core::Result<void> KeyDerivationFunctionCtxSSLPBKDF::Init(ReadOnlyMemRegion targetKeyId,
    AlgId targetAlgId, AllowedUsageFlags allowedUsage, ReadOnlyMemRegion ctxLabel) noexcept
{
  static_cast<void>(ctxLabel);

  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::Init()";

  // According to SWS_CRYPT_21523 Check if any other flag other than the flags required for the symmetric key
  // is set. If not Init() should return SecurityErrorDomain::kUsageViolation.
  if ((allowedUsage & (~allowedUsageForKeyMaterial_)) == opresults::kFailure)
  {
    allowedUsageForDerivedKey_ = allowedUsage;
  }
  else
  {
    aralog_.LogError() << "Given usage flag is not allowed by key derivation function";
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
  }

  targetAlgorithmId_ = targetAlgId;

  // According to SWS_CRYPT_21523 if targetAlgId specifies a cryptographic algorithm different
  // from a symmetric one with key length supported by this ctx then Init
  // shall return SecurityErrorDomain::kIncompatibleArguments.
  if (GetKeySizefromAlgoId(targetAlgorithmId_) == opresults::kFailure)
  {
    aralog_.LogError() << "Given algorithm does not support symmetric key operation";
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kIncompatibleArguments);
  }

  aralog_.LogDebug() << " Key Size : " << GetKeySizefromAlgoId(targetAlgorithmId_);

  targetKeyIdValue_.resize(targetKeyId.size());

  // Copy target key Id.
  for (size_t index{0}; index < targetKeyId.size(); index++)
  {
    targetKeyIdValue_[index] = targetKeyId[index];
  }

  ConfigureKdfCtx();

  isCtxInitialized_ = true;

  return ara::core::Result<void>::FromValue();
}

std::uint32_t KeyDerivationFunctionCtxSSLPBKDF::ConfigIterations(std::uint32_t iterations) noexcept
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::ConfigIterations()";
  static uint8_t const kdefaultprovidedIterationcount{0};
  if (iterations == kdefaultprovidedIterationcount)
  {
    aralog_.LogWarn() << "Given iteration value is 0, applying default iterations!!!";
    itr_ = configInstance_->GetKdfIterationValue();
  }
  else
  {
    itr_ = iterations;
  }

  aralog_.LogDebug() << "Iteration value is : " << itr_;

  return itr_;
}

ara::core::Result<void> KeyDerivationFunctionCtxSSLPBKDF::AddSalt(ReadOnlyMemRegion salt) noexcept
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::AddSalt()";
  static uint8_t constexpr ksizeZero{0};
  if (salt.size() > ksizeZero)
  {
    kdfSalt_.resize(salt.size());
    for (size_t index{0}; index < salt.size(); index++)
    {
      kdfSalt_.at(index) = salt[index];
    }
  }
  else
  {
    ara::core::String seedValue{
        ara::crypto::config::CryptoConfig::GetCryptoConfigInstance()->GetRngSeedValue()};
    ara::core::Vector<uint8_t> seedValueVtr{seedValue.begin(), seedValue.end()};
    kdfSalt_.resize(seedValueVtr.size());
    for (size_t index{0}; index <= salt.size(); index++)
    {
      kdfSalt_.at(index) = seedValueVtr[index];
    }
  }
  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> KeyDerivationFunctionCtxSSLPBKDF::AddSecretSalt(const SecretSeed& salt) noexcept
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::AddSecretSalt()";
  return AddSalt(static_cast<ReadOnlyMemRegion>(salt.GetSeedValue()));
}

ara::core::Result<void> KeyDerivationFunctionCtxSSLPBKDF::SetSourceKeyMaterial(
    const RestrictedUseObject& sourceKM) noexcept
{
  // Set all the flags required for symmetric key.
  AllowedUsageFlags const allowedFlags{kAllowDataEncryption | kAllowDataDecryption | kAllowRngInit
      | kAllowDerivedDataEncryption | kAllowDerivedDataDecryption | kAllowDerivedSignature
      | kAllowDerivedVerification | kAllowDerivedRngInit | kAllowSignature | kAllowKdfMaterial};

  CryptoObjectType const sourceKMType{sourceKM.GetObjectId().mCOType};
  AlgId const sourceKMPremitiveId{sourceKM.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_21525 if the provided key object is incompatible with this
  // symmetric key context then SetSourceKeyMaterial() should return SecurityErrorDomain::kIncompatibleObject
  if (((sourceKMType != CryptoObjectType::kSymmetricKey) && (sourceKMType != CryptoObjectType::kSecretSeed))
      || (sourceKMPremitiveId != algorithmids::kPBKDF2))
  {
    aralog_.LogError() << "Incompatible Object!!!";
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const sourceKMusage{sourceKM.GetAllowedUsage()};

  // According to SWS_CRYPT_21525 if deriving a key is prohibited by the "allowed usage"
  // restrictions of the  provided source key-material then SetSourceKeyMaterial()
  // should return SecurityErrorDomain::kUsageViolation
  if (!(sourceKMusage & allowedFlags))
  {
    aralog_.LogError() << "Usage permissions not allowed";
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kUsageViolation);
  }

  size_t const sourceKMSize{sourceKM.GetPayloadSize()};

  // According to SWS_CRYPT_21525 if key length of the sourceKm is below of
  // an internally defined limitation then SetSourceKeyMaterial()
  // should return SecurityErrorDomain::kBruteForceRisk
  if (sourceKMSize < pbkdf2::kSymmetricIvSize)
  {
    aralog_.LogError() << "Invalid Key Matrial Size";
    return ara::core::Result<void>::FromError(SecurityErrorDomain::Errc::kBruteForceRisk);
  }

  ReadOnlyMemRegion const inValue{sourceKM.GetValue()};
  password_.resize(inValue.size());

  allowedUsageForKeyMaterial_ = sourceKM.GetAllowedUsage();

  allowedUsageForDerivedKey_ = allowedUsageForKeyMaterial_;

  for (size_t index{0}; index < inValue.size(); index++)
  {
    password_[index] = inValue[index];
  }

  ConfigureKdfCtx();

  isSourceKMavailable_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<SymmetricKey::Uptrc> KeyDerivationFunctionCtxSSLPBKDF::DeriveKey(
    bool isSession, bool isExportable) const noexcept
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::DeriveKey()";

  ara::core::Vector<uint8_t> key;

  size_t const keySize{GetKeySizefromAlgoId(targetAlgorithmId_)};

  // According to SWS_CRYPT_21515 if ctx is not initialized before calling
  // DeriveKey(), this method should return SecurityErrorDomain::kUninitializedContext.
  if (false == isCtxInitialized_)
  {
    aralog_.LogError() << "Context is not initialized";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kUninitializedContext);
  }

  key.resize(keySize);

  // Derive symmetric key.
  int32_t const ret{EVP_KDF_derive(kdfctx_, key.data(), keySize, params_)};

  if (ret == 0)
  {
    aralog_.LogError() << "Key derivation failed";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // Create read write memory region object to store into a crypto object.
  ReadWriteMemRegion const derivedKey{key};

  static uint16_t constexpr khigherByteMask{0x00FF};

  CryptoObjectUid objectID;

  // Create symmetric key object.
  SymmetricKey::Uptrc symmetricKeyInstance{std::make_unique<SymmetricKeyImpl>(derivedKey,
      allowedUsageForDerivedKey_ & khigherByteMask, algoIdentification_->GetAlgoName(targetAlgorithmId_),
      targetAlgorithmId_, isSession, isExportable, objectID)};

  return ara::core::Result<SymmetricKey::Uptrc>(std::move(symmetricKeyInstance));
}

ara::core::Result<SecretSeed::Uptrc> KeyDerivationFunctionCtxSSLPBKDF::DeriveSeed(
    bool isSession, bool isExportable) const noexcept
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::DeriveSeed()";

  // According to SWS_CRYPT_21516 if ctx is not initialized before calling
  // DeriveSeed(), this method should return SecurityErrorDomain::kUninitializedContext.
  if (false == isCtxInitialized_)
  {
    aralog_.LogError() << "Context is not initialized";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
  }

  size_t const ivSize{GetIVSizefromAlgoId(targetAlgorithmId_)};
  size_t const keySize{GetKeySizefromAlgoId(targetAlgorithmId_)};

  ara::core::Vector<uint8_t> kdfIv;
  ara::core::Vector<uint8_t> derivedKeyIv;

  kdfIv.resize(ivSize);
  derivedKeyIv.resize(keySize + ivSize);

  // For every salt and password combination there should be unique
  // values of key and IV. So derive key and IV together and extract IV from
  // the derived value.
  int32_t const ret{EVP_KDF_derive(kdfctx_, derivedKeyIv.data(), keySize + ivSize, params_)};

  if (!ret)
  {
    aralog_.LogError() << "IV derivation failed";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(SecurityErrorDomain::Errc::kUninitializedContext);
  }

  for (size_t index{0}; index < ivSize; index++)
  {
    kdfIv[index] = derivedKeyIv[keySize + index];
  }

  // Create read write memory region object to store into a crypto object.
  ReadWriteMemRegion const derivedseed{kdfIv};

  static uint8_t constexpr kLowerByteMast{16};

  CryptoObjectUid objectID;

  // Create secret seed object.
  SecretSeed::Uptrc secretSeedInstance{std::make_unique<SecretSeedImpl>(derivedseed,
      allowedUsageForDerivedKey_ >> kLowerByteMast, algoIdentification_->GetAlgoName(targetAlgorithmId_),
      targetAlgorithmId_, isSession, isExportable, objectID)};

  return ara::core::Result<SecretSeed::Uptrc>(std::move(secretSeedInstance));
}

ExtensionService::Uptr KeyDerivationFunctionCtxSSLPBKDF::GetExtensionService() const noexcept
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::GetExtensionService()";
  uint8_t const kByteSizeinBits{8};

  CryptoObjectUid objectID;

  ExtensionService::Uptr localPtr{std::make_unique<ExtensionServiceImpl>(algorithmids::kPBKDF2,
      password_.size() * kByteSizeinBits, allowedUsageForKeyMaterial_, objectID, isCtxInitialized_)};

  return localPtr;
}

ara::core::Result<void> KeyDerivationFunctionCtxSSLPBKDF::Reset() noexcept
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::Reset()";
  kdfSalt_.clear();
  targetKeyIdValue_.clear();
  password_.clear();
  itr_ = configInstance_->GetKdfIterationValue();
  allowedUsageForDerivedKey_ = kAllowKdfMaterialAnyUsage;
  allowedUsageForKeyMaterial_ = kAllowKdfMaterialAnyUsage;
  targetAlgorithmId_ = kAlgIdUndefined;
  isCtxInitialized_ = false;
  return ara::core::Result<void>::FromValue();
}

std::size_t KeyDerivationFunctionCtxSSLPBKDF::GetTargetKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::GetTargetKeyBitLength()";
  static uint8_t constexpr kNofBitsInBytes{8};
  return (GetKeySizefromAlgoId(targetAlgorithmId_) * kNofBitsInBytes);
}

size_t KeyDerivationFunctionCtxSSLPBKDF::GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept
{
  aralog_.LogDebug() << "In KeyDerivationFunctionCtxSSLPBKDF::GetKeySizefromAlgoId()";

  static uint8_t const kKdfKeySize128Bit{16};
  static uint8_t const kKdfKeySize192Bit{24};
  static uint8_t const kKdfKeySize256Bit{32};

  // If target algorithm Id requires 128 bit key.
  if ((targetAlgId == algorithmids::kAES128CBC) || (targetAlgId == algorithmids::kAES128ECB)
      || (targetAlgId == algorithmids::kAES128CTR) || (targetAlgId == algorithmids::kAES128GCM)
      || (targetAlgId == algorithmids::kKeyWrap128) || (targetAlgId == algorithmids::kKeyWrapPad128)
      || (targetAlgId == algorithmids::kHMAC) || (targetAlgId == algorithmids::kCMAC128)
      || (targetAlgId == algorithmids::kGMAC128))
  {
    return kKdfKeySize128Bit;
  }

  // If target algorithm Id requires 192 bit key.
  else if ((targetAlgId == algorithmids::kAES192CBC) || (targetAlgId == algorithmids::kAES192CTR)
      || (targetAlgId == algorithmids::kAES192ECB) || (targetAlgId == algorithmids::kAES192GCM)
      || (targetAlgId == algorithmids::kKeyWrap192) || (targetAlgId == algorithmids::kKeyWrapPad192)
      || (targetAlgId == algorithmids::kCMAC192) || (targetAlgId == algorithmids::kGMAC192))
  {
    return kKdfKeySize192Bit;
  }

  // If target algorithm Id requires 256 bit key.
  else if ((targetAlgId == algorithmids::kAES256ECB) || (targetAlgId == algorithmids::kAES256CBC)
      || (targetAlgId == algorithmids::kAES256CTR) || (targetAlgId == algorithmids::kAES256GCM)
      || (targetAlgId == algorithmids::kKeyWrap256) || (targetAlgId == algorithmids::kKeyWrapPad256)
      || (targetAlgId == algorithmids::kCMAC256) || (targetAlgId == algorithmids::kGMAC256))
  {
    return kKdfKeySize256Bit;
  }

  else if (targetAlgId == kAlgIdAny)
  {
    return password_.size();
  }
  else
  {
    return 0;
  }
}

size_t KeyDerivationFunctionCtxSSLPBKDF::GetIVSizefromAlgoId(AlgId targetAlgId) const noexcept
{
  aralog_.LogDebug() << "KeyDerivationFunctionCtxSSLPBKDF::GetIVSizefromAlgoId()";

  static uint8_t const kGcmIvsize{12};

  // Check if target algorithm belongs to GCM mode, which requires 12 bit IV.
  if ((targetAlgId == algorithmids::kAES128CBC) || (targetAlgId == algorithmids::kAES128ECB)
      || (targetAlgId == algorithmids::kAES128CTR) || (targetAlgId == algorithmids::kKeyWrap128)
      || (targetAlgId == algorithmids::kKeyWrapPad128) || (targetAlgId == algorithmids::kAES192CBC)
      || (targetAlgId == algorithmids::kAES192CTR) || (targetAlgId == algorithmids::kAES192ECB)
      || (targetAlgId == algorithmids::kKeyWrap192) || (targetAlgId == algorithmids::kKeyWrapPad192)
      || (targetAlgId == algorithmids::kAES256ECB) || (targetAlgId == algorithmids::kAES256CBC)
      || (targetAlgId == algorithmids::kAES256CTR) || (targetAlgId == algorithmids::kKeyWrap256)
      || (targetAlgId == algorithmids::kKeyWrapPad256) || (targetAlgId == kAlgIdAny))
  {
    return pbkdf2::kSymmetricIvSize;
  }
  else if ((targetAlgId == algorithmids::kAES128GCM) || (targetAlgId == algorithmids::kAES192GCM)
      || (targetAlgId == algorithmids::kAES256GCM) || (targetAlgId == algorithmids::kGMAC128)
      || (targetAlgId == algorithmids::kGMAC192) || (targetAlgId == algorithmids::kGMAC256))
  {
    return kGcmIvsize;
  }
  else
  {
    return 0;
  }
}

void KeyDerivationFunctionCtxSSLPBKDF::ConfigureKdfCtx() noexcept
{
  aralog_.LogDebug() << "KeyDerivationFunctionCtxSSLPBKDF::ConfigureKdfCtx()";

  static char sha[]{"sha-256"};
  static char hmac[]{"hmac"};

  uint8_t paramIndex{0};

  // Set password for KDF in openssl
  params_[paramIndex++]
      = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_PASSWORD, password_.data(), password_.size());

  // Set salt for KDF in openssl
  params_[paramIndex++]
      = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT, kdfSalt_.data(), kdfSalt_.size());

  // Set iteration for KDF in openssl
  params_[paramIndex++] = OSSL_PARAM_construct_uint(OSSL_KDF_PARAM_ITER, &itr_);

  // Set digest for KDF in openssl
  params_[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST, sha, sizeof(sha));

  // Set mac for KDF in openssl
  params_[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_MAC, hmac, sizeof(hmac));

  // Set additional properties in openssl
  params_[paramIndex++] = OSSL_PARAM_construct_utf8_string(
      OSSL_KDF_PARAM_PROPERTIES, targetKeyIdValue_.data(), targetKeyIdValue_.size());

  // Specify the end of parameters.
  params_[paramIndex] = OSSL_PARAM_construct_end();
}

} // namespace cryp
} // namespace crypto
} // namespace ara