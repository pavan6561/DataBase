# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for ara-crypto-core compilation and building the ara-crypto-core 
#               library.
# ------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

set(ARA_CRYPTO_CORE_SRC
  crypto_core.cpp
)

add_library(ara-crypto-core STATIC ${ARA_CRYPTO_CORE_SRC})

target_link_libraries(
  ara-crypto-core
  PRIVATE
    ara::log
    apd::manifestreader
  PUBLIC
    ipc-core
    ara_com
    apd::logconfig
    ara::core
    crypto-utils
    crypto-config
    crypto-provider
    certificate-management
)

target_include_directories(
	ara-crypto-core
	PUBLIC
    ../../../../common/inc/public/
    ../inc/public/
)
