cmake_minimum_required(VERSION 3.16)
project(sunray VERSION 0.15.0 LANGUAGES CXX)

if(POLICY CMP0110)
  cmake_policy(SET CMP0110 OLD)
endif()

enable_testing()

include(FetchContent)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/archive)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-DDEBUG)
endif()

include(cmake/fetch_date.cmake)
include(cmake/fetch_fmt.cmake)
include(cmake/fetch_stb.cmake)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT APPLE AND UNIX)
  set(SYSTEM_NAME "linux")
  set(SUNRAY_COMPILE_OPTIONS -g -Wall -Wcast-align -Wconversion -Wfloat-equal -Wformat=2 -Wmissing-declarations -Woverlength-strings -Wshadow -Wunreachable-code -Wextra -Wnon-virtual-dtor -Werror)
elseif(APPLE)
  set(SYSTEM_NAME "macos")
  set(SUNRAY_COMPILE_OPTIONS -g -Wall -Wheader-hygiene -Wcast-align -Wconversion -Wfloat-equal -Wformat=2 -Wmissing-declarations -Woverlength-strings -Wshadow -Wunreachable-code -Wextra -Wpedantic -Wnon-virtual-dtor -Werror -Wno-nested-anon-types -Wno-gnu-anonymous-struct -Wdelete-non-virtual-dtor -Wdeprecated -Wfloat-conversion -Wfloat-overflow-conversion -Wmove -Wredundant-move -Wshadow-all -Wsign-compare -Wsign-conversion -Wunreachable-code)
elseif(WIN32)
  set(SYSTEM_NAME "windows")
  set(SUNRAY_COMPILE_OPTIONS )
else()
  message(FATAL_ERROR "unknown platform")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  message("Setting up SUNRAY ${PROJECT_VERSION} for a debug build")
elseif(CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
  message("Setting up SUNRAY ${PROJECT_VERSION} for a release with debug info build")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
  message("Setting up SUNRAY ${PROJECT_VERSION} for a release build")
else()
  message(FATAL_ERROR "Unknown build type ${CMAKE_BUILD_TYPE} specified")
endif()

if(NOT APPLE AND UNIX)
  message("Setting up for C++17 with libc++.")
elseif(APPLE AND CMAKE_GENERATOR STREQUAL Xcode)
  message("Setting up Xcode for C++17 with libc++.")
elseif(APPLE)
  message("Setting up for C++17 with libc++.")
elseif(WIN32)
  message("Setting up for C++17 with libc++.")
else()
  message(FATAL_ERROR "unknown platform")
endif()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(apps)

