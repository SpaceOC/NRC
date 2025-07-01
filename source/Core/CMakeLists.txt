cmake_minimum_required(VERSION 3.11)

#set(CMAKE_BUILD_TYPE Release)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

project(NRC)

set(CORE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source/Core/src)

add_library(NRC STATIC
                ${CORE_SOURCE_DIR}/other/print.cpp
                ${CORE_SOURCE_DIR}/other/gprint.cpp
                ${CORE_SOURCE_DIR}/users/permissions_enum_class.cpp 
                ${CORE_SOURCE_DIR}/users/user.cpp
                ${CORE_SOURCE_DIR}/users/user_manager.cpp
                ${CORE_SOURCE_DIR}/users/OOBE.cpp
                ${CORE_SOURCE_DIR}/filesystem/nrfs.cpp 
                ${CORE_SOURCE_DIR}/filesystem/pseudo_fs.cpp
                ${CORE_SOURCE_DIR}/experimental/process.cpp
                ${CORE_SOURCE_DIR}/experimental/run_js_code_utils.cpp
                ${CORE_SOURCE_DIR}/experimental/js_code_native_functions/pseudo_filesystem_functions.cpp
                ${CORE_SOURCE_DIR}/experimental/js_code_native_functions/simple_filesystem_functions.cpp
                ${CORE_SOURCE_DIR}/experimental/js_code_native_functions/user_manager_functions.cpp
                ${CORE_SOURCE_DIR}/experimental/js_code_native_functions/other_functions.cpp
                ${CORE_SOURCE_DIR}/experimental/run_js_code.cpp
                ${CORE_SOURCE_DIR}/experimental/event_manager.cpp
                ${CORE_SOURCE_DIR}/command/command_parser.cpp
                ${CORE_SOURCE_DIR}/command/command_sender.cpp
                ${CORE_SOURCE_DIR}/command/handler_commands.cpp
                ${CORE_SOURCE_DIR}/command/commands_functions/commands_utils.cpp
                ${CORE_SOURCE_DIR}/command/commands_functions/filesystem_commands.cpp
                ${CORE_SOURCE_DIR}/command/commands_functions/user_manager_commands.cpp
                ${CORE_SOURCE_DIR}/command/commands_functions/other_commands.cpp
                ${CORE_SOURCE_DIR}/other/variables.cpp
                ${CORE_SOURCE_DIR}/other/utils.cpp
                ${CORE_SOURCE_DIR}/other/fix.cpp
                ${CORE_SOURCE_DIR}/modules/module_base.cpp
                ${CORE_SOURCE_DIR}/main.cpp)

target_link_libraries(NRC PUBLIC cereal mjs_lib)

target_compile_options(NRC
    PUBLIC
        -O2
        -Wall
        -Wextra
        -Werror
        -Wpedantic
)