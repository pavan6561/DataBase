# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

set(SM_UTIL_src
  sm_config.cpp
)

add_library(sm_util STATIC ${SM_UTIL_src})

target_link_libraries(
  sm_util
  PRIVATE
  ara::log
  iceoryx_hoofs::iceoryx_hoofs
)

target_include_directories(sm_util
  PUBLIC
    ../inc/public
    ../../common/inc/public/datatypes/
  PRIVATE
    ../inc/private
)

if (ARA_ENABLE_TESTS)
  enable_testing()
  find_package (GTest REQUIRED)
  find_library (GMOCK gmock)
  find_package (Threads REQUIRED)
  #add_subdirectory(test)
endif()



