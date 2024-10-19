// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509publickeyinfo_proxy_impl.h
//
// Purpose     : X509 provider proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_X509PUBLICKEYINFO_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_X509PUBLICKEYINFO_PROXY_IMPL_H_

#include <ara/core/result.h>
#include <ara/core/vector.h>
#include <ara/log/logging.h>

#include "ara/crypto/x509/x509_public_key_info.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "signer_private_ctx_proxy_impl.h"
#include "vsomeip_interface.h"
#include "x509publickeyinfo_proxy.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for x509 primitives.
namespace x509
{
class X509PublicKeyInfoProxyImpl final : public ara::crypto::x509::X509PublicKeyInfo
{
 public:
  /// @brief Constructor for the X509PublicKeyInfoProxyImpl class.
  /// @param[in] serviceInstanceID CryptoServiceInstance Id for the x509 provider.
  X509PublicKeyInfoProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept;

  /// @brief Destructor for X509PublicKeyInfoProxyImpl class.
  virtual ~X509PublicKeyInfoProxyImpl() noexcept = default;

  /// @trace SWS_CRYPT_24410
  ara::crypto::cryp::CryptoPrimitiveId::Uptrc GetAlgorithmId() override;

  /// @brief Get the hash size required by current signature algorithm.
  /// @returns required hash size in bytes
  /// @trace SWS_CRYPT_24411
  std::size_t GetRequiredHashSize() const noexcept override;

  /// @brief Get an ID of hash algorithm required by current signature algorithm.
  /// @returns required hash algorithm ID or @c kAlgIdAny if the signature algorithm specification does not
  /// include a concrete hash function
  /// @trace SWS_CRYPT_24412
  CryptoAlgId GetRequiredHashAlgId() const noexcept override;

  /// @brief Get size of the signature value produced and required by the current algorithm.
  /// @returns size of the signature value in bytes
  /// @trace SWS_CRYPT_24413
  std::size_t GetSignatureSize() const noexcept override;

  /// @brief Get public key object of the subject.
  /// Created @c PublicKey object is @b session and @c non-exportable, because generic X.509 certificate or
  /// certificate signing request (CSR) doesn't have COUID of the public key, therefore it should be saved
  /// or transmitted only as a part of correspondent certificate or CSR.
  /// @returns unique smart pointer to the created public key of the subject
  /// @trace SWS_CRYPT_24414
  ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc> GetPublicKey() const noexcept override;

  /// @brief Verify the sameness of the provided and kept public keys.
  /// This method compare the public key values only.
  /// @param[in] publicKey the public key object for comparison
  /// @returns @c true if values of the stored public key and object provided by the argument are identical
  /// and @c false otherwise
  /// @trace SWS_CRYPT_24415
  bool IsSameKey(const ara::crypto::cryp::PublicKey& publicKey) const noexcept override;

  /// @brief Serialize itself publicly.
  /// @param[in] formatId the Crypto Provider specific identifier of the output format
  /// @returns a buffer with the serialized object
  /// @error SecurityErrorDomain::kInsufficientCapacity
  /// if <tt>(output.empty() == false)</tt>, but it's capacity is less than required
  /// @error SecurityErrorDomain::kUnknownIdentifier if an unknown format ID was specified
  /// @error SecurityErrorDomain::kUnsupportedFormat
  /// if the specified format ID is not supported for this object type
  /// @trace SWS_CRYPT_10711
  ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
      FormatId formatId = kFormatDefault) const noexcept override;

 private:
  /// @brief initialize proxy implementation.
  void Initialize(const CryptoServiceInstanceId serviceInstanceID);

  /// @brief vsomeip instance for x509 provider proxy.
  ara::crypto::vsomeip::VsomeipInterface<ara::crypto::ipccom::x509::pki::proxy::X509PublicKeyInfoProxy>
      vsomeipInstance_;

  /// @brief shared handle for x509 provider proxy.
  std::shared_ptr<ara::crypto::ipccom::x509::pki::proxy::X509PublicKeyInfoProxy> x509PublicKeyInfoHandler_;

  /// @brief Logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_X509_X509PUBLICKEYINFO_PROXY_IMPL_H_
