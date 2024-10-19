// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : result.h
//
// Purpose     : This file provides the implementation of class result.
// -----------------------------------------------------------------------------------------------------------

#ifndef CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_RESULT_H_
#define CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_RESULT_H_

#include "ara/core/error_code.h"
#include "ara/core/optional.h"

#include <boost/variant.hpp>
#include "iceoryx_hoofs/cxx/expected.hpp"
#include <system_error>
#include <type_traits>
#include <memory>
#include <utility>
#include <iostream>

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief Adaptive Core.
namespace core
{
/// @brief This class is a type that contains either a value or an error.
/// @tparam T The Data type of argument.
/// @tparam E The Error Code type of error.
/// @trace SWS_CORE_S7.2.1.3.3_P2 SWS_CORE_00002
/// @archtrace 609893
/// @design 611359 614039
template <typename T, typename E = ErrorCode>
class Result final
{
  /// @brief Instance of class iox::cxx::expected.
  iox::cxx::expected<T, E> mData_;

  /// @brief Implimentation of HasAsFirstChecker.
  /// @tparam U The type of Value.
  /// @tparam Args The types of arguments given to this function.
  template <typename U, typename... Args>
  struct HasAsFirstChecker;

  /// @brief Implimentation of HasAsFirstChecker.
  /// @tparam U The type of default Value.
  template <typename U>
  struct HasAsFirstChecker<U> : std::false_type
  {
  };

  /// @brief Implimentation of HasAsFirstChecker.
  /// @tparam U The type of Value.
  /// @tparam Args The types of arguments given to this function.
  template <typename U, typename... Args>
  struct HasAsFirstChecker<U, U, Args...> : std::true_type
  {
  };

  /// @brief Implimentation of HasAsFirstChecker
  /// @tparam U The type of default Value.
  /// @tparam A The types of arguments.
  /// @tparam Args The types of arguments given to this function.
  template <typename U, typename A, typename... Args>
  struct HasAsFirstChecker<U, A, Args...> : std::false_type
  {
  };

  /// @brief Implimentation of HasAsFirst.
  /// @tparam U The type of defaultValue.
  /// @tparam Args The types of arguments given to this function.
  template <typename U, typename... Args>
  struct HasAsFirst
      : public HasAsFirstChecker<typename std::remove_reference<U>::type,
            typename std::remove_reference<Args>::type...>
  {
  };

 public:
  /// @brief Type alias for the type T of values .
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  using ValueType = T;

  /// @brief Type alias for the type E of errors .
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  using ErrorType = E;

  /// @brief Build a new Result from the specified value (given as lvalue).
  /// @param[in] t The value to put into the Result.
  /// @returns A Result that contains the value t.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611360
  static Result FromValue(T const& t)
  {
    return Result(t);
  }

  /// @brief Build a new Result from the specified value (given as rvalue).
  /// @param[in] t The value to put into the Result.
  /// @returns A Result that contains the value t.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611361
  static Result FromValue(T&& t)
  {
    return Result(std::move(t));
  }

  /// @brief Build a new Result from a value that is constructed
  /// in-place from the given arguments.
  /// @tparam Args The types of arguments given to this function.
  /// @tparam X It can be used as an additional function argument, as a return type,
  /// as a class template or function template parameter.
  /// @param[in] args The arguments used for constructing the value.
  /// @returns A Result that contains a value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  template <typename... Args,
      typename X = typename std::enable_if<true //
          && (std::is_constructible<T, Args&&...>::value) //
          && (!HasAsFirst<T, Args...>::value) //
          && (!HasAsFirst<Result, Args...>::value) //
          >::type //
      >
  /// @brief Build a new Result from a value that is constructed in-place from the given arguments.
  /// @returns A Result that contains a value.
  /// @design 611362
  static Result FromValue(Args&&... args) noexcept
  {
    return Result(T{std::forward<Args>(args)...});
  }

  /// @brief Build a new Result from the specified error (given as lvalue).
  /// @param[in] e The error to put into the Result.
  /// @returns A Result that contains the error e.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611363
  static Result FromError(E const& e) noexcept
  {
    return Result(e);
  }

  /// @brief Build a new Result from the specified error (given as rvalue).
  /// @param[in] e The error to put into the Result.
  /// @returns A Result that contains the error e.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611364
  static Result FromError(E&& e) noexcept
  {
    return Result(std::move(e));
  }

  /// @brief Build a new Result from an error that is constructed
  /// in-place from the given arguments.
  /// @tparam Args The types of arguments given to this function.
  /// @tparam X It can be used as an additional function argument, as a return type,
  /// as a class template or function template parameter.
  /// @param[in] args The arguments used for constructing the value.
  /// @returns A Result that contains an error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  template <typename... Args,
      typename X = typename std::enable_if<true //
          && (std::is_constructible<E, Args&&...>::value) //
          && (!HasAsFirst<E, Args...>::value) //
          && (!HasAsFirst<Result, Args...>::value) //
          >::type //
      >
  /// @brief To get the error message.
  /// @param[in] args The types of arguments given to this function.
  /// @returns A Result that contains an error.
  /// @design 611365
  static Result FromError(Args&&... args) noexcept
  {
    return Result(E{std::forward<Args>(args)...});
  }

  /// @brief Construct a new Result from the specified value (given as lvalue).
  /// @param[in] t The value to put into the Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611366
  Result(T const& t) : mData_(iox::cxx::success<T>(t))
  {
  }

  /// @brief Construct a new Result from the specified value (given as rvalue).
  /// @param[in] t The value to put into the Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611367
  Result(T&& t) noexcept : mData_(iox::cxx::success<T>(std::move(t)))
  {
  }

  /// @brief Construct a new Result from the specified error (given as lvalue).
  /// @param[in] e The error to put into the Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611368
  explicit Result(E const& e) noexcept : mData_(iox::cxx::error<E>(e))
  {
  }

  /// @brief Construct a new Result from the specified error (given as lvalue).
  /// @param[in] e The error to put into the Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611369
  explicit Result(E&& e) : mData_(iox::cxx::error<E>(std::move(e)))
  {
  }

  /// @brief Copy-construct a new Result from another instance.
  /// @param[in] other The other source instance.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  Result(Result const& other) = default;

  /// @brief Move-construct a new Result from another instance.
  /// @param[in] other The other source instance.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  Result(Result&& other) noexcept(
      std::is_nothrow_move_constructible<T>::value&& std::is_nothrow_move_constructible<E>::value)
      = default;

  /// @brief Destructor of class Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  ~Result() noexcept = default;

  /// @brief Copy-assign another Result to this instance.
  /// @param[in] other The other source instance.
  /// @returns Reference to this, containing the contents of other.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  Result& operator=(Result const& other) & = default;

  /// @brief Move-assign another Result to this instance.
  /// @param[in] other The other source instance.
  /// @returns Reference to this, containing the contents of other.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  Result& operator=(Result&& other)
      & noexcept(std::is_nothrow_move_constructible<T>::value&& std::is_nothrow_move_assignable<T>::value&&
                std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
      = default;

  // ----------------------------------------

  /// @brief Put a new value into this instance, constructed in-place from the given arguments.
  /// @tparam Args The types of arguments given to this function.
  /// @param[in] args The arguments used for constructing the value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611370
  template <typename... Args>
  void EmplaceValue(Args&&... args) noexcept
  {
    // Unfortunately, Boost.Variant does not support emplace(), so we fall back to assignment here.
    mData_ = std::move(T(std::forward<Args>(args)...));
  }

  /// @brief Put a new error into this instance, constructed in-place from the given arguments.
  /// @tparam Args The types of arguments given to this function.
  /// @param[in] args The arguments used for constructing the error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611371
  template <typename... Args>
  void EmplaceError(Args&&... args) noexcept
  {
    // Unfortunately, Boost.Variant does not support emplace(), so we fall back to assignment here.
    mData_ = std::move(E(std::forward<Args>(args)...));
  }

  /// @brief Exchange the contents of this instance with those of other.
  /// @param[in,out] other The other source instance.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611372
  void Swap(Result& other) noexcept(
      std::is_nothrow_move_constructible<T>::value&& std::is_nothrow_move_assignable<T>::value&&
          std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
  {
    using std::swap;
    swap(mData_, other.mData_);
  }

  /// @brief Check whether reference to this contains a value.
  /// @returns True if reference to this contains a value, false otherwise.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611373
  bool HasValue() const noexcept
  {
    return mData_.has_error() == 0;
  }

  /// @brief Check whether reference to this contains a value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611374
  explicit operator bool() const noexcept
  {
    return HasValue();
  }

  /// @brief Access the contained value.
  /// This functions behavior is undefined if reference to this does not contain a value.
  /// @returns A const_reference to the contained value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611375
  T const& operator*() const&
  {
    return Value();
  }

  /// @brief Access the contained value.
  /// This functions behavior is undefined if reference to this does not contain a value.
  /// @returns An rvalue reference to the contained value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611376
  T&& operator*() &&
  {
    return std::move(*this).Value();
  }

  /// @brief Access the contained value.
  /// This functions behavior is undefined if reference to this does not contain a value.
  /// @returns A pointer to the contained value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611377
  T const* operator->() const
  {
    return std::addressof(Value());
  }

  /// @brief Access the contained value.
  /// The behavior of this function is undefined if reference to this does not contain a value.
  /// @returns A const reference to the contained value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611378
  T const& Value() const& noexcept
  {
    return mData_.value();
  }

  /// @brief Access the contained value.
  /// The behavior of this function is undefined if reference to this does not contain a value.
  /// @returns An rvalue reference to the contained value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611379
  T&& Value() &&
  {
    return std::move(mData_.value());
  }

  /// @brief Access the contained error.
  /// The behavior of this function is undefined if reference to this does not contain an error.
  /// @returns A const reference to the contained error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611380
  E const& Error() const& noexcept
  {
    return mData_.get_error();
  }

  /// @brief Access the contained error.
  /// The behavior of this function is undefined if reference to this does not contain an error.
  /// @returns An rvalue reference to the contained error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611381
  E&& Error() &&
  {
    return std::move(mData_.get_error());
  }

  /// @brief Return the contained value as an Optional.
  /// @returns An Optional with the value, if present.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611382
  Optional<T> Ok() const& noexcept
  {
    return HasValue() ? Optional<T>(Value()) : nullopt;
  }

  /// @brief Return the contained value as an Optional.
  /// @returns An Optional with the value, if present.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611383
  Optional<T> Ok() && noexcept
  {
    return HasValue() ? Optional<T>(std::move(Value())) : nullopt;
  }

  /// @brief Return the contained error as an Optional.
  /// @returns An Optional with the error, if present.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611384
  Optional<E> Err() const& noexcept
  {
    return HasValue() ? nullopt : Optional<E>(Error());
  }

  /// @brief Return the contained error as an Optional.
  /// @returns An Optional with the error, if present.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611385
  Optional<E> Err() && noexcept
  {
    return HasValue() ? nullopt : Optional<E>(std::move(Error()));
  }

  /// @brief Return the contained value or the given default value.
  /// If reference to this contains a value, it is returned.
  /// Otherwise, the specified default value is returned, static_cast d to T.
  /// @tparam U The type of default Value.
  /// @param[in] defaultValue The value to use if reference to this does not contain a value.
  /// @returns  It return the value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611386
  template <typename U>
  T ValueOr(U&& defaultValue) const& noexcept
  {
    return HasValue() ? Value() : static_cast<T>(std::forward<U>(defaultValue));
  }

  /// @brief Return the contained value or the given default value.
  /// If reference to this contains a value, it is returned.
  /// Otherwise, the specified default value is returned, static_cast d to T.
  /// @tparam U The type of default Value.
  /// @param[in] defaultValue The value to use if reference to this does not contain a value.
  /// @returns  It return the value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611387
  template <typename U>
  T ValueOr(U&& defaultValue) &&
  {
    return HasValue() ? std::move(Value()) : static_cast<T>(std::forward<U>(defaultValue));
  }

  /// @brief Return the contained error or the given default error.
  /// If reference to this contains a error, it is returned.
  /// Otherwise, the specified default error is returned, static_cast d to E.
  /// @tparam G The type of default Error.
  /// @param[in] defaultError The value to use if reference to this does not contain a error.
  /// @returns  It return the error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611388
  template <typename G>
  E ErrorOr(G&& defaultError) const& noexcept
  {
    return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : Error();
  }

  /// @brief Return the contained error or the given default error.
  /// If reference to this contains an error, it is moved into the return value.
  /// Otherwise, the specified default error is returned, static_cast d to E.
  /// @tparam G The type of default Error.
  /// @param[in] defaultError The value to use if reference to this does not contain a error
  /// @returns  It return the error
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611389
  template <typename G>
  E ErrorOr(G&& defaultError) &&
  {
    return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : std::move(Error());
  }

  /// @brief Return whether this instance contains the given error.
  /// This call compares the argument error, static_cast d to E,
  /// with the return value from Error().
  /// @tparam G The type of the error argument error.
  /// @param[in] error The error to check.
  /// @returns True if reference to this contains an error that is equivalent
  /// to the given error, false otherwise.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611390
  template <typename G>
  bool CheckError(G&& error) const noexcept
  {
    return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
  }

#ifndef ARA_NO_EXCEPTIONS
  /// @brief Return the contained value or throw an exception.
  /// This function does not participate in overload resolution
  /// when the compiler toolchain does not support C++ exceptions.
  /// @returns A const reference to the contained value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611391
  const T& ValueOrThrow() const& noexcept(false)
  {
    if (HasValue())
    {
      return mData_.value();
    }
    Error().ThrowAsException();
  }

  /// @brief Return the contained value or throw an exception.
  /// This function does not participate in overload resolution
  /// when the compiler toolchain does not support C++ exceptions.
  /// @returns An rvalue reference to the contained value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611392
  T&& ValueOrThrow() && noexcept(false)
  {
    if (HasValue())
    {
      return std::move(mData_.value());
    }
    Error().ThrowAsException();
  }
#endif
  /// @brief Return the contained value or return the result of a function call.
  /// If reference to this contains a value, it is returned.
  /// Otherwise, the specified callable is invoked and its return value
  /// which is to be compatible to type T is returned from this function.
  /// @tparam F It is a Callable type.
  /// @param[in] f Its an Callable instance.
  /// @returns It return the value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611393
  template <typename F>
  T Resolve(F&& f) const noexcept
  {
    return HasValue() ? Value() : std::forward<F>(f)(Error());
  }

 private:
  /// @brief Re-implementation of C++14's std::EnableIfT.
  /// @tparam Condition It is a boolean value.
  /// @tparam U The type U is enabled as member type enable_if::type if Condition is true.
  template <bool Condition, typename U = void>
  using EnableIfT = typename std::enable_if<Condition, U>::type;
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Re-implementation of C++14's std::ResultOfT.
  /// @tparam U If U is a function or a function object type that takes ArgTypes as arguments.
  template <typename U>
  using ResultOfT = typename std::result_of<U>::type;

  /// @brief Implementation of struct IsResult.
  /// @tparam U The default value type.
  template <typename U>
  struct IsResult : std::false_type
  {
  };
  /// @brief Implementation of struct IsResult.
  /// @tparam U The default value type.
  /// @tparam G The default error type.
  template <typename U, typename G>
  struct IsResult<Result<U, G>> : std::true_type
  {
  };
  /// @brief Alias name for EnableIfT.
  /// @tparam F It is a Callable type.
  template <typename F>
  using CallableReturnsResult = EnableIfT<IsResult<ResultOfT<F(T const&)>>::value>;

  /// @brief Alias name for EnableIfT.
  /// @tparam F It is a Callable type.
  template <typename F>
  using CallableReturnsNoResult = EnableIfT<!IsResult<ResultOfT<F(T const&)>>::value>;

 public:
  /// @brief Apply the given Callable to the value of this instance,
  /// and return a new Result with the result of the call.
  /// @tparam F The Callable data type.
  /// @tparam C The Callable return result.
  /// @param[in] f Callable that returns a new Result.
  /// @returns Callable result if the value exists, otherwise a new Result with the copied instance error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611394
  template <typename F, typename C = CallableReturnsResult<F>>
  auto Bind(F&& f) const noexcept -> decltype(f(Value()))
  {
    /// @brief Alias name for decltype.
    using U = decltype(f(Value()));
    return HasValue() ? std::forward<F>(f)(Value()) : U(Error());
  }

  /// @brief Apply the given Callable to the value of this instance,
  /// and return a new Result with the result of the call.
  /// @tparam F It is a Callable type.
  /// @tparam C The Callable do not return result.
  /// @param[in] f The Callable that takes the instance value and returns a new Result.
  /// @returns Callable result if the value exists, otherwise a new Result with the copied instance error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611395
  template <typename F, typename C = CallableReturnsNoResult<F>>
  auto Bind(F&& f) const noexcept -> Result<decltype(f(Value())), E>
  {
    /// @brief Alias name for decltype.
    using U = decltype(f(Value()));
    /// @brief Alias name for Result.
    using R = Result<U, E>;
    return HasValue() ? std::forward<F>(f)(Value()) : R(Error());
  }

  /// @brief Declaration of friend class Result.
  template <typename, typename>
  friend class Result;
};

/// @brief Swap the contents of the two given arguments.
/// @tparam T It is Result value type.
/// @tparam E It is Result error type.
/// @param[in] lhs Instance of the class Result.
/// @param[in] rhs Instance of the class Result.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611850
template <typename T, typename E>
inline void swap(Result<T, E>& lhs, Result<T, E>& rhs) noexcept(noexcept(lhs.Swap(rhs)))
//AVIN_VERIFIED_PREVIOUS CodingStyle-Naming.Function Function names shall be pascal case.
{
  lhs.Swap(rhs);
}

/// @brief Compare two Result instances for equality.
/// @tparam T Instance of the class Result.
/// @tparam E Instance of the class Result.
/// @param[in] lhs The left hand side of the comparison.
/// @param[in] rhs The right hand side of the comparison.
/// @returns True if the two instances compare equal, false otherwise.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611851
template <typename T, typename E>
bool operator==(Result<T, E> const& lhs, Result<T, E> const& rhs) noexcept
{
  if (bool(lhs) != bool(rhs))
  {
    return false;
  }
  else if (lhs)
  {
    return lhs.Value() == rhs.Value();
  }
  else
  {
    return lhs.Error() == rhs.Error();
  }
}

/// @brief Compare two Result instances for inequality.
/// @tparam T The Result value type of T.
/// @tparam E The Result error type of E.
/// @param[in] lhs The left hand side of the comparison.
/// @param[in] rhs The right hand side of the comparison.
/// @returns True if the two instances compare unequal, false otherwise.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611852
template <typename T, typename E>
bool operator!=(Result<T, E> const& lhs, Result<T, E> const& rhs) noexcept
{
  if (bool(lhs) != bool(rhs))
  {
    return true;
  }
  if (lhs)
  {
    return lhs.Value() != rhs.Value();
  }
  return lhs.Error() != rhs.Error();
}

/// @brief Compare a Result instance for equality to a value.
/// @tparam T Value of the type class Result.
/// @tparam E Instance of the class Result.
/// @param[in] lhs Instance of the class Result.
/// @param[in] value The value to compare with.
/// @returns True if the Results value compares equal to the rhs value, false otherwise.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611853
template <typename T, typename E>
bool operator==(Result<T, E> const& lhs, T const& value) noexcept
{
  return lhs ? *lhs == value : false;
}

/// @brief Compare a Result instance for equality to a value.
/// @tparam T The Result value type of T.
/// @tparam E The Result error type of E.
/// @param[in] value The value to compare with.
/// @param[in] rhs Instance of the class Result.
/// @returns True if the Results value compares equal to the lhs value, false otherwise.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611854
template <typename T, typename E>
bool operator==(T const& value, Result<T, E> const& rhs) noexcept
{
  return rhs ? value == *rhs : false;
}

/// @brief Compare a Result instance for inequality to a value.
/// @tparam T The Result value type of T.
/// @tparam E The Result error type of E.
/// @param[in] lhs Instance of the class Result.
/// @param[in] value The value to compare with.
/// @returns True if the Results value compares unequal to the rhs value, false otherwise
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611855
template <typename T, typename E>
bool operator!=(Result<T, E> const& lhs, T const& value) noexcept
{
  return lhs ? *lhs != value : true;
}

/// @brief Compare a Result instance for inequality to a value.
/// @tparam T The Result value type of T.
/// @tparam E The Result error type of E.
/// @param[in] value The value to compare with.
/// @param[in] rhs Instance of the class Result.
/// @returns True if the Results value compares unequal to the lhs value, false otherwise.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611856
template <typename T, typename E>
bool operator!=(T const& value, Result<T, E> const& rhs) noexcept
{
  return rhs ? value != *rhs : true;
}

/// @brief Compare a Result instance for equality to an error.
/// @tparam T The Result value type of T.
/// @tparam E The Result error type of E.
/// @param[in] lhs Instance of the class Result.
/// @param[in] error The error to compare with.
/// @returns True if the Results error compares equal to the rhs error, false otherwise.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611857
template <typename T, typename E>
bool operator==(Result<T, E> const& lhs, E const& error) noexcept
{
  return lhs ? false : lhs.Error() == error;
}

/// @brief Compare a Result instance for equality to an error.
/// @tparam T The Result value type of T.
/// @tparam E The Result error type of E.
/// @param[in] error The error to compare with.
/// @param[in] rhs Instance of the class Result.
/// @returns True if the Results error compares equal to the lhs error, false otherwise.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611858
template <typename T, typename E>
bool operator==(E const& error, Result<T, E> const& rhs) noexcept
{
  return rhs ? false : error == rhs.Error();
}

/// @brief Compare a Result instance for inequality to an error.
/// @tparam T The Result value type of T.
/// @tparam E The Result error type of E.
/// @param[in] lhs Instance of the class Result.
/// @param[in] error The error to compare with .
/// @returns True if the Results error compares unequal to the rhs error, false otherwise.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611859
template <typename T, typename E>
bool operator!=(Result<T, E> const& lhs, E const& error) noexcept
{
  return lhs ? true : lhs.Error() != error;
}

/// @brief Compare a Result instance for inequality to an error.
/// @tparam T The Result value type of T.
/// @tparam E The Result error type of E.
/// @param[in] error The error to compare with.
/// @param[in] rhs Instance of the class Result.
/// @returns True if the Results error compares unequal to the lhs error, false otherwise.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611860
template <typename T, typename E>
bool operator!=(E const& error, Result<T, E> const& rhs) noexcept
{
  return rhs ? true : error != rhs.Error();
}

/// @brief Specialization of class result for "void" values.
/// @tparam E The type of error.
/// @trace SWS_CORE_S7.2.1.3.3_P2
/// @archtrace 609893
/// @design 611507
template <typename E>
class Result<void, E> final
{
  /// @brief Cannot use 'void' for variant, so define our own (default-constructible) Dummy type instead.
  struct Dummy
  {
  };
  /// @brief Alias name for struct Dummy.
  using T = Dummy;

  /// @brief Instance of class expected.
  iox::cxx::expected<void, E> mData_;

 public:
  /// @brief Type alias for the type T of values, always "void" for this specialization.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  using ValueType = void;

  /// @brief Type alias for the type E of errors.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  using ErrorType = E;

  /// @brief Build a new Result with "void" as value.
  /// @returns A Result that contains a "void" value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611508
  static Result FromValue()
  {
    return Result();
  }

  /// @brief Build a new Result from the specified error (given as lvalue).
  /// @param[in] e The error to put into the Result.
  /// @returns A Result that contains the error e.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611509
  static Result FromError(E const& e) noexcept
  {
    return Result(e);
  }

  /// @brief Build a new Result from the specified error (given as rvalue).
  /// @param[in] e The error to put into the Result.
  /// @returns A Result that contains the error e.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611510
  static Result FromError(E&& e)
  {
    return Result(std::move(e));
  }

  /// @brief Build a new Result from an error that is constructed in-place from the given arguments.
  /// @tparam Args The types of arguments given to this function.
  /// @param[in] args The parameter pack used for constructing the error.
  /// @returns A Result that contains an error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611511
  template <typename... Args>
  static Result FromError(Args&&... args) noexcept
  {
    return Result(E{std::forward<Args>(args)...});
  }

  // ----------------------------------------

  /// @brief Construct a new Result with a "void" value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611512
  Result() noexcept : mData_(iox::cxx::success<void>())
  {
  }

  /// @brief Construct a new Result from the specified error (given as lvalue).
  /// @param[in] e The error to put into the Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611513
  explicit Result(E const& e) noexcept : mData_(iox::cxx::error<E>(e))
  {
  }

  /// @brief Construct a new Result from the specified error (given as rvalue).
  /// @param[in] e The error to put into the Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611514
  explicit Result(E&& e) noexcept : mData_(iox::cxx::error<E>(std::move(e)))
  {
  }

  /// @brief Copy-construct a new Result from another instance.
  /// @param[in] other The other source instance.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  Result(Result const& other) = default;

  /// @brief Move-construct a new Result from another instance.
  /// @param[in] other The other source instance.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  Result(Result&& other) noexcept(std::is_nothrow_move_constructible<E>::value) = default;

  /// @brief Destructor.
  /// This destructor is trivial if std::is_trivially_destructible<E>::value is true.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611515
  ~Result() noexcept
  {
  }

  /// @brief Copy-assign another Result to this instance.
  /// @param[in] other The other source instance.
  /// @returns Reference to this, containing the contents of other.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  Result& operator=(Result const& other) & = default;

  /// @brief Move-assign another Result to this instance.
  /// @param[in] other The other source instance.
  /// @returns Reference to this, containing the contents of other.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  Result& operator=(Result&& other)
      & noexcept(std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
      = default;

  // ----------------------------------------

  /// @brief Put a new value into this instance, constructed
  /// in-place from the given arguments.
  /// @tparam Args The types of arguments given to this function.
  /// @param[in] args The arguments used for constructing the value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611516
  template <typename... Args>
  void EmplaceValue(Args&&... args) noexcept
  {
    mData_ = T(args...);
  }

  /// @brief Put a new error into this instance, constructed
  /// in-place from the given arguments.
  /// @tparam Args The types of arguments given to this function.
  /// @param[in] args The arguments used for constructing the error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611517
  template <typename... Args>
  void EmplaceError(Args&&... args) noexcept
  {
    // Unfortunately, Boost.Variant does not support emplace(), so we fall back to assignment here.
    mData_ = std::move(E(std::forward<Args>(args)...));
  }

  /// @brief Exchange the contents of this instance with those of other.
  /// @param[in,out] other Instance of the class Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611518
  void Swap(Result& other) noexcept(
      std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
  {
    using std::swap;
    swap(mData_, other.mData_);
  }

  /// @brief Check whether reference to this contains a value.
  /// @returns True if reference to this contains a value, false otherwise.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611519
  bool HasValue() const noexcept
  {
    return mData_.has_error() == 0;
  }

  /// @brief Check whether result class contains a value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611520
  explicit operator bool() const noexcept
  {
    return HasValue();
  }

  /// @brief Do nothing. The behavior of this function is undefined if reference to this does not contain
  /// a value.
  /// @returns It return value of the class Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611521
  T const& operator*() const
  {
    return Value();
  }

  /// @brief Do nothing. The behavior of this function is undefined if reference to this does not contain
  /// a value.
  /// @returns It return multiple data value of the class Result.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611522
  T const& Value() const
  {
    return mData_.value();
  }

  /// @brief Access the contained error.
  /// @returns A const reference to the contained error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611523
  E const& Error() const& noexcept
  {
    return mData_.get_error();
  }

  /// @brief Access the contained error.
  /// @returns An rvalue reference to the contained error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611524
  E&& Error() &&
  {
    return std::move(mData_.get_error());
  }

  /// @brief Return the contained error as an Optional.
  /// @returns An Optional with the error, if present.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611525
  Optional<E> Err() const& noexcept
  {
    return HasValue() ? nullopt : Optional<E>(Error());
  }

  /// @brief Return the contained error as an Optional.
  /// @returns An Optional with the error, if present.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611526
  Optional<E> Err() && noexcept
  {
    return HasValue() ? nullopt : Optional<E>(std::move(Error()));
  }

  /// @brief Do nothing for method ValueOr.
  /// @tparam U The default value type
  /// @param[in] defaultValue The value to use if reference to this does not contain a value.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611527
  template <typename U>
  void ValueOr(U&& defaultValue) const noexcept
  {
    if (!HasValue())
    {
      static_cast<void>(std::forward<U>(defaultValue));
    }
  }

  /// @brief Return the contained error or the given default error.
  /// If reference to this contains an error, it is returned.
  /// Otherwise, the specified default error is returned, static_castd to E.
  /// @tparam G The default error type.
  /// @param[in] defaultError The error to use if reference to this does not contain an error.
  /// @returns It return the error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611528
  template <typename G>
  E ErrorOr(G&& defaultError) const& noexcept
  {
    return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : Error();
  }

  /// @brief Return the contained error or the given default error.
  /// If reference to this contains an error, it is std moved into the return value.
  /// Otherwise, the specified default error is returned, static_castd to E.
  /// @tparam G The default error type.
  /// @param[in] defaultError The error to use if reference to this does not contain an error.
  /// @returns It return the error.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611529
  template <typename G>
      E ErrorOr(G&& defaultError) && noexcept
  {
    return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : std::move(Error());
  }

  /// @brief Return whether this instance contains the given error.
  /// This call compares the argument error, static_castd to E,
  /// with the return value from Error().
  /// @tparam G The default error type.
  /// @param[in] error The error to check.
  /// @returns True if reference to this contains an error that is equivalent
  /// to the given error, false otherwise
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611530
  template <typename G>
  bool CheckError(G&& error) const noexcept
  {
    return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
  }

#ifndef ARA_NO_EXCEPTIONS

  /// @brief Return the contained value or throw an exception.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611531
  void ValueOrThrow() const noexcept(false)
  {
    T const* ptr{boost::get<T>(&mData_)};
    if (ptr)
    {
      return;
    }

    E const* e{boost::get<E>(&mData_)};
    e->ThrowAsException();
  }
#endif

  /// @brief Do nothing or call a function.
  /// If reference to this contains a value, this function does nothing.
  /// Otherwise, the specified callable is invoked.
  /// The Callable is expected to be compatible to this interface: void f(const E&);
  /// @tparam F It is a Callable type.
  /// @param[in] f Its an Callable instance.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611532
  template <typename F>
  void Resolve(F&& f) const noexcept
  {
    if (!HasValue())
    {
      std::forward<F>(f)(Error());
    }
    else
    {
      //No Code.
    }
  }

 private:
  /// @brief Re-implementation of C++14's std::EnableIfT.
  /// @tparam Condition It is a boolean value.
  /// @tparam U The type of the Value.
  template <bool Condition, typename U = void>
  using EnableIfT = typename std::enable_if<Condition, U>::type;
  // AVIN_VERIFIED_PREVIOUS_CONSTRUCT CodingStyle-Naming.TemplateConstParameter Names of template non-type
  // parameters shall be camel case.

  /// @brief Re-implementation of C++14's std::ResultOfT
  /// @tparam U The type of the Value.
  template <typename U>
  using ResultOfT = typename std::result_of<U>::type;

  /// @brief Trait that detects whether a type is a Result<...>.
  /// @tparam U The type of the Value.
  template <typename U>
  struct IsResult : std::false_type
  {
  };
  /// @brief Trait that detects whether a type is a Result<...>.
  /// @tparam U The type of the Value.
  /// @tparam G The type of the error argument error.
  template <typename U, typename G>
  struct IsResult<Result<U, G>> : std::true_type
  {
  };

  /// @brief Alias name of EnableIfT.
  /// @tparam F It is a Callable type.
  template <typename F>
  using CallableReturnsResult = EnableIfT<IsResult<ResultOfT<F()>>::value>;

  /// @brief Alias name of EnableIfT.
  /// @tparam F It is a Callable type.
  template <typename F>
  using CallableReturnsNoResult = EnableIfT<!IsResult<ResultOfT<F()>>::value>;

 public:
  /// @brief Call the given Callable, and return a new Result.
  /// with the result of the call.
  /// @tparam F It is a Callable type.
  /// @tparam C The Callable return result.
  /// @param[in] f Its an Callable instance.
  /// @returns A new Result instance of the possibly transformed type.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611533
  template <typename F, typename C = CallableReturnsResult<F>>
  auto Bind(F&& f) const noexcept -> decltype(f())
  {
    /// @brief Alias name for decltype.
    using U = decltype(f());
    return HasValue() ? std::forward<F>(f)() : U(Error());
  }

  /// @brief Call the given Callable, and return a new Result, with the result of the call.
  /// @tparam F It is a Callable type.
  /// @tparam C The Callable do not return result.
  /// @param[in] f Its an Callable instance.
  /// @returns A new Result instance of the possibly transformed type.
  /// @trace SWS_CORE_S7.2.1.3.3_P2
  /// @archtrace 609893
  /// @design 611534
  template <typename F, typename C = CallableReturnsNoResult<F>>
  auto Bind(F&& f) const noexcept -> Result<decltype(f()), E>
  {
    /// @brief Alias name of decltype.
    using U = decltype(f());
    /// @brief Alias name of class Result.
    using R = Result<U, E>;
    return HasValue() ? std::forward<F>(f)() : R(Error());
  }
  /// @brief Declaration of friend class result.
  /// @tparam U The type of the value.
  /// @tparam G the type of the error.
  template <typename U, typename G>
  friend class Result;
};

} // namespace core
} // namespace ara

#endif // CORE_SOURCE_CORE_TYPES_INC_PUBLIC_ARA_CORE_RESULT_H_
// -----------------------------------------------------------------------------------------------------------
// AVIN Guideline Exception(s)
// -----------------------------------------------------------------------------------------------------------
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