// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : array.h
//
// Purpose     : This file provides the implementation of class Array.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_ARRAY_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_ARRAY_H_
#include <algorithm>
#include <iterator>
#include <utility>
#include <type_traits>
#include <tuple>
#include <cstddef>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Internal.
namespace internal
{
/// @brief Definition of ArrayHolder.
/// @tparam T The type of element in the array.
/// @tparam N The number of elements in the array.
template <typename T, std::size_t N>
struct ArrayHolder final
{
  /// @brief Nth element in the array..
  using Type = T[N];
  /// @brief Function Ptr to return const Pointer.
  /// @param[in] data Instance of the Type.
  /// @param[in] i Index of the data.
  /// @returns const Pointer pointing to the index.
  static constexpr T const* Ptr(Type const& data, std::size_t i) noexcept
  {
    return &data[i];
  }
  /// @brief Function Ptr to return const Pointer.
  /// @param[in] data Instance of the Type.
  /// @param[in] i Index of the data.
  /// @returns const Pointer pointing to the index.
  static constexpr T* Ptr(Type& data, std::size_t i) noexcept
  {
    return &data[i];
  }
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Definition of ArrayHolder.
/// @tparam T The type of element in the array.
template <typename T>
struct ArrayHolder<T, 0> final
{
  /// @brief Definition of struct Type.
  struct Type final
  {
  };
  /// @brief Function Ptr to return const Pointer.
  /// @param[in] data Instance of the Type.
  /// @param[in] i Index of the data.
  /// @returns const Pointer pointing to the index.
  static constexpr T const* Ptr(Type const&, std::size_t) noexcept
  {
    return nullptr;
  }
  /// @brief Function Ptr to return const Pointer.
  /// @param[in] data Instance of the Type.
  /// @param[in] i Index of the data.
  /// @returns const Pointer pointing to the index.
  static constexpr T* Ptr(Type&, std::size_t) noexcept
  {
    return nullptr;
  }
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

// This helper namespace basically re-implements C++17's std::IsNothrowSwappable.
// The key to this is that ADL has to be used for the swap() call within noexcept.
namespace adl
{
/// @brief using std::swap.
using std::swap;

/// @brief Definition of IsNothrowSwappable.
/// @tparam T The type of element in the array.
template <typename T>
struct IsNothrowSwappable
    : std::integral_constant<bool, noexcept(swap(std::declval<T&>(), std::declval<T&>()))>
{
};

} // namespace adl
} // namespace internal

/// @brief Encapsulation of fixed size arrays.
/// @tparam T The type of element in the array.
/// @tparam N The number of elements in the array.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611458 614039
template <typename T, std::size_t N>
class Array final
{
 public:
  /// @brief Alias type for a Reference to an element.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  using Reference = T&;

  /// @brief Alias type for a ConstReference to an element.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  using ConstReference = T const&;

  /// @brief The type of a Iterator to elements.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  using Iterator = T*;

  /// @brief The type of a const_iterator to elements.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  using const_iterator = T const*;

  /// @brief Alias for the type of parameters that indicate an index into the Array.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  using SizeType = std::size_t;

  /// @brief Alias for the type of elements in this Array.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  using value_type = T;

  /// @brief The type of a ReverseIterator to elements.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  using ReverseIterator = std::reverse_iterator<Iterator>;

  /// @brief The type of a ConstReverseIterator to elements.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  using ConstReverseIterator = std::reverse_iterator<const_iterator>;

  // no explicit construct/copy/destroy for aggregate type

  /// @brief Alias name for ArrayHolder.
  /// @tparam T The type of element in the array.
  /// @tparam N The number of elements in the array.
  using AH = internal::ArrayHolder<T, N>;

  /// @brief Alias name for Type.
  typename AH::Type mData;

  /// @brief Assign the given value to all elements of this Array.
  /// @param[in] u Value to assign to the elements in the filled range.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611459
  void fill(T const& u) noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    std::fill(begin(), end(), u);
  }

  /// @brief Exchange the contents of this Array with those of other.
  /// @param[in] other Container to exchange the contents with.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611460
  void swap(Array<T, N>& other) noexcept(internal::adl::IsNothrowSwappable<T>::value)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    std::swap_ranges(begin(), end(), other.begin());
  }

  // iterators:

  /// @brief Return an Iterator pointing to the first element of this Array.
  /// @returns The Iterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611461
  Iterator begin() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return AH::Ptr(mData, 0);
  }

  /// @brief Return a const_iterator pointing to the first element of this Array.
  /// @returns The const_iterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611462
  const_iterator begin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return AH::Ptr(mData, 0);
  }
  /// @brief Return an Iterator pointing past the last element of this Array.
  /// @returns The Iterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611463
  Iterator end() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return AH::Ptr(mData, N);
  }

  /// @brief Return a const_iterator pointing past the last element of this Array.
  /// @returns The const_iterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611464
  const_iterator end() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return AH::Ptr(mData, N);
  }
  /// @brief Return a ReverseIterator pointing to the last element of this Array.
  /// @returns The ReverseIterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611465
  ReverseIterator rbegin() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ReverseIterator(end());
  }

  /// @brief Return a ConstReverseIterator pointing to the last element of this Array.
  /// @returns The ConstReverseIterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611466
  ConstReverseIterator rbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ConstReverseIterator(end());
  }

  /// @brief Return a ReverseIterator pointing past the first element of this Array.
  /// @returns The ReverseIterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611467
  ReverseIterator rend() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ReverseIterator(begin());
  }

  /// @brief Return a ConstReverseIterator pointing past the first element of this Array.
  /// @returns The ConstReverseIterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611468
  ConstReverseIterator rend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ConstReverseIterator(begin());
  }

  /// @brief Return a const_iterator pointing to the first element of this Array.
  /// @returns The const_iterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611469
  const_iterator cbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return begin();
  }

  /// @brief Return a const_iterator pointing past the last element of this Array.
  /// @returns The const_iterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611470
  const_iterator cend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return end();
  }

  /// @brief Return a ConstReverseIterator pointing to the last element of this Array.
  /// @returns The ConstReverseIterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611471
  ConstReverseIterator crbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return rbegin();
  }

  /// @brief Return a ConstReverseIterator pointing past the first element of this Array.
  /// @returns The ConstReverseIterator.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611472
  ConstReverseIterator crend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return rend();
  }

  // capacity:

  /// @brief Return the number of elements in this Array.
  /// @returns Size of an array.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611473
  constexpr SizeType size() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return N;
  }

  /// @brief Return the maximum number of elements supported by this Array.
  /// @returns Size of an array.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611474
  constexpr SizeType max_size() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return N;
  }

  /// @brief Return whether this Array is empty.
  /// @returns True if this Array contains 0 elements, false otherwise.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611475
  constexpr bool empty() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return N == 0;
  }

  // element access:

  /// @brief Return a Reference to the n-th element of this Array.
  /// @param[in] n The index into this Array.
  /// @returns The Reference.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611476
  Reference operator[](SizeType n) noexcept
  {
    return *AH::Ptr(mData, n);
  }

  /// @brief Return a ConstReference to the n-th element of this Array.
  /// @param[in] n The index into this Array.
  /// @returns The ConstReference.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611477
  constexpr ConstReference operator[](SizeType n) const noexcept
  {
    return *AH::Ptr(mData, n);
  }

  /// @brief Return a Reference to the first element of this Array.
  /// @returns The Reference.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611478
  Reference front() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return *AH::Ptr(mData, 0);
  }

  /// @brief Return a ConstReference to the first element of this Array.
  /// @returns The Reference.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611479
  constexpr ConstReference front() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return *AH::Ptr(mData, 0);
  }

  /// @brief Return a Reference to the last element of this Array.
  /// @returns The Reference.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611480
  Reference back() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return *AH::Ptr(mData, N - 1);
  }

  /// @brief Return a ConstReference to the last element of this Array.
  /// @returns The Reference.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611481
  constexpr ConstReference back() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return *AH::Ptr(mData, N - 1);
  }

  /// @brief Return a Pointer to the first element of this Array.
  /// @returns The Pointer.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611482
  T* data() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return AH::Ptr(mData, 0);
  }

  /// @brief Return a ConstPointer to the first element of this Array.
  /// @returns The ConstPointer.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  /// @design 611483
  T const* data() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return AH::Ptr(mData, 0);
  }
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.
// comparison operators:

/// @brief Return true if the two Arrays have equal content.
/// @tparam T The type of element in the Array.
/// @tparam N The number of elements in the Array.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns True if the Arrays are equal, false otherwise.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611812
template <typename T, std::size_t N>
bool operator==(Array<T, N> const& lhs, Array<T, N> const& rhs) noexcept
{
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Return true if the two Arrays have non-equal content.
/// @tparam T The type of element in the Array.
/// @tparam N The number of elements in the Array.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns True if the Arrays are non-equal, false otherwise.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611813
template <typename T, std::size_t N>
bool operator!=(Array<T, N> const& lhs, Array<T, N> const& rhs) noexcept
{
  return !operator==(lhs, rhs);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Return true if the contents of lhs are lexicographically
/// less than the contents of rhs.
/// @tparam T The type of element in the Array.
/// @tparam N The number of elements in the Array.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns True if lhs is less than rhs, false otherwise.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611814
template <typename T, std::size_t N>
bool operator<(Array<T, N> const& lhs, Array<T, N> const& rhs) noexcept
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Return true if the contents of rhs are lexicographically
/// less than the contents of lhs.
/// @tparam T The type of element in the Array.
/// @tparam N The number of elements in the Array.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns True if rhs is less than lhs, false otherwise.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611815
template <typename T, std::size_t N>
bool operator>(Array<T, N> const& lhs, Array<T, N> const& rhs) noexcept
{
  return rhs < lhs;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Return true if the contents of lhs are lexicographically
/// less than or equal to the contents of rhs.
/// @tparam T The type of element in the Array.
/// @tparam N The number of elements in the Array.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns True if lhs is less than or equal to rhs, false otherwise.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611816
template <typename T, std::size_t N>
bool operator<=(Array<T, N> const& lhs, Array<T, N> const& rhs) noexcept
{
  return !(rhs < lhs);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Return true if the contents of rhs are lexicographically
/// less than or equal to the contents of lhs.
/// @tparam T The type of element in the Array.
/// @tparam N The number of elements in the Array.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns True if rhs is less than or equal to lhs, false otherwise.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611817
template <typename T, std::size_t N>
bool operator>=(Array<T, N> const& lhs, Array<T, N> const& rhs) noexcept
{
  return !(lhs < rhs);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Overload of std::swap for ara::core::Array.
/// @tparam T The type of element in the Arrays.
/// @tparam N The number of elements in the Arrays.
/// @param[in] lhs The left-hand side of the call.
/// @param[in] rhs The right-hand side of the call.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611818
template <typename T, std::size_t N>
void swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs)))
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  lhs.swap(rhs);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

// tuple interface (1/2)
// The C++14 standard prohibits extension of the std namespace with new overloads,
// so we need to put them into our own namespace and rely on ADL to have them found.

/// @brief Overload of std::get for an lvalue mutable ara::core::Array.
/// @tparam I The integer value in the array.
/// @tparam T The type of element in the array.
/// @tparam N The number of elements in the array.
/// @param[in] a Array of N elements of type T.
/// @returns Value of type T.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611819
template <std::size_t I, typename T, std::size_t N>
constexpr T& get(Array<T, N>& a) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  static_assert(I < N, "index out of bounds");
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  return a[I];
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Overload of std::get for an rvalue ara::core::Array.
/// @tparam I The integer value in the array.
/// @tparam T The type of element in the array.
/// @tparam N The number of elements in the array.
/// @param[in] a Array of N elements of type T.
/// @returns returns value of type T.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611820
template <std::size_t I, typename T, std::size_t N>
constexpr T&& get(Array<T, N>&& a) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  return std::move(get<I>(a));
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Overload of std::get for an lvalue const ara::core::Array.
/// @tparam I The integer value in the array.
/// @tparam T The type of element in the array.
/// @tparam N The number of elements in the array.
/// @param[in] a Array of N elements of type T.
/// @returns returns value of type T.
/// @trace SWS_CORE_11200
/// @archtrace 609885
/// @design 611821
template <std::size_t I, typename T, std::size_t N>
constexpr T const& get(Array<T, N> const& a) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  static_assert(I < N, "index out of bounds");
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  return a[I];
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

} // namespace core
} // namespace ara

// tuple interface (2/2)
// Specialization of these symbols in the std namespace is permitted in C++14 by 17.6.4.2.1 [namespace.std],
// because they depend on a user-defined type (i.e. ara::core::Array).

/// @brief namespace std.
namespace std
{
/// @brief Specialization of std::tuple_size for ara::core::Array.
/// @tparam T The type of element in the array.
/// @tparam N The number of elements in the array.
/// @trace SWS_CORE_11200
/// @archtrace 609885
template <typename T, std::size_t N>
struct tuple_size<ara::core::Array<T, N>> : integral_constant<size_t, N>
{
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Specialization of std::tuple_element for ara::core::Array.
/// @tparam T The type of element in the array.
/// @tparam N The number of elements in the array.
/// @trace SWS_CORE_11200
/// @archtrace 609885
template <std::size_t I, typename T, std::size_t N>
struct tuple_element<I, ara::core::Array<T, N>>
{
  /// @brief Alias for the type of the Array element with the given index.
  /// @trace SWS_CORE_11200
  /// @archtrace 609885
  using type = T;
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

} // namespace std

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_ARRAY_H_

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
//
// - A5.1.1-2
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A5.1.1 required
//   Description:         Use of magic string literal.
//   Justification:       This will create another violation for initialization with magic string literal.This
//                        has been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
