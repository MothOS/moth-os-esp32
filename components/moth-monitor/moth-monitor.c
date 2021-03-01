#include "include/moth-monitor.h"

#include <esp_log.h>
#include <esp_system.h>
#include <string.h>
#include "moth-comm.h"
#include "moth-transport.h"

static const char *TAG = "moth-monitor";

static shared_info_t *shared_info = NULL;
static bool network_connected = false;

static void moth_event_handler(void *args, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == MOTH_NETWORK_EVENT) {
        switch (event_id) {
            case MOTH_NETWORK_CONNECTED:
                network_connected = true;
                break;
            case MOTH_NETWORK_DISCONNECTED:
                network_connected = false;
                break;
            default:
                break;
        }
    }
}

static void send_monitornig_data(void) {
    uint32_t free_heap = esp_get_free_heap_size();
    uint8_t base_mac_addr[6] = {0};
    char *mac_addr = calloc(13, sizeof(char));
    esp_efuse_mac_get_default(base_mac_addr);
    snprintf(mac_addr, 13, "%X%X%X%X%X%X", base_mac_addr[0], base_mac_addr[1], base_mac_addr[2], base_mac_addr[3],
             base_mac_addr[4], base_mac_addr[5]);
    char msg[256];
    sprintf(msg, "{\"mac_address\":\"%s\",\"free_heap\":%u}", mac_addr, free_heap);
    free(mac_addr);
    ESP_LOGI(TAG, "Sending monitoring data to server: %s", msg);

    moth_transport_send_post_message("monitor", msg, strlen(msg));
}

_Noreturn void moth_monitor_task(void *pvParameters) {
    shared_info = (shared_info_t *)pvParameters;
    if (shared_info->moth_event_loop == NULL) {
        ESP_LOGE(TAG, "BAD");
    }

    // Subscribe to network events
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(shared_info->moth_event_loop, MOTH_NETWORK_EVENT,
                                                             ESP_EVENT_ANY_ID, &moth_event_handler, NULL, NULL));

    // Initialize the transport layer
    moth_transport_settings_t settings;
    settings.protocol = strdup("http");
    settings.host = strdup("192.168.254.254");
    settings.port = 9090;

    moth_transport_init(&settings);

    const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
    while (1) {
        // Wait for the wifi to be connected
        if (network_connected) {
            send_monitornig_data();
        } else {
            ESP_LOGW(TAG, "WiFi not connected, skipping");
        }

        vTaskDelay(xDelay);
    }
}