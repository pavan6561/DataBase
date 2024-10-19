// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name : cryptoservice_proxy_impl.h
//
// Purpose : Crypto object proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYSERVICE_CRYPTOSERVICE_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYSERVICE_CRYPTOSERVICE_PROXY_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/crypto_service.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "cryptoservice_proxy.h"
#include "cryptoproviderinterface_proxy.h"
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
namespace cryservice
{
/// @brief A common interface for all cryptograhic objects recognizable by the Crypto Provider.
/// This interface (or any its derivative) represents a non-mutable (after completion) object loadable to a
/// temporary transformation context.
/// @trace SWS_CRYPT_20500
class CryptoServiceProxyImpl : public ara::crypto::cryp::CryptoService
{
 public:
  /// @brief Constructor of CryptoServiceProxyImpl class.
  /// @param[in] serviceInstanceID of Crypto service instance id.
  CryptoServiceProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept;

  /// @brief Destructor used to destruct the object of CryptoServiceProxyImpl.
  virtual ~CryptoServiceProxyImpl() noexcept = default;

  /// @brief Get maximum expected size of the input data block.
  /// @c suppressPadding argument and it will be equal to the block size.
  /// @param[in] suppressPadding if @c true then the method calculates the size for the case when the whole
  /// space of the plain data block is used for the payload only
  /// @returns maximum size of the input data block in bytes
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29021
  std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept override;

  /// @brief Get maximum possible size of the output data block.
  /// If <tt>(IsEncryption() == true)</tt> then a value returned by this method is independent from the
  /// @c suppressPadding argument and will be equal to the block size.
  /// @param[in] suppressPadding if @c true then the method calculates the size for the case when the whole
  /// space of the plain data block is used for the payload only
  /// @returns maximum size of the output data block in bytes
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29022
  std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept override;

  /// @brief Get block (or internal buffer) size of the base algorithm.
  /// For digest, byte-wise stream cipher and RNG contexts it is an informative method,
  /// intended only for optimization of the interface usage.
  /// @returns size of the block in bytes
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29023
  std::size_t GetBlockSize() const noexcept override;

  /// @brief Verify supportness of specific key length by the context.
  /// @param[in] keyBitLength length of the key in bits
  /// @returns @c true if provided value of the key length is supported by the context
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29048
  bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept override;

  /// @brief Get minimal supported key length in bits.
  /// @returns minimal supported length of the key in bits
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29043
  std::size_t GetMinKeyBitLength() const noexcept override;

  /// @brief Get maximal supported key length in bits.
  /// @returns maximal supported length of the key in bits
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29044
  std::size_t GetMaxKeyBitLength() const noexcept override;

  /// @brief Get actual bit-length of a key loaded to the context. If no key was set to the context yet then 0
  /// is returned.
  /// @returns actual length of a key (now set to the algorithm context) in bits
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29045

  std::size_t GetActualKeyBitLength() const noexcept override;

  /// @brief Get allowed usages of this context (according to the key object attributes loaded to this
  /// context). If the context is not initialized by a key object yet then zero (all flags are reset)
  /// must be returned.
  /// @returns a combination of bit-flags that specifies allowed usages of the context
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29046
  AllowedUsageFlags GetAllowedUsage() const noexcept override;

  /// @brief Get the COUID of the key deployed to the context this extension service is attached to.
  /// If no key was set to the context yet then an empty COUID (Nil) is returned.
  /// @returns the COUID of the CryptoObject
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29047
  CryptoObjectUid GetActualKeyCOUID() const noexcept override;

  /// @brief Check if a key has been set to this context.
  /// @returns FALSE if no key has been set
  /// @threadsafety{Thread-safe}
  /// @trace SWS_CRYPT_29049
  bool IsKeyAvailable() const noexcept override;

 private:
  /// @brief Method to initializing the server-client connection for ExtensionServiceProxy contex.
  /// @param[in] serviceInstanceID Crypto service instance id.
  void Initialize(const CryptoServiceInstanceId serviceInstanceID);

  /// @brief Vsome-ip instance for CryptoObject context proxy.
  ara::crypto::vsomeip::VsomeipInterface<
      ara::crypto::ipccom::cryp::cryservice::cryptoservice::proxy::CryptoServiceProxy>
      vsomeipInstance_;

  /// @brief Crypto Object context proxy handler.
  std::shared_ptr<ara::crypto::ipccom::cryp::cryservice::cryptoservice::proxy::CryptoServiceProxy>
      cryptoServiceHandler_;

  /// @brief Used to store the crypto service instance id.
  CryptoServiceInstanceId const cryptoObjServiceInstanceID_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYSERVICE_CRYPTOSERVICE_PROXY_IMPL_H_
