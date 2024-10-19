// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : keyslot_impl.cpp
//
// Purpose     : This file contains implementation for KeySlotImpl class.
// -----------------------------------------------------------------------------------------------------------

// NOTE:- This file is out of scope for MS5-IR1 plese do not consider this file for review or static analysis.

#include "keyslot_impl.h"

#include "uuid_impl.h"

namespace ara
{
namespace crypto
{
namespace keys
{
std::shared_ptr<config::CryptoConfig> KeySlotImpl::cryptoConfig_{
  config::CryptoConfig::GetCryptoConfigInstance()
};

KeySlotImpl::KeySlotImpl(ara::core::String& iSpecify)
{
  bool breakoccured{ false };
  algoIdentification_ = AlgorithmIdentification::GetAlogIdentification();
  for (config::CryptoProviderConfig& cryptoProvider : cryptoConfig_->GetCryptoProviders())
  {
    for (config::CryptoKeyslots& keySlot : cryptoProvider.cryptoKeySlot_)
    {
      if (iSpecify == keySlot.uuid_)
      {
        SetSlotType(keySlot.slotType_);
        slotProperties_.mAlgId = algoIdentification_->GetAlgoId(keySlot.cryptoAlgId_);
        slotProperties_.mSlotCapacity = keySlot.slotCapacity_;
        slotProperties_.mObjectType = GetObjectType(keySlot.cryptoObjectType_);
        slotProperties_.mAllocateSpareSlot = keySlot.allocateShadowCopy_;
        for (config::CryptoKeyslotAllowedModification& modifications :
          keySlot.cryptoKeyslotAllowedModification_)
        {
          slotProperties_.mAllowContentTypeChange = modifications.allowContentTypeChange_;
          slotProperties_.mMaxUpdateAllowed = modifications.maxNumberOfAllowedUpdates_;
          slotProperties_.mExportAllowed = modifications.exportability_;
        }
        slotProperties_.mContentAllowedUsage = 0;
        for (config::CryptoKeyslotContentallowedUsage& contentAllowdUsage :
          keySlot.cryptokeyslotcontentallowedusage_)
        {
          slotProperties_.mContentAllowedUsage |= GetAllowedUsage(contentAllowdUsage.allowedKeyslotUsage_);
        }
        uuid_ = keySlot.uuid_;
        contentProperties_.mObjectUid.mGeneratorUid = uuidImpl::UuidStrToAraUuid(keySlot.uuid_);
        contentProperties_.mAlgId = slotProperties_.mAlgId;
        contentProperties_.mObjectSize = slotProperties_.mSlotCapacity;
        contentProperties_.mContentAllowedUsage = slotProperties_.mContentAllowedUsage;
        contentProperties_.mObjectType = slotProperties_.mObjectType;
        breakoccured = true;
        break;
      }
    }
    if (breakoccured)
    {
      break;
    }
  }
}

ara::core::Result<KeySlotPrototypeProps> KeySlotImpl::GetPrototypedProps() const noexcept
{
  /*aralog_.LogInfo() << "Key slot type is : " << static_cast<uint32_t>(slotProperties_.mSlotType);
    aralog_.LogInfo() << "Key Algo Id is : " << slotProperties_.mAlgId;
    aralog_.LogInfo() << "Key Slot Capacity is : " << slotProperties_.mSlotCapacity;
    aralog_.LogInfo() << "Key Object Type is : " << static_cast<uint32_t>(slotProperties_.mObjectType);
    aralog_.LogInfo() << "Alocate saperate slot flag is : " << slotProperties_.mAllocateSpareSlot;
    aralog_.LogInfo() << "Allow Content Type change flag is : " << slotProperties_.mAllowContentTypeChange;
    aralog_.LogInfo() << "Max Update allowed is : " << slotProperties_.mMaxUpdateAllowed;
    aralog_.LogInfo() << "Export allowed is : " << slotProperties_.mExportAllowed;
    aralog_.LogInfo() << "Content allowed Usage is : " << slotProperties_.mContentAllowedUsage;*/
  return ara::core::Result<KeySlotPrototypeProps>::FromValue(slotProperties_);
}

ara::core::Result<KeySlotContentProps> KeySlotImpl::GetContentProps() const noexcept
{
  /*aralog_.LogInfo() << "Key slot uuid is : " ;
    aralog_.LogInfo() << contentProperties_.mObjectUid.mGeneratorUid.mQwordMs;
    aralog_.LogInfo() << contentProperties_.mObjectUid.mGeneratorUid.mQwordLs;
    aralog_.LogInfo() << "Key Algo Id is : " << contentProperties_.mAlgId;
    aralog_.LogInfo() << "Key Object size is : " << contentProperties_.mObjectSize;
    aralog_.LogInfo() << "Key Object Type is : " << static_cast<uint32_t>(contentProperties_.mObjectType);
    aralog_.LogInfo() << "allowed usage is : " << slotProperties_.mContentAllowedUsage;*/
  return ara::core::Result<KeySlotContentProps>::FromValue(contentProperties_);
}

ara::core::Result<IOInterface::Uptr> KeySlotImpl::Open(bool subscribeForUpdates, bool writeable) const
  noexcept
{
  static_cast<void>(subscribeForUpdates);

  IOInterface::Uptr localPtr = std::make_unique<IOInterfaceSqlite>(contentProperties_.mObjectUid, uuid_,
    slotProperties_.mSlotCapacity, contentProperties_.mObjectSize, slotProperties_.mContentAllowedUsage,
    contentProperties_.mAlgId, contentProperties_.mObjectType, slotProperties_.mObjectType, false, false,
    slotProperties_.mExportAllowed, writeable);
  return ara::core::Result<IOInterface::Uptr>::FromValue(std::move(localPtr));
}

/*ara::core::Result<cryp::CryptoProvider::Uptr> KeySlotImpl::MyProvider() const noexcept
{
    return ara::core::Result<cryp::CryptoProvider::Uptr>::FromValue(std::make_unique<cryp::CryptoProviderSSL>());
}*/

AllowedUsageFlags KeySlotImpl::GetAllowedUsage(ara::core::String& allowedUsageStringValue)
{
  ara::core::String kAllowDataEncryptionUsage("ALLOW-DATA-ENCRYPTION");
  ara::core::String kAllowDataDecryptionUsage("ALLOW-DATA-DECRYPTION");
  ara::core::String kAllowSignatureUsage("ALLOW-SIGNATURE");
  ara::core::String kAllowVerificationUsage("ALLOW-VERIFICATION");
  ara::core::String kAllowAgreementUsage("ALLOW-KEY-AGREEMENT");
  ara::core::String kAllowDiversifyUsage("ALLOW-KEY-DIVERSIFY");
  ara::core::String kAllowDrngInitUsage("ALLOW-DRNG-INIT");
  ara::core::String kAllowKdfMaterialUsage("ALLOW-KDF-MATERIAL");
  ara::core::String kAllowExportingUsage("ALLOW-KEY-EXPORTING");
  ara::core::String kAllowImportingUsage("ALLOW-KEY-IMPORTING");
  ara::core::String kAllowExactModeOnlyUsage("ALLOW-EXACT-MODE-ONLY");
  ara::core::String kAllowDerivedDataEncryptionUsage("ALLOW-DERIVED-DATA-ENCRYPTION");
  ara::core::String kAllowDerivedDataDecryptionUsage("ALLOW-DERIVED-DATA-DECRYPTION");
  ara::core::String kAllowDerivedSignatureUsage("ALLOW-DERIVED-SIGNATURE");
  ara::core::String kAllowDerivedVerificationUsage("ALLOW-DERIVED-VERIFICATION");
  ara::core::String kAllowDerivedDiversifyUsage("ALLOW-DERIVED-DIVERSIFY");
  ara::core::String kAllowDerivedDrngInitUsage("ALLOW-DERIVED-DRNG-INIT");
  ara::core::String kAllowDerivedKdfMaterialUsage("ALLOW-DERIVED-KDF-MATERIAL");
  ara::core::String kAllowDerivedKeyExportingUsage("ALLOW-DERIVED-KEY-EXPORTING");
  ara::core::String kAllowDerivedKeyImportingUsage("ALLOW-DERIVED-KEY-IMPORTING");
  ara::core::String kAllowDerivedExactModeOnlyUsage("ALLOW-DERIVED-EXACT-MODE-ONLY");

  if (allowedUsageStringValue == kAllowDataEncryptionUsage)
  {
    return kAllowDataEncryption;
  }
  else if (allowedUsageStringValue == kAllowDataDecryptionUsage)
  {
    return kAllowDataDecryption;
  }
  else if (allowedUsageStringValue == kAllowSignatureUsage)
  {
    return kAllowSignature;
  }
  else if (allowedUsageStringValue == kAllowVerificationUsage)
  {
    return kAllowVerification;
  }
  else if (allowedUsageStringValue == kAllowAgreementUsage)
  {
    return kAllowKeyAgreement;
  }
  else if (allowedUsageStringValue == kAllowDiversifyUsage)
  {
    return kAllowKeyDiversify;
  }
  else if (allowedUsageStringValue == kAllowDrngInitUsage)
  {
    return kAllowRngInit;
  }
  else if (allowedUsageStringValue == kAllowKdfMaterialUsage)
  {
    return kAllowKdfMaterial;
  }
  else if (allowedUsageStringValue == kAllowExportingUsage)
  {
    return kAllowKeyExporting;
  }
  else if (allowedUsageStringValue == kAllowImportingUsage)
  {
    return kAllowKeyImporting;
  }
  else if (allowedUsageStringValue == kAllowExactModeOnlyUsage)
  {
    return kAllowExactModeOnly;
  }
  else if (allowedUsageStringValue == kAllowDerivedDataEncryptionUsage)
  {
    return kAllowDerivedDataEncryption;
  }
  else if (allowedUsageStringValue == kAllowDerivedDataDecryptionUsage)
  {
    return kAllowDerivedDataDecryption;
  }
  else if (allowedUsageStringValue == kAllowDerivedSignatureUsage)
  {
    return kAllowDerivedSignature;
  }
  else if (allowedUsageStringValue == kAllowDerivedVerificationUsage)
  {
    return kAllowDerivedVerification;
  }
  else if (allowedUsageStringValue == kAllowDerivedDiversifyUsage)
  {
    return kAllowDerivedKeyDiversify;
  }
  else if (allowedUsageStringValue == kAllowDerivedDrngInitUsage)
  {
    return kAllowDerivedRngInit;
  }
  else if (allowedUsageStringValue == kAllowDerivedKdfMaterialUsage)
  {
    return kAllowDerivedKdfMaterial;
  }
  else if (allowedUsageStringValue == kAllowDerivedKeyExportingUsage)
  {
    return kAllowDerivedKeyExporting;
  }
  else if (allowedUsageStringValue == kAllowDerivedKeyImportingUsage)
  {
    return kAllowDerivedKeyImporting;
  }
  else if (allowedUsageStringValue == kAllowDerivedExactModeOnlyUsage)
  {
    return kAllowDerivedExactModeOnly;
  }

  return kAllowPrototypedOnly;
}

CryptoObjectType KeySlotImpl::GetObjectType(ara::core::String& CryptoObjecTypeVal)
{
  ara::core::String kSymmetricKeyType{ "SymmetricKey" };
  ara::core::String kPrivateKeyType{ "PrivateKey" };
  ara::core::String kPublicKeyType{ "PublicKey" };
  ara::core::String kSignatureType{ "Signature" };
  ara::core::String kSecretSeedType{ "SecretSeed" };

  if (CryptoObjecTypeVal == kSymmetricKeyType)
  {
    return CryptoObjectType::kSymmetricKey;
  }
  else if (CryptoObjecTypeVal == kPrivateKeyType)
  {
    return CryptoObjectType::kPrivateKey;
  }
  else if (CryptoObjecTypeVal == kPublicKeyType)
  {
    return CryptoObjectType::kPublicKey;
  }
  else if (CryptoObjecTypeVal == kSignatureType)
  {
    return CryptoObjectType::kSignature;
  }
  else if (CryptoObjecTypeVal == kSecretSeedType)
  {
    return CryptoObjectType::kSecretSeed;
  }

  return CryptoObjectType::kUndefined;
}

void KeySlotImpl::SetSlotType(ara::core::String& slotType)
{
  const ara::core::String kMachineSlot{ "machine" };
  if (slotType == kMachineSlot)
  {
    slotProperties_.mSlotType = KeySlotType::kMachine;
  }
  else
  {
    slotProperties_.mSlotType = KeySlotType::kApplication;
  }
}
} // namespace keys
} // namespace crypto
} // namespace ara
