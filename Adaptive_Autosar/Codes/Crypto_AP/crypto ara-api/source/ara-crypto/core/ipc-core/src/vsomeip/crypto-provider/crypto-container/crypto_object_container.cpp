// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_Object_container.cpp
//
// Purpose     : Crypto object container implementation to initialize static container variables of crypto
//               object skeleton implementations.
// -----------------------------------------------------------------------------------------------------------

#include "private_key_container.h"
#include "public_key_container.h"
#include "restricted_used_object_container.h"
#include "secret_seed_container.h"
#include "signature_container.h"
#include "symmetrickey_container.h"

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
std::shared_ptr<PublicKeyContainer> PublicKeyContainer::publicKeyPtr_{nullptr};
std::shared_ptr<PrivateKeyContainer> PrivateKeyContainer::privateKeyPtr_{nullptr};
std::shared_ptr<RestrictedUsedObjectContainer> RestrictedUsedObjectContainer::restrictedUseObjPtr_{nullptr};
std::shared_ptr<SecretSeedContainer> SecretSeedContainer::secretSeedPtr_{nullptr};
std::shared_ptr<SignatureContainer> SignatureContainer::signatureContainerPtr_{nullptr};
std::shared_ptr<SymmetricKeyContainer> SymmetricKeyContainer::symmetricKeyPtr_{nullptr};
} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara