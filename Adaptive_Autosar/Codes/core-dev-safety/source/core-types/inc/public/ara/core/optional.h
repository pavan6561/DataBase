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

#ifndef IOX_HOOFS_CXX_OPTIONAL_CORE__HPP
#define IOX_HOOFS_CXX_OPTIONAL_CORE__HPP

#include "iceoryx_hoofs/cxx/function_ref.hpp"
#include "iceoryx_hoofs/cxx/functional_interface.hpp"
#include "iceoryx_hoofs/cxx/requires.hpp"
#include "iceoryx_hoofs/cxx/types.hpp"

#include <new> // needed for placement new in the construct_value member function
#include <utility>

namespace ara
{
namespace core
{
/// @brief Helper struct which is used to signal an empty Optional.
/// It is equivalent to no value.
struct NullOptT
{
};
constexpr NullOptT nullopt = NullOptT();

/// @brief Helper struct which is used to call the in-place-construction constructor
struct InPlaceT
{
};

constexpr InPlaceT in_place{};
/// @brief Creates an a Optional class.
/// @tparam T The type of element in the class.
/// @archtrace 609892
/// @design 611614 614039
template <typename T>
class Optional : public iox::cxx::FunctionalInterface<Optional<T>, T, void>
{
 public:
  /// @brief Creates an Optional which has no value. If you access such an
  /// Optional via .value() or the arrow operator the behavior is undefined.
  /// @archtrace 609892
  /// @design 611615
  Optional() noexcept;

  /// @brief Creates an Optional which has no value. If you access such an
  /// Optional via .value() or the arrow operator the behavior is defined
  /// in the cxx::Expects handling.
  /// @archtrace 609892
  /// @design 611616
  Optional(const NullOptT&) noexcept;

  /// @brief Creates an Optional by forwarding value to the constructor of T.
  /// This Optional has a value.
  /// @param[in] value The rvalue of type T which will be moved into the Optional.
  /// @archtrace 609892
  /// @design 611617
  Optional(T&& value) noexcept;

  /// @brief Creates an Optional by using the copy constructor of T.
  /// @param[in] value The lvalue of type T which will be copy constructed into the Optional.
  /// @archtrace 609892
  /// @design 611618
  Optional(const T& value) noexcept;

  /// @brief Creates an Optional and an object inside the Optional on construction by
  /// perfectly forwarding args to the constructor of T.
  /// Could be used e.g. when T is not copyable/movable.
  /// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
  /// @param[in] InPlaceT A compile time variable to distinguish between constructors with certain behavior
  /// @param[in] args The arguments used for constructing the value.
  /// @archtrace 609892
  /// @design 611619
  template <typename... Targs>
  Optional(InPlaceT, Targs&&... args) noexcept;

  /// @brief The destructor will call the destructor of T if a value is set.
  /// @archtrace 609892
  /// @design 611620
  ~Optional() noexcept;

  /// @brief Constructs a value with the copy constructor if rhs has a value.
  /// Otherwise, it contains no value.
  /// @param[in] rhs The source of the copy.
  /// @archtrace 609892
  /// @design 611621
  Optional(const Optional& rhs) noexcept;

  /// @brief Constructs a value with the move constructor if rhs has a value.
  /// Otherwise, it contains no value.
  /// @param[in] rhs The source of the move.
  /// @archtrace 609892
  /// @design 611622
  Optional(Optional&& rhs) noexcept;

  /// @brief Copies an Optional. If the Optional has a value then the copy assignment
  /// of that value is called. If the Optional has no value a new value is constructed
  /// with the copy constructor.
  /// @param[in] rhs The source of the copy.
  /// @returns reference to the current Optional
  /// @archtrace 609892
  /// @design 611623
  Optional& operator=(const Optional& rhs) noexcept;

  /// @brief Moves an Optional. If the Optional has a value then the move assignment of
  /// that value is called. If the Optional has no value a new value is constructed with
  /// the move constructor.
  /// @param[in] rhs The source of the move.
  /// @returns reference to the current Optional
  /// @archtrace 609892
  /// @design 611624
  Optional& operator=(Optional&& rhs) noexcept;

  /// @brief If the optionals have values it compares these values
  /// by using their comparision operator.
  /// @param[in] rhs Value to which this Optional should be compared to.
  /// @returns true if the contained values are equal, otherwise false
  /// @archtrace 609892
  /// @design 611625
  constexpr bool operator==(const Optional<T>& rhs) const noexcept;

  /// @brief Comparision with NullOptT for easier unset Optional comparision.
  /// @returns true if the Optional is unset, otherwise false
  /// @archtrace 609892
  /// @design 611626
  constexpr bool operator==(const NullOptT&) const noexcept;

  /// @brief If the optionals have values it compares these values
  /// by using their comparision operator.
  /// @param[in] rhs Value to which this Optional should be compared to.
  /// @returns true if the contained values are not equal, otherwise false
  /// @archtrace 609892
  /// @design 611627
  constexpr bool operator!=(const Optional<T>& rhs) const noexcept;

  /// @brief Comparision with NullOptT for easier unset Optional comparision.
  /// @returns true if the Optional is set, otherwise false
  /// @archtrace 609892
  /// @design 611628
  constexpr bool operator!=(const NullOptT&) const noexcept;

  /// @brief Direct assignment of the underlying value.
  /// If the Optional has no value then a new T is constructed by forwarding the assignment
  /// to T's constructor. If the Optional has a value the assignment operator of T is called.
  /// @tparam U True if U is convertible to T.
  /// @param[in] value A value to assign to the underlying Optional value.
  /// @returns reference to the current Optional
  /// @archtrace 609892
  /// @design 611629
  template <typename U = T>
  typename std::enable_if<!std::is_same<U, Optional<T>&>::value, Optional>::type& operator=(
      U&& value) noexcept;

  /// @brief Returns a pointer to the underlying value.
  /// If the Optional has no value the behavior is undefined. You need to verify
  /// that the Optional has a value by calling has_value() before using it.
  /// @returns pointer of type const T to the underlying type
  /// @archtrace 609892
  /// @design 611630
  const T* operator->() const noexcept;

  /// @brief Returns a reference to the underlying value.
  /// If the Optional has no value the behavior is undefined. You need to verify
  /// that the Optional has a value by calling has_value() before using it.
  /// @returns reference of type const T to the underlying type
  /// @archtrace 609892
  /// @design 611631
  const T& operator*() const noexcept;

  /// @brief Returns a pointer to the underlying value.
  /// If the Optional has no value the behavior is undefined. You need to verify
  /// that the Optional has a value by calling has_value() before using it.
  /// @returns pointer of type T to the underlying type
  /// @archtrace 609892
  /// @design 611632
  T* operator->() noexcept;

  /// @brief Returns a reference to the underlying value.
  /// If the Optional has no value the behavior is undefined. You need to verify
  /// that the Optional has a value by calling has_value() before using it.
  /// @returns reference of type T to the underlying type.
  /// @archtrace 609892
  /// @design 611633
  T& operator*() noexcept;

  /// @brief Will return true if the Optional contains a value, otherwise false.
  /// @archtrace 609892
  /// @design 611634
  constexpr explicit operator bool() const noexcept;

  /// @brief Will return true if the Optional contains a value, otherwise false.
  /// @returns true if Optional contains a value, otherwise false.
  /// @archtrace 609892
  /// @design 611635
  constexpr bool has_value() const noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief A new element is constructed by forwarding the arguments to the
  /// constructor of T. If the Optional has a value then the destructor of T is called.
  /// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
  /// @param[in] args The arguments used for constructing the value.
  /// @returns reference to the underlying type
  /// @archtrace 609892
  /// @design 611636
  template <typename... Targs>
  T& emplace(Targs&&... args) noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief Calls the destructor of T if the Optional has a value. If the Optional has
  /// no value, nothing happens. After that call the Optional has no more value.
  /// @archtrace 609892
  /// @design 611637
  void reset() noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief Returns a reference to the underlying value.
  /// If the Optional has no value the application terminates. You need to verify
  /// that the Optional has a value by calling has_value() before using it.
  /// @returns reference to the underlying type
  /// @archtrace 609892
  /// @design 611638
  T& value() & noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief Returns a const reference to the underlying value.
  /// If the Optional has no value the application terminates. You need to verify
  /// that the Optional has a value by calling has_value() before using it.
  /// @returns const reference to the underlying type
  /// @archtrace 609892
  /// @design 611639
  const T& value() const& noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief Returns a rvalue reference to the underlying value.
  /// If the Optional has no value the application terminates. You need to verify
  /// that the Optional has a value by calling has_value() before using it.
  /// @returns rvalue reference to the underlying type
  /// @archtrace 609892
  /// @design 611640
  T&& value() && noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief Returns a const rvalue reference to the underlying value.
  /// If the Optional has no value the application terminates. You need to verify
  /// that the Optional has a value by calling has_value() before using it.
  /// @returns const rvalue reference to the underlying type
  /// @archtrace 609892
  /// @design 611641
  const T&& value() const&& noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// Avin Implemented.
  /// @brief Copy assignment operator of the class Optional.
  /// @param[in] value To assign to the contained value.
  /// @returns Reference to this.
  /// @archtrace 609892
  /// @design 611642
  Optional<T>& operator=(const T& value) noexcept;

  /// Avin Implemented.
  /// @brief Move assignment operator of the class Optional.
  /// @param[in] value To assign to the contained value.
  /// @returns Reference to this.
  /// @archtrace 609892
  /// @design 611643
  Optional<T>& operator=(T&& value) noexcept;

  /// Avin Implemented.
  /// @brief It is an empty class type used to indicate optional type with uninitialized state.
  /// @param[in] NullOptT It is a null pointer value.
  /// @archtrace 609892
  /// @design 611644
  void operator=(NullOptT) noexcept;

 private:
  /// @brief It creates an mHasValue_ variable.
  bool mHasValue_{false};
  /// @brief It creates an alignas structure.
  struct alignas(T) element_t
  {
    /// @brief It creates an data.
    iox::cxx::byte_t data[sizeof(T)];
  };
  /// @brief It creates an mData_.
  element_t mData_;

 private:
  /// @brief It creates an construct_value.
  /// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
  /// @param[in] args The arguments used for constructing the value.
  /// @design 611619
  template <typename... Targs>
  void construct_value(Targs&&... args) noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.

  /// @brief It creates an destruct_value.
  /// @design 611637
  void destruct_value() noexcept;
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
};

/// @brief Adaptive Core.
namespace // unnamed
{
/// @brief A struct is_optional. Is used as a building block in type traits,
/// which represent the values false as types.
/// @tparam T The type of element in the class.
template <typename T>
struct is_optional : std::false_type
{
};

/// @brief A struct is_optional. Is used as a building block in type traits,
/// which represent the values true as types.
/// @tparam T The type of element in the class.
template <typename T>
struct is_optional<Optional<T>> : std::true_type
{
};

/// @brief Alias for the std::enable_if.
/// @tparam U True if U is convertible to T.
/// @tparam T True if T is convertible to U.
template <typename U, typename T>
using EnableIfConvertible = typename std::enable_if<std::is_convertible<U, T>::value>::type;

/// @brief Alias for the Enable If Copy Constructible
/// @tparam T The type of the value.
template <typename T>
using EnableIfCopyConstructible = typename std::enable_if<std::is_copy_constructible<T>::value>::type;

/// @brief Alias for the Enable If Move Constructible.
/// @tparam T The type of the value.
template <typename T>
using EnableIfMoveConstructible = typename std::enable_if<std::is_move_constructible<T>::value>::type;

/// @brief Alias for the Enable If Not Optional.
/// @tparam T The type of the value.
template <typename T>
using EnableIfNotOptional = typename std::enable_if<!is_optional<typename std::decay<T>::type>::value>::type;

/// @brief Alias for the Enable If LValue Reference.
/// @tparam T The type of the value.
template <typename T>
using EnableIfLValueReference = typename std::enable_if<std::is_lvalue_reference<T>::value>::type;

/// @brief Alias for the Enable If Not LValue Reference.
/// @tparam T The type of the value.
template <typename T>
using EnableIfNotLValueReference = typename std::enable_if<!std::is_lvalue_reference<T>::value>::type;

/// @brief Alias for the Enable If Constructible.
/// @tparam T The type of the value.
/// @tparam Args The types of arguments given to this function.
template <typename T, typename... Args>
using EnableIfConstructible = typename std::enable_if<std::is_constructible<T, Args...>::value>::type;
} // unnamed namespace

/// @brief It creates an operator==().
/// @tparam T The type of the value.
/// @param[in] v The lvalue of type T which will be moved into the Optional.
/// @param[in] rhs Value to which this Optional should be compared to.
/// @returns true if the contained values are equal, otherwise false.
/// @archtrace 609892
/// @design 611848
template <typename T>
constexpr bool operator==(const T& v, const Optional<T>& rhs);

/// @brief Creates an Optional which contains a value by forwarding
/// the arguments to the constructor of T.
/// @tparam OptionalBaseType It is an option base type argument.
/// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
/// @param[in] args The arguments which will be perfectly forwarded
/// to the constructor of T.
/// @returns Optional which contains T constructed with args.
/// @archtrace 609892
/// @design 611849
template <typename OptionalBaseType, typename... Targs>
Optional<OptionalBaseType> make_optional(Targs&&... args) noexcept;

} // namespace core
} // namespace ara

#include "ara/core/optional.inl"

#endif // IOX_HOOFS_CXX_OPTIONAL_CORE__HPP
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
// -----------------------------------------------------------------------------------------------------------
