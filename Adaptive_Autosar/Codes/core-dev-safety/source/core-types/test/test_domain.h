// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_domain.h
//
// Purpose     : This file provides the Implementation of Test class Domain.
// -----------------------------------------------------------------------------------------------------------

#ifndef APD_ARA_CORE_TEST_DOMAIN_H_
#define APD_ARA_CORE_TEST_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

enum class TestErrc : ara::core::ErrorDomain::CodeType
{
  some_error = 1,
  another_error,
};

struct TestException : public ara::core::Exception
{
  explicit TestException(ara::core::ErrorCode err) noexcept : Exception(err)
  {
  }
};

class TestErrorDomain final : public ara::core::ErrorDomain
{
 public:
  using Errc = TestErrc;
  using Exception = TestException;

  constexpr TestErrorDomain() noexcept : ErrorDomain(1234)
  {
  }

  char const* Name() const noexcept override
  {
    return "dummy";
  }

  char const* Message(ErrorDomain::CodeType errorCode) const noexcept override
  {
    static_cast<void>(errorCode);
    return "dummy";
  }

  void ThrowAsException(ara::core::ErrorCode const& errorCode) const noexcept(false) override
  {
    ara::core::ThrowOrTerminate<Exception>(errorCode);
  }
};

constexpr TestErrorDomain g_testErrorDomain;

inline constexpr ara::core::ErrorDomain const& GetTestErrorDomain() noexcept
{
  return g_testErrorDomain;
}

inline constexpr ara::core::ErrorCode MakeErrorCode(
    TestErrc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
  return ara::core::ErrorCode(
      static_cast<ara::core::ErrorDomain::CodeType>(code), GetTestErrorDomain(), data);
}

#endif // APD_ARA_CORE_TEST_DOMAIN_H_
