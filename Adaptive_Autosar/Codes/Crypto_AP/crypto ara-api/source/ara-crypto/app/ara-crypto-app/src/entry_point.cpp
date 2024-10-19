// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : entry_point.cpp
//
// Purpose     : Entry Point implementation to forward functional calls to crypto daemon.
// -----------------------------------------------------------------------------------------------------------

#include "ara/crypto/common/entry_point.h"

#include <ara/core/instance_specifier.h>
#include <iostream>

#include "crypto_provider_proxy_impl.h"
#include "x509_provider_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
ara::core::String GetCryptoProviderInstanceIdentifier(ara::core::String instanceKey);

ara::crypto::cryp::CryptoProvider::Uptr LoadCryptoProvider(
  const ara::core::InstanceSpecifier& iSpecifier) noexcept
{
  ara::crypto::cryp::CryptoProvider::Uptr cpUptr{ nullptr };
  // Fetch instanceIDIterator iterator from CryptoProviderInstanceSpecifierMapping.
  ara::core::String strValue{ iSpecifier.ToString() };

  ara::core::String instanceValue = GetCryptoProviderInstanceIdentifier(strValue);
  if (instanceValue.empty() == false)
  {
    // Fetch instanceIdentifier.
    cpUptr = std::make_unique<ara::crypto::cryp::CryptoProviderproxyImpl>(instanceValue);
  }
  return cpUptr;
}

ara::crypto::x509::X509Provider::Uptr LoadX509Provider() noexcept
{
  std::unique_ptr<ara::crypto::x509::X509ProviderProxyImpl> providerProxyImpl = std::make_unique<ara::crypto::x509::X509ProviderProxyImpl>("SOME/IP:19999");
  if(providerProxyImpl->IsX509HandlerEstablished() == true)
  {
    return providerProxyImpl;
  }
  else
  {
    return nullptr;
  }
}

} // namespace crypto
} // namespace ara
