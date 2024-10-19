// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : public_key_impl.cpp
//
// Purpose     : This file contains implementation for PublicKeyImpl class.
// -----------------------------------------------------------------------------------------------------------

#include "public_key_impl.h"

#include "ara/com/internal/vsomeip/vsomeip_marshalling.h"
/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace cryp
{
PublicKeyImpl::PublicKeyImpl(ReadWriteMemRegion key, Usage usageFlagsVal, ara::core::String primitiveName,
  CryptoPrimitiveId::AlgId primitiId, bool sessionValue, bool exportableValue,
  CryptoObjectUid uuid) noexcept :
  PublicKey{ key },
  primitiveNameValue{ primitiveName }, primitiIdValue{ primitiId }, payloadSize_{ key.size() },
  usageFlags{ usageFlagsVal }, isSessionValue{ sessionValue }, isExportableValue{ exportableValue }
{
  aralog_.LogDebug() << "In PublicKeyImpl Constructor";
  objectDependantID.mCOType = CryptoObjectType::kUndefined;
  objectID.mCOType = CryptoObjectType::kPublicKey;
  objectID.mCouid = uuid;

  BIO* rsaPublicBIO = BIO_new_mem_buf(key.data(), -1);

  //BIO* rsaPublicBIO = BIO_new(BIO_s_mem());

  //PEM_write_bio(rsaPublicBIO, "PUBLIC KEY", " ", key.data(), key.size());
  rsaPublicKeyPtr_ = PEM_read_bio_PUBKEY(rsaPublicBIO, nullptr, nullptr, nullptr);
  BIO_free(rsaPublicBIO);
}

CryptoPrimitiveId::Uptr PublicKeyImpl::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In PublicKeyImpl::GetCryptoPrimitiveId()";
  CryptoPrimitiveId::Uptr localPtr
    = std::make_unique<CryptoPrimitiveIdImpl>(primitiveNameValue, primitiIdValue);
  return localPtr;
}

std::size_t PublicKeyImpl::GetPayloadSize() const noexcept
{
  aralog_.LogDebug() << "In PublicKeyImpl::GetPayloadSize()";
  return this->GetPublicKeyValue().size();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> PublicKeyImpl::HashPublicKey(
  HashFunctionCtx& hashFunc) const noexcept
{
  aralog_.LogDebug() << "In PublicKeyImpl::HashPublicKey()";

  if (!hashFunc.IsInitialized())
  {
    aralog_.LogError() << "Hash Function ctx is not initialized yet by calling Start()";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrc::kIncompleteArgState);
  }
  ReadWriteMemRegion keyValue(this->GetPublicKeyValue());
  ReadOnlyMemRegion key(keyValue.data(), keyValue.data() + keyValue.size());
  hashFunc.Update(key);
  return hashFunc.Finish();
}

EVP_PKEY* PublicKeyImpl::GetPublicKey() const noexcept
{
  aralog_.LogDebug() << "In PublicKeyImpl::GetPublicKey()";
  return rsaPublicKeyPtr_;
}

/*ara::core::Result<ara::core::Vector<ara::core::Byte>> PublicKeyImpl::ExportPublicly(FormatId formatId) const noexcept
{
  ara::core::Vector<std::uint8_t> output;
  ara::core::String publicKeyStringValue;
  ReadWriteMemRegion publicKeyValue{GetPublicKeyValue()};
  ara::com::internal::vsomeip::common::Marshaller marshaller(output);
  ara::core::Vector<ara::core::Byte> result;

  if((formatId != Serializable::kFormatDefault) && (formatId != Serializable::kFormatRawValueOnly) &&
    (formatId != Serializable::kFormatDerEncoded) && (formatId != Serializable::kFormatPemEncoded))
  {
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if((formatId != Serializable::kFormatDefault) && (formatId != Serializable::kFormatRawValueOnly)
  && (formatId != Serializable::kFormatPemEncoded))
  {
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kUnsupportedFormat);
  }

  for(std::size_t index = 0; index < publicKeyValue.size(); index++)
  {
    publicKeyStringValue.push_back(publicKeyValue[index]);
  }

  marshaller.marshal(*this, publicKeyStringValue);

  if((formatId == Serializable::kFormatPemEncoded) || (formatId == Serializable::kFormatDefault))
  {
    BIO* publicKeyBIO = BIO_new(BIO_s_mem());

    PEM_write_bio(publicKeyBIO, "PublicKey", " ", output.data(), output.size());

    int publicKeyLen = BIO_pending(publicKeyBIO);
    
    output.clear();
    output.resize(publicKeyLen);

    BIO_read(publicKeyBIO, output.data(), output.size());

    BIO_free(publicKeyBIO);
  }

  for (size_t index = 0; index < output.size(); index++) {
    result.push_back(static_cast<ara::core::Byte>(output[index]));
  }

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(result);
}*/

/*ara::core::Result<void> PublicKeyImpl::Save(IOInterface& container) const noexcept
{
  if(isSessionValue && !container.IsObjectSession())
  {
    return ara::core::Result<void>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }

  //|| (objectID.mCouid != container.GetObjectId())
  if((primitiIdValue != container.GetPrimitiveId()) || (objectID.mCOType != container.GetCryptoObjectType())
  ||(usageFlags != container.GetAllowedUsage()))
  {
    return ara::core::Result<void>::FromError(
    ara::crypto::SecurityErrorDomain::Errc::kContentRestrictions);
  }

  if(GetPayloadSize() > container.GetCapacity())
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