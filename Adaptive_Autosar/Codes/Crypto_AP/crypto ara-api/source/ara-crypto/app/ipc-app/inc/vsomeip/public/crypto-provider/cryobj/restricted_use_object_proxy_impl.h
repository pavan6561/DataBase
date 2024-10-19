// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : restricted_use_object_proxy_impl.h
//
// Purpose     : Restricted use object proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYOBJ_RESTRICTED_USE_OBJECT_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYOBJ_RESTRICTED_USE_OBJECT_PROXY_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoproviderinterface_proxy.h"
#include "restricteduseobject_proxy.h"
#include "symmetrickeywrappercontext_proxy.h"
#include "vsomeip_interface.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for crypto object.
namespace cryobj
{
/// @brief A common interface for all objects supporting the usage restriction.
/// @trace SWS_CRYPT_24800
class RestrictedUseObjectProxyImpl : public ara::crypto::cryp::RestrictedUseObject
{
 public:
  /// @brief Constructor of RestrictedUseObjectProxyImpl class.
  /// @param[in] serviceInstanceID of Crypto service instance id.
  RestrictedUseObjectProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept;

  /// @brief Destructor used to destruct the object of RestrictedUseObjectProxyImpl.
  virtual ~RestrictedUseObjectProxyImpl() noexcept = default;

  /// @brief Method for Get the crypto service instance id.
  /// @returns Crypto service instance id of RestrictedUseObjectProxyImpl.
  CryptoServiceInstanceId GetRestrictedUseObjectServiceInstanceId() const
  {
    return this->restrictedUseObjServiceInstanceID_;
  }

  /// @brief Get allowed usages of this object.
  /// @returns a combination of bit-flags that specifies allowed applications of the object
  /// @trace SWS_CRYPT_24811
  ara::crypto::cryp::RestrictedUseObject::Usage GetAllowedUsage() const noexcept override;

  /// @brief Return the CryptoPrimitivId of this CryptoObject.
  /// @trace SWS_CRYPT_20505
  ara::crypto::cryp::CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

  /// @brief Return the "session" (or "temporary") attribute of the object.
  /// A temporary object cannot be saved to a persistent storage location pointed to by an IOInterface!
  /// A temporary object will be securely destroyed together with this interface instance!
  /// A non-session object must have an assigned @a COUID (see @c GetObjectId()).
  /// @returns @c true if the object is temporary (i.e. its life time is limited by the current session only)
  /// @trace SWS_CRYPT_20512
  bool IsSession() const noexcept override;

  /// @brief Get the exportability attribute of the crypto object.
  /// An exportable object must have an assigned @a COUID (see @c GetObjectId()).
  /// @returns @c true if the object is exportable (i.e. if it can be exported outside the trusted
  /// environment of the Crypto Provider)
  /// @trace SWS_CRYPT_20513
  bool IsExportable() const noexcept override;

  /// @brief Return the object's COIdentifier, which includes the object's type and UID.
  /// An object that has no assigned @a COUID cannot be (securely) serialized / exported or saved to a
  /// non-volatile storage.
  /// An object should not have a @a COUID if it is session and non-exportable simultaneously
  /// A few related objects of different types can share a single @a COUID (e.g. private and public keys),
  /// but a combination of @a COUID and object type must be unique always!
  /// @returns the object's COIdentifier including the object's type and COUID (or an empty COUID, if this
  /// object is not identifiable).
  /// @trace SWS_CRYPT_20514
  ara::crypto::cryp::CryptoObject::COIdentifier GetObjectId() const noexcept override;

  /// @brief Return the COIdentifier of the CryptoObject that this CryptoObject depends on.
  /// For signatures objects this method @b must return a reference to correspondent signature verification
  /// public key!
  /// Unambiguous identification of a CryptoObject requires both components: @c CryptoObjectUid and @c
  /// CryptoObjectType.
  /// @returns target COIdentifier of the existing dependence or @c CryptoObjectType::kUnknown and empty
  /// COUID, if the current object does not depend on another CryptoObject
  /// @trace SWS_CRYPT_20515
  ara::crypto::cryp::CryptoObject::COIdentifier HasDependence() const noexcept override;

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
  /// @trace SWS_CRYPT_20517
  ara::core::Result<void> Save(IOInterface& container) const noexcept override;

 private:
  /// @brief Method to initializing the server-client connection for RestrictedUseObjectProxyImpl context.
  /// @param[in] serviceInstanceID Crypto service instance id.
  void Initialize(const CryptoServiceInstanceId serviceInstanceID);

  /// @brief Vsome-ip instance for Restricted Used Object context proxy.
  ara::crypto::vsomeip::VsomeipInterface<
    ara::crypto::ipccom::cryp::cryobj::restricteduseobject::proxy::RestrictedUseObjectProxy>
    vsomeipInstance_;

  /// @brief Crypto Object context proxy handler.
  std::shared_ptr<ara::crypto::ipccom::cryp::cryobj::restricteduseobject::proxy::RestrictedUseObjectProxy>
    restricteduseObjectHandler_;

  /// @brief Used to store the crypto service instance id.
  CryptoServiceInstanceId const restrictedUseObjServiceInstanceID_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYOBJ_RESTRICTED_USE_OBJECT_PROXY_IMPL_H_
