# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
# of this document as well as the communication of its contents to others without explicit authorization is
# prohibited. Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides make environment.
# -----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

project(ara-core VERSION 1.0.0 LANGUAGES CXX)

include(AGNOSAR/buildPlatform)
include(GNUInstallDirs)

find_package(apd-manifestreader REQUIRED)
find_package(ara-log REQUIRED)
find_package(ara-core-types REQUIRED)
find_package(iceoryx_hoofs REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX AND ARA_RUN_TESTS)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
 list(APPEND common_flags -pedantic -Wall -Wextra)
 list(APPEND cxx_only_flags -fuse-cxa-atexit)
 list(APPEND cxx_only_flags -Wsign-promo -Wnon-virtual-dtor -Wctor-dtor-privacy -Woverloaded-virtual)
 list(APPEND common_flags -fno-inline -fno-inline-small-functions -fno-default-inline -fno-elide-constructors)
 list(APPEND common_flags -g -O0  -fPIC --coverage)
 add_compile_options(${common_flags})
 add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${cxx_only_flags}>")
 set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -O0 -fPIC --coverage -pthread")
endif()

add_subdirectory(src)

if(ARA_ENABLE_TESTS)
	enable_testing()
	find_package(GTest REQUIRED)
	find_library(GMOCK gmock)
	add_subdirectory(src/test)
endif()

if(ARA_RUN_TESTS)
docker_exec_test(apd-core-testabort ${CMAKE_CURRENT_BINARY_DIR}/src/test)
docker_exec_test(apd-core-testinitialize ${CMAKE_CURRENT_BINARY_DIR}/src/test)
endif()