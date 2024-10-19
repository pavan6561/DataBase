// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : type_traits.h
//
// Purpose     : This file provides the implementation of class type_traits.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_TYPE_TRAITS_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_TYPE_TRAITS_H_

#include <cstdint>
#include <type_traits>

namespace ara
{
namespace core
{
/// @brief Conditionally add const to type T if C has the const qualifier.
/// @tparam T Is the type to conditionally add the const qualifier.
/// @tparam C Condition is the type which determines if the const qualifier needs to be added to T.
template <typename T, typename C>
struct add_const_conditionally
{
  using type = T;
};

/// @brief Conditionally add const to type T if C has the const qualifier.
/// @tparam T Is the type to conditionally add the const qualifier.
/// @tparam C Condition is the type which determines if the const qualifier needs to be added to T.
template <typename T, typename C>
struct add_const_conditionally<T, const C>
{
  using type = const T;
};

/// @brief Helper type for add_const_conditionally which adds const to type T if C has the const qualifier.
/// @tparam T Is the type to conditionally add the const qualifier.
/// @tparam C Condition is the type which determines if the const qualifier needs to be added to T.
template <typename T, typename C>
using add_const_conditionally_t = typename add_const_conditionally<T, C>::type;

/// @brief Helper value to bind a static_assert to a type.
/// @code
/// static_assert(always_false_v<Foo>, "Not implemented for the given type!");
/// @endcode
template <typename>
constexpr bool always_false_v = false;

// windows defines __cplusplus as 199711L
#if __cplusplus < 201703L && !defined(_WIN32)
template <typename C, typename... Cargs>
using invoke_result = std::result_of<C(Cargs...)>;
#elif __cplusplus >= 201703L || defined(_WIN32)
template <typename C, typename... Cargs>
using invoke_result = std::invoke_result<C, Cargs...>;
#endif

/// @brief Verifies whether the passed Callable type is in fact invocable with the given arguments
/// @tparam Callable Is something that can be called like a function.
/// @tparam ArgTypes A arguments parameter pack.
template <typename Callable, typename... ArgTypes>
struct is_invocable
{
  // This variant is chosen when Callable(ArgTypes) successfully resolves to a valid type, i.e. is invocable.
  /// @tparam C Condition is the type which determines if the const qualifier needs to be added to T.
  /// @note result_of is deprecated, switch to invoke_result in C++17.
  template <typename C, typename... As>
  static constexpr std::true_type test(typename invoke_result<C, As...>::type*) noexcept
  {
    return {};
  }

  // This is chosen if Callable(ArgTypes) does not resolve to a valid type.
  /// @tparam C Condition is the type which determines if the const qualifier needs to be added to T.
  template <typename C, typename... As>
  static constexpr std::false_type test(...) noexcept
  {
    return {};
  }

  // Test with nullptr as this can stand in for a pointer to any type.
  static constexpr bool value = decltype(test<Callable, ArgTypes...>(nullptr))::value;
};

/// @brief Verifies whether the passed Callable type is in fact invocable with the
/// given arguments and the result of the invocation is convertible to ReturnType.
/// @tparam ReturnType For returning the parameter value for primitives.
/// @tparam Callable Is something that can be called like a function.
/// @tparam ArgTypes A arguments parameter pack.
/// @note This is an implementation of std::is_invokable_r (C++17).
template <typename ReturnType, typename Callable, typename... ArgTypes>
struct is_invocable_r
{
  /// @tparam C Condition is the type which determines if the const qualifier needs to be added to T.
  template <typename C, typename... As>
  static constexpr std::true_type test(std::enable_if_t<
      std::is_convertible<typename invoke_result<C, As...>::type, ReturnType>::value>*) noexcept
  {
    return {};
  }

  /// @tparam C Condition is the type which determines if the const qualifier needs to be added to T.
  template <typename C, typename... As>
  static constexpr std::false_type test(...) noexcept
  {
    return {};
  }

  // Test with nullptr as this can stand in for a pointer to any type.
  static constexpr bool value = decltype(test<Callable, ArgTypes...>(nullptr))::value;
};

/// @brief Check whether T is a function pointer with arbitrary signature.
/// @tparam T The type of element in the class.
template <typename T>
struct is_function_pointer : std::false_type
{
};

/// @brief Struct to check whether an argument is a function pointer.
/// @tparam ReturnType For returning the parameter value for primitives.
/// @tparam ArgTypes A arguments parameter pack.
template <typename ReturnType, typename... ArgTypes>
struct is_function_pointer<ReturnType (*)(ArgTypes...)> : std::true_type
{
};

/// @brief A fixed-length string of N bytes.
/// @tparam Capacity The capacity of the given string.
template <uint64_t Capacity>
class FixedString;

/// @brief Struct to check whether an argument is a char array.
/// @tparam T The type of element in the class.
template <typename T>
struct is_char_array : std::false_type
{
};

/// @brief Struct to check whether an argument is a char array.
/// @tparam N The number of elements in the class.
template <uint64_t N>
struct is_char_array<char[N]> : std::true_type
{
};

/// @brief Struct to check whether an argument is a cxx string.
/// @tparam T The type of element in the class.
template <typename T>
struct is_cxx_string : std::false_type
{
};

/// @brief Struct to check whether an argument is a cxx string.
/// @tparam N The number of elements in the class.
template <uint64_t N>
struct is_cxx_string<::ara::core::FixedString<N>> : std::true_type
{
};

/// @brief Maps a sequence of any types to the type void.
/// @tparam T The type of element in the class.
template <typename... T>
using void_t = void;
} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_TYPE_TRAITS_H_
