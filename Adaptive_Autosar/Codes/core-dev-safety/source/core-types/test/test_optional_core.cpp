// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : test_optional.cpp
//
// Purpose     : This file provides the unit test cases of class Optional.
// -----------------------------------------------------------------------------------------------------------

#include "ara/core/optional.h"
using ara::core::make_optional;
using ara::core::nullopt;
using ara::core::Optional;

#include "gtest/gtest.h"

#include <string>
/*******************************************************************************
@TestCaseId:          UT_CORE_0405
@Test_Description:    To test the basic functionality of Optional.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          OptionalTest
@Requirement Tracing: -
@Design Tracing:      611614
*******************************************************************************/
TEST(OptionalTest, TypeIsAvailableAndCompilable)
{
  Optional<int> v1;
  Optional<std::string> v2;

  SUCCEED();
  /* @Start: UT_CORE_0405 */
  /* @TS1: Create an instance of Optional for int data type. */
  /* @TE1: Instance should be created. */
  /* @TS2: Create an instance of Optional for std::string data type. */
  /* @TE2: Instance should be created. */
  /* @End: UT_CORE_0405 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0406
@Test_Description:    To test the functionality of constructor of Optional.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          OptionalTest
@Requirement Tracing: -
@Design Tracing:      611614, 611624
*******************************************************************************/
TEST(OptionalTest, DisengagedCtor)
{
  Optional<int> v1;
  Optional<std::string> v2;

  ASSERT_TRUE(!v1);
  ASSERT_TRUE(!v2);

  Optional<int> v3{nullopt};
  Optional<std::string> v4{nullopt};

  ASSERT_TRUE(!v3);
  ASSERT_TRUE(!v4);

  /* @Start: UT_CORE_0406 */
  /* @TS1: Create an instance of Optional as v1. */
  /* @TE1: Instance v1 should be created. */
  /* @TS2: Create an instance of Optional as v2. */
  /* @TE2: Instance v2 should be created. */
  /* @TS3: Invoke the !v1. */
  /* @TE3: !v1 should not have any value. */
  /* @TS4: Invoke the !v2. */
  /* @TE4: !v2 should not have any value. */
  /* @TS5: Create an instance of Optional as v3 and insert nullopt in it. */
  /* @TE5: Instance v3 should be created with nullopt inserted. */
  /* @TS6: Create an instance of Optional as v4 and insert nullopt in it. */
  /* @TE6: Instance v4 should be created with nullopt inserted. */
  /* @TS7: Invoke the !v3. */
  /* @TE7: !v3 should not have any value. */
  /* @TS8: Invoke the !v4. */
  /* @TE8: !v4 should not have any value. */
  /* @End: UT_CORE_0406 */
}

struct Stateful
{
  enum class State
  {
    DefaultConstructed,
    CopyConstructed,
    MoveConstructed
  };
  State m_state;
  int m_value;

  explicit Stateful(int value = 0) : m_state(State::DefaultConstructed), m_value(value)
  {
  }
  Stateful(const Stateful& other) : m_state(State::CopyConstructed), m_value(other.m_value)
  {
  }
  Stateful(Stateful&& other) : m_state(State::MoveConstructed), m_value(std::move(other.m_value))
  {
  }
  Stateful& operator=(const Stateful&) = delete;
  Stateful& operator=(Stateful&&) = delete;
};

struct DefaultConstructible : Stateful
{
  explicit DefaultConstructible(int val = 0) : Stateful(val)
  {
  }
  DefaultConstructible(const DefaultConstructible&) = delete;
  DefaultConstructible(DefaultConstructible&&) = delete;
  DefaultConstructible& operator=(const DefaultConstructible&) = delete;
  DefaultConstructible& operator=(DefaultConstructible&&) = delete;
};

struct CopyConstructible : Stateful
{
  explicit CopyConstructible(int val = 0) : Stateful(val)
  {
  }
  CopyConstructible(const CopyConstructible&) = default;
  CopyConstructible(CopyConstructible&&) = default;
  CopyConstructible& operator=(const CopyConstructible&) = default;
  CopyConstructible& operator=(CopyConstructible&&) = default;
};

struct MoveConstructible : Stateful
{
  explicit MoveConstructible(int val = 0) : Stateful(val)
  {
  }
  MoveConstructible(const MoveConstructible&) = delete;
  MoveConstructible(MoveConstructible&&) = default;
  MoveConstructible& operator=(const MoveConstructible&) = delete;
  MoveConstructible& operator=(MoveConstructible&&) = default;
};

/*******************************************************************************
@TestCaseId:          UT_CORE_0407
@Test_Description:    To test the functionality of ara::core::Optional.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          OptionalTest
@Requirement Tracing: -
@Design Tracing:      611615, 611616, 611617, 611618
*******************************************************************************/
static const int g_intTest{111};
static const std::string g_strTest{"test"};
static const Stateful g_valTest{g_intTest};

TEST(OptionalTest, ValueCtor)
{
  Optional<int> v1{g_intTest};
  Optional<std::string> v2{g_strTest};
  Optional<Stateful> v3{g_valTest};

  ASSERT_FALSE(!v1);
  ASSERT_FALSE(!v2);
  ASSERT_FALSE(!v3);
  ASSERT_EQ(*v1, g_intTest);
  ASSERT_EQ(*v2, g_strTest);
  ASSERT_EQ((*v3).m_state, Stateful::State::CopyConstructed);
  ASSERT_EQ(v3->m_state, Stateful::State::CopyConstructed);
  /* @Start: UT_CORE_0407 */
  /* @TS1: Create an instance of Optional as v1 and insert g_intTest to it. */
  /* @TE1: Instance v1 should be created with g_intTest inserted. */
  /* @TS2: Create an instance of Optional as v2 and insert g_strTest to it. */
  /* @TE2: Instance v2 should be created with g_strTest inserted. */
  /* @TS3: Create an instance of Optional as v3 and insert g_valTest to it. */
  /* @TE3: Instance v3 should be created with g_valTest inserted. */
  /* @TS4: Invoke the !v1. */
  /* @TE4: !v1 should not have any value. */
  /* @TS5: Invoke the !v2. */
  /* @TE5: !v2 should not have any value. */
  /* @TS6: Invoke the !v3. */
  /* @TE6: !v3 should not have any value. */
  /* @TS7: Invoke the *v1. */
  /* @TE7: *v1 should be equal to g_intTest. */
  /* @TS8: Invoke the *v2. */
  /* @TE8: *v2 should be equal to g_strTest. */
  /* @TS9: Invoke the *v3.m_state. */
  /* @TE9: *v3.m_state should be equal to CopyConstructed. */
  /* @TS10: Invoke the v3->m_state. */
  /* @TE10: v3->m_state should be equal to CopyConstructed. */
  /* @End: UT_CORE_0407 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0408
@Test_Description:    To test the functionality of CopyCtor.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          OptionalTest
@Requirement Tracing: -
@Design Tracing:      611617
*******************************************************************************/
TEST(OptionalTest, CopyCtor)
{
  Optional<int> v1{g_intTest};
  Optional<std::string> v2{g_strTest};

  Optional<int> c1{v1};
  Optional<std::string> c2{v2};

  ASSERT_FALSE(!c1);
  ASSERT_FALSE(!c2);
  ASSERT_EQ(*c1, g_intTest);
  ASSERT_EQ(*c2, g_strTest);

  /* @Start: UT_CORE_0408 */
  /* @TS1: Invoke the copy constructor of Optional class. */
  /* @TE1: copy constructor should copy the data from one object to another object. */
  /* @End: UT_CORE_0408 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0409
@Test_Description:    To test the functionality of MoveCtor.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          OptionalTest
@Requirement Tracing: -
@Design Tracing:      611624
*******************************************************************************/
TEST(OptionalTest, MoveCtor)
{
  Optional<int> c1{Optional<int>{g_intTest}};
  Optional<std::string> c2{Optional<std::string>{g_strTest}};

  ASSERT_FALSE(!c1);
  ASSERT_FALSE(!c2);

  ASSERT_EQ(*c1, g_intTest);
  ASSERT_EQ(*c2, g_strTest);

  Optional<CopyConstructible> c4{Optional<CopyConstructible>{CopyConstructible{g_intTest}}};
  ASSERT_EQ(c4->m_value, g_intTest);

  Optional<MoveConstructible> c5{Optional<MoveConstructible>{MoveConstructible{g_intTest}}};
  ASSERT_EQ(c5->m_value, g_intTest);

  /* @Start: UT_CORE_0409 */
  /* @TS1: Invoke the move constructor of Optional class. */
  /* @TE1: move constructor should move the data from one object to another object. */
  /* @End: UT_CORE_0409 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0410
@Test_Description:    To test the functionality of emplace.
@Test_Type:           Fault injection Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          OptionalTest
@Requirement Tracing: -
@Design Tracing:      611636
*******************************************************************************/
TEST(OptionalTest, emplace)
{
  Optional<std::string> c1;
  Optional<std::string> c2;

  //ASSERT_TRUE(!c1);
  //ASSERT_TRUE(!c2);

  //c1.emplace("C-style string", 7);
  //c2.emplace({'C', '-', 's', 't', 'y', 'l', 'e'});

  //ASSERT_FALSE(!c1);
  //ASSERT_FALSE(!c2);
  //ASSERT_EQ(*c1, "C-style");
  //ASSERT_EQ(*c2, "C-style");

  /* @Start: UT_CORE_0410 */
  /* @TS1: Create an instance of Optional as c1. */
  /* @TE1: Instance c1 should be created. */
  /* @TS2: Create an instance of Optional as c2. */
  /* @TE2: Instance c2 should be created. */
  /* @End: UT_CORE_0410 */
}

/*******************************************************************************
@TestCaseId:          UT_CORE_0411
@Test_Description:    To test the functionality of is_optional.
@Test_Type:           Interface Test
@Test_Configuration:  -
@Pre-Condition:       
@Post-Condition:      -
@Comments:            -
@Test_Group:          OptionalTest
@Requirement Tracing: -
@Design Tracing:      611614
*******************************************************************************/
TEST(OptionalTest, is_optional)
{
  Optional<int> v1;
  Optional<std::string> v2;
  Optional<Stateful> v3;
  ASSERT_TRUE(ara::core::is_optional<decltype(v1)>::value);
  ASSERT_TRUE(ara::core::is_optional<decltype(v2)>::value);
  ASSERT_TRUE(ara::core::is_optional<decltype(v3)>::value);

  ASSERT_FALSE(ara::core::is_optional<decltype(g_intTest)>::value);
  ASSERT_FALSE(ara::core::is_optional<decltype(g_strTest)>::value);
  ASSERT_FALSE(ara::core::is_optional<decltype(g_valTest)>::value);
  /* @Start: UT_CORE_0411 */
  /* @TS1: Create an instance of Optional for int data type and invoke the is_optional. */
  /* @TE1: is_optional should return true. */
  /* @TS2: Create an instance of Optional for std::string data type and invoke the is_optional. */
  /* @TE2: is_optional should return true. */
  /* @TS3: Create an instance of Optional for struct data type and invoke the is_optional. */
  /* @TE3: is_optional should return true. */
  /* @End: UT_CORE_0411. */

  /* @Start: UT_CORE_0411 */
  /* @TS4: Invoke the is_optional for int type. */
  /* @TE4: is_optional should return false. */
  /* @TS5: Invoke the is_optional for std::string type. */
  /* @TE5: is_optional should return false. */
  /* @TS6: Invoke the is_optional for struct type. */
  /* @TE6: is_optional should return false. */
  /* @End: UT_CORE_0411. */
}