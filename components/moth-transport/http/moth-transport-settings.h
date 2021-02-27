#ifndef MOTH_OS_ESP32_MOTH_TRANSPORT_SETTINGS_H
#define MOTH_OS_ESP32_MOTH_TRANSPORT_SETTINGS_H

struct moth_transport_settings {
    char *protocol;
    char *host;
    uint16_t port;
};

#endif  // MOTH_OS_ESP32_MOTH_TRANSPORT_SETTINGS_H
