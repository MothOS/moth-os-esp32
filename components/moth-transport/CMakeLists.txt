set(_SRCS
        "http/moth-transport-http.c"
        "http/moth-transport-settings.h")

set(_INCLUDE
        "include"
        "http")

message(STATUS "RUNNING moth-transport component CMakeLists")

if (UNIT_TESTING)
    if (${COMPONENT_NAME} IN_LIST TEST_COMPONENTS)
        set(mock_headers "")
        foreach (include_dir ${_INCLUDE})
            file(GLOB_RECURSE tmp_headers "${include_dir}/*.h")
            LIST(APPEND mock_headers "${tmp_headers}")
        endforeach ()
        string(REPLACE ";" " " mock_headers_str "${mock_headers}")

        message(STATUS "Component directory ${COMPONENT_DIR}")
        execute_process(
                WORKING_DIRECTORY ${COMPONENT_DIR}
                COMMAND ${CMAKE_COMMAND} -E env "UNITY_DIR=${IDF_PATH}/components/unity/unity" ruby ${CMOCK_DIR}/lib/cmock.rb ${mock_headers_str}
        )

        LIST(APPEND _INCLUDE "mocks")
        file(GLOB _SRCS "mocks/*.c")
    endif ()
endif ()

idf_component_register(SRCS ${_SRCS}
        INCLUDE_DIRS ${_INCLUDE}
        REQUIRES "moth-comm" "esp_http_client")