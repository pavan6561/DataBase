// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : block_service_impl.cpp
//
// Purpose     : This file contains implementation for BlockServiceImpl class.
// -----------------------------------------------------------------------------------------------------------

#include "block_service_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
namespace blocksizes
{
size_t const kIvSizeAES{16};
size_t const kBlockSizeAES{16};
size_t const kIvSizeGCM{12};
size_t const kZeroIvSize{0};
size_t const kOneByteSize{8};
} // namespace blocksizes

BlockServiceImpl::BlockServiceImpl(CryptoAlgId algoid, CryptoObjectUid keyuuid, CryptoObjectUid* seeduuid,
    size_t keySize, size_t ivSize, AllowedUsageFlags usage, bool isCtxInitialized) noexcept :
    algId_{algoid},
    seedUuId_{seeduuid}, ivSizeInBits{ivSize}, extservice_{new ExtensionServiceImpl(
                                                   algoid, keySize, usage, keyuuid, isCtxInitialized)},
    aralog_{ara::log::CreateLogger("BLCS", "BlockService context", ara::log::LogLevel::kVerbose)}
{
  aralog_.LogDebug() << "In BlockServiceImpl::BlockServiceImpl()";
}

BlockServiceImpl::~BlockServiceImpl() noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::~BlockServiceImpl()";
  delete extservice_;
}

std::size_t BlockServiceImpl::GetIvSize() const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::GetIvSize()";

  if ((algId_ == algorithmids::kAES128CBC) || (algId_ == algorithmids::kAES192CBC)
      || (algId_ == algorithmids::kAES192CBC) || (algId_ == algorithmids::kAES128ECB)
      || (algId_ == algorithmids::kAES192ECB) || (algId_ == algorithmids::kAES256ECB)
      || (algId_ == algorithmids::kAES128CTR) || (algId_ == algorithmids::kAES192CTR)
      || (algId_ == algorithmids::kAES256CTR))
  {
    return blocksizes::kIvSizeAES;
  }

  else if ((algId_ == algorithmids::kAES128GCM) || (algId_ == algorithmids::kAES192GCM)
      || (algId_ == algorithmids::kAES256GCM) || (algId_ == algorithmids::kGMAC128)
      || (algId_ == algorithmids::kGMAC192) || (algId_ == algorithmids::kGMAC256))
  {
    return blocksizes::kIvSizeGCM;
  }

  else
  {
    return blocksizes::kZeroIvSize;
  }
}

std::size_t BlockServiceImpl::GetBlockSize() const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::GetBlockSize()";

  if ((algId_ == algorithmids::kAES128CBC) || (algId_ == algorithmids::kAES192CBC)
      || (algId_ == algorithmids::kAES192CBC) || (algId_ == algorithmids::kAES128ECB)
      || (algId_ == algorithmids::kAES192ECB) || (algId_ == algorithmids::kAES256ECB)
      || (algId_ == algorithmids::kAES128CTR) || (algId_ == algorithmids::kAES192CTR)
      || (algId_ == algorithmids::kAES256CTR) || (algId_ == algorithmids::kAES128GCM)
      || (algId_ == algorithmids::kAES192GCM) || (algId_ == algorithmids::kAES256GCM))
  {
    return blocksizes::kBlockSizeAES;
  }

  else
  {
    return blocksizes::kOneByteSize;
  }
}

bool BlockServiceImpl::IsValidIvSize(std::size_t ivSize) const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::IsKeyBitLengthSupported()";

  if (((algId_ == algorithmids::kAES128CBC) || (algId_ == algorithmids::kAES192CBC)
          || (algId_ == algorithmids::kAES192CBC) || (algId_ == algorithmids::kAES128ECB)
          || (algId_ == algorithmids::kAES192ECB) || (algId_ == algorithmids::kAES256ECB)
          || (algId_ == algorithmids::kAES128CTR) || (algId_ == algorithmids::kAES192CTR)
          || (algId_ == algorithmids::kAES256CTR))
      && (ivSize == blocksizes::kIvSizeAES))
  {
    return true;
  }

  else if (((algId_ == algorithmids::kAES128GCM) || (algId_ == algorithmids::kAES192GCM)
               || (algId_ == algorithmids::kAES256GCM) || (algId_ == algorithmids::kGMAC128)
               || (algId_ == algorithmids::kGMAC192) || (algId_ == algorithmids::kGMAC256))
      && (ivSize == blocksizes::kIvSizeGCM))
  {
    return true;
  }

  else if (((algId_ == algorithmids::ksha256Hash) || (algId_ == algorithmids::kHMAC)
               || (algId_ == algorithmids::kCMAC128) || (algId_ == algorithmids::kCMAC192)
               || (algId_ == algorithmids::kCMAC256))
      && (ivSize == blocksizes::kZeroIvSize))
  {
    return true;
  }

  return false;
}

std::size_t BlockServiceImpl::GetActualIvBitLength(ara::core::Optional<CryptoObjectUid> ivUid) const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::GetActualIvBitLength()";

  if (seedUuId_ != nullptr)
  {
    ivUid->mGeneratorUid = seedUuId_->mGeneratorUid;
    ivUid->mVersionStamp = seedUuId_->mVersionStamp;
  }

  return ivSizeInBits;
}

bool BlockServiceImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::IsKeyBitLengthSupported()";

  return extservice_->IsKeyBitLengthSupported(keyBitLength);
}

std::size_t BlockServiceImpl::GetMinKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::GetMinKeyBitLength()";

  return extservice_->GetMinKeyBitLength();
}

std::size_t BlockServiceImpl::GetMaxKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::GetMaxKeyBitLength()";

  return extservice_->GetMaxKeyBitLength();
}

std::size_t BlockServiceImpl::GetActualKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::GetActualKeyBitLength()";
  return extservice_->GetActualKeyBitLength();
}

AllowedUsageFlags BlockServiceImpl::GetAllowedUsage() const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::GetAllowedUsage()";
  return extservice_->GetAllowedUsage();
}

CryptoObjectUid BlockServiceImpl::GetActualKeyCOUID() const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::GetActualKeyCOUID()";
  return extservice_->GetActualKeyCOUID();
}

bool BlockServiceImpl::IsKeyAvailable() const noexcept
{
  aralog_.LogDebug() << "In BlockServiceImpl::IsKeyAvailable()";
  return extservice_->IsKeyAvailable();
}

} // namespace cryp
} // namespace crypto
} // namespace ara