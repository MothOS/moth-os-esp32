#include "moth-transport.h"

#include <stdio.h>

#include <esp_http_client.h>
#include <esp_log.h>

static const char *TAG = "moth-transport-http";

moth_transport_settings_t *settings = NULL;

uint8_t moth_transport_init(moth_transport_settings_t *_settings) {
    // TODO: Check settings
    if (settings != NULL) {
        free(settings);
        settings = NULL;
    }
    settings = _settings;
    return 0;
}

uint8_t moth_transport_send_post_message(char* endpoint, char *msg, size_t msg_len) {
    char *url = calloc(256, sizeof(char));
    snprintf(url, 256, "%s://%s:%hu/%s", settings->protocol, settings->host, settings->port, endpoint);

    esp_http_client_config_t config = {.url = url};

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, msg, msg_len);
    esp_err_t err = esp_http_client_perform(client);

    free(url);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Status = %d, content_length = %d", esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    esp_http_client_cleanup(client);

    return 0;
}

uint8_t moth_transport_testeable(){
  return 0;
}