#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <nvs_flash.h>
#include <string.h>

#include "moth-comm.h"
#include "moth-monitor.h"
#include "moth-network.h"

static shared_info_t *shared_info;

/* Event source task related definitions */
ESP_EVENT_DEFINE_BASE(MOTH_NETWORK_EVENT);

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

    // Initialize the event loop
    esp_event_loop_args_t moth_event_loop_args = {
        .queue_size = 5,
        .task_name = "moth_loop_task", // task will be created
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 2048,
        .task_core_id = tskNO_AFFINITY
    };
    ESP_ERROR_CHECK(esp_event_loop_create(&moth_event_loop_args, &shared_info->moth_event_loop));

//    if (moth_comm_init() != 0) {
//        abort();
//    }

    xTaskCreate(moth_network_task, "NETWORKING", 5000, (void *)shared_info, 2, NULL);
    xTaskCreate(moth_monitor_task, "MONITORIG", 5000, (void *)shared_info, 10, NULL);
}
