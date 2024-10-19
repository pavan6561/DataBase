// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_data_container.cpp
//
// Purpose     : X509 data container implementation to initialize static container variables of x509
//               functionality contexts.
// -----------------------------------------------------------------------------------------------------------

#include "x509dn_container.h"
#include "x509publickeyinfo_container.h"

namespace ara
{
namespace crypto
{
namespace common
{
namespace datastorage
{
// Initializing static variables for all x509 data storage singleton container classes

// Initializing static variable of x509 container class
std::shared_ptr<X509DnContainer> X509DnContainer::x509DnPtr_{nullptr};

std::shared_ptr<X509PublicKeyInfoContainer> X509PublicKeyInfoContainer::x509PkiPtr_{nullptr};

} // namespace datastorage
} // namespace common
} // namespace crypto
} // namespace ara