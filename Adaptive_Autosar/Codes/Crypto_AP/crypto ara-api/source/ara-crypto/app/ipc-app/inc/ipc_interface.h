// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ipc_interface.h
//
// Purpose     : Abstract base class with IPC methods to establish connection between client and server.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_IPC_INTERFACE_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_IPC_INTERFACE_H_

#include <ara/core/string.h>
#include <stdint.h>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for interface.
namespace interface
{
/// @brief IpcInterface abstract class with common interface methods for all derived IPC mechanisms.
class IpcInterface
{
 public:
  /// @brief Constructor of IpcInterface class.
  IpcInterface() noexcept : serviceInstanceID_{ "" }
  {
  }

  /// @brief Destructor of IpcInterface class.
  virtual ~IpcInterface() noexcept = default;

  /// @brief Method to establish server client connection for given service instance ID
  /// @param[in] serviceInstanceID string parameter used at client side for establishing
  /// @returns true if connection was established successfully, else false.
  virtual bool CreateInstance(const ara::core::String serviceInstanceID) noexcept = 0;

  /// @brief Method to initialize the context data members.
  virtual void Initialize() noexcept = 0;

  /// @brief Method to de-initialize the intialized contexts
  virtual void Deinitialize() noexcept = 0;

  /// @brief Getter method to retrieve ervice instance ID
  /// @returns Crypto service instance ID
  ara::core::String GetInstanceID()
  {
    return serviceInstanceID_;
  }

  /// @brief Setter method to erase stored service instance ID
  void DeleteInstanceID()
  {
    serviceInstanceID_ = "";
  }

  /// @brief Serivce Instance Id data member
  ara::core::String serviceInstanceID_;
};

} // namespace interface
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_IPC_INTERFACE_H_
