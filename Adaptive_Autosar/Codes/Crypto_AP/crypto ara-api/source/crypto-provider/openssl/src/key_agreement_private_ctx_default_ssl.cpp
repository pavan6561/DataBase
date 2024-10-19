// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : key_agreement_private_ctx_default_ssl.cpp
//
// Purpose     : This file contains implementation for KeyAgreementPrivateCtxDefaultssl class.
// -----------------------------------------------------------------------------------------------------------

#include "key_agreement_private_ctx_default_ssl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
ara::core::Result<void> KeyAgreementPrivateCtxDefaultssl::SetKey(const PrivateKey& key) noexcept
{
  aralog_.LogDebug() << "KeyAgreementPrivateCtxDefaultssl::SetKey()";

  if ((key.kObjectType != CryptoObjectType::kPrivateKey)
      || (key.GetCryptoPrimitiveId()->GetPrimitiveId() != algId_))
  {
    aralog_.LogError() << "This crypto object does not belongs to private key";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  if (!(key.GetAllowedUsage() & kAllowKeyAgreement))
  {
    aralog_.LogError() << "This key is not allowed to use for signature production";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUsageViolation);
  }

  privateKey_ = key.GetPrivateKey();

  isInitialize_ = true;

  return ara::core::Result<void>::FromValue();
}

ExtensionService::Uptr KeyAgreementPrivateCtxDefaultssl::GetExtensionService() const noexcept
{
  aralog_.LogDebug() << "KeyAgreementPrivateCtxDefaultssl::GetExtensionService()";

  CryptoObjectUid objectID;

  ExtensionService::Uptr localPtr{
      std::make_unique<ExtensionServiceImpl>(algId_, 2048, kAllowKeyAgreement, objectID, isInitialize_)};

  return localPtr;
}

ara::core::Result<void> KeyAgreementPrivateCtxDefaultssl::Reset() noexcept
{
  aralog_.LogDebug() << "KeyAgreementPrivateCtxDefaultssl::Reset()";
  isInitialize_ = false;
  privateKey_ = nullptr;
  return ara::core::Result<void>::FromValue();
}

ara::core::Result<SecretSeed::Uptrc> KeyAgreementPrivateCtxDefaultssl::AgreeSeed(
    const PublicKey& otherSideKey, SecretSeed::Usage allowedUsage) const noexcept
{
  aralog_.LogDebug() << "KeyAgreementPrivateCtxDefaultssl::AgreeSeed()";

  if (!isInitialize_)
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  if (otherSideKey.GetCryptoPrimitiveId()->GetPrimitiveId() != algId_)
  {
    aralog_.LogError() << "This key corresponds to different algorithm than the private key!!!";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  EVP_PKEY_CTX* ctx{EVP_PKEY_CTX_new(privateKey_, NULL)};
  size_t skeylen{0};
  ara::core::Vector<uint8_t> sSeed;

  if (1 != EVP_PKEY_derive_init(ctx))
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  if (1 != EVP_PKEY_derive_set_peer(ctx, otherSideKey.GetPublicKey()))
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  if (1 != EVP_PKEY_derive(ctx, NULL, &skeylen))
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  sSeed.resize(skeylen);

  if (1 != EVP_PKEY_derive(ctx, sSeed.data(), &skeylen))
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  EVP_PKEY_CTX_free(ctx);

  ReadWriteMemRegion agreedSeed{sSeed};

  CryptoObjectUid objecId;
  SecretSeed::Uptrc secretSeed = std::make_unique<SecretSeedImpl>(agreedSeed, allowedUsage,
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_, true, false, objecId);

  return ara::core::Result<SecretSeed::Uptrc>::FromValue(std::move(secretSeed));
}

ara::core::Result<SymmetricKey::Uptrc> KeyAgreementPrivateCtxDefaultssl::AgreeKey(
    const PublicKey& otherSideKey, KeyDerivationFunctionCtx& kdf, AlgId targetAlgId,
    AllowedUsageFlags allowedUsage, ReadOnlyMemRegion salt, ReadOnlyMemRegion ctxLable) const noexcept
{
  aralog_.LogDebug() << "KeyAgreementPrivateCtxDefaultssl::AgreeSeed()";

  if (!isInitialize_)
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  if (otherSideKey.GetCryptoPrimitiveId()->GetPrimitiveId() != algId_)
  {
    aralog_.LogError() << "This key corresponds to different algorithm than the private key!!!";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  EVP_PKEY_CTX* ctx{EVP_PKEY_CTX_new(privateKey_, NULL)};
  size_t skeylen{0};
  ara::core::Vector<uint8_t> sSeed;

  if (1 != EVP_PKEY_derive_init(ctx))
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  if (1 != EVP_PKEY_derive_set_peer(ctx, otherSideKey.GetPublicKey()))
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  if (1 != EVP_PKEY_derive(ctx, NULL, &skeylen))
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  sSeed.resize(skeylen);

  if (1 != EVP_PKEY_derive(ctx, sSeed.data(), &skeylen))
  {
    aralog_.LogError() << "Key Agreement Private Ctx is not initialized yet!!!";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUninitializedContext);
  }

  EVP_PKEY_CTX_free(ctx);

  ReadWriteMemRegion agreedSeed{sSeed};

  CryptoObjectUid objecId;

  RestrictedUseObject::Uptrc secretSeed
      = std::make_unique<SecretSeedImpl>(agreedSeed, allowedUsage | kAllowKdfMaterial,
          AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(
              kdf.GetCryptoPrimitiveId()->GetPrimitiveId()),
          kdf.GetCryptoPrimitiveId()->GetPrimitiveId(), true, false, objecId);

  ReadOnlyMemRegion targeKeyId;

  kdf.SetSourceKeyMaterial(*secretSeed);

  kdf.Init(targeKeyId, targetAlgId, allowedUsage, ctxLable);

  kdf.AddSalt(salt);

  return kdf.DeriveKey(true, false);
}

CryptoPrimitiveId::Uptr KeyAgreementPrivateCtxDefaultssl::GetCryptoPrimitiveId() const noexcept
{
  // According to SWS_CRYPT_20411 GetCryptoPrimitiveId() should return the primitive Id
  // pointer which will have primitive information related to this particular ctx.
  CryptoPrimitiveId::Uptr primitiveInfoPtr(std::make_unique<CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(algId_), algId_));
  return primitiveInfoPtr;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
