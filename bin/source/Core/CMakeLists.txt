cmake_minimum_required(VERSION 3.11)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

project(NRC)

set(CORE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source/Core/src)

add_library(NRC STATIC
                ${CORE_SOURCE_DIR}/fix.cpp 
                ${CORE_SOURCE_DIR}/base/print.cpp
                ${CORE_SOURCE_DIR}/base/users/OOBE.cpp 
                ${CORE_SOURCE_DIR}/base/users/permissions_enum_class.cpp 
                ${CORE_SOURCE_DIR}/base/users/user.cpp 
                ${CORE_SOURCE_DIR}/base/users/user_manager.cpp 
                ${CORE_SOURCE_DIR}/base/filesystem/pseudo_fs.cpp 
                ${CORE_SOURCE_DIR}/extra/variables.cpp 
                ${CORE_SOURCE_DIR}/base/data/file_manager.cpp 
                ${CORE_SOURCE_DIR}/base/data/data_manager.cpp
                ${CORE_SOURCE_DIR}/experimental/process.cpp
                ${CORE_SOURCE_DIR}/base/command/commands.cpp
                ${CORE_SOURCE_DIR}/base/command/handler_commands.cpp
                ${CORE_SOURCE_DIR}/core.cpp)





