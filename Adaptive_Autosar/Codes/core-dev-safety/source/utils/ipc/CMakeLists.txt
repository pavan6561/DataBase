# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PRIVATE LIMITED 2022 - All Rights Reserved Reproduction, distribution and utilization
# of this document as well as the communication of its contents to others without explicit authorization is
# prohibited. Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides make environment.
# -----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

project(core-ipc VERSION 1.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(AGNOSAR/buildPlatform)
include(GNUInstallDirs)
include(FindPkgConfig)

find_package(ara-log REQUIRED)
find_package(iceoryx_hoofs REQUIRED)

OPTION(IPC_NAMEDPIPE "IPC channel type" OFF)
IF(IPC_NAMEDPIPE)
    add_definitions(-DIPC_NAMEDPIPE=1)
ENDIF(IPC_NAMEDPIPE)

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

set(LIBRARY_NAME ${PROJECT_NAME})

set(IPC_SRC
  src/ipc_interface_base.cpp
  src/ipc_interface_creator.cpp
  src/ipc_interface_user.cpp
  src/ipc_message_types.cpp
  src/ipc_message.cpp
  src/version_info.cpp
  src/ipc_logging.cpp
)

add_library(${LIBRARY_NAME} STATIC ${IPC_SRC})

set_target_properties(${LIBRARY_NAME}
  PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    CXX_STANDARD 14
    OUTPUT_NAME ${LIBRARY_NAME}
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/>
)
target_link_libraries(${LIBRARY_NAME} 
  ara::log
  iceoryx_hoofs::iceoryx_hoofs
)

add_library(ara::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

if(ARA_ENABLE_TESTS)
	enable_testing()
	find_package(GTest REQUIRED)
	find_library(GMOCK gmock)
  add_subdirectory(src/test)
endif()

if(PKG_CONFIG_FOUND)
  configure_file(${PROJECT_SOURCE_DIR}/files/pkgconfig/${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

# Create *-config.cmake file
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/files/cmake/${PROJECT_NAME}-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NO_SET_AND_CHECK_MACRO
)
install(
  TARGETS ${LIBRARY_NAME}
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  COMPATIBILITY ExactVersion
)
install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}-targets
    NAMESPACE ara::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    COMPONENT dev
)

install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
    COMPONENT dev
)
if(ARA_RUN_TESTS)
docker_exec_test(test_message_ipc ${CMAKE_CURRENT_BINARY_DIR}/src/test) 
endif()
