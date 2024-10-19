
cmake_minimum_required (VERSION 3.0.0)
project (TCP)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(TCP_demo_server INTERFACE)
target_sources(TCP_demo_server INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/server.cpp"
)
target_include_directories(TCP_demo_server INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_library(TCP_demo_client INTERFACE)
target_sources(TCP_demo_client INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/client.cpp"
)
target_include_directories(TCP_demo_client INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )



    add_executable(server server.cpp)
    target_link_libraries(server TCP_demo_server )

    add_executable(client client.cpp)
    target_link_libraries(client TCP_demo_client)
