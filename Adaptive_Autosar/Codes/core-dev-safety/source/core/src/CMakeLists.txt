# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
# of this document as well as the communication of its contents to others without explicit authorization is
# prohibited. Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides make environment.
# -----------------------------------------------------------------------------------------------------------

set(LIBRARY_NAME "core")
set(LIBRARY_FILE_NAME "ara_core")

add_library(${LIBRARY_NAME}
	initialize.cpp
	abort.cpp
)

target_include_directories(${LIBRARY_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc/public/>
	PRIVATE
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc/private/>
)

target_link_libraries(${LIBRARY_NAME}
	apd::manifestreader
	ara::core-types
	ara::log
)

set_target_properties(${LIBRARY_NAME}
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
		CXX_STANDARD 14
		OUTPUT_NAME ${LIBRARY_FILE_NAME}
)

add_library(ara::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

# -----------------------------

include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
    set(LIBRARIES "-l${LIBRARY_FILE_NAME}")
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
    TARGETS ${LIBRARY_NAME}
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
    DIRECTORY ${PROJECT_SOURCE_DIR}/inc/public/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT dev
    FILES_MATCHING PATTERN "*.h"
)