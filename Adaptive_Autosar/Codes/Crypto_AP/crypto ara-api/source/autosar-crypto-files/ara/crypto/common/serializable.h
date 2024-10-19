// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : serializable.h
//
// Purpose     : This file contains definitions for Serializable abstract class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_SERIALIZABLE_H_
#define CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_SERIALIZABLE_H_

#include <ara/core/result.h>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief Serializable object interface.
/// @trace SWS_CRYPT_10700
class Serializable
{
 public:
  /// @brief Constructor for Serializable class.
  Serializable() noexcept = default;

  /// @brief Move constructor for Serializable class.
  Serializable(Serializable&& other) noexcept = default;

  /// @brief Destructor.
  /// @trace SWS_CRYPT_10710
  virtual ~Serializable() noexcept = default;

  /// @brief Copy-assign another Serializable to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30204
  Serializable& operator=(const Serializable& other) = default;

  /// @brief Move-assign another Serializable to this instance.
  /// @param other  the other instance
  /// @returns *this, containing the contents of @a other
  /// @trace SWS_CRYPT_30205
  Serializable& operator=(Serializable&& other) = default;

  /// @brief A container type for the encoding format identifiers.
  /// @trace SWS_CRYPT_10701
  using FormatId = std::uint32_t;

  /// @brief Default serialization format.
  /// @trace SWS_CRYPT_10750
  static FormatId const kFormatDefault{ 0 };

  /// @brief Export only raw value of an object.
  /// @trace SWS_CRYPT_10751
  static FormatId const kFormatRawValueOnly{ 1 };

  /// @brief Export DER-encoded value of an object.
  /// @trace SWS_CRYPT_10752
  static FormatId const kFormatDerEncoded{ 2 };

  /// @brief Export PEM-encoded value of an object.
  /// @trace SWS_CRYPT_10753
  static FormatId const kFormatPemEncoded{ 3 };

  /// @brief Serialize itself publicly.
  /// @param[in] formatId  the Crypto Provider specific identifier of the output format
  /// @returns a buffer with the serialized object
  /// @error SecurityErrorDomain::kInsufficientCapacity
  /// if <tt>(output.empty() == false)</tt>, but it's capacity is less than required
  /// @error SecurityErrorDomain::kUnknownIdentifier  if an unknown format ID was specified
  /// @error SecurityErrorDomain::kUnsupportedFormat
  /// if the specified format ID is not supported for this object type
  /// @trace SWS_CRYPT_10711
  virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
    FormatId formatId = kFormatDefault) const noexcept
    = 0;

  /// @brief Serialize itself publicly.
  /// This method sets the size of the output container according to actually saved value!
  /// @tparam Alloc custom allocator type of the output container
  /// @returns  pre-reserved managed container for the serialization output
  /// @param[in] formatId  the Crypto Provider specific identifier of the output format
  /// @error SecurityErrorDomain::kInsufficientCapacity
  /// if capacity of the output buffer is less than required
  /// @error SecurityErrorDomain::kUnknownIdentifier  if an unknown format ID was specified
  /// @error SecurityErrorDomain::kUnsupportedFormat
  /// if the specified format ID is not supported for this object type
  /// @trace SWS_CRYPT_10712
  //template <typename Alloc = VENDOR_IMPLEMENTATION_DEFINED>
  //ara::core::Result<ByteVector<Alloc>> ExportPublicly(FormatId formatId = kFormatDefault
  //) const noexcept
  //{
  //    	auto result = ExportPublicly(formatId);
  //        if (result)
  //        {
  //            ByteVector<Alloc> output;
  //            output.resize(result.Value().size());
  //            memcpy(core::data(output), result.Value().data(), result.Value().size());
  //            return ara::core::Result<ByteVector<Alloc>>::FromValue(output);
  //        }
  //        else
  //        {
  //            return ara::core::Result<ByteVector<Alloc>>::FromError(result.Error());
  //        }
  //}
};

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_AUTOSAR_CRYPTO_FILES_ARA_CRYPTO_COMMON_SERIALIZABLE_H_
