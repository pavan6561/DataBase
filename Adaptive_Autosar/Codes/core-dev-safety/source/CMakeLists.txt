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

add_subdirectory(core)
add_subdirectory(core-types)
add_subdirectory(iceoryx_hoofs)
add_subdirectory(iceoryx_posh)
add_subdirectory(utils/config-reader)
add_subdirectory(utils/timer)
add_subdirectory(utils/thread)
add_subdirectory(utils/ipc)
add_subdirectory(utils/cond-var)