# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------

set(APP_NAME StateManager)
set(APP_EXECUTABLE ${APP_NAME})
set(ARA_LIBRARIES ara::ara_log)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(AGNOSAR/buildPlatform)

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ara-api/lge-ap/ExecutionManager/qemu/usr/include/")

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ara-api/lge-ap/ExecutionManager/qnx/board/usr/include/")

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
    set(OS "QNX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -D${OS} -L${AUTOSAR_BASE_DIR}/build/install/usr/local/lib")
    include_directories(${AUTOSAR_BASE_DIR}/build/install/usr/local/include)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g")
endif()

# Application
set(ADAPTIVEAUTOSARAPPLICATION_src
     application.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../gen/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../gen/ara/phm/
)
find_package(core-ipc REQUIRED)
find_package(iceoryx_hoofs REQUIRED)
find_package(appclient REQUIRED)
find_package(utils REQUIRED)
find_package(ara-log REQUIRED)
find_package(ara-core REQUIRED)
find_package(ara-exec-execution-client REQUIRED)
find_package(ara-exec-state-client REQUIRED)
find_package(Threads REQUIRED)
find_package(smserver REQUIRED)
find_package(build_platform REQUIRED)

# Build adaptive application (incl. Core glue code)
add_executable(${APP_EXECUTABLE} ${ADAPTIVEAUTOSARAPPLICATION_src})


if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  target_link_libraries(${APP_EXECUTABLE} ${CMAKE_THREAD_LIBS_INIT} ara-phm-app  dlt ara::log ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ara-api/lge-ap/ExecutionManager/qnx/board/usr/lib/libara_exec_execution_client.a 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ara-api/lge-ap/ExecutionManager/qnx/board/usr/lib/libara_exec_state_client.a) 
else()
  target_link_libraries(${APP_EXECUTABLE} ${CMAKE_THREAD_LIBS_INIT} iceoryx_hoofs::iceoryx_hoofs  ara::core-ipc  ara::appclient  ara::smserver ara::log ara::exec_execution_client ara::exec_state_client )
endif()

 ara_install_app_executable(${APP_EXECUTABLE} ${APP_EXECUTABLE})


# This has to be checked by EMO
if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  install(FILES ../../config/manifest/MANIFEST_phmsm_app.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION /opt/${APP_EXECUTABLE}/etc)
else()
  ara_install_app_file_rename( ${APP_EXECUTABLE} etc  ../../config/manifest/MANIFEST_phmsm_app.json MANIFEST.json)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  install( ${APP_EXECUTABLE} RUNTIME DESTINATION /opt/${APP_EXECUTABLE}/bin)
  execute_process(COMMAND mkfifo -m 0666 phmcore-client-fifo)
else()
  #ara_install_app_executable( {APP_EXECUTABLE} {APP_EXECUTABLE})
endif()
