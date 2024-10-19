// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : Algorithm_identification.cpp
//
// Purpose     : This file contains definitions for static variables in AlgorithmIdentification class.
// -----------------------------------------------------------------------------------------------------------

#include "algorithm_identification.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
std::shared_ptr<AlgorithmIdentification> AlgorithmIdentification::stAlgoIdentificationInstance_{ nullptr };
}
} // namespace ara