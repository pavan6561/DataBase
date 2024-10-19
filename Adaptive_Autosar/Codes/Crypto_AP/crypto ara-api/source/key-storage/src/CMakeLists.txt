# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for compilation process of key-storage library.
# ------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

set(KEY_STORAGE_SRC
  keystorage.cpp
  key_storage_provider_impl.cpp
  keyslot_impl.cpp

)

add_library(key-storage STATIC ${KEY_STORAGE_SRC})

target_include_directories(key-storage
  PRIVATE
    ../inc/private/
  PUBLIC
    ../inc/public/
    ../../common/inc/public
    ../../autosar-crypto-files
)

target_link_libraries(key-storage PUBLIC crypto)
target_link_libraries(key-storage PUBLIC crypto-utils)
target_link_libraries(key-storage PUBLIC crypto-config)
target_link_libraries(key-storage PUBLIC crypto-provider)
#target_link_libraries(key-storage PUBLIC Boost::thread)

option(ARA_ENABLE_TESTS "Enable unit testing" OFF)

if (ARA_ENABLE_TESTS)
  enable_testing()
  find_package (GTest REQUIRED)
  find_library (GMOCK gmock)
  find_package (Threads REQUIRED)
endif()
