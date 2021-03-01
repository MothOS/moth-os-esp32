#include "include/moth-network.h"

#include <string.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"

#include "moth-comm.h"

#define ESP_WIFI_SSID "iot-node"
#define ESP_WIFI_PASS "goodlife"
//#define ESP_WIFI_SSID "Mitm"
//#define ESP_WIFI_PASS "password"
#define ESP_MAXIMUM_RETRY 5

static shared_info_t *shared_info = NULL;

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_DISCONNECTED_BIT BIT1
#define WIFI_FAIL_BIT BIT2
static EventGroupHandle_t s_wifi_event_group;

static const char *TAG = "moth-network";

static int s_retry_num = 0;
static bool network_connected = false;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            break;
        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "WiFi event: WIFI_EVENT_STA_CONNECTED");
            esp_event_post_to(shared_info->moth_event_loop, MOTH_NETWORK_EVENT, MOTH_NETWORK_CONNECTED, NULL, 0, 0);
            xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "WiFi event: WIFI_EVENT_STA_DISCONNECTED");
            esp_event_post_to(shared_info->moth_event_loop, MOTH_NETWORK_EVENT, MOTH_NETWORK_DISCONNECTED, NULL, 0, 0);
            xEventGroupSetBits(s_wifi_event_group, WIFI_DISCONNECTED_BIT);
            if (s_retry_num < ESP_MAXIMUM_RETRY) {
                ESP_LOGI(TAG, "Retry to connect to the AP");
                esp_wifi_connect();
                s_retry_num++;
            } else {
                xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            }
            break;
        case IP_EVENT_STA_GOT_IP: {
            ESP_LOGI(TAG, "IP event: IP_EVENT_STA_GOT_IP");
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            ESP_LOGI(TAG, "Got ip:" IPSTR, IP2STR(&event->ip_info.ip));
            break;
        }
        case IP_EVENT_STA_LOST_IP:
            ESP_LOGI(TAG, "IP event: IP_EVENT_STA_LOST_IP");
            break;
        default:
            break;
    }
}

void wifi_init_sta(void) {
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(NULL, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .sta =
            {
                .ssid = ESP_WIFI_SSID,
                .password = ESP_WIFI_PASS,
                .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                .pmf_cfg = {.capable = true, .required = false},
            },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi STA initialized");
}

uint8_t wifi_connect(void) {
    ESP_LOGI(TAG, "Connecting to AP SSID:%s password:%s", ESP_WIFI_SSID, ESP_WIFI_PASS);
    s_retry_num = 0;
    esp_wifi_connect();

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() */
    EventBits_t bits =
        xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected");
        network_connected = true;
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        xEventGroupClearBits(s_wifi_event_group, WIFI_DISCONNECTED_BIT);
        xEventGroupClearBits(s_wifi_event_group, WIFI_FAIL_BIT);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect");
        return 1;
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    return 0;
}

_Noreturn void moth_network_task(void *pvParameters) {
    shared_info = (shared_info_t *)pvParameters;
    if (shared_info->moth_event_loop == NULL) {
        ESP_LOGE(TAG, "BAD");
    }

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
    wifi_connect();

    const TickType_t period = 1000 / portTICK_PERIOD_MS;
    const TickType_t reconnect_delay = 10000 / portTICK_PERIOD_MS;
    while (1) {
        EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_DISCONNECTED_BIT, pdFALSE, pdFALSE, 0);

        if (network_connected) {
            if (bits & WIFI_DISCONNECTED_BIT) {
                xEventGroupClearBits(s_wifi_event_group, WIFI_DISCONNECTED_BIT);
                network_connected = false;
            }
        } else {
            // Cannot connect, wait a bit longer and try again.
            if (wifi_connect() != 0) {
                vTaskDelay(reconnect_delay);
                continue;
            }
        }

        vTaskDelay(period);
    }
}
