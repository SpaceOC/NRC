cmake_minimum_required(VERSION 3.11)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

project(OS)

set(OS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source/OS/src)

link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(OS 
				${OS_SOURCE_DIR}/os_main.cpp 
				${OS_SOURCE_DIR}/main.cpp 
				${OS_SOURCE_DIR}/other/commands.hpp)
target_link_libraries(OS NRC)

