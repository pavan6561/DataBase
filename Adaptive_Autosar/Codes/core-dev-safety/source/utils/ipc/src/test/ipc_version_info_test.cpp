// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : ipc_version_info_test.cpp
//
// Purpose     : This file contains Unit testing test cases for IpcVersionInfoTest file.
// -----------------------------------------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <gtest/gtest.h>
#include "version_info.hpp"
#include "iceoryx_hoofs/cxx/convert.hpp"
#include <algorithm>

namespace ara
{
namespace core
{
namespace ipc
{
namespace version
{
/*******************************************************************************
@TestCaseId:          UT_CORE_0922
@Test_Description:    Validate VersionInfo instance and constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      611913
*******************************************************************************/
TEST(VersionInfo, VersionInfo)
{
  /* @Start: UT_CORE_0922 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class constructor should be called successfully.*/
  /* @End: UT_CORE_0922 */
  uint16_t versionMajor;
  uint16_t versionMinor;
  uint16_t versionPatch;
  uint16_t versionTweak;
  BuildDateStringT buildDateString;
  CommitIdStringT commitIdString;
  VersionInfo obj(versionMajor, versionMinor, versionPatch, versionTweak, buildDateString, commitIdString);
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0923
@Test_Description:    Validate VersionInfo instance and constructor.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      611913
*******************************************************************************/
TEST(VersionInfo, VersionInfo1)
{
  /* @Start: UT_CORE_0923 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class constructor should be called successfully.*/
  /* @End: UT_CORE_0923 */
  iox::cxx::Serialization obj(" ");
  VersionInfo obj1(obj);
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0924
@Test_Description:    Validate IsValid function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      611195
*******************************************************************************/

TEST(VersionInfo, IsValid)
{
  /* @Start: UT_CORE_0924 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should invoke IsValid().*/
  /* @End: UT_CORE_0924 */
  iox::cxx::Serialization obj(" ");
  VersionInfo obj1(obj);
  obj1.IsValid();
  EXPECT_EQ(obj1.IsValid(), 0);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0925
@Test_Description:    Validate the operator function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      611915
*******************************************************************************/
TEST(VersionInfo, operator_10)
{
  /* @Start: UT_CORE_0925 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should be created successfully.*/
  /* @End: UT_CORE_0925 */
  VersionInfo obj(1, 0, 0, 0, "2023-01-31T14:08:07Z", "Avin");
  iox::cxx::Serialization obj1(obj);
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0926
@Test_Description:    Validate the operator function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      611187
*******************************************************************************/
TEST(VersionInfo, operator_1)
{
  /* @Start: UT_CORE_0926 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should be created successfully.*/
  /* @End: UT_CORE_0926 */
  iox::cxx::Serialization obj5(" "), obj6(" ");
  VersionInfo obj(obj5);
  VersionInfo obj1(obj6);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0927
@Test_Description:    Validate the operator function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      611187
*******************************************************************************/
TEST(VersionInfo, operator)
{
  /* @Start: UT_CORE_0927 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should be called successfully.*/
  /* @End: UT_CORE_0927 */
  iox::cxx::Serialization obj5(" "), obj6(" ");
  VersionInfo obj(obj5);
  VersionInfo obj1(obj6);
  obj == obj1;
  EXPECT_EQ(obj, obj1);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0928
@Test_Description:    Validate the operator function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      611188
*******************************************************************************/
TEST(VersionInfo, operator1)
{
  /* @Start: UT_CORE_0928 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should invoke IsValid().*/
  /* @End: UT_CORE_0928 */
  iox::cxx::Serialization obj5("sys"), obj6("Avin");
  VersionInfo obj(obj5);
  VersionInfo obj1(obj6);
  obj != obj1;
  EXPECT_EQ(obj, obj1);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0929
@Test_Description:    Validate GetCurrentVersion function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      613180
*******************************************************************************/
TEST(VersionInfo, GetCurrentVersion)
{
  /* @Start: UT_CORE_0929 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should invoke IsValid().*/
  /* @End: UT_CORE_0929 */
  iox::cxx::Serialization obj5(" ");
  VersionInfo obj(obj5);
  obj.GetCurrentVersion();
  SUCCEED();
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0930
@Test_Description:    Validate CheckCompatibility function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      613182
*******************************************************************************/
TEST(VersionInfo, kOff)
{
  /* @Start: UT_CORE_0930 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should invoke CheckCompatibility().*/
  /* @End: UT_CORE_0930 */
  VersionInfo obj(1, 0, 0, 0, "2023-01-31T14:08:07Z", "Avin");
  ara::core::ipc::version::CompatibilityCheckLevel compatibilityCheckLevel{
      ara::core::ipc::version::CompatibilityCheckLevel::kOff};
  VersionInfo obj1(obj);
  obj.CheckCompatibility(obj1, compatibilityCheckLevel);
  EXPECT_EQ(obj.CheckCompatibility(obj1, compatibilityCheckLevel), 1);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0931
@Test_Description:    Validate CheckCompatibility function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      613184
*******************************************************************************/
TEST(VersionInfo, checkCompatibility_1)
{
  /* @Start: UT_CORE_0931 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should invoke CheckCompatibility().*/
  /* @End: UT_CORE_0931 */
  VersionInfo obj(1, 0, 0, 0, "2023-01-31T14:08:07Z", "Avin");
  ara::core::ipc::version::CompatibilityCheckLevel compatibilityCheckLevel{
      ara::core::ipc::version::CompatibilityCheckLevel::kMajor};
  VersionInfo obj1(obj);
  obj.CheckCompatibility(obj1, compatibilityCheckLevel);
  EXPECT_EQ(obj.CheckCompatibility(obj1, compatibilityCheckLevel), 1);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0932
@Test_Description:    Validate CheckCompatibility function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      613185
*******************************************************************************/
TEST(VersionInfo, checkCompatibility1)
{
  /* @Start: UT_CORE_0932 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should invoke CheckCompatibility().*/
  /* @End: UT_CORE_0932 */
  VersionInfo obj(1, 0, 0, 0, "2023-01-31T14:08:07Z", "Avin");
  ara::core::ipc::version::CompatibilityCheckLevel compatibilityCheckLevel{
      ara::core::ipc::version::CompatibilityCheckLevel::kMinor};
  VersionInfo obj1(obj);
  obj.CheckCompatibility(obj1, compatibilityCheckLevel);
  EXPECT_EQ(obj.CheckCompatibility(obj1, compatibilityCheckLevel), 1);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0933
@Test_Description:    Validate CheckCompatibility function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      613186
*******************************************************************************/
TEST(VersionInfo, checkCompatibility2)
{
  /* @Start: UT_CORE_0933 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should invoke CheckCompatibility().*/
  /* @End: UT_CORE_0933 */
  VersionInfo obj(1, 0, 0, 0, "2023-01-31T14:08:07Z", "Avin");
  ara::core::ipc::version::CompatibilityCheckLevel compatibilityCheckLevel{
      ara::core::ipc::version::CompatibilityCheckLevel::kPatch};
  VersionInfo obj1(obj);
  obj.CheckCompatibility(obj1, compatibilityCheckLevel);
  EXPECT_EQ(obj.CheckCompatibility(obj1, compatibilityCheckLevel), 1);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0934
@Test_Description:    Validate CheckCompatibility function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      613187
*******************************************************************************/

TEST(VersionInfo, checkCompatibility3)
{
  /* @Start: UT_CORE_0934 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should invoke CheckCompatibility().*/
  /* @End: UT_CORE_0934 */
  VersionInfo obj(1, 0, 0, 0, "2023-01-31T14:08:07Z", "Avin");
  ara::core::ipc::version::CompatibilityCheckLevel compatibilityCheckLevel{
      ara::core::ipc::version::CompatibilityCheckLevel::kCommitId};
  VersionInfo obj1(obj);
  obj.CheckCompatibility(obj1, compatibilityCheckLevel);
  EXPECT_EQ(obj.CheckCompatibility(obj1, compatibilityCheckLevel), 1);
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0935
@Test_Description:    Validate CheckCompatibility function of VersionInfo class.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          IpcVersionInfoTest
@Requirement Tracing: -
@Design Tracing:      613189, 613188, 611205
*******************************************************************************/
TEST(VersionInfo, checkCompatibility4)
{
  /* @Start: UT_CORE_0935 */
  /* @TS1: Initialize VersionInfo class instance. */
  /* @TE1.1: VersionInfo class instance should invoke CheckCompatibility().*/
  /* @End: UT_CORE_0935 */
  VersionInfo obj(1, 0, 0, 0, "2023-01-31T14:08:07Z", "Avin");
  ara::core::ipc::version::CompatibilityCheckLevel compatibilityCheckLevel{
      ara::core::ipc::version::CompatibilityCheckLevel::kBuildDate};
  VersionInfo obj1(obj);
  obj.CheckCompatibility(obj1, compatibilityCheckLevel);
  EXPECT_EQ(obj.CheckCompatibility(obj1, compatibilityCheckLevel), 1);
}

} // namespace version
} // namespace ipc
} // namespace core
} // namespace ara
