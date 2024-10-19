// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 by Apex.AI Inc. All rights reserved.
// Copyright (c) 2021 by Perforce All rights reserved.
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
#ifndef VARIANT_INL
#define VARIANT_INL

namespace ara
{
namespace core
{
template <typename... Types>
/// @brief Variant constructor is implementation.
inline constexpr Variant<Types...>::Variant(const Variant& rhs) noexcept : mTypeIndex_(rhs.mTypeIndex_)
{
  if (mTypeIndex_ != INVALID_VARIANT_INDEX)
  {
    iox::cxx::internal::call_at_index<0, Types...>::copyConstructor(
        mTypeIndex_, const_cast<iox::cxx::internal::byte_t*>(rhs.mStorage_), mStorage_);
  }
}

  // AVIN_VERIFIED_NEXT_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.
template <typename... Types>
template <uint64_t N, typename... CTorArguments>
/// @brief Variant constructor is implementation.
inline constexpr Variant<Types...>::Variant(const InPlaceIndex<N>&, CTorArguments&&... args) noexcept
{
  emplace_at_index<N>(std::forward<CTorArguments>(args)...);
}

template <typename... Types>
template <typename T, typename... CTorArguments>
/// @brief Variant constructor is implementation.
inline constexpr Variant<Types...>::Variant(const InPlaceType<T>&, CTorArguments&&... args) noexcept
{
  emplace<T>(std::forward<CTorArguments>(args)...);
}

template <typename... Types>
template <typename T, typename,
    typename std::enable_if_t<!iox::cxx::internal::is_in_place_index<std::decay_t<T>>::value, bool>,
    typename std::enable_if_t<!iox::cxx::internal::is_in_place_type<std::decay_t<T>>::value, bool>>
/// @brief Variant constructor is implementation.
inline constexpr Variant<Types...>::Variant(T&& arg) noexcept :
    Variant(InPlaceType<std::decay_t<T>>(), std::forward<T>(arg))
{
}

template <typename... Types>
/// @brief operator= operator is implementation.
inline constexpr Variant<Types...>& Variant<Types...>::operator=(const Variant& rhs) noexcept
{
  if (this != &rhs)
  {
    if (mTypeIndex_ != rhs.mTypeIndex_)
    {
      call_element_destructor();
      mTypeIndex_ = rhs.mTypeIndex_;

      if (mTypeIndex_ != INVALID_VARIANT_INDEX)
      {
        iox::cxx::internal::call_at_index<0, Types...>::copyConstructor(
            mTypeIndex_, const_cast<iox::cxx::internal::byte_t*>(rhs.mStorage_), mStorage_);
      }
    }
    else
    {
      if (mTypeIndex_ != INVALID_VARIANT_INDEX)
      {
        iox::cxx::internal::call_at_index<0, Types...>::copy(
            mTypeIndex_, const_cast<iox::cxx::internal::byte_t*>(rhs.mStorage_), mStorage_);
      }
    }
  }
  return *this;
}

template <typename... Types>
/// @brief Variant constructor is implementation.
inline constexpr Variant<Types...>::Variant(Variant&& rhs) noexcept : mTypeIndex_{std::move(rhs.mTypeIndex_)}
{
  if (mTypeIndex_ != INVALID_VARIANT_INDEX)
  {
    iox::cxx::internal::call_at_index<0, Types...>::moveConstructor(mTypeIndex_, rhs.mStorage_, mStorage_);
  }
}

template <typename... Types>
/// @brief operator= operator is implementation.
inline constexpr Variant<Types...>& Variant<Types...>::operator=(Variant&& rhs) noexcept
{
  if (this != &rhs)
  {
    if (mTypeIndex_ != rhs.mTypeIndex_)
    {
      call_element_destructor();
      mTypeIndex_ = std::move(rhs.mTypeIndex_);
      if (mTypeIndex_ != INVALID_VARIANT_INDEX)
      {
        iox::cxx::internal::call_at_index<0, Types...>::moveConstructor(
            mTypeIndex_, rhs.mStorage_, mStorage_);
      }
    }
    else
    {
      if (mTypeIndex_ != INVALID_VARIANT_INDEX)
      {
        iox::cxx::internal::call_at_index<0, Types...>::move(mTypeIndex_, rhs.mStorage_, mStorage_);
      }
    }
  }
  return *this;
}

template <typename... Types>
/// @brief Variant destructor is implementation.
inline Variant<Types...>::~Variant() noexcept
{
  call_element_destructor();
}

template <typename... Types>
/// @brief call_element_destructor method is implementation.
inline void Variant<Types...>::call_element_destructor() noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  if (mTypeIndex_ != INVALID_VARIANT_INDEX)
  {
    iox::cxx::internal::call_at_index<0, Types...>::destructor(mTypeIndex_, mStorage_);
  }
}

template <typename... Types>
    template <typename T>
    inline typename std::enable_if<!std::is_same<T, Variant<Types...>&>::value, Variant<Types...>>::type&
    /// @brief operator= operator is implementation.
    Variant<Types...>::operator=(T&& rhs)
    & noexcept
{
  if (mTypeIndex_ == INVALID_VARIANT_INDEX)
  {
    mTypeIndex_ = iox::cxx::internal::get_index_of_type<0, T, Types...>::index;
  }

  if (!has_bad_variant_element_access<T>())
  {
    auto storage{static_cast<T*>(static_cast<void*>(mStorage_))};
    *storage = (std::forward<T>(rhs));
  }
  else
  {
    ErrorMessage(__PRETTY_FUNCTION__,
        "wrong Variant type assignment, another type is already "
        "set in Variant");
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A5.1.1 Use of magic string literal.

  return *this;
}

template <typename... Types>
template <uint64_t TypeIndex, typename... CTorArguments>
/// @brief created the emplace_at_index method.
inline bool Variant<Types...>::emplace_at_index(CTorArguments&&... args) noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  static_assert(TypeIndex <= sizeof...(Types), "TypeIndex is out of bounds");
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.

  /// @brief T is alias of typename.
  using T = typename iox::cxx::internal::get_type_at_index<0, TypeIndex, Types...>::type;

  call_element_destructor();
  new (mStorage_) T(std::forward<CTorArguments>(args)...);
  mTypeIndex_ = TypeIndex;

  return true;
}
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

template <typename... Types>
template <typename T, typename... CTorArguments>
/// @brief created the emplace_at_index method.
inline bool Variant<Types...>::emplace(CTorArguments&&... args) noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  if ((mTypeIndex_ != INVALID_VARIANT_INDEX) && (has_bad_variant_element_access<T>()))
  {
    ErrorMessage(__PRETTY_FUNCTION__,
        "wrong Variant type emplacement, another type is already "
        "set in Variant");
    return false;
  }
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT AutosarC++19_03-A5.1.1 Use of magic string literal.

  if (mTypeIndex_ != INVALID_VARIANT_INDEX)
  {
    call_element_destructor();
  }

  new (mStorage_) T(std::forward<CTorArguments>(args)...);
  mTypeIndex_ = iox::cxx::internal::get_index_of_type<0, T, Types...>::index;

  return true;
}

template <typename... Types>
template <uint64_t TypeIndex>
/// @brief created the get_at_index method.
inline typename iox::cxx::internal::get_type_at_index<0, TypeIndex, Types...>::type*
    Variant<Types...>::get_at_index() noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  if (TypeIndex != mTypeIndex_)
  {
    return nullptr;
  }
  /// @brief T is alias of typename.
  using T = typename iox::cxx::internal::get_type_at_index<0, TypeIndex, Types...>::type;

  return static_cast<T*>(static_cast<void*>(mStorage_));
}
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

template <typename... Types>
template <uint64_t TypeIndex>
inline const typename iox::cxx::internal::get_type_at_index<0, TypeIndex, Types...>::type*
    /// @brief created the get_at_index method.
    Variant<Types...>::get_at_index() const noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  /// @brief T is alias of typename.
  using T = typename iox::cxx::internal::get_type_at_index<0, TypeIndex, Types...>::type;
  return const_cast<const T*>(const_cast<Variant*>(this)->template get_at_index<TypeIndex>());
}
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

template <typename... Types>
template <typename T>
/// @brief created the get method.
inline const T* Variant<Types...>::get() const noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  if (has_bad_variant_element_access<T>())
  {
    return nullptr;
  }
  else
  {
    return static_cast<const T*>(static_cast<const void*>(mStorage_));
    // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
  }
}

template <typename... Types>
template <typename T>
/// @brief created the get method.
inline T* Variant<Types...>::get() noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  return const_cast<T*>(const_cast<const Variant*>(this)->get<T>());
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
}

template <typename... Types>
template <typename T>
/// @brief created the get_if method.
inline T* Variant<Types...>::get_if(T* default_value) noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  return const_cast<T*>(const_cast<const Variant*>(this)->get_if<T>(const_cast<const T*>(default_value)));
}

template <typename... Types>
template <typename T>
/// @brief created the get_if method.
inline const T* Variant<Types...>::get_if(const T* default_value) const noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  if (has_bad_variant_element_access<T>())
  {
    return default_value;
  }

  return this->get<T>();
}

template <typename... Types>
/// @brief created the index method.
constexpr uint64_t Variant<Types...>::index() const noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  return mTypeIndex_;
}

template <typename... Types>
template <typename T>
/// @brief created the has_bad_variant_element_access method.
inline bool Variant<Types...>::has_bad_variant_element_access() const noexcept
// AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Method Methods shall use pascal case.
{
  static_assert(
      iox::cxx::internal::does_contain_type<T, Types...>::value, "Variant does not contain given type");
      // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
  return (mTypeIndex_ != iox::cxx::internal::get_index_of_type<0, T, Types...>::index);
}

template <typename... Types>
/// @brief created the ErrorMessage method.
inline void Variant<Types...>::ErrorMessage(const char* source, const char* msg) noexcept
// AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A27.0.4 C-style strings should not be used.
{
  std::cerr << source << " ::: " << msg << std::endl;
  // AVIN_VERIFIED_PREVIOUS AutosarC++19_03-A5.1.1 Use of magic string literal.
}

template <typename T, typename... Types>
/// @brief created the holds_alternative method.
inline constexpr bool holds_alternative(const Variant<Types...>& Variant) noexcept
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  return Variant.template get<T>() != nullptr;
}

} // namespace core
} // namespace ara

#endif // VARIANT_INL

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
// -----------------------------------------------------------------------------------------------------------
