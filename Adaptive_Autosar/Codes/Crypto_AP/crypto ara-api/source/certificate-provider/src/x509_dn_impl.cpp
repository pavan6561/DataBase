// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_dn_impl.cpp
//
// Purpose     : This file provides x509_dn_impl class definitions.
// -----------------------------------------------------------------------------------------------------------

#include "x509_dn_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
namespace crypto
{
namespace x509
{
static ara::core::Map<X509DN::AttributeId, ara::core::String> const attrStrMap{
    {X509DN::AttributeId::kCommonName, "CN"}, {X509DN::AttributeId::kCountry, "C"},
    {X509DN::AttributeId::kState, "ST"}, {X509DN::AttributeId::kLocality, "L"},
    {X509DN::AttributeId::kOrganization, "O"}, {X509DN::AttributeId::kOrgUnit, "OU"},
    {X509DN::AttributeId::kStreet, "street"}, {X509DN::AttributeId::kPostalCode, "postalCode"},
    {X509DN::AttributeId::kTitle, "title"}, {X509DN::AttributeId::kSurname, "SN"},
    {X509DN::AttributeId::kGivenName, "GN"}, {X509DN::AttributeId::kInitials, "initials"},
    {X509DN::AttributeId::kPseudonym, "pseudonym"},
    {X509DN::AttributeId::kGenerationQualifier, "generationQualifier"},
    {X509DN::AttributeId::kDomainComponent, "DC"}, {X509DN::AttributeId::kDnQualifier, "dnQualifier"},
    {X509DN::AttributeId::kEmail, "emailAddress"}, {X509DN::AttributeId::kUri, "url?"},
    {X509DN::AttributeId::kDns, "dnsName"}, {X509DN::AttributeId::kHostName, "unstructuredName"},
    {X509DN::AttributeId::kIpAddress, "unstructuredAddress"},
    {X509DN::AttributeId::kSerialNumbers, "serialNumber"}, {X509DN::AttributeId::kUserId, "UID"}};

X509DNImpl::X509DNImpl(X509_NAME* dn) noexcept : X509DN{}, domainName_{dn}
{
  ara::core::String dnString{" "};
  if (domainName_ != nullptr)
  {
    char* const domainNameString{X509_NAME_oneline(domainName_, nullptr, 0)};

    if (domainNameString != nullptr)
    {
      dnString.clear();

      for (size_t index{0}; index < strlen(domainNameString); index++)
      {
        dnString.push_back(domainNameString[index]);
      }

      aralog_.LogDebug() << "Domain name is " << dnString;

      distName_ = dnString;

      PrseOpensSSlDn(distName_);
    }
    else
    {
      aralog_.LogWarn() << "Dn String not found!!!!";
      distName_ = dnString;
    }

    OPENSSL_free(domainNameString);
  }
  else
  {
    aralog_.LogWarn() << "Dn String not found!!!!";
    distName_ = dnString;
  }
}

void X509DNImpl::PrseOpensSSlDn(ara::core::StringView dn) noexcept
{
  aralog_.LogDebug() << "In X509DNImpl::PrseOpensSSlDn()";
  int64_t slashLocation{0};
  int64_t equaltoLocation;
  int64_t plusLocation;
  ara::core::String multiattrValue;
  bool isMultiAttr{false};
  multiattrValue.clear();
  while (static_cast<size_t>(slashLocation) <= dn.length())
  {
    if (slashLocation == 0)
    {
      slashLocation = dn.find('/', slashLocation);
    }

    plusLocation = dn.find('+', slashLocation);

    if (plusLocation < dn.length())
    {
      aralog_.LogDebug() << "This is a multi attribute value";
      isMultiAttr = true;
    }
    equaltoLocation = dn.find('=', slashLocation);

    slashLocation++;
    ara::core::String field{dn.data() + slashLocation, static_cast<size_t>(equaltoLocation - slashLocation)};

    if (isMultiAttr == false)
    {
      slashLocation = dn.find('/', slashLocation);
    }

    ara::core::String value;
    if (slashLocation <= dn.length())
    {
      ara::core::String tempValue{
          dn.data() + equaltoLocation + 1, static_cast<size_t>(slashLocation - (equaltoLocation + 1))};
      if ((isMultiAttr == true) || (multiattrValue.length() > 0))
      {
        multiattrValue += tempValue;
        multiattrValue += "+";
        aralog_.LogDebug() << multiattrValue;
      }
      else
      {
        value = tempValue;
      }
    }
    else
    {
      ara::core::String tempValue{
          dn.data() + equaltoLocation + 1, dn.length() - static_cast<size_t>(equaltoLocation + 1)};
      if ((isMultiAttr == true) || (multiattrValue.length() > 0))
      {
        multiattrValue += "+";
        multiattrValue += tempValue;
        aralog_.LogDebug() << multiattrValue;
      }
      else
      {
        value = tempValue;
      }
    }
    if (isMultiAttr == false)
    {
      if (multiattrValue.length() > 0)
      {
        attrMap_.emplace(field, multiattrValue);
      }
      else
      {
        attrMap_.emplace(field, value);
      }
    }
    else
    {
      slashLocation = plusLocation + 1;
    }
    isMultiAttr = false;
  }
}

ara::core::Result<ara::core::StringView> X509DNImpl::GetDnString() const noexcept
{
  aralog_.LogDebug() << "In X509DNImpl::GetDnString()";

  return ara::core::Result<ara::core::StringView>::FromValue(distName_);
}

ara::core::Result<void> X509DNImpl::SetDn(ara::core::StringView dn) noexcept
{
  aralog_.LogDebug() << "In X509DNImpl::SetDn()";

  if (domainName_ == nullptr)
  {
    domainName_ = X509_NAME_new();
  }

  PrseOpensSSlDn(dn);

  for (ara::core::Map<ara::core::String, ara::core::String>::const_reference& it : attrMap_)
  {
    X509_NAME_add_entry_by_txt(domainName_, it.first.c_str(), MBSTRING_ASC,
        reinterpret_cast<const uint8_t*>(it.second.data()), -1, -1, 0);
  }

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::StringView> X509DNImpl::GetAttribute(AttributeId id) const noexcept
{
  aralog_.LogError() << "In X509DNImpl::GetAttribute()";

  ara::core::Map<X509DN::AttributeId, ara::core::String>::const_iterator const atrIt{attrStrMap.find(id)};

  if (atrIt == attrStrMap.end())
  {
    aralog_.LogError() << "Invalid argument received";
    return ara::core::Result<ara::core::StringView>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  ara::core::Map<ara::core::String, ara::core::String>::const_iterator const dnIt{
      attrMap_.find(atrIt->second)};

  if (dnIt == attrMap_.end())
  {
    aralog_.LogError() << "Given attribute is not present in the give Distinguished Name";
    return ara::core::Result<ara::core::StringView>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  return ara::core::Result<ara::core::StringView>::FromValue(dnIt->second);
}

ara::core::Result<void> X509DNImpl::SetAttribute(AttributeId id, ara::core::StringView attribute) noexcept
{
  aralog_.LogError() << "In X509DNImpl::SetAttribute()";
  ara::core::Map<X509DN::AttributeId, ara::core::String>::const_iterator const atrIt{attrStrMap.find(id)};

  if (atrIt == attrStrMap.end())
  {
    aralog_.LogError() << "Invalid argument received";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  ara::core::String dn{attribute};

  attrMap_.emplace(atrIt->second, dn);

  X509_NAME_add_entry_by_txt(domainName_, atrIt->second.c_str(), MBSTRING_ASC,
      reinterpret_cast<const uint8_t*>(dn.data()), -1, -1, 0);

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::StringView> X509DNImpl::GetAttribute(AttributeId id, unsigned index) const
    noexcept
{
  aralog_.LogError() << "In X509DNImpl::GetAttribute()";

  ara::core::Map<X509DN::AttributeId, ara::core::String>::const_iterator const atrIt{attrStrMap.find(id)};

  if (atrIt == attrStrMap.end())
  {
    aralog_.LogError() << "Unknown Identifier is received";
    return ara::core::Result<ara::core::StringView>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if ((id != X509DN::AttributeId::kOrgUnit) && (id != X509DN::AttributeId::kDomainComponent) && (index > 0))
  {
    aralog_.LogError() << "Invalid argument received";
    return ara::core::Result<ara::core::StringView>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  ara::core::Map<ara::core::String, ara::core::String>::const_iterator const dnIt{
      attrMap_.find(atrIt->second)};

  if (dnIt == attrMap_.end())
  {
    aralog_.LogError() << "Given attribute is not present in the give Distinguished Name";
    return ara::core::Result<ara::core::StringView>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  ara::core::String attribute{dnIt->second};

  ara::core::Vector<int64_t> indexPositions;

  int64_t plusPosition{0};

  uint8_t noOfMutAttr{0};

  while (plusPosition <= attribute.length())
  {
    plusPosition = attribute.find('+', plusPosition);
    indexPositions.push_back(plusPosition);
    plusPosition++;
    noOfMutAttr++;
  }

  if (index >= noOfMutAttr)
  {
    aralog_.LogError()
        << "The index value is greater than or equal to the actual number of components in the specified "
        << " attribute";
    return ara::core::Result<ara::core::StringView>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kAboveBoundary);
  }

  if (noOfMutAttr == 1)
  {
    return ara::core::Result<ara::core::StringView>::FromValue(attribute);
  }
  else if (index == static_cast<unsigned>(noOfMutAttr - 1))
  {
    ara::core::String attrCompnt{attribute.data() + indexPositions[index] + 1,
        static_cast<size_t>(attribute.length() - (indexPositions[index] + 1))};
    return ara::core::Result<ara::core::StringView>::FromValue(attrCompnt);
  }
  else
  {
    ara::core::String attrCompnt{attribute.data() + indexPositions[index] + 1,
        static_cast<size_t>(indexPositions[index + 1] - (indexPositions[index] + 1))};
    return ara::core::Result<ara::core::StringView>::FromValue(attrCompnt);
  }
}

ara::core::Result<void> X509DNImpl::SetAttribute(
    AttributeId id, unsigned index, ara::core::StringView attribute) noexcept
{
  aralog_.LogDebug() << "In X509DNImpl::SetAttribute()";

  ara::core::Map<X509DN::AttributeId, ara::core::String>::const_iterator const atrIt{attrStrMap.find(id)};

  if (atrIt == attrStrMap.end())
  {
    aralog_.LogError() << "Unknown Identifier is received";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if ((id != X509DN::AttributeId::kOrgUnit) && (id != X509DN::AttributeId::kDomainComponent) && (index > 0))
  {
    aralog_.LogError() << "Invalid argument received";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  ara::core::Map<ara::core::String, ara::core::String>::const_iterator const dnIt{
      attrMap_.find(atrIt->second)};

  if (dnIt == attrMap_.end())
  {
    aralog_.LogError() << "Given attribute is not present in the give Distinguished Name";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  ara::core::Vector<int64_t> indexPositions;

  int64_t plusPosition{0};

  uint8_t noOfMutAttr{0};

  while (plusPosition <= dnIt->second.length())
  {
    plusPosition = dnIt->second.find('+', plusPosition);
    indexPositions.push_back(plusPosition);
    plusPosition++;
    noOfMutAttr++;
  }

  if (index >= noOfMutAttr)
  {
    aralog_.LogError()
        << "The index value is greater than or equal to the actual number of components in the specified "
        << " attribute";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kAboveBoundary);
  }

  ara::core::String attr{attribute};
  ara::core::Vector<ara::core::String> multiAttr;

  multiAttr.resize(noOfMutAttr);

  for (size_t loopCount{0}; loopCount < indexPositions.size(); loopCount++)
  {
    if (loopCount == index)
    {
      if (index > 0)
      {
        multiAttr[loopCount] += "+";
      }
      multiAttr[loopCount] = attr;
    }
    else
    {
      multiAttr[loopCount] = dnIt->second.substr(
          indexPositions[loopCount], indexPositions[loopCount + 1] - indexPositions[loopCount]);
    }

    X509_NAME_add_entry_by_txt(domainName_, atrIt->second.c_str(), MBSTRING_ASC,
        reinterpret_cast<const uint8_t*>(multiAttr[loopCount].data()), -1, -1, 0);
  }

  for (ara::core::Map<ara::core::String, ara::core::String>::reference ref : attrMap_)
  {
    if (ref.first == atrIt->second)
    {
      ref.second.clear();
      for (ara::core::String& attrVal : multiAttr)
      {
        ref.second += attrVal;
      }
    }
  }

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> X509DNImpl::ExportPublicly(
    Serializable::FormatId formatId) const noexcept
{
  aralog_.LogDebug() << "In X509DNImpl::ExportPublicly()";

  if ((formatId == Serializable::kFormatDerEncoded) || (formatId == Serializable::kFormatDefault))
  {
    uint8_t* domainNamePtr{nullptr};

    int32_t len{i2d_X509_NAME(domainName_, &domainNamePtr)};

    if (len < 0)
    {
      aralog_.LogError() << "Error in DER encoding";
      len = 0;
    }

    ara::core::Vector<uint8_t> tempResult{domainNamePtr, domainNamePtr + len};
    ara::core::Vector<ara::core::Byte> const result{tempResult.begin(), tempResult.end()};

    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(result);
  }
  else if (formatId == Serializable::kFormatRawValueOnly)
  {
    char* domainNameString{X509_NAME_oneline(domainName_, nullptr, 0)};

    ara::core::String dnString{domainNameString};

    aralog_.LogDebug() << "Domain name is " << dnString.c_str();

    OPENSSL_free(domainNameString);

    ara::core::Vector<ara::core::Byte> const result{dnString.data(), dnString.data() + dnString.size()};

    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(result);
  }
  else
  {
    // According to SWS_CRYPT_10711 if the formatId argument has unknown value
    // then ExportPublicly() should return SecurityErrorDomain::Errc::kUnknownIdentifier.
    aralog_.LogError() << "Invalid format Id";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }
}

} // namespace x509
} // namespace crypto
} // namespace ara
