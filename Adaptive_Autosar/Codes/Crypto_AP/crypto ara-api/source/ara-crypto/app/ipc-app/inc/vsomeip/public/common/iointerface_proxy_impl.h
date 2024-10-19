// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : iointerface_proxy_impl.h
//
// Purpose     : IO-Interface proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef _ARA_CRYPTO_COMMON_IOINTERFACE_PROXY_IMPL_H_
#define _ARA_CRYPTO_COMMON_IOINTERFACE_PROXY_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/common/io_interface.h"
#include "impl_type_cryptoobjecttypeipc.h"
#include "impl_type_cryptoserviceinstanceid.h"
#include "iointerface_proxy.h"
#include "keyslotinterface_proxy.h"
#include "vsomeip_interface.h"

namespace ara
{
namespace crypto
{
namespace common
{
class IoInterfaceProxyImpl : public ara::crypto::IOInterface
{
 public:
  explicit IoInterfaceProxyImpl() = default;
  IoInterfaceProxyImpl(CryptoServiceInstanceId const serviceInstanceID) noexcept;
  virtual ~IoInterfaceProxyImpl() = default;

  using UptrProxy = std::unique_ptr<IoInterfaceProxyImpl>;

  CryptoServiceInstanceId GetIOInterfaceInstanceId() const
  {
    return this->serviceInstanceID_;
  }

  ara::crypto::CryptoObjectUid GetObjectId() const noexcept override;
  ara::crypto::CryptoObjectType GetCryptoObjectType() const noexcept override;
  std::size_t GetCapacity() const noexcept override;
  bool IsVolatile() const noexcept override;
  bool IsObjectSession() const noexcept override;
  bool IsObjectExportable() const noexcept override;
  std::size_t GetPayloadSize() const noexcept override;
  ara::crypto::CryptoObjectType GetTypeRestriction() const noexcept override;
  ara::crypto::AllowedUsageFlags GetAllowedUsage() const noexcept override;
  bool IsWritable() const noexcept override;
  ara::crypto::CryptoAlgId GetPrimitiveId() const noexcept override;
  bool IsValid() const noexcept override;

 private:
  bool Initialize(CryptoServiceInstanceId const serviceInstanceID);
  CryptoServiceInstanceId const serviceInstanceID_;
  ara::crypto::vsomeip::VsomeipInterface<ara::crypto::ipccom::common::iointerface::proxy::IOInterfaceProxy>
    vsomeipIOInterfaceInstance_;
  std::shared_ptr<ara::crypto::ipccom::common::iointerface::proxy::IOInterfaceProxy> ioInterfaceProxyHandler_;
  ara::log::Logger& logger_;
};

} // namespace common
} // namespace crypto
} // namespace ara
#endif