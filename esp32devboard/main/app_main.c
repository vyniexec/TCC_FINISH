#include <stdio.h>
#include "includes.h"
#include "geral.h"
#include <stdlib.h>
#include "mqtt.h"

int bolean = 0;

void esteiraOutput() {
    while (true) {
        uint8_t readInputs = 0;
        readInputs = io_le_escreve(0x00);
        char entrada[10];
        char comp[10];
        sprintf(entrada, "%d", readInputs);
        
        if (readInputs != 0) esp_mqtt_client_publish(client, "cfp127/TT3B/I00/esp", entrada, 0, 2, 0);
        for (int i = 0; i < 9; i++) {
            if (infomInt[jandira] == 1) {
                bolean = 1;
                int test = 1 << jandira;
                readInputs = io_le_escreve(test);
                while (infomInt[jandira] == 1) {
                    readInputs = io_le_escreve(test);
                    sprintf(entrada, "%d", readInputs);
                    esp_mqtt_client_publish(client, "cfp127/TT3B/I00/esp", entrada, 0, 2, 0);
                    vTaskDelay(10);
                }
            }
        }
        vTaskDelay(10);
    }
}

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    connect_wifi();
    mqtt_app_start();
    devboardInit();
    xTaskCreate(esteiraOutput, "LOGIC", 1024 * 2, NULL, 5, NULL);
    ESP_LOGI(TAG, "Iniciando processos!!!");
    vTaskDelay(10);
}
