// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certmanager.h
//
// Purpose     : This file provides CertManager class declaration
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CERTMANAGER_H_
#define CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CERTMANAGER_H_

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
namespace cryto
{
namespace certi
{
/// @brief class for Certificate manager.
class CertManager //  a class definition
{
 public:
  /// @brief Constructor for CertManager.
  CertManager();

  /// @brief Distructor for CertManager.
  ~CertManager();
};
} // namespace certi
} // namespace cryto
} // namespace ara
#endif // CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_CERTMANAGER_H_
