cmake_minimum_required(VERSION 3.11)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

project(NRC-OS)

set(OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY}/bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# https://stackoverflow.com/questions/38700198/how-to-partially-disabling-cmake-c-c-custom-compiler-checking
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

include(FetchContent)

# Я не знаю, как отключить тесты (при попытке скомпилировать их выходит ошибка).
# I don't know how to disable the tests (an error comes out when I try to compile them).
#FetchContent_Declare(
#  mjs
#  URL https://github.com/mras0/mjs/archive/refs/heads/master.zip
#)
#FetchContent_MakeAvailable(mjs)

#set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "" FORCE) # cereal требует библиотеку Boost для этого, поэтому поставлено на ON
#set(BUILD_SANDBOX OFF CACHE BOOL "" FORCE)

#FetchContent_Declare(
#  cereal
#  URL https://github.com/USCiLab/cereal/archive/refs/tags/v1.3.2.zip
#)
#FetchContent_MakeAvailable(cereal)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(headers)

add_subdirectory(${CMAKE_SOURCE_DIR}/source/ThirdParty/mjs)
add_subdirectory(${CMAKE_SOURCE_DIR}/source/Core)
add_subdirectory(${CMAKE_SOURCE_DIR}/source/OS)

set(TESTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/tests/src)

link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

enable_testing()

add_executable(
  tests
  ${TESTS_SOURCE_DIR}/disk.cc
  ${TESTS_SOURCE_DIR}/command_parser.cc
)

target_link_libraries(
  tests
  GTest::gtest_main
  NRC
  mjs_lib
)

include(GoogleTest)
gtest_discover_tests(tests)