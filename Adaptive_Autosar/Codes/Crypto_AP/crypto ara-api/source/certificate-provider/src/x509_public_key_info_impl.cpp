// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_public_key_info_impl.cpp
//
// Purpose     : This file contains Cryptographic API. X509PublicKeyInfo interface definition.
// -----------------------------------------------------------------------------------------------------------

#include "x509_public_key_info_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Namespace for crypto.
namespace crypto
{
/// @brief Namespace for Certificates.
namespace x509
{
ara::crypto::cryp::CryptoPrimitiveId::Uptrc X509PublicKeyInfoImpl::GetAlgorithmId() const
{
  aralog_.LogDebug() << "In X509PublicKeyInfoImpl::GetAlgorithmId()";

  CryptoAlgId const id{ioPtr_->GetPrimitiveId()};

  // According to SWS_CRYPT_24410 GetAlgorithmId() should return the primitive Id
  // pointer which will have primitive information related to this particular key.
  cryp::CryptoPrimitiveId::Uptrc primitiveInfoPtr{std::make_unique<cryp::CryptoPrimitiveIdImpl>(
      AlgorithmIdentification::GetAlogIdentification()->GetAlgoName(id), id)};

  return primitiveInfoPtr;
}

std::size_t X509PublicKeyInfoImpl::GetRequiredHashSize() const noexcept
{
  aralog_.LogDebug() << "In X509PublicKeyInfoImpl::GetRequiredHashSize()";

  CryptoAlgId const id{ioPtr_->GetPrimitiveId()};

  return AlgorithmIdentification::GetAlogIdentification()->GetRequiredHashSize(id);
}

CryptoAlgId X509PublicKeyInfoImpl::GetRequiredHashAlgId() const noexcept
{
  aralog_.LogDebug() << "In X509PublicKeyInfoImpl::GetRequiredHashAlgId()";

  CryptoAlgId const id{ioPtr_->GetPrimitiveId()};

  return AlgorithmIdentification::GetAlogIdentification()->GetRequiredHashAlgId(id);
}

std::size_t X509PublicKeyInfoImpl::GetSignatureSize() const noexcept
{
  aralog_.LogDebug() << "In X509PublicKeyInfoImpl::GetSignatureSize()";

  CryptoAlgId const id{ioPtr_->GetPrimitiveId()};

  return AlgorithmIdentification::GetAlogIdentification()->GetSignatureSize(id);
}

ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc> X509PublicKeyInfoImpl::GetPublicKey() const noexcept
{
  aralog_.LogDebug() << "In X509PublicKeyInfoImpl::GetPublicKey()";

  ara::crypto::cryp::PublicKey::Uptrc localPtr{cryptoProviderPtr_->LoadPublicKey(*ioPtr_).Value()};

  return ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc>::FromValue(std::move(localPtr));
}

bool X509PublicKeyInfoImpl::IsSameKey(const ara::crypto::cryp::PublicKey& publicKey) const noexcept
{
  aralog_.LogDebug() << "In X509PublicKeyInfoImpl::GetPublicKey()";

  ara::crypto::cryp::PublicKey::Uptrc const localPtr{cryptoProviderPtr_->LoadPublicKey(*ioPtr_).Value()};

  if (localPtr->GetValueInVector() == publicKey.GetValueInVector())
  {
    return true;
  }
  else
  {
    return false;
  }
}

} // namespace x509
} // namespace crypto
} // namespace ara
