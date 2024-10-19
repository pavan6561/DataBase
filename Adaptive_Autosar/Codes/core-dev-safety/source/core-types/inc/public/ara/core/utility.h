// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : utility.h
//
// Purpose     : This file provides the implementation of class utility.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_UTILITY_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_UTILITY_H_

#include <initializer_list>
#include <iterator>
#include <cstddef>
#include <climits>

#ifdef __GNUC__
#ifndef ATTR_PACKED
#define APD_PACKED __attribute__((packed))
#else
// Do here what is necessary for achieving SWS_CORE_10101 for this compiler!
#define APD_PACKED
#endif
#endif

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Internal.
namespace internal
{
/// @brief A non-integral binary type.
/// This is a class-based implementation. It fulfills all the requirements but also allows
/// other, non-conforming uses, in particular initialization with brackets, e.g. "Byte b(42)".
/// This is not possible with a std::byte-based implementation and thus should not be used.
/// It is implementation-defined whether this type can be used
/// for type aliasing without triggering Undefined Behavior.
/// If in doubt, instruct your compiler to exhibit non-strict aliasing
/// behavior with something like gcc's -fno-strict-aliasing.
/// @trace SWS_CORE_10100 SWS_CORE_10101 SWS_CORE_10104 SWS_CORE_10105
/// @archtrace 609897
/// @design 611303 614039
class Byte final
{
  /// @brief Value range.
  /// @trace SWS_CORE_10102
  /// @archtrace 609897
  unsigned char mValue_;

 public:
  /// @brief Default constructor.
  /// This constructor deliberately does not initialize anything, so that
  /// default-initialization leaves the instance with indeterminate value.
  /// It also deliberately is "defaulted on first declaration", so that this
  /// class does NOT have a "user-provided" default constructor, which is
  /// necessary for achieving the desired equivalence to std::byte.
  /// @trace SWS_CORE_10104
  /// @archtrace 609897
  Byte() noexcept = default;

  /// @brief Converting constructor from 'unsigned char'.
  /// @param[in] value A maximum value that can be stored in an unsigned char data type.
  /// @trace SWS_CORE_10102 SWS_CORE_10103 SWS_CORE_10106
  /// @archtrace 609897
  /// @design 611304
  constexpr explicit Byte(unsigned char value) noexcept : mValue_(value)
  {
  }

  /// @brief Conversion operator to 'unsigned char'.
  /// @trace SWS_CORE_10107 SWS_CORE_10108
  /// @archtrace 609897
  /// @design 611305
  constexpr explicit operator unsigned char() const noexcept
  {
    return mValue_;
  }
} APD_PACKED;

/// @brief The equality comparison of Byte.
/// @param[in] b1 Instance of the first Byte.
/// @param[in] b2 Instance of the second Byte.
/// @returns True if the b1 and b2 are equal.
/// @trace SWS_CORE_10109
/// @archtrace 609897
/// @design 611940
constexpr bool operator==(Byte b1, Byte b2) noexcept
{
  return (static_cast<unsigned char>(b1) == static_cast<unsigned char>(b2));
}

/// @brief The non-equality comparison of Byte.
/// @param[in] b1 Instance of the first Byte.
/// @param[in] b2 Instance of the second Byte.
/// @returns True if the b1 and b2 are not equal.
/// @trace SWS_CORE_10110
/// @archtrace 609897
/// @design 611941
constexpr bool operator!=(Byte b1, Byte b2) noexcept
{
  return (static_cast<unsigned char>(b1) != static_cast<unsigned char>(b2));
}

} // namespace internal

#if __cplusplus < 201703L
/// @brief A non-integral binary type.
/// @trace SWS_CORE_10103 SWS_CORE_10100
/// @archtrace 609897
using Byte = internal::Byte;
#else

/// @brief A non-integral binary type.
/// @trace SWS_CORE_10103 SWS_CORE_10100
/// @archtrace 609897
using Byte = std::byte;
#endif

// -------------------------------------------------------------------------

/// @brief Denote an operation to be performed in-place.
/// @trace SWS_CORE_10100
/// @archtrace 609897
struct InPlaceT1
{
  /// @brief The default constructor.
  /// @trace SWS_CORE_10100 SWS_CORE_10104
  /// @archtrace 609897
  explicit InPlaceT1() = default;
};

/// @brief The singleton instance of InPlaceT1.
/// @trace SWS_CORE_10100
/// @archtrace 609897
constexpr InPlaceT1 in_place1{};

/// @brief Denote a type-distinguishing operation to be performed in-place.
/// @tparam T The type of element.
/// @trace SWS_CORE_10100
/// @archtrace 609897
template <typename T>
struct InPlaceTypeT
{
  /// @brief The default constructor.
  /// @trace SWS_CORE_10100
  /// @archtrace 609897
  explicit InPlaceTypeT() = default;
};

/// @brief The singleton instances (one for each T) of InPlaceTypeT.
/// @tparam T The type to address.
/// @trace SWS_CORE_10100
/// @archtrace 609897
template <typename T>
constexpr InPlaceTypeT<T> in_place_type{};

/// @brief Denote an index-distinguishing operation to be performed in-place.
/// @tparam I Size of the index.
/// @trace SWS_CORE_10101
/// @archtrace 609897
template <std::size_t I>
struct InPlaceIndexT
{
  /// @brief The default constructor.
  /// @trace SWS_CORE_10100
  /// @archtrace 609897
  explicit InPlaceIndexT() = default;
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief The singleton instances (one for each I) of InPlaceIndexT.
/// @tparam I the index to address.
/// @trace SWS_CORE_10101
/// @archtrace 609897
template <std::size_t I>
constexpr InPlaceIndexT<I> in_place_index{};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

// -------------------------------------------------------------------------

#if (__cpp_lib_nonmember_container_access - 0) >= 201411
/// @brief Returns a pointer to the block of memory containing the elements of the range.
/// @trace SWS_CORE_10101
/// @archtrace 609897
using std::data;
/// @brief Returns the size of variable, container or an array.
/// @trace SWS_CORE_10101
/// @archtrace 609897
using std::size;
/// @brief Used to check whether the set container is empty or not.
/// @trace SWS_CORE_10101
/// @archtrace 609897
using std::empty;
#else

/// @brief Return a pointer to the block of memory that contains the elements of a container.
/// @tparam Container A type with a data() method.
/// @param[in] c Instance of the class Container.
/// @returns a pointer to the first element of the container.
/// @trace SWS_CORE_10100
/// @archtrace 609897
/// @design 611866
template <typename Container>
constexpr auto data(Container& c) -> decltype(c.data())
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  return c.data();
}

/// @brief Return a const_pointer to the block of memory that contains the elements of a container.
/// @tparam Container A type with a data() method.
/// @param[in] c An instance of Container.
/// @returns a pointer to the first element of the container.
/// @trace SWS_CORE_10100
/// @archtrace 609897
/// @design 611867
template <typename Container>
constexpr auto data(Container const& c) -> decltype(c.data())
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  return c.data();
}

/// @brief Return a pointer to the block of memory that contains the elements of a raw array.
/// @tparam T The type of array elements.
/// @tparam N The number of elements in the array.
/// @param[in] array Reference to a raw array.
/// @returns a pointer to the first element of the array.
/// @trace SWS_CORE_10101
/// @archtrace 609897
/// @design 611868
template <typename T, std::size_t N>
constexpr T* data(T (&array)[N]) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  return array;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Return a pointer to the block of memory that contains the elements of a std::initializer_list.
/// @tparam E The type of elements in the std::initializer_list.
/// @param[in] il It is an initializer list.
/// @returns a pointer to the first element of the std::initializer_list.
/// @trace SWS_CORE_10103
/// @archtrace 609897
/// @design 611869
template <typename E>
constexpr E const* data(std::initializer_list<E> il) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  return il.begin();
}

/// @brief Return the size of a container.
/// @tparam Container A type with a data() method.
/// @param[in] c An instance of Container.
/// @returns the size of the container.
/// @trace SWS_CORE_10101
/// @archtrace 609897
/// @design 611870
template <typename Container>
constexpr auto size(Container const& c) -> decltype(c.size())
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  return c.size();
}

/// @brief Return the size of a raw array.
/// @tparam T The type of array elements.
/// @tparam N The number of elements in the array.
/// @param[in] array Reference to a raw array.
/// @returns the size of the array, i.e. N.
/// @trace SWS_CORE_10101
/// @archtrace 609897
/// @design 611871
template <typename T, std::size_t N>
constexpr std::size_t size(T const (&array)[N]) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  static_cast<void>(array);
  return N;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Return whether the given container is empty.
/// @tparam Container A type with a empty() method.
/// @param[in] c Instance of the class Container.
/// @returns true if the container is empty, false otherwise.
/// @trace SWS_CORE_10100
/// @archtrace 609897
/// @design 611872
template <typename Container>
constexpr auto empty(Container const& c) -> decltype(c.empty())
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  return c.empty();
}

/// @brief Return whether the given raw array is empty.
/// As raw arrays cannot have zero elements in C++, this function always returns false.
/// @tparam T The type of array elements.
/// @tparam N The number of elements in the array.
/// @param[in] array It is an raw array.
/// @returns It returns false.
/// @trace SWS_CORE_10101
/// @archtrace 609897
/// @design 611873
template <typename T, std::size_t N>
constexpr bool empty(T const (&array)[N]) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  static_cast<void>(array);
  return false;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Return whether the given std::initializer_list is empty.
/// @tparam E The type of elements in the std::initializer_list.
/// @param[in] il The standard initializer list.
/// @returns true if the std::initializer_list is empty, false otherwise.
/// @trace SWS_CORE_10103
/// @archtrace 609897
/// @design 611874
template <typename E>
constexpr bool empty(std::initializer_list<E> il) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  return il.size() == 0;
}
#endif

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_UTILITY_H_
// -----------------------------------------------------------------------------------------------------------
// AVIN Guideline Exception(s)
// -----------------------------------------------------------------------------------------------------------
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
