// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : volatile_trusted_container_impl.cpp
//
// Purpose     : This file contains implementation for VolatileTrustedContainerImpl class.
// -----------------------------------------------------------------------------------------------------------

#include "volatile_trusted_container_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
VolatileTrustedContainerImpl::VolatileTrustedContainerImpl(size_t capacity) noexcept :
  containerCapacity_{ capacity }
{
}

IOInterface& VolatileTrustedContainerImpl::GetIOInterface() const noexcept
{
  static IOInterfaceVolatile volatileInterface{ containerCapacity_, true, true, true, true };
  return volatileInterface;
}

} // namespace crypto
} // namespace ara
