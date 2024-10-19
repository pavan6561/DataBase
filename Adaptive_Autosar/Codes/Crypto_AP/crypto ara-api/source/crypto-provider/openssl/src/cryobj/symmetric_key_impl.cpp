// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : symmetric_key_impl.cpp
//
// Purpose     : This file contains implementation for SymmetricKeyImpl class.
// -----------------------------------------------------------------------------------------------------------

#include "symmetric_key_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
SymmetricKeyImpl::SymmetricKeyImpl(ReadWriteMemRegion key, Usage usageFlagsVal,
  ara::core::String primitiveName, CryptoPrimitiveId::AlgId primitiId, bool sessionValue,
  bool exportableValue, CryptoObjectUid uuid) noexcept :
  SymmetricKey{ key },
  primitiveNameValue{ primitiveName }, primitiIdValue{ primitiId }, usageFlags{ usageFlagsVal },
  isSessionValue{ sessionValue }, isExportableValue{ exportableValue }
{
  aralog_.LogDebug() << "In SymmetricKeyImpl constructor";
  objectDependantID.mCOType = CryptoObjectType::kUndefined;
  objectID.mCOType = CryptoObjectType::kSymmetricKey;
  objectID.mCouid = uuid;
}

CryptoPrimitiveId::Uptr SymmetricKeyImpl::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyImpl::GetCryptoPrimitiveId()";
  aralog_.LogDebug() << "GetCryptoPrimitiveId() : Primitive Name is " << primitiveNameValue;
  aralog_.LogDebug() << "GetCryptoPrimitiveId() : Primitive Id is " << primitiIdValue;

  CryptoPrimitiveId::Uptr localPtr{ std::make_unique<CryptoPrimitiveIdImpl>(
    primitiveNameValue, primitiIdValue) };

  return localPtr;
}

std::size_t SymmetricKeyImpl::GetPayloadSize() const noexcept
{
  aralog_.LogDebug() << "In SymmetricKeyImpl::GetPayloadSize()";
  aralog_.LogDebug() << "GetPayloadSize() : payload size is " << this->GetSymmetricKey().size();

  return this->GetSymmetricKey().size();
}

} // namespace cryp
} // namespace crypto
} // namespace ara