# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction,
# distribution and utilization of this document as well as the communication of
# its contents to others without explicit authorization is prohibited.
# Offenders will be held liable for the payment of damages.
# -------------------------------------------------core-app1----------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : To invoke the src makefile.
# -----------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.8)

project(core-app2 VERSION 1.4.0 LANGUAGES CXX)

include(AGNOSAR/buildPlatform)
set(APP_EXECUTABLE core-app2)

include(GNUInstallDirs)

find_package(ara-log REQUIRED)
find_package(apd-logconfig REQUIRED)
find_package(ara-exec-execution-client REQUIRED)
find_package(ara-core REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(iceoryx_posh REQUIRED)
find_package(iceoryx_hoofs REQUIRED)
find_package(build_platform REQUIRED)
find_package(core-ipc REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAS_VSOMEIP_BINDING")

add_executable(${APP_EXECUTABLE} ${ALL_GEN_CPP} src/main.cpp src/application.cpp src/sm_handler.cpp)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
)

target_link_libraries(${APP_EXECUTABLE} ${CMAKE_THREAD_LIBS_INIT}  ara::log apd::logconfig core-ipc
                         ara::exec_execution_client ara::core build_platform ${GTEST_BOTH_LIBRARIES})

 ara_install_app_executable(${APP_EXECUTABLE} ${APP_EXECUTABLE})

set(APP_FOLDER core-app2)
 ara_install_app_manifest(${APP_FOLDER}  ${CMAKE_CURRENT_SOURCE_DIR}/manifest/MANIFEST_core_sample_app2.json)

set(APP_FOLDER core-app2)
ara_install_app_manifest(${APP_FOLDER}/etc/  ${CMAKE_CURRENT_SOURCE_DIR}/manifest/sm_config.json)
