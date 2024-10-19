# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for utils (Crypto Stack).
# ------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

set(UTILS_SRC
  utils.cpp
  io_interface_sqlite.cpp
  io_interface_volatile.cpp
  volatile_trusted_container_impl.cpp
)

add_library(crypto-utils STATIC ${UTILS_SRC})

target_link_libraries(
  crypto-utils
  PRIVATE
    ara::log
    apd::logconfig
    apd::manifestreader
    sqlite3
)

target_include_directories(crypto-utils
  PUBLIC
    ../inc/public/
    ../../common/inc/public/
    ../../autosar-crypto-files/
)
