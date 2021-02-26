#ifndef MOTH_OS_ESP32_COMPONENT_INTERFACE_H
#define MOTH_OS_ESP32_COMPONENT_INTERFACE_H

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
typedef struct {
    EventGroupHandle_t event_group;
} shared_info_t;

#endif  // MOTH_OS_ESP32_COMPONENT_INTERFACE_H
