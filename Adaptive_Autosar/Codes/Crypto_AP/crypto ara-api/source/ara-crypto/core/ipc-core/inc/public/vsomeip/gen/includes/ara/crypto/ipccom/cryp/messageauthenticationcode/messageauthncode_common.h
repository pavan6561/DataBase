// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : messageauthncode_common.h
//
// Purpose     : Implementation of file messageauthncode_common.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-03-14T12:58:58+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_MESSAGEAUTHENTICATIONCODE_MESSAGEAUTHNCODE_COMMON_H_
#define CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_MESSAGEAUTHENTICATIONCODE_MESSAGEAUTHNCODE_COMMON_H_

#include <cstring>
#include <ara/core/array.h>

#include <ara/com/types.h>

#include <ara/com/exception.h>
#include <ara/core/error_code.h>

#include "ara/crypto/ipccom/common/error/error_domain_cryptoerrordomain.h"
#include "impl_type_boolean.h"
#include "impl_type_bytevectortype.h"
#include "impl_type_cryptoprimitiveiduptrstructure.h"
#include "impl_type_cryptoreadonlymemregion.h"
#include "impl_type_cryptoserviceinstanceid.h"
#include "impl_type_cryptotransformdatatype.h"
#include "impl_type_restricteduseobjectstructure.h"
#include "impl_type_uint64.h"
#include "impl_type_uint8.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace messageauthenticationcode
{

/// @brief Class for MessageAuthnCode.
class  MessageAuthnCode {
 public:
  /// @brief Definition for MessageAuthnCode.
  virtual ~MessageAuthnCode() noexcept
  {}
  static constexpr ara::com::internal::ServiceId service_id{0xA4FD};
  static constexpr ara::com::internal::ServiceVersion service_version_major{0x0};
  static constexpr ara::com::internal::ServiceVersion service_version_minor{0x1};
  static constexpr ara::core::Array<ara::core::ErrorCode, 2>Check_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject, 0),
    }
  };
  /// @brief class for CheckOutput.
  class CheckOutput
  {
    public:
    /// @brief Argument for result.
    Boolean result;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(result);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 2>Finish_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation, 0),
    }
  };
  /// @brief class for FinishOutput.
  class FinishOutput
  {
    public:
    /// @brief Argument for finishInstanceId.
    CryptoServiceInstanceId finishInstanceId;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(finishInstanceId);
    }
  };
  /// @brief class for GetCryptoPrimitiveIdOutput.
  class GetCryptoPrimitiveIdOutput
  {
    public:
    /// @brief Argument for cryptoPrimitiveIdUptrStruct.
    cryptoPrimitiveIdUptrStructure cryptoPrimitiveIdUptrStruct;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(cryptoPrimitiveIdUptrStruct);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 2>GetDigest_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotFinished, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation, 0),
    }
  };
  /// @brief class for GetDigestOutput.
  class GetDigestOutput
  {
    public:
    /// @brief Argument for digestResult.
    ByteVectorType digestResult;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(digestResult);
    }
  };
  /// @brief class for GetDigestServiceOutput.
  class GetDigestServiceOutput
  {
    public:
    /// @brief Argument for instanceId.
    CryptoServiceInstanceId instanceId;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(instanceId);
    }
  };
  /// @brief class for IsInitializedOutput.
  class IsInitializedOutput
  {
    public:
    /// @brief Argument for result.
    Boolean result;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(result);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 2>SetKey_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleObject, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation, 0),
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 3>StartReadOnlyMem_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext, 0),
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 4>StartSecretSeed_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnsupported, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUsageViolation, 0),
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 1>Update_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted, 0),
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 1>UpdateReadOnlyMem_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted, 0),
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 1>UpdateRestrictedUseObject_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted, 0),
    }
  };
};

} // namespace messageauthenticationcode
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOSERVEREXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_MESSAGEAUTHENTICATIONCODE_MESSAGEAUTHNCODE_COMMON_H_

