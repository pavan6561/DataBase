// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_core.cpp
//
// Purpose     : Source file of Crypto Core.
// -----------------------------------------------------------------------------------------------------------

#include "crypto_core.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for ara-crypto core
namespace core
{
CryptoCore::CryptoCore()
{
  logger_.LogDebug() << "In CryptoCore constructor";
  crypto_config_ = ara::crypto::config::CryptoConfig::GetCryptoConfigInstance();
}

CryptoCore::~CryptoCore() noexcept
{
  logger_.LogDebug() << "In CryptoCore distructor";
}

void CryptoCore::Initialize()
{
  logger_.LogDebug() << "In CryptoCore::Initialize()";

  // load crypto configuration from the json file.
  crypto_config_->LoadConfig(std::getenv("CRYPTO_CONFIGURATION"));

  logger_.LogDebug() << "Parsing of crypto config Done...";

  // instantiate crypto provider.
  InstantiateCryptoProvider();

  // load crypto configuration from the json file.
  crypto_config_->LoadCertConfig(std::getenv("CERTIFICATE_CONFIGURATION"));

  logger_.LogDebug() << "Parsing of certificate config Done...";  

  // instantiate x509 provider.
  InstantiateCertificateManagerProvider();
}

void CryptoCore::InstantiateCryptoProvider()
{
  logger_.LogDebug() << "In CryptoCore::InstantiateCryptoProvider()";

  ara::crypto::cryp::CryptoProvider::Uptr crypUptr{ nullptr };

  std::unique_ptr<ara::crypto::cryp::CryptoProviderSkeletonImpl> localCrypProvider;

  ara::crypto::config::CryptoProviderConfig data{ crypto_config_->GetCryptoProviders().at(0) };

  logger_.LogDebug() << "Crypto Provider configuration for default crypto provider found";

  // For default crypto-provider instantiate object of CryptoProviderSSL class.
  crypUptr = std::make_unique<ara::crypto::cryp::CryptoProviderSSL>();

  // Instantiate crypto provider skeleton.
  localCrypProvider = std::make_unique<ara::crypto::cryp::CryptoProviderSkeletonImpl>(
    data.instanceId, crypto_config_, std::move(crypUptr));
  localCrypProvider->OfferService();

  // Create pair of instance id and crypto-provider.
  cryptoProviderSkeletonObj_.emplace(data.instanceId, std::move(localCrypProvider));

  logger_.LogDebug() << "InstantiateCryptoProvider";
}

void CryptoCore::InstantiateKeyStorageProvider()
{
  logger_.LogDebug() << "In CryptoCore::InstantiateKeyStorageProvider()";
}

void CryptoCore::InstantiateCertificateManagerProvider()
{
  logger_.LogDebug() << "CryptoCore::InstantiateCertificateManagerProvider() start";

  ara::crypto::x509::X509Provider::Uptr x509Uptr{nullptr};

  //ara::crypto::config::CryptoCertificateConfig data{crypto_config_->GetCertificateProviders().at(0)};

  x509Uptr = std::make_unique<ara::crypto::x509::X509ProviderImpl>();

  logger_.LogDebug() << "CryptoCore:: X509 Provider configuration for default x509 provider found";

  ara::core::String certInstanceId{"SOME/IP:19999"};

  // Instantiate x509 provider skeleton.
  std::unique_ptr<ara::crypto::x509::X509ProviderSkeletonImpl> localX509Provider = std::make_unique<ara::crypto::x509::X509ProviderSkeletonImpl>(
      certInstanceId, crypto_config_, std::move(x509Uptr));
  localX509Provider->OfferService();

  // Create pair of instance id and x509-provider.
  x509ProviderSkeletonObj_.emplace(certInstanceId, std::move(localX509Provider));

  logger_.LogDebug() << "CryptoCore:: InstantiateX509Provider end";
}

void CryptoCore::Run() const
{
  // Run Crypto core.
  logger_.LogDebug() << "Running CRYPTOCORE";
}

void CryptoCore::Shutdown() const
{
  // Shutting down Crypto Core.
  logger_.LogDebug() << "Shutting Down CRYPTOCORE";
}

} // namespace core
} // namespace crypto
} // namespace ara
