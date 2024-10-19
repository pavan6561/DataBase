//! \file eggs/invoke.hpp
// Eggs.Invoke
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2017-2020
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef CORE_SOURCE_UTILS_CONFIG_READER_INC_PUBLIC_ARA_CORE_CONFIGREADER_INVOKE_H_
#define CORE_SOURCE_UTILS_CONFIG_READER_INC_PUBLIC_ARA_CORE_CONFIGREADER_INVOKE_H_

#include <functional>
#include <type_traits>
#include <utility>

#define EGGS_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

#if __cplusplus > 201703L // C++20: P0306
#define EGGS_INVOKE(F, ...) \
  (static_cast<decltype(::eggs::detail::Invoke(F __VA_OPT__(, ) __VA_ARGS__))>(F)(__VA_ARGS__))
#elif _MSVC_TRADITIONAL
#define EGGS_INVOKE(F, ...) (static_cast<decltype(::eggs::detail::Invoke(F, __VA_ARGS__))>(F)(__VA_ARGS__))
#else
#define EGGS_INVOKE(F, ...) (static_cast<decltype(::eggs::detail::Invoke(F, ##__VA_ARGS__))>(F)(__VA_ARGS__))
#endif

#define EGGS_INVOKE_R(R, ...) (::eggs::detail::InvokeR<R>::Call(__VA_ARGS__))

namespace eggs
{
namespace detail
{
/// @brief Pointer to member that will be wrapped.
/// @tparam C The class is a member to a pointer.
/// @tparam T The type to check.
/// @tparam isRef It reference to converts the object to bool, returns value.
/// @tparam isRefWrapper The wrapping a reference in a copy constructible and copy
/// assignable object of type T.
/// @tparam isFunction It checks whether T is a function type.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
template <typename C, typename T, bool isRef, bool isRefWrapper, bool isFunction = std::is_function<T>::value>
struct InvokeMemPtr;

/// @brief Pointer to member that will be wrapped.
/// @tparam C The class is a member to a pointer.
/// @tparam T The type to check.
template <typename C, typename T>
struct InvokeMemPtr<C, T, true, false, false>
{
  /// @brief A pm is a pointer to member of a class C.
  T C::*pm;

#if !__cpp_aggregate_paren_init
  /// @brief Invoke the member pointer.
  constexpr InvokeMemPtr(T C::*pm) noexcept : pm(pm)
  {
  }
#endif

  /// @brief To Calls the Callable object, reference to which is stored.
  /// @tparam T1 The template type parameter.
  /// @param[in] t1 The type to first object.
  /// @returns The return value of the called function.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  template <typename T1>
  constexpr auto operator()(T1&& t1) const noexcept(noexcept(EGGS_FWD(t1).*pm)) -> decltype(EGGS_FWD(t1).*pm)
  {
    return EGGS_FWD(t1).*pm;
  }
};

/// @brief Invoke a pointer to member that will be wrapped.
/// @tparam C The class is a member to a pointer.
/// @tparam T The type to check.
template <typename C, typename T>
struct InvokeMemPtr<C, T, true, false, true>
{
  /// @brief A pm is a pointer to member of a class C.
  T C::*pm;

#if !__cpp_aggregate_paren_init
  /// @brief Invoke the member pointer.
  constexpr InvokeMemPtr(T C::*pm) noexcept : pm(pm)
  {
  }
#endif

  /// @brief Invokes the stored callable function target with the parameters object.
  /// @tparam T1 The template type parameter.
  /// @tparam Tn The type to function object.
  /// @returns The return value of the invocation of the stored callable object.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  template <typename T1, typename... Tn>
  constexpr auto operator()(T1&& t1, Tn&&... tn) const noexcept(noexcept((EGGS_FWD(t1).*pm)(EGGS_FWD(tn)...)))
      -> decltype((EGGS_FWD(t1).*pm)(EGGS_FWD(tn)...))
  {
    return (EGGS_FWD(t1).*pm)(EGGS_FWD(tn)...);
  }
};

/// @brief Pointer to member that will be wrapped.
/// @tparam C The class is a member to a pointer.
/// @tparam T The type to check.
template <typename C, typename T>
struct InvokeMemPtr<C, T, false, true, false>
{
  /// @brief pm is a pointer to member of a class C.
  T C::*pm;

#if !__cpp_aggregate_paren_init
  /// @brief Invoke the member pointer.
  constexpr InvokeMemPtr(T C::*pm) noexcept : pm(pm)
  {
  }
#endif
  /// @brief To Calls the Callable object, reference to which is stored.
  /// @tparam T1 The template type parameter.
  /// @param[in] t1 The type to first object.
  /// @returns The return value of the called function.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  template <typename T1>
  constexpr auto operator()(T1&& t1) const noexcept(noexcept(t1.get().*pm)) -> decltype(t1.get().*pm)
  {
    return t1.get().*pm;
  }
};

/// @brief Pointer to member that will be wrapped.
/// @tparam C The class is a member to a pointer.
/// @tparam T The type to check.
template <typename C, typename T>
struct InvokeMemPtr<C, T, false, true, true>
{
  /// @brief A pm is a pointer to member of a class C.
  T C::*pm;

#if !__cpp_aggregate_paren_init
  /// @brief Invoke the member pointer.
  constexpr InvokeMemPtr(T C::*pm) noexcept : pm(pm)
  {
  }
#endif
  /// @brief Invokes the stored callable function target with the parameters object.
  /// @tparam T1 The template type parameter.
  /// @tparam Tn The type to function object.
  /// @param[in] t1 The type to first object.
  /// @param[in] tn The parameters to pass to the stored callable function target.
  /// @returns Returns a pointer to the stored callable function.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  template <typename T1, typename... Tn>
  constexpr auto operator()(T1&& t1, Tn&&... tn) const noexcept(noexcept((t1.get().*pm)(EGGS_FWD(tn)...)))
      -> decltype((t1.get().*pm)(EGGS_FWD(tn)...))
  {
    return (t1.get().*pm)(EGGS_FWD(tn)...);
  }
};

/// @brief Pointer to member that will be wrapped.
/// @tparam C The class is a member to a pointer.
/// @tparam T The type to check.
template <typename C, typename T>
struct InvokeMemPtr<C, T, false, false, false>
{
  /// @brief A pm is a pointer to member of a class C.
  T C::*pm;

#if !__cpp_aggregate_paren_init
  /// @brief Invoke the member pointer.
  constexpr InvokeMemPtr(T C::*pm) noexcept : pm(pm)
  {
  }
#endif
  /// @brief To Calls the Callable object, reference to which is stored.
  /// @tparam T1 The template type parameter.
  /// @param[in] t1 The type to first object.
  /// @returns Returns a pointer to the called function.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  template <typename T1>
  constexpr auto operator()(T1&& t1) const noexcept(noexcept((*EGGS_FWD(t1)).*pm))
      -> decltype((*EGGS_FWD(t1)).*pm)
  {
    return (*EGGS_FWD(t1)).*pm;
  }
};

/// @brief Pointer to member that will be wrapped.
/// @tparam C The class is a member to a pointer.
/// @tparam T The type to check.
template <typename C, typename T>
struct InvokeMemPtr<C, T, false, false, true>
{
  /// @brief A pm is a pointer to member of a class C.
  T C::*pm;

#if !__cpp_aggregate_paren_init
  /// @brief Invoke the member pointer.
  constexpr InvokeMemPtr(T C::*pm) noexcept : pm(pm)
  {
  }
#endif

  /// @brief Invokes the stored callable function target with the parameters object.
  /// @tparam T1 The template type parameter.
  /// @tparam Tn The type to function object.
  /// @returns It returns a pointer to the stored callable function object.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  template <typename T1, typename... Tn>
  constexpr auto operator()(T1&& t1, Tn&&... tn) const
      noexcept(noexcept(((*EGGS_FWD(t1)).*pm)(EGGS_FWD(tn)...)))
          -> decltype(((*EGGS_FWD(t1)).*pm)(EGGS_FWD(tn)...))
  {
    return ((*EGGS_FWD(t1)).*pm)(EGGS_FWD(tn)...);
  }
};

/// @brief Invoke the Callable object F.
/// @tparam F Callable object to be invoked.
/// @returns It will return value of respective data type.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611168
template <typename F>
auto Invoke(F&&, ...) -> F&&;

/// @brief Pointer to member that will be wrapped.
/// @tparam T The type to check.
/// @tparam C The class is a member to a pointer.
/// @tparam T1 The template type parameter.
/// @returns It will return value of respective data type.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611169 614016
template <typename T, typename C, typename T1>
auto Invoke(T C::*, T1 const&, ...) -> InvokeMemPtr<C, T, std::is_base_of<C, T1>::value, false>;

/// @brief Pointer to member that will be wrapped.
/// @tparam T The type to check.
/// @tparam C The class is a member to a pointer.
/// @tparam X The reference variable wrapped to the pointer.
/// @returns It will return value of respective data type.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611170
template <typename T, typename C, typename X>
auto Invoke(T C::*, std::reference_wrapper<X>, ...) -> InvokeMemPtr<C, T, false, true>;

template <typename R, typename RD = typename std::remove_cv<R>::type>
/// @brief Created the InvokeR structure.
struct InvokeR
{
 private:
  /// @brief Created the Conversion method in private.
  static R Conversion(R) noexcept;

 public:
  /// @brief Invoke the Callable object f with the parameters args.
  /// @tparam F The type of callable object to function.
  /// @tparam Args The types of arguments given to this function.
  /// @param[in] f The callable object to be invoked.
  /// @param[in] args The arguments to pass to f.
  /// @returns The value returned by f.
  /// @trace REQ_AP_EC_CORE_0003
  /// @archtrace 608895
  template <typename F, typename... Args>
  static constexpr auto Call(F&& f, Args&&... args) noexcept(
      noexcept(Conversion(EGGS_INVOKE(EGGS_FWD(f), EGGS_FWD(args)...))))
      -> decltype(Conversion(EGGS_INVOKE(EGGS_FWD(f), EGGS_FWD(args)...)))
  {
    return EGGS_INVOKE(EGGS_FWD(f), EGGS_FWD(args)...);
  }
};

/// @brief Invokes any Callable object with given arguments and possibility to specify return type.
/// @tparam R Variable is the type of datatype of parameter.
/// @returns The value returned by f.
template <typename R>
struct InvokeR<R, void>
{
  /// @brief Invokes any Callable object with given arguments and possibility
  /// to specify return type.
  /// @tparam F The type of callable object to function.
  /// @tparam Args The types of arguments given to this function.
  /// @param[in] f The callable object to be invoked.
  /// @param[in] args The arguments to pass to f.
  /// @returns The value returned by f.
  template <typename F, typename... Args>
  static constexpr auto Call(F&& f, Args&&... args) noexcept(
      noexcept(EGGS_INVOKE(EGGS_FWD(f), EGGS_FWD(args)...)))
      -> decltype(static_cast<void>(EGGS_INVOKE(EGGS_FWD(f), EGGS_FWD(args)...)))
  {
    return static_cast<void>(EGGS_INVOKE(EGGS_FWD(f), EGGS_FWD(args)...));
  }
};

} // namespace detail
} // namespace eggs

namespace eggs
{
namespace detail
{
/// @brief Determines whether Fn can be invoked with the arguments ArgTypes.
/// @tparam T Variable is type of datatype of parameter.
/// @tparam Enable The enable variable to the function.
template <typename T, typename Enable = void>
struct InvokeResultImpl
{
};

/// @brief Determines whether Fn can be invoked with the arguments ArgTypes.
/// @tparam F The type of functional object.
/// @tparam Ts The types of check.
template <typename F, typename... Ts>
struct InvokeResultImpl<F(Ts...), decltype((void)EGGS_INVOKE(std::declval<F>(), std::declval<Ts>()...))>
{
  /// @brief decltype alias of type.
  using type = decltype(EGGS_INVOKE(std::declval<F>(), std::declval<Ts>()...));
};
} // namespace detail

/// @brief Invoking F with the ArgTypes.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename Fn, typename... ArgTypes>
struct InvokeResult : detail::InvokeResultImpl<Fn && (ArgTypes && ...)>
{
};

/// @brief Invoking F with the ArgTypes.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename Fn, typename... ArgTypes>
using InvokeResultT = typename InvokeResult<Fn, ArgTypes...>::type;

namespace detail
{
/// @brief Definition of struct IsInvocableImpl.
/// @tparam T Variable is type of datatype of parameter.
/// @tparam Enable The enable variable to the function.
template <typename T, typename Enable = void>
struct IsInvocableImpl : std::false_type
{
};

/// @brief Definition of struct IsInvocableImpl that inherit std::true_type.
/// @tparam F The type of functional object.
/// @tparam Ts The types of check.
template <typename F, typename... Ts>
struct IsInvocableImpl<F(Ts...), decltype((void)EGGS_INVOKE(std::declval<F>(), std::declval<Ts>()...))>
    : std::true_type
{
};
} // namespace detail

/// @brief Definition of struct IsInvocable.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename Fn, typename... ArgTypes>
struct IsInvocable : detail::IsInvocableImpl<Fn && (ArgTypes && ...)>::type
{
};

#if __cpp_variable_templates
/// @brief Determines whether Fn can be invoked with the arguments ArgTypes.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename Fn, typename... ArgTypes>
#if __cpp_inline_variables
inline
#endif
    constexpr bool is_invocable_v{IsInvocable<Fn, ArgTypes...>::value};
#endif
namespace detail
{
/// @brief Determines whether it can be invoked.
/// @tparam R Variable is the type of datatype of parameter.
/// @tparam T The types to check.
/// @tparam Enable The enable variable to the function.
template <typename T, typename R, typename Enable = void>
struct IsInvocableRImpl : std::false_type
{
};

/// @brief The result is implicitly converted to R if R is not possibly
/// cv-qualified void, or discarded otherwise.
/// @tparam F The type of functional object.
/// @tparam Ts The types of check.
/// @tparam R Variable is the type of datatype of parameter.
template <typename F, typename... Ts, typename R>
struct IsInvocableRImpl<F(Ts...), R,
    decltype((void)EGGS_INVOKE_R(R, std::declval<F>(), std::declval<Ts>()...))> : std::true_type
{
};
} // namespace detail

/// @brief Determines whether Fn can be invoked with the arguments ArgTypes.
/// @tparam R Variable is the type of datatype of parameter.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename R, typename Fn, typename... ArgTypes>
struct IsInvocableR : detail::IsInvocableRImpl<Fn && (ArgTypes && ...), R>::type
{
};

#if __cpp_variable_templates
/// @brief Determines whether Fn can be invoked with the arguments ArgTypes.
/// @tparam R Variable is the type of datatype of parameter.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename R, typename Fn, typename... ArgTypes>
#if __cpp_inline_variables
inline
#endif
    constexpr bool is_invocable_r_v{IsInvocableR<R, Fn, ArgTypes...>::value};
#endif

namespace detail
{
/// @brief Determines whether it can be invoked.
/// @tparam T The types to check.
/// @tparam Enable The enable variable to the function.
template <typename T, typename Enable = void>
struct IsNothrowInvocableImpl : std::false_type
{
};

/// @brief Determines whether Fn can be invoked with the arguments ArgTypes.
/// @tparam F The type of functional object.
/// @tparam Ts The types of check.
template <typename F, typename... Ts>
struct IsNothrowInvocableImpl<F(Ts...), decltype((void)EGGS_INVOKE(std::declval<F>(), std::declval<Ts>()...))>
    : std::integral_constant<bool, noexcept(EGGS_INVOKE(std::declval<F>(), std::declval<Ts>()...))>
{
};
} // namespace detail

/// @brief Determines whether Fn is callable with the arguments ArgTypes.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename Fn, typename... ArgTypes>
struct IsNothrowInvocable : detail::IsNothrowInvocableImpl<Fn && (ArgTypes && ...)>::type
{
};

#if __cpp_variable_templates
/// @brief Determines whether Fn is callable with the arguments ArgTypes.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename Fn, typename... ArgTypes>
#if __cpp_inline_variables
inline
#endif
    constexpr bool is_nothrow_invocable_v{IsNothrowInvocable<Fn, ArgTypes...>::value};
#endif

namespace detail
{
template <typename T, typename R, typename Enable = void>
/// @brief It created IsNothrowInvocableRImpl structure.
struct IsNothrowInvocableRImpl : std::false_type
{
};

/// @brief Determines whether Fn can be invoked with the arguments ArgTypes.
/// @tparam F The type of functional object.
/// @tparam Ts The types of check.
/// @tparam R Variable is the type of datatype of parameter.
template <typename F, typename... Ts, typename R>
struct IsNothrowInvocableRImpl<F(Ts...), R,
    decltype((void)EGGS_INVOKE_R(R, std::declval<F>(), std::declval<Ts>()...))>
    : std::integral_constant<bool, noexcept(EGGS_INVOKE_R(R, std::declval<F>(), std::declval<Ts>()...))>
{
};
} // namespace detail

/// @brief Determines whether Fn can be invoked with the arguments ArgTypes.
/// @tparam R Variable is the type of datatype of parameter.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename R, typename Fn, typename... ArgTypes>
struct IsNothrowInvocableR : detail::IsNothrowInvocableRImpl<Fn && (ArgTypes && ...), R>::type
{
};

#if __cpp_variable_templates
/// @brief Determines whether Fn can be invoked with the arguments ArgTypes.
/// @tparam R Variable is the type of datatype of parameter.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
template <typename R, typename Fn, typename... ArgTypes>
#if __cpp_inline_variables
inline
#endif
    constexpr bool is_nothrow_invocable_r_v{IsNothrowInvocableR<R, Fn, ArgTypes...>::value};
#endif

/// @brief Invoke the Callable object f with the parameters args.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
/// @param[in] f Callable object to be invoked.
/// @param[in] args The arguments to pass to f.
/// @returns The value returned by f.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611166
template <typename Fn, typename... ArgTypes>
constexpr auto Invoke(Fn&& f, ArgTypes&&... args) noexcept(noexcept(
    EGGS_INVOKE(EGGS_FWD(f), EGGS_FWD(args)...))) -> decltype(EGGS_INVOKE(EGGS_FWD(f), EGGS_FWD(args)...))
{
  return EGGS_INVOKE(EGGS_FWD(f), EGGS_FWD(args)...);
}

/// @brief The result is implicitly converted to R if R is not possibly.
/// cv-qualified void, or discarded otherwise.
/// @tparam R Variable is the type of datatype of parameter.
/// @tparam Fn The callable function type.
/// @tparam ArgTypes The types of arguments given to this function.
/// @param[in] f Callable object to be invoked.
/// @param[in] args The arguments to pass to f.
/// @returns The value returned by f, implicitly converted to R, if R is not void. None otherwise.
/// @trace REQ_AP_EC_CORE_0003
/// @archtrace 608895
/// @design 611167
template <typename R, typename Fn, typename... ArgTypes>
constexpr auto InvokeR(Fn&& f, ArgTypes&&... args) noexcept(
    noexcept(EGGS_INVOKE_R(R, EGGS_FWD(f), EGGS_FWD(args)...)))
    -> decltype(EGGS_INVOKE_R(R, EGGS_FWD(f), EGGS_FWD(args)...))
{
  return EGGS_INVOKE_R(R, EGGS_FWD(f), EGGS_FWD(args)...);
}

} // namespace eggs
#undef EGGS_FWD
#endif // CORE_SOURCE_UTILS_CONFIG_READER_INC_PUBLIC_ARA_CORE_CONFIGREADER_INVOKE_H_
