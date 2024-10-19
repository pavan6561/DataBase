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

set(version 1.2.0)

project(ara-core-types VERSION 1.0.0 LANGUAGES CXX)

include(AGNOSAR/buildPlatform)
include(GNUInstallDirs)

find_package(Boost REQUIRED)
find_package(iceoryx_hoofs REQUIRED)


add_compile_options(-Wconversion -Wsign-conversion)
if(CMAKE_COMPILER_IS_GNUCXX AND ARA_RUN_TESTS)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
 list(APPEND common_flags -pedantic -Wall -Wextra)
 list(APPEND cxx_only_flags -fuse-cxa-atexit)
 list(APPEND cxx_only_flags -Wsign-promo -Wnon-virtual-dtor -Wctor-dtor-privacy -Woverloaded-virtual)
#  -fno-elide-constructors is causing the segfault issue.
 list(APPEND common_flags -fno-inline -fno-inline-small-functions -fno-default-inline)
 list(APPEND common_flags -g -O0  -fPIC --coverage)
 add_compile_options(${common_flags})
 add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${cxx_only_flags}>")
 set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -O0 -fPIC --coverage -pthread")
endif()

add_library(core-types STATIC src/monotonic_buffer_resource.cpp
                              src/polymorphic_allocator.cpp
                              src/pool_resource.cpp
                              src/slist.cpp
                              src/unsynchronized_pool_resource.cpp)
                         
target_link_libraries(core-types PUBLIC
		Boost::boost
        iceoryx_hoofs::iceoryx_hoofs
        
)

target_include_directories(core-types PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc/public>
	$<INSTALL_INTERFACE:include>
)
add_library(ara::core-types ALIAS core-types)

if(ARA_ENABLE_TESTS)
	enable_testing()
	find_package(GTest REQUIRED)
	find_library(GMOCK gmock)
	add_subdirectory(test)
endif()

# -----------------------------

include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
    configure_file(${PROJECT_SOURCE_DIR}/files/pkgconfig/${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

# Create *-config.cmake file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/files/cmake/${PROJECT_NAME}-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    NO_SET_AND_CHECK_MACRO
)
# Create *-config-version.cmake file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    COMPATIBILITY ExactVersion
)
# Install *-config.cmake and *-version.cmake files
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
# Install exported targets, i.e. *-targets.cmake
install(
    TARGETS core-types
    EXPORT ${PROJECT_NAME}-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    EXPORT ${PROJECT_NAME}-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    NAMESPACE ara::
)

# Install header files
install(
    DIRECTORY inc/public/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT dev
    FILES_MATCHING PATTERN "*.h"
)

# Install header files
install(
    DIRECTORY inc/public/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT dev
    FILES_MATCHING PATTERN "*.inl"
)

if(ARA_RUN_TESTS)
docker_exec_test(apd-core-testmap ${CMAKE_CURRENT_BINARY_DIR}/test) 
docker_exec_test(apd-core-testvariant ${CMAKE_CURRENT_BINARY_DIR}/test) 
docker_exec_test(apd-core-testoptional ${CMAKE_CURRENT_BINARY_DIR}/test) 
docker_exec_test(apd-core-testvector ${CMAKE_CURRENT_BINARY_DIR}/test) 
docker_exec_test(apd-core-testspan ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-testarray ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-testbyte ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-teststring ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-testresult_noex ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-testresult_ex ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-testfuture ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-testfixedvector ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-testpromise ${CMAKE_CURRENT_BINARY_DIR}/test) 
docker_exec_test(apd-core-teststringview ${CMAKE_CURRENT_BINARY_DIR}/test) 
docker_exec_test(apd-core-testcoreerrordomain ${CMAKE_CURRENT_BINARY_DIR}/test) 
docker_exec_test(apd-core-testerrorcode ${CMAKE_CURRENT_BINARY_DIR}/test) 
docker_exec_test(apd-core-testinstancespecifier ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-teststeadyclock ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-testfixedstring ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-core-testbasicstring ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-pmr-test ${CMAKE_CURRENT_BINARY_DIR}/test)
docker_exec_test(apd-convert-test ${CMAKE_CURRENT_BINARY_DIR}/test)
endif()
