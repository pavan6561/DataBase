# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for compilation process of crypto daemon and creation of crypto
#               service daemon executable.
# ------------------------------------------------------------------------------------------------------------

include(AUTOSAR/apd)
set(CRYPTO_DAEMON_src
  main.cpp
  crypto_daemon.cpp
  ../../ara-crypto/core/ipc-core/src/vsomeip/gen/ara_com_main-cryptoserverexe.cpp
)

add_executable(CryptoServiceDaemon ${CRYPTO_DAEMON_src})

target_link_libraries(CryptoServiceDaemon PUBLIC ara::core)
target_link_libraries(CryptoServiceDaemon PUBLIC ara_com)
target_link_libraries(CryptoServiceDaemon PUBLIC ipc-core)
target_link_libraries(CryptoServiceDaemon PUBLIC ara::log)
target_link_libraries(CryptoServiceDaemon PUBLIC apd::rng)
target_link_libraries(CryptoServiceDaemon PUBLIC Threads::Threads)
target_link_libraries(CryptoServiceDaemon PUBLIC ara::exec_execution_client)
target_link_libraries(CryptoServiceDaemon PUBLIC apd::logconfig)
target_link_libraries(CryptoServiceDaemon PUBLIC crypto)
target_link_libraries(CryptoServiceDaemon PUBLIC ssl)
target_link_libraries(CryptoServiceDaemon PUBLIC certificate-management)
target_link_libraries(CryptoServiceDaemon PUBLIC crypto-utils)
target_link_libraries(CryptoServiceDaemon PUBLIC crypto-config)
target_link_libraries(CryptoServiceDaemon PUBLIC key-storage)
target_link_libraries(CryptoServiceDaemon PUBLIC crypto-provider)
target_link_libraries(CryptoServiceDaemon PUBLIC ara-crypto-core)
target_link_libraries(CryptoServiceDaemon PUBLIC ara_vsomeipbinding)

target_include_directories(CryptoServiceDaemon
  PRIVATE
    ../inc/private
)

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  install(TARGETS CryptoServiceDaemon RUNTIME DESTINATION /opt/CryptoServiceDaemon/bin)
else()
  apd_install_app_executable(crypto-core CryptoServiceDaemon)
endif()
