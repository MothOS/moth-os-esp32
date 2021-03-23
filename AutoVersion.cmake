# References:
# https://github.com/mpb27/cmake-auto-version
#

### Git ###
find_package(Git)
if (GIT_FOUND)
    message(STATUS "Found Git")
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            OUTPUT_VARIABLE GIT_REVISION
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Current Git commit hash: " ${GIT_REVISION})

    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags
            OUTPUT_VARIABLE GIT_TAG
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "Current Git tag: " ${GIT_TAG})
endif ()

##
# Version format: #define VERSION "<major>.<minor>.<build>-<revision>"
# If the tag is not defined then the version will be set to 0.0.0
##
if (NOT ${GIT_TAG} STREQUAL "")
    string(REGEX MATCH "^v([0-9]+)\\.([0-9]+)\\.([0-9]+)$" MY_MATCHES "${GIT_TAG}")
    set(V_MAJOR ${CMAKE_MATCH_1})
    set(V_MINOR ${CMAKE_MATCH_2})
    set(V_BUILD ${CMAKE_MATCH_3})
else ()
    set(V_MAJOR 0)
    set(V_MINOR 0)
    set(V_BUILD 0)
endif ()

if (NOT ${GIT_REVISION} STREQUAL "")
    set(V_REV ${GIT_REVISION})
else ()
    set(V_REV 0000000)
endif ()

message(STATUS "Major: ${V_MAJOR}   Minor: ${V_MINOR}   Build: ${V_BUILD}    Revision: ${V_REV}")

# Create the version file
string(TOUPPER ${_PROJECT_NAME} _PROJECT_NAME)
string(REPLACE "-" "_" _PROJECT_NAME ${_PROJECT_NAME})
set(VERSION_FILE_HEADER "${_PROJECT_NAME}_VERSION_H")
file(WRITE ${_VERSION_FILE} "//This file is automatically created. DO NOT EDIT.

#ifndef ${VERSION_FILE_HEADER}
#define ${VERSION_FILE_HEADER}

#define V_MAJOR ${V_MAJOR}
#define V_MINOR ${V_MINOR}
#define V_BUILD ${V_BUILD}
#define V_REV   \"${V_REV}\"

#define MOTH_VERSION \"${V_MAJOR}.${V_MINOR}.${V_BUILD}-${V_REV}\"

#endif //${VERSION_FILE_HEADER}
")

# References:
#    - get GIT hash : https://stackoverflow.com/questions/949314/
#


