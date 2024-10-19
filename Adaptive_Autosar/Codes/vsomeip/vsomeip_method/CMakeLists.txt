
cmake_minimum_required (VERSION 3.0.0)
project (vSomeIPdemo)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(VSOMEIP_NAME "vsomeip3")
include_directories(${VSOMEIP_INCLUDE_DIRS})

add_library(vsomeip_demo_service INTERFACE)
target_sources(vsomeip_demo_service INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/service-example.cpp"
)
target_include_directories(vsomeip_demo_service INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_library(vsomeip_demo_client INTERFACE)
target_sources(vsomeip_demo_client INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/client-example.cpp"
)
target_include_directories(vsomeip_demo_client INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )

    find_package(${VSOMEIP_NAME})
    if (NOT ${VSOMEIP_NAME}_FOUND)
        message("${VSOMEIP_NAME} was not found. Please specify vsomeip_DIR")
    endif()

    add_executable(service service-example.cpp)
    target_link_libraries(service vsomeip_demo_service ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

    add_executable(client client-example.cpp)
    target_link_libraries(client vsomeip_demo_client ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
