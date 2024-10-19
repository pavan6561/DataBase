// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : digest_service_impl.cpp
//
// Purpose     :  This file contains definitions for DigestService class implementation.
// -----------------------------------------------------------------------------------------------------------

#include "digest_service_impl.h"

namespace ara
{
namespace crypto
{
namespace cryp
{
DigestServiceImpl::DigestServiceImpl(CryptoAlgId algoid, CryptoObjectUid keyuuid, CryptoObjectUid* seeduuid,
    size_t keySize, size_t ivSize, AllowedUsageFlags usage, ReadOnlyMemRegion digest, bool isCtxInitialized,
    bool isStarted, bool isFinished) noexcept :
    blockServiceInstance_{
        new BlockServiceImpl(algoid, keyuuid, seeduuid, keySize, ivSize, usage, isCtxInitialized)},
    algId_{algoid}, isProcessStarted_{isStarted}, isProcessFinished_{isFinished},
    aralog_{ara::log::CreateLogger("CDS", "Crypto Digest context", ara::log::LogLevel::kVerbose)}
{
  aralog_.LogDebug() << "In DigestServiceImpl::DigestServiceImpl()";

  for (size_t index{0}; index < digest.size(); index++)
  {
    digetsValue_.push_back(digest[index]);
  }
}

DigestServiceImpl::~DigestServiceImpl() noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::~DigestServiceImpl()";
  delete blockServiceInstance_;
}

std::size_t DigestServiceImpl::GetDigestSize() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::GetDigestSize()";
  return digetsValue_.size();
}

ara::core::Result<bool> DigestServiceImpl::Compare(ReadOnlyMemRegion expected, std::size_t offset) const
    noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::GetDigestSize()";
  uint8_t const kEightByteSize{8};

  if (isProcessFinished_ == false)
  {
    aralog_.LogError() << "Processing is not finished yet";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kProcessingNotFinished);
  }

  if (((blockServiceInstance_->GetAllowedUsage() & kAllowSignature) == 0x00)
      && (expected.size() < kEightByteSize) && (algId_ != algorithmids::ksha256Hash))
  {
    aralog_.LogError() << "Allow Signature permission is not given for the deployed key!!!!";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kBruteForceRisk);
  }

  for (size_t index{offset}; index < digetsValue_.size(); index++)
  {
    if (digetsValue_[index] != expected[index])
    {
      return ara::core::Result<bool>::FromValue(false);
    }
  }

  return ara::core::Result<bool>::FromValue(true);
}

bool DigestServiceImpl::IsStarted() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::IsStarted()";
  return isProcessStarted_;
}

bool DigestServiceImpl::IsFinished() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::IsFinished()";
  return isProcessFinished_;
}

std::size_t DigestServiceImpl::GetIvSize() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::IsFinished()";
  return blockServiceInstance_->GetIvSize();
}

std::size_t DigestServiceImpl::GetBlockSize() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::GetBlockSize()";
  return blockServiceInstance_->GetBlockSize();
}

bool DigestServiceImpl::IsValidIvSize(std::size_t ivSize) const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::IsValidIvSize()";
  return blockServiceInstance_->IsValidIvSize(ivSize);
}

std::size_t DigestServiceImpl::GetActualIvBitLength(ara::core::Optional<CryptoObjectUid> ivUid) const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::GetActualIvBitLength()";
  return blockServiceInstance_->GetActualIvBitLength(ivUid);
}

bool DigestServiceImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::IsKeyBitLengthSupported()";
  return blockServiceInstance_->IsKeyBitLengthSupported(keyBitLength);
}

std::size_t DigestServiceImpl::GetMinKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::GetMinKeyBitLength()";
  return blockServiceInstance_->GetMinKeyBitLength();
}

std::size_t DigestServiceImpl::GetMaxKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::GetMaxKeyBitLength()";
  return blockServiceInstance_->GetMaxKeyBitLength();
}

std::size_t DigestServiceImpl::GetActualKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::GetActualKeyBitLength()";
  return blockServiceInstance_->GetActualKeyBitLength();
}

AllowedUsageFlags DigestServiceImpl::GetAllowedUsage() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::GetAllowedUsage()";
  return blockServiceInstance_->GetAllowedUsage();
}

CryptoObjectUid DigestServiceImpl::GetActualKeyCOUID() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::GetActualKeyCOUID()";
  return blockServiceInstance_->GetActualKeyCOUID();
}

bool DigestServiceImpl::IsKeyAvailable() const noexcept
{
  aralog_.LogDebug() << "In DigestServiceImpl::IsKeyAvailable()";
  return blockServiceInstance_->IsKeyAvailable();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
