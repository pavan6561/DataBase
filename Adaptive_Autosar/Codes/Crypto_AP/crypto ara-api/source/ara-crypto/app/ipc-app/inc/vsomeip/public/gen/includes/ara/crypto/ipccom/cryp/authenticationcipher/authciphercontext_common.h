// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : authciphercontext_common.h
//
// Purpose     : Implementation of file authciphercontext_common.h
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_AUTHENTICATIONCIPHER_AUTHCIPHERCONTEXT_COMMON_H_
#define CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_AUTHENTICATIONCIPHER_AUTHCIPHERCONTEXT_COMMON_H_

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
namespace authenticationcipher
{

/// @brief Class for AuthCipherContext.
class  AuthCipherContext {
 public:
  /// @brief Definition for AuthCipherContext.
  virtual ~AuthCipherContext() noexcept
  {}
  static constexpr ara::com::internal::ServiceId service_id{0xCD90};
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
    /// @brief Argument for isSuccess.
    Boolean isSuccess;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(isSuccess);
    }
  };
  /// @brief class for GetBlockServiceOutput.
  class GetBlockServiceOutput
  {
    public:
    /// @brief Argument for blockInstanceId.
    CryptoServiceInstanceId blockInstanceId;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(blockInstanceId);
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
    /// @brief Argument for vtrResult.
    ByteVectorType vtrResult;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(vtrResult);
    }
  };
  /// @brief class for GetMaxAssociatedDataSizeOutput.
  class GetMaxAssociatedDataSizeOutput
  {
    public:
    /// @brief Argument for maxDataSize.
    uint64 maxDataSize;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(maxDataSize);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 1>GetTransformation_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUninitializedContext, 0),
    }
  };
  /// @brief class for GetTransformationOutput.
  class GetTransformationOutput
  {
    public:
    /// @brief Argument for CryptoTransform.
    CryptoTransformDataType CryptoTransform;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(CryptoTransform);
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
  static constexpr ara::core::Array<ara::core::ErrorCode, 3>ProcessConfidentialDataReadOnlyMem_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidInputSize, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kAuthTagNotValid, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted, 0),
    }
  };
  /// @brief class for ProcessConfidentialDataReadOnlyMemOutput.
  class ProcessConfidentialDataReadOnlyMemOutput
  {
    public:
    /// @brief Argument for vtrByte.
    ByteVectorType vtrByte;
    /// @brief Using for success.
    using IsEnumerableTag = void;
    template<typename F>
    /// @brief Using for enumerate.
    void enumerate(F& fun) noexcept(false) {
      fun(vtrByte);
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
  static constexpr ara::core::Array<ara::core::ErrorCode, 2>UpdateAssociatedData_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidUsageOrder, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted, 0),
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 2>UpdateAssociatedDataReadOnlyMem_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidUsageOrder, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted, 0),
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 2>UpdateAssociatedDataResUseObj_PossibleErrors {
    {
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidUsageOrder, 0),
    ::ara::crypto::ipccom::common::error::MakeErrorCode(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kProcessingNotStarted, 0),
    }
  };
};

} // namespace authenticationcipher
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara

#endif // CRYPTOCLIENTEXE_INCLUDES_ARA_CRYPTO_IPCCOM_CRYP_AUTHENTICATIONCIPHER_AUTHCIPHERCONTEXT_COMMON_H_

