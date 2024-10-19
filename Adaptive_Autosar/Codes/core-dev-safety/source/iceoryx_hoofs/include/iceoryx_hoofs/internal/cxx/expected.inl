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
#ifndef IOX_HOOFS_CXX_EXPECTED_INL
#define IOX_HOOFS_CXX_EXPECTED_INL

#include "iceoryx_hoofs/cxx/expected.hpp"

namespace iox
{
namespace cxx
{
namespace internal
{
template <typename... T>
struct IsOptional : std::false_type
{
};
template <typename T>
struct IsOptional<iox::cxx::optional<T>> : std::true_type
{
};
} // namespace internal

template <typename T>
inline success<T>::success(const T& t) noexcept
    : value(t)
{
}

template <typename T>
inline success<T>::success(T&& t) noexcept
    : value(std::forward<T>(t))
{
}

template <typename T>
template <typename... Targs>
inline success<T>::success(Targs&&... args) noexcept
    : value(std::forward<Targs>(args)...)
{
}

template <typename T>
inline error<T>::error(const T& t) noexcept
    : value(t)
{
}

template <typename T>
inline error<T>::error(T&& t) noexcept
    : value(std::forward<T>(t))
{
}

template <typename T>
template <typename... Targs>
inline error<T>::error(Targs&&... args) noexcept
    : value(std::forward<Targs>(args)...)
{
}


template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(variant<ValueType, ErrorType>&& f_store) noexcept
    : m_store(std::move(f_store))
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(const success<ValueType>& successValue) noexcept
    : m_store(in_place_index<VALUE_INDEX>(), successValue.value)
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(success<ValueType>&& successValue) noexcept
    : m_store(in_place_index<VALUE_INDEX>(), std::move(successValue.value))
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(const error<ErrorType>& errorValue) noexcept
    : m_store(in_place_index<ERROR_INDEX>(), errorValue.value)
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(error<ErrorType>&& errorValue) noexcept
    : m_store(in_place_index<ERROR_INDEX>(), std::move(errorValue.value))
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(expected<ValueType, ErrorType>&& rhs) noexcept
{
    *this = std::move(rhs);
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>&
expected<ValueType, ErrorType>::operator=(expected<ValueType, ErrorType>&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_store = std::move(rhs.m_store);
    }
    return *this;
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>&
expected<ValueType, ErrorType>::operator=(const expected<ValueType, ErrorType>& rhs) noexcept
{
if (this != &rhs)
    {
        m_store =rhs.m_store;
    }
    return *this;
}



template <typename ValueType, typename ErrorType>
template <typename... Targs>
inline expected<ValueType, ErrorType> expected<ValueType, ErrorType>::create_value(Targs&&... args) noexcept
{
    expected<ValueType, ErrorType> returnValue{
        variant<ValueType, ErrorType>(in_place_index<VALUE_INDEX>(), std::forward<Targs>(args)...)};

    return returnValue;
}

template <typename ValueType, typename ErrorType>
template <typename... Targs>
inline expected<ValueType, ErrorType> expected<ValueType, ErrorType>::create_error(Targs&&... args) noexcept
{
    expected<ValueType, ErrorType> returnValue{
        variant<ValueType, ErrorType>(in_place_index<ERROR_INDEX>(), std::forward<Targs>(args)...)};

    return returnValue;
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::operator bool() const noexcept
{
    return !has_error();
}

template <typename ValueType, typename ErrorType>
inline bool expected<ValueType, ErrorType>::has_error() const noexcept
{
    return m_store.index() == ERROR_INDEX;
}

template <typename ValueType, typename ErrorType>
inline ErrorType&& expected<ValueType, ErrorType>::get_error() && noexcept
{
    return std::move(*m_store.template get_at_index<ERROR_INDEX>());
}

template <typename ValueType, typename ErrorType>
inline ErrorType& expected<ValueType, ErrorType>::get_error() & noexcept
{
    return *m_store.template get_at_index<ERROR_INDEX>();
}

template <typename ValueType, typename ErrorType>
inline ValueType&& expected<ValueType, ErrorType>::value() && noexcept
{
    return std::move(*m_store.template get_at_index<VALUE_INDEX>());
}

template <typename ValueType, typename ErrorType>
inline const ValueType& expected<ValueType, ErrorType>::value() const& noexcept
{
    return *m_store.template get_at_index<VALUE_INDEX>();
}

template <typename ValueType, typename ErrorType>
inline ValueType& expected<ValueType, ErrorType>::value() & noexcept
{
    return *m_store.template get_at_index<VALUE_INDEX>();
}

template <typename ErrorType>
inline const ErrorType& expected<ErrorType>::get_error() const& noexcept
{
    return *m_store.template get_at_index<ERROR_INDEX>();
}

template <typename ValueType, typename ErrorType>
inline ValueType* expected<ValueType, ErrorType>::operator->() noexcept
{
    return m_store.template get_at_index<VALUE_INDEX>();
}

template <typename ValueType, typename ErrorType>
inline const ValueType* expected<ValueType, ErrorType>::operator->() const noexcept
{
    return const_cast<expected*>(this)->operator->();
}

template <typename ValueType, typename ErrorType>
inline ValueType& expected<ValueType, ErrorType>::operator*() noexcept
{
    return *m_store.template get_at_index<VALUE_INDEX>();
}

template <typename ValueType, typename ErrorType>
inline const ValueType& expected<ValueType, ErrorType>::operator*() const noexcept
{
    return const_cast<expected*>(this)->operator*();
}

template <typename ValueType, typename ErrorType>
template <typename T>
inline expected<ValueType, ErrorType>::operator expected<T>() const noexcept
{
    if (has_error())
    {
        return error<ErrorType>(get_error());
    }
    return success<>();
}

template <typename ValueType, typename ErrorType>
inline optional<ValueType> expected<ValueType, ErrorType>::to_optional() const noexcept
{
    optional<ValueType> returnValue;
    if (!has_error())
    {
        returnValue.emplace(value());
    }
    return returnValue;
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::operator optional<ValueType>() const noexcept
{
    return this->to_optional();
}
// expected<ErrorType>

template <typename ErrorType>
inline expected<ErrorType>::expected(variant<ErrorType>&& f_store) noexcept
    : m_store(std::move(f_store))
{
}

template <typename ErrorType>
inline expected<ErrorType>::expected(const success<void>&) noexcept
{
}

template <typename ErrorType>
inline expected<ErrorType>::expected(expected<ErrorType>&& rhs) noexcept
{
    *this = std::move(rhs);
}

template <typename ErrorType>
inline expected<ErrorType>& expected<ErrorType>::operator=(expected<ErrorType>&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_store = std::move(rhs.m_store);
    }
    return *this;
}

template <typename ErrorType>
inline expected<ErrorType>& expected<ErrorType>::operator=(const expected<ErrorType>& rhs) noexcept
{
    if (this != &rhs)
    {
        m_store = rhs.m_store;
    }
    return *this;

}



template <typename ErrorType>
inline expected<ErrorType>::expected(const error<ErrorType>& errorValue) noexcept
    : m_store(in_place_index<ERROR_INDEX>(), errorValue.value)
{
}

template <typename ErrorType>
inline expected<ErrorType>::expected(error<ErrorType>&& errorValue) noexcept
    : m_store(in_place_index<ERROR_INDEX>(), std::move(errorValue.value))
{
}

#if defined(_WIN32)
template <typename ErrorType>
template <typename ValueType>
inline expected<ErrorType>::expected(const expected<ValueType, ErrorType>& rhs) noexcept
{
    if (rhs.has_error())
    {
        m_store.emplace_at_index<ERROR_INDEX>(rhs.get_error());
    }
}

template <typename ErrorType>
template <typename ValueType>
inline expected<ErrorType>::expected(expected<ValueType, ErrorType>&& rhs) noexcept
{
    if (rhs.has_error())
    {
        m_store.emplace_at_index<ERROR_INDEX>(std::move(rhs.get_error()));
    }
}

template <typename ErrorType>
template <typename ValueType>
inline expected<ErrorType>& expected<ErrorType>::operator=(const expected<ValueType, ErrorType>& rhs) noexcept
{
    if (has_error() && rhs.has_error())
    {
        m_store.get_error() = rhs.get_error();
    }
    else if (rhs.has_error())
    {
        m_store = variant<ErrorType>(in_place_type<ErrorType>(), rhs.get_error());
    }
}

template <typename ErrorType>
template <typename ValueType>
inline expected<ErrorType>& expected<ErrorType>::operator=(expected<ValueType, ErrorType>&& rhs) noexcept
{
    if (has_error() && rhs.has_error())
    {
        m_store.get_error() = std::move(rhs.get_error());
    }
    else if (rhs.has_error())
    {
        m_store = variant<ErrorType>(in_place_type<ErrorType>(), std::move(rhs.get_error()));
    }
}
#endif

template <typename ErrorType>
inline expected<ErrorType> expected<ErrorType>::create_value() noexcept
{
    expected<ErrorType> returnValue{variant<ErrorType>()};

    return returnValue;
}

template <typename ErrorType>
template <typename... Targs>
inline expected<ErrorType> expected<ErrorType>::create_error(Targs&&... args) noexcept
{
    expected<ErrorType> returnValue(variant<ErrorType>(in_place_index<ERROR_INDEX>(), std::forward<Targs>(args)...));

    return returnValue;
}

template <typename ErrorType>
inline expected<ErrorType>::operator bool() const noexcept
{
    return !has_error();
}

template <typename ErrorType>
inline bool expected<ErrorType>::has_error() const noexcept
{
    return (m_store.index() == ERROR_INDEX);
}

template <typename ErrorType>
inline ErrorType&& expected<ErrorType>::get_error() && noexcept
{
    return std::move(*m_store.template get_at_index<ERROR_INDEX>());
}

template <typename ValueType, typename ErrorType>
inline const ErrorType& expected<ValueType, ErrorType>::get_error() const& noexcept
{
    return *m_store.template get_at_index<ERROR_INDEX>();
}

template <typename ErrorType>
inline ErrorType& expected<ErrorType>::get_error() & noexcept
{
    return *m_store.template get_at_index<ERROR_INDEX>();
}
} // namespace cxx
} // namespace iox

#endif // IOX_HOOFS_CXX_EXPECTED_INL