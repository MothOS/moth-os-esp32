#ifndef MOTH_OS_ESP32_MOTH_COMM_H
#define MOTH_OS_ESP32_MOTH_COMM_H

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/semphr.h>

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
typedef struct {
    EventGroupHandle_t event_group;
} shared_info_t;

uint8_t moth_comm_init(void);

//
// Network
//

typedef enum {
    NETWORK_DISCONNECTED,
    NETWORK_CONNECTED,
} network_status_t;

network_status_t moth_comm_get_network_status(void);
void moth_comm_set_network_status(network_status_t status);

#endif  // MOTH_OS_ESP32_MOTH_COMM_H
