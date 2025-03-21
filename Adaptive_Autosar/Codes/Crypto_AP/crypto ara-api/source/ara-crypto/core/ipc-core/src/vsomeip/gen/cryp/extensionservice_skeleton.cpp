// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : extensionservice_skeleton.cpp
//
// Purpose     : Implementation of file extensionservice_skeleton.cpp
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-03-08T09:56:00+05:30
// -----------------------------------------------------------------------------------------------------------

#include "extensionservice_skeleton.h"
namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace cryp
{
namespace cryservice
{
namespace extensionservice
{
namespace skeleton
{

ExtensionServiceSkeleton::~ExtensionServiceSkeleton() noexcept {
  StopOfferService();
}

void ExtensionServiceSkeleton::OfferService() {
  ara::com::internal::skeleton::TypedServiceImplBase<ExtensionServiceSkeleton>::OfferService();
}

} // namespace skeleton
} // namespace extensionservice
} // namespace cryservice
} // namespace cryp
} // namespace ipccom
} // namespace crypto
} // namespace ara


