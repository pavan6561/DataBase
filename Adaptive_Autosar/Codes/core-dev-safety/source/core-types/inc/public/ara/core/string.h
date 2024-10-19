// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : string.h
//
// Purpose     : This file provides the implementation of class string.
// -----------------------------------------------------------------------------------------------------------

/// @file
/// @brief Interface to class ara::core::String

#ifndef APD_ARA_CORE_STRING_H_
#define APD_ARA_CORE_STRING_H_

#include "ara/core/string_view.h"
#include "ara/core/polymorphic_allocator.h"

#include <string>
#include <type_traits>
#include <functional>
#include <iostream>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief Internal
namespace internal
{
/// @brief This class represents a sequence of characters.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611645 614039
template <typename CharT, typename Traits = std::char_traits<CharT>,
    typename Allocator = std::allocator<CharT>>
class basic_string
{
  /// @brief Alias name for basic string.
  using InternalType = std::basic_string<CharT, Traits, Allocator>;
  friend struct std::hash<basic_string<CharT, Traits, Allocator>>;
  /// @brief created the internal type.
  InternalType value_;

 public:
  /// @brief Alias type for a size type.
  using SizeType = typename InternalType::size_type;
  /// @brief Alias type for a allocator type.
  /// @brief Alias type for a value type.
  using value_type = typename InternalType::value_type;
  /// @brief allocator_type type for AllocatorType.
  using AllocatorType = typename InternalType::allocator_type;
  /// @brief Alias type for a Iterator type.
  using Iterator = typename InternalType::iterator;
  /// @brief Alias type for a constant Iterator type.
  using ConstIterator = typename InternalType::const_iterator;
  /// @brief Alias type for a reverse Iterator type.
  using ReverseIterator = typename InternalType::reverse_iterator;
  /// @brief Alias type for a constant reverse Iterator type.
  using ConstReverseIterator = typename InternalType::const_reverse_iterator;

  static const SizeType npos{InternalType::npos};
  /// @brief Construct an empty string using allocator.
  /// @param[in] a The instance of an allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611646
  explicit basic_string(Allocator const& a = Allocator()) : value_(a)
  {
  }

  /// @brief The copy construct for string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611647
  basic_string(InternalType const& other) : value_(other)
  {
  }
  /// @brief The move construct for string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611648
  basic_string(InternalType&& other) noexcept : value_(std::move(other))
  {
  }

  /// @brief Standard copy constructors for string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611649
  basic_string(basic_string const& other) : value_(other.value_)
  {
  }
  /// @brief Standard copy constructors for string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @param[in] a The instance of Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611650
  basic_string(basic_string const& other, Allocator const& a) : value_(other.value_, a)
  {
  }
  /// @brief Standard move constructors for string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611651
  basic_string(basic_string&& other) noexcept : value_(std::move(other.value_))
  {
  }
  /// @brief Standard move constructors for string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @param[in] a The instance of Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611652
  basic_string(basic_string&& other, Allocator const& a) : value_(std::move(other.value_), a)
  {
  }
  /// @brief Standard copy constructors for string.
  /// @tparam T Data type of argument.
  /// @param[in] t Object to initialize the string with.
  /// @param[in] pos Position of the first character to include.
  /// @param[in] n Size of the resulting string.
  /// @param[in] a The instance of Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611653
  template <typename T>
  basic_string(T const& t, SizeType pos, SizeType n, Allocator const& a = Allocator()) :
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
      basic_string(basic_string_view<CharT, Traits>(t).substr(pos, n), a)
  {
  }

  /// @brief Constructs the string with count copies of character ch.
  /// @param[in] count Size of the resulting string.
  /// @param[in] ch Value to initialize the string with.
  /// @param[in] a The instance of Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611654
  basic_string(SizeType count, CharT ch, Allocator const& a = Allocator()) : value_(count, ch, a)
  {
  }

  /// @brief Constructs the string with a substring of other.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @param[in] pos Position of the first character to include.
  /// @param[in] count Size of the resulting string.
  /// @param[in] a The instance of Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611655
  basic_string(
      basic_string const& other, SizeType pos, SizeType count = npos, Allocator const& a = Allocator()) :
      value_(other.value_, pos, count, a)
  {
  }

  /// @brief Exchange the contents of this String with those of other.
  /// @param[in] s Pointer to an array of characters to use as source to initialize the string with.
  /// @param[in] count Size of the resulting string.
  /// @param[in] a The instance of Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611656
  basic_string(CharT const* s, SizeType count, Allocator const& a = Allocator()) : value_(s, count, a)
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
  }

  /// @brief  Construct string as copy of a C string.
  /// @param[in] s Pointer to an array of characters to use as source to initialize the string with.
  /// @param[in] a The instance of Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611657
  basic_string(CharT const* s, Allocator const& a = Allocator()) : value_(s, a)
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
  }

  /// @brief Construct string as copy of a range.
  /// @tparam InputIt It is an input string to an Iterator.
  /// @param[in] first Start of the range.
  /// @param[in] last End of the range.
  /// @param[in] a The instance of Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611658
  template <class InputIt>
  basic_string(InputIt first, InputIt last, Allocator const& a = Allocator()) : value_(first, last, a)
  {
  }

  /// @brief Construct string from an initializer list.
  /// @param[in] ilist The initializer list of characters.
  /// @param[in] a The instance of the Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611659
  basic_string(std::initializer_list<CharT> ilist, Allocator const& a = Allocator()) : value_(ilist, a)
  {
  }

  /// @brief Construct an empty string using allocator a.
  /// @param[in] sv An object convertible to string_view containing characters to locate.
  /// @param[in] a The instance of Allocator.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611660
  explicit basic_string(basic_string_view<CharT, Traits> sv, Allocator const& a = Allocator()) :
      value_(sv.data(), sv.size(), a)
  {
  }

  /// @brief Assign the value of str to this string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611661
  basic_string& operator=(basic_string const& other) &
  {
    basic_string tmp{other};
    assign(tmp);
    return *this;
  }

  /// @brief Copy constructor of basic string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611662
  basic_string& operator=(basic_string&& other) & noexcept
  {
    basic_string tmp{other};
    assign(std::move(tmp));
    return *this;
  }

  /// @brief Assign the value of other to this string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611663
  basic_string& operator=(InternalType const& other) &
  {
    assign(other);
    return *this;
  }

  /// @brief Move assign the value of other to this string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611664
  basic_string& operator=(InternalType&& other) &
  {
    assign(std::move(other));
    return *this;
  }

  /// @brief Replaces the contents with those of null-terminated character string pointed to by s.
  /// @param[in] s Pointer to an array of characters to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611665
  basic_string& operator=(CharT const* s) &
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    assign(s);
    return *this;
  }

  /// @brief Replaces the contents with character ch as if by assign(std::addressof(ch), 1)
  /// @param[in] c Value to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611666
  basic_string& operator=(CharT c) &
  {
    assign(1, c);
    return *this;
  }

  /// @brief Replaces the contents with those of the initializer list ilist as if by
  /// assign(ilist.begin(), ilist.size()).
  /// @param[in] ilist The initializer list to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611667
  basic_string& operator=(std::initializer_list<CharT> ilist) &
  {
    assign(ilist);
    return *this;
  }

  /// @brief A string view representing the entire contents of the string.
  /// @returns A string view representing the entire contents of the string.
  /// @param[in] sv An object convertible to string_view containing characters to locate.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611668
  basic_string& operator=(basic_string_view<CharT, Traits> sv) &
  {
    return assign(sv);
  }

  /// @brief Append a string to this string.
  /// @param[in] other The string to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611669
  basic_string& operator+=(basic_string const& other) &
  {
    return append(other);
  }

  /// @brief Append a string to this string.
  /// @param[in] other The string to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611670
  basic_string& operator+=(InternalType const& other) &
  {
    return value_.append(other);
  }

  /// @brief Appends a character c.
  /// @param[in] c The character value to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611671
  basic_string& operator+=(CharT c) &
  {
    return append(static_cast<SizeType>(1), c);
  }

  /// @brief Appends the null-terminated character string pointed to by s.
  /// @param[in] s Pointer to a null-terminated character string to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611672
  basic_string& operator+=(CharT const* s) &
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  {
    return append(s);
  }

  /// @brief Append an initializer_list of characters.
  /// @param[in] ilist The initializer list with the characters to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611673
  basic_string& operator+=(std::initializer_list<CharT> ilist) &
  {
    return append(ilist);
  }

  /// @brief Implicitly converts to a string view sv.
  /// @param[in] sv An object convertible to string_view containing characters to locate.
  /// @returns A string view representing the entire contents of the string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611674
  basic_string& operator+=(basic_string_view<CharT, Traits> sv) &
  {
    return append(sv);
  }
  /// @brief Append the multiple characters.
  /// @param[in] count Number of characters to append.
  /// @param[in] c The character value to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611675
  basic_string& append(SizeType count, CharT c)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(count, c);
    return *this;
  }

  /// @brief Append a string to this string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611676
  basic_string& append(basic_string const& other)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(other.value_);
    return *this;
  }

  /// @brief Append a substring.
  /// @param[in] count Number of characters to append.
  /// @param[in] pos The index of the first character to append.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611677
  basic_string& append(basic_string const& other, SizeType pos, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(other.value_, pos, count);
    return *this;
  }

  /// @brief Append a string to this string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611678
  basic_string& append(InternalType const& other)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(other);
    return *this;
  }

  /// @brief Append a substring to string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @param[in] pos The index of the first character to append.
  /// @param[in] count Number of characters to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611679
  basic_string& append(InternalType const& other, SizeType pos, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(other, pos, count);
    return *this;
  }

  /// @brief Append a C substring to string.
  /// @param[in] s The Pointer to the character string to append.
  /// @param[in] pos The index of the first character to append.
  /// @param[in] count Number of characters to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611680
  basic_string& append(CharT const* s, SizeType pos, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(s + pos, count);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Append a C substring to string.
  /// @param[in] s The Pointer to the character string to append.
  /// @param[in] count Number of characters to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611681
  basic_string& append(CharT const* s, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(s, count);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Append a C string.
  /// @param[in] s The Pointer to the character string to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611682
  basic_string& append(CharT const* s)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(s);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Append a range of characters.
  /// @tparam InputIt It is an input string to an Iterator.
  /// @param[in] first Iterator referencing the first character to append.
  /// @param[in] last Iterator marking the end of the range.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611683
  template <class InputIt>
  basic_string& append(InputIt first, InputIt last)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(first, last);
    return *this;
  }

  /// @brief Append an initializer_list of characters.
  /// @param[in] ilist The initializer list of characters to append.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611684
  basic_string& append(std::initializer_list<CharT> ilist)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(ilist);
    return *this;
  }

  /// @brief Append a string to this string.
  /// @param[in] sv An object convertible to string_view containing characters to locate.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611685
  basic_string& append(basic_string_view<CharT, Traits> sv)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.append(sv.data(), sv.size());
    return *this;
  }

  /// @brief Append a range of characters from a string_view.
  /// @tparam T Data type of argument.
  /// @tparam E It is designed to be used to initialize default constructor or
  /// function parameter at compile time.
  /// @param[in] t Object convertible to basic_string_view with the characters to append
  /// @param[in] pos The position in the string_view to append from.
  /// @param[in] n The number of characters to append from the string_view.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611686
  template <typename T,
      typename E = typename std::enable_if<!std::is_convertible<T const&, CharT const*>::value>::type>
  basic_string& append(T const& t, SizeType pos, SizeType n = npos)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    // TODO: out_of_range if pos > sv.size()
    basic_string_view<CharT, Traits> const sv{t};
    SizeType const rlen{std::min(n, SizeType(sv.size() - pos))};
    value_.append(sv.data() + pos, rlen);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Set value to multiple characters.
  /// @param[in] count Size of the resulting string.
  /// @param[in] c Value to initialize characters of the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611687
  basic_string& assign(SizeType count, CharT c)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(count, c);
    return *this;
  }

  /// @brief Set value to contents of another string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611688
  basic_string& assign(basic_string const& other)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(other.value_);
    return *this;
  }

  /// @brief Set value to a substring of a string.
  /// @param[in] count Size of the resulting string.
  /// @param[in] pos Index of the first character to take.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611689
  basic_string& assign(basic_string const& other, SizeType pos, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(other.value_, pos, count);
    return *this;
  }

  /// @brief Set value to contents of another string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611690
  basic_string& assign(basic_string&& other)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(std::move(other.value_));
    return *this;
  }

  /// @brief Set value to contents of another string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611691
  basic_string& assign(InternalType const& other)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(other);
    return *this;
  }

  /// @brief Set value to a substring of a string.
  /// @param[in] count Size of the resulting string.
  /// @param[in] pos Index of the first character to take.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611692
  basic_string& assign(InternalType const& other, SizeType pos, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(other, pos, count);
    return *this;
  }

  /// @brief Set value to contents of another string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611693
  basic_string& assign(InternalType&& other)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(std::move(other));
    return *this;
  }

  /// @brief Set value to a C substring.
  /// @param[in] s Pointer to a character string to use as source to initialize the string with.
  /// @param[in] pos Index of the first character to take.
  /// @param[in] count Size of the resulting string.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611694
  basic_string& assign(CharT const* s, SizeType pos, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(s + pos, count);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Set value to a C substring.
  /// @param[in] s Pointer to a character string to use as source to initialize the string with.
  /// @param[in] count Size of the resulting string.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611695
  basic_string& assign(CharT const* s, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(s, count);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Set value to contents of a C string.
  /// @param[in] s Pointer to a character string to use as source to initialize the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611696
  basic_string& assign(CharT const* s)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(s);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Set value to a range of characters.
  /// @tparam InputIt It is an input string to an Iterator.
  /// @param[in] first Iterator referencing the first character to append.
  /// @param[in] last Iterator marking the end of the range.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611697
  template <class InputIt>
  basic_string& assign(InputIt first, InputIt last)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(first, last);
    return *this;
  }

  /// @brief Set value to an initializer_list of characters.
  /// @param[in] ilist To initialize the characters of the string with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611698
  basic_string& assign(std::initializer_list<CharT> ilist)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(ilist);
    return *this;
  }
  /// @brief Set value to a C substring.
  /// @param[in] sv An object convertible to string_view containing characters to locate.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611699
  basic_string& assign(basic_string_view<CharT, Traits> sv)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.assign(sv.data(), sv.size());
    return *this;
  }

  /// @brief Set value to a substring of a string.
  /// @tparam T Data type of argument.
  /// @tparam E It is designed to be used to initialize default constructor or
  /// function parameter at compile time.
  /// @param[in] t Object to initialize the characters of the string with.
  /// @param[in] pos Index of the first character to take
  /// @param[in] n Number of characters to use.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611700
  template <typename T,
      typename E
      = typename std::enable_if<(std::is_convertible<T const&, basic_string_view<CharT, Traits>>::value)
          && (!std::is_convertible<T const&, CharT const*>::value)>::type>
  basic_string& assign(T const& t, SizeType pos, SizeType n = npos)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    // TODO: out_of_range if pos > sv.size()
    basic_string_view<CharT, Traits> const sv{t};
    SizeType const rlen{std::min(n, SizeType(value_.size() - pos))};
    value_.assign(sv.data() + pos, rlen);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Inserts count copies of character ch at the position index.
  /// @param[in] index Position at which the content will be inserted.
  /// @param[in] count Number of characters to insert.
  /// @param[in] ch The character to insert.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611701
  basic_string& insert(SizeType index, SizeType count, CharT ch)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.insert(index, count, ch);
    return *this;
  }
  /// @brief To insert a C string.
  /// @param[in] index Position at which the content will be inserted.
  /// @param[in] s Pointer to the character string to insert.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611702
  basic_string& insert(SizeType index, CharT const* s)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.insert(index, s);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief To insert multiple characters.
  /// @param[in] index Position at which the content will be inserted.
  /// @param[in] s Pointer to the character string to insert.
  /// @param[in] count Number of characters to insert.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611703
  basic_string& insert(SizeType index, CharT const* s, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.insert(index, s, count);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief To insert multiple characters.
  /// @param[in] index Position at which the content will be inserted.
  /// @param[in] other It is a Source string.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611704
  basic_string& insert(SizeType index, basic_string const& other)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.insert(index, other.value_);
    return *this;
  }

  /// @brief To insert multiple characters.
  /// @param[in] index Position at which the content will be inserted.
  /// @param[in] index_str The string to insert.
  /// @param[in] count Number of characters to insert.
  /// @param[in] other It is a Source string.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611705
  basic_string& insert(SizeType index, basic_string const& other, SizeType index_str, SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.insert(index, other.value_, index_str, count);
    return *this;
  }

  /// @brief To insert one character.
  /// @param[in] pos Iterator before which the characters will be inserted.
  /// @param[in] ch  The character to insert.
  /// @returns Iterator referencing newly inserted char.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611706
  Iterator insert(ConstIterator pos, CharT ch)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.insert(pos, ch);
  }

  /// @brief To insert multiple characters.
  /// @param[in] pos Iterator before which the characters will be inserted.
  /// @param[in] count Number of characters to insert.
  /// @param[in] ch  The character to insert.
  /// @returns An Iterator which refers to the copy of the first inserted character
  /// or pos if no characters were inserted.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611707
  Iterator insert(ConstIterator pos, SizeType count, CharT ch)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.insert(pos, count, ch);
  }

  /// @brief Inserts elements from range (first, last) before pos.
  /// @tparam InputIt It is an input string to an Iterator.
  /// @param[in] pos Iterator before which the characters will be inserted
  /// @param[in] first The range of elements to insert at start.
  /// @param[in] last The range of elements to insert at last.
  /// @returns Iterator pointing to the first element inserted.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611708
  template <class InputIt>
  Iterator insert(ConstIterator pos, InputIt first, InputIt last)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.insert(pos, first, last);
  }

  /// @brief Insert an initializer_list of characters.
  /// @param[in] pos Iterator before which the characters will be inserted.
  /// @param[in] ilist To insert the characters from.
  /// @returns Iterator pointing to the list of characters inserted.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611709
  Iterator insert(ConstIterator pos, std::initializer_list<CharT> ilist)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.insert(pos, ilist);
  }

  /// @brief Insert value of a string.
  /// @param[in] pos Iterator before which the characters will be inserted.
  /// @param[in] sv Pointer to the character string to insert.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611710
  basic_string& insert(SizeType pos, basic_string_view<CharT, Traits> sv)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.insert(pos, sv.data(), sv.size());
    return *this;
  }

  /// @brief To insert a substring.
  /// @tparam T Data type of argument.
  /// @tparam E It is designed to be used to initialize default constructor or
  /// function parameter at compile time.
  /// @param[in] pos1 Position in string to insert at.
  /// @param[in] t Object to insert the characters from.
  /// @param[in] pos2 Start of characters in str to insert.
  /// @param[in] n Number of characters to insert.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611711
  template <typename T,
      typename E
      = typename std::enable_if<(std::is_convertible<T const&, basic_string_view<CharT, Traits>>::value)
          && (!std::is_convertible<T const&, CharT const*>::value)>::type>
  basic_string& insert(SizeType pos1, T const& t, SizeType pos2, SizeType n = npos)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    // TODO: out_of_range if pos1 > size() or pos2 > sv.size()
    basic_string_view<CharT, Traits> sv{t};
    SizeType const rlen{std::min(n, SizeType(value_.size() - pos2))};
    value_.insert(pos1, sv.data() + pos2, rlen);
    return *this;
  }

  /// @brief Removes characters starting at index.
  /// @param[in] index First characters to remove.
  /// @param[in] count Number of characters to remove.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611712
  basic_string& erase(SizeType index = 0, SizeType count = npos)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.erase(index, count);
    return *this;
  }

  /// @brief Removes the character at position.
  /// @param[in] position Iterator to the character to remove.
  /// @returns Iterator pointing to the character immediately following the character erased.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611713
  Iterator erase(ConstIterator position)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.erase(position);
  }

  /// @brief Removes the characters in the range.
  /// @param[in] first Iterator referencing the first character to remove.
  /// @param[in] last Iterator referencing the last character to remove.
  /// @returns Iterator referencing location of first after removal.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611714
  Iterator erase(ConstIterator first, ConstIterator last)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.erase(first, last);
  }

  /// @brief Replace characters with value from another string.
  /// @param[in] pos Start of the substring that is going to be replaced.
  /// @param[in] count Length of the substring that is going to be replaced.
  /// @param[in] other The Source string to use for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611715
  basic_string& replace(SizeType pos, SizeType count, basic_string const& other)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(pos, count, other.value_);
    return *this;
  }

  /// @brief Set value to a range of characters.
  /// @param[in] first Iterator referencing the first character to replace.
  /// @param[in] last Iterator referencing the last character to replace.
  /// @param[in] other The Source string to use for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611716
  basic_string& replace(ConstIterator first, ConstIterator last, basic_string const& other)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(first, last, other.value_);
    return *this;
  }

  /// @brief Replace characters with value from another string.
  /// @param[in] pos Start of the substring that is going to be replaced.
  /// @param[in] count Length of the substring that is going to be replaced.
  /// @param[in] other The Source string to use for replacement.
  /// @param[in] pos2 Start of the substring to replace with.
  /// @param[in] count2 Number of characters to replace with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611717
  basic_string& replace(
      // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
      SizeType pos, SizeType count, basic_string const& other, SizeType pos2, SizeType count2)
  {
    value_.replace(pos, count, other.value_, pos2, count2);
    return *this;
  }

  /// @brief Set value to a range of characters.
  /// @tparam InputIt It is an input string to an Iterator.
  /// @param[in] first Iterator referencing the first character to replace.
  /// @param[in] last Iterator referencing the last character to replace.
  /// @param[in] first2 Iterator referencing the first character for replacement.
  /// @param[in] last2 Iterator referencing the last character for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611718
  template <class InputIt>
  basic_string& replace(ConstIterator first, ConstIterator last, InputIt first2, InputIt last2)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(first, last, first2, last2);
    return *this;
  }

  /// @brief Replace characters with value of a C substring.
  /// @param[in] pos Start of the substring that is going to be replaced.
  /// @param[in] count Length of the substring that is going to be replaced.
  /// @param[in] cstr Pointer to the character string to use for replacement.
  /// @param[in] count2 Number of characters to replace.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611719
  basic_string& replace(SizeType pos, SizeType count, CharT const* cstr, SizeType count2)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(pos, count, cstr, count2);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Set value to a range of characters.
  /// @param[in] first Iterator referencing the first character to replace.
  /// @param[in] last Iterator referencing the last character to replace.
  /// @param[in] cstr Pointer to the character string to use for replacement.
  /// @param[in] count2 Number of characters to replace with.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611720
  basic_string& replace(ConstIterator first, ConstIterator last, CharT const* cstr, SizeType count2)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(first, last, cstr, count2);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief  Replace characters with value of a C string.
  /// @param[in] pos Start of the substring that is going to be replaced.
  /// @param[in] count Length of the substring that is going to be replaced.
  /// @param[in] cstr Pointer to the character string to use for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611721
  basic_string& replace(SizeType pos, SizeType count, CharT const* cstr)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(pos, count, cstr);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Set value to a range of characters.
  /// @param[in] first Iterator referencing the first character to replace.
  /// @param[in] last Iterator referencing the last character to replace.
  /// @param[in] cstr Pointer to the character string to use for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611722
  basic_string& replace(ConstIterator first, ConstIterator last, CharT const* cstr)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(first, last, cstr);
    return *this;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Replace characters with multiple characters.
  /// @param[in] pos Start of the substring that is going to be replaced.
  /// @param[in] count Length of the substring that is going to be replaced.
  /// @param[in] count2 Number of characters to replace with.
  /// @param[in] ch Character value to use for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611723
  basic_string& replace(SizeType pos, SizeType count, SizeType count2, CharT ch)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(pos, count, count2, ch);
    return *this;
  }

  /// @brief Replace range of characters with multiple characters
  /// @param[in] first Iterator referencing the first character to replace.
  /// @param[in] last Iterator referencing the last character to replace.
  /// @param[in] count2 Number of characters to replace with.
  /// @param[in] ch Character value to use for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611724
  basic_string& replace(ConstIterator first, ConstIterator last, SizeType count2, CharT ch)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(first, last, count2, ch);
    return *this;
  }

  /// @brief Replace range of characters with initializer_list.
  /// @param[in] first Iterator referencing the first character to replace.
  /// @param[in] last Iterator referencing the first character to replace.
  /// @param[in] ilist Initializer list with the characters to use for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611725
  basic_string& replace(ConstIterator first, ConstIterator last, std::initializer_list<CharT> ilist)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(first, last, ilist);
    return *this;
  }

  /// @brief Replace characters with value from another string.
  /// @param[in] pos1 Start of the substring that is going to be replaced
  /// @param[in] n1 Number of characters to be replaced.
  /// @param[in] sv String to use for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611726
  basic_string& replace(SizeType pos1, SizeType n1, basic_string_view<CharT, Traits> sv)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.replace(pos1, n1, sv.data(), sv.size());
    return *this;
  }

  /// @brief Replace characters with value from another string.
  /// @tparam T Data type of argument.
  /// @tparam E  It is designed to be used to initialize default constructor or
  /// function parameter at compile time.
  /// @param[in] pos1 Start of the substring that is going to be replaced.
  /// @param[in] n1 Number of characters to be replaced.
  /// @param[in] t Object with the characters to use for replacement
  /// @param[in] pos2 Start of the substring to replace with.
  /// @param[in] n2 Number of characters from str to use.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611727
  template <typename T,
      typename E
      = typename std::enable_if<(std::is_convertible<T const&, basic_string_view<CharT, Traits>>::value)
          && (!std::is_convertible<T const&, CharT const*>::value)>::type>
  basic_string& replace(SizeType pos1, SizeType n1, T const& t, SizeType pos2, SizeType n2 = npos)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    // TODO: out_of_range if pos1 > size() or pos2 > sv.size()
    basic_string_view<CharT, Traits> sv{t};
    SizeType const rlen{std::min(n2, SizeType(sv.size() - pos2))};
    value_.replace(pos1, n1, sv.data() + pos2, rlen);
    return *this;
  }

  /// @brief Replace range of characters with string.
  /// @param[in] i1 Iterator referencing start of range to replace.
  /// @param[in] i2 Iterator referencing end of range to replace.
  /// @param[in] sv String to use for replacement.
  /// @returns Reference to this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611728
  basic_string& replace(ConstIterator i1, ConstIterator i2, basic_string_view<CharT, Traits> sv)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return replace(i1 - value_.begin(), i2 - i1, sv);
  }

  /// @brief Convert to a string_view.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611729
  operator basic_string_view<CharT, Traits>() const noexcept
  {
    return basic_string_view<CharT, Traits>(value_.data(), value_.size());
  }

  /// @brief Finds the first substring equal to str.
  /// @param[in] str String to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or npos if
  /// no such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611730
  SizeType find(basic_string const& str, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find(str.value_, pos);
  }

  /// @brief Finds the first substring equal to the range.
  /// @param[in] s Pointer to a character string to search for.
  /// @param[in] pos Position at which to start the search.
  /// @param[in] count Length of substring to search for.
  /// @returns Position of the first character of the found substring or npos if no
  /// such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611731
  SizeType find(CharT const* s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find(s, pos, count);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Finds the first substring equal to the character string pointed to by s.
  /// @param[in] s Pointer to a character string to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or npos if
  /// no such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611732
  SizeType find(CharT const* s, SizeType pos = 0) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find(s, pos);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Finds the first character ch.
  /// @param[in] ch Character to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or npos if
  /// no such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611733
  SizeType find(CharT ch, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find(ch, pos);
  }

  /// @brief Implicitly converts t to a string view sv, then finds the first substring equal to sv.
  /// @param[in] sv String to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character of the found substring or npos if
  /// no such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611734
  SizeType find(basic_string_view<CharT, Traits> sv, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find(sv.data(), pos, sv.size());
  }

  /// @brief Finds the last substring equal to str.
  /// @param[in] str String to search for.
  /// @param[in] pos Position at which to begin searching.
  /// @returns Position of the first character of the found substring or npos if
  /// no such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611735
  SizeType rfind(basic_string const& str, SizeType pos = npos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.rfind(str.value_, pos);
  }

  /// @brief Finds the last substring equal to the range.
  /// @param[in] s Pointer to a character string to search for.
  /// @param[in] pos Position at which to begin searching.
  /// @param[in] count Pointer to a character string to search for.
  /// @returns Position of the first character of the found substring or npos if
  /// no such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611736
  SizeType rfind(CharT const* s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.rfind(s, pos, count);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Finds the last substring equal to the character string pointed to by s.
  /// @param[in] s Pointer to a character string to search for.
  /// @param[in] pos Position at which to begin searching.
  /// @returns Position of the first character of the found substring or npos if
  /// no such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611737
  SizeType rfind(CharT const* s, SizeType pos = npos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.rfind(s, pos);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Finds the last character equal to ch.
  /// @param[in] ch Character to search for.
  /// @param[in] pos Position at which to begin searching.
  /// @returns Position of the first character of the found substring or npos if
  /// no such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611738
  SizeType rfind(CharT ch, SizeType pos = npos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.rfind(ch, pos);
  }

  /// @brief Implicitly converts t to a string view sv, then finds the last substring equal
  /// to the contents of sv.
  /// @param[in] sv String to search for.
  /// @param[in] pos Position at which to begin searching.
  /// @returns Position of the first character of the found substring or npos if
  /// no such substring is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611739
  SizeType rfind(basic_string_view<CharT, Traits> sv, SizeType pos = npos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.rfind(sv.data(), pos, sv.size());
  }

  /// @brief Finds the first character equal to one of the characters in the range.
  /// @param[in] s Pointer to a character string identifying characters to search for.
  /// @param[in] pos Position at which to begin searching
  /// @param[in] count Length of character string identifying characters to search for.
  /// @returns Position of the found character or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611740
  SizeType find_first_of(CharT const* s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_of(s, pos, count);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Finds the first character equal to one of the characters in character string pointed to by s.
  /// @param[in] s Pointer to a character string identifying characters to search for.
  /// @param[in] pos Position at which to begin searching
  /// @returns Position of the found character or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611741
  SizeType find_first_of(CharT const* s, SizeType pos = 0) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_of(s, pos);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Finds the first character equal to ch.
  /// @param[in] ch Character to search for.
  /// @param[in] pos Position at which to begin searching
  /// @returns Position of the found character or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611742
  SizeType find_first_of(CharT ch, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_of(ch, pos);
  }

  /// @brief Implicitly converts t to a string view sv, then finds the first character equal
  /// to one of the characters in sv.
  /// @param[in] other Object identifying characters to search for.
  /// @param[in] pos Position at which to begin searching
  /// @returns Position of the found character or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611743
  SizeType find_first_of(basic_string const& other, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_of(other.value_, pos);
  }

  /// @brief Implicitly converts t to a string view sv, then finds the first character equal
  /// to one of the characters in sv.
  /// @param[in] sv String identifying characters to search for.
  /// @param[in] pos Position at which to begin searching
  /// @returns Position of the found character or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611744
  SizeType find_first_of(basic_string_view<CharT, Traits> sv, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_of(sv.data(), pos, sv.size());
  }

  /// @brief Find last position of a character of C substring.
  /// @param[in] s Pointer to a string of characters to search for.
  /// @param[in] pos Pointer to a string of characters to search for.
  /// @param[in] count Length of the string of characters to search for.
  /// @returns Position of the last occurrence of any character of the substring, or
  /// npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611745
  SizeType find_last_of(CharT const* s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_of(s, pos, count);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Find last position of a character of C string.
  /// @param[in] s Pointer to a string of characters to search for.
  /// @param[in] pos Pointer to a string of characters to search for.
  /// @returns Position of the last occurrence of any character of the substring, or
  /// npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611746
  SizeType find_last_of(CharT const* s, SizeType pos = npos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_of(s, pos);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Find last position of a character.
  /// @param[in] ch Character to search for.
  /// @param[in] pos Pointer to a string of characters to search for.
  /// @returns Position of the last occurrence of any character of the substring, or
  /// npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611747
  SizeType find_last_of(CharT ch, SizeType pos = npos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_of(ch, pos);
  }

  /// @brief Finds the last occurence of any of the characters ending at position pos.
  /// @param[in] other String identifying to search for.
  /// @param[in] pos Index of character to search back from (default end).
  /// @returns Position of the last occurrence of any character of the substring, or
  /// npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611748
  SizeType find_last_of(basic_string const& other, SizeType pos = npos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_of(other.value_, pos);
  }

  /// @brief Finds the last occurence of any of the characters of Sv in this view, ending at position pos.
  /// @param[in] sv An object convertible to string_view containing characters to locate.
  /// @param[in] pos Index of character to search back from (default end).
  /// @returns Position of the last occurrence of any character of the substring, or
  /// npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611749
  SizeType find_last_of(basic_string_view<CharT, Traits> sv, SizeType pos = npos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_of(sv.data(), pos, sv.size());
  }

  /// @brief Find position of a character not in C substring.
  /// @param[in] s Pointer to a string of characters to compare.
  /// @param[in] pos Position at which to start the search.
  /// @param[in] count Length of the string of characters to compare.
  /// @returns Position of the first character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611750
  SizeType find_first_not_of(CharT const* s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_not_of(s, pos, count);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Find position of a character not in C substring.
  /// @param[in] s Pointer to a string of characters to compare.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611751
  SizeType find_first_not_of(CharT const* s, SizeType pos = 0) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_not_of(s, pos);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Find position of a different character.
  /// @param[in] ch  Character to compare.
  /// @param[in] pos  Position at which to start the search.
  /// @returns Position of the first character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611752
  SizeType find_first_not_of(CharT ch, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_not_of(ch, pos);
  }

  /// @brief Finds the first character not equal to any of the characters, starting at position pos.
  /// @param[in] other String identifying to search for.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611753
  SizeType find_first_not_of(basic_string const& other, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_not_of(other.value_, pos);
  }

  /// @brief Finds the first character not equal to any of the characters of sv in this view,
  /// starting at position pos.
  /// @param[in] sv An object convertible to string_view containing characters to locate.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the first character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611754
  SizeType find_first_not_of(basic_string_view<CharT, Traits> sv, SizeType pos = 0) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_first_not_of(sv.data(), pos, sv.size());
  }

  /// @brief Find position of a different character.
  /// @param[in] s Pointer to a string of characters to compare.
  /// @param[in] pos Position at which to start the search.
  /// @param[in] count Length of the string of characters to compare
  /// @returns Position of the last character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611755
  SizeType find_last_not_of(CharT const* s, SizeType pos, SizeType count) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_not_of(s, pos, count);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Find last position of a character not in C string.
  /// @param[in] s Pointer to a string of characters to compare.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the last character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611756
  SizeType find_last_not_of(CharT const* s, SizeType pos = npos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_not_of(s, pos);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Find position of a different character.
  /// @param[in] ch  Character to compare.
  /// @param[in] pos  Position at which to start the search.
  /// @returns Position of the last character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611757
  SizeType find_last_not_of(CharT ch, SizeType pos = npos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_not_of(ch, pos);
  }

  /// @brief Finds the last character not equal to any of the characters, starting at position pos.
  /// @param[in] other The Source string
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the last character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611758
  SizeType find_last_not_of(basic_string const& other, SizeType pos = npos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_not_of(other.value_, pos);
  }

  /// @brief Finds the last character not equal to any of the characters of sv in this view,
  /// starting at position pos.
  /// @param[in] sv Pointer to a string of characters to compare.
  /// @param[in] pos Position at which to start the search.
  /// @returns Position of the last character not equal to any of the characters
  /// in the given string, or npos if no such character is found.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611759
  SizeType find_last_not_of(basic_string_view<CharT, Traits> sv, SizeType pos = npos) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.find_last_not_of(sv.data(), pos, sv.size());
  }

  /// @brief Compare to a string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Index of first occurrence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611760
  std::int32_t compare(basic_string const& other) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.compare(other.value_);
  }

  /// @brief Find position of a different character.
  /// @param[in] count Number of characters of this string to compare.
  /// @param[in] pos Position of the first character in this string to compare
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Index of first occurrence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611761
  std::int32_t compare(SizeType pos, SizeType count, basic_string const& other) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.compare(pos, count, other.value_);
  }

  /// @brief Compare to a string_view.
  /// @param[in] pos1  Position of the first character in this string to compare.
  /// @param[in] count1  Number of characters of this string to compare.
  /// @param[in] pos2 Position of the first character of the given string to compare
  /// @param[in] count2  Number of characters of the given string to compare.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns negative value if this view is less than the other character sequence,
  /// zero if the both character sequences are equal, positive value if this
  /// view is greater than the other character sequence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611762
  std::int32_t compare(
      // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
      SizeType pos1, SizeType count1, basic_string const& other, SizeType pos2, SizeType count2) const
  {
    return value_.compare(pos1, count1, other.value_, pos2, count2);
  }

  /// @brief Compare to a string.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns Index of first occurence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611763
  std::int32_t compare(InternalType const& other) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.compare(other);
  }

  /// @brief Compare substring to a string.
  /// @param[in] pos Position at which to start the search.
  /// @param[in] count Number of characters of this view to compare.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns negative value if this view is less than the other character sequence,
  /// zero if the both character sequences are equal, positive value if this
  /// view is greater than the other character sequence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611764
  std::int32_t compare(SizeType pos, SizeType count, InternalType const& other) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.compare(pos, count, other);
  }

  /// @brief Find position of a different character.
  /// @param[in] pos1 Position of the first character in this string to compare.
  /// @param[in] count1 Number of characters of this string to compare.
  /// @param[in] pos2 Position of the first character of the given string to compare
  /// @param[in] count2 Number of characters of the given string to compare.
  /// @param[in] other The another string to use as source to initialize the string with.
  /// @returns negative value if this view is less than the other character sequence,
  /// zero if the both character sequences are equal, positive value if this
  /// view is greater than the other character sequence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611765
  std::int32_t compare(
      // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
      SizeType pos1, SizeType count1, InternalType const& other, SizeType pos2, SizeType count2) const

  {
    return value_.compare(pos1, count1, other, pos2, count2);
  }

  /// @brief Compare to a C string.
  /// @param[in] s Pointer to the character string to compare to.
  /// @returns negative value if this view is less than the other character sequence,
  /// zero if the both character sequences are equal, positive value if this
  /// view is greater than the other character sequence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611766
  std::int32_t compare(CharT const* s) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.compare(s);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Find position of a different character.
  /// @param[in] pos1 Position of the first character in this string to compare.
  /// @param[in] count1 Number of characters of this string to compare.
  /// @param[in] s Pointer to the character string to compare to.
  /// @returns Index of first occurrence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611767
  std::int32_t compare(SizeType pos1, SizeType count1, CharT const* s) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.compare(pos1, count1, s);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Compare substring against a character %array.
  /// @param[in] count1 Number of characters of this view to compare.
  /// @param[in] pos1 Position of the first character in this view to compare.
  /// @param[in] count2 Number of characters of the given view to compare.
  /// @param[in] pos2 Position of the first character of the given view to compare.
  /// @param[in] s Character array to compare against.
  /// @returns negative value if this view is less than the other character sequence,
  /// zero if the both character sequences are equal, positive value if this
  /// view is greater than the other character sequence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611768
  std::int32_t compare(SizeType pos1, SizeType count1, CharT const* s, SizeType pos2, SizeType count2) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.compare(pos1, count1, s + pos2, count2);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Find position of a different character.
  /// @param[in] pos1 Position of the first character in this string to compare.
  /// @param[in] count1 Number of characters of this string to compare.
  /// @param[in] s Pointer to the character string to compare to.
  /// @param[in] count2 Number of characters of the given string to compare.
  /// @returns negative value if this view is less than the other character sequence,
  /// zero if the both character sequences are equal, positive value if this
  /// view is greater than the other character sequence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611769
  std::int32_t compare(SizeType pos1, SizeType count1, CharT const* s, SizeType count2) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.compare(pos1, count1, s, count2);
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Compare to a string.
  /// @param[in] sv String to compare against.
  /// @returns Index of first occurence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611770
  std::int32_t compare(basic_string_view<CharT, Traits> sv) const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    SizeType const rlen{std::min(value_.size(), sv.size())};
    std::int32_t const ret{Traits::compare(value_.data(), sv.data(), rlen)};
    if (ret == 0)
    {
      return (value_.size() < sv.size()) ? -1 : value_.size() > sv.size();
    }
    return ret;
  }

  /// @brief Compare to a string_view.
  /// @param[in] pos1  A position in the string to start comparing from.
  /// @param[in] n1 The number of characters to compare.
  /// @param[in] sv An object convertible to string_view to compare against.
  /// @returns negative value if this view is less than the other character sequence,
  /// zero if the both character sequences are equal, positive value if this
  /// view is greater than the other character sequence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611771
  std::int32_t compare(SizeType pos1, SizeType n1, basic_string_view<CharT, Traits> sv) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return basic_string_view<CharT, Traits>(value_.data(), value_.size()).substr(pos1, n1).compare(sv);
  }

  /// @brief Compare to a string_view.
  /// @tparam T Data type of argument.
  /// @tparam E  It is designed to be used to initialize default constructor or function
  /// parameter at compile time.
  /// @param[in] pos1 A position in the string to start comparing from.
  /// @param[in] n1 The number of characters to compare.
  /// @param[in] t An object convertible to string_view to compare against.
  /// @param[in] pos2 A position in the string_view to start comparing from.
  /// @param[in] n2 The number of characters to compare.
  /// @returns negative value if this view is less than the other character sequence,
  /// zero if the both character sequences are equal, positive value if this
  /// view is greater than the other character sequence.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611772
  template <typename T,
      typename E
      = typename std::enable_if<(std::is_convertible<T const&, basic_string_view<CharT, Traits>>::value)
          && (!std::is_convertible<T const&, CharT const*>::value)>::type>
  std::int32_t compare(SizeType pos1, SizeType n1, T const& t, SizeType pos2, SizeType n2 = npos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    basic_string_view<CharT, Traits> const sv{t};
    return basic_string_view<CharT, Traits>(value_.data(), value_.size())
        .substr(pos1, n1)
        .compare(sv.substr(pos2, n2));
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Return whether this String is empty.
  /// @returns an empty instance to string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611773
  bool empty() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.empty();
  }

  /// @brief Used to check the size of the String .
  /// @returns Return the number of elements in this String..
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611774
  SizeType size() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.size();
  }

  /// @brief Used to check the length of the String .
  /// @returns Return the number of elements in this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611775
  SizeType length() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.length();
  }

  /// @brief Used to check the maximum size of the String .
  /// @returns Return the maximum number of elements supported by this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611776
  SizeType max_size() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.max_size();
  }

  /// @brief Returns the total number of characters that the string can hold before needing to
  /// allocate more memory.
  /// @returns Returns the size of allocate storage, expressed in terms of elements.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611777
  SizeType capacity() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.capacity();
  }

  /// @brief It request a change in capacity.
  /// @param[in] newCap New capacity of the string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611778
  void reserve(SizeType newCap = 0)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.reserve(newCap);
  }

  /// @brief Resizes the string to contain count characters.
  /// @param[in] count New size of the string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611779
  void resize(SizeType count)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.resize(count);
  }

  /// @brief Resizes the string to the specified number of characters.
  /// @param[in] count New size of the string.
  /// @param[in] ch Character to initialize the new characters with
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611780
  void resize(SizeType count, CharT ch)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.resize(count, ch);
  }

  /// @brief Requests the string to reduce its capacity to fit its size.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611781
  void shrink_to_fit()
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.shrink_to_fit();
  }

  /// @brief Erases the string, making it empty.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611782
  void clear() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.clear();
  }
  /// @brief Return const Pointer to null-terminated contents.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @returns A Pointer to the c-string representation of the string objects value.
  /// @design 611783
  CharT const* c_str() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.c_str();
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Return const Pointer to contents.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @returns A Pointer to the data representation of the string objects value.
  /// @design 611784
  CharT const* data() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.data();
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

  /// @brief Returns a Reference to the first character in the string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @returns Reference to the first character.
  /// @design 611785
  CharT& front()
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.front();
  }

  /// @brief Returns a constant Reference to the first character in the string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @returns Reference to the first character.
  /// @design 611786
  CharT const& front() const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.front();
  }

  /// @brief Returns a Reference to the last character in the string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @returns Reference to the last character.
  /// @design 611787
  CharT& back()
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.back();
  }

  /// @brief Returns a constant Reference to the last character in the string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @returns Reference to the last character.
  /// @design 611788
  CharT const& back() const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.back();
  }

  /// @brief Append a single character.
  /// @param[in] ch The character to append.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611789
  void push_back(CharT ch)
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.push_back(ch);
  }

  /// @brief Remove the last character.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611790
  void pop_back()
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.pop_back();
  }

  /// @brief To get a substring.
  /// @param[in] pos Position of the first character to include
  /// @param[in] count Length of the substring.
  /// @returns String containing the substring.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611791
  basic_string substr(SizeType pos = 0, SizeType count = npos) const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.substr(pos, count);
  }

  /// @brief Swap contents with another string.
  /// @param[in] other String to exchange the contents with.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611792
  void swap(basic_string& other) noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    value_.swap(other.value_);
  }

  /// @brief It returns an Iterator to the beginning of the string.
  /// @returns Return a Iterator pointing to the first element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611793
  Iterator begin() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.begin();
  }

  /// @brief Used to return an Iterator pointing to the first element.
  /// @returns Return a ConstIterator pointing to the first element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611794
  ConstIterator begin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.begin();
  }

  /// @brief Used to return an const Iterator refers to the first element.
  /// @returns Return a ConstIterator pointing to the first element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611795
  ConstIterator cbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.cbegin();
  }

  /// @brief Used to return an Iterator pointing to the last element.
  /// @returns Return a Iterator pointing to the last element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611796
  Iterator end() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.end();
  }

  /// @brief Used to return an ConstIterator pointing to the last element.
  /// @returns Return a ConstIterator pointing to the last element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611797
  ConstIterator end() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.end();
  }

  /// @brief Used to return an const Iterator pointing to the last element.
  /// @returns Return a ConstIterator pointing to the last element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611798
  ConstIterator cend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.cend();
  }

  /// @brief Used to return an reverse Iterator pointing to the last element.
  /// @returns Return a ReverseIterator pointing to the last element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611799
  ReverseIterator rbegin() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.rbegin();
  }

  /// @brief Used to return an const reverse Iterator pointing to the last element.
  /// @returns Return a ConstReverseIterator pointing to the last element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611800
  ConstReverseIterator rbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.rbegin();
  }

  /// @brief Used to return const reverse Iterator pointing to the last element.
  /// @returns Return a ConstReverseIterator pointing to the last element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611801
  ConstReverseIterator crbegin() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.crbegin();
  }

  /// @brief Used to return reverse Iterator pointing to the first element.
  /// @returns Return a ReverseIterator pointing to the first element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611802
  ReverseIterator rend() noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.rend();
  }

  /// @brief Used to return const reverse Iterator pointing to the first element.
  /// @returns Return a ConstReverseIterator pointing to the first element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611803
  ConstReverseIterator rend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.rend();
  }
  /// @brief Used to return const reverse Iterator pointing to the first element.
  /// @returns Return a ConstReverseIterator pointing to the first element of this String.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @design 611804
  ConstReverseIterator crend() const noexcept
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.crend();
  }

  /// @brief Return copy of allocator used to construct this string.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  /// @returns It returns the allocator.
  /// @design 611805
  AllocatorType get_allocator() const
  // AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
  {
    return value_.get_allocator();
  }
};

/// @brief To concatenate two strings.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns New string with value of lhs followed by rhs.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611884
template <typename CharT, typename Traits, typename Allocator>
basic_string<CharT, Traits, Allocator> operator+(
    basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
}

/// @brief Concatenate string and C string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns New string with value of lhs followed by rhs.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611885
template <typename CharT, typename Traits, typename Allocator>
basic_string<CharT, Traits, Allocator> operator+(
    basic_string<CharT, Traits, Allocator> const& lhs, CharT const* rhs)
{
  return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief To concatenate C string and string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns New string with value of lhs followed by rhs.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611886
template <typename CharT, typename Traits, typename Allocator>
basic_string<CharT, Traits, Allocator> operator+(
    CharT const* lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief To concatenate two strings.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns New string with value of lhs followed by rhs.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611887
template <typename CharT, typename Traits, typename Allocator>
basic_string<CharT, Traits, Allocator> operator+(
    std::basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
}

/// @brief To concatenate two strings.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns New string with value of lhs followed by rhs.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611888
template <typename CharT, typename Traits, typename Allocator>
basic_string<CharT, Traits, Allocator> operator+(
    basic_string<CharT, Traits, Allocator> const& lhs, std::basic_string<CharT, Traits, Allocator> const& rhs)
{
  return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
}

/// @brief Test equivalence of two strings.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs.compare(rhs) == 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611889
template <typename CharT, typename Traits, typename Allocator>
bool operator==(
    basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) == 0;
}

/// @brief Test difference of two strings.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs.compare(rhs) != 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611890
template <typename CharT, typename Traits, typename Allocator>
bool operator!=(
    basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return !(lhs == rhs);
}

/// @brief Test if string precedes string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs precedes rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611891
template <typename CharT, typename Traits, typename Allocator>
bool operator<(
    basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) < 0;
}

/// @brief Test if s is atring doesn't follow string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't follow rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611892
template <typename CharT, typename Traits, typename Allocator>
bool operator<=(
    basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) <= 0;
}

/// @brief Test if string follows string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs follows rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611893
template <typename CharT, typename Traits, typename Allocator>
bool operator>(
    basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) > 0;
}

/// @brief Test if string doesn't precede string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't precede rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611894
template <typename CharT, typename Traits, typename Allocator>
bool operator>=(
    basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) >= 0;
}

/// @brief Test equivalence of two strings.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs compare rhs == 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611895
template <typename CharT, typename Traits, typename Allocator>
bool operator==(
    basic_string<CharT, Traits, Allocator> const& lhs, std::basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) == 0;
}

/// @brief Test difference of two strings.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs.compare(rhs) != 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611896
template <typename CharT, typename Traits, typename Allocator>
bool operator!=(
    basic_string<CharT, Traits, Allocator> const& lhs, std::basic_string<CharT, Traits, Allocator> const& rhs)
{
  return !(lhs == rhs);
}

/// @brief Test if string precedes string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs precedes rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611897
template <typename CharT, typename Traits, typename Allocator>
bool operator<(
    basic_string<CharT, Traits, Allocator> const& lhs, std::basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) < 0;
}

/// @brief Test if string doesn't follow string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't follow rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611898
template <typename CharT, typename Traits, typename Allocator>
bool operator<=(
    basic_string<CharT, Traits, Allocator> const& lhs, std::basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) <= 0;
}

/// @brief Test if string follows string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs follows rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611899
template <typename CharT, typename Traits, typename Allocator>
bool operator>(
    basic_string<CharT, Traits, Allocator> const& lhs, std::basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) > 0;
}

/// @brief Test if string doesn't precede string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't precede rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611900
template <typename CharT, typename Traits, typename Allocator>
bool operator>=(
    basic_string<CharT, Traits, Allocator> const& lhs, std::basic_string<CharT, Traits, Allocator> const& rhs)
{
  return lhs.compare(rhs) >= 0;
}

/// @brief Test equivalence of two strings.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs.compare(rhs) == 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611901
template <typename CharT, typename Traits, typename Allocator>
bool operator==(
    std::basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) == 0;
}

/// @brief Test difference of two strings.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs.compare(rhs) != 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611902
template <typename CharT, typename Traits, typename Allocator>
bool operator!=(
    std::basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return !(lhs == rhs);
}

/// @brief Test if string precedes string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs precedes rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611903
template <typename CharT, typename Traits, typename Allocator>
bool operator<(
    std::basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) > 0;
}

/// @brief Test if string doesn't follow string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't follow rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611904
template <typename CharT, typename Traits, typename Allocator>
bool operator<=(
    std::basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) <= 0;
}

/// @brief Test if string follows string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs follows rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611905
template <typename CharT, typename Traits, typename Allocator>
bool operator>(
    std::basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) < 0;
}

/// @brief Test if string doesn't precede string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't precede rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611906
template <typename CharT, typename Traits, typename Allocator>
bool operator>=(
    std::basic_string<CharT, Traits, Allocator> const& lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) >= 0;
}

/// @brief Test equivalence of C string and string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if rhs.compare(lhs) == 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611907
template <typename CharT, typename Traits, typename Allocator>
bool operator==(CharT const* lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) == 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Test equivalence of string and C string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs.compare(rhs) == 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611908
template <typename CharT, typename Traits, typename Allocator>
bool operator==(basic_string<CharT, Traits, Allocator> const& lhs, CharT const* rhs)
{
  return lhs.compare(rhs) == 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Test difference of C string and string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if rhs.compare(lhs) != 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611909
template <typename CharT, typename Traits, typename Allocator>
bool operator!=(CharT const* lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return !(lhs == rhs);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Test difference of string and C string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs.compare(rhs) != 0, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611910
template <typename CharT, typename Traits, typename Allocator>
bool operator!=(basic_string<CharT, Traits, Allocator> const& lhs, CharT const* rhs)
{
  return !(lhs == rhs);
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Test if C string precedes string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs precedes rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611911
template <typename CharT, typename Traits, typename Allocator>
bool operator<(CharT const* lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) > 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Test if string precedes C string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs precedes rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611912
template <typename CharT, typename Traits, typename Allocator>
bool operator<(basic_string<CharT, Traits, Allocator> const& lhs, CharT const* rhs)
{
  return lhs.compare(rhs) < 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Test if C string doesn't follow string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't follow rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611913
template <typename CharT, typename Traits, typename Allocator>
bool operator<=(CharT const* lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) >= 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Test if string doesn't follow C string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't follow rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611914
template <typename CharT, typename Traits, typename Allocator>
bool operator<=(basic_string<CharT, Traits, Allocator> const& lhs, CharT const* rhs)
{
  return lhs.compare(rhs) <= 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Test if C string follows string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs follows rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611915
template <typename CharT, typename Traits, typename Allocator>
bool operator>(CharT const* lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) < 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Test if string follows C string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs follows rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611916
template <typename CharT, typename Traits, typename Allocator>
bool operator>(basic_string<CharT, Traits, Allocator> const& lhs, CharT const* rhs)
{
  return lhs.compare(rhs) > 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief  Test if C string doesn't precede string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't precede rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611917
template <typename CharT, typename Traits, typename Allocator>
bool operator>=(CharT const* lhs, basic_string<CharT, Traits, Allocator> const& rhs)
{
  return rhs.compare(lhs) <= 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief  Test if string doesn't precede C string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] lhs Arguments to the left-hand side of the operator.
/// @param[in] rhs Arguments to the right-hand side of the operator.
/// @returns True if lhs doesn't precede rhs, false otherwise.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611918
template <typename CharT, typename Traits, typename Allocator>
bool operator>=(basic_string<CharT, Traits, Allocator> const& lhs, CharT const* rhs)
{
  return lhs.compare(rhs) >= 0;
}
// AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A27.0.4 C-style strings should not be used.

/// @brief Write string to a stream.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] os It is an output stream.
/// @param[in] str String to write out.
/// @returns Reference to the output stream.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611919
template <class CharT, class Traits, class Allocator>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, basic_string<CharT, Traits, Allocator> const& str)
{
  return os << str.c_str();
}

/// @brief Read stream into a string.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] is It is an input stream.
/// @param[in] str Buffer to store into.
/// @returns Reference to the input stream.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611920
template <class CharT, class Traits, class Allocator>
std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is, basic_string<CharT, Traits, Allocator>& str)
{
  std::string value;
  is >> value;
  basic_string<CharT, Traits, Allocator>(value).swap(str);
  return is;
}

} // namespace internal

/// @brief Representation of a contiguous sequence of narrow characters with custom allocator
/// @tparam Allocator The type used to allocate internal storage.
/// @trace SWS_CORE_00040
/// @archtrace 609886
template <typename Allocator = std::allocator<char>>
using basic_string = internal::basic_string<char, std::char_traits<char>, Allocator>;

/// @brief Representation of a contiguous sequence of narrow characters
/// @trace SWS_CORE_00040
/// @archtrace 609886
using String = basic_string<>;
namespace pmr
{
using String = basic_string<pmr::polymorphic_allocator<char>>;
}
/// @brief Add overload of swap() for BasicString.
/// @tparam Allocator Allocator type used to allocate internal storage.
/// @param[in] lhs The first argument of the swap invocation.
/// @param[in] rhs The second argument of the swap invocation.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611861
template <typename Allocator>
void swap(basic_string<Allocator>& lhs, basic_string<Allocator>& rhs) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  lhs.swap(rhs);
}

/// @brief Extracts characters from stream until end of line or the specified delimiter delim.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] input The stream to get data from.
/// @param[in] s Pointer to the character string to store the characters to.
/// @param[in] delim Delimiting character to stop the extraction at. It is extracted but not stored.
/// @returns Return the input.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611862
template <class CharT, class Traits, class Allocator>
std::basic_istream<CharT, Traits>& getline(
    //AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
    std::basic_istream<CharT, Traits>& input, internal::basic_string<CharT, Traits, Allocator>& s,
    CharT delim)
{
  std::basic_string<CharT, Traits, Allocator> line;
  std::basic_istream<CharT, Traits>& result{std::getline(input, line, delim)};
  s.assign(line.c_str());
  return result;
}

/// @brief Extracts characters from stream until end of line or the specified delimiter delim.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] input The stream to get data from.
/// @param[in] s Pointer to the character string to store the characters to.
/// @param[in] delim Delimiting character to stop the extraction at. It is extracted but not stored.
/// @returns Return the input.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611863
template <class CharT, class Traits, class Allocator>
std::basic_istream<CharT, Traits>& getline(std::basic_istream<CharT, Traits>&& input,
    //AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
    internal::basic_string<CharT, Traits, Allocator>& s, CharT delim)
{
  std::basic_string<CharT, Traits, Allocator> line;
  std::basic_istream<CharT, Traits>& result{std::getline(input, line, delim)};
  s.assign(line.c_str());
  return result;
}

/// @brief The end of file condition occurs in the input sequence.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] input The stream to get data from.
/// @param[in] s Pointer to the character string to store the characters to.
/// @returns Return the input.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611864
template <class CharT, class Traits, class Allocator>
std::basic_istream<CharT, Traits>& getline(
    //AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
    std::basic_istream<CharT, Traits>& input, internal::basic_string<CharT, Traits, Allocator>& s)
{
  std::basic_string<CharT, Traits, Allocator> line;
  std::basic_istream<CharT, Traits>& result{std::getline(input, line)};
  s.assign(line.c_str());
  return result;
}

/// @brief The end of file condition occurs in the input sequence.
/// @tparam CharT It is a character type.
/// @tparam Traits The traits class specifying the operations on the character type.
/// @tparam Allocator The type used to allocate internal storage.
/// @param[in] input The stream to get data from.
/// @param[in] s Pointer to the character string to store the characters to.
/// @returns Return the input.
/// @trace SWS_CORE_00040
/// @archtrace 609886
/// @design 611865
template <class CharT, class Traits, class Allocator>
std::basic_istream<CharT, Traits>& getline(
    //AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
    std::basic_istream<CharT, Traits>&& input, internal::basic_string<CharT, Traits, Allocator>& s)
{
  std::basic_string<CharT, Traits, Allocator> line;
  std::basic_istream<CharT, Traits>& result{std::getline(input, line)};
  s.assign(line.c_str());
  return result;
}

} // namespace core
} // namespace ara

namespace std
{
template <>
/// @brief Specialization of std::hash for ara::core::String.
struct hash<ara::core::String>
{
  /// @brief The size_t alias of ResultType.
  using ResultType = std::size_t;

  /// @brief Function call operator to retrieve the hash value of ara::core::String.
  /// By providing this, ara::core::String can be used in std::unordered_map and std::unordered_set.
  /// @param[in] s The String to be hashed.
  /// @returns It return hash value.
  /// @trace SWS_CORE_00040
  /// @archtrace 609886
  ResultType operator()(ara::core::String const& s) const noexcept
  {
    using std::hash;
    return hash<std::string>()(s.value_);
  }
};

} // namespace std

#endif // APD_ARA_CORE_STRING_H_
