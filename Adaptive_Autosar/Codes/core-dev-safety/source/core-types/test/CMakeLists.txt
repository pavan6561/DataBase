# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2022 - All Rights Reserved Reproduction, distribution and utilization
# of this document as well as the communication of its contents to others without explicit authorization is
# prohibited. Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides make environment.
# -----------------------------------------------------------------------------------------------------------

find_package(iceoryx_hoofs REQUIRED)
set(TEST_LINK_LIBRARIES ara::core-types iceoryx_hoofs::iceoryx_hoofs ${GTEST_BOTH_LIBRARIES} ${GMOCK})

add_executable(apd-core-testresult_noex test_result.cpp)
target_compile_options(apd-core-testresult_noex PUBLIC -fno-exceptions -fno-rtti)
target_compile_definitions(apd-core-testresult_noex PUBLIC ARA_NO_EXCEPTIONS)
target_link_libraries(apd-core-testresult_noex
    PRIVATE
    iceoryx_hoofs::iceoryx_hoofs
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testmap test_map.cpp)
target_link_libraries(apd-core-testmap
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testvariant test_variant.cpp)
target_link_libraries(apd-core-testvariant
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testresult_ex test_result.cpp)
target_link_libraries(apd-core-testresult_ex
    PUBLIC
    iceoryx_hoofs::iceoryx_hoofs
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testoptional test_optional_core.cpp)
target_link_libraries(apd-core-testoptional
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testvector test_vector.cpp)
target_link_libraries(apd-core-testvector
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testfixedvector test_fixed_vector.cpp)
target_link_libraries(apd-core-testfixedvector
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testspan test_span.cpp)
target_link_libraries(apd-core-testspan
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-teststringview test_string_view.cpp)
target_link_libraries(apd-core-teststringview
    PRIVATE
       ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testarray test_array.cpp)
target_link_libraries(apd-core-testarray
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testfixedstring test_fixed_string.cpp)
target_link_libraries(apd-core-testfixedstring
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testcoreerrordomain test_core_error_domain.cpp)
target_link_libraries(apd-core-testcoreerrordomain
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testerrorcode test_error_code.cpp)
target_link_libraries(apd-core-testerrorcode
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testinstancespecifier test_instance_specifier.cpp)
target_link_libraries(apd-core-testinstancespecifier
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-teststeadyclock test_steady_clock.cpp)
target_link_libraries(apd-core-teststeadyclock
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-testbyte test_byte.cpp)
target_link_libraries(apd-core-testbyte
    PRIVATE
        ${TEST_LINK_LIBRARIES}
)

add_executable(apd-core-teststring test_string.cpp)
target_link_libraries(apd-core-teststring
   PRIVATE
       ${TEST_LINK_LIBRARIES}
)
add_executable(apd-core-testfuture test_future.cpp)
target_link_libraries(apd-core-testfuture
  PRIVATE
      ${TEST_LINK_LIBRARIES}
) 

add_executable(apd-core-testfutureerrordomain test_future_error_domain.cpp)
target_link_libraries(apd-core-testfutureerrordomain
  PRIVATE
      ${TEST_LINK_LIBRARIES}
) 

add_executable(apd-core-testpromise test_promise.cpp)
target_link_libraries(apd-core-testpromise
   PRIVATE
       ${TEST_LINK_LIBRARIES}
)  
add_executable(apd-core-testbasicstring test_basicstring.cpp)
target_link_libraries(apd-core-testbasicstring
   PRIVATE
       ${TEST_LINK_LIBRARIES}
)
add_executable(apd-pmr-test test_pmr.cpp)
target_link_libraries(apd-pmr-test
   PRIVATE
       ${TEST_LINK_LIBRARIES}
)

add_executable(apd-convert-test test_convert.cpp)
target_link_libraries(apd-convert-test
   PRIVATE
       ${TEST_LINK_LIBRARIES}
)

######################################################################################
set(test-targets      
                      apd-core-testmap
					  apd-core-testvariant
					  #apd-core-testresult_ex
					  apd-core-testoptional
					  apd-core-testvector
					  #apd-core-testfixedvector
					  apd-core-testspan
					  apd-core-teststringview
					  apd-core-testarray
					  #apd-core-testfixedstring
					  apd-core-testcoreerrordomain
					  apd-core-testerrorcode
					  apd-core-testinstancespecifier
					  apd-core-teststeadyclock
					  apd-core-testbyte
					  apd-core-teststring 
                      apd-core-testfuture
                      apd-core-testfutureerrordomain
                      apd-core-testpromise
                      apd-pmr-test
                      apd-convert-test
                    )

if(NOT ARA_TEST_DIR)
	set(ARA_TEST_DIR ${CMAKE_INSTALL_PREFIX}/bintest)
endif()

install(
    TARGETS
    ${test-targets}
    RUNTIME DESTINATION ${ARA_TEST_DIR}
)
