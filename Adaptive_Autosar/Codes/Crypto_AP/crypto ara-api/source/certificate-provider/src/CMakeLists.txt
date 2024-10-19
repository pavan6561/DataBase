# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
# of this document as well as the communication of its contents to others without explicit authorization is
# prohibited. Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for utils (Crypto Stack).
# -----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

set(CERTIFICATE_MANAGEMENT_SRC
  certmanger.cpp
  certificate_impl.cpp
  x509_dn_impl.cpp
  cert_sign_request_impl.cpp
  x509_provider_impl.cpp
  x509_public_key_info_impl.cpp
  ocsp_request_impl.cpp
  ocsp_response_impl.cpp
  ./../../common/src/algorithm_identification.cpp
)

add_library(certificate-management STATIC ${CERTIFICATE_MANAGEMENT_SRC})

target_include_directories(certificate-management
  PRIVATE

  PUBLIC
    ../inc/public/
    ../../autosar-crypto-files/
    ../../common/inc/public
)

target_link_libraries(certificate-management PUBLIC crypto-provider)

target_link_libraries(certificate-management PRIVATE sqlite3)

option(ARA_ENABLE_TESTS "Enable unit testing" OFF)

if (ARA_ENABLE_TESTS)
  enable_testing()
  find_package (GTest REQUIRED)
  find_library (GMOCK gmock)
  find_package (Threads REQUIRED)
endif()
