// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_initialize.cpp
//
// Purpose     : This file provides the unit test cases of class Initialize.
// -----------------------------------------------------------------------------------------------------------
#include "apd/manifestreader/manifest_reader.h"
#include "ara/core/initialization.h"
#include "ara/core/result.h"
#include "../initialize.cpp"
#include "ara/core/initialization_impl.h"
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "gtest/gtest.h"
#include "boost/filesystem.hpp"
#include <fstream>

/*******************************************************************************
@TestCaseId:          UT_CORE_0376
@Test_Description:    Test the functionality of ara::core::Initialize().
@Test_Type:           Fault Injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InitTest
@Requirement Tracing: -
@Design Tracing:      -
*******************************************************************************/
TEST(InitTest, Initialize)
{
  // SETUP - prepare exec manifest for the test
  static const ara::core::String manifestFile = "/usr/var/apdtest/core/manifest.json";

  {
    std::ofstream os(manifestFile.c_str());
    os << "{                                                \
            \"logtrace_process_id\": \"AAAA\",                  \
            \"logtrace_process_desc\": \"Test description\",    \
            \"logtrace_log_modes\": [                            \
                \"CONSOLE\",                                    \
                \"NETWORK\",                                    \
                \"FILE\"                                        \
            ],                                                  \
            \"logtrace_default_log_level\": \"WARN\",           \
            \"logtrace_file_path\": \"var/redirected\",         \
            \"executable\":                                     \
            {                                                   \
                \"uses_logging\": true                          \
            }                                                   \
        }";
  }

  // TEST
  ara::core::Result<void> result = ara::core::internal::Initialize(manifestFile);
  /* @Start: UT_CORE_0376 */
  /* @TS1: Invoke the Initialize(). */
  /* @TE1: Initialize() should return HasValue() as true. */
  /* @End: UT_CORE_0376 */
  EXPECT_FALSE(result.HasValue());

  // CLEANUP - remove the manifest created before
  boost::filesystem::remove(manifestFile.c_str());
}
/*******************************************************************************
@TestCaseId:          UT_CORE_0377
@Test_Description:    Test the functionality of ara::core::Deinitialize().
@Test_Type:           Fault Injection Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          InitTest
@Requirement Tracing: -
@Design Tracing:      -
*******************************************************************************/
TEST(InitTest, Deinitialize)
{
  ara::core::Result<void> result = ara::core::Deinitialize();
  /* @Start: UT_CORE_0377 */
  /* @TS2: Invoke the Deinitialize(). */
  /* @TE2: Deinitialize() should return HasValue() as true. */
  /* @End: UT_CORE_0377 */
  EXPECT_TRUE(result.HasValue());
}

///@todo To provide the test specifications.
TEST(InitTest, ConvertStringVectorToLogMode)
{
  ara::core::String str1 = "NETWORK";
  const ara::core::Vector<ara::core::String> input{str1};
  ara::log::LogMode retValue = ConvertStringVectorToLogMode(input);
}

///@todo To provide the test specifications.
TEST(InitTest, ConvertStringToLogLevel)
{
  const ara::core::String str1 = "FATAL";
  ara::log::LogLevel retValue1 = ConvertStringToLogLevel(str1);
  EXPECT_EQ(retValue1, ara::log::LogLevel::kFatal);

  const ara::core::String str2 = "ERROR";
  ara::log::LogLevel retValue2 = ConvertStringToLogLevel(str2);
  EXPECT_EQ(retValue2, ara::log::LogLevel::kError);

  const ara::core::String str3 = "WARN";
  ara::log::LogLevel retValue3 = ConvertStringToLogLevel(str3);
  EXPECT_EQ(retValue3, ara::log::LogLevel::kWarn);

  const ara::core::String str4 = "INFO";
  ara::log::LogLevel retValue4 = ConvertStringToLogLevel(str4);
  EXPECT_EQ(retValue4, ara::log::LogLevel::kInfo);

  const ara::core::String str5 = "core";
  ara::log::LogLevel retValue5 = ConvertStringToLogLevel(str5);
  EXPECT_EQ(retValue5, ara::log::LogLevel::kVerbose);
}

///@todo To provide the test specifications.
TEST(InitTest, Initialize2)
{
  EXPECT_NO_THROW(ara::core::Initialize());
}

///@todo To provide the test specifications.
void WriteToFile(const ara::core::String& path, const ara::core::String& ptree)
{
  std::ofstream out;
  out.open(path.c_str(), std::ios::out);
  if (out.is_open())
  {
    out << ptree;
    out.close();
  }
}

///@todo To provide the test specifications.
void SetUpCore()
{
  ara::core::String sm_json = R"( {
"logtrace_process_id":"UNIT",
"logtrace_process_desc":"UNIT",
"logtrace_log_modes":["UNIT"],
"logtrace_default_log_level":"UNIT",
"logtrace_file_path":"UNIT",
"executable.uses_logging":true
})";

  ara::core::String path = "/etc/sm.json";
  WriteToFile(path, sm_json);
}

///@todo To provide the test specifications.
TEST(InitTest, ConfigureLogging)
{
  SetUpCore();
  sleep(1);
  ara::core::StringView path = "/etc/sm.json";
  auto manifestRes = apd::manifestreader::OpenManifest(path);
  auto manifest = std::move(manifestRes).Value();
  ConfigureLogging(std::move(manifest));
  SetUpCore();
  ara::core::Result<void> result = ara::core::internal::Initialize(path);
}
