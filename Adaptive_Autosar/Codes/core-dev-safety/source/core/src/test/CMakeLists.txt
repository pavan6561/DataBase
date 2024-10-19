# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
# of this document as well as the communication of its contents to others without explicit authorization is
# prohibited. Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides make environment.
# -----------------------------------------------------------------------------------------------------------

# We need BOOST filesystem for creating test manifest
find_package(Boost 1.60.0 REQUIRED COMPONENTS filesystem)

add_executable(apd-core-testabort test_abort.cpp)
target_link_libraries(apd-core-testabort
    PRIVATE
        ara::core
        ${GTEST_BOTH_LIBRARIES}
        ${GMOCK}
)

add_executable(apd-core-testinitialize 
test_initialize.cpp)

target_compile_definitions(apd-core-testinitialize PRIVATE BOOST_FILESYSTEM_NO_DEPRECATED)
target_link_libraries(apd-core-testinitialize
    PRIVATE
        ara::core
        GTest::Main
        Boost::filesystem
)
target_include_directories(apd-core-testinitialize
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc/private/>
)

if(NOT ARA_TEST_DIR)
	set(ARA_TEST_DIR ${CMAKE_INSTALL_PREFIX}/bintest)
endif()

install(
    TARGETS
        apd-core-testabort
        apd-core-testinitialize
    RUNTIME DESTINATION ${ARA_TEST_DIR}
)
