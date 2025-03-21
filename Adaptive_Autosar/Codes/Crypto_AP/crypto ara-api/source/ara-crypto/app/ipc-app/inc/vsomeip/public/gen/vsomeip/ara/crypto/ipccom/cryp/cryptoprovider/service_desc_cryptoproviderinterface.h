// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : service_desc_cryptoproviderinterface.h
//
// Purpose     : Implementation of file service_desc_cryptoproviderinterface.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOCLIENTEXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_CRYPTOPROVIDER_SERVICE_DESC_CRYPTOPROVIDERINTERFACE_H_
#define CRYPTOCLIENTEXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_CRYPTOPROVIDER_SERVICE_DESC_CRYPTOPROVIDERINTERFACE_H_

#include "ara/crypto/ipccom/cryp/cryptoprovider/cryptoproviderinterface_common.h"


#include <ara/com/internal/vsomeip/vsomeip_types.h>


namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace cryptoprovider
{

namespace cryptoproviderinterface_binding
{
namespace vsomeip
{
namespace descriptors
{

namespace internal
{

/// @brief class for Service.
class Service {
 public:
  /// @brief Service Destructor.
  virtual ~Service() noexcept {};
  static constexpr ara::com::internal::vsomeip::types::ServiceId service_id{0x2649};
  static constexpr ara::com::internal::vsomeip::types::ServiceVersionMajor service_version_major{0x1};
  static constexpr ara::com::internal::vsomeip::types::ServiceVersionMinor service_version_minor{0x0};
  static constexpr ara::com::internal::vsomeip::types::ServiceVersionMinor required_minimum_minor_version{0xFFFFFFFFU};
  
  static constexpr bool is_minimum_minor_policy_enabled{false};
  static constexpr uint32_t blacklisted_versions_length{0};
  static constexpr ara::com::internal::vsomeip::types::BlacklistedVersion blacklisted_versions[1] {
            // If blacklistedVersions are absence then just a placeholder  { 0x0, 0x0 } is added. Value will never be used but required for compilation.
            { 0x0, 0x0 }
      };
    };
}
/// @brief class for CreateAuthCipher.
class CreateAuthCipher : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x8};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateDecryptorPrivate.
class CreateDecryptorPrivate : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0xB};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateEncryptorPublic.
class CreateEncryptorPublic : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0xA};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateHashDigest.
class CreateHashDigest : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x1C};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateHashFunction.
class CreateHashFunction : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x2};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateKeyAgreementPrivate.
class CreateKeyAgreementPrivate : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x12};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateKeyDecapsulatorPrivate.
class CreateKeyDecapsulatorPrivate : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0xD};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateKeyDerivationFunction.
class CreateKeyDerivationFunction : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x9};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateKeyEncapsulatorPublic.
class CreateKeyEncapsulatorPublic : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0xC};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateMessageAuthCode.
class CreateMessageAuthCode : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x3};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateMsgRecoveryPublic.
class CreateMsgRecoveryPublic : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0xF};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateRandomGenerator.
class CreateRandomGenerator : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x1};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateSigEncodePrivate.
class CreateSigEncodePrivate : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0xE};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateSignature.
class CreateSignature : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x1B};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateSignerPrivate.
class CreateSignerPrivate : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x10};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateStreamCipher.
class CreateStreamCipher : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x7};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateSymmetricBlockCipher.
class CreateSymmetricBlockCipher : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x5};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateSymmetricKeyWrapper.
class CreateSymmetricKeyWrapper : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x6};
  static constexpr bool is_reliable{false};
};
/// @brief class for CreateVerifierPublic.
class CreateVerifierPublic : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x11};
  static constexpr bool is_reliable{false};
};
/// @brief class for GenPrivateKey.
class GenPrivateKey : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x14};
  static constexpr bool is_reliable{false};
};
/// @brief class for GenSeed.
class GenSeed : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x15};
  static constexpr bool is_reliable{false};
};
/// @brief class for GenSymmetricKey.
class GenSymmetricKey : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x4};
  static constexpr bool is_reliable{false};
};
/// @brief class for LoadObject.
class LoadObject : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x16};
  static constexpr bool is_reliable{false};
};
/// @brief class for LoadPrivateKey.
class LoadPrivateKey : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x19};
  static constexpr bool is_reliable{false};
};
/// @brief class for LoadPublicKey.
class LoadPublicKey : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x18};
  static constexpr bool is_reliable{false};
};
/// @brief class for LoadSecretSeed.
class LoadSecretSeed : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x1A};
  static constexpr bool is_reliable{false};
};
/// @brief class for LoadSymmetricKey.
class LoadSymmetricKey : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x17};
  static constexpr bool is_reliable{false};
};
/// @brief class for StopCryptoProviderService.
class StopCryptoProviderService : public internal::Service {
 public:
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id{0x13};
  static constexpr bool is_reliable{false};
};



} // namespace descriptors
} // namespace vsomeip
} // namespace cryptoproviderinterface_binding
} // namespace cryptoprovider
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOCLIENTEXE_VSOMEIP_ARA_CRYPTO_IPCCOM_CRYP_CRYPTOPROVIDER_SERVICE_DESC_CRYPTOPROVIDERINTERFACE_H_


