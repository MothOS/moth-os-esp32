#include "include/moth-comm.h"

//static SemaphoreHandle_t network_semaphore = NULL;
//static network_status_t network_status;

#define MOTH_COMM_LOCK(c) xSemaphoreTake(c, portMAX_DELAY)
#define MOTH_COMM_UNLOCK(c) xSemaphoreGive(c)

//uint8_t moth_comm_init(void) {
//    network_semaphore = xSemaphoreCreateMutex();
//    if (network_semaphore == NULL) {
//        return 1;
//    }
//    network_status = NETWORK_DISCONNECTED;
//    return 0;
//}
//
////
//// Network
////
//
//network_status_t moth_comm_get_network_status(void) {
//    MOTH_COMM_LOCK(network_semaphore);
//    network_status_t status = network_status;
//    MOTH_COMM_UNLOCK(network_semaphore);
//    return status;
//}
//
//void moth_comm_set_network_status(network_status_t status) {
//    MOTH_COMM_LOCK(network_semaphore);
//    network_status = status;
//    MOTH_COMM_UNLOCK(network_semaphore);
//}