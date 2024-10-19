// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name : digestservice_proxy_impl.h
//
// Purpose : Crypto object proxy implementation to forward functional calls from application to ipc-core.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYSERVICE_DIGESTSERVICE_PROXY_IMPL_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYSERVICE_DIGESTSERVICE_PROXY_IMPL_H_

#include <ara/log/logging.h>

#include "ara/crypto/cryp/digest_service.h"
#include "crypto_primitive_id_proxy_impl.h"
#include "digestservice_proxy.h"
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
class DigestServiceProxyImpl : public ara::crypto::cryp::DigestService
{
 public:
  /// @brief Constructor of DigestServiceProxyImpl class.
  /// @param[in] serviceInstanceID of Crypto service instance id.
  DigestServiceProxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept;

  /// @brief Destructor used to destruct the object of DigestServiceProxyImpl.
  virtual ~DigestServiceProxyImpl() noexcept = default;

  /// @brief Get the output digest size.
  /// @returns size of the full output from this digest-function in bytes
  /// @trace SWS_CRYPT_29012
  std::size_t GetDigestSize() const noexcept override;

  /// @brief Compare the calculated digest against an expected value.
  /// Entire digest value is kept in the context up to next call @c Start(), therefore any its part can be
  /// verified again or extracted.
  /// If <tt>(full_digest_size <= offset) || (expected.size() == 0)</tt> then return false;
  /// else <tt>comparison_size = min(expected.size(), (full_digest_size - offset))</tt> bytes.
  /// This method can be implemented as "inline" after standartization of function ara::core::memcmp().
  /// @param[in] expected the memory region containing an expected digest value
  /// @param[in] offset position of the first byte in calculated digest for the comparison starting
  /// @returns true if the expected bytes sequence is identical to first bytes of calculated digest
  /// @error SecurityErrorDomain::kProcessingNotFinished if the digest calculation was not finished
  /// by a call of the Finish() method
  /// @error SecurityErrorDomain::kBruteForceRisk if the buffered digest belongs to a MAC/HMAC/AE/AEAD
  /// context, which was initialized by a key without kAllowSignature permission, but actual
  /// size of requested digest is less than 8 bytes (it is a protection from the brute-force attack)
  /// @trace SWS_CRYPT_29013
  ara::core::Result<bool> Compare(ReadOnlyMemRegion expected, std::size_t offset = 0) const noexcept override;

  /// @brief Check current status of the stream processing: started or no.
  /// @returns true if the processing was start by a call of the Start() methods and was not finished yet
  /// @trace SWS_CRYPT_29014
  bool IsStarted() const noexcept override;

  /// @brief Check current status of the stream processing: finished or no.
  /// @returns true if a previously started stream processing was finished by a call of the Finish()
  /// or FinishBytes() methods
  /// @trace SWS_CRYPT_29015
  bool IsFinished() const noexcept override;

  /// @brief Get default expected size of the Initialization Vector (IV) or nonce.
  /// @returns default expected size of IV in bytes
  /// @trace SWS_CRYPT_29032
  std::size_t GetIvSize() const noexcept override;

  /// @brief Get block (or internal buffer) size of the base algorithm.
  /// @returns size of the block in bytes
  /// @trace SWS_CRYPT_29033
  std::size_t GetBlockSize() const noexcept override;

  /// @brief Verify validity of specific Initialization Vector (IV) length.
  /// @param[in] ivSize the length of the IV in bytes
  /// @returns true if provided IV length is supported by the algorithm and false otherwise
  /// @trace SWS_CRYPT_29034
  bool IsValidIvSize(std::size_t ivSize) const noexcept override;

  /// @brief Get actual bit-length of an IV loaded to the context.
  /// @param[in] ivUid optional pointer to a buffer for saving an @a COUID of a IV object now loaded to the
  /// context.If the context was initialized by a @c SecretSeed object then the output buffer *ivUid must be
  /// filled by COUID of this loaded IV object, in other cases *ivUid must be filled by all zeros.
  /// @returns actual length of the IV (now set to the algorithm context) in bits
  /// @trace SWS_CRYPT_29035
  std::size_t GetActualIvBitLength(ara::core::Optional<CryptoObjectUid> ivUid) const noexcept override;

  /// @brief Verify supportness of specific key length by the context.
  /// @param[in] keyBitLength length of the key in bits
  /// @returns @c true if provided value of the key length is supported by the context
  /// @trace SWS_CRYPT_29048
  bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept override;

  /// @brief Get minimal supported key length in bits.
  /// @returns minimal supported length of the key in bits
  /// @trace SWS_CRYPT_29043
  std::size_t GetMinKeyBitLength() const noexcept override;

  /// @brief Get maximal supported key length in bits.
  /// @returns maximal supported length of the key in bits
  /// @trace SWS_CRYPT_29044
  std::size_t GetMaxKeyBitLength() const noexcept override;

  /// @brief Get actual bit-length of a key loaded to the context. If no key was set to the
  /// context yet then 0 is returned.
  /// @returns actual length of a key (now set to the algorithm context) in bits
  /// @trace SWS_CRYPT_29045
  std::size_t GetActualKeyBitLength() const noexcept override;

  /// @brief Get allowed usages of this context (according to the key object attributes loaded to this
  /// context).
  /// If the context is not initialized by a key object yet then zero (all flags are reset) must be returned.
  /// @returns a combination of bit-flags that specifies allowed usages of the context
  /// @trace SWS_CRYPT_29046
  AllowedUsageFlags GetAllowedUsage() const noexcept override;

  /// @brief Get the COUID of the key deployed to the context this extension service is attached to.
  /// If no key was set to the context yet then an empty COUID (Nil) is returned.
  /// @returns the COUID of the CryptoObject
  /// @trace SWS_CRYPT_29047
  CryptoObjectUid GetActualKeyCOUID() const noexcept override;

  /// @brief Check if a key has been set to this context.
  /// @returns FALSE if no key has been set
  /// @trace SWS_CRYPT_29049
  bool IsKeyAvailable() const noexcept override;

 private:
  /// @brief Method to initializing the server-client connection for ExtensionServiceProxy contex.
  /// @param[in] serviceInstanceID Crypto service instance id.
  void Initialize(const CryptoServiceInstanceId serviceInstanceID);

  /// @brief Vsome-ip instance for CryptoObject context proxy.
  ara::crypto::vsomeip::VsomeipInterface<
      ara::crypto::ipccom::cryp::cryservice::digestservice::proxy::DigestServiceProxy>
      vsomeipInstance_;

  /// @brief Crypto Object context proxy handler.
  std::shared_ptr<ara::crypto::ipccom::cryp::cryservice::digestservice::proxy::DigestServiceProxy>
      digestServiceHandler_;

  /// @brief Used to store the crypto service instance id.
  CryptoServiceInstanceId const cryptoObjServiceInstanceID_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& logger_;
};

} // namespace cryservice
} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_APP_IPC_APP_INC_VSOMEIP_PUBLIC_CRYPTO_PROVIDER_CRYSERVICE_DIGESTSERVICE_PROXY_IMPL_H_
