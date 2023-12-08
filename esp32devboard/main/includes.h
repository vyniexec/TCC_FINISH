#ifndef __INCLUDES__H__
#define __INCLUDES__H__

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "mqtt_client.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

// -- Nomeando as saídas e entradas do ESP32 --;
// -- Definindo os pinos do registrador de saídas digitais --;
#define IO_SH       GPIO_NUM_14                 // -- Pino do registrador de deslocamento (74HC595) saída - SERIAL IN --;
#define IO_CLK      GPIO_NUM_12                 // -- Pino do registrador de deslocamento (74HC595) saída - CLOCK --;
#define IO_WR       GPIO_NUM_27                 // -- Pino do registrador de deslocamento (74HC595) saída - LATCH --;
#define IO_DT_RD    GPIO_NUM_13                 // -- Pino do registrador de deslocamento (74HC595) saída - LATCH --;

#endif  //!__INCLUDES__H__