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

set(TEST_LINK_LIBRARIES ara::core-types iceoryx_hoofs::iceoryx_hoofs core-config-reader ${GTEST_BOTH_LIBRARIES} ${GMOCK})

add_executable(test_miniparser test_miniparser.cpp)
target_link_libraries(test_miniparser
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

######################################################################################
set(test-targets  test_miniparser
                )
#######################################################################################

#if(NOT ARA_TEST_DIR)
#	set(ARA_TEST_DIR ${CMAKE_INSTALL_PREFIX}/bintest)
#endif()

#install(
#    TARGETS
#    ${test-targets}
#    RUNTIME DESTINATION ${ARA_TEST_DIR}
#)