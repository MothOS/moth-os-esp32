# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(_project_name "moth-os-esp32")

# References:
# https://github.com/mpb27/cmake-auto-version
#

# Automatically create a header file which contains the version
execute_process(
        COMMAND ${CMAKE_COMMAND}
        -D _VERSION_FILE=${CMAKE_BINARY_DIR}/version/moth-version.h
        -D _PROJECT_NAME=${_project_name}
        -P ${CMAKE_SOURCE_DIR}/AutoVersion.cmake
)
include_directories(${CMAKE_BINARY_DIR}/version)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(${_project_name})
