// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : gmock_matchers.h
//
// Purpose     : This file provides the unit test for gmock matchers.
// -----------------------------------------------------------------------------------------------------------

#ifndef APD_GMOCK_MATCHERS_H_
#define APD_GMOCK_MATCHERS_H_

#include "gmock/gmock.h"

#include <string>
#include <cstddef>

// Check whether a type has a specific return value from .size()
MATCHER_P(HasSize, expected,
    "has a size which is " + std::string(negation ? "not " : "") + ::testing::PrintToString(expected))
{
  *result_listener << "that has a size which is " << ::testing::PrintToString(arg.size());

  size_t const expectedSize = static_cast<size_t>(expected);
  return arg.size() == expectedSize;
}

// Check whether an AUTOSAR ValueOrError or ValueOrNone type has no value.
MATCHER(ArNoValue, "has " + std::string(negation ? "a" : "no") + " value")
{
  if (arg.HasValue())
  {
    *result_listener << "that has a value which is " << ::testing::PrintToString(arg.Value());
    return false;
  }

  *result_listener << "that has no value";

  return true;
}

// Check whether an AUTOSAR ValueOrError or ValueOrNone type has a value.
MATCHER(ArHasValue, "has " + std::string(negation ? "no" : "a") + " value")
{
  if (!arg.HasValue())
  {
    *result_listener << "that has no value";
    return false;
  }

  *result_listener << "that has a value which is " << ::testing::PrintToString(arg.Value());

  return true;
}

// Check whether an AUTOSAR ValueOrError or ValueOrNone type has a value, and whether it is the expected one.
MATCHER_P(ArHasValueWhichIs, expected,
        "has " + std::string(negation ? "no" : "a") + " value "
        "which is " + ::testing::PrintToString(expected))
{
  if (!arg.HasValue())
  {
    *result_listener << "that has no value";
    return false;
  }

  *result_listener << "that has a value which is " << ::testing::PrintToString(arg.Value());

  return arg.Value() == expected;
}

#endif // APD_GMOCK_MATCHERS_H_
