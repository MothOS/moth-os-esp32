#include "include/moth-monitor.h"

#include <esp_log.h>
#include <esp_system.h>
#include <string.h>
#include "moth-comm.h"
#include "moth-transport.h"

static const char *TAG = "moth-monitor";

static shared_info_t *shared_info = NULL;

_Noreturn void moth_monitor_task(void *pvParameters) {
    shared_info = (shared_info_t *)pvParameters;
    if (shared_info->event_group == NULL) {
        ESP_LOGE(TAG, "Cannot start moth_monitor_task, events group");
    }

    // Initialize the transport layer
    moth_transport_settings_t settings;
    settings.protocol = strdup("http");
    settings.host = strdup("192.168.254.254");
    settings.port = 9090;

    moth_transport_init(&settings);

    const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
    while (1) {
        // Wait for the wifi to be connected
        network_status_t network_status = moth_comm_get_network_status();
        if (network_status == NETWORK_CONNECTED) {
            uint32_t free_heap = esp_get_free_heap_size();
            uint8_t base_mac_addr[6] = {0};
            char *mac_addr = calloc(13, sizeof(char));
            esp_efuse_mac_get_default(base_mac_addr);
            snprintf(mac_addr, 13, "%X%X%X%X%X%X", base_mac_addr[0], base_mac_addr[1], base_mac_addr[2],
                     base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);
            char msg[256];
            sprintf(msg, "{\"mac_address\":\"%s\",\"free_heap\":%u}", mac_addr, free_heap);
            free(mac_addr);
            ESP_LOGI(TAG, "Sending monitoring data to server: %s", msg);

            moth_transport_send_post_message("monitor", msg, strlen(msg));
        } else {
            ESP_LOGW(TAG, "WiFi not connected, skipping");
        }

        vTaskDelay(xDelay);
    }
}