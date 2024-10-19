
cmake_minimum_required (VERSION 3.0.0)
project (UDP)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(UDP_demo_server INTERFACE)
target_sources(UDP_demo_server INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/server.cpp"
)
target_include_directories(UDP_demo_server INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_library(UDP_demo_client INTERFACE)
target_sources(UDP_demo_client INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/client.cpp"
)
target_include_directories(UDP_demo_client INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )



    add_executable(server server.cpp)
    target_link_libraries(server UDP_demo_server )

    add_executable(client client.cpp)
    target_link_libraries(client UDP_demo_client)
