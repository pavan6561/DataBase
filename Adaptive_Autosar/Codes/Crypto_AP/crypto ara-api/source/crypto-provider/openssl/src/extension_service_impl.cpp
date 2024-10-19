// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : extension_service_impl.cpp
//
// Purpose     : This file contains implementation for ExtensionServiceImpl class.
// -----------------------------------------------------------------------------------------------------------

#include "extension_service_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
namespace keybitSize
{
size_t const kKeySize128Bit{128};
size_t const kKeySize192Bit{192};
size_t const kKeySize256Bit{256};
size_t const kKeySize2048Bit{2048};
} // namespace keybitSize
ExtensionServiceImpl::ExtensionServiceImpl(CryptoAlgId algoid, size_t currentKeyBitSize,
    AllowedUsageFlags currentUsageFlags, CryptoObjectUid id, bool isKeyAvailableinCrrentCtx) noexcept :
    algId_{algoid},
    currentKeyLength_{currentKeyBitSize}, usageFlags_{currentUsageFlags}, objectId_{id},
    isKeyAvaible_{isKeyAvailableinCrrentCtx}, aralog_{ara::log::CreateLogger("EXTS",
                                                  "ExtensionService context", ara::log::LogLevel::kVerbose)}
{
}

bool ExtensionServiceImpl::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
{
  aralog_.LogDebug() << "In ExtensionServiceImpl::IsKeyBitLengthSupported()";

  if (((algId_ == algorithmids::kAES128CBC) || (algId_ == algorithmids::kAES128ECB)
          || (algId_ == algorithmids::kAES128CTR) || (algId_ == algorithmids::kAES128GCM)
          || (algId_ == algorithmids::kKeyWrap128) || (algId_ == algorithmids::kKeyWrapPad128)
          || (algId_ == algorithmids::kHMAC) || (algId_ == algorithmids::kCMAC128)
          || (algId_ == algorithmids::kGMAC128))
      && (keyBitLength == keybitSize::kKeySize128Bit))
  {
    return true;
  }

  // If target algorithm Id requires 192 bit key.
  else if (((algId_ == algorithmids::kAES192CBC) || (algId_ == algorithmids::kAES192CTR)
               || (algId_ == algorithmids::kAES192ECB) || (algId_ == algorithmids::kAES192GCM)
               || (algId_ == algorithmids::kKeyWrap192) || (algId_ == algorithmids::kKeyWrapPad192)
               || (algId_ == algorithmids::kCMAC192) || (algId_ == algorithmids::kGMAC192))
      && (keyBitLength == keybitSize::kKeySize192Bit))
  {
    return true;
  }

  // If target algorithm Id requires 256 bit key.
  else if (((algId_ == algorithmids::kAES256ECB) || (algId_ == algorithmids::kAES256CBC)
               || (algId_ == algorithmids::kAES256CTR) || (algId_ == algorithmids::kAES256GCM)
               || (algId_ == algorithmids::kKeyWrap256) || (algId_ == algorithmids::kKeyWrapPad256)
               || (algId_ == algorithmids::kCMAC256) || (algId_ == algorithmids::kGMAC256))
      && (keyBitLength == keybitSize::kKeySize256Bit))
  {
    return true;
  }

  else if (((algId_ == algorithmids::kRSA2048) || (algId_ == algorithmids::kRSA2048PKCSV1_5)
               || (algId_ == algorithmids::kRSASSA2048PKCSV1_5) || (algId_ == algorithmids::kEC)
               || (algId_ == algorithmids::kDSA) || (algId_ == algorithmids::kDH)
               || (algId_ == algorithmids::kECDSA) || (algId_ == algorithmids::kECDSAP256)
               || (algId_ == algorithmids::kECDH))
      && (keyBitLength == keybitSize::kKeySize2048Bit))
  {
    return true;
  }

  else if (((algId_ == algorithmids::kDrbgCtrBasedRng) || (algId_ == algorithmids::kPBKDF2))
      && ((keyBitLength == keybitSize::kKeySize128Bit) || (keyBitLength == keybitSize::kKeySize192Bit)
             || (keyBitLength == keybitSize::kKeySize256Bit)))
  {
    return true;
  }

  else
  {
    return false;
  }
}

std::size_t ExtensionServiceImpl::GetMinKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In ExtensionServiceImpl::GetMinKeyBitLength()";

  if ((algId_ == algorithmids::kAES128CBC) || (algId_ == algorithmids::kAES128ECB)
      || (algId_ == algorithmids::kAES128CTR) || (algId_ == algorithmids::kAES128GCM)
      || (algId_ == algorithmids::kKeyWrap128) || (algId_ == algorithmids::kKeyWrapPad128)
      || (algId_ == algorithmids::kHMAC) || (algId_ == algorithmids::kCMAC128)
      || (algId_ == algorithmids::kGMAC128))
  {
    return keybitSize::kKeySize128Bit;
  }

  // If target algorithm Id requires 192 bit key.
  else if ((algId_ == algorithmids::kAES192CBC) || (algId_ == algorithmids::kAES192CTR)
      || (algId_ == algorithmids::kAES192ECB) || (algId_ == algorithmids::kAES192GCM)
      || (algId_ == algorithmids::kKeyWrap192) || (algId_ == algorithmids::kKeyWrapPad192)
      || (algId_ == algorithmids::kCMAC192) || (algId_ == algorithmids::kGMAC192))
  {
    return keybitSize::kKeySize192Bit;
  }

  // If target algorithm Id requires 256 bit key.
  else if ((algId_ == algorithmids::kAES256ECB) || (algId_ == algorithmids::kAES256CBC)
      || (algId_ == algorithmids::kAES256CTR) || (algId_ == algorithmids::kAES256GCM)
      || (algId_ == algorithmids::kKeyWrap256) || (algId_ == algorithmids::kKeyWrapPad256)
      || (algId_ == algorithmids::kCMAC256) || (algId_ == algorithmids::kGMAC256))
  {
    return keybitSize::kKeySize256Bit;
  }

  else if ((algId_ == algorithmids::kRSA2048) || (algId_ == algorithmids::kRSA2048PKCSV1_5)
      || (algId_ == algorithmids::kRSASSA2048PKCSV1_5) || (algId_ == algorithmids::kEC)
      || (algId_ == algorithmids::kDSA) || (algId_ == algorithmids::kDH) || (algId_ == algorithmids::kECDSA)
      || (algId_ == algorithmids::kECDSAP256) || (algId_ == algorithmids::kECDH))
  {
    return keybitSize::kKeySize2048Bit;
  }

  else if ((algId_ == algorithmids::kDrbgCtrBasedRng) || (algId_ == algorithmids::kPBKDF2))
  {
    return keybitSize::kKeySize128Bit;
  }

  else
  {
    return 0;
  }
}

std::size_t ExtensionServiceImpl::GetMaxKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In ExtensionServiceImpl::GetMaxKeyBitLength()";

  if ((algId_ == algorithmids::kAES128CBC) || (algId_ == algorithmids::kAES128ECB)
      || (algId_ == algorithmids::kAES128CTR) || (algId_ == algorithmids::kAES128GCM)
      || (algId_ == algorithmids::kKeyWrap128) || (algId_ == algorithmids::kKeyWrapPad128)
      || (algId_ == algorithmids::kHMAC) || (algId_ == algorithmids::kCMAC128)
      || (algId_ == algorithmids::kGMAC128))
  {
    return keybitSize::kKeySize128Bit;
  }

  // If target algorithm Id requires 192 bit key.
  else if ((algId_ == algorithmids::kAES192CBC) || (algId_ == algorithmids::kAES192CTR)
      || (algId_ == algorithmids::kAES192ECB) || (algId_ == algorithmids::kAES192GCM)
      || (algId_ == algorithmids::kKeyWrap192) || (algId_ == algorithmids::kKeyWrapPad192)
      || (algId_ == algorithmids::kCMAC192) || (algId_ == algorithmids::kGMAC192))
  {
    return keybitSize::kKeySize192Bit;
  }

  // If target algorithm Id requires 256 bit key.
  else if ((algId_ == algorithmids::kAES256ECB) || (algId_ == algorithmids::kAES256CBC)
      || (algId_ == algorithmids::kAES256CTR) || (algId_ == algorithmids::kAES256GCM)
      || (algId_ == algorithmids::kKeyWrap256) || (algId_ == algorithmids::kKeyWrapPad256)
      || (algId_ == algorithmids::kCMAC256) || (algId_ == algorithmids::kGMAC256))
  {
    return keybitSize::kKeySize256Bit;
  }

  else if ((algId_ == algorithmids::kRSA2048) || (algId_ == algorithmids::kRSA2048PKCSV1_5)
      || (algId_ == algorithmids::kRSASSA2048PKCSV1_5) || (algId_ == algorithmids::kEC)
      || (algId_ == algorithmids::kDSA) || (algId_ == algorithmids::kDH) || (algId_ == algorithmids::kECDSA)
      || (algId_ == algorithmids::kECDSAP256) || (algId_ == algorithmids::kECDH))
  {
    return keybitSize::kKeySize2048Bit;
  }

  else if ((algId_ == algorithmids::kDrbgCtrBasedRng) || (algId_ == algorithmids::kPBKDF2))
  {
    return keybitSize::kKeySize256Bit;
  }

  else
  {
    return 0;
  }
}

std::size_t ExtensionServiceImpl::GetActualKeyBitLength() const noexcept
{
  aralog_.LogDebug() << "In ExtensionServiceImpl::GetActualKeyBitLength()";
  return currentKeyLength_;
}

AllowedUsageFlags ExtensionServiceImpl::GetAllowedUsage() const noexcept
{
  aralog_.LogDebug() << "In ExtensionServiceImpl::GetAllowedUsage()";
  return usageFlags_;
}

CryptoObjectUid ExtensionServiceImpl::GetActualKeyCOUID() const noexcept
{
  aralog_.LogDebug() << "In ExtensionServiceImpl::GetActualKeyCOUID()";
  return objectId_;
}

bool ExtensionServiceImpl::IsKeyAvailable() const noexcept
{
  aralog_.LogDebug() << "In ExtensionServiceImpl::IsKeyAvailable()";
  return isKeyAvaible_;
}

} // namespace cryp
} // namespace crypto
} // namespace ara