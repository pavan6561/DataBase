// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_context_container.cpp
//
// Purpose     : Crypto context container implementation to initialize static container variables of crypto
//               provider functionality contexts.
// -----------------------------------------------------------------------------------------------------------

#include "hash_function_container.h"
#include "key_derivation_function_container.h"
#include "signer_private_container.h"

namespace ara
{
namespace crypto
{
namespace common
{
namespace crypdatastore
{
// Initializing static variables for all crypto provider singleton container class

// Initializing static variable of HashFunctionContainer class
std::shared_ptr<HashFunctionContainer> HashFunctionContainer::hashPtr_{nullptr};

// Initializing static variable of KeyDerivationFunctionContainer class
std::shared_ptr<KeyDerivationFunctionContainer> KeyDerivationFunctionContainer::kdfPtr_{nullptr};

// Initializing static variable of SignerPrivateContainer class
std::shared_ptr<SignerPrivateContainer> SignerPrivateContainer::signerPrivatePtr_{nullptr};

} // namespace crypdatastore
} // namespace common
} // namespace crypto
} // namespace ara