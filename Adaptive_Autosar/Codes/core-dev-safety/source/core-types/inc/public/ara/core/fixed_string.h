// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
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
#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FIXED_STRING_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_FIXED_STRING_H_

#include "ara/core/type_traits.h"
#include "ara/core/string_internal.h"
#include "ara/core/optional.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
namespace ara
{
namespace core
{
/// @brief Alias name for std::enable_if.
/// @tparam T The type of element in the String.
/// @tparam ReturnType For returning the parameter value for primitives.
template <typename T, typename ReturnType>
using IsStringOrCharArrayOrChar =
    typename std::enable_if<(is_cxx_string<T>::value || is_char_array<T>::value
                                || std::is_same<T, std::string>::value || std::is_same<T, char>::value),
        ReturnType>::type;

static const uint64_t npos = static_cast<uint64_t>(std::string::npos);

/// @brief Alias name for std::enable_if.
/// @tparam T The type of element in the String.
/// @tparam ReturnType For returning the parameter value for primitives.
template <typename T, typename ReturnType>
using IsStringOrCharArray = typename std::enable_if<(is_cxx_string<T>::value || is_char_array<T>::value
                                                        || std::is_same<T, std::string>::value),
    ReturnType>::type;

/// @brief Alias name for std::enable_if.
/// @tparam T The type of element in the String.
/// @tparam ReturnType For returning the parameter value for primitives.
template <typename T, typename ReturnType>
using IsStdStringOrCharArrayOrChar =
    typename std::enable_if<(is_char_array<T>::value || std::is_same<T, std::string>::value
                                || std::is_same<T, char>::value),
        ReturnType>::type;

/// @brief Alias name for std::enable_if.
/// @tparam T The type of element in the String.
/// @tparam ReturnType For returning the parameter value for primitives.
template <typename T, typename ReturnType>
using IsCxxStringOrCharArray =
    typename std::enable_if<(is_cxx_string<T>::value || is_char_array<T>::value), ReturnType>::type;

/// @brief Concatenates two fixed strings/string literals.
/// @tparam T1 The template type parameter to first object.
/// @tparam T2 The template type parameter to second object.
/// @param[in] t1 It is the  fixed strings.
/// @param[in] t2 The string literals to concatenate.
/// @returns A new fixed string with capacity equal to the sum of the capacities of the concatenated strings.
/// @code
///     string<5> fuu("cdefg");
///     auto bar = iox::cxx::concatenate(fuu, "ahc");
/// @endcode
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
template <typename T1, typename T2>
typename std::enable_if<(is_char_array<T1>::value || is_cxx_string<T1>::value)
        && (is_char_array<T2>::value || is_cxx_string<T2>::value),
    FixedString<GetCapa<T1>::capa + GetCapa<T2>::capa>>::type
    concatenate(const T1& t1, const T2& t2) noexcept;

/// @brief Concatenates an arbitrary number of fixed strings or string literals.
/// @tparam T1 The template type parameter to first object.
/// @tparam T2 The template type parameter to second object.
/// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
/// @param[in] t1 It is the fixed strings.
/// @param[in] t2 The string literals to concatenate.
/// @param[in] targs Is the template parameter pack.
/// @returns A new fixed string with capacity equal to the sum of the capacities of the concatenated strings.
/// @code
///     string<4> fuu("cdef");
///     auto bar = iox::cxx::concatenate(fuu, "g", "ah", fuu);
/// @endcode
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
/// @design 611828
template <typename T1, typename T2, typename... Targs>
typename std::enable_if<(is_char_array<T1>::value || is_cxx_string<T1>::value)
        && (is_char_array<T2>::value || is_cxx_string<T2>::value),
    FixedString<SumCapa<T1, T2, Targs...>::value>>::type
    concatenate(const T1& t1, const T2& t2, const Targs&... targs) noexcept;

/// @brief Concatenates two fixed strings or one fixed fixed string and
/// one string literal; concatenation of two string literals is not possible.
/// @tparam T1 The template type parameter to first object.
/// @tparam T2 The template type parameter to second object.
/// @param[in] t1 It is the fixed strings.
/// @param[in] t2 The string literal to concatenate.
/// @returns A new fixed string with capacity equal to the sum of the capacities of the concatenated strings.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
/// @design 611829
template <typename T1, typename T2>
typename std::enable_if<(is_char_array<T1>::value && is_cxx_string<T2>::value)
        || (is_cxx_string<T1>::value && is_char_array<T2>::value)
        || (is_cxx_string<T1>::value && is_cxx_string<T2>::value),
    FixedString<GetCapa<T1>::capa + GetCapa<T2>::capa>>::type
    operator+(const T1& t1, const T2& t2) noexcept;

/// @brief Struct used to define a compile time variable which is used to
/// distinguish between constructors with certain behavior.
struct TruncateToCapacity_t
{
  explicit TruncateToCapacity_t() = default;
};
constexpr TruncateToCapacity_t TruncateToCapacity{};

/// @brief FixedString implementation with some adjustments in the API,
/// because we are not allowed to throw exceptions or use heap.
/// @tparam Capacity The capacity of the given string.
/// @archtrace 609886
/// @design 611396 614039
template <uint64_t Capacity>
class FixedString
{
  static_assert(Capacity > 0U, "The capacity of the fixed FixedString must be greater than 0!");

 public:
  /// @brief Creates an empty FixedString with size 0
  constexpr FixedString() noexcept = default;

  /// @brief The copy constructor for the class FixedString.
  /// @param[in] other Is the copy origin.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611397
  FixedString(const FixedString& other) noexcept;

  /// @brief The move constructor for the class FixedString.
  /// @param[in] other Is the move origin.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611398
  FixedString(FixedString&& other) noexcept;

  /// @brief The copy assignment for the class FixedString.
  /// @param[in] rhs Is the copy origin.
  /// @returns The reference to self.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611399
  FixedString& operator=(const FixedString& rhs) noexcept;

  /// @brief The move assignment for the class FixedString.
  /// @param[in] rhs Is the move origin.
  /// @returns The reference to self.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611400
  FixedString& operator=(FixedString&& rhs) noexcept;

  /// @brief Creates a new FixedString of given capacity as a copy of other with compile time
  /// check whether the capacity of other is less than or equal to this' capacity.
  /// @tparam N A fixed-length string of N bytes (neither characters nor code points).
  /// @param[in] other Is the copy origin.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611401
  template <uint64_t N>
  FixedString(const FixedString<N>& other) noexcept;

  /// @brief Moves other to this with compile time check whether the capacity
  /// of other is less than or equal to this' capacity.
  /// @tparam N A fixed-length string of N bytes (neither characters nor code points).
  /// @param[in] other Is the move origin.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611402
  template <uint64_t N>
  FixedString(FixedString<N>&& other) noexcept;

  /// @brief Assigns rhs fixed FixedString to this with compile time check
  /// whether the capacity of rhs is less than or equal to this' capacity.
  /// @tparam N A fixed-length string of N bytes (neither characters nor code points).
  /// @param[in] rhs Is the copy origin.
  /// @returns The reference to self.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611403
  template <uint64_t N>
  FixedString& operator=(const FixedString<N>& rhs) noexcept;

  /// @brief Moves rhs fixed FixedString to this with compile time check whether
  /// the capacity of rhs is less than or equal to this' capacity.
  /// @tparam N A fixed-length string of N bytes (neither characters nor code points).
  /// @param[in] rhs Is the move origin.
  /// @returns The reference to self.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611404
  template <uint64_t N>
  FixedString& operator=(FixedString<N>&& rhs) noexcept;

  /// @brief Conversion constructor for char array with compile time check if the array size is
  /// less than or equal
  /// to the FixedString capacity
  /// @tparam N Is the implicit template parameter for the char array size.
  /// @param[in] other Is the char array.
  /// @note if the array is not zero-terminated, the last value will be overwritten with 0
  /// @code
  ///     #include "iceoryx_hoofs/cxx/FixedString.hpp"
  ///     using namespace iox::cxx;
  ///
  ///     int main()
  ///     {
  ///         FixedString<4> fuu("abcd");
  ///     }
  /// @endcode
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611405
  template <uint64_t N>
  FixedString(const char (&other)[N]) noexcept;

  /// @brief Conversion constructor for cstring to FixedString which truncates
  /// characters if the size is greater than the FixedString capacity.
  /// @param[in] TruncateToCapacity_t Is a compile time variable which is used
  /// to distinguish between constructors with certain behavior.
  /// @param[in] other Is the cstring to convert.
  /// @attention Truncates characters if the size is greater than the FixedString capacity.
  /// @code
  ///     #include "iceoryx_hoofs/cxx/FixedString.hpp"
  ///     using namespace iox::cxx;
  ///
  ///     int main()
  ///     {
  ///         FixedString<4> fuu(TruncateToCapacity, "abcd");
  ///     }
  /// @endcode
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611406
  FixedString(TruncateToCapacity_t, const char* const other) noexcept;

  /// @brief Conversion constructor for std::string to FixedString which truncates
  /// characters if the std::string size is greater than the FixedString capacity.
  /// @param[in] TruncateToCapacity_t Is a compile time variable which is used to
  /// distinguish between constructors with certain behavior.
  /// @param[in] other Is the std::string to convert.
  /// @attention Truncates characters if the std::string size is greater than the FixedString capacity.
  /// @code
  ///     #include "iceoryx_hoofs/cxx/FixedString.hpp"
  ///     using namespace iox::cxx;
  ///
  ///     int main()
  ///     {
  ///         std::string bar = "bar";
  ///         FixedString<4> fuu(TruncateToCapacity, bar);
  ///     }
  /// @endcode
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611407
  FixedString(TruncateToCapacity_t, const std::string& other) noexcept;

  /// @brief Constructor from cstring to FixedString.
  /// Constructs the FixedString with the first count characters of the cstring
  /// including null characters. If count is greater than the FixedString capacity
  /// the remainder of the characters are truncated.
  /// @param[in] TruncateToCapacity_t Is a compile time variable which is used to
  /// distinguish between constructors with certain behavior.
  /// @param[in] other Is the cstring to convert.
  /// @param[in] count Is the number of characters for constructing the FixedString.
  /// @code
  ///     #include "iceoryx_hoofs/cxx/FixedString.hpp"
  ///     using namespace iox::cxx;
  ///
  ///     int main()
  ///     {
  ///         FixedString<4> fuu(TruncateToCapacity, "abcd", 2);
  ///     }
  /// @endcode
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611408
  FixedString(TruncateToCapacity_t, const char* const other, const uint64_t count) noexcept;

  /// @brief Assigns a char array to FixedString with compile time check if the
  /// array size is less than or equal to the FixedString capacity.
  /// @tparam N Is the implicit template parameter for the char array size.
  /// @param[in] rhs Is the char array.
  /// @returns The reference to self.
  /// @note If the array is not zero-terminated, the last value will be overwritten with 0.
  /// @code
  ///     #include "iceoryx_hoofs/cxx/FixedString.hpp"
  ///     using namespace iox::cxx;
  ///
  ///     int main()
  ///     {
  ///         FixedString<4> fuu = "abcd";
  ///     }
  /// @endcode
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611409
  template <uint64_t N>
  FixedString& operator=(const char (&rhs)[N]) noexcept;

  /// @brief FixedString assignment with compile time check
  /// if capacity of str is less than or equal to this' capacity.
  /// @tparam N A fixed-length string of N bytes (neither characters nor code points).
  /// @param[in] str Is the fixed FixedString object to assign
  /// @returns The reference to self.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611410
  template <uint64_t N>
  FixedString& assign(const FixedString<N>& str) noexcept;

  /// @brief Assigns a char array to FixedString with compile time check
  /// if the array size is less than or equal to the FixedString capacity.
  /// @tparam N Is the implicit template parameter for the char array size
  /// @param[in] str Is the char array.
  /// @returns The reference to self.
  /// @note If the array is not zero-terminated, the last value will be overwritten with 0.
  /// @code
  ///     #include "iceoryx_hoofs/cxx/FixedString.hpp"
  ///     using namespace iox::cxx;
  ///
  ///     int main()
  ///     {
  ///         FixedString<4> fuu;
  ///         char bar[] = "abcd";
  ///         fuu.assign(bar);
  ///     }
  /// @endcode
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611411
  template <uint64_t N>
  FixedString& assign(const char (&str)[N]) noexcept;

  /// @brief Assigns a cstring to FixedString. The assignment fails
  /// if the cstring size is greater than the FixedString capacity.
  /// @param[in] str Is the cstring to assign.
  /// @returns True if the assignment succeeds, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611412
  bool unsafe_assign(const char* const str) noexcept;

  /// @brief Assigns a std::string to FixedString. The assignment fails
  /// if the std::string size is greater than the FixedString capacity.
  /// @param[in] str Is the std::string to assign.
  /// @returns True if the assignment succeeds, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611413
  bool unsafe_assign(const std::string& str) noexcept;

  /// @brief Compares self and an iox::cxx::FixedString, std::string or char array.
  /// @tparam T The type of element in the String.
  /// @param[in] other Is the FixedString to compare with self.
  /// @returns an integer < 0 if the first character that does not match has a lower value
  /// in self than in other, 0 if the contents of both strings are equal, an integer > 0
  /// if the first character that does not match has a greater value in self than in other.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611414
  template <typename T>
  IsStringOrCharArray<T, int64_t> compare(const T& other) const noexcept;

  /// @brief Checks if self is equal to rhs.
  /// @tparam T The type of element in the String.
  /// @param[in] rhs Is the iox::cxx::FixedString, std::string, char array
  /// or char to compare with self.
  /// @returns True if both strings are equal, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611415
  template <typename T>
  IsStringOrCharArrayOrChar<T, bool> operator==(const T& rhs) const noexcept;

  /// @brief Checks if self is not equal to rhs.
  /// @tparam T The type of element in the String.
  /// @param[in] rhs Is the iox::cxx::FixedString, std::string, char array
  /// or char to compare with self.
  /// @returns True if both strings are not equal, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611416
  template <typename T>
  IsStringOrCharArrayOrChar<T, bool> operator!=(const T& rhs) const noexcept;

  /// @brief Checks if self is less than rhs, in lexicographical order.
  /// @tparam T The type of element in the String.
  /// @param[in] rhs Is the iox::cxx::FixedString, std::string, char array
  /// or char to compare with self.
  /// @returns True if self is less than rhs, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611417
  template <typename T>
  IsStringOrCharArrayOrChar<T, bool> operator<(const T& rhs) const noexcept;

  /// @brief Checks if self is less than or equal to rhs, in lexicographical order.
  /// @tparam T The type of element in the String.
  /// @param[in] rhs Is the iox::cxx::FixedString, std::string, char array
  /// or char to compare with self
  /// @returns True if self is less than or equal to rhs, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611418
  template <typename T>
  IsStringOrCharArrayOrChar<T, bool> operator<=(const T& rhs) const noexcept;

  /// @brief Checks if self is greater than rhs, in lexicographical order.
  /// @tparam T The type of element in the String.
  /// @param[in] rhs Is the iox::cxx::FixedString, std::string, char array
  /// or char to compare with self.
  /// @returns True if self is greater than rhs, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611419
  template <typename T>
  IsStringOrCharArrayOrChar<T, bool> operator>(const T& rhs) const noexcept;

  /// @brief Checks if self is greater than or equal to rhs, in lexicographical order.
  /// @tparam T The type of element in the String.
  /// @param[in] rhs Is the iox::cxx::FixedString, std::string, char array
  /// or char to compare with self.
  /// @returns True if self is greater than or equal to rhs, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611420
  template <typename T>
  IsStringOrCharArrayOrChar<T, bool> operator>=(const T& rhs) const noexcept;

  /// @brief Compares self and a char.
  /// @param[in] other Is the char to compare with self.
  /// @returns An integer < 0 if the first character that does not match has a lower value
  /// in self than in other, 0 if the contents of both strings are equal, an integer > 0
  /// if the first character that does not match has a greater value in self than in other.
  /// @note The logic is the same as in the other compare method with other treated as a
  /// FixedString with size 1.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611421
  int64_t compare(char other) const noexcept;

  /// @brief Returns a pointer to the char array of self.
  /// @returns A pointer to the char array of self.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611422
  const char* c_str() const noexcept;

  /// @brief Returns the number of characters stored in the FixedString.
  /// @returns The number of characters stored in the FixedString.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611423
  constexpr uint64_t size() const noexcept;

  /// @brief Returns the maximum number of characters that can be stored in the FixedString.
  /// @returns The maximum number of characters that can be stored in the FixedString.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611439
  static constexpr uint64_t capacity() noexcept;

  /// @brief Returns if the FixedString is empty or not.
  /// @returns True if size() == 0 otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611424
  constexpr bool empty() const noexcept;

  /// @brief Clears the content of the FixedString.
  /// @returns Remove all elements from the string.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611425
  constexpr void clear() noexcept;

  /// @brief Converts the FixedString to a std::string.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611426
  operator std::string() const noexcept;

  /// @brief Since there are two valid options for what should happen when appending a
  /// string larger than this' capacity (failing or truncating), the fixed string does not
  /// support operator+=; use append for truncating or unsafe_append for failing in that case.
  /// @tparam T The type of element in the String.
  /// @param[in] t Is the fixed FixedString/FixedString literal to append.
  /// @returns Reference to this string.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611427
  template <typename T>
  FixedString& operator+=(const T& t) noexcept;

  /// @brief Appends a fixed FixedString or FixedString literal to the end of this.
  /// If this' capacity is too small for appending the whole FixedString (literal)
  /// the remainder of the characters are truncated.
  /// @tparam T The type of element in the String.
  /// @param[in] TruncateToCapacity_t Is a compile time variable which is used to make
  /// the user aware of the possible truncation.
  /// @param[in] t Is the fixed FixedString/FixedString literal to append.
  /// @returns The reference to self.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 613228
  template <typename T>
  IsCxxStringOrCharArray<T, FixedString&> append(TruncateToCapacity_t, const T& t) noexcept;

  /// @brief Appends a fixed FixedString or FixedString literal to the end of this.
  /// The appending fails if the sum of both sizes is greater than this' capacity.
  /// @tparam T The type of element in the String.
  /// @param[in] t Is the fixed FixedString/FixedString literal to append.
  /// @returns True if the appending succeeds, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611428
  template <typename T>
  IsCxxStringOrCharArray<T, bool> unsafe_append(const T& t) noexcept;

  /// @brief Inserts a cxx:FixedString or char array in the range [str[0], str[count])
  /// at position pos. The insertion fails if the FixedString capacity would be exceeded
  /// or pos is greater than the FixedString size or count is greater than the
  /// FixedString to be inserted.
  /// @tparam T The type of element in the String.
  /// @param[in] pos Position at which the FixedString shall be inserted.
  /// @param[in] str The cxx::FixedString or char array to be inserted.
  /// @param[in] count The number of characters to be inserted.
  /// @returns True if the insertion was successful, otherwise false.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611429
  template <typename T>
  IsCxxStringOrCharArray<T, bool> insert(const uint64_t pos, const T& str, const uint64_t count) noexcept;

  /// @brief Creates a substring containing the characters from pos until count;
  /// if pos+count is greater than the size of the original FixedString the returned
  /// substring only contains the characters from pos until size();
  /// nullopt is returned if pos is greater than the size of the original FixedString.
  /// @param[in] pos Is the position of the first character used for the substring.
  /// @param[in] count Is the requested length of the substring.
  /// @returns An Optional containing the substring, nullopt if pos is greater
  /// than the size of the original FixedString.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611430
  Optional<FixedString<Capacity>> substr(const uint64_t pos, const uint64_t count) const noexcept;

  /// @brief Creates a substring containing the characters from pos until size();
  /// nullopt is returned if pos is greater than the size of the original FixedString.
  /// @param[in] pos Is the position of the first character used for the substring.
  /// @returns An Optional containing the substring, nullopt if pos is greater
  /// than the size of the original FixedString.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611431
  Optional<FixedString<Capacity>> substr(const uint64_t pos = 0U) const noexcept;

  /// @brief Finds the first occurence of the given character sequence;
  /// returns the position of the first character of the found substring,
  /// returns nullopt if no substring is found or if pos is greater than this' size.
  /// @tparam T The type of element in the String.
  /// @param[in] t Is the character sequence to search for; must be a cxx::FixedString, FixedString
  /// literal or std::string.
  /// @param[in] pos Is the position at which to start the search.
  /// @returns An Optional containing the position of the first character
  /// of the found substring, nullopt if no substring is found.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611432
  template <typename T>
  IsStringOrCharArray<T, Optional<uint64_t>> find(const T& t, const uint64_t pos = 0U) const noexcept;

  /// @brief Finds the first occurence of a character equal to one of the characters
  /// of the given character sequence and returns its position; returns nullopt if no
  /// character is found or if pos is greater than this' size.
  /// @tparam T The type of element in the String.
  /// @param[in] t Is the character sequence to search for; must be a cxx::FixedString, FixedString
  /// literal or std::string.
  /// @param[in] pos Is the position at which to start the search
  /// @returns An Optional containing the position of the first character equal to one of
  /// the characters of the given character sequence, nullopt if no character is found.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611433
  template <typename T>
  IsStringOrCharArray<T, Optional<uint64_t>> find_first_of(const T& t, const uint64_t pos = 0U) const
      noexcept;

  /// @brief Finds the last occurence of a character equal to one of the characters of the
  /// given character sequence and returns its position; returns nullopt if no character is found.
  /// @tparam T The type of element in the String.
  /// @param[in] t Is the character sequence to search for; must be a cxx::FixedString,
  /// FixedString literal or std::string.
  /// @param[in] pos Is the position at which to finish the search.
  /// @returns An Optional containing the position of the last character equal to one of
  /// the characters of the given character sequence, nullopt if no character is found.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611434
  template <typename T>
  IsStringOrCharArray<T, Optional<uint64_t>> find_last_of(const T& t, const uint64_t pos = Capacity) const
      noexcept;

  /// @brief It returns a reference to the character stored at pos.
  /// @param[in] pos Position of character to return.
  /// @returns The reference to the character.
  /// @note Out of bounds access leads to program termination.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611435
  constexpr char& at(const uint64_t pos) noexcept;

  /// @brief It returns a reference to the character stored at pos.
  /// @param[in] pos Position of character to return.
  /// @returns The const reference to the character.
  /// @note Out of bounds access leads to program termination.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611436
  constexpr const char& at(const uint64_t pos) const noexcept;

  /// @brief It returns a reference to the character stored at pos.
  /// @param[in] pos Position of the character to return.
  /// @returns The reference to the character.
  /// @note out of bounds access leads to program termination.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611437
  constexpr char& operator[](const uint64_t pos) noexcept;

  /// @brief It returns a const reference to the character stored at pos.
  /// @param[in] pos Position of the character to return.
  /// @returns The const reference to the character.
  /// @note Out of bounds access leads to program termination.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611438
  constexpr const char& operator[](const uint64_t pos) const noexcept;

  /// @brief Declaration of friend class FixedString.
  /// @tparam N A fixed-length string of N bytes (neither characters nor code points).
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  template <uint64_t N>
  friend class FixedString;

  /// @brief Concatenates two fixed strings/string literals.
  /// @tparam T1 The template type parameter to first object.
  /// @tparam T2 The template type parameter to second object.
  /// @param[in] t1 The fixed strings.
  /// @param[in] t2 The string literals to concatenate.
  /// @returns A new fixed string with capacity equal to the sum of the capacities of the
  /// concatenated strings.
  /// @trace REQ_AP_EC_CORE_0002
  /// @archtrace 609886
  /// @design 611827
  template <typename T1, typename T2>
  friend typename std::enable_if<(is_char_array<T1>::value || is_cxx_string<T1>::value)
          && (is_char_array<T2>::value || is_cxx_string<T2>::value),
      FixedString<GetCapa<T1>::capa + GetCapa<T2>::capa>>::type
      concatenate(const T1& t1, const T2& t2) noexcept;

 private:
  /// @brief It copies rhs fixed FixedString to this with compile time
  /// check whether rhs capacity is less than or equal to this' capacity.
  /// @tparam N A fixed-length string of N bytes (neither characters nor code points).
  /// @param[in] rhs Is the copy origin.
  /// @returns The reference to self.
  /// @design 611403
  template <uint64_t N>
  FixedString& copy(const FixedString<N>& rhs) noexcept;

  /// @brief It moves rhs fixed FixedString to this with compile time
  /// check whether rhs capacity is less than or equal to this' capacity.
  /// @tparam N A fixed-length string of N bytes (neither characters nor code points).
  /// @param[in] rhs Is the move origin.
  /// @returns The reference to self.
  /// @design 611404
  template <uint64_t N>
  FixedString& move(FixedString<N>&& rhs) noexcept;

  char m_rawstring[Capacity + 1U]{'\0'};
  uint64_t m_rawstringSize{0U};
};

/// @brief Checks if a lhs std::string, char array or char is equal to a rhs iox::cxx::FixedString.
/// @tparam T The type of element in the String.
/// @tparam Capacity The capacity of the given string.
/// @param[in] lhs Is the std::string, char array or char.
/// @param[in] rhs Is the iox::cxx::FixedString.
/// @returns True if the contents of lhs and rhs are equal, otherwise false.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
/// @design 611830
template <typename T, uint64_t Capacity>
IsStdStringOrCharArrayOrChar<T, bool> operator==(const T& lhs, const FixedString<Capacity>& rhs) noexcept;

/// @brief Checks if a lhs std::string, char array or char is not equal to a rhs iox::cxx::FixedString.
/// @tparam T The type of element in the String.
/// @tparam Capacity The capacity of the given string.
/// @param[in] lhs Is the std::string, char array or char.
/// @param[in] rhs Is the iox::cxx::FixedString.
/// @returns True if the contents of lhs and rhs are not equal, otherwise false.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
/// @design 611831
template <typename T, uint64_t Capacity>
IsStdStringOrCharArrayOrChar<T, bool> operator!=(const T& lhs, const FixedString<Capacity>& rhs) noexcept;

/// @brief Checks if a lhs std::string, char array or char is less than a rhs iox::cxx::FixedString.
/// @tparam T The type of element in the String.
/// @tparam Capacity The capacity of the given string.
/// @param[in] lhs Is the std::string, char array or char.
/// @param[in] rhs Is the iox::cxx::FixedString.
/// @returns True if lhs is less than rhs, otherwise false.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
/// @design 611832
template <typename T, uint64_t Capacity>
IsStdStringOrCharArrayOrChar<T, bool> operator<(const T& lhs, const FixedString<Capacity>& rhs) noexcept;

/// @brief Checks if a lhs std::string, char array or char is less than or equal to a rhs
/// iox::cxx::FixedString.
/// @tparam T The type of element in the String.
/// @tparam Capacity The capacity of the given string.
/// @param[in] lhs Is the std::string, char array or char.
/// @param[in] rhs Is the iox::cxx::FixedString.
/// @returns True if lhs is less than or equal to rhs, otherwise false.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
/// @design 611833
template <typename T, uint64_t Capacity>
IsStdStringOrCharArrayOrChar<T, bool> operator<=(const T& lhs, const FixedString<Capacity>& rhs) noexcept;

/// @brief Checks if a lhs std::string, char array or char is greater than a rhs iox::cxx::FixedString.
/// @tparam T The type of element in the String.
/// @tparam Capacity The capacity of the given string.
/// @param[in] lhs Is the std::string, char array or char.
/// @param[in] rhs Is the iox::cxx::FixedString.
/// @returns True if lhs is greater than rhs, otherwise false.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
/// @design 611834
template <typename T, uint64_t Capacity>
IsStdStringOrCharArrayOrChar<T, bool> operator>(const T& lhs, const FixedString<Capacity>& rhs) noexcept;

/// @brief Checks if a lhs std::string, char array or char is greater than or equal to a rhs
/// iox::cxx::FixedString.
/// @tparam T The type of element in the String.
/// @tparam Capacity The capacity of the given string.
/// @param[in] lhs Is the std::string, char array or char.
/// @param[in] rhs Is the iox::cxx::FixedString.
/// @returns True if lhs is greater than or equal to rhs, otherwise false.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
/// @design 611835
template <typename T, uint64_t Capacity>
IsStdStringOrCharArrayOrChar<T, bool> operator>=(const T& lhs, const FixedString<Capacity>& rhs) noexcept;

/// @brief Outputs the fixed FixedString on stream.
/// @tparam Capacity The capacity of the given string.
/// @param[in] stream Is the output stream.
/// @param[in] str Is the fixed FixedString.
/// @returns The stream output of the fixed FixedString.
/// @trace REQ_AP_EC_CORE_0002
/// @archtrace 609886
/// @design 611836
template <uint64_t Capacity>
inline std::ostream& operator<<(std::ostream& stream, const FixedString<Capacity>& str) noexcept;

} // namespace core
} // namespace ara
#include "ara/core/fixed_string.inl"

#endif // FIXED_STRING_H
