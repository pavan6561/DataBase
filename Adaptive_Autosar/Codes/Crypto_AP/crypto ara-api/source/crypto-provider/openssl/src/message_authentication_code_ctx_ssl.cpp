// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : message_authentication_code_ctx_ssl.cpp
//
// Purpose     : This file contains implementation for MessageAuthnCodeCtxDefaultSsl class.
// -----------------------------------------------------------------------------------------------------------

#include "message_authentication_code_ctx_ssl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
MessageAuthnCodeCtxDefaultSsl::MessageAuthnCodeCtxDefaultSsl(CryptoPrimitiveId::AlgId algoId) noexcept :
    MessageAuthnCodeCtx{}, algId_{algoId}
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl constructor";

  // Fetch openssl MAC instance.
  EVP_MAC* const evpMacinstance{EVP_MAC_fetch(nullptr, GetMacAlgoName().data(), nullptr)};

  // Create openssl mac context.
  evpMacctx = EVP_MAC_CTX_new(evpMacinstance);

  // Delete MAC instance.
  EVP_MAC_free(evpMacinstance);
}

MessageAuthnCodeCtxDefaultSsl::~MessageAuthnCodeCtxDefaultSsl() noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl Destructor";

  // Delete MAC context.
  EVP_MAC_CTX_free(evpMacctx);
}

DigestService::Uptr MessageAuthnCodeCtxDefaultSsl::GetDigestService() const noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::GetDigestService()";

  bool isProcessStart{false};
  bool isProcessFinished{false};
  uint8_t const kByteSizeinBits{8};
  uint8_t const kZeroSize{0};
  uint8_t const kNoUsageAllowed{0};

  CryptoObjectUid objectID;

  if (macProcessState_ >= macExecutionStates::kMacStart)
  {
    isProcessStart = true;
  }

  if (macProcessState_ == macExecutionStates::kMacFinal)
  {
    isProcessFinished = true;
  }

  if (symKey_ != nullptr)
  {
    DigestService::Uptr localPtr{std::make_unique<DigestServiceImpl>(algId_, objectID, nullptr,
        symKey_->GetPayloadSize() * kByteSizeinBits, macIv_.size() * kByteSizeinBits,
        symKey_->GetAllowedUsage(), macValue_, isinitialized_, isProcessStart, isProcessFinished)};

    return localPtr;
  }
  else
  {
    DigestService::Uptr localPtr{std::make_unique<DigestServiceImpl>(algId_, objectID, nullptr, kZeroSize,
        macIv_.size() * kByteSizeinBits, kNoUsageAllowed, macValue_, isinitialized_, isProcessStart,
        isProcessFinished)};

    return localPtr;
  }
}

ara::core::Result<void> MessageAuthnCodeCtxDefaultSsl::Start(const SecretSeed& iv) noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::Start() with secret seed";

  AllowedUsageFlags const ivusage{iv.GetAllowedUsage()};

  // If transformation is other than kMacGenerate and kSigGenerate or transformation is kSigGenerate
  // and IV do not have valid permissions to generate signature then return the error code
  // SecurityErrc::kUsageViolation.
  if (((transformation_ != CryptoTransform::kMacGenerate)
          && (transformation_ != CryptoTransform::kSigGenerate))
      || ((transformation_ == CryptoTransform::kSigGenerate) && (!(ivusage & kAllowSignature))))
  {
    aralog_.LogError() << "Given IV do not have permission to create MAC.";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kUsageViolation);
  }

  return Start(static_cast<ReadOnlyMemRegion>(iv.GetSeedValue()));
}

ara::core::Result<void> MessageAuthnCodeCtxDefaultSsl::Start(ReadOnlyMemRegion iv) noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::Start()";

  static uint8_t const kGMACIvLength{12};

  if (isinitialized_ == false)
  {
    // According to SWS_CRYPT_22110 if this ctx is not initialize by calling SetKey()
    // then Start() should return the error as SecurityErrc::kUninitializedContext.
    aralog_.LogError() << "Context is not initialized";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kUninitializedContext);
  }

  // Check if algo ID and iv size are valid for other than GMAC.
  if (((algId_ != algorithmids::kGMAC128) && (algId_ != algorithmids::kGMAC192)
          && (algId_ != algorithmids::kGMAC256))
      && (iv.size() != 0))
  {
    // According to SWS_CRYPT_22110 if implementation does not supports IV and IV size
    // is greater than 0 then Start() should return error as SecurityErrc::kUnsupported.
    aralog_.LogError() << "IV is not supported by the algorithm";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kUnsupported);
  }

  // Check if algo ID and iv size are valid for GMAC.
  else if (((algId_ == algorithmids::kGMAC128) || (algId_ == algorithmids::kGMAC192)
               || (algId_ == algorithmids::kGMAC256))
      && (iv.size() < kGMACIvLength))
  {
    // According to SWS_CRYPT_22110 if implementation supports IV and IV length is
    // not according to the IV length supported by the MAC algorithm then Start should
    // return error as SecurityErrc::kInvalidInputSize.
    aralog_.LogError() << "IV size is  not supported by the algorithm";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kInvalidInputSize);
  }
  else
  {
    // No operation is required.
  }

  if ((algId_ == algorithmids::kGMAC128) || (algId_ == algorithmids::kGMAC192)
      || (algId_ == algorithmids::kGMAC256))
  {
    macIv_.resize(iv.size());

    for (size_t index{0}; index < kGMACIvLength; index++)
    {
      macIv_[index] = iv[index];
    }
  }

  static uint8_t constexpr kNoOfMACParameters{4};

  OSSL_PARAM params[kNoOfMACParameters];

  // Generate parameters according to IV.
  GetParameters(params);

  // Initialize openssl MAC context.
  if (opresults::kSuccess
      != EVP_MAC_init(
             evpMacctx, symKey_->GetSymmetricKey().data(), symKey_->GetSymmetricKey().size(), params))
  {
    aralog_.LogError() << " MAC Init failed for " << GetMacAlgoName().c_str();
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kUninitializedContext);
  }

  aralog_.LogDebug() << " MAC Initialization Done";
  macProcessState_ = macExecutionStates::kMacStart;
  return ara::core::Result<void>();
}

ara::core::Result<void> MessageAuthnCodeCtxDefaultSsl::Update(const RestrictedUseObject& in) noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::Update() with Restricted Used Object";
  return Update(static_cast<ReadOnlyMemRegion>(in.GetValue()));
}

ara::core::Result<void> MessageAuthnCodeCtxDefaultSsl::Update(ReadOnlyMemRegion in) noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::Update()";

  // According to SWS_CRYPT_01205 application can call Update() multiple times,
  // each time providing a new chunk of data. Update should update the hash value
  // calculation with each new chunk. Update should return a SecurityErrorDomain::
  // kProcessingNotStarted error, if Start has not been called before.
  if ((macProcessState_ != macExecutionStates::kMacStart)
      && (macProcessState_ != macExecutionStates::kMacUpdate))
  {
    aralog_.LogError() << " MAC calculation process is not started yet";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
  }

  // Update data in openssl MAC context.
  if (opresults::kSuccess != EVP_MAC_update(evpMacctx, in.data(), in.size()))
  {
    aralog_.LogError() << " MAC update failed ";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
  }

  aralog_.LogDebug() << " MAC Updation is Done ";
  macProcessState_ = macExecutionStates::kMacUpdate;

  return ara::core::Result<void>();
}

ara::core::Result<void> MessageAuthnCodeCtxDefaultSsl::Update(std::uint8_t in) noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::Update() with uint8_t as an argument";

  // According to SWS_CRYPT_01205 application can call Update() multiple times,
  // each time providing a new chunk of data. Update should update the hash value
  // calculation with each new chunk. Update should return a SecurityErrorDomain::
  // kProcessingNotStarted error, if Start has not been called before.
  if ((macProcessState_ != macExecutionStates::kMacStart)
      && (macProcessState_ != macExecutionStates::kMacUpdate))
  {
    aralog_.LogError() << " MAC calculation process is not started yet";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
  }

  // Update data in openssl MAC context.
  if (opresults::kSuccess != EVP_MAC_update(evpMacctx, &in, sizeof(uint8_t)))
  {
    aralog_.LogError() << " MAC update failed ";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
  }

  aralog_.LogDebug() << " MAC Updation is Done ";
  macProcessState_ = macExecutionStates::kMacUpdate;

  return ara::core::Result<void>();
}

ara::core::Result<Signature::Uptrc> MessageAuthnCodeCtxDefaultSsl::Finish(bool makeSignatureObject) noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::Finish()";

  static size_t const kMacSize{32};

  // According to SWS_CRYPT_01207 Finish should return a SecurityErrorDomain::kProcessingNotStarted
  // error, if Start has not been successfully called before.
  if ((macProcessState_ != macExecutionStates::kMacStart)
      && (macProcessState_ != macExecutionStates::kMacUpdate))
  {
    aralog_.LogError() << " MAC calculation process is not started yet";
    return ara::core::Result<Signature::Uptrc>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
  }
  // According to SWS_CRYPT_01207 Finish should return a SecurityErrorDomain::kInvalidUsageOrder
  // error, if Update has not been called successfully after the last call to Start.
  else if (macProcessState_ != macExecutionStates::kMacUpdate)
  {
    aralog_.LogError() << " Invalid Usage Order, Update is not called yet";
    return ara::core::Result<Signature::Uptrc>::FromError(ara::crypto::SecurityErrc::kInvalidUsageOrder);
  }
  else
  {
    // No operation is required.
  }

  macValue_.resize(kMacSize);

  // Finalize the mac calculation process.
  if (opresults::kSuccess != EVP_MAC_final(evpMacctx, macValue_.data(), &outputlen_, kMacSize))
  {
    aralog_.LogError() << " MAC calculation finish failed";
    return ara::core::Result<Signature::Uptrc>::FromError(ara::crypto::SecurityErrc::kProcessingNotStarted);
  }

  aralog_.LogDebug() << " MAC Finish Done ";

  Signature::Uptrc signKey{nullptr};

  if (makeSignatureObject == true)
  {
    // According to SWS_CRYPT_22115 if the buffered digest belongs to a MAC/HMAC/AE/AEAD
    // context initialized by a key without kAllowSignature permission, and
    // makeSignatureObject == true then Finish() should return error as SecurityErrc::kUsageViolation.
    if ((symKey_->GetAllowedUsage() & kAllowSignature) == opresults::kFailure)
    {
      aralog_.LogError() << " Allowed usage is not given for signature object";
      return ara::core::Result<Signature::Uptrc>::FromError(ara::crypto::SecurityErrc::kUsageViolation);
    }

    bool isSession{true};
    bool isExportable{true};
    ReadWriteMemRegion signatureVal{macValue_};

    if (algId_ == algorithmids::kHMAC)
    {
      // Create signature object.
      signKey = std::make_unique<SignatureImpl>(signatureVal, GetMacAlgoName(), algId_,
          algorithmids::ksha256Hash, kMacSize, isSession, isExportable, symKey_);
    }
    else
    {
      // Create signature object.
      signKey = std::make_unique<SignatureImpl>(
          signatureVal, GetMacAlgoName(), algId_, kAlgIdNone, kMacSize, isSession, isExportable, symKey_);
    }
  }

  macProcessState_ = macExecutionStates::kMacFinal;

  return ara::core::Result<Signature::Uptrc>(std::move(signKey));
}

ara::core::Result<void> MessageAuthnCodeCtxDefaultSsl::Reset() noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::Reset()";

  macProcessState_ = macExecutionStates::kMacIdle;
  isinitialized_ = false;
  transformation_ = CryptoTransform::kMacGenerate;
  outputlen_ = 0;
  macValue_.clear();
  macIv_.clear();
  symKey_ = nullptr;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> MessageAuthnCodeCtxDefaultSsl::SetKey(
    const SymmetricKey& key, CryptoTransform transform) noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::SetKey()";

  size_t const expectedKeySize{GetKeySizefromAlgoId(algId_)};
  size_t const actualKeySize{key.GetPayloadSize()};

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_22118 if the provided key object is incompatible with
  // this symmetric key context
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)) || (expectedKeySize != actualKeySize))
  {
    aralog_.LogError() << "This crypto object does not belongs to MAC ctx.";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // If transform given for this context is not kMacGenerate and kSigGenerate or transform given for this
  // context is kSignGenerate and key does not have required permissions to generate signature then return
  // error for usage violation.
  if (((transform != CryptoTransform::kMacGenerate) && (transform != CryptoTransform::kSigGenerate))
      || ((transform == CryptoTransform::kSigGenerate) && (!(setkeyusage & kAllowSignature))))
  {
    aralog_.LogError() << "Given key usage does not match with transform";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kUsageViolation);
  }

  transformation_ = transform;
  symKey_ = &key;
  isinitialized_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> MessageAuthnCodeCtxDefaultSsl::GetDigest(
    std::size_t offset) const noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::GetDigest()";

  ara::core::Vector<ara::core::Byte> digestValue;

  // According SWS_CRYPT_01220 GetDigest should return a SecurityErrorDomain::kProcessingNotFinished
  // error, if Finish has not been called for the current digest value calculation.
  if (macProcessState_ != macExecutionStates::kMacFinal)
  {
    aralog_.LogError() << "Cannot get digest value as Finish is not called yet";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrc::kProcessingNotFinished);
  }

  // According to SWS_CRYPT_22116 if the buffered digest belongs to a MAC/HMAC/AE/AEAD
  // context initialized by a key without kAllowSignature permission this GetDigest
  // should return SecurityErrorDomain::kUsageViolation.
  if ((symKey_->GetAllowedUsage() & kAllowSignature) == opresults::kFailure)
  {
    aralog_.LogError() << " Allowed usage is not given for signature object";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrc::kUsageViolation);
  }

  digestValue.resize(outputlen_ - offset);

  for (size_t index{offset}; index < outputlen_; index++)
  {
    digestValue[index] = static_cast<ara::core::Byte>(macValue_[index]);
  }

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(digestValue);
}

ara::core::Result<bool> MessageAuthnCodeCtxDefaultSsl::Check(const Signature& expected) const noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::Check()";

  ReadWriteMemRegion const expectedSignatureVal{expected.GetSignatureValue()};

  // According to SWS_CRYPT_22119 Check should return a SecurityErrorDomain::kProcessingNotFinished
  // error, if Finish has not been called for the current digest value calculation.
  if (macProcessState_ != macExecutionStates::kMacFinal)
  {
    aralog_.LogError() << "Can not compare signature as Finish is not called yet";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrc::kProcessingNotFinished);
  }

  // According to SWS_CRYPT_22119 if the provided "signature" object was produced by
  // another crypto primitive type then Check should return SecurityErrorDomain::kIncompatibleObject
  if (expected.GetCryptoPrimitiveId()->GetPrimitiveId() != algId_)
  {
    aralog_.LogError() << " Given Object is not a signature object";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrc::kIncompatibleObject);
  }

  bool ismatched{true};

  for (size_t index{0}; index < expectedSignatureVal.size(); index++)
  {
    if (index > outputlen_)
    {
      ismatched = false;
      aralog_.LogDebug() << "Signature length mismatched!!!!";
      break;
    }
    if (expectedSignatureVal[index] != macValue_[index])
    {
      ismatched = false;
      aralog_.LogDebug() << "Signature value mismatched!!!!";
      break;
    }
  }

  if (!ismatched)
  {
    return ara::core::Result<bool>::FromValue(false);
  }

  return ara::core::Result<bool>::FromValue(true);
}

ara::core::String MessageAuthnCodeCtxDefaultSsl::GetMacAlgoName() const noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::GetMacAlgoName()";

  ara::core::String algoname;

  if (algId_ == algorithmids::kHMAC)
  {
    algoname = "hmac";
  }
  else if ((algId_ == algorithmids::kCMAC128) || (algId_ == algorithmids::kCMAC192)
      || (algId_ == algorithmids::kCMAC256))
  {
    algoname = "cmac";
  }
  else if ((algId_ == algorithmids::kGMAC128) || (algId_ == algorithmids::kGMAC192)
      || (algId_ == algorithmids::kGMAC256))
  {
    algoname = "gmac";
  }
  else
  {
    algoname = " ";
  }

  return algoname;
}

void MessageAuthnCodeCtxDefaultSsl::GetParameters(OSSL_PARAM* params) noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::GetParameters()";

  static char stSHA[]{"sha-256"};
  static char stAES128CBC[]{"aes-128-cbc"};
  static char stAES192CBC[]{"aes-192-cbc"};
  static char stAES256CBC[]{"aes-256-cbc"};
  static char stAES128GCM[]{"aes-128-gcm"};
  static char stAES192GCM[]{"aes-192-gcm"};
  static char stAES256GCM[]{"aes-256-gcm"};
  uint8_t paramIndex{0};

  switch (algId_)
  {
  case algorithmids::kHMAC:
  {
    params[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, stSHA, sizeof(stSHA));
    params[paramIndex] = OSSL_PARAM_construct_end();
  }
  break;

  case algorithmids::kCMAC128:
  {
    params[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, stSHA, sizeof(stSHA));
    params[paramIndex++]
        = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_CIPHER, stAES128CBC, sizeof(stAES128CBC));
    params[paramIndex] = OSSL_PARAM_construct_end();
  }
  break;

  case algorithmids::kCMAC192:
  {
    params[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, stSHA, sizeof(stSHA));
    params[paramIndex++]
        = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_CIPHER, stAES192CBC, sizeof(stAES192CBC));
    params[paramIndex] = OSSL_PARAM_construct_end();
  }
  break;

  case algorithmids::kCMAC256:
  {
    params[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, stSHA, sizeof(stSHA));
    params[paramIndex++]
        = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_CIPHER, stAES256CBC, sizeof(stAES256CBC));
    params[paramIndex] = OSSL_PARAM_construct_end();
  }
  break;

  case algorithmids::kGMAC128:
  {
    params[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, stSHA, sizeof(stSHA));
    params[paramIndex++]
        = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_CIPHER, stAES128GCM, sizeof(stAES128GCM));
    params[paramIndex++] = OSSL_PARAM_construct_octet_string(OSSL_MAC_PARAM_IV, macIv_.data(), macIv_.size());
    params[paramIndex] = OSSL_PARAM_construct_end();
  }
  break;

  case algorithmids::kGMAC192:
  {
    params[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, stSHA, sizeof(stSHA));
    params[paramIndex++]
        = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_CIPHER, stAES192GCM, sizeof(stAES192GCM));
    params[paramIndex++] = OSSL_PARAM_construct_octet_string(OSSL_MAC_PARAM_IV, macIv_.data(), macIv_.size());
    params[paramIndex++] = OSSL_PARAM_construct_end();
  }
  break;

  case algorithmids::kGMAC256:
  {
    params[paramIndex++] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, stSHA, sizeof(stSHA));
    params[paramIndex++]
        = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_CIPHER, stAES256GCM, sizeof(stAES256GCM));
    params[paramIndex++] = OSSL_PARAM_construct_octet_string(OSSL_MAC_PARAM_IV, macIv_.data(), macIv_.size());
    params[paramIndex] = OSSL_PARAM_construct_end();
  }
  break;

  default:
  {
  }
  break;
  }
}

size_t MessageAuthnCodeCtxDefaultSsl::GetKeySizefromAlgoId(AlgId targetAlgId) const noexcept
{
  aralog_.LogDebug() << "In MessageAuthnCodeCtxDefaultSsl::GetKeySizefromAlgoId()";

  static size_t const kMacKeySize128Bit{16};
  static size_t const kMacKeySize192Bit{24};
  static size_t const kMacKeySize256Bit{32};

  size_t keySize;

  // If target algorithm Id requires 128 bit key.
  if ((targetAlgId == algorithmids::kHMAC) || (targetAlgId == algorithmids::kCMAC128)
      || (targetAlgId == algorithmids::kGMAC128))
  {
    keySize = kMacKeySize128Bit;
  }

  // If target algorithm Id requires 256 bit key.
  else if ((targetAlgId == algorithmids::kCMAC256) || (targetAlgId == algorithmids::kGMAC256))
  {
    keySize = kMacKeySize256Bit;
  }

  // If target algorithm Id requires 192 bit key.
  else if ((targetAlgId == algorithmids::kCMAC192) || (targetAlgId == algorithmids::kCMAC192))
  {
    keySize = kMacKeySize192Bit;
  }

  // else use default size.
  else
  {
    keySize = kMacKeySize128Bit;
  }

  return keySize;
}
} // namespace cryp
} // namespace crypto
} // namespace ara
