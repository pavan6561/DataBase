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
#ifndef FIXED_STRING_INL
#define FIXED_STRING_INL

namespace ara
{
namespace core
{
template <uint64_t Capacity>
inline FixedString<Capacity>::FixedString(const FixedString& other) noexcept
{
    copy(other);
}

template <uint64_t Capacity>
inline FixedString<Capacity>::FixedString(FixedString&& other) noexcept
{
    move(std::move(other));
}

template <uint64_t Capacity>
inline FixedString<Capacity>& FixedString<Capacity>::operator=(const FixedString& rhs) noexcept
{
    if (this == &rhs)
    {
        return *this;
    }
    return copy(rhs);
}

template <uint64_t Capacity>
inline FixedString<Capacity>& FixedString<Capacity>::operator=(FixedString&& rhs) noexcept
{
    if (this == &rhs)
    {
        return *this;
    }
    return move(std::move(rhs));
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>::FixedString(const FixedString<N>& other) noexcept
{
    static_assert(N <= Capacity,
                  "Construction failed. The capacity of the given fixed FixedString is larger than the capacity of this.");
    copy(other);
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>::FixedString(FixedString<N>&& other) noexcept
{
    static_assert(N <= Capacity,
                  "Construction failed. The capacity of the given fixed FixedString is larger than the capacity of this.");
    move(std::move(other));
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>& FixedString<Capacity>::operator=(const FixedString<N>& rhs) noexcept
{
    static_assert(N <= Capacity,
                  "Assignment failed. The capacity of the given fixed FixedString is larger than the capacity of this.");
    return copy(rhs);
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>& FixedString<Capacity>::operator=(FixedString<N>&& rhs) noexcept
{
    static_assert(N <= Capacity,
                  "Assignment failed. The capacity of the given fixed FixedString is larger than the capacity of this.");
    return move(std::move(rhs));
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>::FixedString(const char (&other)[N]) noexcept
{
    *this = other;
}

template <uint64_t Capacity>
inline FixedString<Capacity>::FixedString(TruncateToCapacity_t, const char* const other) noexcept
    : FixedString(TruncateToCapacity, other, [&]() -> uint64_t { return other ? strnlen(other, Capacity) : 0U; }())
{
}

template <uint64_t Capacity>
inline FixedString<Capacity>::FixedString(TruncateToCapacity_t, const std::string& other) noexcept
    : FixedString(TruncateToCapacity, other.c_str(), other.size())
{
}

template <uint64_t Capacity>
inline FixedString<Capacity>::FixedString(TruncateToCapacity_t, const char* const other, const uint64_t count) noexcept
{
    if (other == nullptr)
    {
        clear();
    }
    else if (Capacity < count)
    {
        std::memcpy(&(m_rawstring[0]), other, Capacity);
        m_rawstring[Capacity] = '\0';
        m_rawstringSize = Capacity;
        std::cerr << "Constructor truncates the last " << count - Capacity << " characters of " << other
                  << ", because the char array length is larger than the capacity." << std::endl;
    }
    else
    {
        std::memcpy(&(m_rawstring[0]), other, count);
        m_rawstring[count] = '\0';
        m_rawstringSize = count;
    }
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>& FixedString<Capacity>::operator=(const char (&rhs)[N]) noexcept
{
    static_assert(N <= Capacity + 1U,
                  "Assignment failed. The given char array is larger than the capacity of the fixed FixedString.");

    if (c_str() == rhs)
    {
        return *this;
    }

    std::memcpy(&(m_rawstring[0]), rhs, N);

    m_rawstringSize = std::min(Capacity, static_cast<uint64_t>(strnlen(rhs, N)));
    m_rawstring[m_rawstringSize] = '\0';

    if (rhs[m_rawstringSize] != '\0')
    {
        std::cerr << "iox::cxx::FixedString: Assignment of array which is not zero-terminated! Last value of array "
                     "overwritten with 0!"
                  << std::endl;
    }
    return *this;
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>& FixedString<Capacity>::assign(const FixedString<N>& str) noexcept
{
    static_assert(N <= Capacity,
                  "Assignment failed. The capacity of the given fixed FixedString is larger than the capacity of this.");
    *this = str;
    return *this;
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>& FixedString<Capacity>::assign(const char (&str)[N]) noexcept
{
    *this = str;
    return *this;
}

template <uint64_t Capacity>
inline bool FixedString<Capacity>::unsafe_assign(const char* const str) noexcept
{
    if ((c_str() == str) || (str == nullptr))
    {
        return false;
    }
    const uint64_t strSize = strnlen(str, Capacity + 1U);
    if (Capacity < strSize)
    {
        std::cerr << "Assignment failed. The given cstring is larger (" << strSize << ") than the capacity ("
                  << Capacity << ") of the fixed FixedString." << std::endl;
        return false;
    }
    std::memcpy(&(m_rawstring[0]), str, strSize);
    m_rawstring[strSize] = '\0';
    m_rawstringSize = strSize;
    return true;
}

template <uint64_t Capacity>
inline bool FixedString<Capacity>::unsafe_assign(const std::string& str) noexcept
{
    uint64_t strSize = str.size();
    if (Capacity < strSize)
    {
        std::cerr << "Assignment failed. The given std::string is larger than the capacity of the fixed string."
                  << std::endl;
        return false;
    }
    std::memcpy(&(m_rawstring[0]), str.c_str(), strSize);
    m_rawstring[strSize] = '\0';
    m_rawstringSize = strSize;
    return true;
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArray<T, int64_t> FixedString<Capacity>::compare(const T& other) const noexcept
{
    uint64_t otherSize = GetSize<T>::call(other);
    auto result = memcmp(c_str(), GetData<T>::call(other), std::min(m_rawstringSize, otherSize));
    if (result == 0)
    {
        if (m_rawstringSize < otherSize)
        {
            return -1;
        }
        else if (m_rawstringSize > otherSize)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return result;
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArrayOrChar<T, bool> FixedString<Capacity>::operator==(const T& rhs) const noexcept
{
    return (compare(rhs) == 0);
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArrayOrChar<T, bool> FixedString<Capacity>::operator!=(const T& rhs) const noexcept
{
    return (compare(rhs) != 0);
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArrayOrChar<T, bool> FixedString<Capacity>::operator<(const T& rhs) const noexcept
{
    return (compare(rhs) < 0);
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArrayOrChar<T, bool> FixedString<Capacity>::operator<=(const T& rhs) const noexcept
{
    return (compare(rhs) <= 0);
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArrayOrChar<T, bool> FixedString<Capacity>::operator>(const T& rhs) const noexcept
{
    return (compare(rhs) > 0);
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArrayOrChar<T, bool> FixedString<Capacity>::operator>=(const T& rhs) const noexcept
{
    return (compare(rhs) >= 0);
}

template <typename T, uint64_t Capacity>
inline IsStdStringOrCharArrayOrChar<T, bool> operator==(const T& lhs, const FixedString<Capacity>& rhs) noexcept
{
    return (rhs.compare(lhs) == 0);
}

template <typename T, uint64_t Capacity>
inline IsStdStringOrCharArrayOrChar<T, bool> operator!=(const T& lhs, const FixedString<Capacity>& rhs) noexcept
{
    return (rhs.compare(lhs) != 0);
}

template <typename T, uint64_t Capacity>
inline IsStdStringOrCharArrayOrChar<T, bool> operator<(const T& lhs, const FixedString<Capacity>& rhs) noexcept
{
    return (rhs.compare(lhs) > 0);
}

template <typename T, uint64_t Capacity>
inline IsStdStringOrCharArrayOrChar<T, bool> operator<=(const T& lhs, const FixedString<Capacity>& rhs) noexcept
{
    return (rhs.compare(lhs) >= 0);
}

template <typename T, uint64_t Capacity>
inline IsStdStringOrCharArrayOrChar<T, bool> operator>(const T& lhs, const FixedString<Capacity>& rhs) noexcept
{
    return (rhs.compare(lhs) < 0);
}

template <typename T, uint64_t Capacity>
inline IsStdStringOrCharArrayOrChar<T, bool> operator>=(const T& lhs, const FixedString<Capacity>& rhs) noexcept
{
    return (rhs.compare(lhs) <= 0);
}

template <uint64_t Capacity>
inline int64_t FixedString<Capacity>::compare(char other) const noexcept
{
    auto result = memcmp(c_str(), &other, 1U);
    if (result == 0)
    {
        if (empty())
        {
            return -1;
        }
        else if (m_rawstringSize > 1U)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return result;
}

template <uint64_t Capacity>
inline const char* FixedString<Capacity>::c_str() const noexcept
{
    return m_rawstring;
}

template <uint64_t Capacity>
inline constexpr uint64_t FixedString<Capacity>::size() const noexcept
{
    return m_rawstringSize;
}

template <uint64_t Capacity>
inline constexpr uint64_t FixedString<Capacity>::capacity() noexcept
{
    return Capacity;
}

template <uint64_t Capacity>
inline constexpr bool FixedString<Capacity>::empty() const noexcept
{
    return m_rawstringSize == 0U;
}

template <uint64_t Capacity>
inline constexpr void FixedString<Capacity>::clear() noexcept
{
    m_rawstring[0U] = '\0';
    m_rawstringSize = 0U;
}

template <uint64_t Capacity>
inline FixedString<Capacity>::operator std::string() const noexcept
{
    return std::string(c_str());
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>& FixedString<Capacity>::copy(const FixedString<N>& rhs) noexcept
{
    static_assert(N <= Capacity,
                  "Assignment failed. The capacity of the given fixed FixedString is larger than the capacity of this.");
    uint64_t strSize = rhs.size();
    std::memcpy(&(m_rawstring[0]), rhs.c_str(), strSize);
    m_rawstring[strSize] = '\0';
    m_rawstringSize = strSize;
    return *this;
}

template <uint64_t Capacity>
template <uint64_t N>
inline FixedString<Capacity>& FixedString<Capacity>::move(FixedString<N>&& rhs) noexcept
{
    static_assert(N <= Capacity,
                  "Assignment failed. The capacity of the given fixed FixedString is larger than the capacity of this.");
    uint64_t strSize = rhs.size();
    std::memcpy(&(m_rawstring[0]), rhs.c_str(), strSize);
    m_rawstring[strSize] = '\0';
    m_rawstringSize = strSize;
    rhs.clear();
    return *this;
}

template <uint64_t Capacity>
inline std::ostream& operator<<(std::ostream& stream, const FixedString<Capacity>& str) noexcept
{
    stream << str.c_str();
    return stream;
}

template <uint64_t Capacity>
template <typename T>
inline FixedString<Capacity>& FixedString<Capacity>::operator+=(const T&) noexcept
{
    static_assert(iox::cxx::always_false_v<FixedString<Capacity>>,
                  "operator += is not supported by cxx::FixedString, please use append or unsafe_append instead");
    return *this;
}

template <typename T1, typename T2>
inline typename std::enable_if<(is_char_array<T1>::value || is_cxx_string<T1>::value)
                                   && (is_char_array<T2>::value || is_cxx_string<T2>::value),
                               FixedString<GetCapa<T1>::capa + GetCapa<T2>::capa>>::type
concatenate(const T1& t1, const T2& t2) noexcept
{
    uint64_t size1 = GetSize<T1>::call(t1);
    uint64_t size2 = GetSize<T2>::call(t2);
    using NewStringType = FixedString<GetCapa<T1>::capa + GetCapa<T2>::capa>;
    NewStringType newString;
    std::memcpy(&(newString.m_rawstring[0]), GetData<T1>::call(t1), size1);
    std::memcpy(&(newString.m_rawstring[0]) + size1, GetData<T2>::call(t2), size2);
    newString.m_rawstring[size1 + size2] = '\0';
    newString.m_rawstringSize = size1 + size2;

    return newString;
}

template <typename T1, typename T2, typename... Targs>
inline typename std::enable_if<(is_char_array<T1>::value || is_cxx_string<T1>::value)
                                   && (is_char_array<T2>::value || is_cxx_string<T2>::value),
                               FixedString<SumCapa<T1, T2, Targs...>::value>>::type
concatenate(const T1& t1, const T2& t2, const Targs&... targs) noexcept
{
    return concatenate(concatenate(t1, t2), targs...);
}

template <typename T1, typename T2>
inline typename std::enable_if<(is_char_array<T1>::value && is_cxx_string<T2>::value)
                                   || (is_cxx_string<T1>::value && is_char_array<T2>::value)
                                   || (is_cxx_string<T1>::value && is_cxx_string<T2>::value),
                               FixedString<GetCapa<T1>::capa + GetCapa<T2>::capa>>::type
operator+(const T1& t1, const T2& t2) noexcept
{
    return concatenate(t1, t2);
}

template <uint64_t Capacity>
template <typename T>
inline IsCxxStringOrCharArray<T, bool> FixedString<Capacity>::unsafe_append(const T& t) noexcept
{
    uint64_t tSize = GetSize<T>::call(t);
    const char* tData = GetData<T>::call(t);
    uint64_t clampedTSize = std::min(Capacity - m_rawstringSize, tSize);

    if (tSize > clampedTSize)
    {
        std::cerr << "Appending failed because the sum of sizes exceeds this' capacity." << std::endl;
        return false;
    }

    std::memcpy(&(m_rawstring[m_rawstringSize]), tData, clampedTSize);
    m_rawstringSize += clampedTSize;
    m_rawstring[m_rawstringSize] = '\0';
    return true;
}

template <uint64_t Capacity>
template <typename T>
inline IsCxxStringOrCharArray<T, FixedString<Capacity>&> FixedString<Capacity>::append(TruncateToCapacity_t, const T& t) noexcept
{
    uint64_t tSize = GetSize<T>::call(t);
    const char* tData = GetData<T>::call(t);
    uint64_t clampedTSize = std::min(Capacity - m_rawstringSize, tSize);

    std::memcpy(&(m_rawstring[m_rawstringSize]), tData, clampedTSize);
    if (tSize > clampedTSize)
    {
        std::cerr << "The last " << tSize - Capacity + m_rawstringSize << " characters of " << tData
                  << " are truncated, because the length is larger than the capacity." << std::endl;
    }

    m_rawstringSize += clampedTSize;
    m_rawstring[m_rawstringSize] = '\0';
    return *this;
}

template <uint64_t Capacity>
template <typename T>
inline IsCxxStringOrCharArray<T, bool>
FixedString<Capacity>::insert(const uint64_t pos, const T& str, const uint64_t count) noexcept
{
    if (count > GetSize<T>::call(str))
    {
        return false;
    }
    const auto new_size = m_rawstringSize + count;
    // check if the new size would exceed capacity or a size overflow occured
    if (new_size > Capacity || new_size < m_rawstringSize)
    {
        return false;
    }

    if (pos > m_rawstringSize)
    {
        return false;
    }
    std::memmove(&m_rawstring[pos + count], &m_rawstring[pos], m_rawstringSize - pos);
    std::memcpy(&m_rawstring[pos], GetData<T>::call(str), count);

    m_rawstring[new_size] = '\0';
    m_rawstringSize = new_size;

    return true;
}

template <uint64_t Capacity>
inline Optional<FixedString<Capacity>> FixedString<Capacity>::substr(const uint64_t pos, const uint64_t count) const noexcept
{
    if (pos > m_rawstringSize)
    {
        return nullopt;
    }

    uint64_t length = std::min(count, m_rawstringSize - pos);
    FixedString subString;
    std::memcpy(&(subString.m_rawstring[0]), &m_rawstring[pos], length);
    subString.m_rawstring[length] = '\0';
    subString.m_rawstringSize = length;
    return subString;
}

template <uint64_t Capacity>
inline Optional<FixedString<Capacity>> FixedString<Capacity>::substr(const uint64_t pos) const noexcept
{
    return substr(pos, m_rawstringSize);
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArray<T, Optional<uint64_t>> FixedString<Capacity>::find(const T& t, const uint64_t pos) const noexcept
{
    if (pos > m_rawstringSize)
    {
        return nullopt;
    }
    const char* found = std::strstr(c_str() + pos, GetData<T>::call(t));
    if (found == nullptr)
    {
        return nullopt;
    }
    return (found - c_str());
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArray<T, Optional<uint64_t>> FixedString<Capacity>::find_first_of(const T& t,
                                                                                  const uint64_t pos) const noexcept
{
    if (pos > m_rawstringSize)
    {
        return nullopt;
    }
    const char* found = nullptr;
    const char* data = GetData<T>::call(t);
    for (auto p = pos; p < m_rawstringSize; ++p)
    {
        found = std::strchr(data, m_rawstring[p]);
        if (found != nullptr)
        {
            return p;
        }
    }
    return nullopt;
}

template <uint64_t Capacity>
template <typename T>
inline IsStringOrCharArray<T, Optional<uint64_t>> FixedString<Capacity>::find_last_of(const T& t,
                                                                                 const uint64_t pos) const noexcept
{
    if (m_rawstringSize == 0U)
    {
        return nullopt;
    }

    auto p = pos;
    if (m_rawstringSize - 1U < p)
    {
        p = m_rawstringSize - 1U;
    }
    const char* found = nullptr;
    const char* data = GetData<T>::call(t);
    for (; p > 0U; --p)
    {
        found = std::strchr(data, m_rawstring[p]);
        if (found != nullptr)
        {
            return p;
        }
    }
    found = std::strchr(data, m_rawstring[p]);
    if (found != nullptr)
    {
        return 0U;
    }
    return nullopt;
}

template <uint64_t Capacity>
inline constexpr char& FixedString<Capacity>::at(const uint64_t pos) noexcept
{
    return const_cast<char&>(const_cast<const FixedString<Capacity>*>(this)->at(pos));
}

template <uint64_t Capacity>
inline constexpr const char& FixedString<Capacity>::at(const uint64_t pos) const noexcept
{
    iox::cxx::Expects(pos < size() && "Out of bounds access!");
    return m_rawstring[pos];
}

template <uint64_t Capacity>
inline constexpr char& FixedString<Capacity>::operator[](const uint64_t pos) noexcept
{
    return at(pos);
}

template <uint64_t Capacity>
inline constexpr const char& FixedString<Capacity>::operator[](const uint64_t pos) const noexcept
{
    return at(pos);
}
} // namespace core
} // namespace ara

#endif // FIXED_STRING_INL
