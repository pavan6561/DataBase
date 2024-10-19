// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : result_demo.cpp
//
// Purpose     : This file provides the unit test cases of class Array.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/result.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "ara/core/promise.h"
#include "ara/core/future.h"

#include <string>
#include <type_traits>
#include <iostream>
#include <cstdint>
#include <cstdio>

using ara::core::Result;

inline std::ostream& operator<<(std::ostream& out, ara::core::Exception const& ex)
{
  return (out << "Exception: " << ex.Error() << " (" << ex.what() << ")");
}

enum class posix_errc : ara::core::ErrorDomain::CodeType
{
  invalid_argument = 22,
  operation_not_permitted,
  no_such_file_or_directory,
};

class PosixException : public ara::core::Exception
{
 public:
  explicit PosixException(ara::core::ErrorCode err) noexcept : Exception(err)
  {
  }
};

class TestErrorDomain final : public ara::core::ErrorDomain
{
  constexpr static ErrorDomain::IdType kId = 0x1234;

 public:
  using Errc = posix_errc;
  using Exception = PosixException;

  constexpr TestErrorDomain() noexcept : ErrorDomain(kId)
  {
  }

  char const* Name() const noexcept override
  {
    return "Test";
  }

  char const* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override
  {
    Errc const code = static_cast<Errc>(errorCode);
    switch (code)
    {
    case Errc::invalid_argument:
      return "Invalid argument";
    default:
      return "Unknown error";
    }
  }

  void ThrowAsException(ara::core::ErrorCode const& errorCode) const noexcept(false) override
  {
    ara::core::ThrowOrTerminate<Exception>(errorCode);
  }
};

namespace internal
{
constexpr TestErrorDomain g_testErrorDomain;
}

inline constexpr ara::core::ErrorDomain const& GetTestErrorDomain() noexcept
{
  return internal::g_testErrorDomain;
}

inline constexpr ara::core::ErrorCode MakeErrorCode(
    posix_errc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
  return ara::core::ErrorCode(
      static_cast<ara::core::ErrorDomain::CodeType>(code), GetTestErrorDomain(), data);
}

static Result<int> func_ok()
{
  return 256;
}

static Result<int> func_fail()
{
  return Result<int>::FromError(posix_errc::operation_not_permitted);
}

static Result<void> func_void_ok()
{
  return Result<void>();
}

static Result<void> func_void_fail()
{
  return Result<void>::FromError(posix_errc::operation_not_permitted);
}

// -------------------------------------------------------------------------

class NoDefault
{
  int mX;

  friend std::ostream& operator<<(std::ostream& os, NoDefault const& v)
  {
    return (os << "NoDefault:" << v.mX);
  }

 public:
  explicit NoDefault(int x) : mX(x)
  {
  }
};

// -------------------------------------------------------------------------

class Instrumented
{
 public:
  enum
  {
    IS_COPY_CTORD = 1,
    IS_MOVE_CTORD = 2,
    IS_COPY_ASSIGNED = 4,
    IS_MOVE_ASSIGNED = 8,
  };

  unsigned mOps = 0;

  Instrumented(int, int)
  {
  }

  Instrumented(Instrumented const& other) : mOps(other.mOps | IS_COPY_CTORD)
  {
  }

  Instrumented(Instrumented&& other) : mOps(other.mOps | IS_MOVE_CTORD)
  {
  }

  Instrumented& operator=(Instrumented const& other)
  {
    mOps = other.mOps | IS_COPY_ASSIGNED;
    return *this;
  }

  Instrumented& operator=(Instrumented&& other)
  {
    mOps = other.mOps | IS_MOVE_ASSIGNED;
    return *this;
  }
};

// -------------------------------------------------------------------------

class ClassWithConstructionToken
{
  struct ConstructionToken final
  {
    std::uint32_t value1_ = 0;

    constexpr explicit ConstructionToken(std::uint32_t value1) noexcept : value1_{value1}
    {
    }
  };

 public:
  static Result<ConstructionToken> Preconstruct(std::int32_t value1) noexcept
  {
    if (value1 < 0)
    {
      return Result<ConstructionToken>::FromError(posix_errc::invalid_argument);
    }
    else
    {
      return Result<ConstructionToken>::FromValue(ConstructionToken(static_cast<std::uint32_t>(value1)));
    }
  }

  explicit ClassWithConstructionToken(ConstructionToken&& token) noexcept : value1_{std::move(token.value1_)}
  {
  }

 private:
  std::uint32_t value1_{0};
};

class ClassWithFileResource
{
  struct ConstructionToken final
  {
    std::FILE* file_ = nullptr;

    constexpr explicit ConstructionToken(std::FILE* file) noexcept : file_{file}
    {
    }

    ~ConstructionToken()
    {
      if (file_ != nullptr)
      {
        std::fclose(file_);
      }
    }

    ConstructionToken(ConstructionToken&&) = default;
    ConstructionToken(ConstructionToken const&) = delete;

    ConstructionToken& operator=(ConstructionToken&&) = default;
    ConstructionToken& operator=(ConstructionToken const&) = delete;
  };

 public:
  // All resource acquisition, and everything that can fail, shall be done within this function.
  static Result<ConstructionToken> Preconstruct(char const* fileName) noexcept
  {
    // perform file name check
    std::FILE* file = std::fopen(fileName, "r");
    if (file != nullptr)
    {
      return Result<ConstructionToken>::FromValue(ConstructionToken(file));
    }
    else
    {
      // file could not be opened
      return Result<ConstructionToken>::FromError(posix_errc::no_such_file_or_directory);
    }
  }

  // With a valid "constructor token", this constructor is guaranteed to succeed.
  // Care must be taken, though, that the data within 'token' can all be copied/moved in a non-throwing way.
  explicit ClassWithFileResource(ConstructionToken&& token) noexcept : state_{std::move(token)}
  {
  }

 private:
  ConstructionToken state_;
};

// -------------------------------------------------------------------------

int main()
{
  std::cout << "Starting...\n";

  std::cout << "----\nTest 1: return with success\n";
  Result<int> res1 = func_ok();
  if (res1)
  {
    std::cout << "res1 success: " << res1.Value() << "\n";
  }
  else
  {
    std::cout << "res1 failure: " << res1.Error() << "\n";
  }

  std::cout << "----\nTest 2: return with error, handle noexcept way\n";
  Result<int> res2 = func_fail();
  if (res2)
  {
    std::cout << "res2 success: " << res2.Value() << "\n";
  }
  else
  {
    std::cout << "res2 failure: " << res2.Error() << "\n";
  }

#ifndef ARA_NO_EXCEPTIONS
  std::cout << "----\nTest 3: return with error, handle as exception\n";
  try
  {
    int res3 = func_fail().ValueOrThrow();
    std::cout << "res3 = " << res3 << "\n";
  }
  catch (PosixException const& ex)
  {
    std::cout << "res3 failure, got exception: " << ex.what() << " -> " << ex.Error() << "\n";
    std::cout << "  ex = " << ex << "\n";
  }
#else
  std::cout << "----\nTest 3: (skipped)\n";
#endif

  std::cout << "----\nTest 4: return with error, use .ValueOr()\n";
  int res4 = func_fail().ValueOr(-1);
  std::cout << "res4 = " << res4 << "\n";
  Result<int> r4b = func_fail();
  int res4b = r4b.ValueOr(-1);
  std::cout << "res4b = " << res4b << "\n";

  std::cout << "----\nTest 5: return with error, resolve with a lambda\n";
  int res5 = func_fail().Resolve([](ara::core::ErrorCode const&) { return -2; });
  std::cout << "res5 = " << res5 << "\n";

#ifndef ARA_NO_EXCEPTIONS
  std::cout << "----\nTest 6: future success\n";
  ara::core::Promise<int> p6;
  p6.set_value(42);
  auto f6 = p6.get_future();
  int res6 = f6.get();
  std::cout << "res6 = " << res6 << "\n";

  std::cout << "----\nTest 7: future error, handle as exception\n";
  ara::core::Promise<int> p7;
  p7.SetError(ara::core::ErrorCode{posix_errc::operation_not_permitted});
  auto f7 = p7.get_future();
  try
  {
    int res7 = f7.get();
    std::cout << "res7 = " << res7 << "\n";
  }
  catch (ara::core::Exception const& ex)
  {
    std::cout << "res7 failure, got exception: " << ex.what() << " -> " << ex.Error() << "\n";
    std::cout << "  ex = " << ex << "\n";
  }

  std::cout << "----\nTest 7a: future error with broken promise, handle as exception\n";
  ara::core::Future<int> f7a;
  {
    ara::core::Promise<int> p7a;
    f7a = p7a.get_future();
  }
  try
  {
    int res7a = f7a.get();
    std::cout << "res7a = " << res7a << "\n";
  }
  catch (ara::core::Exception const& ex)
  {
    std::cout << "res7a failure, got exception: " << ex.what() << " -> " << ex.Error() << "\n";
    std::cout << "  ex = " << ex << "\n";
  }

#else
  std::cout << "----\nTest 7: (skipped)\n";
#endif

  std::cout << "----\nTest 8: return with success, use .Bind() successfully\n";
  std::string res8
      = func_ok() //
            .Bind([](int v) { return v + 1; }) //
            .Bind([](int v) { return Result<int>::FromValue(v + 1); }) //
            .Bind([](int v) { return Result<std::string>::FromValue("'" + std::to_string(v) + "'"); }) //
            .Value();
  std::cout << "res8 = " << res8 << "\n";

  std::cout << "----\nTest 9: return with success, use .Bind() with failure\n";
  Result<int> res9 = func_ok()
                         .Bind([](int v) { return Result<int>::FromValue(v + 1); }) //
                         .Bind([](int) { //
                           return Result<int>::FromError(posix_errc::operation_not_permitted); //
                         }) //
                         .Bind([](int v) { return Result<int>::FromValue(v + 2); }); //
  if (res9)
  {
    std::cout << "res9 success: " << res9.Value() << "\n";
  }
  else
  {
    std::cout << "res9 failure: " << res9.Error() << "\n";
  }

  {
    std::cout << "----\nTest 10: construction token with argument check\n";
    constexpr std::int32_t value1 = 1;
    auto pre_constr = ClassWithConstructionToken::Preconstruct(value1);
    if (pre_constr)
    {
      ClassWithConstructionToken object{std::move(pre_constr).Value()};
      std::cout << "res10 success: Object constructed." << std::endl;
    }
    else
    {
      std::cout << "res10 failure: Object cannot be constructed due to an invalid argument.\n";
    }
  }

  {
    std::cout << "----\nTest 11: construction token with operation that can fail\n";
    auto pre_constr = ClassWithFileResource::Preconstruct("SomeFileThatDoesNotExist.txt");
    if (pre_constr)
    {
      ClassWithFileResource object{std::move(pre_constr).Value()};
      std::cout << "res11 success: Object constructed.\n";
    }
    else
    {
      std::cout << "res11 failure: Object cannot be constructed due to an unavailable file.\n";
    }
  }

  {
    std::cout << "----\nTest 12: emplace operations\n";

    Result<int> res12 = func_ok();
    std::cout << "res12 before emplacement: " << res12.Value() << "\n";
    res12.EmplaceValue(42);
    std::cout << "res12 after value emplacement: " << res12.Value() << "\n";
    res12.EmplaceError(posix_errc::operation_not_permitted);
    std::cout << "res12 after error emplacement: " << res12.Error() << "\n";
  }

  {
    std::cout << "----\nTest 13: access operators\n";

    Result<std::string> res13 = std::string("foobar");
    std::cout << "res13 success value: " << *res13 << "\n";
    std::cout << "res13 success size: " << res13->size() << "\n";
  }

  {
    std::cout << "----\nTest 14: nameless constructor from E (with default exception type)\n";

    ara::core::ErrorCode err(posix_errc::operation_not_permitted);
    Result<int> res14a(err);
    std::cout << "res14a lvalue error: " << res14a.Error() << "\n";

    Result<int> res14b(ara::core::ErrorCode{posix_errc::operation_not_permitted});
    std::cout << "res14b rvalue error: " << res14b.Error() << "\n";

#ifndef ARA_NO_EXCEPTIONS
    try
    {
      int val14b = res14b.ValueOrThrow();
      static_cast<void>(val14b);
    }
    catch (ara::core::Exception const& ex)
    {
      std::cout << "res14b failure, got exception: " << ex.what() << " -> " << ex.Error() << "\n";
      std::cout << "  ex = " << ex << "\n";
    }
#endif
  }

  {
    std::cout << "----\nTest 15: non-default-constructible value type\n";

    Result<NoDefault> res15a = NoDefault(42);
    std::cout << "res15a success: " << res15a.Value() << "\n";

    Result<NoDefault> res15b = Result<NoDefault>::FromError(posix_errc::operation_not_permitted);
    std::cout << "res15b error: " << res15b.Error() << "\n";

    NoDefault lvalue(43);
    Result<NoDefault> res15c = lvalue;
    std::cout << "res15c success: " << res15c.Value() << "\n";
  }

  {
    std::cout << "----\nTest 16: assignment\n";

    Result<int> res16v1 = 42;
    Result<int> res16a = res16v1;
    std::cout << "res16a value (copy constructed): " << res16a.Value() << "\n";

    Result<int> res16v2 = 43;
    res16a = res16v2;
    std::cout << "res16a value (copy assigned): " << res16a.Value() << "\n";

    Result<int> res16b = Result<int>(42);
    std::cout << "res16b value (move constructed): " << res16b.Value() << "\n";

    res16b = Result<int>(43);
    std::cout << "res16b value (move assigned): " << res16b.Value() << "\n";
  }

  {
    std::cout << "----\nTest 17: swap\n";

    Result<int> res17a = 42;
    Result<int> res17b = 43;

    res17b.Swap(res17a);
    std::cout << "res17a value (after swap): " << res17a.Value() << "\n";
    std::cout << "res17b value (after swap): " << res17b.Value() << "\n";

    using std::swap;
    swap(res17b, res17a);

    std::cout << "res17a value (after swap): " << res17a.Value() << "\n";
    std::cout << "res17b value (after swap): " << res17b.Value() << "\n";
  }

  {
    std::cout << "----\nTest 19: specialization for void\n";

    Result<void> res19a = func_void_ok();
    res19a.Value(); // void function!
    std::cout << "res19a void success!\n";

    Result<void> res19b = func_void_fail();
    std::cout << "res19b void failure: " << res19b.Error() << "\n";

    res19b.EmplaceValue();
    res19b.Value();
    std::cout << "res19b void now successful after EmplaceValue\n";
  }

  {
    std::cout << "----\nTest 20: support data\n";

    Result<void> res20a = Result<void>::FromError(posix_errc::operation_not_permitted, 2147483647);
    std::cout << "res20a void failure with support data: " << res20a.Error() << "\n";
    std::cout << "  support data = " << res20a.Error().SupportData() << "\n";
  }

  {
    std::cout << "----\nTest 21: FromValue() forwarding named ctor\n";

    Result<NoDefault> res21a = Result<NoDefault>::FromValue(42);
    std::cout << "res21a success: " << res21a.Value() << "\n";
  }

  {
    std::cout << "----\nTest 22: FromError with default exception type\n";

    ara::core::ErrorCode err(posix_errc::operation_not_permitted);
    Result<int> res22a = Result<int>::FromError(err);
    std::cout << "res22a lvalue error: " << res22a.Error() << "\n";

    Result<int> res22b = Result<int>::FromError(posix_errc::operation_not_permitted);
    std::cout << "res22b rvalue error: " << res22b.Error() << "\n";

#ifndef ARA_NO_EXCEPTIONS
    try
    {
      int val22a = res22a.ValueOrThrow();
      static_cast<void>(val22a);
    }
    catch (ara::core::Exception const& ex)
    {
      std::cout << "res22a failure, got exception: " << ex.what() << " -> " << ex.Error() << "\n";
      std::cout << "  ex = " << ex << "\n";
    }
#endif
  }

  {
    std::cout << "----\nTest 23: FromError with support data\n";

    Result<int> res23d = Result<int>::FromError(posix_errc::operation_not_permitted, 123456);
    std::cout << "res23d error: " << res23d.Error() << "\n";
  }

  {
    std::cout << "----\nTest 24: translation of error codes into strings\n";

    Result<int> res24a = Result<int>::FromError(posix_errc::invalid_argument);
    std::cout << "res24a error: " << res24a.Error() << " - " << res24a.Error().Message() << "\n";
  }

  {
    std::cout << "----\nTest 25: .ErrorOr()\n";

    ara::core::ErrorCode err25a = func_fail().ErrorOr(posix_errc::invalid_argument);
    std::cout << "err25a = " << err25a << "\n";

    ara::core::ErrorCode err25b = func_ok().ErrorOr(posix_errc::invalid_argument);
    std::cout << "err25b = " << err25b << "\n";
  }

  {
    std::cout << "----\nTest 26: comparisons\n";

    ara::core::ErrorCode err26a = func_fail().Error();
    if (err26a == posix_errc::invalid_argument)
      std::cout << "err26a: is kInvalidArgument\n";
    else if (err26a == ara::core::future_errc::no_state)
      std::cout << "err26a: is no_state future\n";
    else if (err26a == posix_errc::operation_not_permitted)
      std::cout << "err26a: is SomeError\n";
    else
      std::cout << "err26a: is something else (" << err26a << ")\n";
  }

  {
    std::cout << "----\nTest 27: constexpr ErrorCode\n";
    constexpr ara::core::ErrorCode error = posix_errc::operation_not_permitted;
    constexpr ara::core::ErrorDomain const& errorDomain = error.Domain();
    static_cast<void>(errorDomain);
    constexpr ara::core::ErrorDomain::CodeType errorValue = error.Value();
    constexpr ara::core::ErrorDomain::SupportDataType errorExtra = error.SupportData();
    constexpr bool error_is_einval = error == posix_errc::invalid_argument;
    constexpr bool error_is_not_einval = error != posix_errc::invalid_argument;
    std::cout << "Values from ErrorCode: " << errorValue << ", " << errorExtra << "'\n";
    std::cout << "  " << error_is_einval << ", " << error_is_not_einval << "\n";
  }

  {
    std::cout << "----\nTest 28: .CheckError()\n";

    bool ok28a = func_fail().CheckError(posix_errc::invalid_argument);
    std::cout << "ok28a = " << ok28a << "\n";

    bool ok28b = func_fail().CheckError(posix_errc::operation_not_permitted);
    std::cout << "ok28b = " << ok28b << "\n";
  }

  {
    std::cout << "----\nTest 29: Correct FromValue overloads are called\n";

    Instrumented input1(6, 7);
    Result<Instrumented> r1 = Result<Instrumented>::FromValue(input1);
    assert(r1.Value().mOps == Instrumented::IS_COPY_CTORD); // only copy ctor shall be called

    Instrumented input2(6, 7);
    Result<Instrumented> r2 = Result<Instrumented>::FromValue(std::move(input2));
    assert(r2.Value().mOps == Instrumented::IS_MOVE_CTORD); // only move ctor shall be called

    Result<Instrumented> r3 = Result<Instrumented>::FromValue(6, 7);
    assert(r3.Value().mOps == Instrumented::IS_MOVE_CTORD); // only move ctor shall be called
  }
}
