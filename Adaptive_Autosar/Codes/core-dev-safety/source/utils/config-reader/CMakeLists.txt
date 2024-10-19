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

set(version 1.4.0)

project(core-config-reader VERSION 1.4.0 LANGUAGES CXX)

set(LIBRARY_NAME ${PROJECT_NAME})

include(AGNOSAR/buildPlatform)
include(GNUInstallDirs)
include(FindPkgConfig)
include(CMakePackageConfigHelpers)

find_package(iceoryx_hoofs REQUIRED)
find_package(ara-core-types REQUIRED)
find_package(ara-log REQUIRED)

message(STATUS "TESTEST:: ${CMAKE_CURRENT_SOURCE_DIR}")
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

add_library(${LIBRARY_NAME} SHARED
src/config_reader.cpp
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/public/>
)

target_link_libraries(${LIBRARY_NAME}
	PUBLIC
    ara::core-types
)

#Generate *.pc file

find_package(PkgConfig REQUIRED)

if(PKG_CONFIG_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/files/pkgconfig/${LIBRARY_NAME}.pc.in ${LIBRARY_NAME}.pc @ONLY)
endif()

# Install the Library

if(ARA_ENABLE_TESTS)
	enable_testing()
	find_package(GTest REQUIRED)
	find_library(GMOCK gmock)
	add_subdirectory(src/test)
endif()

#add_library(config_reader ALIAS core-types)

install(
	TARGETS ${LIBRARY_NAME}
 	EXPORT ${CMAKE_CURRENT_SOURCE_DIR}-targets
 	ARCHIVE DESTINATION share/doc/config_reader
 	LIBRARY DESTINATION /usr/lib
 )

if(PKG_CONFIG_FOUND)
    configure_file(${PROJECT_SOURCE_DIR}/files/pkgconfig/${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

# Create *-config.cmake file

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/files/cmake/${PROJECT_NAME}-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    NO_SET_AND_CHECK_MACRO
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
    TARGETS ${LIBRARY_NAME}
    EXPORT ${PROJECT_NAME}-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install the imported target definitions

install(EXPORT ${PROJECT_NAME}-targets
    NAMESPACE ara::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    COMPONENT dev
)
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/public/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
    COMPONENT dev
)

if(ARA_RUN_TESTS)
docker_exec_test(test_miniparser ${CMAKE_CURRENT_BINARY_DIR}/src/test) 
endif()