// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_service_container.cpp
//
// Purpose     : Crypto object container implementation to initialize static container variables of crypto
//               object skeleton implementations.
// -----------------------------------------------------------------------------------------------------------

#include "block_service_container.h"
#include "digest_service_container.h"
#include "crypto_service_container.h"
#include "signature_service_container.h"
#include "extension_service_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for common.
namespace common
{
/// @brief namespace for cryobj
namespace datastorage
{
std::shared_ptr<BlockServiceContainer> BlockServiceContainer::blockServiceContainerPtr_{nullptr};
std::shared_ptr<DigestServiceContainer> DigestServiceContainer::digestServiceContainerPtr_{nullptr};
std::shared_ptr<CryptoServiceContainer> CryptoServiceContainer::cryptoServiceContainerPtr_{nullptr};
std::shared_ptr<SignatureServiceContainer> SignatureServiceContainer::signatureServiceContainerPtr_{nullptr};
std::shared_ptr<ExtensionServiceContainer> ExtensionServiceContainer::extensionServiceContainerPtr_{nullptr};
} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara
