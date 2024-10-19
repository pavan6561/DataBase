// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_object_skeleton_impl.h
//
// Purpose     : crypto object skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CRYPTO_OBJECT_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CRYPTO_OBJECT_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/promise.h>
#include <ara/log/logging.h>

#include "ara/crypto/cryp/cryobj/crypto_object.h"
#include "cryptoobject_skeleton.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for crypto objects.
namespace cryobj
{
/// @brief This class contains the methods for skeleton implementation for crypto object.
class CryptoObjectSkeletonImpl
  : public ara::crypto::ipccom::cryp::cryobj::cryptoobject::skeleton::CryptoObjectSkeleton
{
 public:
  /// @brief Constructor for CryptoObjectSkeletonImpl class.
  CryptoObjectSkeletonImpl(CryptoServiceInstanceId const instanceId,
    ara::crypto::cryp::CryptoObject::Uptrc cryptoObjectUtr) noexcept;

  /// @brief Destructor for CryptoObjectSkeletonImpl class.
  virtual ~CryptoObjectSkeletonImpl() override = default;

  /// @brief Getter method for crypto object unique pointer.
  ara::crypto::cryp::CryptoObject::Uptrc& GetCryptoObjectUptrInstance()
  {
    return cryptoObjectUptr_;
  }

  /// @brief Definition for GetCryptoPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::GetCryptoPrimitiveIdOutput>
    GetCryptoPrimitiveId() override;

  /// @brief Definition for GetObjectId.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::GetObjectIdOutput>
    GetObjectId() override;

  /// @brief Definition for GetPayloadSize.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::GetPayloadSizeOutput>
    GetPayloadSize() override;

  /// @brief Definition for HasDependence.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::HasDependenceOutput>
    HasDependence() override;

  /// @brief Definition for IsExportable.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::IsExportableOutput>
    IsExportable() override;

  /// @brief Definition for IsSession.
  ara::core::Future<ara::crypto::ipccom::cryp::cryobj::cryptoobject::CryptoObject::IsSessionOutput>
    IsSession() override;

  /// @brief Definition for Save.
  ara::core::Future<void> Save(const CryptoServiceInstanceId& IOInterfaceInstanceID) override;

 private:
  /// @brief Smart unique pointer for crypto object class.
  ara::crypto::cryp::CryptoObject::Uptrc cryptoObjectUptr_;

  /// @brief logger_ is used to print the logs.
  ara::log::Logger& logger_{ ara::log::CreateLogger("CRYP", "CRYPTO Object", ara::log::LogLevel::kVerbose) };
};

} // namespace cryobj
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_CRYPTO_PROVIDER_CRYOBJ_CRYPTO_OBJECT_SKELETON_IMPL_H_
