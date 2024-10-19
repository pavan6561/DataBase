# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
# of this document as well as the communication of its contents to others without explicit authorization is
# prohibited. Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides make environment.
# -----------------------------------------------------------------------------------------------------------

find_package(iceoryx_hoofs REQUIRED)

set(TEST_LINK_LIBRARIES core-cond-var iceoryx_hoofs::iceoryx_hoofs ${GTEST_BOTH_LIBRARIES} ${GMOCK})
set(SM_CORE_src
test_popo_condition_variable.cpp
mock/src/timing_test.cpp)
add_executable(test_condition ${SM_CORE_src})

target_include_directories(test_condition
  PUBLIC
mock/inc
)
target_link_libraries(test_condition
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)
    #add_test(NAME test_condition COMMAND test_condition)

######################################################################################
set(test-targets  test_condition
                )

#######################################################################################

if(NOT ARA_TEST_DIR)
	set(ARA_TEST_DIR ${CMAKE_INSTALL_PREFIX}/bintest)
endif()

install(
    TARGETS
    ${test-targets}
    RUNTIME DESTINATION ${ARA_TEST_DIR}
)