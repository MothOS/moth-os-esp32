#ifndef MOTH_OS_ESP32_MOTH_COMM_H
#define MOTH_OS_ESP32_MOTH_COMM_H


#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
//#include <freertos/semphr.h>
#include <esp_event.h>

typedef struct {
    esp_event_loop_handle_t moth_event_loop;
} shared_info_t;

ESP_EVENT_DECLARE_BASE(MOTH_NETWORK_EVENT);  // declaration of the task events family
enum { MOTH_NETWORK_CONNECTED, MOTH_NETWORK_DISCONNECTED };

//uint8_t moth_comm_init(void);
//
////
//// Network
////
//typedef enum {
//    NETWORK_DISCONNECTED,
//    NETWORK_CONNECTED,
//} network_status_t;
//
//network_status_t moth_comm_get_network_status(void);
//void moth_comm_set_network_status(network_status_t status);

#endif  // MOTH_OS_ESP32_MOTH_COMM_H
