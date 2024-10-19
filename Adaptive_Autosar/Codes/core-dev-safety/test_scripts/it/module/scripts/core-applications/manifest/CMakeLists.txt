# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction,
# distribution and utilization of this document as well as the communication of
# its contents to others without explicit authorization is prohibited.
# Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
project(Manifest VERSION 1.0.0 LANGUAGES CXX)

  #######################################################################################
  ## Setup for someip json Generation                                                  ##
  #######################################################################################
  set (INPUT_ARXML ${CMAKE_CURRENT_SOURCE_DIR}/../config/49)
  set(GEN_DEST_SV ${CMAKE_CURRENT_SOURCE_DIR}/etc)

  

add_subdirectory(phm)
add_subdirectory(machine_manifest)

ara_install_app_file(FILES ./etc/QtMachine_someip_config.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION etc/ RENAME vsomeip.json)
