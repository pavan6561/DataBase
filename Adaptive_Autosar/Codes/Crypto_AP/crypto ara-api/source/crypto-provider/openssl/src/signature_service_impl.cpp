// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : signature_service_impl.cpp
//
// Purpose     :  This file contains the definition required for signatureservice application.
// -----------------------------------------------------------------------------------------------------------

#include "signature_service_impl.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
SignatureServiceImpl::SignatureServiceImpl(CryptoAlgId algoid, size_t currentKeyBitSize,
    AllowedUsageFlags currentUsageFlags, CryptoObjectUid id, bool isKeyAvailableinCrrentCtx) noexcept :
    algId_{algoid},
    extensionServiInstance_{new ExtensionServiceImpl(
        algoid, currentKeyBitSize, currentUsageFlags, id, isKeyAvailableinCrrentCtx)},
    aralog_{ara::log::CreateLogger("CSS", "Crypto Singnature Service context", ara::log::LogLevel::kVerbose)}
{
  aralog_.LogDebug() << "In SignatureServiceImpl::SignatureServiceImpl()";
}

SignatureServiceImpl::~SignatureServiceImpl() noexcept
{
  aralog_.LogDebug() << "In SignatureServiceImpl::~SignatureServiceImpl()";
  delete extensionServiInstance_;
}

std::size_t SignatureServiceImpl::GetRequiredHashSize() const noexcept
{
  aralog_.LogDebug() << "In SignatureServiceImpl::GetRequiredHashSize()";
  return AlgorithmIdentification::GetAlogIdentification()->GetRequiredHashSize(algId_);
}

CryptoPrimitiveId::AlgId SignatureServiceImpl::GetRequiredHashAlgId() const noexcept
{
  aralog_.LogDebug() << "In SignatureServiceImpl::GetRequiredHashAlgId()";
  return AlgorithmIdentification::GetAlogIdentification()->GetRequiredHashAlgId(algId_);
}

std::size_t SignatureServiceImpl::GetSignatureSize() const noexcept
{
  aralog_.LogDebug() << "In SignatureServiceImpl::GetSignatureSize()";
  return AlgorithmIdentification::GetAlogIdentification()->GetSignatureSize(algId_);
}

bool SignatureServiceImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  aralog_.LogDebug() << "SignatureServiceImpl::IsKeyBitLengthSupported()";
  return extensionServiInstance_->IsKeyBitLengthSupported(keyBitLength);
}

std::size_t SignatureServiceImpl::GetMinKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "SignatureServiceImpl::GetMinKeyBitLength()";
  return extensionServiInstance_->GetMinKeyBitLength();
}

std::size_t SignatureServiceImpl::GetMaxKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "SignatureServiceImpl::GetMaxKeyBitLength()";
  return extensionServiInstance_->GetMaxKeyBitLength();
}

std::size_t SignatureServiceImpl::GetActualKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "SignatureServiceImpl::GetActualKeyBitLength()";
  return extensionServiInstance_->GetActualKeyBitLength();
}

AllowedUsageFlags SignatureServiceImpl::GetAllowedUsage() const noexcept
{
  aralog_.LogDebug() << "SignatureServiceImpl::GetAllowedUsage()";
  return extensionServiInstance_->GetAllowedUsage();
}

CryptoObjectUid SignatureServiceImpl::GetActualKeyCOUID() const noexcept
{
  aralog_.LogDebug() << "SignatureServiceImpl::GetAllowedUsage()";
  return extensionServiInstance_->GetActualKeyCOUID();
}

bool SignatureServiceImpl::IsKeyAvailable() const noexcept
{
  aralog_.LogDebug() << "SignatureServiceImpl::GetAllowedUsage()";
  return extensionServiInstance_->IsKeyAvailable();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
