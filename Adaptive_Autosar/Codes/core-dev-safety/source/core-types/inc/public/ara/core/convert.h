// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : convert.h
//
// Purpose     : This file provides the implementation of class convert.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_CONVERT_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_CONVERT_H_

#include "iceoryx_hoofs/cxx/string.hpp"
#include <iceoryx_hoofs/posix_wrapper/posix_call.hpp>

#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
namespace ara
{
namespace core
{
/// @brief Collection of static methods for conversion from and to string.
/// @code
///     std::string number      = cxx::convert::toString(123);
///     std::string someClass   = cxx::convert::toString(someToStringConvertableObject);
///
///     int i;
///     unsigned int a;
///     if ( cxx::convert::fromString("123", i) ) {}  // will succeed
///     if ( cxx::convert::fromString("-123", a) ) {} // will fail since -123 is not unsigned
/// @endcode
/// @design 642376
class convert
{
 public:
  enum class NumberType
  {
    INTEGER,
    UNSIGNED_INTEGER,
    FLOAT
  };

  static constexpr int32_t STRTOULL_BASE = 10;

  /// @brief Converts every type which is either a pod (plain old data) type or is convertable
  /// to a string (this means that the operator std::string() is defined)
  /// @tparam Source Type of the value which should be converted to a string.
  /// @param[in] t A Value which should be converted to a string.
  /// @return A string representation of t.
  /// @design 642377
  template <typename Source>
  static typename std::enable_if<!std::is_convertible<Source, std::string>::value, std::string>::type
      toString(const Source& t) noexcept;

  /// @brief Converts every type which is either a pod (plain old data) type or is convertable
  /// to a string (this means that the operator std::string() is defined)
  /// @param Source Type of the value which should be converted to a string.
  /// @param[in] t A Value which should be converted to a string.
  /// @return A string representation of t.
  /// @design 642401
  template <typename Source>
  static typename std::enable_if<std::is_convertible<Source, std::string>::value, std::string>::type toString(
      const Source& t) noexcept;

  /// @brief Sets dest from a given string. If the conversion fails, false is
  /// returned and the value of dest is undefined.
  /// @param[in] v A string which contains the value of dest.
  /// @param[in] dest A destination to which the value should be written.
  /// @return false = if the conversion fails otherwise true.
  /// @design 642402
  template <typename Destination>
  static bool fromString(const char* v, Destination& dest) noexcept;

  /// @brief Checks if a given string v is a number.
  /// @param[in] v A string which contains the number.
  /// @param[in] type Is the expected contained type in v.
  /// @return true if the given string is a number, otherwise false.
  /// @design 642403
  static bool stringIsNumber(const char* v, const NumberType type) noexcept;

 private:
  /// @brief Checks if a given string v is a number with error message.
  /// @param[in] v A string which contains the number with error message.
  /// @param[in] type Is the expected contained type in v.
  /// @return true if the given string is a number with error message, otherwise false.
  static bool stringIsNumberWithErrorMessage(const char* v, const NumberType type) noexcept;
};

} // namespace core
} // namespace ara

#include "ara/core/convert.inl"

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_CONVERT_H_