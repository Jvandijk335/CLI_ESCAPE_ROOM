cmake_minimum_required(VERSION 3.10)

project(EscapeRoomServer)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find ZeroMQ (libzmq + cppzmq)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_ZMQ REQUIRED libzmq)
find_path(CPPZMQ_INCLUDE_DIRS zmq.hpp)
include_directories(${PC_ZMQ_INCLUDE_DIRS} ${CPPZMQ_INCLUDE_DIRS})
link_directories(${PC_ZMQ_LIBRARY_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/GameManager.cpp
    src/Item.cpp
    src/PlayerSession.cpp
    src/Puzzle.cpp
    src/Room.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link ZeroMQ
target_link_libraries(${PROJECT_NAME} ${PC_ZMQ_LIBRARIES})