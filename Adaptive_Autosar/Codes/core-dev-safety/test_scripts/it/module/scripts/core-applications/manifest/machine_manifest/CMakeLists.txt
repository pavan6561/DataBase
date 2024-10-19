# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction,
# distribution and utilization of this document as well as the communication of
# its contents to others without explicit authorization is prohibited.
# Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
project(cmts VERSION 1.0.0 LANGUAGES CXX)

include(AGNOSAR/buildPlatform)

set(MACHINE_NAME /Machines/QtMachine)
message(STATUS "---------------------------------------------------")
message(STATUS "${APP_EXECUTABLE} - Importing dependencies -------------------")
set(GEN_DEST ${CMAKE_CURRENT_SOURCE_DIR}/../gen)
set(GEN_DEST_MANIFETS ${CMAKE_CURRENT_SOURCE_DIR}/./etc)

message(STATUS "ARAGEN_PATH: ${ARAGEN_PATH}")

set(CMAKE_VERBOSE_MAKEFILE on)

set (INPUT_ARXML ${CMAKE_CURRENT_SOURCE_DIR}/../../config/49/)

install(FILES ./etc/qtmachine_machine_manifest.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION etc/system/ RENAME machine_manifest.json)
