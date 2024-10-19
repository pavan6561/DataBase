// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : vsomeip_interface.h
//
// Purpose     : Vsome-Ip interface implementation for service establish connection between server and client.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_VSOMEIP_INTERFACE_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_VSOMEIP_INTERFACE_H_

#include <chrono>
#include <condition_variable>

#include <ara/log/logging.h>
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>

#include "ara/com/internal/runtime.h"
#include "impl_type_cryptoserviceinstanceid.h"
#include "ipc_interface.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for vsomeip.
namespace vsomeip
{
/// @brief VsomeIPInterface class with common interface methods for all derived IPC mechanisms.
template <typename ProxyType>
class VsomeipInterface : public ara::crypto::interface::IpcInterface
{
  /// @brief HandleType aliase name.
  using HandleType = typename ProxyType::HandleType;

 public:
  /// @brief Destructor of VsomeipInterface class.
  virtual ~VsomeipInterface() noexcept
  {
    ProxyType::StopFindService(localHandler);
  }

  /// @brief Method to establish server client connection for given service instance ID
  /// @param[in] serviceInstanceID string parameter used at client side for establishing
  /// @returns true if connection was established successfully, else false.
  bool CreateInstance(const ara::core::String serviceInstanceID) noexcept override;

  /// @brief Method to initialize the context data members.
  void Initialize() noexcept override;

  /// @brief Method to de-initialize the intialized contexts.
  void Deinitialize() noexcept override;

  /// @brief Method to get the handler instance.
  std::shared_ptr<ProxyType> GetHandlerInstance() noexcept;

  /// @brief Method to get the crypto service instance id.
  CryptoServiceInstanceId GetCryptoServiceInstanceId() noexcept;

  /// @brief data member to store the crypto service instance id.
  CryptoServiceInstanceId serviceInstanceID_{ "" };

 private:
  /// @brief Method to get the service availability callback.
  /// @param[in] handles service handle container.
  void serviceAvailabilityCallback(ara::com::ServiceHandleContainer<HandleType> handles);

  /// @brief proxy handler data member.
  std::shared_ptr<ProxyType> proxyhandler_{ nullptr };

  /// @brief mutex data member.
  std::mutex mutex_;

  /// @brief wait time data member.
  const uint16_t waitTime{ 5000 };

  /// @brief condition variable data member.
  std::condition_variable cond_var_;

  /// @brief find service local handler data member.
  ara::com::FindServiceHandle localHandler;

  /// @brief Logger variable for ara::log.
  ara::log::Logger& logger_{ ara::log::CreateLogger(
    "VINT", "Vsomeip Interface", ara::log::LogLevel::kVerbose) };
};

template <typename ProxyType>
using HandleType = typename ProxyType::HandleType;

template <typename ProxyType>
bool VsomeipInterface<ProxyType>::CreateInstance(const ara::core::String serviceInstID) noexcept
{
  bool issuccess;
  serviceInstanceID_ = serviceInstID;
  std::unique_lock<std::mutex> lock{ mutex_ };
  std::chrono::time_point<std::chrono::system_clock> const currentTime{ std::chrono::system_clock::now() };
  std::thread([&] { Initialize(); }).detach();
  if (cond_var_.wait_until(lock, currentTime + std::chrono::milliseconds(waitTime))
    == std::cv_status::timeout)
  {
    logger_.LogError() << "Conditional wait expired for registered serviceAvailabilityCallback function";
    issuccess = false;
  }
  else
  {
    logger_.LogInfo() << "Successfully received serviceAvailabilityCallback function for registered service";
    issuccess = true;
  }
  lock.unlock();
  return issuccess;
}

template <typename ProxyType>
void VsomeipInterface<ProxyType>::Initialize() noexcept
{
  logger_.LogDebug() << "VsomeipInterface Initialize : " << serviceInstanceID_;
  ProxyType::StartFindService(
    [this](ara::com::ServiceHandleContainer<HandleType> handles, ara::com::FindServiceHandle handler) {
      localHandler = handler;
      serviceAvailabilityCallback(std::move(handles));
    },
    ara::com::InstanceIdentifier(serviceInstanceID_));
}

template <typename ProxyType>
void VsomeipInterface<ProxyType>::Deinitialize() noexcept
{
  logger_.LogInfo() << "Perform StopFindService for proxy with InstanceID : " << serviceInstanceID_;
}

template <typename ProxyType>
void VsomeipInterface<ProxyType>::serviceAvailabilityCallback(
  ara::com::ServiceHandleContainer<HandleType> handles)
{
  logger_.LogDebug() << "Received serviceAvailabilityCallback";
  for (auto it : handles)
  {
    logger_.LogInfo() << "Vsomeip Crypto Interface with instance -> " << it.GetInstanceId().ToString()
                      << " is available";
  }
  if (handles.size() > 0)
  {
    if (proxyhandler_ == nullptr)
    {
      proxyhandler_ = std::make_unique<ProxyType>(handles[0]);
      cond_var_.notify_all();
    }
    else
    {
      logger_.LogInfo() << "ProxyType is not nullptr";
    }
  }
}

template <typename ProxyType>
std::shared_ptr<ProxyType> VsomeipInterface<ProxyType>::GetHandlerInstance() noexcept
{
  return proxyhandler_;
}

template <typename ProxyType>
CryptoServiceInstanceId VsomeipInterface<ProxyType>::GetCryptoServiceInstanceId() noexcept
{
  return serviceInstanceID_;
}

} // namespace vsomeip
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_VSOMEIP_INTERFACE_H_
