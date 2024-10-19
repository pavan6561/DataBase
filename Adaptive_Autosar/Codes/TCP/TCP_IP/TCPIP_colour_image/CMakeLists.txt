cmake_minimum_required(VERSION 3.16)
project( TCP_vehicle_coloure )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(server server.cpp)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS} -lboost_system -lpthread)
add_executable(client client.cpp)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS} -lboost_system -lpthread)

  
