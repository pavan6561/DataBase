// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : msg_recovery_public_ctx_default_ssl.cpp
//
// Purpose     : This file contains implementation for MsgRecoveryPublicCtxDefaultssl class.
// -----------------------------------------------------------------------------------------------------------

#include "msg_recovery_public_ctx_default_ssl.h"

#include <iomanip>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
ExtensionService::Uptr MsgRecoveryPublicCtxDefaultssl::GetExtensionService() const noexcept
{
  aralog_.LogDebug() << "MsgRecoveryPublicCtxDefaultssl::GetExtensionService()";

  CryptoObjectUid objectID;

  ExtensionService::Uptr localPtr{std::make_unique<ExtensionServiceImpl>(
      algId_, 2048, (kAllowDataDecryption | kAllowVerification), objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<void> MsgRecoveryPublicCtxDefaultssl::SetKey(const PublicKey& key) noexcept
{
  aralog_.LogDebug() << "In MsgRecoveryPublicCtxDefaultssl::SetKey()";

  AlgId const keyAlgId{key.GetCryptoPrimitiveId()->GetPrimitiveId()};

  // According to SWS_CRYPT_22211 if the provided key object is incompatible
  // with this Publickey key context then SetKey() should return SecurityErrorDomain::kIncompatibleObject.
  if (((keyAlgId != algId_) && (keyAlgId != kAlgIdAny)))
  {
    aralog_.LogError() << "This crypto object does not belongs to MsgRecoveryPublicCtxDefaultssl";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  AllowedUsageFlags const setkeyusage{key.GetAllowedUsage()};

  // According to SWS_CRYPT_22211 if the transformation type associated with this context
  // is prohibited by the "allowed usage" restrictions of provided key object then
  // SetKey should return SecurityErrorDomain::kUsageViolation
  if (!(setkeyusage & (kAllowDataDecryption | kAllowVerification)))
  {
    aralog_.LogError() << "This key is not allowed to use for signature production";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  publicKey_ = key.GetPublicKey();

  isInitialize_ = true;

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> MsgRecoveryPublicCtxDefaultssl::Reset() noexcept
{
  aralog_.LogDebug() << "In MsgRecoveryPublicCtxDefaultssl::Reset()";
  isInitialize_ = false;
  publicKey_ = nullptr;
  return ara::core::Result<void>::FromValue();
}

std::size_t MsgRecoveryPublicCtxDefaultssl::GetMaxInputSize(bool suppressPadding) const noexcept
{
  aralog_.LogDebug() << "In MsgRecoveryPublicCtxDefaultssl::GetMaxInputSize()";
  uint16_t const kMaxInputSize{402};
  static_cast<void>(suppressPadding);
  return kMaxInputSize;
}

std::size_t MsgRecoveryPublicCtxDefaultssl::GetMaxOutputSize(bool suppressPadding) const noexcept
{
  aralog_.LogDebug() << "In MsgRecoveryPublicCtxDefaultssl::GetMaxOutputSize()";
  static_cast<void>(suppressPadding);
  uint16_t const kMaxOutputSize{32};
  return kMaxOutputSize;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> MsgRecoveryPublicCtxDefaultssl::DecodeAndVerify(
    ReadOnlyMemRegion in) const noexcept
{
  aralog_.LogDebug() << "In MsgRecoveryPublicCtxDefaultssl::DecodeAndVerify()";
  size_t messageLength{0};

  char* name{nullptr};
  char* header{nullptr};
  unsigned char* data{nullptr};
  long datalength{0};

  // According to SWS_CRYPT_22215 if if the context was not initialized by a key value
  // then SignAndEncode() should return SecurityErrorDomain::kUninitializedContext.
  if (isInitialize_ == false)
  {
    aralog_.LogError() << "Contex is not initialized yet!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  // According to SWS_CRYPT_22215 if input size is not same as the maximum input size then DecodeAndVerify()
  // should return SecurityErrorDomain::kInvalidInputSize.
  if (in.size() != GetMaxInputSize(false))
  {
    aralog_.LogError() << "Input size is invalid!!!";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  BIO* signatureBIO = BIO_new(BIO_s_mem());

  BIO_write(signatureBIO, in.data(), in.size());

  PEM_read_bio(signatureBIO, &name, &header, &data, &datalength);

  BIO_free(signatureBIO);

  EVP_PKEY_CTX* keyctx = EVP_PKEY_CTX_new(publicKey_, NULL);

  ara::core::Vector<uint8_t> message;

  if (1 != EVP_PKEY_verify_recover_init(keyctx))
  {
    aralog_.LogError() << "Receovery init failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  ConfigureVerifyCtx(keyctx);

  if (1 != EVP_PKEY_verify_recover(keyctx, NULL, &messageLength, data, datalength))
  {
    aralog_.LogError() << "Receovery failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  message.resize(messageLength);

  if (1 != EVP_PKEY_verify_recover(keyctx, message.data(), &messageLength, data, datalength))
  {
    aralog_.LogError() << "Receovery failed";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidInputSize);
  }

  message.resize(messageLength);

  ara::core::Vector<ara::core::Byte> retVal{message.begin(), message.end()};

  OPENSSL_free(name);
  OPENSSL_free(header);
  OPENSSL_free(data);
  EVP_PKEY_CTX_free(keyctx);

  aralog_.LogDebug() << "Message Recovered Succesfully";

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(retVal);
}

void MsgRecoveryPublicCtxDefaultssl::ConfigureVerifyCtx(EVP_PKEY_CTX* keyCtx) const noexcept
{
  aralog_.LogDebug() << "In MsgRecoveryPublicCtxDefaultssl::ConfigureVerifyCtx()";

  if (algId_ == algorithmids::kRSASSA2048PKCSV1_5)
  {
    if (1 != EVP_PKEY_CTX_set_rsa_padding(keyCtx, RSA_PKCS1_PADDING))
    {
      aralog_.LogError() << "Padding failed for RSA";
    }

    if (1 != EVP_PKEY_CTX_set_signature_md(keyCtx, EVP_sha256()))
    {
      aralog_.LogError() << "md failed for RSA";
    }
  }
}

CryptoPrimitiveId::Uptr MsgRecoveryPublicCtxDefaultssl::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In MsgRecoveryPublicCtxDefaultssl::GetCryptoPrimitiveId()";
  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr(std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_));
  return primitiveInfoPtr;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
