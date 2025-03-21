// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ara_com_main-cryptoserverexe.cpp
//
// Purpose     : Implementation of file ara_com_main-cryptoserverexe.cpp
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#include <ara/com/internal/ara_com_main.h>
#include <ara/com/internal/vsomeip/vsomeip_binding.h>
#include <ara/com/internal/vsomeip/vsomeip_error.h>
#include <ara/com/internal/vsomeip/vsomeip_error_domains.h>

#include "ara/crypto/ipccom/common/error/error_domain_cryptoerrordomain.h"

namespace ara
{
namespace com
{
namespace internal
{
namespace runtime
{

void Initialize() {
  vsomeip::runtime::Register();
  ara::com::internal::vsomeip::common::error_domains::Registry::registerDomain(ara::crypto::ipccom::common::error::GetCryptoErrorDomainErrorDomain());
}

} // namespace runtime
} // namespace internal
} // namespace com
} // namespace ara


