#include "include/moth-monitor.h"

#include <component_interface.h>
#include <esp_log.h>

static const char *TAG = "moth-monitor";

static shared_info_t *shared_info = NULL;

_Noreturn void moth_monitor_task(void *pvParameters) {
    shared_info = (shared_info_t *)pvParameters;
    if (shared_info->event_group == NULL) {
        ESP_LOGE(TAG, "Cannot start moth_monitor_task, events group");
    }

    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
    while (1) {
        // Wait for the wifi to be connected
        EventBits_t bits = xEventGroupWaitBits(shared_info->event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE,
                                               pdFALSE, 0);
        if (bits & WIFI_CONNECTED_BIT) {
            ESP_LOGI(TAG, "Sending monitoring data to server...");
        } else{
            ESP_LOGW(TAG, "WiFi not connected, skipping");
        }

        vTaskDelay(xDelay);
    }
}