// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : keystorageprovider_skeleton.cpp
//
// Purpose     : Implementation of file keystorageprovider_skeleton.cpp
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#include "keystorageprovider_skeleton.h"
namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace keys
{
namespace keystorageprovider
{
namespace skeleton
{

KeyStorageProviderSkeleton::~KeyStorageProviderSkeleton() noexcept {
  StopOfferService();
}

void KeyStorageProviderSkeleton::OfferService() {
  ara::com::internal::skeleton::TypedServiceImplBase<KeyStorageProviderSkeleton>::OfferService();
}

} // namespace skeleton
} // namespace keystorageprovider
} // namespace keys
} // namespace ipccom
} // namespace crypto
} // namespace ara


