// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : keystorage.h
//
// Purpose     : This file provides KeyStorage class declaration
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_KEY_STORAGE_INC_PUBLIC_KEYSTORAGE_H_
#define CRYPTO_SOURCE_KEY_STORAGE_INC_PUBLIC_KEYSTORAGE_H_

// NOTE:- This file is out of scope for MS5-IR1 plese do not consider this file for review or static analysis.

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for keystorage.
namespace keystorage
{
/// @brief Class for key storage.
class KeyStorage //  a class definition
{
 public:
  /// @brief Constructor for KeyStorage class.
  KeyStorage();

  /// @brief Destructor for KeyStorage class.
  ~KeyStorage();
};

} // namespace keystorage
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_KEY_STORAGE_INC_PUBLIC_KEYSTORAGE_H_
