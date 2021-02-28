set(_SRCS
        "http/moth-transport-http.c"
        "http/moth-transport-settings.h")

set(_INCLUDE
        "http")

idf_component_register(SRCS ${_SRCS}
        INCLUDE_DIRS "include" ${_INCLUDE}
        REQUIRES "moth-comm" "esp_http_client")