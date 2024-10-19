# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2020 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
project(cmts VERSION 1.0.0 LANGUAGES CXX)
include(AGNOSAR/buildPlatform)
set(PROCESS_NAME /Processes/phm_core)

find_package(build_platform REQUIRED)

message(STATUS "---------------------------------------------------")
message(STATUS "${APP_EXECUTABLE} - Importing dependencies -------------------")
set(GEN_DEST_MANIFETS ${CMAKE_CURRENT_SOURCE_DIR}/etc)

message(STATUS "ARAGEN_PATH: ${ARAGEN_PATH}")

set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB ALL_ARXMLS ${CMAKE_CURRENT_SOURCE_DIR}/../../config/48/*.arxml)
set(ARXMLS ${ALL_ARXMLS} )

set (INPUT_ARXML ${CMAKE_CURRENT_SOURCE_DIR}/../../config/48/)



file(GLOB ALL_ARXMLS ${CMAKE_CURRENT_SOURCE_DIR}/../../config/49/*.arxml)
set(ARXMLS ${ALL_ARXMLS} )

set (INPUT_ARXML ${CMAKE_CURRENT_SOURCE_DIR}/../../config/49/)



install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/etc/platform/gen-manifest/phm.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION /opt/phm-core/etc)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/var/kvsdatabase.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION /opt/phm-core/var)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/etc/MANIFEST_phm_core.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION /opt/phm-core/etc RENAME  MANIFEST.json)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/etc/log_config.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION /opt/phm-core/etc)
