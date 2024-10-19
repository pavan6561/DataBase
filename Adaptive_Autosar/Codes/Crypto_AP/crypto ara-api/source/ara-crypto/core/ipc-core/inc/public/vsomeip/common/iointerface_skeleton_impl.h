// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : iointerface_skeleton_impl.h
//
// Purpose     : IO-interface skeleton implementation to forward functional calls to crypto-daemon.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_COMMON_IOINTERFACE_SKELETON_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_COMMON_IOINTERFACE_SKELETON_IMPL_H_

#include <ara/core/future.h>
#include <ara/core/map.h>
#include <ara/core/promise.h>
#include <ara/log/logging.h>

#include "common/io_interface.h"
#include "impl_type_cryptoserviceinstanceid.h"
#include "iointerface_skeleton.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for common.
namespace common
{
/// @brief This class containes skeleton implementation methods for IOInterface.
class IOInterfaceSkeletonImpl : public ara::crypto::ipccom::common::iointerface::skeleton::IOInterfaceSkeleton
{
 public:
  /// @brief Construcor for IOInterfaceSkeletonImpl class.
  IOInterfaceSkeletonImpl(
    CryptoServiceInstanceId const instanceId, ara::crypto::IOInterface::Uptr ioUtr) noexcept;

  /// @brief Destructor for IOInterfaceSkeletonImpl class.
  virtual ~IOInterfaceSkeletonImpl() = default;

  /// @brief getter method for iointerface instance Id.
  CryptoServiceInstanceId GetIOInterfaceInstanceId()
  {
    return instanceId_;
  }

  /// @brief getter method for iointerface object.
  ara::crypto::IOInterface::Uptr& GetIOInterfaceObj()
  {
    return ioInterfaceUptr_;
  }

  /// @brief Definition for GetAllowedUsage.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetAllowedUsageOutput>
    GetAllowedUsage() override;

  /// @brief Definition for GetCapacity.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetCapacityOutput>
    GetCapacity() override;

  /// @brief Definition for GetCryptoObjectType.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetCryptoObjectTypeOutput>
    GetCryptoObjectType() override;

  /// @brief Definition for GetObjectId.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetObjectIdOutput>
    GetObjectId() override;

  /// @brief Definition for GetPayloadSize.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetPayloadSizeOutput>
    GetPayloadSize() override;

  /// @brief Definition for GetPrimitiveId.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetPrimitiveIdOutput>
    GetPrimitiveId() override;

  /// @brief Definition for GetTypeRestriction.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::GetTypeRestrictionOutput>
    GetTypeRestriction() override;

  /// @brief Definition for IsObjectExportable.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsObjectExportableOutput>
    IsObjectExportable() override;

  /// @brief Definition for IsObjectSession.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsObjectSessionOutput>
    IsObjectSession() override;

  /// @brief Definition for IsValid.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsValidOutput> IsValid() override;

  /// @brief Definition for IsVolatile.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsVolatileOutput>
    IsVolatile() override;

  /// @brief Definition for IsWritable.
  ara::core::Future<ara::crypto::ipccom::common::iointerface::IOInterface::IsWritableOutput>
    IsWritable() override;

 private:
  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;

  /// @brief Instance id for this IOInterface instance.
  CryptoServiceInstanceId instanceId_;

  /// @brief Smart unique pointer for IOInterface class.
  ara::crypto::IOInterface::Uptr ioInterfaceUptr_;
};

} // namespace common
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_CORE_IPC_CORE_INC_PUBLIC_VSOMEIP_COMMON_IOINTERFACE_SKELETON_IMPL_H_
