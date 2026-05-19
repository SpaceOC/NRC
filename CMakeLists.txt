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

set(NRC_USE_TUI TRUE)

if(NRC_USE_TUI)
add_compile_definitions(USETUI)
endif()

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
		dylib
		GIT_REPOSITORY  "https://github.com/martin-olivier/dylib"
		GIT_TAG         "v3.0.1"
)

if(NRC_USE_TUI)
# fetch FTXUI
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        v6.1.9
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
endif()

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(dylib)

if(NRC_USE_TUI)
FetchContent_MakeAvailable(ftxui)
endif()

include_directories(headers)

add_subdirectory(${CMAKE_SOURCE_DIR}/source/ThirdParty/mjs)
add_subdirectory(${CMAKE_SOURCE_DIR}/source/Core)
add_subdirectory(${CMAKE_SOURCE_DIR}/source/OS)