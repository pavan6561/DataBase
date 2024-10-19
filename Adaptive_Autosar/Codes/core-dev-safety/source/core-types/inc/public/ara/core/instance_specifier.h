// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : instance_specifier.h
//
// Purpose     : This file provides the implementation of class instance specifier.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INSTANCE_SPECIFIER_H
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INSTANCE_SPECIFIER_H

#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/core/string.h"
#include "ara/core/core_error_domain.h"

#include <algorithm>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Class representing an AUTOSAR Instance Specifier,
/// which is basically an AUTOSAR shortname-path wrapper.
/// @trace SWS_CORE_10200
/// @archtrace 609891
/// @design 611449 614039
class InstanceSpecifier final
{
  /// @brief Variable to store the String.
  ara::core::String mMetaModelIdentifier_;
  /// @brief Valid InstanceSpecifier representations.
  /// @param[in] metaModelIdentifier Arguments which are used by the constructor of newly.
  /// @returns meta-model paths that are syntactically invalid.
  /// @trace SWS_CORE_10200 SWS_CORE_10201
  /// @archtrace 609891
  /// @design 611449
  static Result<void> Validate(StringView metaModelIdentifier) noexcept
  {
    size_t endOffset = -1; // point this to a "virtual leading '/'"
    do
    {
      size_t const currentOffset{endOffset + 1}; // skip the '/'
      endOffset = metaModelIdentifier.find('/', currentOffset);
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
      size_t const len{(endOffset == StringView::stkNpos) ? StringView::stkNpos : endOffset - currentOffset};
      StringView const pathComponent{metaModelIdentifier.substr(currentOffset, len)};

      if (pathComponent.empty())
      {
        return Result<void>::FromError(CoreErrc::kInvalidMetaModelPath);
      }
      if (!IsValidMetamodelIdentifierPart(pathComponent))
      {
        return Result<void>::FromError(CoreErrc::kInvalidMetaModelShortname);
      }
    } while (endOffset != StringView::stkNpos);

    return Result<void>();
  }
  /// @brief Definition to check character is valid or not.
  /// @param[in] ch First character.
  /// @returns true if the character is valid.
  /// @design 611449
  constexpr static bool IsValidStartChar(char ch) noexcept
  {
    return (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
  /// @brief Definition to check character is valid or not.
  /// @param[in] ch First character.
  /// @returns true if the character is valid.
  /// @design 611449
  constexpr static bool IsValidChar(char ch) noexcept
  {
    return (IsValidStartChar(ch)) || ((ch >= '0') && (ch <= '9')) || (ch == '_');
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
  /// @brief Definition to check meta model Identifier.
  /// @param[in] text Instance of StringView.
  /// @returns true if the meta model Identifier is valid.
  /// @design 611449
  constexpr static bool IsValidMetamodelIdentifierPart(StringView text) noexcept
  {
    if (text.empty())
    {
      return false;
    }
    return (IsValidStartChar(text[0])) && (std::all_of(text.begin() + 1, text.end(), IsValidChar));
  }
  /// @brief Constructor of InstanceSpecifier.
  /// @param[in] metaModelIdentifier Stringified form of InstanceSpecifier.
  /// @param[in] tag Tag of a InstanceSpecifier.
  /// @design 611449
  InstanceSpecifier(StringView metaModelIdentifier, bool tag) : mMetaModelIdentifier_(metaModelIdentifier)
  {
    static_cast<void>(tag);
  }

 public:
  /// @brief Create a new instance of InstanceSpecifier class.
  /// @param[in] metaModelIdentifier Stringified form of InstanceSpecifier.
  /// @returns a Result, containing either a syntactically valid
  /// InstanceSpecifier, or an ErrorCode.
  /// @trace SWS_CORE_10202
  /// @archtrace 609891
  /// @design 611450
  static Result<InstanceSpecifier> Create(StringView metaModelIdentifier)
  {
    Result<void> const ret{Validate(metaModelIdentifier)};
    if (!ret)
    {
      return Result<InstanceSpecifier>::FromError(ret.Error());
    }
    return InstanceSpecifier(metaModelIdentifier, true);
  }

  /// @brief Throwing ctor from meta-model string
  /// @param[in] metaModelIdentifier Stringified meta model identifier
  /// where path separator is '/'. Lifetime of underlying
  /// string has to exceed the lifetime of the constructed InstanceSpecifier.
  /// @throws InstanceSpecifierException In case the given metamodel_identifier
  /// isn't a valid meta-model identifier/short name path.
  /// @trace SWS_CORE_10202
  /// @archtrace 609891
  /// @design 611451
  explicit InstanceSpecifier(StringView metaModelIdentifier) :
      InstanceSpecifier(Create(metaModelIdentifier).ValueOrThrow())
  {
  }

  /// @brief The Copy constructor.
  /// @param[in] other The other instance.
  /// @trace SWS_CORE_10200
  /// @archtrace 609891
  InstanceSpecifier(InstanceSpecifier const& other) = default;

  /// @brief The Move constructor.
  /// @param[in] other The other instance.
  /// @trace SWS_CORE_10200
  /// @archtrace 609891
  InstanceSpecifier(InstanceSpecifier&& other) noexcept = default;

  /// @brief Copy assignment operator.
  /// @param[in] other The other instance.
  /// @returns this
  /// @trace SWS_CORE_10200
  /// @archtrace 609891
  InstanceSpecifier& operator=(InstanceSpecifier const& other) = default;

  /// @brief Move assignment operator.
  /// @param[in] other The other instance.
  /// @returns this
  /// @trace SWS_CORE_10200
  /// @archtrace 609891
  InstanceSpecifier& operator=(InstanceSpecifier&& other) noexcept = default;

  /// @brief The Destructor for class.
  /// @trace SWS_CORE_10200
  /// @archtrace 609891
  ~InstanceSpecifier() = default;

  /// @brief Method to return the stringified form of InstanceSpecifier.
  /// @returns stringified form of InstanceSpecifier.
  /// Lifetime of the underlying string is only guaranteed for the lifetime
  /// of the underlying string of the StringView passed to the constructor.
  /// @trace SWS_CORE_10201
  /// @archtrace 609891
  /// @design 611452
  StringView ToString() const noexcept
  {
    return mMetaModelIdentifier_;
  }

  /// @brief Equal operator to compare with other InstanceSpecifier instance.
  /// @param[in] other InstanceSpecifier instance to compare this one with.
  /// @returns true in case both InstanceSpecifiers are denoting exactly
  /// the same model element, false else.
  /// @trace SWS_CORE_10201
  /// @archtrace 609891
  /// @design 611453
  bool operator==(InstanceSpecifier const& other) const noexcept
  {
    return mMetaModelIdentifier_ == other.ToString();
  }

  /// @brief Equal operator to compare with other InstanceSpecifier instance.
  /// @param[in] other String representation to compare this one with.
  /// @returns true in case this InstanceSpecifiers is denoting exactly
  /// the same model element as other, false else.
  /// @trace SWS_CORE_10201
  /// @archtrace 609891
  /// @design 611454
  bool operator==(StringView other) const noexcept
  {
    return mMetaModelIdentifier_ == other;
  }

  /// @brief Unequal operator to compare with other InstanceSpecifier instance.
  /// @param[in] other InstanceSpecifier instance to compare this one with.
  /// @returns false in case both InstanceSpecifiers are denoting exactly
  /// the same model element, true else.
  /// @trace SWS_CORE_10201
  /// @archtrace 609891
  /// @design 611455
  bool operator!=(InstanceSpecifier const& other) const noexcept
  {
    return mMetaModelIdentifier_ != other.ToString();
  }

  /// @brief Unequal operator to compare with other InstanceSpecifier string representation.
  /// @param[in] other String representation to compare this one with.
  /// @returns false in case this InstanceSpecifiers is denoting exactly
  /// the same model element as other, true else.
  /// @trace SWS_CORE_10201
  /// @archtrace 609891
  /// @design 611456
  bool operator!=(StringView other) const noexcept
  {
    return mMetaModelIdentifier_ != other;
  }

  /// @brief Lower than operator to compare with other InstanceSpecifier
  /// for ordering purposes (f.i. when collecting identifiers in maps).
  /// @param[in] other InstanceSpecifier instance to compare this one with.
  /// @returns true in case this InstanceSpecifiers is lexically lower than
  /// other, false else.
  /// @trace SWS_CORE_10201
  /// @archtrace 609891
  /// @design 611457
  bool operator<(InstanceSpecifier const& other) const noexcept
  {
    return mMetaModelIdentifier_ < other.mMetaModelIdentifier_;
  }
};

/// @brief Non-member function op to allow char on lhs.
/// @param[in] lhs Stringified form of a InstanceSpecifier.
/// @param[in] rhs An Instance Specifier.
/// @returns true in case rhs string representation equals lhs.
/// @trace SWS_CORE_10201
/// @archtrace 609891
/// @design 611845
inline bool operator==(StringView lhs, InstanceSpecifier const& rhs) noexcept
{
  return lhs == rhs.ToString();
}

/// @brief Non-member function op to allow char on lhs.
/// @param[in] lhs Stringified form of a InstanceSpecifier.
/// @param[in] rhs An Instance Specifier.
/// @returns true in case rhs string representation not equals lhs.
/// @trace SWS_CORE_10201
/// @archtrace 609891
/// @design 611846
inline bool operator!=(StringView lhs, InstanceSpecifier const& rhs) noexcept
{
  return lhs != rhs.ToString();
}

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_INSTANCE_SPECIFIER_H
// -----------------------------------------------------------------------------------------------------------
// - A5.1.1-2
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A5.1.1 required
//   Description:         Use of magic string literal.
//   Justification:       This will create another violation for initialization with magic string literal.This
//                        has been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
