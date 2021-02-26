#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <nvs_flash.h>
#include <string.h>

#include "component_interface.h"
#include "moth-network.h"
#include "moth-monitor.h"

static shared_info_t *shared_info;

void nvs_init(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void app_main(void) {
    nvs_init();

    shared_info = malloc(sizeof(shared_info_t));
    shared_info->event_group = xEventGroupCreate();

    xTaskCreate(moth_network_task, "NETWORKING", 5000, (void *)shared_info, 2, NULL);
    xTaskCreate(moth_monitor_task, "MONITORIG", 5000, (void *)shared_info, 10, NULL);
}
