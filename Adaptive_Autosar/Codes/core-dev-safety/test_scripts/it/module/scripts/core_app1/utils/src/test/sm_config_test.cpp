// ----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// ----------------------------------------------------------------------------------------------------------
// File Name   : sm_config_test.cpp
//
// Purpose     : This file provides the implementation of SM.
// ----------------------------------------------------------------------------------------------------------
#include "boost/property_tree/ptree.hpp"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "sm_config.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstdlib>
using ::testing::_;
using ::testing::Assign;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::SetArgReferee;

namespace ara
{
namespace sm
{
namespace smmanager
{
void WriteToFile(const std::string& path, const ara::core::String& ptree)
{
  std::ofstream out;
  out.open(path.c_str(), std::ios::out);
  if (out.is_open())
  {
    out << ptree;
    out.close();
  }
}
class SmConfigFixture : public ::testing::Test
{
 public:
  SmConfigFixture() : smConfigObj()
  {
  }

  ara::sm::config::SmConfig smConfigObj;
  void SetUp()
  {
    ara::core::String sm_json = R"( {
   "SMConfiguration":{
   "PowerOnFunctionGrp":[
      {    
           
               "function_group_name":"Mode_request",
               "function_group_state":"On"          
         
      } ,
      {    
           
               "function_group_name":"Engine_FG",
               "function_group_state":"On"          
         
      } 
   ],
   "UpdateSessionTimeout":90000,

   "ResetRequestTimeout":10000,

   "DiagnosticAddress":[
        {
            "address":"0XFFFF",
            "function_group_states":
            [
                {
                    "function_group_name": "Mode_request",
                    "function_group_state": "On"
                }
                
            ]
        }
    ],
 "NetworkHandler":[
        {
            "name":"nm_handler_0",
            "instance_id": "SOME/IP:22",
            "machine_states": [  ],
            "function_group_states": [
            "Lane_Change_Assistance.On"
            ],
            "afterrun_timeout_ms": 5000
        }
   ],  "PowerModes":[
   {
      "instance_id":"SOME/IP:301",
      "function_group_name":"Mode_request"
   },
   {
      "instance_id":"SOME/IP:302",
      "function_group_name":"Mode_request"
   }
],
"TriggerIn":[
   {
      "InstanceSpecifier":"SOME/IP:20",
      "FunctionGroupName":"Lane_Change_Assistance"
   }
 ],
"TriggerOut":[
   {
      "InstanceSpecifier":"SOME/IP:21",
      "FunctionGroupName":"Lane_Change_Assistance"
   }
 ],
"TriggerInOut":[
   {
      "InstanceSpecifier":"SOME/IP:19",
      "FunctionGroupName":"Update_Session"
   }
 ]
}})";
    std::string path = "/etc/sm.json";
    WriteToFile(path, sm_json);
    smConfigObj.LoadConfig(path);
  }
};
class SmConfigemptyjsonFixture : public ::testing::Test
{
 public:
  SmConfigemptyjsonFixture() : smConfig_Obj()
  {
  }

  ara::sm::config::SmConfig smConfig_Obj;
  void SetUp()
  {
    ara::core::String phm_empty_json = R"( {})";
    std::string path = "/etc/sm_empty.json";
    WriteToFile(path, phm_empty_json);
    // smConfig_Obj.LoadConfig(path);
  }
};
/*******************************************************************************
@TestCaseId:          UT_SM_0071
@Test_Description:    Test triggers LoadPowerOnFunctionGrpConfig.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SmConfigFixture
@Requirement Tracing: -
@Design Tracing:      529469,529463,529462,529461,529454,529453,529452,529435,
                       529434,529425
*******************************************************************************/
TEST_F(SmConfigFixture, LoadPowerOnFunctionGrpConfig)
{
  EXPECT_EQ(smConfigObj.powerOnFunctionGrp_[0].function_group_name, "Mode_request");
  /* @Start: UT_SM_0071 */
  /* @TS1: call the LoadPowerOnFunctionGrpConfig. */
  /* @TE1.1: powerOnFunctionGrp_[0].function_group_name should be "Mode_request". */
  /* @End: UT_SM_0071 */
}
/*******************************************************************************
@TestCaseId:          UT_SM_0073
@Test_Description:    Test triggers LoadPowerOnFunctionGrpConfig.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SmConfigFixture
@Requirement Tracing: -
@Design Tracing:      529435,529434,529425
*******************************************************************************/
TEST_F(SmConfigemptyjsonFixture, LoadPowerOnFunctionGrpConfig)
{
  EXPECT_TRUE(smConfig_Obj.powerOnFunctionGrp_.size() == 0);
  /* @Start: UT_SM_0073 */
  /* @TS1: call the LoadPowerOnFunctionGrpConfig method of empty json. */
  /* @TE1.1:sizeof of powerOnFunctionGrp_ should be equel to 0. */
  /* @End: UT_SM_0073 */
}
/*******************************************************************************
@TestCaseId:          UT_SM_0519
@Test_Description:    Test triggers the LoadConfig() of SmConfig.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SmConfig
@Requirement Tracing: -
@Design Tracing:      529469,529465
*******************************************************************************/
TEST_F(SmConfigFixture, LoadConfig)
{
  ara::sm::config::SmConfig smConfigObj;
  std::string fileName{""};
  EXPECT_NO_THROW(smConfigObj.LoadConfig(fileName));
  /* @Start: UT_SM_0519 */
  /* @TS1: Invoke the LoadConfig() of SmConfig. */
  /* @TE1.1: LoadConfig() should able to load the mentioned json file. */
  /* @End: UT_SM_0519 */
}
/*******************************************************************************
@TestCaseId:          UT_SM_0521
@Test_Description:    Test triggers the LoadDiagnosticAddressConfig() of SmConfig.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SmConfig
@Requirement Tracing: -
@Design Tracing:      529469,529451,529450,529449
*******************************************************************************/
TEST_F(SmConfigFixture, LoadDiagnosticAddressConfig)
{
  EXPECT_FALSE(smConfigObj.diagnosticAddress_[0].address != "0XFFFF");
  EXPECT_FALSE(smConfigObj.diagnosticAddress_[0].functionGrp[0].function_group_name != "Mode_request");
  /* @Start: UT_SM_0521 */
  /* @TS1:   Invoke the LoadDiagnosticAddressConfig() of SmConfig class. */
  /* @TE1.1: value of diagnosticAddress_ should be 0XFFFF. */
  /* @TE1.2: value of diagnosticAddress_ function_group_name should be "Mode_request". */
  /* @End: UT_SM_0521 */
}
/*******************************************************************************
@TestCaseId:          UT_SM_0522
@Test_Description:    Test triggers the LoadNetworkHandlerConfig() of SmConfig.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SmConfig
@Requirement Tracing: -
@Design Tracing:      529469,529457,529456,529455
*******************************************************************************/
TEST_F(SmConfigFixture, LoadNetworkHandlerConfig)
{
  EXPECT_FALSE(smConfigObj.networkHandler_[0].handleName != "nm_handler_0");
  /* @Start: UT_SM_0522 */
  /* @TS1: Invoke the LoadNetworkHandlerConfig() of SmConfig class. */
  /* @TE1.1: value of networkHandler_ should be nm_handler_0. */
  /* @End: UT_SM_0522 */
}
/*******************************************************************************
@TestCaseId:          UT_SM_0524
@Test_Description:    Test triggers the LoadPowerMode() of SmConfig.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SmConfig
@Requirement Tracing: -
@Design Tracing:      529469,529460,529459,529458
*******************************************************************************/
TEST_F(SmConfigFixture, LoadPowerMode)
{
  EXPECT_FALSE(smConfigObj.powerModes_[0].instance_id != "SOME/IP:301");
  /* @Start : UT_SM_0524 */
  /* @TS1: Invoke the LoadPowerMode() of SmConfig class. */
  /* @TE1.1: value of powerModes_ should be "SOME/IP:301". */
  /* @End: UT_SM_0524 */
}

/*******************************************************************************
@TestCaseId:          UT_SM_0526
@Test_Description:    Test triggers the LoadTriggerIn() of SmConfig.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SmConfig
@Requirement Tracing: -
@Design Tracing:      529472,529471,529470,529466
*******************************************************************************/
TEST_F(SmConfigFixture, LoadTriggerIn)
{
  EXPECT_FALSE(smConfigObj.triggerIn_[0].instanceSpecifier != "SOME/IP:20");
  EXPECT_FALSE(smConfigObj.triggerIn_[0].functionGroupName != "Lane_Change_Assistance");
  /* @Start: UT_SM_0526 */
  /* @TS1: Invoke the LoadTriggerIn() of SmConfig class. */
  /* @TE1.1: value of triggerIn_.instanceSpecifier should be "SOME/IP:20". */
  /* @TE1.2: value of triggerIn_.functionGroupName should be "Lane_Change_Assistance". */
  /* @End: UT_SM_0526 */
}
/*******************************************************************************
@TestCaseId:          UT_SM_0527
@Test_Description:    Test triggers the LoadTriggerOut() of SmConfig.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SmConfig
@Requirement Tracing: -
@Design Tracing:      529478,529477,529476,529467
*******************************************************************************/
TEST_F(SmConfigFixture, LoadTriggerOut)
{
  EXPECT_FALSE(smConfigObj.triggerOut_[0].instanceSpecifier != "SOME/IP:21");
  EXPECT_FALSE(smConfigObj.triggerOut_[0].functionGroupName != "Lane_Change_Assistance");
  /* @Start: UT_SM_0527 */
  /* @TS1: Invoke the LoadTriggerOut() of SmConfig class. */
  /* @TE1.1: value of triggerOut_.instanceSpecifier should be "SOME/IP:21". */
  /* @TE1.2: value of triggerOut_.functionGroupName should be "Lane_Change_Assistance". */
  /* @End: UT_SM_0527 */
}
/*******************************************************************************
@TestCaseId:          UT_SM_0528
@Test_Description:    Test triggers the LoadTriggerInOut() of SmConfig.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       -
@Post-Condition:      -
@Comments:            -
@Test_Group:          SmConfig
@Requirement Tracing: -
@Design Tracing:      529475,529474,529473,529468
*******************************************************************************/
TEST_F(SmConfigFixture, LoadTriggerInOut)
{
  EXPECT_FALSE(smConfigObj.triggerInOut_[0].instanceSpecifier != "SOME/IP:19");
  EXPECT_FALSE(smConfigObj.triggerInOut_[0].functionGroupName != "Update_Session");
  /* @Start: UT_SM_0528 */
  /* @TS1: Invoke the LoadTriggerInOut() of SmConfig class. */
  /* @TE1.1: value of triggerInOut_.instanceSpecifier should be "SOME/IP:19". */
  /* @TE1.2: value of triggerInOut_.instanceSpecifier should be "Update_Session". */
  /* @End: UT_SM_0528 */
}

} // namespace smmanager
} // namespace sm
} // namespace ara
