cmake_minimum_required(VERSION 3.10)

project(EscapeRoomClient)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include path
include_directories(include)

# Find ZeroMQ
find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_ZMQ REQUIRED libzmq)
find_path(CPPZMQ_INCLUDE_DIRS zmq.hpp)
include_directories(${PC_ZMQ_INCLUDE_DIRS} ${CPPZMQ_INCLUDE_DIRS})
link_directories(${PC_ZMQ_LIBRARY_DIRS})

# Add the Player library
add_library(PlayerLib src/Player.cpp)

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} PlayerLib ${PC_ZMQ_LIBRARIES})
