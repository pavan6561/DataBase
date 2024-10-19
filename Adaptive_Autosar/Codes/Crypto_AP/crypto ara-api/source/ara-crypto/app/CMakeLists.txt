# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for include sub directory of ara-crypto-app source and unit test.
# ------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)
project(ara-crypto-app VERSION 2.5.0 LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Boost 1.33 REQUIRED COMPONENTS thread)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(AUTOSAR/apd)
include(GNUInstallDirs)

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  add_definitions(-D_LIBCPP_STD_VER=14)
  set(OS "QNX")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -D${OS} -L${AUTOSAR_BASE_DIR}/build/install/usr/local/lib")
  include_directories(${AUTOSAR_BASE_DIR}/build/install/usr/local/include)
endif()

include(FindPkgConfig)

if(CMAKE_COMPILER_IS_GNUCXX AND ARA_RUN_TESTS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
  list(APPEND common_flags -pedantic -Wall -Wextra)
  list(APPEND cxx_only_flags -fuse-cxa-atexit)
  list(APPEND cxx_only_flags -Wsign-promo -Wnon-virtual-dtor -Wctor-dtor-privacy -Woverloaded-virtual)
  list(APPEND common_flags -fno-inline -fno-default-inline -fno-inline-small-functions -fno-elide-constructors)
  list(APPEND common_flags -g -O0 --coverage)
  add_compile_options(${common_flags})
  add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${cxx_only_flags}>")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -O0 --coverage")
endif()

find_package(ara-core REQUIRED)
find_package(ara-com REQUIRED)
find_package(ara-log REQUIRED)
find_package(apd-logconfig REQUIRED)

include_directories(../autosar)
add_subdirectory(ara-crypto-app)
