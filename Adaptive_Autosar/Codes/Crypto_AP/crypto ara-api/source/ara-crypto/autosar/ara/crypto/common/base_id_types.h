// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : base_id_types.h
//
// Purpose     : This file contains definitions of basic data types for crypto provider.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_BASE_ID_TYPES_H_
#define CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_BASE_ID_TYPES_H_

#include <cstdint>

#include <ara/core/vector.h>
#include <memory>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
namespace crypto
{
/// @brief Container type of the Crypto Algorithm Identifier.
/// @trace SWS_CRYPT_10014
using CryptoAlgId = std::uint64_t;

/// @brief A container type and constant bit-flags of allowed usages of a key or a secret seed object.
/// Only directly specified usages of a key are allowed, all other are prohibited!
/// Similar set of flags are defined for the usage restrictions of original key/seed and for a symmetric key
/// or seed that potentially can be derived from the original one.
/// A symmetric key or secret seed can be derived from the original one, only if it supports @c
/// kAllowKeyAgreement or @c kAllowKeyDiversify or @c kAllowKeyDerivation!
/// @trace SWS_CRYPT_10015
using AllowedUsageFlags = std::uint32_t;

/// @brief Enumeration of all types of crypto objects, i.e. types of content that can be stored to a key slot.
/// @trace SWS_CRYPT_10016
enum class CryptoObjectType : std::uint32_t
{

  /// Object type is currently not defined (empty container)
  kUndefined = 0,
  /// @c cryp::SymmetricKey object
  kSymmetricKey = 1,
  /// @c cryp::PrivateKey object
  kPrivateKey = 2,
  /// @c cryp::PublicKey object
  kPublicKey = 3,
  /// @c cryp::Signature object (asymmetric digital signature or symmetric MAC/HMAC or hash digest)
  kSignature = 4,
  /// @c cryp::SecretSeed object. @b Note: the seed cannot have an associated crypto algorithm!
  kSecretSeed = 5

};

/// @brief Enumeration of all known Provider types.
/// @trace SWS_CRYPT_10017
enum class ProviderType : std::uint32_t
{

  /// Undefined/Unknown Provider type (or applicable for the whole Crypto Stack)
  kUndefinedProvider = 0,
  /// Cryptography Provider
  kCryptoProvider = 1,
  /// Key Storage Provider
  kKeyStorageProvider = 2,
  /// X.509 Provider
  kX509Provider = 3

};

/// @brief Enumeration of key-slot types; currently only machine and application key-slots are defined.
/// @trace SWS_CRYPT_10018
enum class KeySlotType : std::uint32_t
{

  /// machine type key-slot - can be managed by application
  kMachine = 1,
  /// application exclusive type key-slot
  kApplication = 2

};

/// @brief Enumeration of cryptographic transformations.
/// @trace SWS_CRYPT_10019
enum class CryptoTransform : std::uint32_t
{

  /// encryption
  kEncrypt = 1,
  /// decryption
  kDecrypt = 2,
  /// MAC verification
  kMacVerify = 3,
  /// MAC generation
  kMacGenerate = 4,
  /// key wrapping
  kWrap = 5,
  /// key unwrapping
  kUnwrap = 6,
  /// signature verification
  kSigVerify = 7,
  /// signature generation
  kSigGenerate = 8

};

// Common grouped constants should be numbered in the way similar to interfaces:
// SWS_CRYPT_1YYXX:
// - ZZ - constants group ID (30-59)
// - XX - constant definition ID (00-99)

/// Effective values of Crypto Algorithm IDs are specific for concrete Crypto Stack implementation.
/// But the zero value is reserved for especial purposes, that can differ depending from a usage context.
/// This group defines a few constant names of the single zero value, but semantically they have different
/// meaning specific for concrete application of the constant.

/// @brief Algorithm ID is undefined.
/// Also this value may be used in meanings: Any or Default algorithm, None of algorithms.
/// @trace SWS_CRYPT_13000
const CryptoAlgId kAlgIdUndefined = 0u;

/// @brief Any Algorithm ID is allowed.
/// @trace SWS_CRYPT_13001
const CryptoAlgId kAlgIdAny = kAlgIdUndefined;

/// @brief Default Algorithm ID (in current context/primitive).
/// @trace SWS_CRYPT_13002
const CryptoAlgId kAlgIdDefault = kAlgIdUndefined;

/// @brief None of Algorithm ID (i.e. an algorithm definition is not applicable).
/// @trace SWS_CRYPT_13003
const CryptoAlgId kAlgIdNone = kAlgIdUndefined;

/// This group contains list of constant 1-bit values predefined for Allowed Usage flags.

/// @brief The key/seed usage will be fully specified by a key slot prototype (the object can be used only
/// after reloading from the slot).
/// @trace SWS_CRYPT_13100
const AllowedUsageFlags kAllowPrototypedOnly = 0;

/// @brief The key/seed can be used for data encryption initialization (applicable to symmetric and asymmetric
/// algorithms).
/// @trace SWS_CRYPT_13101
const AllowedUsageFlags kAllowDataEncryption = 0x0001;

/// @brief The key/seed can be used for data decryption initialization (applicable to symmetric and asymmetric
/// algorithms).
/// @trace SWS_CRYPT_13102
const AllowedUsageFlags kAllowDataDecryption = 0x0002;

/// @brief The key/seed can be used for digital signature or MAC/HMAC production (applicable to symmetric and
/// asymmetric algorithms).
/// @trace SWS_CRYPT_13103
const AllowedUsageFlags kAllowSignature = 0x0004;

/// @brief The key/seed can be used for digital signature or MAC/HMAC verification (applicable to symmetric
/// and.
/// asymmetric algorithms).
/// @trace SWS_CRYPT_13104
const AllowedUsageFlags kAllowVerification = 0x0008;

/// @brief The seed or asymmetric key can be used for key-agreement protocol execution.
/// @trace SWS_CRYPT_13105
const AllowedUsageFlags kAllowKeyAgreement = 0x0010;

/// @brief The seed or symmetric key can be used for slave-keys diversification.
/// @trace SWS_CRYPT_13106
const AllowedUsageFlags kAllowKeyDiversify = 0x0020;

/// @brief The seed or symmetric key can be used for seeding of a RandomGeneratorCtx
/// @trace SWS_CRYPT_13107
const AllowedUsageFlags kAllowRngInit = 0x0040;

/// @brief The object can be used as an input key material to KDF.
/// The seed or symmetric key can be used as a @c RestrictedUseObject for slave-keys derivation via a Key
/// Derivation Function (KDF).
/// @trace SWS_CRYPT_13108
const AllowedUsageFlags kAllowKdfMaterial = 0x0080;

/// @brief The key can be used as "transport" one for Key-Wrap or Encapsulate transformations (applicable to
/// symmetric and asymmetric keys).
/// @trace SWS_CRYPT_13109
const AllowedUsageFlags kAllowKeyExporting = 0x0100;

/// @brief The key can be used as "transport" one for Key-Unwrap or Decapsulate transformations (applicable
/// to symmetric and asymmetric keys).
/// @trace SWS_CRYPT_13110
const AllowedUsageFlags kAllowKeyImporting = 0x0200;

/// @brief The key can be used only for the mode directly specified by @c Key::AlgId.
/// @trace SWS_CRYPT_13111
const AllowedUsageFlags kAllowExactModeOnly = 0x8000;

/// @brief A derived seed or symmetric key can be used for data encryption.
/// @trace SWS_CRYPT_13112
const AllowedUsageFlags kAllowDerivedDataEncryption = kAllowDataEncryption << 16;

/// @brief A derived seed or symmetric key can be used for data decryption.
/// @trace SWS_CRYPT_13113
const AllowedUsageFlags kAllowDerivedDataDecryption = kAllowDataDecryption << 16;

/// @brief A derived seed or symmetric key can be used for MAC/HMAC production.
/// @trace SWS_CRYPT_13114
const AllowedUsageFlags kAllowDerivedSignature = kAllowSignature << 16;

/// @brief A derived seed or symmetric key can be used for MAC/HMAC verification.
/// @trace SWS_CRYPT_13115
const AllowedUsageFlags kAllowDerivedVerification = kAllowVerification << 16;

/// @brief A derived seed or symmetric key can be used for slave-keys diversification.
/// @trace SWS_CRYPT_13116
const AllowedUsageFlags kAllowDerivedKeyDiversify = kAllowKeyDiversify << 16;

/// @brief A derived seed or symmetric key can be used for seeding of a RandomGeneratorContext
/// @trace SWS_CRYPT_13117
const AllowedUsageFlags kAllowDerivedRngInit = kAllowRngInit << 16;

/// @brief A derived seed or symmetric key can be used as a @c RestrictedUseObject for slave-keys derivation
/// via a Key Derivation Function (KDF).
/// @trace SWS_CRYPT_13118
const AllowedUsageFlags kAllowDerivedKdfMaterial = kAllowKdfMaterial << 16;

/// @brief A derived seed or symmetric key can be used as a "transport" one for Key-Wrap transformation.
/// @trace SWS_CRYPT_13119
const AllowedUsageFlags kAllowDerivedKeyExporting = kAllowKeyExporting << 16;

/// @brief A derived seed or symmetric key can be used as a "transport" one for Key-Unwrap transformation.
/// @trace SWS_CRYPT_13120
const AllowedUsageFlags kAllowDerivedKeyImporting = kAllowKeyImporting << 16;

/// @brief Restrict usage of derived objects to specified operation mode only.
/// A derived seed or symmetric key can be used only for the mode directly specified by @c Key::AlgId.
/// @trace SWS_CRYPT_13121
const AllowedUsageFlags kAllowDerivedExactModeOnly = kAllowExactModeOnly << 16;

/// @brief Allow usage of the object as a key material for KDF and any usage of derived objects.
/// The seed or symmetric key can be used as a @c RestrictedUseObject for a Key Derivation Function (KDF) and
/// the derived "slave" keys can be used without limitations.
/// @trace SWS_CRYPT_13122
const AllowedUsageFlags kAllowKdfMaterialAnyUsage = kAllowKdfMaterial | kAllowDerivedDataEncryption
  | kAllowDerivedDataDecryption | kAllowDerivedSignature | kAllowDerivedVerification
  | kAllowDerivedKeyDiversify | kAllowDerivedRngInit | kAllowDerivedKdfMaterial | kAllowDerivedKeyExporting
  | kAllowDerivedKeyImporting;

// just make it compile
// DO NOT MAKE A SWS_ID!!
#define VENDOR_IMPLEMENTATION_DEFINED std::allocator<std::uint8_t>

/// @brief Alias of a bytes' vector template with customizable allocator
/// @tparam Alloc  custom allocator of bytes sequences
/// @trace SWS_CRYPT_10042
template <class Alloc = VENDOR_IMPLEMENTATION_DEFINED>
using ByteVector = ara::core::Vector<std::uint8_t, Alloc>;

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_ARA_CRYPTO_AUTOSAR_ARA_CRYPTO_COMMON_BASE_ID_TYPES_H_
