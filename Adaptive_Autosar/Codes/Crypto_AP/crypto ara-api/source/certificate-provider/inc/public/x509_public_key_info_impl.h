// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_public_key_info_impl.h
//
// Purpose     : This file contains Cryptographic API. X509PublicKeyInfo interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_PUBLIC_KEY_INFO_IMPL_H_
#define CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_PUBLIC_KEY_INFO_IMPL_H_

#include "algorithm_identification.h"
#include "ara/crypto/common/io_interface.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/x509/x509_public_key_info.h"
#include "crypto_primitive_id_impl.h"

namespace ara
{
namespace crypto
{
namespace x509
{
/// @brief X.509 Public Key Information interface.
/// @trace SWS_CRYPT_24400
class X509PublicKeyInfoImpl : public X509PublicKeyInfo
{
 public:
  /// @brief Constructor for X509PublicKeyInfoImpl class.
  X509PublicKeyInfoImpl(cryp::CryptoProvider::Uptr cryptoProvider, IOInterface::Uptr ioIntrfc) noexcept :
      X509PublicKeyInfo{}, cryptoProviderPtr_{std::move(cryptoProvider)}, ioPtr_{std::move(ioIntrfc)}
  {
  }

  /// @brief Get the CryptoPrimitiveId instance of this class.
  /// @trace SWS_CRYPT_24410
  ara::crypto::cryp::CryptoPrimitiveId::Uptrc GetAlgorithmId() const override;

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
      FormatId formatId = kFormatDefault) const noexcept override
  {
    static_cast<void>(formatId);
    ara::core::Vector<ara::core::Byte> tmpVtr;
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(tmpVtr);
  }

 private:
  /// @brief smart unique pointer to the target crypto provider.
  cryp::CryptoProvider::Uptr cryptoProviderPtr_;

  /// @brief smart unique pointer to the IOInterface.
  IOInterface::Uptr ioPtr_;

  /// @brief logger variable for ara::log.
  ara::log::Logger& aralog_{
      ara::log::CreateLogger("XPKI", "X509 Public Key Info", ara::log::LogLevel::kVerbose)};
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // SOURCE_AUTOSAR_HEADER_FILES_ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H_
