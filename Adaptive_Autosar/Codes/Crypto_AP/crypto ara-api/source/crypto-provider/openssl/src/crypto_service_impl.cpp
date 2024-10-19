// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_service_imp.cpp
//
// Purpose     :  This file contains the definition required for cryptoservice application.
// -----------------------------------------------------------------------------------------------------------

#include "crypto_service_impl.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
namespace blocksizes
{
size_t const kMaxBlockSize{15};
size_t const kMinBlockSize{8};
size_t const kRSA2048BlockSize{256};
size_t const kRSA2048MaxInputSize{244};
} // namespace blocksizes

CryptoServiceImpl::CryptoServiceImpl(CryptoAlgId algoid, size_t currentKeyBitSize,
    AllowedUsageFlags currentUsageFlags, CryptoObjectUid id, bool isKeyAvailableinCrrentCtx) noexcept :
    algId_{algoid},
    extensionServiInstance_{new ExtensionServiceImpl(
        algoid, currentKeyBitSize, currentUsageFlags, id, isKeyAvailableinCrrentCtx)},
    aralog_{ara::log::CreateLogger("CSIP", "Crypto Service context", ara::log::LogLevel::kVerbose)}
{
  aralog_.LogDebug() << "CryptoServiceImpl::CryptoServiceImpl()";
}

CryptoServiceImpl::~CryptoServiceImpl() noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::~CryptoServiceImpl()";
  delete extensionServiInstance_;
}

std::size_t CryptoServiceImpl::GetMaxInputSize(bool suppressPadding) const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::GetMaxInputSize()";
  static_cast<void>(suppressPadding);

  if ((algId_ == algorithmids::kAES128CBC) || (algId_ == algorithmids::kAES192CBC)
      || (algId_ == algorithmids::kAES256CBC) || (algId_ == algorithmids::kAES128ECB)
      || (algId_ == algorithmids::kAES192ECB) || (algId_ == algorithmids::kAES256ECB))
  {
    return blocksizes::kMaxBlockSize;
  }
  else
  {
    return blocksizes::kRSA2048MaxInputSize;
  }
}

std::size_t CryptoServiceImpl::GetMaxOutputSize(bool suppressPadding) const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::GetMaxOutputSize()";
  static_cast<void>(suppressPadding);

  if ((algId_ == algorithmids::kAES128CBC) || (algId_ == algorithmids::kAES192CBC)
      || (algId_ == algorithmids::kAES256CBC) || (algId_ == algorithmids::kAES128ECB)
      || (algId_ == algorithmids::kAES192ECB) || (algId_ == algorithmids::kAES256ECB))
  {
    return (blocksizes::kMaxBlockSize + 1);
  }
  else
  {
    return blocksizes::kRSA2048BlockSize;
  }
}

std::size_t CryptoServiceImpl::GetBlockSize() const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::GetBlockSize()";

  if ((algId_ == algorithmids::kAES128CBC) || (algId_ == algorithmids::kAES192CBC)
      || (algId_ == algorithmids::kAES256CBC) || (algId_ == algorithmids::kAES128ECB)
      || (algId_ == algorithmids::kAES192ECB) || (algId_ == algorithmids::kAES256ECB))
  {
    aralog_.LogDebug() << "Current block size in CryptoServiceDemon is : " << (blocksizes::kMaxBlockSize + 1);
    return (blocksizes::kMaxBlockSize + 1);
  }
  else
  {
    return blocksizes::kRSA2048BlockSize;
  }
}

bool CryptoServiceImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::IsKeyBitLengthSupported()";
  return extensionServiInstance_->IsKeyBitLengthSupported(keyBitLength);
}

std::size_t CryptoServiceImpl::GetMinKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::GetMinKeyBitLength()";
  return extensionServiInstance_->GetMinKeyBitLength();
}

std::size_t CryptoServiceImpl::GetMaxKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::GetMaxKeyBitLength()";
  return extensionServiInstance_->GetMaxKeyBitLength();
}

std::size_t CryptoServiceImpl::GetActualKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::GetActualKeyBitLength()";
  return extensionServiInstance_->GetActualKeyBitLength();
}

AllowedUsageFlags CryptoServiceImpl::GetAllowedUsage() const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::GetAllowedUsage()";
  return extensionServiInstance_->GetAllowedUsage();
}

CryptoObjectUid CryptoServiceImpl::GetActualKeyCOUID() const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::GetAllowedUsage()";
  return extensionServiInstance_->GetActualKeyCOUID();
}

bool CryptoServiceImpl::IsKeyAvailable() const noexcept
{
  aralog_.LogDebug() << "CryptoServiceImpl::GetAllowedUsage()";
  return extensionServiInstance_->IsKeyAvailable();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
