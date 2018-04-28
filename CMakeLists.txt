cmake_minimum_required(VERSION 3.9)
project(cluster)

find_package( OpenCV 3.3.0 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)
set(HEADERS cluster.hpp)
add_executable(cluster cluster.cpp helpers.cpp randomGenerator.cpp ${HEADERS})

target_link_libraries( cluster ${OpenCV_LIBS})