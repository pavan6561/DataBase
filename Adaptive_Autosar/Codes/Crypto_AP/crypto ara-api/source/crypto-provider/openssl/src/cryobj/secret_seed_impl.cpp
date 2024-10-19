// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : secret_seed_impl.cpp
//
// Purpose     : This file contains implementation for SecretSeedImpl class.
// -----------------------------------------------------------------------------------------------------------

#include "secret_seed_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
/// @brief namespace for minmaxrange.
namespace minmaxrange
{
std::int64_t constexpr kMinUint8Value{ 0x00 };
uint8_t constexpr kMaxUint8Value{ 0xFF };
} // namespace minmaxrange

SecretSeedImpl::SecretSeedImpl(ReadWriteMemRegion seed, Usage usageFlagsVal, ara::core::String primitiveName,
  CryptoPrimitiveId::AlgId primitiId, bool sessionValue, bool exportableValue,
  CryptoObjectUid uuid) noexcept :
  SecretSeed{ seed },
  primitiveNameValue{ primitiveName }, primitiIdValue{ primitiId }, usageFlags{ usageFlagsVal },
  isSessionValue{ sessionValue }, isExportableValue{ exportableValue }
{
  aralog_.LogDebug() << "In SecretSeedImpl constructor";
  objectDependantID.mCOType = CryptoObjectType::kUndefined;
  objectID.mCOType = CryptoObjectType::kSecretSeed;
  objectID.mCouid = uuid;
}

CryptoPrimitiveId::Uptr SecretSeedImpl::GetCryptoPrimitiveId() const noexcept
{
  aralog_.LogDebug() << "In SecretSeedImpl::GetCryptoPrimitiveId()";

  CryptoPrimitiveId::Uptr localPtr{ std::make_unique<CryptoPrimitiveIdImpl>(
    primitiveNameValue, primitiIdValue) };

  return localPtr;
}

std::size_t SecretSeedImpl::GetPayloadSize() const noexcept
{
  aralog_.LogDebug() << "In SecretSeedImpl::GetPayloadSize()";
  aralog_.LogDebug() << "GetPayloadSize() : payload size is " << this->GetSeedValue().size();

  return this->GetSeedValue().size();
}

ara::core::Result<SecretSeed::Uptr> SecretSeedImpl::Clone(ReadOnlyMemRegion xorDelta) const noexcept
{
  aralog_.LogDebug() << "In SecretSeedImpl::Clone()";

  ReadWriteMemRegion localRegion{ this->GetSeedValue() };

  aralog_.LogDebug() << "Clone() : cloning is in process";

  // x-or the given xorDelta value with the current secret seed value to create new seed value.
  for (size_t index{ 0 }; (index < localRegion.size()) && (index < xorDelta.size()); index++)
  {
    localRegion[index] ^= xorDelta[index];
  }

  SecretSeed::Uptr localPtr{ std::make_unique<SecretSeedImpl>(localRegion, usageFlags, primitiveNameValue,
    primitiIdValue, isSessionValue, isExportableValue, objectID.mCouid) };

  aralog_.LogDebug() << "Clone() : cloning successful";

  return ara::core::Result<SecretSeed::Uptr>(std::move(localPtr));
}

ara::core::Result<void> SecretSeedImpl::JumpFrom(const SecretSeed& from, std::int64_t steps) noexcept
{
  aralog_.LogDebug() << "In SecretSeedImpl::JumpFrom()";
  ReadWriteMemRegion const fromValue{ from.GetSeedValue() };

  CryptoPrimitiveId::AlgId const fromSeedId{ from.GetCryptoPrimitiveId()->GetPrimitiveId() };
  CryptoPrimitiveId::AlgId const thisSeedId{ this->GetCryptoPrimitiveId()->GetPrimitiveId() };

  if (fromSeedId != thisSeedId)
  {
    aralog_.LogError() << "Given object is incompatible with secret seed";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kIncompatibleObject);
  }

  if (fromValue.size() < this->GetSeedValue().size())
  {
    aralog_.LogError() << "Given object is incompatible with secret seed";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrc::kInvalidInputSize);
  }

  if (from.GetPayloadSize() > 0)
  {
    // Check if need to jump in forward or reverse direction.
    if (steps > minmaxrange::kMinUint8Value)
    {
      for (std::int64_t stepNo{ minmaxrange::kMinUint8Value }; stepNo < steps; stepNo++)
      {
        bool isValueincremented{ false };
        size_t index{ minmaxrange::kMinUint8Value };

        // Increment the secret seed value bit vise.
        while (!isValueincremented)
        {
          // If first byte of secret seed overflows increment value of next byte
          // and make the first byte as 0. Need to do this for all the bytes in
          // secret seed.
          if ((static_cast<std::uint16_t>(fromValue[index]) + 1) > minmaxrange::kMaxUint8Value)
          {
            fromValue[index] = minmaxrange::kMinUint8Value;
            index++;
          }
          else
          {
            fromValue[index]++;
            isValueincremented = true;
          }
        }
      }
    }
    else if (steps < minmaxrange::kMinUint8Value)
    {
      for (std::int64_t stepNo{ minmaxrange::kMinUint8Value }; stepNo > steps; stepNo--)
      {
        bool isValuedecremented{ false };
        size_t index{ minmaxrange::kMinUint8Value };

        // Decrement the secret seed value bit vise.
        while (!isValuedecremented)
        {
          // If first byte of secret seed reaches the 0, decrement value of next byte
          // and make the first byte as 0xFF. Need to do this for all the bytes in
          // secret seed.
          if (fromValue[index] == minmaxrange::kMinUint8Value)
          {
            fromValue[index] = minmaxrange::kMaxUint8Value;
            index++;
          }
          else
          {
            fromValue[index]--;
            isValuedecremented = true;
          }
        }
      }
    }
    else
    {
      // No Operation is required.
      aralog_.LogError() << "Payload Size is 0";
    }
  }

  //Set newly incremented value to the secret seed object.
  SetSeedValue(fromValue);

  return ara::core::Result<void>::FromValue();
}

SecretSeed& SecretSeedImpl::Next() noexcept
{
  aralog_.LogDebug() << "In SecretSeedImpl::Next()";

  bool isValueincremented{ false };
  size_t index{ minmaxrange::kMinUint8Value };
  ReadWriteMemRegion const valueTobeIncremented{ GetSeedValue() };

  if (GetPayloadSize() > 0)
  {
    // Increment the secret seed value bit vise.
    while (!isValueincremented)
    {
      // If first byte of secret seed overflows increment value of next byte
      // and make the first byte as 0. Need to do this for all the bytes in
      // secret seed.
      if ((static_cast<std::uint16_t>(valueTobeIncremented[index]) + 1) > minmaxrange::kMaxUint8Value)
      {
        valueTobeIncremented[index] = minmaxrange::kMinUint8Value;
        index++;
      }
      else
      {
        valueTobeIncremented[index]++;
        isValueincremented = true;
      }
    }
  }

  // Set newly incremented value to the secret seed object.
  SetSeedValue(valueTobeIncremented);

  return *this;
}

SecretSeed& SecretSeedImpl::Jump(std::int64_t steps) noexcept
{
  aralog_.LogDebug() << "In SecretSeedImpl::Jump()";

  ReadWriteMemRegion const valueToJumpfrom{ GetSeedValue() };

  if (GetPayloadSize() > 0)
  {
    // Check if need to jump in forward or reverse direction.
    if (steps > minmaxrange::kMinUint8Value)
    {
      for (std::int64_t stepNo{ minmaxrange::kMinUint8Value }; stepNo < steps; stepNo++)
      {
        bool isValueincremented{ false };
        size_t index{ minmaxrange::kMinUint8Value };

        // Increment the secret seed value bit vise.
        while (!isValueincremented)
        {
          // If first byte of secret seed overflows increment value of next byte
          // and make the first byte as 0. Need to do this for all the bytes in
          // secret seed.
          if ((static_cast<std::uint16_t>(valueToJumpfrom[index]) + 1) > minmaxrange::kMaxUint8Value)
          {
            valueToJumpfrom[index] = minmaxrange::kMinUint8Value;
            index++;
          }
          else
          {
            valueToJumpfrom[index]++;
            isValueincremented = true;
          }
        }
      }
    }
    else if (steps < minmaxrange::kMinUint8Value)
    {
      for (std::int64_t stepNo{ minmaxrange::kMinUint8Value }; stepNo > steps; stepNo--)
      {
        bool isValuedecremented{ false };
        size_t index{ minmaxrange::kMinUint8Value };

        // Decrement the secret seed value bit vise.
        while (!isValuedecremented)
        {
          // If first byte of secret seed reaches the 0, decrement value of next byte
          // and make the first byte as 0xFF. Need to do this for all the bytes in
          // secret seed.
          if (valueToJumpfrom[index] == minmaxrange::kMinUint8Value)
          {
            valueToJumpfrom[index] = minmaxrange::kMaxUint8Value;
            index++;
          }
          else
          {
            valueToJumpfrom[index]--;
            isValuedecremented = true;
          }
        }
      }
    }
    else
    {
      // No operation is required.
    }
  }
  // Set newly incremented value to the secret seed object.
  SetSeedValue(valueToJumpfrom);

  return *this;
}

SecretSeed& SecretSeedImpl::operator^=(const SecretSeed& source) noexcept
{
  aralog_.LogDebug() << "In SecretSeedImpl::operator^=() with secret seed object";

  ReadWriteMemRegion const valueOfSource{ source.GetSeedValue() };
  ReadWriteMemRegion const valueofDestination{ GetSeedValue() };

  for (size_t index{ 0 }; (index < valueofDestination.size()) && (index < valueOfSource.size()); index++)
  {
    valueofDestination[index] ^= valueOfSource[index];
  }
  SetSeedValue(valueofDestination);
  return *this;
}

SecretSeed& SecretSeedImpl::operator^=(ReadOnlyMemRegion source) noexcept
{
  aralog_.LogDebug() << "In SecretSeedImpl::operator^=()";

  ReadWriteMemRegion const valueofDestination{ GetSeedValue() };

  for (size_t index{ 0 }; (index < valueofDestination.size()) && (index < source.size()); index++)
  {
    valueofDestination[index] ^= source[index];
  }

  SetSeedValue(valueofDestination);

  return *this;
}

} // namespace cryp
} // namespace crypto
} // namespace ara