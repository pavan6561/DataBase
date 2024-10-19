// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
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
#ifndef STRING_INTERNAL_HPP_
#define STRING_INTERNAL_HPP_

#include <cstdint>
#include <cstring>
#include <string>

namespace ara
{
namespace core
{
/// @brief FixedString implementation from the C++17 standard with C++11.
/// @tparam N The number of elements in the class.
template <uint64_t N>
class FixedString;

/// @brief Alias name for char Array.
/// @tparam N The number of elements in the class.
template <uint64_t N>
using charArray = char[N];

/// @brief Struct to get capacity of fixed string/string literal.
/// @tparam T The type of element in the class.
/// @note capa Is a dummy value for any type other than cxx::string and char.
template <typename T>
struct GetCapa
{
  static constexpr uint64_t capa = 0U;
};

/// @brief Struct to get capacity of fixed string.
/// @tparam N The number of elements in the class.
template <uint64_t N>
struct GetCapa<FixedString<N>>
{
  static constexpr uint64_t capa = N;
};

/// @brief Struct to get capacity of char.
/// @tparam N The number of elements in the class.
template <uint64_t N>
struct GetCapa<char[N]>
{
  static constexpr uint64_t capa = N - 1U;
};

/// @brief Struct to get size of fixed string/string literal/std::string.
/// @tparam T The type of element in the class.
template <typename T>
struct GetSize;

/// @brief Struct to get size of fixed string.
/// @tparam N The number of elements in the class.
template <uint64_t N>
struct GetSize<FixedString<N>>
{
  static uint64_t call(const FixedString<N>& data) noexcept
  {
    return data.size();
  }
};

/// @brief Struct to get size of char.
/// @tparam N The number of elements in the class.
template <uint64_t N>
struct GetSize<char[N]>
{
  static uint64_t call(const charArray<N>& data) noexcept
  {
    return strnlen(data, N);
  }
};

/// @brief Struct to get size of std::string.
template <>
struct GetSize<std::string>
{
  static uint64_t call(const std::string& data) noexcept
  {
    return data.size();
  }
};

/// @brief Struct to get a pointer to the char array of the fixed string/string literal/std::string.
/// @tparam T The type of element in the class.
template <typename T>
struct GetData;

/// @brief Struct to get data of fixed string.
/// @tparam N The number of elements in the class.
template <uint64_t N>
struct GetData<FixedString<N>>
{
  static const char* call(const FixedString<N>& data) noexcept
  {
    return data.c_str();
  }
};

/// @brief Struct to get data of char.
/// @tparam N The number of elements in the class.
template <uint64_t N>
struct GetData<char[N]>
{
  static const char* call(const charArray<N>& data) noexcept
  {
    return &data[0];
  }
};

/// @brief Struct to get data of std::string.
/// @tparam N The number of elements in the class.
template <>
struct GetData<std::string>
{
  static const char* call(const std::string& data) noexcept
  {
    return data.data();
  }
};

/// @brief Struct to get the sum of the capacities of fixed strings/string literals.
/// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
template <typename... Targs>
struct SumCapa;

/// @brief Struct to get the sum of the capacities.
template <>
struct SumCapa<>
{
  static constexpr uint64_t value = 0U;
};

/// @brief Struct to get the sum of the capacities of arguments to be passed.
/// @tparam T The type of element in the class.
/// @tparam Targs Is the template parameter pack for the perfectly forwarded arguments.
template <typename T, typename... Targs>
struct SumCapa<T, Targs...>
{
  static constexpr uint64_t value = GetCapa<T>::capa + SumCapa<Targs...>::value;
};

} // namespace core
} // namespace ara
#endif // STRING_INTERNAL_HPP
