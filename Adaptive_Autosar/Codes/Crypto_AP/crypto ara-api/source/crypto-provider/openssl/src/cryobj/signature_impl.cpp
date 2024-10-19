// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature_impl.cpp
//
// Purpose     : This file contains implementation for SignatureImpl class.
// -----------------------------------------------------------------------------------------------------------

#include "signature_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
SignatureImpl::SignatureImpl(ReadWriteMemRegion digest, ara::core::String primitiveNameValue,
  CryptoPrimitiveId::AlgId primitiId, CryptoPrimitiveId::AlgId hashAlgoIdValue, size_t requiredHashSizeValue,
  bool isSession, bool isExportable, const RestrictedUseObject* key) :
  Signature{ digest },
  primitiveName{ primitiveNameValue }, primitiIdValue{ primitiId }, hashAlgoId{ hashAlgoIdValue },
  requiredHashSize{ requiredHashSizeValue }, isSessionValue{ isSession }, isExportableValue{ isExportable }
{
  aralog_.LogDebug() << "In SignatureImpl constructor";
  if (key != nullptr)
  {
    COIdentifier const keyIdentifier{ key->GetObjectId() };
    objectDependantID.mCOType = keyIdentifier.mCOType;
    objectDependantID.mCouid = keyIdentifier.mCouid;
  }
  objectID.mCOType = CryptoObjectType::kSignature;
}

} // namespace cryp
} // namespace crypto
} // namespace ara