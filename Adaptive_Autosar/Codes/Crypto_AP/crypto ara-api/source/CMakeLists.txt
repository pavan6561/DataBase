# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for building CryptoServiceDaemon executable.
# ------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

project(CryptoServiceDaemon VERSION 2.5.0 LANGUAGES CXX)

include(AUTOSAR/apd)
include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
    "Choose the type of build, one of: Debug, Release, RelWithDebInfo, MinSizeRel"
    FORCE
  )
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  add_definitions(-D_LIBCPP_STD_VER=14)
  set(OS "QNX")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -D${OS} -L${AUTOSAR_BASE_DIR}/build/install/usr/local/lib")
  include_directories(${AUTOSAR_BASE_DIR}/build/install/usr/local/include)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -D_GLIBCXX_USE_NANOSLEEP -DLINUX -DHAS_VSOMEIP_BINDING -DARA_COM")

# dependencies
set(CMAKE_THREAD_PREFER_PTHREAD ON)

if("${CMAKE_VERSION}" VERSION_EQUAL "3.10.0" OR "${CMAKE_VERSION}" VERSION_GREATER "3.10.0")
  # This leads to build failure when cross-compiling in CMake below 3.10.0
  set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()

find_package(Threads REQUIRED)
if("${CMAKE_VERSION}" VERSION_LESS "3.10.0" AND CMAKE_USE_PTHREADS_INIT)
  # Fallback to modifying CXX_FLAGS in older versions
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

find_package(ara-core REQUIRED)
find_package(ara-com REQUIRED)
find_package(ara-log REQUIRED)
find_package(apd-rng REQUIRED)
find_package(apd-manifestreader REQUIRED)
find_package(apd-logconfig REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)
if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
    message(STATUS "Using OpenSSL ${OPENSSL}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
    message(FATAL "OpenSSL Not found")
endif()


find_package(ara-exec-execution-client REQUIRED)
find_package(Boost REQUIRED)
#find_package(ara-gen REQUIRED)

include(GNUInstallDirs)

message(STATUS "  Package ara-core found: ${ara-core_FOUND}")
message(STATUS "  Package ara-com found: ${ara-com_FOUND}")
message(STATUS "  Package ara-gen found: ${ara-gen_FOUND}")
message(STATUS "  Package ara-exec-execution-client found: ${ara-exec-execution-client_FOUND}")
message(STATUS "  Package ara-log found: ${ara-log_FOUND}")
message(STATUS "  Package ara-per found: ${ara-per_FOUND}")
message(STATUS "  Package apd-rng found: ${apd-rng_FOUND}")
message(STATUS "  Package apd-logconfig found: ${apd-logconfig_FOUND}")
message(STATUS "  Package apd-manifestreader found: ${apd-manifestreader_FOUND}")
message(STATUS "  Package OpenSSL found: ${OpenSSL_FOUND}")

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  set(ARA_LIBRARIES dlt ara_com ara_vsomeipbinding ara::log apd::rng)
else()
  set(ARA_LIBRARIES ara_com ara_vsomeipbinding ara::log apd::rng)
endif()

option(ARA_ENABLE_UBSAN "Use UndefinedBehaviorSanitizer?" OFF)

set(ARA_MODEL_DIR "${PROJECT_SOURCE_DIR}/files/manifests")

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -Wall -Wextra -Wpedantic -Wsign-promo -Wnon-virtual-dtor -Wctor-dtor-privacy -Woverloaded-virtual -Wold-style-cast -Wpointer-arith -Wshadow -Wunused -Wuninitialized -Winit-self -Wdeprecated -Wfloat-equal")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -Wall -Wextra -Wpedantic -Wsign-promo -Wnon-virtual-dtor -Wctor-dtor-privacy -Woverloaded-virtual -Wold-style-cast -Wpointer-arith -Wshadow -Wunused -Wuninitialized -Winit-self -Wdeprecated -Wfloat-equal")
endif()

if(ARA_ENABLE_UBSAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()

add_definitions(-DHAS_VSOMEIP_BINDING)

message(STATUS "Compiler options: ${CMAKE_CXX_FLAGS}")

# Offer the user the choice of overriding the installation directories
set(INSTALL_CMAKE_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/cmake/DiagnosticManager" CACHE PATH "Installation directory for CMake files")

# Execution Manager expects some configs in the location which doesn't conform
# GNUInstallDirs special cases ((ara-api/exec/execution_manager/README.md).
# Workaround is to use EMCONFDIR instead of SYSCONFDIR in this case.
set(INSTALL_EMCONFDIR "${CMAKE_INSTALL_PREFIX}/etc" CACHE PATH "configuration files location for ExecutionManager")

option(
  ENABLE_GCOV
  "Use Gcov for meassuring test coverage?"
  OFF
)
message(STATUS "option ENABLE_GCOV=" ${ENABLE_GCOV})

option(
  ENABLE_DOXYGEN
  "Create and install the HTML based API documentation "
  OFF
)
message(	STATUS "option ENABLE_DOXYGEN=" ${ENABLE_DOXYGEN})

option(ARA_ENABLE_TESTS "Enable unit testing" OFF)

#if(ENABLE_GCOV)
#  if(CMAKE_COMPILER_IS_GNUCXX)
#    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
#    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
#  endif()
#endif()

if(CMAKE_COMPILER_IS_GNUCXX AND ARA_ENABLE_TESTS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
  list(APPEND common_flags -pedantic -Wall  -Wextra)
  list(APPEND cxx_only_flags -fuse-cxa-atexit)
  list(APPEND cxx_only_flags -Wsign-promo -Wnon-virtual-dtor -Wctor-dtor-privacy -Woverloaded-virtual)
  list(APPEND common_flags -fno-inline -fno-default-inline -fno-elide-constructors)
  list(APPEND common_flags --coverage -fprofile-dir=/build)
  add_compile_options(${common_flags})
  add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${cxx_only_flags}>")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

add_subdirectory(utils)
add_subdirectory(config)
add_subdirectory(crypto-provider)
add_subdirectory(key-storage)
add_subdirectory(certificate-provider)
add_subdirectory(ara-crypto/core)
add_subdirectory(crypto-daemon)
if (ARA_ENABLE_TESTS)
  #enable_testing()
  #add_test(DMUtil_tests "/usr/bin/docker" "exec" "unit_test" "${PROJECT_BINARY_DIR}/utils/src/tst/DMUtil_tests" "--gtest_color=yes")
endif()

