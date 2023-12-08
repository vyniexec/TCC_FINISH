#ifndef __MQTT__H__
#define __MQTT__H__

#include "esp_log.h"
#include "includes.h"
#include "geral.h"
#include <stdio.h>
#include <string.h>

int jandira = 0;

/* == Criando a TAG1 de sinalização == */
static const char *TAG1 = "MQTTZINHO";
uint32_t MQTT_CONNEECTED = 0;

/* == Chamando a função MQTT == */
static void mqtt_app_start(void);
char buffer1[16];
int infomInt[10];
char *recived1;
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG1, "Desapacho da base do loop de eventos=%s, evento=%d", base, (int)event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    switch ((esp_mqtt_event_id_t)event_id)
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG1, "MQTT CONECTADO COM SUCESSO ");
            MQTT_CONNEECTED=1;
            esp_mqtt_client_subscribe(client, "cfp127/TT3B", 2);
            esp_mqtt_client_subscribe(client, "cfp127/TT3B/I00", 2);
            esp_mqtt_client_subscribe(client, "cfp127/TT3B/I01", 2);
            esp_mqtt_client_subscribe(client, "cfp127/TT3B/I02", 2);
            esp_mqtt_client_subscribe(client, "cfp127/TT3B/I03", 2);
            esp_mqtt_client_subscribe(client, "cfp127/TT3B/I04", 2);
            esp_mqtt_client_subscribe(client, "cfp127/TT3B/I05", 2);
            esp_mqtt_client_subscribe(client, "cfp127/TT3B/I06", 2);
            ESP_LOGI(TAG1, "Mensagem recebida com sucesso, msg_id=%d", event->msg_id);
        break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG1, "MQTT DESCONECTADO");
            MQTT_CONNEECTED=0;
        break;
        case MQTT_EVENT_SUBSCRIBED:
            //ESP_LOGI(TAG1, "MENSAGEM RECEBIDA, msg_id = %d", event->msg_id);
        break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG1, "Evento cancelado, msg_id = %d", event->msg_id);
        break;
        case MQTT_EVENT_PUBLISHED:
            //ESP_LOGI(TAG1, "Mensagem publicada, msg_id = %d", event->msg_id);
        break;
        case MQTT_EVENT_DATA:
            char topic[] = "cfp127/TT3B/I";
            char *topicMod;
            topicMod = event->topic;
            sprintf(buffer1, "%s", topicMod);
            int comprimento = strlen(buffer1);
            if (comprimento >= 2) {
                int indiceUltimoCaractere = comprimento - 1;
                buffer1[indiceUltimoCaractere] = '\0';
            }
            for (int i = 0; i < 9; i++)
            {
                char topicCompair[20];
                sprintf(topicCompair, "%s%02d", topic, i);
                if (strcmp(buffer1, topicCompair) == 0)
                {   
                    infomInt[i] = atoi(event->data);
                    jandira = i;
                }
            }
        break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG1, "MQTT ERRO!!!");
        break;
        default:
            ESP_LOGI(TAG1, "Outro evento: %d", event->event_id);
        break;
    }
}

esp_mqtt_client_handle_t client = NULL;
static void mqtt_app_start(void)
{
    ESP_LOGI(TAG1, "Iniciando MQTT");
    esp_mqtt_client_config_t mqttConfig = {
        .broker.address.uri = "mqtt://192.168.0.101",
        .broker.address.port = 1883,
        .credentials.username = "tecT3B", 
        .credentials.authentication.password = "0z9y8x7w", 
        .credentials.client_id = "esteira_clp"
    };
    client = esp_mqtt_client_init(&mqttConfig);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}
#endif  //!__MQTT__H__