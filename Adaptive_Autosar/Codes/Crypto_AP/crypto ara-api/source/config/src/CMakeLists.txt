# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for compilation process of crypto-config library.
# ------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

set(CONFIG_SRC
  crypto_config.cpp
)

add_library(crypto-config STATIC ${CONFIG_SRC})

target_link_libraries(
  crypto-config
  PRIVATE
    ara::log
    apd::logconfig
    apd::manifestreader
)

target_include_directories(
  crypto-config
  PUBLIC
    ../inc/public/
    ../../autosar-crypto-files/
    ../../common/inc/public/
)

option(ARA_ENABLE_TESTS "Enable unit testing" OFF)

if (ARA_ENABLE_TESTS)
  enable_testing()
  find_package (GTest REQUIRED)
  find_library (GMOCK gmock)
  find_package (Threads REQUIRED)
endif()
