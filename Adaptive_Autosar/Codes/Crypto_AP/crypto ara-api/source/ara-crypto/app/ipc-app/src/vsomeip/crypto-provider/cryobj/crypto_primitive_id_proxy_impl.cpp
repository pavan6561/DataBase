// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_primitive_id_proxy_impl.cpp
//
// Purpose     : Crypto primitive id proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "crypto_primitive_id_proxy_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
CryptoPrimitiveIdProxyImpl::CryptoPrimitiveIdProxyImpl(
    const ara::core::StringView primitiveName, const AlgId algorithmId) noexcept :
    primitiveName_{primitiveName.data()},
    algorithmId_{algorithmId}, logger_{ara::log::CreateLogger(
                                   "CPID", "Crypto PrimitiveId Proxy Impl", ara::log::LogLevel::kVerbose)}
{
}

const ara::core::StringView CryptoPrimitiveIdProxyImpl::GetPrimitiveName() const noexcept
{
  logger_.LogDebug() << "IPC-APP::GetPrimitiveName -> " << primitiveName_;
  return primitiveName_;
}

CryptoPrimitiveId::AlgId CryptoPrimitiveIdProxyImpl::GetPrimitiveId() const noexcept
{
  logger_.LogDebug() << "IPC-APP::GetPrimitiveId -> " << algorithmId_;
  return algorithmId_;
}

} // namespace cryp
} // namespace crypto
} // namespace ara
