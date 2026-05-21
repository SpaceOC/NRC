cmake_minimum_required(VERSION 3.11)
#set(CMAKE_BUILD_TYPE Release)
project(NRC)

include(FetchContent)

FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CORE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source/Core/src)

set(NRC_OPTIONAL_FILES)
set(NRC_OPTIONAL_LIBS)
if(NRC_USE_TUI)
  set(NRC_OPTIONAL_FILES 
    ${CORE_SOURCE_DIR}/tui/elements/StrElement.cpp
    ${CORE_SOURCE_DIR}/tui/elements/ListElement.cpp
    ${CORE_SOURCE_DIR}/tui/screens/base_list_screen.cpp
    ${CORE_SOURCE_DIR}/tui/screens/user_info_screen.cpp
    ${CORE_SOURCE_DIR}/tui/screens/user_manager_screen.cpp
    ${CORE_SOURCE_DIR}/tui/screen_manager.cpp
  )

  set(NRC_OPTIONAL_LIBS 
    ftxui::screen
    ftxui::dom
    ftxui::component
  )
endif()

add_library(NRC STATIC
                ${CORE_SOURCE_DIR}/utils/string_util.cpp
                ${CORE_SOURCE_DIR}/utils/other_util.cpp
                ${CORE_SOURCE_DIR}/utils/language_util.cpp
                ${CORE_SOURCE_DIR}/other/print.cpp
                ${CORE_SOURCE_DIR}/other/gprint.cpp
                ${CORE_SOURCE_DIR}/users/permissions_enum_class.cpp 
                ${CORE_SOURCE_DIR}/users/user.cpp
                ${CORE_SOURCE_DIR}/users/user_manager.cpp
                ${CORE_SOURCE_DIR}/users/OOBE.cpp
                ${CORE_SOURCE_DIR}/filesystem/nrfs.cpp 
                ${CORE_SOURCE_DIR}/filesystem/pseudo_fs.cpp
                ${CORE_SOURCE_DIR}/experimental/thread_manager.cpp
                ${CORE_SOURCE_DIR}/experimental/run_js_code_utils.cpp
                ${CORE_SOURCE_DIR}/experimental/js_code_native_functions/pseudo_filesystem_functions.cpp
                ${CORE_SOURCE_DIR}/experimental/js_code_native_functions/simple_filesystem_functions.cpp
                ${CORE_SOURCE_DIR}/experimental/js_code_native_functions/user_manager_functions.cpp
                ${CORE_SOURCE_DIR}/experimental/js_code_native_functions/other_functions.cpp
                ${CORE_SOURCE_DIR}/experimental/run_js_code.cpp
                ${CORE_SOURCE_DIR}/experimental/event_manager.cpp
                ${CORE_SOURCE_DIR}/command/command_parser.cpp
                ${CORE_SOURCE_DIR}/command/command_sender.cpp
                ${CORE_SOURCE_DIR}/command/commands_handler.cpp
                ${CORE_SOURCE_DIR}/command/commands_functions/commands_utils.cpp
                ${CORE_SOURCE_DIR}/command/commands_functions/filesystem_commands.cpp
                ${CORE_SOURCE_DIR}/command/commands_functions/user_manager_commands.cpp
                ${CORE_SOURCE_DIR}/command/commands_functions/other_commands.cpp
                ${CORE_SOURCE_DIR}/other/variables.cpp
                ${CORE_SOURCE_DIR}/other/fix.cpp
                ${CORE_SOURCE_DIR}/modules/module_metadata.cpp
                ${CORE_SOURCE_DIR}/modules/module_base.cpp
                ${NRC_OPTIONAL_FILES}
                ${CORE_SOURCE_DIR}/main.cpp)

target_link_libraries(NRC
  PUBLIC mjs_lib dylib
  PRIVATE ${NRC_OPTIONAL_LIBS}
)

target_compile_options(NRC
    PUBLIC
        -O2
        -Wall
        -Wextra
        -Werror
        -Wpedantic
)

# Don't touch!!
# Don't touch!!
# Don't touch!!
# Don't touch!!
# Don't touch!!
# Don't touch!!

add_custom_command(
    TARGET NRC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
    COMMENT "Creating data directory"
)

#add_custom_command(
#    TARGET NRC POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -D "URL=https://cdn.simplelocalize.io/public/v1/locales" -D "OUTPUT=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/locales.json" -P ${CMAKE_SOURCE_DIR}/cmake_files/download.cmake
#    COMMENT "Downloading local codes from SimpleLocalize" # Я надеюсь... всё будет ок..?
#)

set(TESTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source/Core/tests/src)

link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

enable_testing()

add_executable(
	tests
	${TESTS_SOURCE_DIR}/pseudo_fs_operations.cc
	${TESTS_SOURCE_DIR}/disk.cc
	${TESTS_SOURCE_DIR}/command_parser.cc
	#${TESTS_SOURCE_DIR}/module_test.cc
)

target_link_libraries(tests PUBLIC GTest::gtest_main PRIVATE NRC
)

include(GoogleTest)
gtest_discover_tests(tests)