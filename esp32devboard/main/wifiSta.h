#ifndef __WIFISTA__H__
#define __WIFISTA__H__
#include "includes.h"

/* == Conectando a REDE == */
#define NAME_WIFI "IoT_AP"
#define PASSWORD_WIFI "12345678"
#define MAXIMUM_RETRY 5
static const char *TAG = "@vynizinho";
/* == Sinalizar se a conexão foi estabelecida ou não == */
static EventGroupHandle_t s_wifi_event_group;

/* == Definindo WIFI CONNECTED como BIT0 == */
#define WIFI_CONNECTED_BIT BIT0 

/* == Definindo WIFI FAIL como BIT1 == */
#define WIFI_FAIL_BIT BIT1

static int s_retry_num = 0;

/* == Iiniciando a função para o ESP se conectar com o WIFI definido em Kconfig.projbuild == */
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "static ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void connect_wifi(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        sta_netif,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        sta_netif,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = NAME_WIFI,
            .password = PASSWORD_WIFI,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Função wifi finalizada!!!");

    /* == Aguardando até que conexão seja estabelecida (WIFI_CONNECTED_BIT) ou
    falhe pelo numero de tentativas(WIFI_FAIL_BIT). O numero de tentativas são definidas acima por event_handler() == */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    /* == Testa qual evento realmente ocorreu pelo "bits" == */
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "Conectado ao SSID:%s password:%s",
                 NAME_WIFI, PASSWORD_WIFI);
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "A conexão ao SSID:%s, password:%s falhou!!!",
                 NAME_WIFI, PASSWORD_WIFI);
    }
    else
    {
        ESP_LOGE(TAG, "Evento inesperado!!!");
    }
    vEventGroupDelete(s_wifi_event_group);
}
#endif  //!__WIFI__H__