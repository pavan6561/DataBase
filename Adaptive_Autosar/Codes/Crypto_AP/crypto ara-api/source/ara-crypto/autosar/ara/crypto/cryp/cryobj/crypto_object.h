// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_object.h
//
// Purpose     : This file contains definitions for CryptoObject abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_OBJECT_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_OBJECT_H_

#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"
#include "ara/crypto/common/io_interface.h"
#include "ara/crypto/common/security_error_domain.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief A common interface for all cryptograhic objects recognizable by the Crypto Provider.
/// This interface (or any its derivative) represents a non-mutable (after completion) object loadable to a
/// temporary transformation context.
/// @trace SWS_CRYPT_20500
class CryptoObject
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_20501
  using Uptr = std::unique_ptr<CryptoObject>;

  /// @brief Unique smart pointer of the constant interface.
  /// @trace SWS_CRYPT_20502
  using Uptrc = std::unique_ptr<const CryptoObject>;

  /// @brief Destructor.
  /// @trace SWS_CRYPT_20503
  virtual ~CryptoObject() noexcept = default;

  /// @brief Copy-assign another CryptoObject to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30208
  CryptoObject& operator=(const CryptoObject& other) = default;

  /// @brief Move-assign another CryptoObject to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30209
  CryptoObject& operator=(CryptoObject&& other) = default;

  /// @brief Unique identifier of this CryptoObject.
  /// @trace SWS_CRYPT_20504
  struct COIdentifier
  {
    /// @brief type of objext
    /// @trace SWS_CRYPT_20506
    CryptoObjectType mCOType;

    /// @brief object identifier
    /// @trace SWS_CRYPT_20507
    CryptoObjectUid mCouid;
  };

  /// @brief type of objext
  CryptoObjectType cryObjType = CryptoObjectType::kUndefined;

  /// @brief Return the CryptoPrimitivId of this CryptoObject.
  /// @trace SWS_CRYPT_20505
  virtual CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept = 0;

  /// @brief Return the "session" (or "temporary") attribute of the object.
  /// A temporary object cannot be saved to a persistent storage location pointed to by an IOInterface!
  /// A temporary object will be securely destroyed together with this interface instance!
  /// A non-session object must have an assigned @a COUID (see @c GetObjectId()).
  /// @returns @c true if the object is temporary (i.e. its life time is limited by the current session only)
  /// @trace SWS_CRYPT_20512
  virtual bool IsSession() const noexcept = 0;

  /// @brief Get the exportability attribute of the crypto object.
  /// An exportable object must have an assigned @a COUID (see @c GetObjectId()).
  /// @returns @c true if the object is exportable (i.e. if it can be exported outside the trusted
  /// environment of the Crypto Provider)
  /// @trace SWS_CRYPT_20513
  virtual bool IsExportable() const noexcept = 0;

  /// @brief Return the object's COIdentifier, which includes the object's type and UID.
  /// An object that has no assigned @a COUID cannot be (securely) serialized / exported or saved to a
  /// non-volatile storage.
  /// An object should not have a @a COUID if it is session and non-exportable simultaneously
  /// A few related objects of different types can share a single @a COUID (e.g. private and public keys),
  /// but a combination of @a COUID and object type must be unique always!
  /// @returns the object's COIdentifier including the object's type and COUID (or an empty COUID, if this
  /// object is not identifiable).
  /// @trace SWS_CRYPT_20514
  virtual COIdentifier GetObjectId() const noexcept = 0;

  /// @brief Return the COIdentifier of the CryptoObject that this CryptoObject depends on.
  /// For signatures objects this method @b must return a reference to correspondent signature verification
  /// public key!
  /// Unambiguous identification of a CryptoObject requires both components: @c CryptoObjectUid and @c
  /// CryptoObjectType.
  /// @returns target COIdentifier of the existing dependence or @c CryptoObjectType::kUnknown and empty
  /// COUID, if the current object does not depend on another CryptoObject
  /// @trace SWS_CRYPT_20515
  virtual COIdentifier HasDependence() const noexcept = 0;

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
  virtual std::size_t GetPayloadSize() const noexcept = 0;

  /// @brief Save itself to provided IOInterface
  /// A CryptoObject with property "session" cannot be saved in a KeySlot.
  /// @param[in] container IOInterface representing underlying storage
  /// @error SecurityErrorDomain::kIncompatibleObject  if the object is "session", but the IOInterface
  /// represents a KeySlot.
  /// @error SecurityErrorDomain::kContentRestrictions  if the object doesn't satisfy the slot restrictions
  /// (@see keys::KeySlotPrototypeProps)
  /// @error SecurityErrorDomain::kInsufficientCapacity  if the capacity of the target container is not
  /// enough, i.e. if <tt>(container.Capacity() < this->StorageSize())</tt>
  /// @error SecurityErrorDomain::kModifiedResource if the underlying resource has been modified after
  /// the IOInterface has been opened, i.e., the IOInterface has been invalidated.
  /// @error SecurityErrorDomain::kUnreservedResource  if the IOInterface is not opened writeable.
  /// @traceid SWS_CRYPT_20517
  virtual ara::core::Result<void> Save(IOInterface& container) const noexcept = 0;

  /// @brief Downcast and move unique smart pointer from the generic @c CryptoObject interface to concrete
  /// derived object.
  /// @tparam ConcreteObject  target type (derived from @c CryptoObject) for downcasting
  /// @param[in] object  unique smart pointer to the constant generic @c CryptoObject interface
  /// @returns unique smart pointer to downcasted constant interface of specified derived type
  /// @error SecurityErrorDomain::kBadObjectType  if an actual type of the @c object is not the specified
  /// ConcreteObject.
  /// @trace SWS_CRYPT_20518
  template <class ConcreteObject>
  static ara::core::Result<typename ConcreteObject::Uptrc> Downcast(CryptoObject::Uptrc&& object) noexcept
  {
    using DstValType = typename ConcreteObject::Uptrc;
    if (object)
    {
      const CryptoObjectType kObjType = object->GetObjectId().mCOType;
      if (kObjType != ConcreteObject::kObjectType)
      {
        ara::core::ErrorDomain::SupportDataType data
          = (static_cast<ara::core::ErrorDomain::SupportDataType>(kObjType) << 16)
          | static_cast<ara::core::ErrorDomain::SupportDataType>(ConcreteObject::kObjectType);
        return ara::core::Result<DstValType>(ara::crypto::MakeErrorCode(SecurityErrc::kBadObjectType, data));
      }
    }
    return DstValType{ static_cast<const ConcreteObject*>(object.release()) };
  }
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_OBJECT_H_
