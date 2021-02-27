#ifndef MOTH_OS_ESP32_MOTH_TRANSPORT_H
#define MOTH_OS_ESP32_MOTH_TRANSPORT_H

#include <stdint.h>
#include <stddef.h>
#include "moth-transport-settings.h"

typedef struct moth_transport_settings moth_transport_settings_t;

uint8_t moth_transport_init(moth_transport_settings_t *_settings);
uint8_t moth_transport_send_post_message(char* endpoint, char *msg, size_t msg_len);

#endif  // MOTH_OS_ESP32_MOTH_TRANSPORT_H
