project(DesktopJodel)

cmake_minimum_required(VERSION 3.0.2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE debug CACHE STRING "Choose the type of build, options are: debug, release" FORCE)
endif()

set(CMAKE_CONFIGURATION_TYPES "debug;release")
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS          "-Wall -Werror -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG    "-O0 -ggdb")
endif()

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUI ON)

find_package(X11 REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${PROJECT_BINARY_DIR})

add_subdirectory(src)
