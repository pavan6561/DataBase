// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : public_key_impl.h
//
// Purpose     : This file contains definitions for PublicKeyImpl class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYOBJ_PUBLIC_KEY_IMPL_H_
#define CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYOBJ_PUBLIC_KEY_IMPL_H_

#include <ara/log/logging.h>

#include "ara/core/string.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "crypto_primitive_id_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for cryp.
namespace cryp
{
/// @brief General Asymmetric Public %Key interface.
/// @trace SWS_CRYPT_22700
class PublicKeyImpl : public PublicKey
{
 public:
  /// @brief Constructor for PublicKeyImpl class.
  PublicKeyImpl(ReadWriteMemRegion key, Usage usageFlagsVal, ara::core::String primitiveName,
    CryptoPrimitiveId::AlgId primitiId, bool sessionValue, bool exportableValue,
    CryptoObjectUid uuid) noexcept;

  /// @brief Calculate hash of the Public Key value.
  /// The original public key value BLOB is available via the @c Serializable interface.
  /// @param[in] hashFunc  a hash-function instance that should be used the hashing
  /// @returns a buffer preallocated for the resulting hash value
  /// @error SecurityErrorDomain::kInsufficientCapacity  if size of the hash buffer is not enough for
  /// storing of the result
  /// @error SecurityErrorDomain::kIncompleteArgState  if the @c hashFunc context is not initialized
  /// @trace SWS_CRYPT_22712
  ara::core::Result<ara::core::Vector<ara::core::Byte>> HashPublicKey(HashFunctionCtx& hashFunc) const
    noexcept override;

  /// @brief Return the CryptoPrimitivId of this CryptoObject.
  /// @trace SWS_CRYPT_20505
  CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

  /// @brief Get allowed usages of this object.
  /// @returns a combination of bit-flags that specifies allowed applications of the object
  /// @trace SWS_CRYPT_24811
  Usage GetAllowedUsage() const noexcept override
  {
    aralog_.LogDebug() << "In PublicKeyImpl::GetAllowedUsage()";
    return usageFlags;
  }

  /// @brief Return the "session" (or "temporary") attribute of the object.
  /// A temporary object cannot be saved to a persistent storage location pointed to by an IOInterface!
  /// A temporary object will be securely destroyed together with this interface instance!
  /// A non-session object must have an assigned @a COUID (see @c GetObjectId()).
  /// @returns @c true if the object is temporary (i.e. its life time is limited by the current session only)
  /// @trace SWS_CRYPT_20512
  bool IsSession() const noexcept override
  {
    aralog_.LogDebug() << "In PublicKeyImpl::IsSession()";
    return isSessionValue;
  }

  /// @brief Get the exportability attribute of the crypto object.
  /// An exportable object must have an assigned @a COUID (see @c GetObjectId()).
  /// @returns @c true if the object is exportable (i.e. if it can be exported outside the trusted environment
  /// of the Crypto Provider)
  /// @trace SWS_CRYPT_20513
  bool IsExportable() const noexcept override
  {
    aralog_.LogDebug() << "In PublicKeyImpl::IsExportable()";
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
    aralog_.LogDebug() << "In PublicKeyImpl::GetObjectId()";
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
    aralog_.LogDebug() << "In PublicKeyImpl::HasDependence()";
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
  std::size_t GetPayloadSize() const noexcept override;

  /// @brief Getter method to get openssl public key pointer.
  EVP_PKEY* GetPublicKey() const noexcept;

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

  using IsEnumerableTag = void;

  template <typename F>
  void enumerate(F& fun) noexcept(false)
  {
    fun(objectID);
    fun(primitiveNameValue);
    fun(primitiIdValue);
    fun(usageFlags);
  }

 private:
  /// @brief primitive name supported by this crypto object.
  ara::core::String primitiveNameValue;

  /// @brief primitive Id supported by this crypto object.
  CryptoPrimitiveId::AlgId primitiIdValue;

  /// @brief variable to store the payload size of the public key.
  size_t payloadSize_;

  /// @brief allowed usage flags for this crypto object.
  Usage usageFlags;

  /// @brief session flag to set the life time of this crypto object.
  bool isSessionValue;

  /// @brief session flag to set the scope of this crypto object.
  bool isExportableValue;

  /// @brief crypto object identifier for this crypto object.
  CryptoObject::COIdentifier objectID;

  /// @brief dependent crypto object identifier for this crypto object.
  CryptoObject::COIdentifier objectDependantID;

  /// @brief pointer to hold the openssl private key pointer.
  EVP_PKEY* rsaPublicKeyPtr_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{ ara::log::CreateLogger(
    "PUCO", "Public Key Impl Object", ara::log::LogLevel::kVerbose) };
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CRYPTO_PROVIDER_OPENSSL_INC_PUBLIC_CRYOBJ_PUBLIC_KEY_IMPL_H_
