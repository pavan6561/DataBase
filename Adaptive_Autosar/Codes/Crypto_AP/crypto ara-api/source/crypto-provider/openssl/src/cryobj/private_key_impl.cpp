// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : private_key_impl.cpp
//
// Purpose     : This file contains implementation for PrivateKeyImpl class.
// -----------------------------------------------------------------------------------------------------------

#include "private_key_impl.h"
/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace cryp
{
PrivateKeyImpl::PrivateKeyImpl(ReadWriteMemRegion key, Usage usageFlagsVal, ara::core::String primitiveName,
  CryptoPrimitiveId::AlgId primitiId, bool sessionValue, bool exportableValue,
  CryptoObjectUid uuid) noexcept :
  PrivateKey{ key },
  primitiveNameValue{ primitiveName }, primitiIdValue{ primitiId }, usageFlags{ usageFlagsVal },
  isSessionValue{ sessionValue }, isExportableValue{ exportableValue }
{
  aralog_.LogDebug() << "In PrivateKeyImpl Constructor";
  objectDependantID.mCOType = CryptoObjectType::kUndefined;
  objectID.mCOType = CryptoObjectType::kPrivateKey;
  objectID.mCouid = uuid;

  //Get the raw value and create RSA key
  BIO* rsaPrivateBIO = BIO_new_mem_buf(key.data(), -1);
  rsaPrivateKeyPtr_ = PEM_read_bio_PrivateKey(rsaPrivateBIO, nullptr, nullptr, nullptr);
}

CryptoPrimitiveId::Uptr PrivateKeyImpl::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In PrivateKeyImpl::GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr localPtr
    = std::make_unique<CryptoPrimitiveIdImpl>(primitiveNameValue, primitiIdValue);
  return localPtr;
}

std::size_t PrivateKeyImpl::GetPayloadSize() const noexcept
{
  aralog_.LogDebug() << "In PrivateKeyImpl::GetPayloadSize()";
  return this->GetPrivateKeyValue().size();
}

EVP_PKEY* PrivateKeyImpl::GetPrivateKey() const noexcept
{
  aralog_.LogDebug() << "In PrivateKeyImpl::GetPrivateKey()";
  return rsaPrivateKeyPtr_;
}

ara::core::Result<PublicKey::Uptrc> PrivateKeyImpl::GetPublicKey() const noexcept
{
  aralog_.LogDebug() << "In PrivateKeyImpl::GetPublicKey()";

  ara::core::Vector<uint8_t> publicKeyChar;

  // extract public key as string
  // create a place to dump the IO, in this case in memory
  BIO* publicBIO = BIO_new(BIO_s_mem());
  // dump key to IO
  PEM_write_bio_PUBKEY(publicBIO, rsaPrivateKeyPtr_);
  // get buffer length
  int publicKeyLen = BIO_pending(publicBIO);
  // create char reference of public key length
  publicKeyChar.resize(publicKeyLen);

  // read the key from the buffer and put it in the char reference
  BIO_read(publicBIO, publicKeyChar.data(), publicKeyLen);

  // at this point we can save the public somewhere
  ReadWriteMemRegion keyValue{ publicKeyChar };

  BIO_free(publicBIO);

  PublicKey::Uptrc publicKey = std::make_unique<PublicKeyImpl>(keyValue, usageFlags, primitiveNameValue,
    primitiIdValue, isSessionValue, isExportableValue, objectID.mCouid);

  return ara::core::Result<PublicKey::Uptrc>::FromValue(std::move(publicKey));
}

/*ara::core::Result<void> PrivateKeyImpl::Save(IOInterface& container) const noexcept
{
  if(isSessionValue && !container.IsObjectSession())
  {
    return ara::core::Result<void>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  if((primitiIdValue != container.GetPrimitiveId()) || (objectID.mCOType != container.GetCryptoObjectType())
  ||(usageFlags != container.GetAllowedUsage()) || (objectID.mCouid != container.GetObjectId()))
  {
    return ara::core::Result<void>::FromError(
    ara::crypto::SecurityErrorDomain::Errc::kContentRestrictions);
  }

  if(GetPayloadSize() >= container.GetCapacity())
  {
    return ara::core::Result<void>::FromError(
    ara::crypto::SecurityErrorDomain::Errc::kInsufficientCapacity);
  }

  if(!container.IsValid())
  {
    return ara::core::Result<void>::FromError(
    ara::crypto::SecurityErrorDomain::Errc::kModifiedResource);
  }

  if(!container.SetObjectValue(GetValueInVector()))
  {
    return ara::core::Result<void>::FromError(
    ara::crypto::SecurityErrorDomain::Errc::kUnreservedResource);
  }

  return ara::core::Result<void>::FromValue();
}*/

} // namespace cryp
} // namespace crypto
} // namespace ara