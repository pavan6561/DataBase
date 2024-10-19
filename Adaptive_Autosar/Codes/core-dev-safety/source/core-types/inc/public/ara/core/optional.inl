// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
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

#ifndef IOX_HOOFS_CXX_OPTIONAL_CORE_INL
#define IOX_HOOFS_CXX_OPTIONAL_CORE_INL

namespace ara
{
namespace core
{
template <typename T>
/// @brief Creates an Optional.
inline Optional<T>::Optional(const NullOptT&) noexcept
{
}

template <typename T>
/// @brief Creates an Optional.
inline Optional<T>::Optional() noexcept : Optional(NullOptT())
{
}

template <typename T>
/// @brief Creates an Optional.
inline Optional<T>::Optional(T&& value) noexcept
{
  construct_value(std::forward<T>(value));
}

template <typename T>
/// @brief Creates an Optional.
inline Optional<T>::Optional(const T& value) noexcept
{
  construct_value(value);
}

template <typename T>
/// @brief Creates an Optional.
inline Optional<T>::Optional(const Optional& rhs) noexcept
{
  if (rhs.mHasValue_)
  {
    construct_value(rhs.value());
  }
}

template <typename T>
/// @brief Creates an Optional.
inline Optional<T>::Optional(Optional&& rhs) noexcept
{
  if (rhs.mHasValue_)
  {
    construct_value(std::move(rhs.value()));
    rhs.destruct_value();
  }
}

template <typename T>
template <typename... Targs>
/// @brief Creates an Optional.
inline Optional<T>::Optional(InPlaceT, Targs&&... args) noexcept
{
  construct_value(std::forward<Targs>(args)...);
}

template <typename T>
/// @brief Creates an operator=.
inline Optional<T>& Optional<T>::operator=(const Optional& rhs) noexcept
{
  if (this != &rhs)
  {
    if ((!rhs.mHasValue_) && (mHasValue_))
    {
      destruct_value();
    }
    else if ((rhs.mHasValue_) && (mHasValue_))
    {
      value() = rhs.value();
    }
    else if ((rhs.mHasValue_) && (!mHasValue_))
    {
      construct_value(rhs.value());
    }
  }
  return *this;
}

template <typename T>
/// @brief Creates an operator=.
inline Optional<T>& Optional<T>::operator=(const T& v) noexcept
{
  if (mHasValue_)
  {
    value() = v;
  }
  else if (!mHasValue_)
  {
    construct_value(v);
  }
  return *this;
}

template <typename T>
/// @brief Creates an operator=.
inline Optional<T>& Optional<T>::operator=(Optional&& rhs) noexcept
{
  if (this != &rhs)
  {
    if ((!rhs.mHasValue_) && (mHasValue_))
    {
      destruct_value();
    }
    else if ((rhs.mHasValue_) && (mHasValue_))
    {
      value() = std::move(rhs.value());
    }
    else if ((rhs.mHasValue_) && (!mHasValue_))
    {
      construct_value(std::move(rhs.value()));
    }
    if (rhs.mHasValue_)
    {
      rhs.destruct_value();
    }
  }
  return *this;
}

template <typename T>
/// @brief Creates an operator=.
inline Optional<T>& Optional<T>::operator=(T&& v) noexcept
{
  if (mHasValue_)
  {
    value() = std::move(v);
  }
  else if (!mHasValue_)
  {
    construct_value(std::move(v));
  }
  return *this;
}
template <typename T>
/// @brief Creates an ~Optional().
inline Optional<T>::~Optional() noexcept
{
  if (mHasValue_)
  {
    destruct_value();
  }
}

template <typename T>
template <typename U>
/// @brief Creates an move assignment operator.
inline typename std::enable_if<!std::is_same<U, Optional<T>&>::value, Optional<T>>::type& Optional<T>::
    operator=(U&& newValue) noexcept
{
  if (mHasValue_)
  {
/// @todo broken msvc compiler, see:
///  https://developercommunity.visualstudio.com/content/problem/858688/
///  stdforward-none-of-these-2-overloads-could-convert.html
/// remove this as soon as it is fixed;
#ifdef _WIN32
    value() = newValue;
#else
    value() = std::forward<T>(newValue);
#endif
  }
  else
  {
/// @todo again broken msvc compiler
#ifdef _WIN32
    construct_value(newValue);
#else
    construct_value(std::forward<T>(newValue));
#endif
  }
  return *this;
}

template <typename T>
/// @brief Creates an operator==.
constexpr inline bool Optional<T>::operator==(const Optional<T>& rhs) const noexcept
{
  return (!mHasValue_ && !rhs.mHasValue_) || ((mHasValue_ && rhs.mHasValue_) && (value() == rhs.value()));
}

template <typename T>
/// @brief Creates an operator==.
constexpr inline bool Optional<T>::operator==(const NullOptT&) const noexcept
{
  return !mHasValue_;
}

template <typename T>
/// @brief Creates an operator!=.
constexpr inline bool Optional<T>::operator!=(const Optional<T>& rhs) const noexcept
{
  return !(*this == rhs);
}

template <typename T>
/// @brief Creates an operator!=.
constexpr inline bool Optional<T>::operator!=(const NullOptT& rhs) const noexcept
{
  return !(*this == rhs);
}

template <typename T>
/// @brief Creates an operator->.
inline const T* Optional<T>::operator->() const noexcept
{
  return &value();
}

template <typename T>
/// @brief Creates an operator*.
inline const T& Optional<T>::operator*() const noexcept
{
  return value();
}

template <typename T>
/// @brief Creates an operator->.
inline T* Optional<T>::operator->() noexcept
{
  return &value();
}

template <typename T>
/// @brief Creates an operator*.
inline T& Optional<T>::operator*() noexcept
{
  return value();
}

template <typename T>
/// @brief Creates an operator bool.
inline constexpr Optional<T>::operator bool() const noexcept
{
  return mHasValue_;
}

template <typename T>
template <typename... Targs>
/// @brief Creates an emplace.
inline T& Optional<T>::emplace(Targs&&... args) noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  if (mHasValue_)
  {
    destruct_value();
  }

  construct_value(std::forward<Targs>(args)...);
  return value();
}

template <typename T>
/// @brief Creates an has_value.
inline constexpr bool Optional<T>::has_value() const noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  return mHasValue_;
}

template <typename T>
/// @brief Creates an reset.
inline void Optional<T>::reset() noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  if (mHasValue_)
  {
    destruct_value();
  }
}

template <typename T>
    /// @brief Creates an value.
    inline T& Optional<T>::value() & noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  iox::cxx::Expects(has_value());
  return *static_cast<T*>(static_cast<void*>(&mData_));
}

template <typename T>
/// @brief Creates an value.
inline const T& Optional<T>::value() const& noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  // PRQA S 3066 1 # const cast to avoid code duplication
  return const_cast<Optional<T>*>(this)->value();
}

template <typename T>
    /// @brief Creates an value.
    inline T&& Optional<T>::value() && noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  iox::cxx::Expects(has_value());
  return std::move(*static_cast<T*>(static_cast<void*>(&mData_)));
}

template <typename T>
/// @brief Creates an value.
inline const T&& Optional<T>::value() const&& noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  return std::move(*const_cast<Optional<T>*>(this)->value());
}

template <typename T>
template <typename... Targs>
/// @brief Creates an construct_value.
inline void Optional<T>::construct_value(Targs&&... args) noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  new (&mData_) T(std::forward<Targs>(args)...);
  mHasValue_ = true;
}

template <typename T>
/// @brief Creates an destruct_value.
inline void Optional<T>::destruct_value() noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  value().~T();
  mHasValue_ = false;
}

template <typename OptionalBaseType, typename... Targs>
/// @brief Creates an make_optional.
inline Optional<OptionalBaseType> make_optional(Targs&&... args) noexcept
{
  Optional<OptionalBaseType> returnValue = NullOptT();
  returnValue.emplace(std::forward<Targs>(args)...);
  return returnValue;
}

template <typename T>
/// @brief Creates an operator=.
inline void Optional<T>::operator=(NullOptT) noexcept
{
  reset();
}

template <class T>
/// @brief Creates an operator<.
constexpr bool operator<(const Optional<T>& x, const Optional<T>& y)
{
  return !bool(y) ? false : (!bool(x) ? true : std::less<T>{}(*x, *y));
}

template <class T>
/// @brief Creates an operator>.
constexpr bool operator>(const Optional<T>& x, const Optional<T>& y)
{
  return (!(x < y)) && (!(x == y));
}

template <class T>
/// @brief Creates an operator<=.
constexpr bool operator<=(const Optional<T>& x, const Optional<T>& y)
{
  return (x < y) || (x == y);
}

template <class T>
/// @brief Creates an operator>=.
constexpr bool operator>=(const Optional<T>& x, const Optional<T>& y)
{
  return !(x < y);
}

template <class T>
/// @brief Creates an operator==.
constexpr bool operator==(NullOptT, const Optional<T>& x) noexcept
{
  return !bool(x);
}

template <class T>
/// @brief Creates an operator!=.
constexpr bool operator!=(NullOptT, const Optional<T>& x) noexcept
{
  return !(nullopt == x);
}

template <class T>
/// @brief Creates an operator<.
constexpr bool operator<(const Optional<T>& x, NullOptT) noexcept
{
  return false;
}

template <class T>
/// @brief Creates an operator<.
constexpr bool operator<(NullOptT, const Optional<T>& x) noexcept
{
  return bool(x);
}

template <class T>
/// @brief Creates an operator<=.
constexpr bool operator<=(const Optional<T>& x, NullOptT) noexcept
{
  return (x < nullopt) || (x == nullopt);
}

template <class T>
/// @brief Creates an operator<=.
constexpr bool operator<=(NullOptT, const Optional<T>& x) noexcept
{
  return (nullopt < x) || (nullopt == x);
}

template <class T>
/// @brief Creates an operator>.
constexpr bool operator>(const Optional<T>& x, NullOptT) noexcept
{
  return (!(x < nullopt)) && (!(x == nullopt));
}

template <class T>
/// @brief Creates an operator>.
constexpr bool operator>(NullOptT, const Optional<T>& x) noexcept
{
  return (!(nullopt < x)) && (!(nullopt == x));
}

template <class T>
/// @brief Creates an operator>=.
constexpr bool operator>=(const Optional<T>& x, NullOptT) noexcept
{
  return !(x < nullopt);
}

template <class T>
/// @brief Creates an operator>=.
constexpr bool operator>=(NullOptT, const Optional<T>& x) noexcept
{
  return !(nullopt < x);
}

template <class T>
/// @brief Creates an operator==.
constexpr bool operator==(const Optional<T>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T>
/// @brief Creates an operator==.
constexpr bool operator==(const T& v, const Optional<T>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T>
/// @brief Creates an operator!=.
constexpr bool operator!=(const Optional<T>& x, const T& v)
{
  return !(x == v);
}

template <class T>
/// @brief Creates an operator!=.
constexpr bool operator!=(const T& v, const Optional<T>& x)
{
  return !(v == x);
}

template <class T>
/// @brief Creates an operator<.
constexpr bool operator<(const Optional<T>& x, const T& v)
{
  return bool(x) ? std::less<T>{}(*x, v) : true;
}

template <class T>
/// @brief Creates an operator<.
constexpr bool operator<(const T& v, const Optional<T>& x)
{
  return bool(x) ? std::less<T>{}(v, *x) : false;
}

template <class T>
/// @brief Creates an operator<=.
constexpr bool operator<=(const Optional<T>& x, const T& v)
{
  return (x < v) || (x == v);
}

template <class T>
/// @brief Creates an operator<=.
constexpr bool operator<=(const T& v, const Optional<T>& x)
{
  return (v < x) || (v == x);
}

template <class T>
/// @brief Creates an operator>.
constexpr bool operator>(const Optional<T>& x, const T& v)
{
  return (!(x < v)) && (!(x == v));
}

template <class T>
/// @brief Creates an operator>.
constexpr bool operator>(const T& v, const Optional<T>& x)
{
  return (!(v < x)) && (!(v == x));
}

template <class T>
/// @brief Creates an operator>=.
constexpr bool operator>=(const Optional<T>& x, const T& v)
{
  return !(x < v);
}

template <class T>
/// @brief Creates an operator>=.
constexpr bool operator>=(const T& v, const Optional<T>& x)
{
  return !(v < x);
}

} // namespace core
} // namespace ara

#endif // IOX_HOOFS_CXX_OPTIONAL_CORE_INL
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
// -----------------------------------------------------------------------------------------------------------
