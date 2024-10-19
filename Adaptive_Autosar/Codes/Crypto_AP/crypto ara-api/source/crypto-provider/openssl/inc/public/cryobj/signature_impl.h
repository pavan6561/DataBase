// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature_impl.h
//
// Purpose     : This file contains definitions for SignatureImpl class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYOBJ_SIGNATURE_IMPL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYOBJ_SIGNATURE_IMPL_H_

#include <cstring>

#include <ara/core/result.h>
#include <ara/core/string.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "crypto_primitive_id_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Signature container interface
/// This interface is applicable for keeping the Digital Signature, Hash Digest, (Hash-based) Message
/// Authentication Code (MAC/HMAC).
/// In case of a keyed signature (Digital Signature or MAC/HMAC) a @a COUID of the signature verification key
/// can be obtained by a call of @c CryptoObject::HasDependence()!
/// @trace SWS_CRYPT_23300
class SignatureImpl : public Signature
{
 public:
  /// @brief constructor for SignatureImpl class.
  /// @param[in] digest value in the form of ReadWriteMemRegion.
  /// @param[in] algorithm name supported by this signature object.
  /// @param[in] algorithm Id supported by this signature object.
  /// @param[in] session value to state the life time of this object.
  /// @param[in] exportable flag to state whether this object can be exported or not.
  SignatureImpl(ReadWriteMemRegion digest, ara::core::String primitiveNameValue,
    CryptoPrimitiveId::AlgId primitiId, CryptoPrimitiveId::AlgId hashAlgoIdValue,
    size_t requiredHashSizeValue, bool isSession, bool isExportable,
    const RestrictedUseObject* key = nullptr);

  /// @brief Get an ID of hash algorithm used for this signature object production.
  /// @returns ID of used hash algorithm only (without signature algorithm specification)
  /// @trace SWS_CRYPT_23311
  CryptoPrimitiveId::AlgId GetHashAlgId() const noexcept override
  {
    aralog_.LogDebug() << "In SignatureImpl GetHashAlgId()";
    return hashAlgoId;
  }

  /// @brief Get the hash size required by current signature algorithm.
  /// @returns required hash size in bytes
  /// @trace SWS_CRYPT_23312
  std::size_t GetRequiredHashSize() const noexcept override
  {
    aralog_.LogDebug() << "In SignatureImpl GetRequiredHashSize()";
    return requiredHashSize;
  }

  /// @brief Return the CryptoPrimitivId of this CryptoObject.
  /// @trace SWS_CRYPT_20505
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override
  {
    aralog_.LogDebug() << "In SignatureImpl GetCryptoPrimitiveId()";
    aralog_.LogDebug() << "GetCryptoPrimitiveId() : Primitive Name is " << primitiveName;
    aralog_.LogDebug() << "GetCryptoPrimitiveId() : Primitive Id is " << primitiIdValue;
    CryptoPrimitiveId::Uptr localPtr{ std::make_unique<CryptoPrimitiveIdImpl>(
      primitiveName, primitiIdValue) };
    return localPtr;
  }

  /// @brief Return the "session" (or "temporary") attribute of the object.
  /// A temporary object cannot be saved to a persistent storage location pointed to by an IOInterface!
  /// A temporary object will be securely destroyed together with this interface instance!
  /// A non-session object must have an assigned @a COUID (see @c GetObjectId()).
  /// @returns @c true if the object is temporary (i.e. its life time is limited by the current session only)
  /// @trace SWS_CRYPT_20512
  bool IsSession() const noexcept override
  {
    aralog_.LogDebug() << "In SignatureImpl IsSession()";
    return isSessionValue;
  }

  /// @brief Get the exportability attribute of the crypto object.
  /// An exportable object must have an assigned @a COUID (see @c GetObjectId()).
  /// @returns @c true if the object is exportable (i.e. if it can be exported outside the trusted
  /// environment of the Crypto Provider)
  /// @trace SWS_CRYPT_20513
  bool IsExportable() const noexcept override
  {
    aralog_.LogDebug() << "In SignatureImpl IsExportable()";
    return isExportableValue;
  }

  /// @brief Return the object's COIdentifier, which includes the object's type and UID.
  /// An object that has no assigned @a COUID cannot be (securely) serialized / exported or saved to a
  /// non-volatile storage.
  /// An object should not have a @a COUID if it is session and non-exportable simultaneously
  /// A few related objects of different types can share a single @a COUID (e.g. private and public keys),
  /// but a combination of @a COUID and object type must be unique always!
  /// @returns the object's COIdentifier including the object's type and COUID (or an empty COUID, if this
  /// object is not identifiable).
  /// @trace SWS_CRYPT_20514
  COIdentifier GetObjectId() const noexcept override
  {
    aralog_.LogDebug() << "In SignatureImpl GetObjectId()";
    return objectID;
  }

  /// @brief Return the COIdentifier of the CryptoObject that this CryptoObject depends on.
  /// For signatures objects this method @b must return a reference to correspondent signature verification
  /// public key!
  /// Unambiguous identification of a CryptoObject requires both components: @c CryptoObjectUid and @c
  /// CryptoObjectType.
  /// @returns target COIdentifier of the existing dependence or @c CryptoObjectType::kUnknown and empty
  /// COUID, if the current object does not depend on another CryptoObject
  /// @trace SWS_CRYPT_20515
  COIdentifier HasDependence() const noexcept override
  {
    aralog_.LogDebug() << "In SignatureImpl HasDependence()";
    return objectDependantID;
  }

  /// @brief Return actual size of the object's payload.
  /// Returned value always must be less than or equal to the maximum payload size expected for this
  /// primitive and object type, it is available via call:
  /// <tt>MyProvider().GetPayloadStorageSize(GetObjectType(), GetPrimitiveId()).Value();</tt>
  /// Returned value does not take into account the object's meta-information properties,
  /// but their size is fixed and common for all crypto objects independently from their actual type.
  /// During an allocation of a @c TrustedContainer, Crypto Providers (and %Key Storage Providers) reserve
  /// space for an object's meta-information automatically, according to their implementation details.
  /// @returns size in bytes of the object's payload required for its storage
  /// @trace SWS_CRYPT_20516
  std::size_t GetPayloadSize() const noexcept override
  {
    aralog_.LogDebug() << "In SignatureImpl GetPayloadSize()";
    aralog_.LogDebug() << "GetPayloadSize() : payload size is " << this->GetSignatureValue().size();
    return GetSignatureValue().size();
  }

  /// @brief Serialize itself publicly.
  /// @param[in] formatId  the Crypto Provider specific identifier of the output format
  /// @returns a buffer with the serialized object
  /// @error SecurityErrorDomain::kInsufficientCapacity
  /// if <tt>(output.empty() == false)</tt>, but it's capacity is less than required
  /// @error SecurityErrorDomain::kUnknownIdentifier  if an unknown format ID was specified
  /// @error SecurityErrorDomain::kUnsupportedFormat
  /// if the specified format ID is not supported for this object type
  /// @trace SWS_CRYPT_10711
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
    FormatId formatId = kFormatDefault) const noexcept override
  {
    static_cast<void>(formatId);
    ara::core::Vector<ara::core::Byte> tmpVtr;
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(tmpVtr);
  }

 private:
  /// @brief primitive name supported by this crypto object.
  ara::core::String primitiveName;

  /// @brief primitive Id supported by this crypto object.
  CryptoPrimitiveId::AlgId primitiIdValue;

  /// @brief hash algo Id supported by this crypto object.
  CryptoPrimitiveId::AlgId hashAlgoId;

  /// @brief hash size required by this crypto object.
  size_t requiredHashSize;

  /// @brief session flag to set the life time of this crypto object.
  bool isSessionValue;

  /// @brief session flag to set the scope of this crypto object.
  bool isExportableValue;

  /// @brief crypto object identifier for this crypto object.
  CryptoObject::COIdentifier objectID{};

  /// @brief dependent crypto object identifier for this crypto object.
  CryptoObject::COIdentifier objectDependantID{};

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "SICO", "Signature Impl Crypto Object", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYOBJ_SIGNATURE_IMPL_H_
