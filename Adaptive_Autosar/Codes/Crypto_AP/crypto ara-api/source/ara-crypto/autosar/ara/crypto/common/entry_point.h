// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_object_uid.h
//
// Purpose     : Implementation for CryptoObjectUid class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_ENTRY_POINT_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_ENTRY_POINT_H_

#include "ara/crypto/x509/x509_provider.h"
#include "ara/core/instance_specifier.h"
#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/keys/key_storage_provider.h"

namespace ara
{
namespace crypto
{
/// @brief 128 bit secure counter made up of most significant and least significant
/// @trace SWS_CRYPT_30001
struct SecureCounter
{
  /// @brief least significant 64 bits
  /// @trace{RS_CRYPTO_02401}
  std::uint64_t mLSQW;

  /// @brief most significant 64 bits
  /// @trace SWS_CRYPT_30003
  std::uint64_t mMSQW;
};

/// @brief Factory that creates or return existing single instance of specific Crypto Provider.
///  If <tt>(providerUid == nullptr)</tt> then platform default provider should be loaded.
/// @param[in] iSpecify  the globally unique identifier of required Crypto Provider
/// @returns unique smart pointer to loaded Crypto Provider
/// @trace SWS_CRYPT_20099
ara::crypto::cryp::CryptoProvider::Uptr LoadCryptoProvider(
  const ara::core::InstanceSpecifier& iSpecify) noexcept;

/// @brief Get current value of 128 bit Secure Counter supported by the Crypto Stack.
/// Secure Counter is a non-rollover monotonic counter that ensures incrementation of its value for each
/// following call.
/// The Secure Counter is presented by two 64 bit components: Most Significant Quadword (MSQW) and
/// Least Significant Quadword (LSQW). During normal operation of the Crypto Stack, the MSQW value is fixed
/// (unchangeable) and only LSQW should be incremented. The LSQW counter can be implemented in the
/// "low-power" (always-powered-up) domain of the main CPU, but the MSQW in the Flash/EEPROM storage.
/// But the MSQW must be incremented if the LSQW reaches the maximum value of all ones. Also the MSQW must be
/// incremented during reinitialisation of the whole Crypto Stack (e.g. if the "low-power" supply was
/// interrupted by some reason).
/// Permission to execute this routine is subject of Identity and Access Management control and may be
/// restricted by application manifest!
/// @returns a SecureCounter struct made up of the two unsigned 64 bit values (LSQW and MSQW)

/// @error SecurityErrorDomain::kUnsupported  if the Secure Counter is unsupported by the Crypto Stack
/// implementation on this Platform
/// @error SecurityErrorDomain::kAccessViolation  if current Actor has no permission to call this routine
/// @trace SWS_CRYPT_20098
ara::core::Result<SecureCounter> GetSecureCounter() noexcept;

/// @brief Return an allocated buffer with a generated random sequence of the requested size.
/// @param[in] count number of random bytes to generate
/// @returns @c a buffer filled with the generated random sequence
/// @error SecurityErrorDomain::kBusyResource  if the used RNG is currently out-of-entropy
/// and therefore cannot provide the requested number of random bytes
/// @trace SWS_CRYPT_30098
ara::core::Result<ara::core::Vector<ara::core::Byte>> GenerateRandomData(std::uint32_t count) noexcept;

/// @brief Factory that creates or return existing single instance of the Key Storage Provider.
/// @returns unique smart pointer to loaded Key Storage Provider
/// @error SecurityErrorDomain::kRuntimeFault  if the Key Storage Provider instance cannot be created
/// @trace SWS_CRYPT_30099
ara::crypto::keys::KeyStorageProvider::Uptr LoadKeyStorageProvider() noexcept;

/// @traceid{SWS_CRYPT_40099}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02306}
/// @brief Factory that creates or return existing single instance of the X.509 Provider.
/// X.509 Provider should use the default Crypto Provider for hashing and signature verification!
/// Therefore when you load the X.509 Provider, in background it loads the default Crypto Provider too.
/// @returns unique smart pointer to loaded X.509 Provider
/// @error SecurityErrorDomain::kRuntimeFault  if the X.509 Provider cannot be loaded
/// @threadsafety{Thread-safe}
ara::crypto::x509::X509Provider::Uptr LoadX509Provider() noexcept;

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_ENTRY_POINT_H_
