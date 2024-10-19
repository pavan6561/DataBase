// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : string_view.h
//
// Purpose     : This file provides the implementation of class string_view.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_STRING_VIEW_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_STRING_VIEW_H_

#include <string>
#include <limits>
#include <iterator>
#include <iostream>
#include <utility>
#include <limits>
#include <type_traits>
#include <stdexcept>
#include <cstddef>

#ifdef __GNUC__
#define APD_GCC_CONSTEXPR constexpr
#else
#define APD_GCC_CONSTEXPR
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
/// @brief This class represents a sequence of characters.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @trace SWS_CORE_S7.2.4.3_P1_002 SWS_CORE_S7.2.4.3_P1
/// @archtrace 609896
/// @design 611556 614039
template <typename CharT, typename Traits = std::char_traits<CharT>>
class basic_string_view
{
 public:
  /// @brief Alias type for a value type.
  using value_type = CharT;
  /// @brief Alias type for a constant Pointer type.
  using ConstPointer = CharT const*;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Alias type for a constant Reference type.
  using ConstReference = CharT const&;

  /// @brief Alias type for a constant Iterator type.
  using ConstIterator = CharT const*;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Alias type for a constant reverse Iterator type.
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  /// @brief Alias type for a size type.
  using SizeType = std::size_t;
  /// @brief Alias type for a Pointer difference type
  constexpr static SizeType stkNpos{SizeType(-1)};

  /// @brief Returns the length of the character sequence pointed to by s,
  /// that is, the position of the terminating null character (CharT()).
  /// @param[in] text Pointer to a character sequence to return length.
  /// @returns The length of character sequence pointed to by text.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611557
  APD_GCC_CONSTEXPR static SizeType length(CharT const* text) noexcept
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
#ifdef __GNUC__
    return __builtin_strlen(text);
#else
    return Traits::length(text);
#endif
  }

  /// @brief Compares the value of the basic_string object (or a substring) to the sequence
  /// of characters specified by its arguments.
  /// @param[in] s1 Number of characters of this string to compare
  /// @param[in] s2 Number of characters of the given string to compare.
  /// @param[in] n Pointer to the character string to compare.
  /// @returns negative value if this view is less than the other character sequence,
  /// zero if the both character sequences are equal, positive value if this view is greater
  /// than the other character sequence.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611558
  APD_GCC_CONSTEXPR static std::int32_t compare(CharT const* s1, CharT const* s2, size_t n) noexcept
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
#ifdef __GNUC__
    return __builtin_strncmp(s1, s2, n);
#else
    return Traits::compare(s1, s2, n);
#endif
  }

 public:
  /// @brief It is Construction and assignment.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611559
  constexpr basic_string_view() noexcept : basic_string_view(nullptr, 0)
  {
  }

  /// @brief Construction and assignment.
  /// @param[in] p A constant pointer i.e. The address will remain constant.
  /// @param[in] count The size of the string.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611560
  constexpr basic_string_view(ConstPointer p, SizeType count) : mPtr_(p), mSize_(count)
  {
  }
  /// @brief The default constructor for basic string.
  /// @param[in] p A constant pointer i.e. The address will remain constant.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611561
  constexpr basic_string_view(ConstPointer p) : basic_string_view(p, p == nullptr ? 0 : length(p))
  {
  }
  /// @brief Copy constructor of basic string.
  /// @param[in] other The string to copy.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  constexpr basic_string_view(basic_string_view const& other) noexcept = default;

  /// @brief Copy constructor of basic string.
  /// @param[in] other The string to view.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  basic_string_view& operator=(basic_string_view const& other) & noexcept = default;

  /// @brief Used to return an Iterator pointing to the first element.
  /// @returns Return a ConstIterator pointing to the first element of this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611562
  constexpr ConstIterator begin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mPtr_;
  }
  /// @brief Used to return an const Iterator pointing to the first element.
  /// @returns Return a ConstIterator pointing to the first element of this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611563
  constexpr ConstIterator cbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mPtr_;
  }
  /// @brief Used to return an Iterator pointing to the last element.
  /// @returns Return a ConstIterator pointing to the last element of this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611564
  constexpr ConstIterator end() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mPtr_ + mSize_;
  }
  /// @brief Used to return an const Iterator pointing to the last element.
  /// @returns Return a ConstIterator pointing to the last element of this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611565
  constexpr ConstIterator cend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mPtr_ + mSize_;
  }
  /// @brief Used to return an const Iterator pointing to the last element.
  /// @returns Return a ConstReverseIterator pointing to the last element of this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611566
  constexpr ConstReverseIterator rbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ConstReverseIterator(end());
  }
  /// @brief Used to return const Iterator pointing to the last element.
  /// @returns Return a ConstReverseIterator pointing to the last element of this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611567
  constexpr ConstReverseIterator crbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ConstReverseIterator(end());
  }
  /// @brief Used to return const Iterator pointing to the first element.
  /// @returns Return a ConstReverseIterator pointing to the first element of this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611568
  constexpr ConstReverseIterator rend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ConstReverseIterator(begin());
  }
  /// @brief Used to return const Iterator pointing to the first element.
  /// @returns Return a ConstReverseIterator pointing to the first element of this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611569
  constexpr ConstReverseIterator crend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return ConstReverseIterator(begin());
  }

  /// @brief Used to check the size of the String .
  /// @returns Return the number of elements in this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611570
  constexpr SizeType size() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mSize_;
  }
  /// @brief Used to check the length of the String .
  /// @returns Return the number of elements in this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611571
  constexpr SizeType length() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mSize_;
  }
  /// @brief Used to check the maximum size of the String .
  /// @returns Return the maximum number of elements supported by this String.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611572
  constexpr SizeType max_size() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    // TODO: not for wchar!
    return std::numeric_limits<SizeType>::max();
  }
  /// @brief Return whether this String is empty.
  /// @returns true if this String contains 0 elements, false otherwise.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611573
  constexpr bool empty() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mSize_ == 0;
  }

  /// @brief Returns a const Reference to the element at specified location pos.
  /// @param[in] pos Position of the first character in the compared string.
  /// @returns the ConstReference.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611574
  constexpr ConstReference operator[](SizeType pos) const
  {
    return mPtr_[pos];
  }

  /// @brief Returns a const Reference to the element at specified location pos.
  /// @param[in] pos Position of the first character in the compared string.
  /// @returns the ConstReference.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611575
  constexpr ConstReference at(SizeType pos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    if (pos >= mSize_)
    {
#ifndef ARA_NO_EXCEPTIONS
      throw std::out_of_range("out of range");
#else
      std::terminate();
#endif
    }
    return mPtr_[pos];
  }

  /// @brief Return a ConstReference to the first element of this String.
  /// @returns the Reference.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611576
  constexpr ConstReference front() const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mPtr_[0];
  }

  /// @brief Return a ConstReference to the last element of this String.
  /// @returns the Reference.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611577
  constexpr ConstReference back() const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mPtr_[mSize_ - 1];
  }

  /// @brief Return a ConstPointer to the first element of this String.
  /// @returns the ConstPointer.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611578
  constexpr ConstPointer data() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return mPtr_;
  }

  /// @brief Moves the start of the view forward by n characters.
  /// @param[in] n The position of element.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611579
  void remove_prefix(SizeType n)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    mPtr_ += n;
    mSize_ -= n;
  }
  /// @brief Moves the end of the view back by n characters.
  /// @param[in] n The position of element.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611580
  void remove_suffix(SizeType n)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    mSize_ -= n;
  }
  /// @brief Exchange the contents of this String with those of other.
  /// @param[in,out] other The another string to use as source to initialize the string with.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611581
  void swap(basic_string_view& other) noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    std::swap(mPtr_, other.mPtr_);
    std::swap(mSize_, other.mSize_);
  }

  /// @brief Exchange the contents of this String with those of other.
  /// @param[in] dest Pointer to the destination character string.
  /// @param[in] count Requested substring length.
  /// @param[in] pos Position of the first character.
  /// @returns Number of characters copied.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611582
  SizeType copy(CharT* dest, SizeType count, SizeType pos = 0) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    if (pos > mSize_)
    {
#ifndef ARA_NO_EXCEPTIONS
      throw std::out_of_range("out of range");
#else
      std::terminate();
#endif
    }

    size_t const rcount{std::min(count, mSize_ - pos)};
    std::copy(mPtr_ + pos, mPtr_ + pos + rcount, dest);
    return rcount;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Exchange the contents of this String with those of other.
  /// @param[in] pos Position of the first character.
  /// @param[in] count Requested length of the string.
  /// @returns Returns a newly constructed string object with its value
  /// initialized to a copy of a substring of this object.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611583
  constexpr basic_string_view substr(SizeType pos = 0, SizeType count = stkNpos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    if (pos > mSize_)
    {
#ifndef ARA_NO_EXCEPTIONS
      throw std::out_of_range("out of range");
#else
      std::terminate();
#endif
    }

    size_t const rcount{std::min(count, mSize_ - pos)};
    return basic_string_view(mPtr_ + pos, rcount);
  }
  /// @brief The function compares the two views by calling methods and returns a value.
  /// @param[in] v View to compare a basic string.
  /// @returns True if the basic_string_view is greater than other.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611584
  constexpr std::int32_t compare(basic_string_view v) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    std::int32_t ret{compare(mPtr_, v.mPtr_, std::min(mSize_, v.mSize_))};
    if (ret == 0)
    {
      return (mSize_ < v.mSize_) ? -1 : (mSize_ > v.mSize_);
    }
    return ret;
  }
  /// @brief Compares a substring of this string to str.
  /// @param[in] pos1 Position of the first character in this view to compare.
  /// @param[in] count1 Number of characters of this view to compare.
  /// @param[in] v View to compare a string.
  /// @returns True if the basic_string_view is greater than other.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611585
  constexpr std::int32_t compare(SizeType pos1, SizeType count1, basic_string_view v) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return substr(pos1, count1).compare(v);
  }
  /// @brief Compares two character sequences.
  /// @param[in] pos1 Position of the first character in this view to compare.
  /// @param[in] count1 Number of characters of this view to compare.
  /// @param[in] v View to compare a string.
  /// @param[in] pos2 Position of the second character in this view to compare.
  /// @param[in] count2 Number of characters of the given view to compare.
  /// @returns negative value if this view is less than the other character
  /// sequence, zero if the both character sequences are equal, positive
  /// value if this view is greater than the other character sequence.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611586
  constexpr std::int32_t compare(
      // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
      SizeType pos1, SizeType count1, basic_string_view v, SizeType pos2, SizeType count2) const
  {
    return substr(pos1, count1).compare(v.substr(pos2, count2));
  }

  /// @brief Compares two character sequences.
  /// @param[in] s Pointer to the character string to compare.
  /// @returns negative value if this view is less than the other character
  /// sequence, zero if the both character sequences are equal, positive
  /// value if this view is greater than the other character sequence.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611587
  constexpr std::int32_t compare(ConstPointer s) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return compare(basic_string_view(s));
  }
  /// @brief Compares two character sequences.
  /// @param[in] pos1 Position of the first character in this view to compare
  /// @param[in] count1 Number of characters of this view to compare
  /// @param[in] s Pointer to the character string to compare.
  /// @returns negative value if this view is less than the other character
  /// sequence, zero if the both character sequences are equal, positive
  /// value if this view is greater than the other character sequence.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611588
  constexpr std::int32_t compare(SizeType pos1, SizeType count1, ConstPointer s) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return substr(pos1, count1).compare(basic_string_view(s));
  }
  /// @brief Compares two character sequences.
  /// @param[in] pos1 Position of the first character in this view to compare
  /// @param[in] count1 Number of characters of this view to compare.
  /// @param[in] s Pointer to the character string to compare.
  /// @param[in] count2 Number of characters of the given view to compare.
  /// @returns negative value if this view is less than the other character
  /// sequence, zero if the both character sequences are equal, positive
  /// value if this view is greater than the other character sequence.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611589
  constexpr std::int32_t compare(SizeType pos1, SizeType count1, ConstPointer s, SizeType count2) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return substr(pos1, count1).compare(basic_string_view(s, count2));
  }

  /// @brief Finds the first occurrence of v in this view, starting at position pos.
  /// @param[in] v View to search for string.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or stkNpos if
  /// no such substring is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611590
  SizeType find(basic_string_view v, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find(v.data(), pos, v.size());
  }
  /// @brief Finds the first occurrence of ch in this character, starting at position pos.
  /// @param[in] ch Value to compare the elements.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or stkNpos if
  /// no such substring is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611591
  SizeType find(CharT ch, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find(ch, pos);
  }
  /// @brief Finds the first occurrence of s in this ConstPointer to a character string,
  /// starting at position pos at length of substring.
  /// @param[in] s Value to compare the elements to.
  /// @param[in] pos Position at which to start the search.
  /// @param[in] count Number of elements to examine.
  /// @returns Position of the first character of the found substring or stkNpos if
  /// no such substring is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611592
  SizeType find(ConstPointer s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find(s, pos, count);
  }
  /// @brief Finds the first occurrence of s in this ConstPointer to a character string,
  /// starting at position pos.
  /// @param[in] s Value to compare the elements.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or stkNpos if
  /// no such substring is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611593
  SizeType find(ConstPointer s, SizeType pos = 0) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find(s, pos, length(s));
  }

  /// @brief Finds the last occurrence of v in this view, starting at position pos.
  /// @param[in] v View to search for string.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or stkNpos if
  /// no such substring is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611594
  SizeType rfind(basic_string_view v, SizeType pos = stkNpos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return rfind(v.data(), pos, v.size());
  }
  /// @brief Finds the last occurrence of c in this character, starting at position pos.
  /// @param[in] c Pointer to a character string to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or stkNpos if
  /// no such substring is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611595
  SizeType rfind(CharT c, SizeType pos = stkNpos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).rfind(c, pos);
  }
  /// @brief Finds the last occurrence of s in this ConstPointer to a character string,
  /// starting at position pos at length of substring.
  /// @param[in] s Pointer to a character string to search for.
  /// @param[in] pos Position at which to start the search.
  /// @param[in] count Length of substring to search for.
  /// @returns Position of the first character of the found substring or stkNpos if
  /// no such substring is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611596
  SizeType rfind(ConstPointer s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).rfind(s, pos, count);
  }
  /// @brief Finds the last occurrence of s in this ConstPointer to a character string,
  /// starting at position pos.
  /// @param[in] s Pointer to a character string to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or stkNpos if
  /// no such substring is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611597
  SizeType rfind(ConstPointer s, SizeType pos = stkNpos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return rfind(s, pos, length(s));
  }

  /// @brief Finds the first occurrence of any of the characters of v in this view,
  /// starting at position pos.
  /// @param[in] v View to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns position of the basic_string_view.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611598
  SizeType find_first_of(basic_string_view v, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find_first_of(v.data(), pos, v.size());
  }
  /// @brief Finds the first occurrence of any of the characters of c in this character,
  /// starting at position pos.
  /// @param[in] c Character to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns position of the basic_string_view.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611599
  SizeType find_first_of(CharT c, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find_first_of(c, pos);
  }
  /// @brief Finds the first occurrence of any of the characters of s in this ConstPointer to a string of
  /// characters, starting at position pos at length of the string of characters.
  /// @param[in] s Pointer to a string of characters to search for.
  /// @param[in] pos Position at which to start the search.
  /// @param[in] count Length of the string of characters to search for.
  /// @returns position of the basic_string_view.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611600
  SizeType find_first_of(ConstPointer s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find_first_of(s, pos, count);
  }
  /// @brief Finds the first occurrence of any of the characters of s in this ConstPointer to a character
  /// string,starting at position pos.
  /// @param[in] s Pointer to a string of characters to search for.
  /// @param[in] pos Position of elements to examine.
  /// @returns position of the basic_string_view.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611601
  SizeType find_first_of(ConstPointer s, SizeType pos = 0) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find_first_of(s, pos, length(s));
  }

  /// @brief Finds the last character equal to one of characters in the given character sequence.
  /// The exact search algorithm is not specified.
  /// @param[in] v String identifying characters to search for.
  /// @param[in] pos Position at which the search is to finish.
  /// @returns Position of the found character or stkNpos if no such character is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611602
  SizeType find_last_of(basic_string_view v, SizeType pos = stkNpos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find_last_of(v.data(), pos, v.size());
  }
  /// @brief Finds the last character equal to one of characters in the given character sequence.
  /// The exact search algorithm is not specified.
  /// @param[in] c String identifying characters to search for.
  /// @param[in] pos Position at which the search is to finish.
  /// @returns Position of the found character or stkNpos if no such character is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611603
  SizeType find_last_of(CharT c, SizeType pos = stkNpos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find_last_of(c, pos);
  }
  /// @brief Finds the last character equal to one of characters in the given character sequence.
  /// The exact search algorithm is not specified.
  /// @param[in] s Pointer to an array of characters.
  /// @param[in] pos Position at which the search is to finish.
  /// @param[in] count Length of character string identifying characters to search for.
  /// @returns Position of the found character or stkNpos if no such character is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611604
  SizeType find_last_of(ConstPointer s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find_last_of(s, pos, count);
  }
  /// @brief Finds the last character equal to one of characters in the given character sequence.
  /// The exact search algorithm is not specified.
  /// @param[in] s Pointer to an array of characters.
  /// @param[in] pos Position at which the search is to finish.
  /// @returns Position of the found character or stkNpos if no such character is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611605
  SizeType find_last_of(ConstPointer s, SizeType pos = stkNpos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find_last_of(s, pos, length(s));
  }

  /// @brief Finds the first character equal to none of the characters in the given character sequence.
  /// @param[in] v String identifying characters to search for.
  /// @param[in] pos Position for the search to start from.
  /// @returns Position of the found character or stkNpos if no such character is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611606
  SizeType find_first_not_of(basic_string_view v, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find_first_not_of(v.data(), pos, v.size());
  }
  /// @brief Finds the first character equal to none of the characters in the given character sequence.
  /// @param[in] c Individual character to be searched for.
  /// @param[in] pos Position for the search to start from.
  /// @returns Position of the found character or stkNpos if no such character is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611607
  SizeType find_first_not_of(CharT c, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find_first_not_of(c, pos);
  }
  /// @brief Finds the first character equal to none of the characters in the given character sequence.
  /// @param[in] s Pointer to an array of characters.
  /// @param[in] pos Position for the search to start from.
  /// @param[in] count Length of character string identifying characters to search for.
  /// @returns Position of the found character or stkNpos if no such character is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611608
  SizeType find_first_not_of(ConstPointer s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find_first_not_of(s, pos, count);
  }
  /// @brief Finds the first character equal to none of the characters in the given character sequence.
  /// @param[in] s Pointer to an array of characters.
  /// @param[in] pos Position for the search to start from.
  /// @returns Position of the found character or stkNpos if no such character is found.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611609
  SizeType find_first_not_of(ConstPointer s, SizeType pos = 0) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find_first_not_of(s, pos, length(s));
  }

  /// @brief Finds the last character equal to none of the characters in the given character sequence.
  /// @param[in] v View to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns true if the Arrays are equal, false otherwise.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611610
  SizeType find_last_not_of(basic_string_view v, SizeType pos = stkNpos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find_last_not_of(v.data(), pos, v.size());
  }
  /// @brief Finds the last character equal to none of the characters in the given character sequence.
  /// @param[in] c Individual character to be searched for.
  /// @param[in] pos Position at which to start the search.
  /// @returns true if the Arrays are equal, false otherwise.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611611
  SizeType find_last_not_of(CharT c, SizeType pos = stkNpos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find_last_not_of(c, pos);
  }
  /// @brief Finds the last character equal to none of the characters in the given character sequence.
  /// @param[in] s Pointer to a string of characters to compare.
  /// @param[in] pos Position at which to start the search.
  /// @param[in] count Length of the string of characters to compare.
  /// @returns true if the Arrays are equal, false otherwise.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611612
  SizeType find_last_not_of(ConstPointer s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return std::basic_string<CharT, Traits>(mPtr_, mSize_).find_last_not_of(s, pos, count);
  }
  /// @brief Finds the last character equal to none of the characters in the given character sequence.
  /// @param[in] s Pointer to a string of characters to compare.
  /// @param[in] pos Position at which to start the search.
  /// @returns true if the Arrays are equal, false otherwise.
  /// @trace SWS_CORE_S7.2.4.3_P1_002
  /// @archtrace 609896
  /// @design 611613
  SizeType find_last_not_of(ConstPointer s, SizeType pos = stkNpos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return find_last_not_of(s, pos, length(s));
  }

 private:
  /// @brief Constant char Pointer to store the string address.
  CharT const* mPtr_;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Variable to store the size.
  SizeType mSize_;
};

/// @brief Inserters and extractors operator.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] os Pointer to the stream buffer to read the data from.
/// @param[in] v Integer, floating-point, boolean, or Pointer value to insert.
/// @returns inserters and extractors.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611921
template <typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, basic_string_view<CharT, Traits> const& v)
{
  typename std::basic_ostream<CharT, Traits>::sentry s{os};
  if (s)
  {
    // In case the argument's width does not fit into a 'streamsize', truncate it.
    std::size_t const maxSize{static_cast<std::size_t>(std::numeric_limits<std::streamsize>::max())};
    std::size_t const outputSize{(v.size() <= maxSize) ? v.size() : maxSize};

    std::streamsize const vsize{static_cast<std::streamsize>(outputSize)};
    std::streamsize const width{os.width()};
    if (width > vsize)
    {
      // Stream width is wider than text - must left- or right-align.
      auto fill = [&os](std::streamsize num) {
        CharT const ch{os.fill()};
        auto rdbuf{os.rdbuf()};
        while (num--)
        {
          std::ostream::int_type const ret{rdbuf->sputc(ch)};
          if (Traits::eq_int_type(ret, Traits::eof()))
          {
            os.setstate(std::ios_base::badbit);
            break;
          }
        }
      };

      bool const isLeft{((os.flags() & std::ios_base::adjustfield) == std::ios_base::left)};
      if (!isLeft)
      {
        fill(width - vsize);
      }

      if (os.good())
      {
        os.write(v.data(), vsize);
      }

      if ((isLeft) && (os.good()))
      {
        fill(width - vsize);
      }
    }
    else
    {
      // Text is as long or longer than the defined stream width - just output the whole thing.
      os.write(v.data(), vsize);
    }
  }
  return os;
}

// 24.4.3, non-member comparison functions,
// incl. additional overloads

/// @brief Helper type for forcing template argument deduction to proceed based on
/// only one of the arguments to a comparison operator. See n3766 for details.
/// @tparam T The type of element in the String.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
template <typename T>
using Identity = typename std::decay<T>::type;

/// @brief Return true if the contents of lhs is equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611922
template <typename CharT, typename Traits>
constexpr bool operator==(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) == 0;
}
/// @brief Return true if the contents of lhs is equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611923
template <typename CharT, typename Traits>
constexpr bool operator==(
    basic_string_view<CharT, Traits> lhs, Identity<basic_string_view<CharT, Traits>> rhs) noexcept
{
  return lhs.compare(rhs) == 0;
}
/// @brief Return true if the contents of lhs is equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611924
template <typename CharT, typename Traits>
constexpr bool operator==(
    Identity<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) == 0;
}

/// @brief Return true if the contents of lhs is not equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is not equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611925
template <typename CharT, typename Traits>
constexpr bool operator!=(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) != 0;
}
/// @brief Return true if the contents of lhs is not equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is not equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611926
template <typename CharT, typename Traits>
constexpr bool operator!=(
    basic_string_view<CharT, Traits> lhs, Identity<basic_string_view<CharT, Traits>> rhs) noexcept
{
  return lhs.compare(rhs) != 0;
}
/// @brief Return true if the contents of lhs is not equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is not equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611927
template <typename CharT, typename Traits>
constexpr bool operator!=(
    Identity<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) != 0;
}

/// @brief Return true if the contents of lhs is less than the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is less than rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611928
template <typename CharT, typename Traits>
constexpr bool operator<(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) < 0;
}
/// @brief Return true if the contents of lhs is less than the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is less than rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611929
template <typename CharT, typename Traits>
constexpr bool operator<(
    basic_string_view<CharT, Traits> lhs, Identity<basic_string_view<CharT, Traits>> rhs) noexcept
{
  return lhs.compare(rhs) < 0;
}
/// @brief Return true if the contents of lhs is less than the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is less than rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611930
template <typename CharT, typename Traits>
constexpr bool operator<(
    Identity<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) < 0;
}

/// @brief Return true if the contents of lhs is greater than the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if rhs is less than lhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611931
template <typename CharT, typename Traits>
constexpr bool operator>(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) > 0;
}
/// @brief Return true if the contents of lhs is greater than the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if rhs is less than lhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611932
template <typename CharT, typename Traits>
constexpr bool operator>(
    basic_string_view<CharT, Traits> lhs, Identity<basic_string_view<CharT, Traits>> rhs) noexcept
{
  return lhs.compare(rhs) > 0;
}
/// @brief Return true if the contents of lhs is greater than the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if rhs is less than lhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611933
template <typename CharT, typename Traits>
constexpr bool operator>(
    Identity<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) > 0;
}

/// @brief Return true if the contents of lhs is less than or equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is less than or equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611934
template <typename CharT, typename Traits>
constexpr bool operator<=(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) <= 0;
}
/// @brief Return true if the contents of lhs is less than or equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is less than or equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611935
template <typename CharT, typename Traits>
constexpr bool operator<=(
    basic_string_view<CharT, Traits> lhs, Identity<basic_string_view<CharT, Traits>> rhs) noexcept
{
  return lhs.compare(rhs) <= 0;
}
/// @brief Return true if the contents of lhs is less than or equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is less than or equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611936
template <typename CharT, typename Traits>
constexpr bool operator<=(
    Identity<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) <= 0;
}

/// @brief Return true if the contents of lhs is greater than or equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is greater than or equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611937
template <typename CharT, typename Traits>
constexpr bool operator>=(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) >= 0;
}
/// @brief Return true if the contents of lhs is greater than or equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is greater than or equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611938
template <typename CharT, typename Traits>
constexpr bool operator>=(
    basic_string_view<CharT, Traits> lhs, Identity<basic_string_view<CharT, Traits>> rhs) noexcept
{
  return lhs.compare(rhs) >= 0;
}
/// @brief Return true if the contents of lhs is greater than or equal to the contents of rhs.
/// @tparam CharT The char data type is used to store a character.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @param[in] lhs The left-hand side of the comparison.
/// @param[in] rhs The right-hand side of the comparison.
/// @returns true if lhs is greater than or equal to rhs, false otherwise.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
/// @design 611939
template <typename CharT, typename Traits>
constexpr bool operator>=(
    Identity<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept
{
  return lhs.compare(rhs) >= 0;
}

} // namespace internal

/// @brief A read-only view over a contiguous sequence of characters.
/// @trace SWS_CORE_S7.2.4.3_P1_002
/// @archtrace 609896
using StringView = internal::basic_string_view<char>;

} // namespace core
} // namespace ara
#undef APD_GCC_CONSTEXPR
#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_STRING_VIEW_H_

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
// - A27.0.4
//   Guideline:           AUTOSAR CPP14 Guidelines, AutosarC++19_03-A27.0.4 required
//   Description:         C-style strings should not be used.
//   Justification:       This will create another violation for initialization with C-style string.This
//                        has been analyzed and verified manually. There is no impact on code quality.
//   Risk:                None
//   Verification Method: Code Review
// -----------------------------------------------------------------------------------------------------------
