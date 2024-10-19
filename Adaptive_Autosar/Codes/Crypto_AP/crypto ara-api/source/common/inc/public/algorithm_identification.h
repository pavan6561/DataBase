// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : algorithm_identification.h
//
// Purpose     : This file contains definitions for Algorithm Ids and definition for
//               AlgorithmIdentification class.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_COMMON_INC_PUBLIC_ALGORITHM_IDENTIFICATION_H_
#define CRYPTO_SOURCE_COMMON_INC_PUBLIC_ALGORITHM_IDENTIFICATION_H_

#include <ara/core/map.h>
#include <ara/core/string.h>

#include "ara/crypto/common/base_id_types.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
namespace opresults
{
uint8_t constexpr kFailure{ 0 };
int8_t constexpr kSuccess{ 1 };
} // namespace opresults

namespace algorithmids
{
/// @brief Algorithm Id for RNG.
CryptoAlgId constexpr kDrbgCtrBasedRng{ 2 };
/// @brief Algorithm Id for KDF.
CryptoAlgId constexpr kPBKDF2{ 100 };
/// @brief Algorithm Id for HMAC.
CryptoAlgId constexpr kHMAC{ 300 };
/// @brief Algorithm Id for CMAC-128.
CryptoAlgId constexpr kCMAC128{ 301 };
/// @brief Algorithm Id for CMAC-192.
CryptoAlgId constexpr kCMAC192{ 302 };
/// @brief Algorithm Id for CMAC-256.
CryptoAlgId constexpr kCMAC256{ 303 };
/// @brief Algorithm Id for GMAC-128.
CryptoAlgId constexpr kGMAC128{ 311 };
/// @brief Algorithm Id for GMAC-192.
CryptoAlgId constexpr kGMAC192{ 312 };
/// @brief Algorithm Id for GMAC-256.
CryptoAlgId constexpr kGMAC256{ 313 };
/// @brief Algorithm Id for Hash.
CryptoAlgId constexpr ksha256Hash{ 20 };
/// @brief Algorithm Id for Symmetric block cipher AES-128-CBC.
CryptoAlgId constexpr kAES128CBC{ 400 };
/// @brief Algorithm Id for Symmetric block cipher AES-192-CBC.
CryptoAlgId constexpr kAES192CBC{ 401 };
/// @brief Algorithm Id for Symmetric block cipher AES-256-CBC.
CryptoAlgId constexpr kAES256CBC{ 402 };
/// @brief Algorithm Id for Symmetric block cipher AES-128-ECB.
CryptoAlgId constexpr kAES128ECB{ 410 };
/// @brief Algorithm Id for Symmetric block cipher AES-192-ECB.
CryptoAlgId constexpr kAES192ECB{ 411 };
/// @brief Algorithm Id for Symmetric block cipher AES-256-ECB.
CryptoAlgId constexpr kAES256ECB{ 412 };
/// @brief Algorithm Id for Stream cipher AES-128-CTR.
CryptoAlgId constexpr kAES128CTR{ 420 };
/// @brief Algorithm Id for Stream cipher AES-192-CTR.
CryptoAlgId constexpr kAES192CTR{ 421 };
/// @brief Algorithm Id for Stream cipher AES-256-CTR.
CryptoAlgId constexpr kAES256CTR{ 422 };
/// @brief Algorithm Id for Authentication cipher AES-128-GCM.
CryptoAlgId constexpr kAES128GCM{ 430 };
/// @brief Algorithm Id for Authentication cipher AES-192-GCM.
CryptoAlgId constexpr kAES192GCM{ 431 };
/// @brief Algorithm Id for Authentication cipher AES-256-GCM.
CryptoAlgId constexpr kAES256GCM{ 432 };
/// @brief Algorithm Id for Key wrapping AES-128.
CryptoAlgId constexpr kKeyWrap128{ 500 };
/// @brief Algorithm Id for Key wrapping AES-192.
CryptoAlgId constexpr kKeyWrap192{ 501 };
/// @brief Algorithm Id for Key wrapping AES-256.
CryptoAlgId constexpr kKeyWrap256{ 502 };
/// @brief Algorithm Id for Key wrapping with padding AES-128.
CryptoAlgId constexpr kKeyWrapPad128{ 510 };
/// @brief Algorithm Id for Key wrapping with padding AES-192.
CryptoAlgId constexpr kKeyWrapPad192{ 511 };
/// @brief Algorithm Id for Key wrapping with padding AES-256.
CryptoAlgId constexpr kKeyWrapPad256{ 512 };
/// @brief Algorithm Id for Asymmetric encryption RSA-2048.
CryptoAlgId constexpr kRSA2048{ 600 };
/// @brief Algorithm Id for Asymmetric encryption RSA-2048 with PKCSV1_5 padding.
CryptoAlgId constexpr kRSA2048PKCSV1_5{ 601 };
/// @brief Algorithm Id for Asymmetric encryption RSA-2048 with PKCSV1_5 padding.
CryptoAlgId constexpr kRSASSA2048PKCSV1_5{ 602 };
/// @brief Algorithm Id for Digital Signatures with EC.
CryptoAlgId constexpr kEC{ 700 };
/// @brief Algorithm Id for Digital Signatures with DSA.
CryptoAlgId constexpr kDSA{ 800 };
/// @brief Algorithm Id for Digital Signatures with DH.
CryptoAlgId constexpr kDH{ 900 };
/// @brief Algorithm Id for Digital Signatures with ECDSA.
CryptoAlgId constexpr kECDSA{ 1000 };
/// @brief Algorithm Id for Digital Signatures with EECDSAP256.
CryptoAlgId constexpr kECDSAP256{ 1100 };
/// @brief Algorithm Id for Digital Signatures with ECDH.
CryptoAlgId constexpr kECDH{ 1200 };
} // namespace algorithmids

/// @brief class for crypto algorithm identification.
class AlgorithmIdentification
{
 public:
  /// @brief Method to get the shared pointer of AlgorithmIdentification class.
  static std::shared_ptr<AlgorithmIdentification> GetAlogIdentification()
  {
    if (stAlgoIdentificationInstance_ == nullptr)
    {
      stAlgoIdentificationInstance_ = std::make_shared<AlgorithmIdentification>();
    }
    return stAlgoIdentificationInstance_;
  }

  /// @brief Method to get the algorithm id from algorithm name.
  CryptoAlgId GetAlgoId(ara::core::String algoName) const
  {
    static uint32_t const stringMatched{ 0x00 };
    static uint32_t const kErroralgId{ 65536 };

    ara::core::String algoNameInCaps;
    for (ara::core::Map<ara::core::String, CryptoAlgId>::const_reference& it : AlgoMapping_)
    {
      // According to crypto specifications algorithm name is cas insensitive
      // so if algorithm name is in small case, convert it to capital case.
      for (char c : algoName)
      {
        if (c >= 'a' && c <= 'z')
        {
          c = (c - 'a') + 'A';
        }
        algoNameInCaps.push_back(c);
      }
      if (it.first.compare(algoNameInCaps) == stringMatched)
      {
        return it.second;
      }
      algoNameInCaps.clear();
    }
    return kErroralgId;
  }

  bool IsAlgorithmSupported(CryptoAlgId id) const
  {
    bool isSupported{ false };
    for (ara::core::Map<ara::core::String, CryptoAlgId>::const_reference& it : AlgoMapping_)
    {
      if (it.second == id)
      {
        isSupported = true;
        break;
      }
    }
    return isSupported;
  }

  /// @brief Method to get the algorithm name from algorithm Id.
  ara::core::String GetAlgoName(CryptoAlgId id) const
  {
    ara::core::String returnString;
    for (ara::core::Map<ara::core::String, CryptoAlgId>::const_reference& it : AlgoMapping_)
    {
      if (it.second == id)
      {
        returnString = it.first;
        break;
      }
    }
    return returnString;
  }

  std::size_t GetRequiredHashSize(CryptoAlgId id)
  {
    size_t const kSha256HashSize{ 32 };
    size_t const kSha1HashSize{ 20 };

    if ((id == algorithmids::kHMAC) || (id == algorithmids::ksha256Hash)
      || (id == algorithmids::kRSASSA2048PKCSV1_5) || (id == algorithmids::kECDSA)
      || (id == algorithmids::kECDSAP256))
    {
      return kSha256HashSize;
    }
    else if (id == algorithmids::kRSA2048PKCSV1_5)
    {
      return kSha1HashSize;
    }
    else
    {
      return 0;
    }
  }

  CryptoAlgId GetRequiredHashAlgId(CryptoAlgId id)
  {
    if ((id == algorithmids::kHMAC) || (id == algorithmids::ksha256Hash)
      || (id == algorithmids::kRSASSA2048PKCSV1_5) || (id == algorithmids::kECDSA)
      || (id == algorithmids::kECDSAP256))
    {
      return algorithmids::ksha256Hash;
    }
    else
    {
      return 0;
    }
  }

  std::size_t GetSignatureSize(CryptoAlgId id)
  {
    size_t const kRSASignatureSize{ 256 };
    size_t const kECDSASignSize{ 32 };
    size_t const kAESSignSize{ 16 };

    if ((id == algorithmids::kRSASSA2048PKCSV1_5) || (id == algorithmids::kRSA2048PKCSV1_5))
    {
      return kRSASignatureSize;
    }
    else if ((id == algorithmids::kECDSA) || (id == algorithmids::kECDSAP256) || (id == algorithmids::kHMAC))
    {
      return kECDSASignSize;
    }
    else if ((id == algorithmids::kCMAC128) || (id == algorithmids::kCMAC192)
      || (id == algorithmids::kCMAC256) || (id == algorithmids::kGMAC128) || (id == algorithmids::kGMAC192)
      || (id == algorithmids::kGMAC256))
    {
      return kAESSignSize;
    }
    else
    {
      return 0;
    }
  }

 private:
  /// @brief Mapping of algorithm name and algorithm Id.
  ara::core::Map<ara::core::String, CryptoAlgId> const AlgoMapping_{ { "CTR-AES+DRBG-RNG",
                                                                       algorithmids::kDrbgCtrBasedRng },
    { "HMAC-SHA256+PBKDF2", algorithmids::kPBKDF2 }, { "HMAC-SHA256", algorithmids::kHMAC },
    { "CBC_MAC-AES-128", algorithmids::kCMAC128 }, { "CBC_MAC-AES-192", algorithmids::kCMAC192 },
    { "CBC_MAC-AES-256", algorithmids::kCMAC256 }, { "GCM_MAC-AES-128", algorithmids::kGMAC128 },
    { "GCM_MAC-AES-192", algorithmids::kGMAC192 }, { "GCM_MAC-AES-256", algorithmids::kGMAC256 },
    { "SHA-256", algorithmids::ksha256Hash }, { "CBC-AES-128", algorithmids::kAES128CBC },
    { "CBC-AES-192", algorithmids::kAES192CBC }, { "CBC-AES-256", algorithmids::kAES256CBC },
    { "ECB-AES-128", algorithmids::kAES128ECB }, { "ECB-AES-192", algorithmids::kAES192ECB },
    { "ECB-AES-256", algorithmids::kAES256ECB }, { "CTR-AES-128", algorithmids::kAES128CTR },
    { "CTR-AES-192", algorithmids::kAES192CTR }, { "CTR-AES-256", algorithmids::kAES256CTR },
    { "GCM-AES-128", algorithmids::kAES128GCM }, { "GCM-AES-192", algorithmids::kAES192GCM },
    { "GCM-AES-256", algorithmids::kAES256GCM }, { "KEY-WRAP-AES-128", algorithmids::kKeyWrap128 },
    { "KEY-WRAP-AES-192", algorithmids::kKeyWrap192 }, { "KEY-WRAP-AES-256", algorithmids::kKeyWrap256 },
    { "KEY-WRAP-PAD-AES-128", algorithmids::kKeyWrapPad128 },
    { "KEY-WRAP-PAD-AES-192", algorithmids::kKeyWrapPad192 },
    { "KEY-WRAP-PAD-AES-256", algorithmids::kKeyWrapPad256 },
    { "RSA-SHA1-PKCSV1_5", algorithmids::kRSA2048PKCSV1_5 },
    { "RSA-SSA-SHA256-PKCSV1_5", algorithmids::kRSASSA2048PKCSV1_5 },
    { "ECDSA-SHA256", algorithmids::kECDSA }, { "ECDSA-P-SHA256", algorithmids::kECDSAP256 } };

  static std::shared_ptr<AlgorithmIdentification> stAlgoIdentificationInstance_;
};

} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_COMMON_INC_PUBLIC_ALGORITHM_IDENTIFICATION_H_
