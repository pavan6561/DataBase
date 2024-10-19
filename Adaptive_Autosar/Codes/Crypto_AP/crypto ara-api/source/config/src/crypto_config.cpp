// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_config.h
//
// Purpose     : This file contains implementation for CryptoConfig.
// -----------------------------------------------------------------------------------------------------------

#include "crypto_config.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace config
{
namespace defaultconfig
{
uint64_t constexpr kDefaultMaxBufferSize{ 65535 };
ara::core::String const kDefaultSeedValue{ "0123456789" };
uint32_t constexpr kDefaultKdfIterations{ 2048 };
} // namespace defaultconfig
std::shared_ptr<CryptoConfig> CryptoConfig::stCryptoConfigInstance_{ nullptr };

CryptoConfig::CryptoConfig()
{
}

void CryptoConfig::LoadConfig(ara::core::StringView fileName) noexcept
{
  ara::core::Result<std::unique_ptr<apd::manifestreader::Manifest>> openResult{
    apd::manifestreader::OpenManifest(fileName)
  };

  if (openResult.HasValue() == opresults::kFailure)
  {
    aralog_.LogInfo() << " [Error] in opening json file";
  }
  else
  {
    aralog_.LogInfo() << " [Opened] successfully json file";

    manifest_ = std::move(openResult).Value();

    LoadCryptoDefaultConfigurations();

    LoadCryptoProviderConfig();

    manifest_.reset();

    ConsoleInfo();
  }
}

void CryptoConfig::LoadCertConfig(ara::core::StringView fileName) noexcept
{
  ara::core::Result<std::unique_ptr<apd::manifestreader::Manifest>> openResult{
    apd::manifestreader::OpenManifest(fileName)
  };

  if (openResult.HasValue() == opresults::kFailure)
  {
    aralog_.LogInfo() << " [Error] in opening json file";
  }
  else
  {
    aralog_.LogInfo() << " [Opened] successfully json file";

    certManifest_ = std::move(openResult).Value();

    LoadCryptoCertificateConfig();

    ConsoleCertInfo();

    certManifest_.reset();
  }
}

void CryptoConfig::LoadCryptoDefaultConfigurations() noexcept
{
  ara::core::Result<uint64_t> parsedResult{ manifest_->GetValue<uint64_t>("maxalloweddatabuffer") };

  if (parsedResult.HasValue() == opresults::kFailure)
  {
    aralog_.LogInfo() << " [Error] parsing maxValueBuffer";
    defaultConfig_.maxBufferSizeAllowed = defaultconfig::kDefaultMaxBufferSize;
  }
  else
  {
    defaultConfig_.maxBufferSizeAllowed = std::move(parsedResult).Value();
  }

  ara::core::Result<ara::core::String> seedResult{ manifest_->GetValue<ara::core::String>("rngseedvalue") };

  if (seedResult.HasValue() == opresults::kFailure)
  {
    aralog_.LogInfo() << " [Error] parsing rngseedvalue";
    defaultConfig_.rngSeedValue = defaultconfig::kDefaultSeedValue;
  }
  else
  {
    defaultConfig_.rngSeedValue = std::move(seedResult).Value();
  }

  ara::core::Result<uint32_t> iterationResult{ manifest_->GetValue<uint32_t>("defaultkdfIterations") };

  if (iterationResult.HasValue() == opresults::kFailure)
  {
    aralog_.LogInfo() << " [Error] parsing iterationcount";
    defaultConfig_.kdfIterations = defaultconfig::kDefaultKdfIterations;
  }
  else
  {
    defaultConfig_.kdfIterations = std::move(iterationResult).Value();
  }
}

void CryptoConfig::LoadCryptoProviderConfig() noexcept
{
  ara::core::Result<ara::core::Vector<ara::crypto::config::CryptoProviderConfig>> parsedResult{
    manifest_->GetArray<ara::crypto::config::CryptoProviderConfig>("cryptoprovider")
  };

  if (parsedResult.HasValue() == opresults::kFailure)
  {
    aralog_.LogInfo() << " [Error] parsing LoadCryptoProviderConfig";
  }
  else
  {
    aralog_.LogInfo() << " [Parsed] LoadCryptoProviderConfig";
    cryptoProviderStruct_ = std::move(parsedResult).Value();
  }
}

void CryptoConfig::LoadCryptoCertificateConfig() noexcept
{
  aralog_.LogInfo() << "In CryptoConfig::LoadCryptoCertificateConfig()";
  ara::core::Result<ara::core::Vector<ara::crypto::config::CryptoCertificateConfig>> parsedResult{
    certManifest_->GetArray<ara::crypto::config::CryptoCertificateConfig>("CryptoCertificate")
  };
  if (parsedResult.HasValue() == opresults::kFailure)
  {
    aralog_.LogInfo() << " [Error] parsing LoadCryptoCertificateConfig";
  }
  else
  {
    aralog_.LogInfo() << " [Parsed] LoadCryptoCertificateConfig";
    cryptoCertificateStruct_ = std::move(parsedResult).Value();
  }
}

void CryptoConfig::ConsoleCertInfo() noexcept
{
  for (CryptoCertificateConfig& certData : cryptoCertificateStruct_)
  {
    aralog_.LogInfo() << "===============================================";
    aralog_.LogInfo() << " Certificate Name : " << certData.certificateName;
    aralog_.LogInfo() << " Certificate Catagory : " << certData.certificateCatagory;
    aralog_.LogInfo() << " Certificate InstanceSpceifier : " << certData.certificateInstanceSpceifier;
  }
}

void CryptoConfig::ConsoleInfo() noexcept
{
  aralog_.LogInfo() << "----------------- ConsoleInfo -----------------";
  aralog_.LogInfo() << "Max allowed buffer value : " << defaultConfig_.maxBufferSizeAllowed;
  aralog_.LogInfo() << "default rng seed value : " << defaultConfig_.rngSeedValue;
  for (CryptoProviderConfig& crypData : cryptoProviderStruct_)
  {
    aralog_.LogInfo() << "===============================================";
    aralog_.LogInfo() << " CryptoProvider Name : " << crypData.shortName;
    aralog_.LogInfo() << " CryptoProvider Instance id : " << crypData.instanceId;
    for (CryptoKeyslots& keySlotData : crypData.cryptoKeySlot_)
    {
      aralog_.LogInfo() << "-----------------------------------------------";
      aralog_.LogInfo() << " uuid : " << keySlotData.uuid_;
      aralog_.LogInfo() << " allocateShadowCopy : " << keySlotData.allocateShadowCopy_;
      aralog_.LogInfo() << " cryptoAlgId : " << keySlotData.cryptoAlgId_;
      aralog_.LogInfo() << " cryptoObjectType : " << keySlotData.cryptoObjectType_;
      aralog_.LogInfo() << " slotCapacity : " << keySlotData.slotCapacity_;
      aralog_.LogInfo() << " slotType : " << keySlotData.slotType_;
      for (CryptoKeyslotAllowedModification& keyAllowMod : keySlotData.cryptoKeyslotAllowedModification_)
      {
        aralog_.LogInfo() << " allowContentTypeChange : " << keyAllowMod.allowContentTypeChange_;
        aralog_.LogInfo() << " exportability : " << keyAllowMod.exportability_;
        aralog_.LogInfo() << " maxNumberOfAllowedUpdates : " << keyAllowMod.maxNumberOfAllowedUpdates_;
        aralog_.LogInfo() << " restrictUpdate : " << keyAllowMod.restrictUpdate_;
      }
      for (CryptoKeyslotContentallowedUsage& keyContentAllowed :
        keySlotData.cryptokeyslotcontentallowedusage_)
      {
        aralog_.LogInfo() << " allowedKeyslotUsage : " << keyContentAllowed.allowedKeyslotUsage_;
      }
    }
  }
}

std::shared_ptr<CryptoConfig> CryptoConfig::GetCryptoConfigInstance()
{
  if (stCryptoConfigInstance_ == nullptr)
  {
    stCryptoConfigInstance_ = std::make_shared<CryptoConfig>();
  }

  return stCryptoConfigInstance_;
}

} // namespace config
} // namespace crypto
} // namespace ara