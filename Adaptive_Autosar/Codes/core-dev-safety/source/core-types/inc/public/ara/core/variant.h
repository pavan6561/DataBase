// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0
#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_VARIANT_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_VARIANT_H_

#include "iceoryx_hoofs/cxx/algorithm.hpp"
#include "iceoryx_hoofs/internal/cxx/variant_internal.hpp"

#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>

#include "iceoryx_hoofs/platform/platform_correction.hpp"

namespace ara
{
namespace core
{
/// @brief The helper struct to perform an emplacement at a predefined index
/// in the constructor of a Variant.
/// @tparam N Index where to perform the placement new.
/// @trace SWS_CORE_S7.2.4.3_P1_004
/// @archtrace 609898
template <uint64_t N>
struct InPlaceIndex
{
  static constexpr uint64_t stkValue{N};
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief The helper struct to perform an emplacement of a predefined type
/// in the constructor of a Variant.
/// @tparam T The type which should be created.
/// @trace SWS_CORE_S7.2.4.3_P1_004
/// @archtrace 609898
template <typename T>
struct InPlaceType
{
  /// @brief T alias of type.
  using type = T;
};

/// @brief Value which an invalid Variant index occupies.
/// @trace SWS_CORE_S7.2.4.3_P1_004
/// @archtrace 609898
static constexpr uint64_t INVALID_VARIANT_INDEX{std::numeric_limits<uint64_t>::max()};

template <typename... Types>
/// @brief Variant implementation from the C++17 standard with C++11.
/// The interface is inspired by the C++17 standard but it has changes
/// in get and emplace since we are not allowed to throw exceptions.
/// @tparam Types The variadic list of types which the Variant should be able to store.
/// @trace SWS_CORE_S7.2.4.3_P1_004
/// @archtrace 609898
/// @design 611318 614039
class Variant
{
 private:
  /// @brief Contains the size of the largest element.
  static constexpr uint64_t stkTYPESIZE_{iox::algorithm::max(sizeof(Types)...)};

 public:
  /// @brief The default constructor constructs a Variant which does not contain
  /// an element and returns INVALID_VARIANT_INDEX when index() is called.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  constexpr Variant() noexcept = default;

  /// @brief Creates a Variant and perform an in place construction of the type
  /// stored at index N. If the index N is out of bounds you get a compiler error.
  /// @tparam N Index where to perform the placement new.
  /// @tparam CTorArguments The variadic types of the c'tor arguments.
  /// @param[in] index The index of the type which should be constructed.
  /// @param[in] args The variadic list of arguments which will be forwarded to the
  /// constructor to the type at index.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611319
  template <uint64_t N, typename... CTorArguments>
  constexpr Variant(const InPlaceIndex<N>& index, CTorArguments&&... args) noexcept;
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Creates a Variant and perform an in place construction of the type T.
  /// If T is not part of the Variant you get a compiler error.
  /// @tparam T The type which should be created inside the Variant.
  /// @tparam CTorArguments The variadic types of the c'tor arguments.
  /// @param[in] type The type which should be created inside the Variant.
  /// @param[in] args The variadic list of arguments which will be forwarded
  /// to the constructor to the type.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611320
  template <typename T, typename... CTorArguments>
  constexpr Variant(const InPlaceType<T>& type, CTorArguments&&... args) noexcept;

  /// @brief Creates a Variant from a user supplied value.
  /// @tparam T Type of the value to be stored in the Variant.
  /// @param[in] arg The arg to be forwared to the c'tor of T.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611321
  template <typename T, typename = std::enable_if_t<!std::is_same<std::decay_t<T>, Variant>::value>,
      typename std::enable_if_t<!iox::cxx::internal::is_in_place_index<std::decay_t<T>>::value, bool> = false,
      typename std::enable_if_t<!iox::cxx::internal::is_in_place_type<std::decay_t<T>>::value, bool> = false>
  constexpr Variant(T&& arg) noexcept;

  /// @brief If the Variant contains an element the elements copy constructor
  /// is called otherwise an empty Variant is copied.
  /// @param[in] rhs The source of the copy.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611322
  constexpr Variant(const Variant& rhs) noexcept;

  /// @brief If the Variant contains an element the elements copy assignment operator
  /// is called otherwise an empty Variant is copied.
  /// @param[in] rhs The source of the copy assignment.
  /// @returns Reference to the Variant itself.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611323
  constexpr Variant& operator=(const Variant& rhs) noexcept;

  /// @brief If the Variant contains an element the elements move constructor
  /// is called otherwise an empty Variant is moved.
  /// @param[in] rhs The source of the move.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611324
  constexpr Variant(Variant&& rhs) noexcept;

  /// @brief If the Variant contains an element the elements move assignment operator
  /// is called otherwise an empty Variant is moved.
  /// @param[in] rhs The source of the move assignment.
  /// @returns Reference to the Variant itself.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611325
  constexpr Variant& operator=(Variant&& rhs) noexcept;

  /// @brief If the Variant contains an element the elements destructor
  /// is called otherwise nothing happens.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611326
  ~Variant() noexcept;

  /// @brief If the Variant contains an element the elements assignment operator
  /// is called otherwise we have undefined behavior.It is important that you make
  /// sure that the Variant really contains that type T.
  /// @tparam T The type of the rhs.
  /// @param[in] rhs The source object for the underlying move assignment.
  /// @returns It reference to the Variant itself.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611327
  template <typename T>
      typename std::enable_if<!std::is_same<T, Variant<Types...>&>::value, Variant<Types...>>::type&
      operator=(T&& rhs)
      & noexcept;

  /// @brief Calls the constructor of the type at index TypeIndex and perfectly
  /// forwards the arguments to this constructor. (not stl compliant)
  /// @tparam TypeIndex The index of the type which will be created.
  /// @tparam CTorArguments The variadic types of the c'tor arguments.
  /// @param[in] args The arguments which will be forwarded to the constructor to the type at TypeIndex.
  /// @returns If the Variant already contains a different type it returns false,
  /// if the construction was successful it returns true.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611328
  template <uint64_t TypeIndex, typename... CTorArguments>
  bool emplace_at_index(CTorArguments&&... args) noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Calls the constructor of the type T and perfectly forwards
  /// the arguments to the constructor of T.
  /// @tparam T The type which is created inside the Variant.
  /// @tparam CTorArguments The variadic types of the c'tor arguments.
  /// @param[in] args The arguments which will be forwarded to the constructor to the type at TypeIndex.
  /// @returns If the Variant already contains a different type it returns false,
  /// if the construction was successful it returns true.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611329
  template <typename T, typename... CTorArguments>
  bool emplace(CTorArguments&&... args) noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief Returns a pointer to the type stored at index TypeIndex. (not stl compliant)
  /// @tparam TypeIndex The index of the stored type.
  /// @returns If the Variant does contain the type at index TypeIndex it returns a valid
  /// pointer, if it does contain no type at all or a different type it returns nullptr.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611330
  template <uint64_t TypeIndex>
  typename iox::cxx::internal::get_type_at_index<0, TypeIndex, Types...>::type* get_at_index() noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Returns a pointer to the type stored at index TypeIndex. (not stl compliant)
  /// @tparam TypeIndex The index of the stored type.
  /// @returns If the Variant does contain the type at index TypeIndex
  /// it returns a valid pointer, if it does contain no type at all or
  /// a different type it returns nullptr.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611331
  template <uint64_t TypeIndex>
  const typename iox::cxx::internal::get_type_at_index<0, TypeIndex, Types...>::type* get_at_index() const
      // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
      noexcept;
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Returns a pointer to the type T stored in the Variant. (not stl compliant)
  /// @tparam T The type of the returned pointer.
  /// @returns If the Variant does contain the type T it returns a valid pointer otherwise
  /// if the Variant does contain no type at all or a different type it returns nullptr.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611332
  template <typename T>
  const T* get() const noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Returns a pointer to the type T stored in the Variant. (not stl compliant)
  /// @tparam T The type of the returned pointer.
  /// @returns If the Variant does contain the type T it returns a valid pointer otherwise
  /// if the Variant does contain no type at all or a different type it returns nullptr.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611333
  template <typename T>
  T* get() noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Returns a pointer to the type T if its stored in the Variant otherwise
  /// it returns the provided defaultValue.
  /// @tparam T The type of the returned pointer.
  /// @param[in] defaultValue Value which is returned in case of empty or different type in Variant.
  /// @returns Pointer to the stored value if it is of type T, otherwise defaultValue.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611334
  template <typename T>
  T* get_if(T* defaultValue) noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief Returns a pointer to the type T if its stored in the Variant otherwise
  /// it returns the provided defaultValue.
  /// @tparam T The type of the returned pointer.
  /// @param[in] defaultValue Value which is returned in case of empty or different type in Variant.
  /// @returns Pointer to the stored value if it is of type T, otherwise defaultValue.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611335
  template <typename T>
  const T* get_if(const T* defaultValue) const noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief Returns the index of the stored type in the Variant.
  /// If the Variant does not contain any type it returns INVALID_VARIANT_INDEX.
  /// @returns The index of the stored type.
  /// @trace SWS_CORE_S7.2.4.3_P1_004
  /// @archtrace 609898
  /// @design 611336
  constexpr uint64_t index() const noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

 private:
  /// @brief created the mStorage_ in private.
  alignas(iox::algorithm::max(alignof(Types)...)) iox::cxx::internal::byte_t mStorage_[stkTYPESIZE_]{0U};
  /// @brief created the mTypeIndex_ data type in private.
  uint64_t mTypeIndex_{INVALID_VARIANT_INDEX};

 private:
  template <typename T>
  /// @brief Created the has_bad_variant_element_access method in private.
  /// @design 611318
  bool has_bad_variant_element_access() const noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief Created the ErrorMessage method in private.
  /// @design 611318
  static void ErrorMessage(const char* source, const char* msg) noexcept;
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Created the call_element_destructor method in private.
  /// @design 611326
  void call_element_destructor() noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
};

/// @brief Definition of swap function to swap the content.
/// @tparam Types The type of the Variant.
/// @param[in] lhs First instance of the Variant.
/// @param[in] rhs Second instance of the Variant.
/// @trace SWS_CORE_S7.2.4.3_P1_004
/// @archtrace 609898
/// @design 611875
template <typename... Types>
void Swap(Variant<Types...>& lhs, Variant<Types...>& rhs)
{
  Variant<Types...> temp;
  temp = lhs;
  lhs = rhs;
  rhs = temp;
}

/// @brief Returns true if the Variant holds a given type T, otherwise false.
/// @tparam T The type of the returned pointer.
/// @tparam Types The type of the Variant.
/// @param[in] Variant It is a Variant to examine.
/// @returns True if the Variant currently holds the alternative T, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_004
/// @archtrace 609898
/// @design 611876
template <typename T, typename... Types>
constexpr bool holds_alternative(const Variant<Types...>& Variant) noexcept;
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
} // namespace core
} // namespace ara

#include "ara/core/variant.inl"

#endif // VARIANT_H

// -----------------------------------------------------------------------------------------------------------
// AVIN Guideline Exception(s)
// -----------------------------------------------------------------------------------------------------------
// - CodingStyle-Naming.Method
//   Guideline:           AUTOSAR CPP14 Guidelines, CodingStyle-Naming.Method required
//   Description:         Methods shall use pascal case.
//   Justification:       This will create another violation for initialization with pascal case. This has
//                        been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
//
// - CodingStyle-Naming.TemplateConstParameter
//   Guideline:           AUTOSAR CPP14 Guidelines, CodingStyle-Naming.TemplateConstParameter required
//   Description:         Names of template non-type parameters shall be camel case.
//   Justification:       This will create another violation for initialization with camel case. This has
//                        been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
//
// - CodingStyle-Naming.Function
//   Guideline:           AUTOSAR CPP14 Guidelines, CodingStyle-Naming.Function required
//   Description:         Function names shall be pascal case.
//   Justification:       This will create another violation for initialization with pascal case. This has
//                        been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
