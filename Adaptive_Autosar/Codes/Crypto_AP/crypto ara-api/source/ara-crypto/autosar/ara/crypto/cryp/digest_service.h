// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : digest_service.h
//
// Purpose     : This file contains definitions for DigestService abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_DIGEST_SERVICE_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_DIGEST_SERVICE_H_

#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/block_service.h"
#include "ara/core/result.h"

namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief Extension meta-information service for digest producing contexts.
/// @trace SWS_CRYPT_29010
class DigestService : public BlockService
{
 public:
  /// @brief Unique smart pointer of the interface.
  /// @trace SWS_CRYPT_29011
  using Uptr = std::unique_ptr<DigestService>;

  /// @brief Get the output digest size.
  /// @returns size of the full output from this digest-function in bytes
  /// @trace SWS_CRYPT_29012
  virtual std::size_t GetDigestSize() const noexcept = 0;

  /// @brief Compare the calculated digest against an expected value.
  /// Entire digest value is kept in the context up to next call @c Start(), therefore any its part can be
  /// verified again or extracted.
  /// If <tt>(full_digest_size <= offset) || (expected.size() == 0)</tt> then return false;
  /// else <tt>comparison_size = min(expected.size(), (full_digest_size - offset))</tt> bytes.
  /// This method can be implemented as "inline" after standartization of function ara::core::memcmp().
  /// @param[in] expected  the memory region containing an expected digest value
  /// @param[in] offset  position of the first byte in calculated digest for the comparison starting
  /// @returns true if the expected bytes sequence is identical to first bytes of calculated digest
  /// @error SecurityErrorDomain::kProcessingNotFinished  if the digest calculation was not finished
  /// by a call of the Finish() method
  /// @error SecurityErrorDomain::kBruteForceRisk  if the buffered digest belongs to a MAC/HMAC/AE/AEAD
  /// context, which was initialized by a key without kAllowSignature permission, but actual
  /// size of requested digest is less than 8 bytes (it is a protection from the brute-force attack)
  /// @trace SWS_CRYPT_29013
  virtual ara::core::Result<bool> Compare(ReadOnlyMemRegion expected, std::size_t offset = 0) const noexcept
    = 0;

  /// @brief Check current status of the stream processing: started or no.
  /// @returns true if the processing was start by a call of the Start() methods and was not finished yet
  /// @trace SWS_CRYPT_29014
  virtual bool IsStarted() const noexcept = 0;

  /// @brief Check current status of the stream processing: finished or no.
  /// @returns true if a previously started stream processing was finished by a call of the Finish()
  /// or FinishBytes() methods
  /// @trace SWS_CRYPT_29015
  virtual bool IsFinished() const noexcept = 0;
};

} // namespace cryp
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_CRYP_DIGEST_SERVICE_H_
