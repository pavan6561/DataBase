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
find_package(core-cond-var REQUIRED)

set(TEST_LINK_LIBRARIES core-cond-var ara::core-timer ${GTEST_BOTH_LIBRARIES} ${GMOCK})

add_executable(test_timer_thread timer_thread_test.cpp)
target_link_libraries(test_timer_thread
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)
    #add_test(NAME test_timer_thread COMMAND test_timer_thread)

######################################################################################
set(test-targets  test_timer_thread
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