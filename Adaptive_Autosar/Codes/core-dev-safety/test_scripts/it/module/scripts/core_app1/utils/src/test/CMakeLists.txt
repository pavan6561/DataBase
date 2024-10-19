# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------

include(GNUInstallDirs)
set(TEST_TARGET Utils_test)

find_path(GMOCK_INCLUDE_DIR gmock/gmock.h)
find_package(Threads REQUIRED)
find_library(GMOCK_LIB NAMES libgmock.a)
find_library(GMOCK_LIB_MAIN NAMES libgmock_main.a)

set(GMOCK_LIBRARIES ${GMOCK_LIB} ${GMOCK_LIB_MAIN})
set(GMOCK_INCLUDE_DIRS ${GMOCK_INCLUDE_DIR})
find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -A -pthread -Wall -D_GLIBCXX_USE_NANOSLEEP -DLINUX -DHAS_VSOMEIP_BINDING")
set(SM_TEST_SRC
    ../sm_config.cpp
    mock/ara_com_main-statemanagerexe.cpp
    sm_config_test.cpp
) 

add_executable(${TEST_TARGET} ${SM_TEST_SRC})

target_include_directories(${TEST_TARGET} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/../../inc/public/
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
)

target_link_libraries(${TEST_TARGET} ${CMAKE_THREAD_LIBS_INIT} -pthread ${ARA_LIBRARIES} build_platform  apd::manifestreader)
target_link_libraries (${TEST_TARGET}  ${ARA_LIBRARIES})
target_link_libraries ( ${TEST_TARGET} ${CMAKE_DL_LIBS})
target_link_libraries ( ${TEST_TARGET} Threads::Threads)
target_link_libraries ( ${TEST_TARGET} ${GTEST_BOTH_LIBRARIES})
target_link_libraries ( ${TEST_TARGET} ${GMOCK_LIBRARIES})
#add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET}"--gtest_break_on_failure")

