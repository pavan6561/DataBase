// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : span.h
//
// Purpose     : This file provides the implementation of class span.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_SPAN_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_SPAN_H_

#include "ara/core/array.h"
#include "ara/core/utility.h"

#include <iostream>
#include <type_traits>
#include <limits>
#include <algorithm>
#include <cstddef>
#include <cassert>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
namespace
{
/// @brief The flag DEBUG_SPAN.
/// @trace SWS_CORE_01901
/// @archtrace 609894
constexpr static bool DEBUG_SPAN{false};

/// @brief The definition of the xtrace.
/// @tparam Args A template parameter pack.
/// @param[in] args Multiple arguments in function.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
template <typename... Args>
static inline void xtrace(Args const&... args)
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  if (DEBUG_SPAN)
  {
    /// @brief Alias name for Integer array.
    using Expander = std::int32_t[];
    static_cast<void>(Expander{0, (static_cast<void>(std::cerr << args), 0)...});
  }
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

} // namespace

/// @brief A constant for creating Spans with dynamic sizes.
/// The constant is always set to std::numeric_limits<std::size_t>::max().
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
constexpr std::size_t dynamic_extent{std::numeric_limits<std::size_t>::max()};

/// @brief The forward declaration of Span.
/// @tparam T The type of elements in the Span.
/// @tparam Extent The number of elements in the sequence.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
/// @design 613190 614039
template <typename T, std::size_t Extent>
class Span;
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief namespace internal.
namespace internal
{
/// @brief Combines remove_cv and removes a Reference from the given type.
/// @tparam T The type of elements in the array.
template <typename T>
struct RemoveCvref
{
  /// @brief Alias name for std::remove_cv_t.
  using Type = std::remove_cv_t<std::remove_reference_t<T>>;
};

/// @brief It return the boolean value true if T is without const and volatile
/// qualified, otherwise return false.
/// @tparam T The type of elements in the array.
template <typename T>
using RemoveCvrefT = typename RemoveCvref<T>::Type;

/// @brief Instantitation of integral_constant to represent the bool value false.
/// @tparam T The type of elements in the Span.
template <typename T>
struct IsAraCoreSpanChecker : public std::false_type
{
};

/// @brief Definition of struct IsAraCoreSpanChecker.
/// @tparam T The type of elements in the Span.
/// @tparam Extent The number of elements in the sequence.
template <typename T, std::size_t Extent>
struct IsAraCoreSpanChecker<core::Span<T, Extent>> : public std::true_type
{
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Provides the member typedef type which is the same as T,
/// except that its topmost cv-qualifiers are removed.
/// @tparam T The type of elements in the Span.
template <typename T>
struct IsAraCoreSpan : public IsAraCoreSpanChecker<RemoveCvrefT<T>>
{
};
/// @brief Definition of struct IsAraCoreArrayChecker.
/// @tparam T The type of elements in the Span.
template <typename T>
struct IsAraCoreArrayChecker : public std::false_type
{
};

/// @brief Definition of struct IsAraCoreArrayChecker.
/// @tparam T The type of elements in the Span.
/// @tparam N The size of an array.
template <typename T, std::size_t N>
struct IsAraCoreArrayChecker<core::Array<T, N>> : public std::true_type
{
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Definition of struct IsAraCoreArray.
/// @tparam T The type of elements in the Span.
template <typename T>
struct IsAraCoreArray : public IsAraCoreArrayChecker<RemoveCvrefT<T>>
{
};
/// @brief Definition of struct IsComplete.
/// @tparam T The type of elements in the Span.
/// @tparam S The Size of the Span.
template <typename T, typename S = size_t>
struct IsComplete : std::false_type
{
};

/// @brief Definition of struct IsComplete.
/// @tparam T The type of elements in the Span.
template <typename T>
struct IsComplete<T, decltype(sizeof(T))> : std::true_type
{
};

/// @brief Re-implementation of std::VoidT from C++20.
/// We assume that the compiler implements CWG defect 1558.
/// @tparam Args A template parameter pack.
template <typename... Args>
using VoidT = void;

/// @brief Definition of struct IsComplete.
/// @tparam T The type of elements in the Span.
/// @tparam ArgT Argument type of the IsSuitableContainer.
/// @tparam C Capacity of the IsSuitableContainer.
/// @tparam ElementType The type of element in the Span.
template <typename T, typename ArgT, typename C, typename ElementType = void>
struct IsSuitableContainer : public std::false_type
{
};

/// @brief Definition of struct IsSuitableContainer.
/// @tparam C Capacity of the IsSuitableContainer.
/// @tparam ArgT Argument type of the IsSuitableContainer
/// @tparam ElementType The type of element in the Span.
template <typename C, typename ArgT, typename ElementType>
struct IsSuitableContainer<C, ArgT, ElementType,
    VoidT< //
        std::enable_if_t<!internal::IsAraCoreSpan<C>::value>, //
        std::enable_if_t<!internal::IsAraCoreArray<C>::value>, //
        std::enable_if_t<!std::is_array<C>::value>, //
        decltype(core::data(std::declval<ArgT>())), //
        decltype(core::size(std::declval<ArgT>())), //
        std::enable_if_t<
            std::is_convertible<std::remove_pointer_t<decltype(core::data(std::declval<ArgT>()))> (*)[],
                // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
                ElementType (*)[]>::value> //
        // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
        > //
    > : public std::true_type
{
};

} // namespace internal

/// @brief A view over a contiguous sequence of objects.
/// @tparam T The type of elements in the Span.
/// @tparam Extent The stkExtent to use for this Span.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
template <typename T, std::size_t Extent = dynamic_extent>
class Span
{
  static_assert(std::is_object<T>::value, "Span cannot contain Reference or void types");
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  static_assert(!std::is_abstract<T>::value, "Span cannot contain abstract types");
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  static_assert(internal::IsComplete<T>::value, "Span cannot contain incomplete types");
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.

  /// @brief Alias for the type of elements in this Span.
  /// @tparam U The type of elements within the other Span.
  template <typename U>
  using TypeOfData = decltype(ara::core::data(std::declval<U>()));

 public:
  /// @brief Alias for the type of elements in this Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  using ElementType = T;
  /// @brief Alias for the type of values in this Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  using value_type = typename std::remove_cv<ElementType>::type;
  /// @brief Alias for the type of parameters that indicate an index into the Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  using IndexType = std::size_t;
  /// @brief Alias type for a pointer to an element.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  using pointer = ElementType*;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Alias type for a Reference to an element.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  using Reference = ElementType&;
  /// @brief The type of an Iterator to elements.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  using Iterator = ElementType*;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief The type of a const_iterator to elements.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  using const_iterator = ElementType const*;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief The type of a ReverseIterator to elements.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  using ReverseIterator = std::reverse_iterator<Iterator>;
  /// @brief The type of a ConstReverseIterator to elements.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  using ConstReverseIterator = std::reverse_iterator<const_iterator>;
  /// @brief A constant reflecting the configured Extent of this Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  static constexpr IndexType stkExtent{Extent};

  /// @brief Default constructor of class Span.
  /// @tparam U Void type of elements within the other Span.
  /// @tparam E The type of elements within the other Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613227
  template <typename U = void,
      typename E = typename std::enable_if<(Extent == dynamic_extent) || (Extent == 0), U>::type>
  constexpr Span() noexcept : mData_(nullptr), mExtent_(0)
  {
    xtrace("__ Span()\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }

  /// @brief Construct a new Span from the given pointer and size.
  /// @param[in] ptr A pointer to an object
  /// @param[in] count Number of elements in the sequence
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613191
  constexpr Span(pointer ptr, IndexType count) : mData_(ptr), mExtent_(count)
  {
    xtrace("__ Span(T*, size_t)\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    xtrace("____ ptr = ", ptr, ", count = ", count, " (ex = ", Extent, ")\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    if ((Extent != dynamic_extent) && (count != Extent))
    {
      assert(!"inconsistent stkExtent");
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    }
  }

  /// @brief Construct a new Span from the open range between firstElem, lastElem.
  /// @param[in] firstElem Pointer to the first element.
  /// @param[in] lastElem Pointer to past the last element.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613196
  constexpr Span(pointer firstElem, pointer lastElem) :
      Span(firstElem, static_cast<std::size_t>(lastElem - firstElem))
  {
    xtrace("__ Span(T*, T*)\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    assert(std::distance(firstElem, lastElem) >= 0);
  }

  /// @brief Construct a new Span from the given raw array.
  /// @tparam N The number of element in the sapn.
  /// @tparam E To contain the boolean value.
  /// @param[in] arr Array to construct a view.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613202
  template <std::size_t N,
      typename E = typename std::enable_if<((Extent == dynamic_extent) || (Extent == N))
          && std::is_convertible<typename std::remove_pointer<TypeOfData<ElementType (&)[N]>>::type (*)[],
                 ElementType (*)[]>::value>::type>
  constexpr Span(ElementType (&arr)[N]) noexcept : Span(std::addressof(arr[0]), N)
  {
    xtrace("__ Span(raw)\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Construct a new Span from the given Array.
  /// @tparam N The size of the Array.
  /// @tparam E To contain the value.
  /// @param[in] arr The size of the Array.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613203
  template <std::size_t N,
      typename E = typename std::enable_if<((Extent == dynamic_extent) || (Extent == N))
          && std::is_convertible<typename std::remove_pointer<decltype(
                                     std::declval<Array<value_type, N>>().data())>::type (*)[],
                 ElementType (*)[]>::value>::type>
  constexpr Span(Array<value_type, N>& arr) noexcept : Span(arr.data(), N)
  {
    xtrace("__ Span(Array&)\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Construct a new Span from the given const Array.
  /// @tparam N The size of the Array.
  /// @tparam E To contain the value.
  /// @param[in] arr The size of the Array.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613204
  template <std::size_t N,
      typename E = typename std::enable_if<((Extent == dynamic_extent) || (Extent == N))
          && std::is_convertible<typename std::remove_pointer<decltype(
                                     std::declval<Array<value_type, N> const>().data())>::type (*)[],
                 ElementType (*)[]>::value>::type>
  constexpr Span(Array<value_type, N> const& arr) noexcept : Span(arr.data(), N)
  {
    xtrace("__ Span(Array const&)\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Construct a new Span from the given container.
  /// @tparam Container The type of container tyoe.
  /// @tparam E To contain the element value.
  /// @param[in] cont Instance of the container.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613205
  template <typename Container,
      typename E = typename std::enable_if<
          internal::IsSuitableContainer<Container, Container&, ElementType>::value>::type>
  constexpr Span(Container& cont) : Span(core::data(cont), core::size(cont))
  {
    xtrace("__ Span(Container&)\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    assert(std::distance(core::data(cont), core::data(cont) + core::size(cont)) >= 0);
  }

  /// @brief Construct a new Span from the given const container.
  /// @tparam Container The type of container.
  /// @tparam E To contain the element value.
  /// @param[in] cont Instance of the container.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613206
  template <typename Container,
      typename E = typename std::enable_if<
          internal::IsSuitableContainer<Container, Container const&, ElementType>::value>::type>
  constexpr Span(Container const& cont) : Span(core::data(cont), core::size(cont))
  {
    xtrace("__ Span(Container const&)\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
    assert(std::distance(core::data(cont), core::data(cont) + core::size(cont)) >= 0);
  }

  /// @brief Copy construct a new Span from another instance.
  /// @param[in] other Instance of the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613207
  constexpr Span(Span const& other) noexcept = default;

  /// @brief Converting constructor.
  /// This ctor allows assignment of a cv-Span<> from a normal Span<>,
  /// and also of a dynamic_extent-Span<> from a static stkExtent-one.
  /// @tparam U The type of elements within the other Span.
  /// @tparam N The Extent of the other Span.
  /// @tparam E To contain the element value.
  /// @param[in] s Instance of the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613208
  template <typename U, std::size_t N,
      typename E = typename std::enable_if<((Extent == dynamic_extent) || (Extent == N))
          && std::is_convertible<U (*)[], ElementType (*)[]>::value>::type*>
  constexpr Span(Span<U, N> const& s) noexcept : Span(s.data(), s.size())
  {
    xtrace("__ converting ctor from N ", N, "\n");
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief The destructor for the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  virtual ~Span() noexcept = default;

  /// @brief Copy assignment operator for Span.
  /// @param[in] other Instance of the class Span.
  /// @returns Reference to Instance of the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  Span& operator=(Span const& other) & noexcept = default;

  /// @brief Return a subspan containing only the first elements of this Span.
  /// @tparam Count The number of elements to take over.
  /// @returns Instance of the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613209
  template <std::size_t Count>
  constexpr Span<ElementType, Count> first() const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    assert(Count <= size());
    return {mData_, Count};
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Return a subspan containing only the first elements of this Span.
  /// @param[in] count Number of elements in the sequence.
  /// @returns Instance of the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613210
  constexpr Span<ElementType, dynamic_extent> first(IndexType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    assert(count <= size());

    return {mData_, count};
  }

  /// @brief Return a subspan containing only the last elements of this Span.
  /// @tparam Count Number of elements in the sequence.
  /// @returns Instance of the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613211
  template <std::size_t Count>
  constexpr Span<ElementType, Count> last() const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    assert(Count <= size());

    return {mData_ + (size() - Count), Count};
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Return a subspan containing only the last elements of this Span.
  /// @param[in] count Number of elements in the sequence.
  /// @returns Instance of the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613212
  constexpr Span<ElementType, dynamic_extent> last(IndexType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    assert(count <= size());

    return {mData_ + (size() - count), count};
  }

  /// @brief Return a subspan of this Span.
  /// @tparam Offset Offset into this Span from which to start.
  /// @tparam Count The number of elements to take over.
  /// @returns Instance of the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613213
  template <std::size_t Offset, std::size_t Count = dynamic_extent>
  constexpr auto subspan() const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    assert(Offset <= size());
    assert((Count == dynamic_extent) || ((Offset + Count) <= size()));

    constexpr IndexType E{
        (Count != dynamic_extent) ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent)};

    return Span<ElementType, E>(mData_ + Offset, Count != dynamic_extent ? Count : size() - Offset);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Return a subspan of this Span.
  /// @param[in] offset Offset into this Span from which to start.
  /// @param[in] count The number of elements in the sequence.
  /// @returns Instance of the class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613214
  constexpr Span<ElementType, dynamic_extent> subspan(
      // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
      IndexType offset, IndexType count = dynamic_extent) const
  {
    assert(offset <= size());
    assert((count == dynamic_extent) || ((offset + count) <= size()));

    return {mData_ + offset, count == dynamic_extent ? size() - offset : count};
  }

  /// @brief Return the size of this Span.
  /// @returns The number of elements contained in this Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613215
  constexpr IndexType size() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mExtent_;
  }

  /// @brief Return the size of this Span in bytes.
  /// @returns The number of bytes covered by this Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613215
  constexpr IndexType size_bytes() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mExtent_ * sizeof(T);
  }

  /// @brief Return whether this Span is empty.
  /// @returns true if this Span contains 0 elements, false otherwise.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613216
  constexpr bool empty() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mExtent_ == 0;
  }

  /// @brief Return a Reference to the n-th element of this Span.
  /// @param[in] idx The index into this Span.
  /// @returns the index into this Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613217
  constexpr Reference operator[](IndexType idx) const
  {
    return mData_[idx];
  }

  /// @brief Return a pointer to the start of the memory block covered by this Span.
  /// @returns the pointer of class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613218
  constexpr pointer data() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mData_;
  }

  /// @brief Return an Iterator pointing to the first element of this Span.
  /// @returns the Iterator of class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613219
  constexpr Iterator begin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return &mData_[0];
  }

  /// @brief Return an Iterator pointing past the last element of this Span.
  /// @returns the Iterator of class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613220
  constexpr Iterator end() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return &mData_[mExtent_];
  }

  /// @brief Return a const_iterator pointing to the first element of this Span.
  /// @returns the const_iterator of class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613221
  constexpr const_iterator cbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return &mData_[0];
  }

  /// @brief Return a const_iterator pointing past the last element of this Span.
  /// @returns the const_iterator of class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613222
  constexpr const_iterator cend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return &mData_[mExtent_];
  }

  /// @brief Return a ReverseIterator pointing to the last element of this Span.
  /// @returns the ReverseIterator of class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613223
  constexpr ReverseIterator rbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ReverseIterator(end());
  }

  /// @brief Return a ReverseIterator pointing past the first element of this Span.
  /// @returns  the ReverseIterator of class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613224
  constexpr ReverseIterator rend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ReverseIterator(begin());
  }

  /// @brief Return a ConstReverseIterator pointing to the last element of this Span.
  /// @returns the ConstReverseIterator of class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613225
  constexpr ConstReverseIterator crbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ConstReverseIterator(end());
  }

  /// @brief Return a ConstReverseIterator pointing past the first element of this Span.
  /// @returns the ConstReverseIterator of class Span.
  /// @trace SWS_CORE_S7.2.4.3_P1_003
  /// @archtrace 609894
  /// @design 613226
  constexpr ConstReverseIterator crend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ConstReverseIterator(begin());
  }

 private:
  /// @brief To store the data into Span.
  pointer mData_;
  /// @brief To store the size into Span.
  IndexType mExtent_;
};
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type parameters
// shall be camel case.

#if (__cpp_deduction_guides - 0) >= 201703L
/// @brief Method declaration of Span.
/// @tparam T The type of elements in the Span.
/// @tparam N Number of the element in Span.
template <typename T, std::size_t N>
Span(T (&)[N])->Span<T, N>;

/// @brief Method declaration of Span.
/// @tparam T The type of elements in the Span.
/// @tparam N Number of the element in Span.
template <typename T, std::size_t N>
Span(Array<T, N>&)->Span<T, N>;

/// @brief Method declaration of Span.
/// @tparam T The type of elements in the Span.
/// @tparam N Number of the element in Span.
template <typename T, std::size_t N>
Span(Array<T, N> const&)->Span<T const, N>;

/// @brief Method declaration of Span.
/// @tparam Container The type of a Span.
template <typename Container>
Span(Container&)->Span<typename Container::value_type>;

/// @brief The method declaration of Span.
/// @tparam Container The type of Span.
template <typename Container>
Span(Container const&)->Span<const typename Container::value_type>;
#endif // C++17 deduction guides

/// @brief Return a read-only Span<Byte> over the object representation of the input Span<T>.
/// @tparam ElementType Must be a complete object type that is not an abstract class type
/// @tparam Extent The number of elements in the sequence.
/// @param[in] s Instance of the class Span.
/// @returns Instance of the class Span.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
/// @design 613190
template <typename ElementType, std::size_t Extent>
Span<Byte const, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent> as_bytes(
    //AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
    Span<ElementType, Extent> s) noexcept
{
  return {reinterpret_cast<Byte const*>(s.data()), s.size_bytes()};
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Return a writable Span over the object representation of the input Span<T>.
/// @tparam ElementType Must be a complete object type that is not an abstract class type
/// @tparam Extent The number of elements in the sequence.
/// @param[in] s Instance of the class Span.
/// @returns a Instance of the class Span.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
/// @design 613190
template <typename ElementType, std::size_t Extent>
Span<Byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent> as_writable_bytes(
    //AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
    Span<ElementType, Extent> s) noexcept
{
  return {reinterpret_cast<Byte*>(s.data()), s.size_bytes()};
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Create a new Span from the given pointer and size.
/// @tparam T The type of elements in the Span.
/// @param[in] ptr Pointer of the type T.
/// @param[in] count The number of elements to take from ptr.
/// @returns Instance of the class Span.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
/// @design 613190
template <typename T>
constexpr Span<T> MakeSpan(T* ptr, typename Span<T>::IndexType count)
{
  return Span<T>(ptr, count);
}

/// @brief Create a new Span from the open range between [firstElem, lastElem).
/// @tparam T The type of elements in the Span.
/// @param[in] firstElem Pointer to the first element.
/// @param[in] lastElem Pointer to past the last element.
/// @returns Instance of the class Span.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
/// @design 613190
template <typename T>
constexpr Span<T> MakeSpan(T* firstElem, T* lastElem)
{
  return Span<T>(firstElem, lastElem);
}

/// @brief Create a new Span from the given raw array.
/// @tparam T The type of elements in the Span.
/// @tparam N The size of the raw array.
/// @param[in] arr The size of the raw array.
/// @returns Instance of the class Span.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
/// @design 613190
template <typename T, std::size_t N>
constexpr Span<T, N> MakeSpan(T (&arr)[N]) noexcept
{
  return Span<T, N>(arr);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
// parameters shall be camel case.

/// @brief Create a new Span from the given container.
/// @tparam Container The type of class Span.
/// @param[in] cont Instance of the class Container .
/// @returns Instance of the class Span.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
/// @design 613190
template <typename Container>
constexpr Span<typename Container::value_type> MakeSpan(Container& cont)
{
  return Span<typename Container::value_type>(cont);
}

/// @brief Create a new Span from the given const container.
/// @tparam Container The type of container.
/// @param[in] cont Instance of the Container.
/// @returns Instance of the class Span.
/// @trace SWS_CORE_S7.2.4.3_P1_003
/// @archtrace 609894
/// @design 613190
template <typename Container>
constexpr Span<typename Container::value_type const> MakeSpan(Container const& cont)
{
  return Span<typename Container::value_type const>(cont);
}

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_SPAN_H_

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
//
// - A27.0.4
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A27.0.4 required
//   Description:         C-style strings should not be used.
//   Justification:       This will create another violation for initialization with C-style string.This
//                        has been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
